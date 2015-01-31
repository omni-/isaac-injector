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

        public virtual void OnPlayerAddCollectible(Player player, int a2, int id, int a4)
        {
        }
        public virtual void OnEntitySpawn(PointF velocity, PointF position, int entityID, int variant, int subtype, Entity parent)
        {
        }
        public virtual void OnPlayerPillUse(Player player, int pillID, ref bool handled)
        {
        }
        public virtual void OnPlayerHealthDown(Player player, ref int amount)
        {
        }
        public virtual void OnPlayerHealthUp(Player player, ref int amount)
        {
        }
        public virtual void OnSoulHeartsAdded(Player player, ref int amount)
        {
        }
        public virtual void OnRoomChange(int newRoomIndex)
        {
        }
        public virtual void OnEnemyTearShot(PointF velocity, PointF position, Entity sourceEntity, int pattern, TearInfo tearInfo)
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
        public const int PLAYER_EVENT_CHANGEROOM = 0x07;

        public static Player GetPlayer(IntPtr handle, ref Player player)
        {
            return new Player(handle);
        }
    }

    // classes which provide an easy access to modders.
    // they'll have properties/methods which internally call API-functions in the DLL
    public class Entity
    {
        private IntPtr handle = IntPtr.Zero;

        public IntPtr Handle
        {
            get { return handle; }
        }

        public Entity(IntPtr handle)
        {
            this.handle = handle;
        }
    }

    public class Player : Entity
    {
        public Player(IntPtr handle) : base(handle) { }
    }

// Misc
    [Serializable()]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct PointF
    {
        public float x;
        public float y;
    }

    [Serializable()]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct TearInfo
    {
        public float unknown0;
        public float unknown4;
        public float shotheight;
        public float shotspeed_strange;
        public float shotspeed;
        public float damage;
        public float unknown24;
        public float unknown28;
        public float unknown32;
        public float unknown36;
        public float tearcolor_red;
        public float tearcolor_green;
        public float tearcolor_blue;
        public float tearcolor_alpha;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 0x0F)]
        public float[] unknown56;
        public int type;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 0x11)]
        public float[] unknown64;
    }

// TakePillEvent
    [Serializable()]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct TakePillEvent_Notification
    {
        public int eventID;
        public IntPtr playerHandle;
        public int pillID;

        public static int size()
        {
            return Marshal.SizeOf(typeof(TakePillEvent_Notification));
        }
    }

    [Serializable()]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct TakePillEvent_Response
    {
        public int eventID;
        public bool handled;

        public static int size()
        {
            return Marshal.SizeOf(typeof(TakePillEvent_Response));
        }
    }

// AddCollectibleEvent
    [Serializable()]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct AddCollectibleEvent_Notification
    {
        public int eventID;
        public IntPtr playerHandle;
        public int a2;
        public int itemID;
        public int a4;

        public static int size()
        {
            return Marshal.SizeOf(typeof(AddCollectibleEvent_Notification));
        }
    }

    [Serializable()]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct AddCollectibleEvent_Response
    {
        public int eventID;

        public static int size()
        {
            return Marshal.SizeOf(typeof(AddCollectibleEvent_Response));
        }
    }

// SpawnEntityEvent
    [Serializable()]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct SpawnEntityEvent_Notification
    {
        public int eventID;
        public PointF velocity;
        public PointF position;
        public int entityID;
        public int variant;
        public int subtype;
        public IntPtr parentHandle;

        public static int size()
        {
            return Marshal.SizeOf(typeof(SpawnEntityEvent_Notification));
        }
    }

    [Serializable()]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct SpawnEntityEvent_Response
    {
        public int eventID;

        public static int size()
        {
            return Marshal.SizeOf(typeof(SpawnEntityEvent_Response));
        }
    }

// HpUpEvent
    [Serializable()]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct HpUpEvent_Notification
    {
        public int eventID;
        public IntPtr playerHandle;
        public int amount;

        public static int size()
        {
            return Marshal.SizeOf(typeof(HpUpEvent_Notification));
        }
    }

    [Serializable()]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct HpUpEvent_Response
    {
        public int eventID;
        public int amount;

        public static int size()
        {
            return Marshal.SizeOf(typeof(HpUpEvent_Response));
        }
    }

// HpDownEvent
    [Serializable()]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct HpDownEvent_Notification
    {
        public int eventID;
        public IntPtr playerHandle;
        public int amount;

        public static int size()
        {
            return Marshal.SizeOf(typeof(HpDownEvent_Notification));
        }
    }

    [Serializable()]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct HpDownEvent_Response
    {
        public int eventID;
        public int amount;

        public static int size()
        {
            return Marshal.SizeOf(typeof(HpDownEvent_Response));
        }
    }

// AddSoulHeartsEvent
    [Serializable()]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct AddSoulHeartsEvent_Notification
    {
        public int eventID;
        public IntPtr playerHandle;
        public int amount;

        public static int size()
        {
            return Marshal.SizeOf(typeof(AddSoulHeartsEvent_Notification));
        }
    }

    [Serializable()]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct AddSoulHeartsEvent_Response
    {
        public int eventID;
        public int amount;

        public static int size()
        {
            return Marshal.SizeOf(typeof(AddSoulHeartsEvent_Response));
        }
    }

// EnemyShootTearsEvent
    [Serializable()]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct ShootTearsEvent_Notification
    {
        public int eventID;
        public PointF velocity;
        public PointF position;
        public IntPtr sourceEntityHandle;
        public int pattern;
        public TearInfo tearInfo;

        public static int size()
        {
            return Marshal.SizeOf(typeof(ShootTearsEvent_Notification));
        }
    }

    [Serializable()]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct ShootTearsEvent_Response
    {
        public int eventID;

        public static int size()
        {
            return Marshal.SizeOf(typeof(ShootTearsEvent_Response));
        }
    }

// ChangeRoomEvent
    [Serializable()]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct ChangeRoomEvent_Notification
    {
        public int eventID;
        public int newRoomIndex;

        public static int size()
        {
            return Marshal.SizeOf(typeof(ChangeRoomEvent_Notification));
        }
    }

    [Serializable()]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct ChangeRoomEvent_Response
    {
        public int eventID;

        public static int size()
        {
            return Marshal.SizeOf(typeof(ChangeRoomEvent_Response));
        }
    }
}
