using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace omni_s_modloader
{
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
    struct Entity
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 0x76C)]
        byte[] _unk0;
    }
}
