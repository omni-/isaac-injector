using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using OML.API.Types;

namespace OML.API.Events
{
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
