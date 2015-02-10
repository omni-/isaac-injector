using System;
using System.IO;
using System.IO.Pipes;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace OML
{
    public class API_ConnectionInfo
    {
        public NamedPipeClientStream pipe;
        public BinaryReader inStream;
        public BinaryWriter outStream;

        public API_ConnectionInfo(NamedPipeClientStream _pipe, BinaryReader _inStream, BinaryWriter _outStream)
        {
            pipe = _pipe;
            inStream = _inStream;
            outStream = _outStream;
        }
    }

    public class API_BaseCall
    {
        protected byte[] RawSerialize(object anything)
        {
            int rawSize = Marshal.SizeOf(anything);
            IntPtr buffer = Marshal.AllocHGlobal(rawSize);
            Marshal.StructureToPtr(anything, buffer, false);
            byte[] rawDatas = new byte[rawSize];
            Marshal.Copy(buffer, rawDatas, 0, rawSize);
            Marshal.FreeHGlobal(buffer);
            return rawDatas;
        }

        protected T RawDeserialize<T>(byte[] rawData)
        {
            try
            {
                int rawsize = Marshal.SizeOf(typeof(T));
                IntPtr buffer = Marshal.AllocHGlobal(rawsize);
                Marshal.Copy(rawData, 0, buffer, rawsize);
                T retobj = (T)Marshal.PtrToStructure(buffer, typeof(T));
                Marshal.FreeHGlobal(buffer);
                return retobj;
            }
            catch (ArgumentException) { return default(T); }
        }

        protected int SizeOf(Type anything)
        {
            return Marshal.SizeOf(anything);
        }

        protected API_ConnectionInfo connection;

        public API_BaseCall(API_ConnectionInfo _connection)
        {
            connection = _connection;
        }
    }

    public class API_EndCall : API_BaseCall
    {
        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        protected struct EndCall_Request
        {
            public uint id;
        };

        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct EndCall_Response
        {
            public uint id;
        };

        public API_EndCall(API_ConnectionInfo _connection) : base(_connection) { }

        public void Call()
        {
            EndCall_Request request = new EndCall_Request();
            request.id = _OML.APICALL_END;

            connection.outStream.Write(RawSerialize(request));
            EndCall_Response response = RawDeserialize<EndCall_Response>(connection.inStream.ReadBytes(SizeOf(typeof(EndCall_Response))));
        }
    }

    internal class API_HpUpCall : API_BaseCall
    {
        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct HpUp_Request
        {
            public uint id;
            public IntPtr playerHandle;
            public int amount;
        };

        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct HpUp_Response
        {
            public uint id;
        };

        HpUp_Request request;

        public API_HpUpCall(API_ConnectionInfo _connection, IntPtr _player, int _amount) : base(_connection)
        {
            request.id = _OML.APICALL_HPUP;
            request.playerHandle = _player;
            request.amount = _amount;
        }

        public void Call()
        {
            connection.outStream.Write(RawSerialize(request));
            HpUp_Response response = RawDeserialize<HpUp_Response>(connection.inStream.ReadBytes(SizeOf(typeof(HpUp_Response))));
        }
    }

    internal class API_GetKeysCall : API_BaseCall
    {
        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct GetKeys_Request
        {
            public uint id;
            public IntPtr playerHandle;
        };

        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct GetKeys_Response
        {
            public uint id;
            public int amount;
        };

        private GetKeys_Request request;

        public API_GetKeysCall(API_ConnectionInfo _connection, IntPtr _player) : base(_connection)
        {
            request.id = _OML.APICALL_GETKEYS;
            request.playerHandle = _player;
        }

        public int Call()
        {
            connection.outStream.Write(RawSerialize(request));
            GetKeys_Response response = RawDeserialize<GetKeys_Response>(connection.inStream.ReadBytes(SizeOf(typeof(GetKeys_Response))));

            return response.amount;
        }
    }

    internal class API_SetKeysCall : API_BaseCall
    {
        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct SetKeys_Request
        {
            public uint id;
            public IntPtr playerHandle;
            public int amount;
        };

        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct SetKeys_Response
        {
            public uint id;
        };

        private SetKeys_Request request;

        public API_SetKeysCall(API_ConnectionInfo _connection, IntPtr _player, int _amount) : base(_connection)
        {
            request.id = _OML.APICALL_SETKEYS;
            request.playerHandle = _player;
            request.amount = _amount;
        }

        public void Call()
        {
            connection.outStream.Write(RawSerialize(request));
            SetKeys_Response response = RawDeserialize<SetKeys_Response>(connection.inStream.ReadBytes(SizeOf(typeof(SetKeys_Response))));
        }
    }

    internal class API_GetBombsCall : API_BaseCall
    {
        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct GetBombs_Request
        {
            public uint id;
            public IntPtr playerHandle;
        };

        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct GetBombs_Response
        {
            public uint id;
            public int amount;
        };

        private GetBombs_Request request;

        public API_GetBombsCall(API_ConnectionInfo _connection, IntPtr _player) : base(_connection)
        {
            request.id = _OML.APICALL_GETBOMBS;
            request.playerHandle = _player;
        }

        public int Call()
        {
            connection.outStream.Write(RawSerialize(request));
            GetBombs_Response response = RawDeserialize<GetBombs_Response>(connection.inStream.ReadBytes(SizeOf(typeof(GetBombs_Response))));

            return response.amount;
        }
    }

    internal class API_SetBombsCall : API_BaseCall
    {
        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct SetBombs_Request
        {
            public uint id;
            public IntPtr playerHandle;
            public int amount;
        };

        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct SetBombs_Response
        {
            public uint id;
        };

        private SetBombs_Request request;

        public API_SetBombsCall(API_ConnectionInfo _connection, IntPtr _player, int _amount) : base(_connection)
        {
            request.id = _OML.APICALL_SETBOMBS;
            request.playerHandle = _player;
            request.amount = _amount;
        }

        public void Call()
        {
            connection.outStream.Write(RawSerialize(request));
            SetBombs_Response response = RawDeserialize<SetBombs_Response>(connection.inStream.ReadBytes(SizeOf(typeof(SetBombs_Response))));
        }
    }

    internal class API_GetCoinsCall : API_BaseCall
    {
        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct GetCoins_Request
        {
            public uint id;
            public IntPtr playerHandle;
        };

        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct GetCoins_Response
        {
            public uint id;
            public int amount;
        };

        private GetCoins_Request request;

        public API_GetCoinsCall(API_ConnectionInfo _connection, IntPtr _player) : base(_connection)
        {
            request.id = _OML.APICALL_GETCOINS;
            request.playerHandle = _player;
        }

        public int Call()
        {
            connection.outStream.Write(RawSerialize(request));
            GetCoins_Response response = RawDeserialize<GetCoins_Response>(connection.inStream.ReadBytes(SizeOf(typeof(GetCoins_Response))));

            return response.amount;
        }
    }

    internal class API_SetCoinsCall : API_BaseCall
    {
        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct SetCoins_Request
        {
            public uint id;
            public IntPtr playerHandle;
            public int amount;
        };

        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct SetCoins_Response
        {
            public uint id;
        };

        private SetCoins_Request request;

        public API_SetCoinsCall(API_ConnectionInfo _connection, IntPtr _player, int _amount) : base(_connection)
        {
            request.id = _OML.APICALL_SETCOINS;
            request.playerHandle = _player;
            request.amount = _amount;
        }

        public void Call()
        {
            connection.outStream.Write(RawSerialize(request));
            SetCoins_Response response = RawDeserialize<SetCoins_Response>(connection.inStream.ReadBytes(SizeOf(typeof(SetCoins_Response))));
        }
    }

    internal class API_GetStatCall : API_BaseCall
    {
        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct GetStat_Request
        {
            public uint id;
            public IntPtr playerHandle;
            public int stat;
        };

        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct GetStat_Response
        {
            public uint id;
            public int amount;
        };

        private GetStat_Request request;

        public API_GetStatCall(API_ConnectionInfo _connection, IntPtr _player, PlayerStat _stat)
            : base(_connection)
        {
            request.id = _OML.APICALL_GETSTAT;
            request.playerHandle = _player;
            request.stat = (int)_stat;
        }

        public int Call()
        {
            connection.outStream.Write(RawSerialize(request));
            GetStat_Response response = RawDeserialize<GetStat_Response>(connection.inStream.ReadBytes(SizeOf(typeof(GetStat_Response))));

            return response.amount;
        }
    }
    internal class API_SetStatCall : API_BaseCall
    {
        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct SetStat_Request
        {
            public uint id;
            public IntPtr playerHandle;
            public int amount;
            public int stat;
        };

        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct SetStat_Response
        {
            public uint id;
        };

        private SetStat_Request request;

        public API_SetStatCall(API_ConnectionInfo _connection, IntPtr _player, int _amount, PlayerStat _stat)
            : base(_connection)
        {
            request.id = _OML.APICALL_SETSTAT;
            request.playerHandle = _player;
            request.amount = _amount;
            request.stat = (int)_stat;
        }

        public void Call()
        {
            connection.outStream.Write(RawSerialize(request));
            SetStat_Response response = RawDeserialize<SetStat_Response>(connection.inStream.ReadBytes(SizeOf(typeof(SetStat_Response))));
        }
    }
    internal class API_SpawnEntityCall : API_BaseCall
    {
        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct SpawnEntity_Request
        {
            public uint id;
            public int entityID, variant, subtype;
            public float x, y;
            public IntPtr parentHandle;
        };

        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct SpawnEntity_Response
        {
            public uint id;
            public IntPtr entityHandle;
        };

        private SpawnEntity_Request request;

        public API_SpawnEntityCall(API_ConnectionInfo _connection, int _entityID, int _variant, int _subtype, float _x, float _y, IntPtr _parentHandle)
            : base(_connection)
        {
            request.id = _OML.APICALL_SPAWNENTITY;
            request.entityID = _entityID;
            request.variant = _variant;
            request.subtype = _subtype;
            request.x = _x;
            request.y = _y;
            request.parentHandle = _parentHandle;
        }

        public IntPtr Call()
        {
            connection.outStream.Write(RawSerialize(request));
            SpawnEntity_Response response = RawDeserialize<SpawnEntity_Response>(connection.inStream.ReadBytes(SizeOf(typeof(SpawnEntity_Response))));
        
            return response.entityHandle;
        }
    }
    
    internal class API_GotoFloorCall : API_BaseCall
    {
        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct GotoFloor_Request
        {
            public uint id;
            public uint floorNo;
		}
		
		[Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct GotoFloor_Response
        {
			public uint id;
        }
        
        private GotoFloor_Request request;

        public API_GotoFloorCall(API_ConnectionInfo _connection, uint _floorNo) : base(_connection)
        {
            request.id = _OML.APICALL_GOTOFLOOR;
            request.floorNo = _floorNo;
        }
        
        public void Call()
        {
            connection.outStream.Write(RawSerialize(request));
            GotoFloor_Response response = RawDeserialize<GotoFloor_Response>(connection.inStream.ReadBytes(SizeOf(typeof(GotoFloor_Response))));
        }
	}
		
    internal class API_TeleportCall : API_BaseCall
    {
        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct Teleport_Request
        {
            public uint id;
            public int roomid;
        };

        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct Teleport_Response
        {
            public uint id;
        };

        private Teleport_Request request;

        public API_TeleportCall(API_ConnectionInfo _connection, int _roomid) : base(_connection)
        {
            request.id = _OML.APICALL_TELEPORT;
            request.roomid = _roomid;
        }

        public void Call()
        {
            connection.outStream.Write(RawSerialize(request));
            Teleport_Response response = RawDeserialize<Teleport_Response>(connection.inStream.ReadBytes(SizeOf(typeof(Teleport_Response))));
        }
    }
    internal class API_GetPositionCall : API_BaseCall
    {
        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct GetPosition_Request
        {
            public uint id;
            public IntPtr playerHandle;
        };

        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct GetPosition_Response
        {
            public uint id;
            public PointF position;
        };

        private GetPosition_Request request;

        public API_GetPositionCall(API_ConnectionInfo _connection, IntPtr _player)
            : base(_connection)
        {
            request.id = _OML.APICALL_GETPOSITION;
            request.playerHandle = _player;
        }

        public PointF Call()
        {
            connection.outStream.Write(RawSerialize(request));
            GetPosition_Response response = RawDeserialize<GetPosition_Response>(connection.inStream.ReadBytes(SizeOf(typeof(GetPosition_Response))));

            return response.position;
        }
    }

    internal class API_SetPositionCall : API_BaseCall
    {
        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct SetPosition_Request
        {
            public uint id;
            public IntPtr playerHandle;
            public PointF position;
        };

        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct SetPosition_Response
        {
            public uint id;
        };

        private SetPosition_Request request;

        public API_SetPositionCall(API_ConnectionInfo _connection, IntPtr _player, PointF _position)
            : base(_connection)
        {
            request.id = _OML.APICALL_SETPOSITION;
            request.playerHandle = _player;
            request.position = _position;
        }

        public void Call()
        {
            connection.outStream.Write(RawSerialize(request));
            SetPosition_Response response = RawDeserialize<SetPosition_Response>(connection.inStream.ReadBytes(SizeOf(typeof(SetPosition_Response))));
        }
    }
    internal class API_HasItemCall : API_BaseCall
    {
        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct HasItem_Request
        {
            public uint id;
            public IntPtr playerHandle;
            public int itemid;
        };

        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct HasItem_Response
        {
            public uint id;
            public bool hasitem;
        };

        private HasItem_Request request;

        public API_HasItemCall(API_ConnectionInfo _connection, IntPtr _player, int _itemid)
            : base(_connection)
        {
            request.id = _OML.APICALL_HASITEM;
            request.playerHandle = _player;
            request.itemid = _itemid;
        }

        public bool Call()
        {
            connection.outStream.Write(RawSerialize(request));
            return RawDeserialize<HasItem_Response>(connection.inStream.ReadBytes(SizeOf(typeof(HasItem_Response)))).hasitem;
        }
    }
    internal class API_AddCollectibleCall : API_BaseCall
    {
        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct AddCollectible_Request
        {
            public uint id;
            public IntPtr playerHandle;
            public int itemid;
        };

        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct AddCollectible_Response
        {
            public uint id;
        };

        private AddCollectible_Request request;

        public API_AddCollectibleCall(API_ConnectionInfo _connection, IntPtr _player, int _itemid)
            : base(_connection)
        {
            request.id = _OML.APICALL_ADDCOLLECTIBLE;
            request.playerHandle = _player;
            request.itemid = _itemid;
        }

        public void Call()
        {
            connection.outStream.Write(RawSerialize(request));
            AddCollectible_Response response = RawDeserialize<AddCollectible_Response>(connection.inStream.ReadBytes(SizeOf(typeof(AddCollectible_Response))));
        }
    }
    internal class API_GetCustomItemsCall : API_BaseCall
    {
        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct GetCustomItems_Request
        {
            public uint id;
        };

        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct GetCustomItems_Response
        {
            public uint id;
            public int[] ids;
        };

        private GetCustomItems_Request request;

        public API_GetCustomItemsCall(API_ConnectionInfo _connection)
            : base(_connection)
        {
            request.id = _OML.APICALL_GETCUSTOMITEMS;
        }

        public int[] Call()
        {
            connection.outStream.Write(RawSerialize(request));
            return RawDeserialize<GetCustomItems_Response>(connection.inStream.ReadBytes(SizeOf(typeof(GetCustomItems_Response)))).ids;
        }
    }
    internal class API_AddCustomItemCall : API_BaseCall
    {
        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct AddCustomItem_Request
        {
            public uint id;
            public int itemid;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
            public byte[] name;
            public int type;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
            public byte[] resourcename;
        };

        [Serializable()]
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct AddCustomItem_Response
        {
            public uint id;
        };

        private AddCustomItem_Request request;

        public API_AddCustomItemCall(API_ConnectionInfo _connection, int _itemid, string _name, int _type, string _resourcename)
            : base(_connection)
        {
            request.id = _OML.APICALL_ADDCUSTOMITEM;
            request.itemid = _itemid;
            request.type = _type;

            request.name = Encoding.ASCII.GetBytes(_name);
            Array.Resize<byte>(ref request.name, 16);
            for (int i = request.name.Length; i < 16; i++)
                request.name[i] = new byte();

            request.resourcename = Encoding.ASCII.GetBytes(_resourcename);
            Array.Resize<byte>(ref request.resourcename, 32);
            for (int i = request.resourcename.Length; i < 32; i++)
                request.resourcename[i] = new byte();
        }

        public void Call()
        {
            connection.outStream.Write(RawSerialize(request));
            AddCustomItem_Response response = RawDeserialize<AddCustomItem_Response>(connection.inStream.ReadBytes(SizeOf(typeof(AddCustomItem_Response))));
        }
    }
}
