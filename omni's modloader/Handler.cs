using System;
using System.Diagnostics;
using System.IO;
using System.IO.Pipes;
using System.Runtime.InteropServices;

namespace omni_s_modloader
{
    internal class Handler
    {
        public void Handle(Process proc)
        {
            var server = new NamedPipeServerStream("csserver");
            var client = new NamedPipeClientStream("cppserver");
            var readc = new StreamReader(client);
            var writec = new StreamWriter(client);
            while (!proc.HasExited)
            {
                var plugins = new Loader().GetPlugins();
                string[] input = readc.ReadToEnd().Split('/');
                switch (input[0])
                {
                    case "OnPlayerCollectiblePickup":
                        Player player = (Player)Marshal.PtrToStructure(Marshal.ReadIntPtr(new IntPtr(int.Parse(input[1]))), typeof(Player));
                        foreach(OMLPlugin p in plugins)
                            p.OnPlayerCollectiblePickup(player, int.Parse(input[2]), int.Parse(input[3]), int.Parse(input[4]));
                        break;
                    case "OnEntitySpawn":
                        break;
                    case "OnPlayerPillCardUse":
                        break;
                    case "OnPlayerHealthModify":
                        break;
                    case "PlayerCollectiblePickupCompleted":
                        break;
                    case "EntitySpawnCompleted":
                        break;
                    case "PlayerPillCardUseCompleted":
                        break;
                    case "PlayerHealthModifyCompleted":
                        break;
                    default:
                        break;
                }
                client.Flush();
            }
        }
    }
    public abstract class OMLPlugin
    {
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
        public virtual void OnPlayerCollectiblePickupCompleted()
        {
        }
        public virtual void OnEntitySpawnCompleted()
        {
        }
        public virtual void OnPlayerPillCardUseCompleted()
        {
        }
        public virtual void OnPlayerHealthModifyCompleted()
        {
        }
    }
}
