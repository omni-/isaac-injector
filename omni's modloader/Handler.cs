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
            var server = new NamedPipeServerStream("omlpipe", PipeDirection.InOut, NamedPipeServerStream.MaxAllowedServerInstances,
                PipeTransmissionMode.Byte, PipeOptions.Asynchronous);
            var mutex = new Mutex(false, "omlmutex");
            var read = new BinaryReader(server);
            var formatter = new BinaryFormatter();
            var plugins = Loader.GetPlugins();
            Console.Write("\r\n[INFO]Waiting for connection...");
            server.WaitForConnection();
            Console.WriteLine("Successful connection to injected dll.");

            while (!proc.HasExited)
            {
                mutex.WaitOne(5000);

                string input = Encoding.ASCII.GetString(read.ReadBytes(32)).Replace("\0", "");
                const string sOnPlayerAddCollectible = "OnPlayerAddCollectible";

                switch (input)
                {
                    case sOnPlayerAddCollectible:
                        Player player = RawDeserialize<Player>(read.ReadBytes(Marshal.SizeOf(typeof(Player))), 0);
                        int a2 = BitConverter.ToInt32(read.ReadBytes(sizeof(int)), 0);
                        int id = BitConverter.ToInt32(read.ReadBytes(sizeof(int)), 0);
                        int a4 = BitConverter.ToInt32(read.ReadBytes(sizeof(int)), 0);
                        int a5 = BitConverter.ToInt32(read.ReadBytes(sizeof(int)), 0);

                        foreach (OMLPlugin p in plugins)
                            p.OnPlayerAddCollectible(ref player, ref a2, ref id, ref a4, ref a5);
                        player._keys = 5;

                        server.Flush();
                        var write = new BinaryWriter(server);

                        byte[] unibytes = Encoding.Unicode.GetBytes(sOnPlayerAddCollectible);
                        byte[] asciibytes = Encoding.Convert(Encoding.Unicode, Encoding.ASCII, unibytes);
                        byte[] dest = new byte[32];
                        Array.Copy(asciibytes, dest, asciibytes.Length);
                        write.Write(dest);
                        write.Write(RawSerialize(player));
                        write.Write(a2);
                        write.Write(id);
                        write.Write(a4);
                        write.Write(a5);
                        mutex.ReleaseMutex();
                        break;
                    case "OnEntitySpawn":
                        break;
                    case "OnPlayerPillCardUse":
                        break;
                    case "OnPlayerHealthModify":
                        break;
                    case "OnPlayerAddCollectibleEnd":
                        break;
                    case "OnEntitySpawnEnd":
                        break;
                    case "OnPlayerPillCardUseEnd":
                        break;
                    case "OnPlayerHealthModifyEnd":
                        break;
                    default:
                        break;
                }
                if (server.IsConnected)
                    server.Flush();
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
