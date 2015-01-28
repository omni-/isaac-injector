using System;
using System.Diagnostics;
using System.IO;
using System.IO.Pipes;
using System.Runtime.InteropServices;
using System.Runtime.Serialization.Formatters.Binary;
using System.Text;
using System.Threading;

namespace OML
{
    internal class Handler
    {
        public void Handle(Process proc)
        {
            try
            {
                //init server, mutex, and streams
                var server = new NamedPipeServerStream("omlpipe", PipeDirection.InOut, NamedPipeServerStream.MaxAllowedServerInstances, PipeTransmissionMode.Byte, PipeOptions.Asynchronous, 32768, 32768);
                var mutex = new Mutex(false, "omlmutex");
                var read = new BinaryReader(server);
                var write = new BinaryWriter(server);
                var formatter = new BinaryFormatter();

                //load plugins
                Console.WriteLine("\r\n[INFO] loading plugins...");
                var plugins = Loader.GetPlugins();
                foreach (OMLPlugin p in plugins)
                    Console.Write("\r\nloading {0}-v{1} by {2} ... load successful.", p.PluginName, p.PluginVersion, p.PluginAuthor);
                Console.WriteLine("\r\n\r\n[INFO] plugin load completed.");

                //wait for connection
                Console.Write("\r\n[INFO] waiting for connection...");
                server.WaitForConnection();
                Console.WriteLine("successful connection to injected dll.");

                // Peek named pipe arguments
                byte[] buffer = new byte[1];
                uint bytesRead = 0;
                uint bytesAvail = 0;
                uint bytesLeft = 0;

                while (!proc.HasExited)
                {
                    if (mutex.WaitOne())
                    {
                        if (NativeMethods.PeekNamedPipe(server.SafePipeHandle, buffer, 1, ref bytesRead, ref bytesAvail, ref bytesLeft) && (bytesRead > 0))
                        {
                            Player player;
                            int _event = BitConverter.ToInt32(read.ReadBytes(sizeof(int)), 0);
                            switch (_event)
                            {
                                case OML.PLAYER_EVENT_TAKEPILL: //0
                                    if (Program.verbose)
                                        Console.WriteLine("\r\n[INFO] take pill event received.");
                                    player = RawDeserialize<Player>(read.ReadBytes(Marshal.SizeOf(typeof(Player))), 0);
                                    int pillid = BitConverter.ToInt32(read.ReadBytes(sizeof(int)), 0);
                                    foreach (OMLPlugin p in plugins)
                                        p.OnPlayerPillUse(ref player, ref pillid);
                                    server.Flush();
                                    write.Write(OML.PLAYER_EVENT_TAKEPILL);
                                    write.Write(RawSerialize(player));
                                    if (Program.verbose)
                                        Console.WriteLine("\r\n[INFO] take pill event sent.");
                                    break;
                                case OML.PLAYER_EVENT_ADDCOLLECTIBLE: //1
                                    if (Program.verbose)
                                        Console.WriteLine("\r\n[INFO] add collectible event received.");
                                    player = RawDeserialize<Player>(read.ReadBytes(Marshal.SizeOf(typeof(Player))), 0);
                                    int a2 = BitConverter.ToInt32(read.ReadBytes(sizeof(int)), 0);
                                    int id = BitConverter.ToInt32(read.ReadBytes(sizeof(int)), 0);
                                    int a4 = BitConverter.ToInt32(read.ReadBytes(sizeof(int)), 0);

                                    foreach (OMLPlugin p in plugins)
                                        p.OnPlayerAddCollectible(ref player, ref a2, ref id, ref a4);

                                    player._damage = 1000.0f;

                                    server.Flush();

                                    write.Write(OML.PLAYER_EVENT_ADDCOLLECTIBLE);
                                    write.Write(RawSerialize(player));
                                    write.Write(a2);
                                    write.Write(id);
                                    write.Write(a4);
                                    if (Program.verbose)
                                        Console.WriteLine("\r\n[INFO] add collectibile event sent.");
                                    break;
                                case OML.GAME_EVENT_SPAWNENTITY: //2
                                    if (Program.verbose)
                                        Console.WriteLine("\r\n[INFO] spawnentity event received.");
                                    PointF velocity = RawDeserialize<PointF>(read.ReadBytes(Marshal.SizeOf(typeof(PointF))), 0);
                                    PointF position = RawDeserialize<PointF>(read.ReadBytes(Marshal.SizeOf(typeof(PointF))), 0);
                                    int gameManager = BitConverter.ToInt32(read.ReadBytes(sizeof(int)), 0);
                                    int entityid = BitConverter.ToInt32(read.ReadBytes(sizeof(int)), 0);
                                    int variant = BitConverter.ToInt32(read.ReadBytes(sizeof(int)), 0);
                                    Entity parent = RawDeserialize<Entity>(read.ReadBytes(Marshal.SizeOf(typeof(Entity))), 0);
                                    int subtype = BitConverter.ToInt32(read.ReadBytes(sizeof(int)), 0);
                                    int seed = BitConverter.ToInt32(read.ReadBytes(sizeof(int)), 0);

                                    foreach (OMLPlugin p in plugins)
                                        p.OnEntitySpawn(ref velocity, ref velocity, ref gameManager, ref entityid, ref variant, ref parent, ref subtype, ref seed);

                                    write.Write(OML.GAME_EVENT_SPAWNENTITY);
                                    write.Write(RawSerialize(velocity));
                                    write.Write(RawSerialize(position));
                                    write.Write(gameManager);
                                    write.Write(entityid);
                                    write.Write(variant);
                                    write.Write(RawSerialize(parent));
                                    write.Write(subtype);
                                    write.Write(seed);
                                    if (Program.verbose)
                                        Console.WriteLine("\r\n[INFO] spawn entity event sent.");
                                    break;
                                case OML.PLAYER_EVENT_HPUP: //3
                                    if (Program.verbose)
                                        Console.WriteLine("\r\n[INFO] hp up event received.");
                                    player = RawDeserialize<Player>(read.ReadBytes(Marshal.SizeOf(typeof(Player))), 0);
                                    int hpamount = BitConverter.ToInt32(read.ReadBytes(sizeof(int)), 0);

                                    foreach (OMLPlugin p in plugins)
                                        p.OnPlayerHealthUp(ref player, ref hpamount);

                                    write.Write(OML.PLAYER_EVENT_HPUP);
                                    write.Write(RawSerialize(player));
                                    write.Write(hpamount);
                                    if (Program.verbose)
                                        Console.WriteLine("\r\n[INFO] hp up event sent.");
                                    break;
                                case OML.PLAYER_EVENT_HPDOWN: //4
                                    if (Program.verbose)
                                        Console.WriteLine("\r\n[INFO] hp down event received.");
                                    player = RawDeserialize<Player>(read.ReadBytes(Marshal.SizeOf(typeof(Player))), 0);
                                    int hmamount = BitConverter.ToInt32(read.ReadBytes(sizeof(int)), 0);

                                    foreach (OMLPlugin p in plugins)
                                        p.OnPlayerHealthDown(ref player, ref hmamount);

                                    write.Write(OML.PLAYER_EVENT_HPDOWN);
                                    write.Write(RawSerialize(player));
                                    write.Write(hmamount);
                                    if (Program.verbose)
                                        Console.WriteLine("\r\n[INFO] hp down event sent.");
                                    break;
                                case OML.PLAYER_EVENT_ADDSOULHEARTS: //5
                                    if (Program.verbose)
                                        Console.WriteLine("\r\n[INFO] add soul hearts event received");
                                    player = RawDeserialize<Player>(read.ReadBytes(Marshal.SizeOf(typeof(Player))), 0);
                                    int shamount = BitConverter.ToInt32(read.ReadBytes(sizeof(int)), 0);

                                    foreach (OMLPlugin p in plugins)
                                        p.OnSoulHeartsAdded(ref player, ref shamount);

                                    write.Write(OML.PLAYER_EVENT_ADDSOULHEARTS);
                                    write.Write(RawSerialize(player));
                                    write.Write(shamount);
                                    if (Program.verbose)
                                        Console.WriteLine("\r\n[INFO] add soul hearts event sent");
                                    break;
                                case OML.ENEMY_EVENT_SHOOTTEARS: //6
                                    break;
                                case OML.GAME_EVENT_CHANGEROOM: //7
                                    break;
                                default:
                                    Console.WriteLine("[WARN] unknown packet recieved. event not implemented?");
                                    break;
                            }
                            if (server.IsConnected)
                                server.Flush();
                        }

                        mutex.ReleaseMutex();
                    }
                }
            }
            catch (IOException)
            {
                Console.WriteLine("\r\n[ERROR] pipe error occured.");
                return;
            }
            catch (AbandonedMutexException)
            {
                Console.WriteLine("\r\n[ERROR] mutex abandoned. process crashed?");
                return;
            }
            catch (Exception ex)
            {
                Console.WriteLine("\r\n[ERROR] fatal error.");
                Console.WriteLine(ex.Message);
                Console.WriteLine(ex.Source);
                Console.WriteLine(ex.StackTrace);
                throw;
            }
            finally
            {
            }
        }
        static byte[] GetBytes(string str, int len)
        {
            byte[] bytes = new byte[len];
            Buffer.BlockCopy(str.ToCharArray(), 0, bytes, 0, bytes.Length);
            return bytes;
        }
        public static T RawDeserialize<T>(byte[] rawData, int position)
        {
            int rawsize = Marshal.SizeOf(typeof(T));
            if (rawsize > rawData.Length - position)
                throw new ArgumentException("Not enough data to fill struct. Array length from position: " + (rawData.Length - position) + ", Struct length: " + rawsize);
            IntPtr buffer = Marshal.AllocHGlobal(rawsize);
            Marshal.Copy(rawData, position, buffer, rawsize);
            T retobj = (T)Marshal.PtrToStructure(buffer, typeof(T));
            Marshal.FreeHGlobal(buffer);
            return retobj;
        }
        public static byte[] RawSerialize(object anything)
        {
            int rawSize = Marshal.SizeOf(anything);
            IntPtr buffer = Marshal.AllocHGlobal(rawSize);
            Marshal.StructureToPtr(anything, buffer, false);
            byte[] rawDatas = new byte[rawSize];
            Marshal.Copy(buffer, rawDatas, 0, rawSize);
            Marshal.FreeHGlobal(buffer);
            return rawDatas;
        }
    }
}
