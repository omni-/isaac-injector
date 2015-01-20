using System;
using System.Diagnostics;
using System.IO;
using System.IO.Pipes;
using System.Runtime.InteropServices;
using System.Text;

namespace omni_s_modloader
{
    internal class Handler
    {
        public void Handle(Process proc)
        {
            var server = new NamedPipeServerStream("omlpipe", PipeDirection.InOut, NamedPipeServerStream.MaxAllowedServerInstances,
                PipeTransmissionMode.Byte, PipeOptions.None);
            var read = new BinaryReader(server);
            var write = new BinaryWriter(server);
            var plugins = Loader.GetPlugins();
            Console.Write("\r\n[INFO]Waiting for connection...");
            server.WaitForConnection();
            Console.WriteLine("[INFO]Successful connection to injected dll.");
            while (!proc.HasExited)
            {
                string input = Encoding.ASCII.GetString(read.ReadBytes(32));
                
                switch (input)
                {
                    case "OnPlayerCollectiblePickup":
                        Player player;
                        player = (Player)rawDeserialize(read.ReadBytes(12384), typeof(Player));
                        foreach(OMLPlugin p in plugins)
                            p.OnPlayerCollectiblePickup(player, BitConverter.ToInt32(read.ReadBytes(sizeof(int)), 0),  BitConverter.ToInt32(read.ReadBytes(sizeof(int)), 0),  BitConverter.ToInt32(read.ReadBytes(sizeof(int)), 0));
                        break;
                    case "OnEntitySpawn":
                        break;
                    case "OnPlayerPillCardUse":
                        break;
                    case "OnPlayerHealthModify":
                        break;
                    case "PlayerCollectiblePickupEnd":
                        break;
                    case "EntitySpawnEnd":
                        break;
                    case "PlayerPillCardUseEnd":
                        break;
                    case "PlayerHealthModifyEnd":
                        break;
                    default:
                        break;
                }
                server.Flush();
            }
        }
        internal static object rawDeserialize(byte[] rawData, Type anyType)
        {
            object retVal = null;

            if (rawData != null && rawData.Length > 0)
            {
                int rawsize = Marshal.SizeOf(anyType);
                if (rawsize > rawData.LongLength)
                    return null;
                IntPtr buffer = Marshal.AllocHGlobal(rawsize);
                Marshal.Copy(rawData, 0, buffer, rawsize);
                retVal = Marshal.PtrToStructure(buffer, anyType);
                Marshal.FreeHGlobal(buffer);
            }

            return retVal;
        }
    }

    public class OMLPlugin
    {
        public static readonly string PluginName;
        public static readonly string PluginVersion;
        public static readonly string Author;

        public virtual void OnPlayerCollectiblePickup(Player player, int a2, int id, int a4)
        {
        }
        public virtual void OnEntitySpawn()
        {
        }
        public virtual void OnPlayerPillCardUse()
        {
        }
        public virtual void OnPlayerHealthModify()
        {
        }
        public virtual void OnPlayerCollectiblePickupEnd()
        {
        }
        public virtual void OnEntitySpawnEnd()
        {
        }
        public virtual void OnPlayerPillCardUseEnd()
        {
        }
        public virtual void OnPlayerHealthModifyEnd()
        {
        }
    }
}
