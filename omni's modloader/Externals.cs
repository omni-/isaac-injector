using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace omni_s_modloader
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Player
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 0x3E4)]
        char[] _unk76C;
        int _maxHearts;
        int _hearts;
        int _eternalHearts;
        int _soulHearts;
        int _blackHeartMask;
        int _jarHearts;
        int _keys;
        int _hasGoldenKey;
        int _numBombs;
        int _numCoins;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 0x24E8)]
        char[] _unkB78;
    }
}
