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
            //init server, mutex, and streams
            using (var server = new NamedPipeServerStream("omlpipe", PipeDirection.InOut, NamedPipeServerStream.MaxAllowedServerInstances, PipeTransmissionMode.Message, PipeOptions.WriteThrough, 32768, 32768))
            {
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
                byte[] eventID = new byte[1];
                uint bytesRead = 0;
                uint bytesAvail = 0;
                uint bytesLeft = 0;

                while (!proc.HasExited)
                {
                    try
                    {
                        if (NativeMethods.PeekNamedPipe(server.SafePipeHandle, eventID, 1, ref bytesRead, ref bytesAvail, ref bytesLeft) && (bytesRead > 0))
                        {
                            switch (eventID[0])
                            {
                                case OML.PLAYER_EVENT_TAKEPILL:
                                    if (bytesLeft + 1 == TakePillEvent_Notification.size())
                                    {
                                        // Receive event
                                        if (Program.verbose) Console.WriteLine("\r\n[INFO] PLAYER_EVENT_TAKEPILL received.");

                                        TakePillEvent_Notification notification = RawDeserialize<TakePillEvent_Notification>(read.ReadBytes(TakePillEvent_Notification.size()), 0);
                                        
                                        Player player = new Player(notification.playerHandle);
                                        bool handled = true;
                                        foreach (OMLPlugin p in plugins)
                                            p.OnPlayerPillUse(player, notification.pillID, ref handled);

                                        server.Flush();

                                        // Send response
                                        TakePillEvent_Response response;
                                        response.eventID = OML.PLAYER_EVENT_TAKEPILL;
                                        response.handled = handled;

                                        write.Write(RawSerialize(response));

                                        if (Program.verbose) Console.WriteLine("\r\n[INFO] PLAYER_EVENT_TAKEPILL response sent.");
                                    }
                                    else
                                    { 
                                        if (Program.verbose)
                                            Console.WriteLine("\r\n[WARNING] PLAYER_EVENT_TAKEPILL: expected " + TakePillEvent_Notification.size().ToString() + " bytes, received: " + (bytesLeft+1).ToString() + " bytes");
                                    }
                                    break;

                                case OML.PLAYER_EVENT_ADDCOLLECTIBLE:
                                    if (bytesLeft + 1 == AddCollectibleEvent_Notification.size())
                                    {
                                        // Receive event
                                        if (Program.verbose) Console.WriteLine("\r\n[INFO] PLAYER_EVENT_ADDCOLLECTIBLE received.");

                                        AddCollectibleEvent_Notification notification = RawDeserialize<AddCollectibleEvent_Notification>(read.ReadBytes(AddCollectibleEvent_Notification.size()), 0);

                                        Player player = new Player(notification.playerHandle);
                                        foreach (OMLPlugin p in plugins)
                                            p.OnPlayerAddCollectible(player, notification.a2, notification.itemID, notification.a4);

                                        server.Flush();

                                        // Send response
                                        AddCollectibleEvent_Response response;
                                        response.eventID = OML.PLAYER_EVENT_ADDCOLLECTIBLE;

                                        write.Write(RawSerialize(response));

                                        if (Program.verbose) Console.WriteLine("\r\n[INFO] PLAYER_EVENT_ADDCOLLECTIBLE response sent.");
                                    }
                                    else
                                    {
                                        if (Program.verbose)
                                            Console.WriteLine("\r\n[WARNING] PLAYER_EVENT_ADDCOLLECTIBLE: expected " + AddCollectibleEvent_Notification.size().ToString() + " bytes, received: " + (bytesLeft + 1).ToString() + " bytes");
                                    }
                                    break;

                                case OML.GAME_EVENT_SPAWNENTITY:
                                    if (bytesLeft + 1 == SpawnEntityEvent_Notification.size())
                                    {
                                        // Receive event
                                        if (Program.verbose) Console.WriteLine("\r\n[INFO] GAME_EVENT_SPAWNENTITY received.");

                                        SpawnEntityEvent_Notification notification = RawDeserialize<SpawnEntityEvent_Notification>(read.ReadBytes(SpawnEntityEvent_Notification.size()), 0);

                                        Entity parent = new Entity(notification.parentHandle);
                                        foreach (OMLPlugin p in plugins)
                                            p.OnEntitySpawn(notification.velocity, notification.position, notification.entityID, notification.variant, notification.subtype, parent);

                                        server.Flush();

                                        // Send response
                                        SpawnEntityEvent_Response response;
                                        response.eventID = OML.GAME_EVENT_SPAWNENTITY;

                                        write.Write(RawSerialize(response));

                                        if (Program.verbose) Console.WriteLine("\r\n[INFO] GAME_EVENT_SPAWNENTITY response sent.");
                                    }
                                    else
                                    {
                                        if (Program.verbose)
                                            Console.WriteLine("\r\n[WARNING] GAME_EVENT_SPAWNENTITY: expected " + SpawnEntityEvent_Notification.size().ToString() + " bytes, received: " + (bytesLeft + 1).ToString() + " bytes");
                                    }
                                    break;

                                case OML.PLAYER_EVENT_HPUP:
                                    if (bytesLeft + 1 == HpUpEvent_Notification.size())
                                    {
                                        // Receive event
                                        if (Program.verbose) Console.WriteLine("\r\n[INFO] PLAYER_EVENT_HPUP received.");

                                        HpUpEvent_Notification notification = RawDeserialize<HpUpEvent_Notification>(read.ReadBytes(HpUpEvent_Notification.size()), 0);

                                        Player player = new Player(notification.playerHandle);
                                        int tmpAmount = notification.amount;
                                        foreach (OMLPlugin p in plugins)
                                            p.OnPlayerHealthUp(player, ref tmpAmount);

                                        server.Flush();

                                        // Send response
                                        HpUpEvent_Response response;
                                        response.eventID = OML.PLAYER_EVENT_HPUP;
                                        response.amount = tmpAmount;

                                        write.Write(RawSerialize(response));

                                        if (Program.verbose) Console.WriteLine("\r\n[INFO] PLAYER_EVENT_HPUP response sent.");
                                    }
                                    else
                                    {
                                        if (Program.verbose)
                                            Console.WriteLine("\r\n[WARNING] PLAYER_EVENT_HPUP: expected " + HpUpEvent_Notification.size().ToString() + " bytes, received: " + (bytesLeft + 1).ToString() + " bytes");
                                    }
                                    break;

                                case OML.PLAYER_EVENT_HPDOWN:
                                    if (bytesLeft + 1 == HpDownEvent_Notification.size())
                                    {
                                        // Receive event
                                        if (Program.verbose) Console.WriteLine("\r\n[INFO] PLAYER_EVENT_HPDOWN received.");

                                        HpDownEvent_Notification notification = RawDeserialize<HpDownEvent_Notification>(read.ReadBytes(HpDownEvent_Notification.size()), 0);

                                        Player player = new Player(notification.playerHandle);
                                        int tmpAmount = notification.amount;
                                        foreach (OMLPlugin p in plugins)
                                            p.OnPlayerHealthDown(player, ref tmpAmount);

                                        server.Flush();

                                        // Send response
                                        HpUpEvent_Response response;
                                        response.eventID = OML.PLAYER_EVENT_HPDOWN;
                                        response.amount = tmpAmount;

                                        write.Write(RawSerialize(response));

                                        if (Program.verbose) Console.WriteLine("\r\n[INFO] PLAYER_EVENT_HPDOWN response sent.");
                                    }
                                    else
                                    {
                                        if (Program.verbose)
                                            Console.WriteLine("\r\n[WARNING] PLAYER_EVENT_HPDOWN: expected " + HpDownEvent_Notification.size().ToString() + " bytes, received: " + (bytesLeft + 1).ToString() + " bytes");
                                    }
                                    break;

                                case OML.PLAYER_EVENT_ADDSOULHEARTS:
                                    if (bytesLeft + 1 == AddSoulHeartsEvent_Notification.size())
                                    {
                                        // Receive event
                                        if (Program.verbose) Console.WriteLine("\r\n[INFO] PLAYER_EVENT_ADDSOULHEARTS received.");

                                        AddSoulHeartsEvent_Notification notification = RawDeserialize<AddSoulHeartsEvent_Notification>(read.ReadBytes(AddSoulHeartsEvent_Notification.size()), 0);

                                        Player player = new Player(notification.playerHandle);
                                        int tmpAmount = notification.amount;
                                        foreach (OMLPlugin p in plugins)
                                            p.OnSoulHeartsAdded(player, ref tmpAmount);

                                        server.Flush();

                                        // Send response
                                        AddSoulHeartsEvent_Response response;
                                        response.eventID = OML.PLAYER_EVENT_ADDSOULHEARTS;
                                        response.amount = tmpAmount;

                                        write.Write(RawSerialize(response));

                                        if (Program.verbose) Console.WriteLine("\r\n[INFO] PLAYER_EVENT_ADDSOULHEARTS response sent.");
                                    }
                                    else
                                    {
                                        if (Program.verbose)
                                            Console.WriteLine("\r\n[WARNING] PLAYER_EVENT_ADDSOULHEARTS: expected " + AddSoulHeartsEvent_Notification.size().ToString() + " bytes, received: " + (bytesLeft + 1).ToString() + " bytes");
                                    }
                                    break;

                                case OML.ENEMY_EVENT_SHOOTTEARS:
                                    if (bytesLeft + 1 == ShootTearsEvent_Notification.size())
                                    {
                                        // Receive event
                                        if (Program.verbose) Console.WriteLine("\r\n[INFO] ENEMY_EVENT_SHOOTTEARS received.");

                                        ShootTearsEvent_Notification notification = RawDeserialize<ShootTearsEvent_Notification>(read.ReadBytes(ShootTearsEvent_Notification.size()), 0);

                                        Entity sourceEntity = new Entity(notification.sourceEntityHandle);
                                        foreach (OMLPlugin p in plugins)
                                            p.OnEnemyTearShot(notification.velocity, notification.position, sourceEntity, notification.pattern, notification.tearInfo);

                                        server.Flush();

                                        // Send response
                                        ShootTearsEvent_Response response;
                                        response.eventID = OML.ENEMY_EVENT_SHOOTTEARS;

                                        write.Write(RawSerialize(response));

                                        if (Program.verbose) Console.WriteLine("\r\n[INFO] ENEMY_EVENT_SHOOTTEARS response sent.");
                                    }
                                    else
                                    {
                                        if (Program.verbose)
                                            Console.WriteLine("\r\n[WARNING] ENEMY_EVENT_SHOOTTEARS: expected " + ShootTearsEvent_Notification.size().ToString() + " bytes, received: " + (bytesLeft + 1).ToString() + " bytes");
                                    }
                                    break;

                                case OML.PLAYER_EVENT_CHANGEROOM:
                                    if (bytesLeft + 1 == ChangeRoomEvent_Notification.size())
                                    {
                                        // Receive event
                                        if (Program.verbose) Console.WriteLine("\r\n[INFO] PLAYER_EVENT_CHANGEROOM received.");

                                        ChangeRoomEvent_Notification notification = RawDeserialize<ChangeRoomEvent_Notification>(read.ReadBytes(ChangeRoomEvent_Notification.size()), 0);

                                        foreach (OMLPlugin p in plugins)
                                            p.OnRoomChange(notification.newRoomIndex);

                                        server.Flush();

                                        // Send response
                                        ChangeRoomEvent_Response response;
                                        response.eventID = OML.ENEMY_EVENT_SHOOTTEARS;

                                        write.Write(RawSerialize(response));

                                        if (Program.verbose) Console.WriteLine("\r\n[INFO] PLAYER_EVENT_CHANGEROOM response sent.");
                                    }
                                    else
                                    {
                                        if (Program.verbose)
                                            Console.WriteLine("\r\n[WARNING] PLAYER_EVENT_CHANGEROOM: expected " + ChangeRoomEvent_Notification.size().ToString() + " bytes, received: " + (bytesLeft + 1).ToString() + " bytes");
                                    }
                                    break;
                            }

                            if (server.IsConnected)
                                server.Flush();
                        }
                    }
                    catch (IOException)
                    {
                        Console.WriteLine("\r\n[ERROR] pipe error occured.");
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
                }
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
