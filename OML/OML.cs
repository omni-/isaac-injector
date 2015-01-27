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
        public virtual void OnEntitySpawn(ref PointF zero, ref PointF position, ref int gameManager, ref int EntityID, ref int Variant, ref Entity parent, ref int subtype, ref int seed)
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
        public virtual void OnRoomChange(ref RoomManager roomManeger, ref int roomIDx)
        {
        }
        public virtual void OnTearShot(ref PointF direction, ref PointF startpos, ref Entity mob, ref int type, ref TearInfo tearInfo)
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
        public const int ENEMY_EVENT_SHOOTTEARS = 0x06;
        public const int GAME_EVENT_CHANGEROOM = 0x07;

        public static Player GetPlayer(IntPtr handle, ref Player player)
        {
            return new Player();
        }
    }
    [Serializable()]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct Entity
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 0xC)]
        byte[] _unk0000;

        public int _id;
        public uint _variant;
        public int _subtype;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 0x10)]
        byte[] _unk0018;

        IntPtr Paralysis;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 0x48)]
        byte[] _unk0021;

        public int _tearType;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 0x23C)]
        byte[] _unk0020;

        public float _scaleX;
        public float _scaleY;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 0x88)]
        byte[] _unk000D;

        public float dmg;  // no
        public float dmg2; // no

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 0xC8)]
        byte[] _unk000C;

        public PointF position;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 0x350)]
        byte[] _unk0;
    }

    [Serializable()]
    [StructLayout(LayoutKind.Sequential, Size = 12384, Pack = 1)]
    public struct Player //descends from entity
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 0xC)]
        byte[] _unk0000;

        public int _id;
        public uint _variant;
        public int _subtype;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 0x10)]
        byte[] _unk0018;

        public IntPtr Paralysis;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 0x48)]
        byte[] _unk0021;

        public int _tearType;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 0x23C)]
        byte[] _unk0020;

        public float _scaleX;
        public float _scaleY;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 0x88)]
        byte[] _unk000D;

        public float dmg;  // no
        public float dmg2; // no

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 0xC8)]
        byte[] _unk000C;

        PointF position;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 0x350)]
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
        public float x;
        public float y;
    }

    [Serializable()]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct DebugStruct
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 40)]
        int[] unknown;
    }

    [Serializable()]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct TearInfo
    {
        public float _stuff0;
        public float _stuff4;
        public float _shotheight;
        public float _shotspeed_strange;
        public float _shotspeed;
        public float _damage;
        public float _stuff24;
        public float _stuff28;
        public float _stuff32;
        public float _stuff36;
        public float _tearcolor_red;
        public float _tearcolor_green;
        public float _tearcolor_blue;
        public float _tearcolor_alpha;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 0x0F)]
        float[] unknowns;
        public int _type;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 0x11)]
        float[] unknown2;
    }

    [Serializable()]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct RoomManager
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 0x5D9C)]
        byte[] unknown;
        public uint currRoomIndex;
        public uint previousRoomIndex;
    }

    [Serializable()]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct RoomInfo
    {
        public int unknown1;
        public int type;
        public int variant;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        char[] name;
        public int unknown2;
        public int unknown3;
        public int unknown4;
        public int difficulty;
        // pointer
        public int unknown5;
        public int unknown6;
        //
        public int unknown7;
        // pointer
        public int unknown8;
        public int unknown9;
        // 
        public int unknown10;
        public int unknown11;
    }

    [Serializable()]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct Room
    {
        public RoomInfo info;
        // ?
        public int unknown1;
        // rooms
        public int LeftRoomID;
        public int TopRoomID;
        public int RightRoomID;
        public int BottomRoomID;
        public int BottomLeftRoomID;
        public int TopRightRoomID;
        public int BottomRightRoomID;
        public int BottomRightRoomID2;
        //	?
        public IntPtr unknown2;
        // zero (mostly)
        public int unknown3;
        public int unknown4;
        public int unknown5;
        // pointers
        public DebugStruct unknown6;
        public DebugStruct unknown7;
        public DebugStruct unknown8;
        public DebugStruct unknown9;
        public DebugStruct unknown10;
        public DebugStruct unknown11;
        // zero
        public int unknown12;
        // pointers
        public DebugStruct unknown13;
        public DebugStruct unknown14;
        public DebugStruct unknown15;
        // zero
        public int unknown16;
        public int unknown17;
        // pointers
        public DebugStruct unknown18; // something
        // zero
        public int unknown19;
        public int unknown20;
        // pointers
        public DebugStruct unknown21;
        public DebugStruct unknown22;
        public DebugStruct unknown23;
        // zero
        public int unknown24;
        public int unknown25;
        public int unknown26;
        public int unknown27;
        public int unknown28;
        public int unknown29;
        public int unknown30;
        // -1 x 3
        public int unknown31;
        public int unknown32;
        public int unknown33;
        // index
        public int index;
    }

    [Serializable()]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct PlayerManager
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 0x18)]
        byte[] unknown;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 50)]
        public Room[] rooms; // unknown size.. 50 for now
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 0x3BB8)]
        byte[] unknown2;
        public int RoomCount;
    }
}
