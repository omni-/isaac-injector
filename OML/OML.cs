using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace OML
{
    public class OMLPlugin
    {
        public string PluginName;
        public string PluginVersion;
        public string PluginAuthor;

        public virtual void OnPlayerAddCollectible(ref Player player, ref int a2, ref int id, ref int a4)
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
        public virtual void OnPlayerAddCollectibleEnd()
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
    public class OML
    {
        public void GetPlayer(IntPtr handle, ref Player player)
        {

        }
    }
    [Serializable()]
    [StructLayout(LayoutKind.Sequential, Size = 12384, Pack = 1)]
    public struct Player
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 0x76C)]
        byte[] _unk0;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 0x3E4)]
        byte[] _unk76C;
        public int _maxHearts;
        public int _hearts;
        public int _eternalHearts;
        public int _soulHearts;
        public int _blackHeartMask;
        public int _jarHearts;
        public int _keys;
        public int _hasGoldenKey;
        public int _numBombs;
        public int _numCoins;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 0x24E8)]
        byte[] _unkB78;
    }
}
