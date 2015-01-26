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
        public virtual void OnEntitySpawn(ref PointF zero, ref PointF position, ref int gameManager, ref int EntityID, ref int Variant, ref int unknown_ptr, ref int subtype, ref uint seed)
        {
        }
        public virtual void OnPlayerPillUse(ref Player player, ref int pillID)
        {
        }
        public virtual void OnPlayerHealthDown(ref Player player, ref int amount)
        {
        }
        public virtual void OnPlayerHealthUp(ref Player player, ref int amount)
        {
        }
        public virtual void OnSoulHeartsAdded(ref Player player, ref int amount)
        {
        }

    }
    public static class OML
    {
        public const int PLAYER_EVENT_TAKEPILL = 0x00;
        public const int PLAYER_EVENT_ADDCOLLECTIBLE = 0x01;
        public const int GAME_EVENT_SPAWNENTITY = 0x02;
        public const int PLAYER_EVENT_HPUP = 0x03;
        public const int PLAYER_EVENT_HPDOWN = 0x04;
        public const int PLAYER_EVENT_ADDSOULHEARTS = 0x05;

        public static Player GetPlayer(IntPtr handle, ref Player player)
        {
            return new Player();
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
    [Serializable()]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct PointF
    {
        float x;
        float y;
    }
}
