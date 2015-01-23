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
        public const int PLAYER_EVENT_TAKEPILL = 0x00;
        public const int PLAYER_EVENT_ADDCOLLECTIBLE = 0x01;
        public const int GAME_EVENT_SPAWNENTITY = 0x02;
        public const int PLAYER_EVENT_HPUP = 0x03;
        public const int PLAYER_EVENT_HPDOWN = 0x04;
        public const int PLAYER_EVENT_ADDSOULHEARTS = 0x05;

        public void Handle(Process proc)
        {
            var server = new NamedPipeServerStream("omlpipe", PipeDirection.InOut, NamedPipeServerStream.MaxAllowedServerInstances, PipeTransmissionMode.Byte, PipeOptions.Asynchronous, 32768, 32768);
            var mutex = new Mutex(false, "omlmutex");
            var read = new BinaryReader(server);
            var write = new BinaryWriter(server);
            var formatter = new BinaryFormatter();
            var plugins = Loader.GetPlugins();
            Console.Write("\r\n[INFO]Waiting for connection...");
            server.WaitForConnection();
            Console.WriteLine("Successful connection to injected dll.");
            
            // Peek named pipe arguments
            byte[] buffer = new byte[1];
            uint bytesRead = 0;
            uint bytesAvail = 0;
            uint bytesLeft = 0;

            while (!proc.HasExited)
            {
                if (mutex.WaitOne(50))
                {
                    if (NativeMethods.PeekNamedPipe(server.SafePipeHandle, buffer, 1, ref bytesRead, ref bytesAvail, ref bytesLeft) && (bytesRead > 0))
                    {
                        int _event = BitConverter.ToInt32(read.ReadBytes(sizeof(int)), 0);
                        switch (_event)
                        {
                            case PLAYER_EVENT_TAKEPILL:
                                break;
                            case PLAYER_EVENT_ADDCOLLECTIBLE:
                                Player player = RawDeserialize<Player>(read.ReadBytes(Marshal.SizeOf(typeof(Player))), 0);
                                int a2 = BitConverter.ToInt32(read.ReadBytes(sizeof(int)), 0);
                                int id = BitConverter.ToInt32(read.ReadBytes(sizeof(int)), 0);
                                int a4 = BitConverter.ToInt32(read.ReadBytes(sizeof(int)), 0);

                                foreach (OMLPlugin p in plugins)
                                    p.OnPlayerAddCollectible(ref player, ref a2, ref id, ref a4);
                                player._keys = 5;

                                server.Flush();

                                write.Write(PLAYER_EVENT_ADDCOLLECTIBLE);
                                write.Write(RawSerialize(player));
                                write.Write(a2);
                                write.Write(id);
                                write.Write(a4);
                                break;
                            case 2:
                                break;
                            case 3:
                                break;
                            case 4:
                                break;
                            case 5:
                                break;
                            default:
                                break;
                        }
                        if (server.IsConnected)
                            server.Flush();
                    }

                    mutex.ReleaseMutex();
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
