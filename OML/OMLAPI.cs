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
            int rawsize = Marshal.SizeOf(typeof(T));
            IntPtr buffer = Marshal.AllocHGlobal(rawsize);
            Marshal.Copy(rawData, 0, buffer, rawsize);
            T retobj = (T)Marshal.PtrToStructure(buffer, typeof(T));
            Marshal.FreeHGlobal(buffer);
            return retobj;
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
            request.id = OML.APICALL_END;

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
            request.id = OML.APICALL_HPUP;
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
            request.id = OML.APICALL_GETKEYS;
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
            request.id = OML.APICALL_SETKEYS;
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
            request.id = OML.APICALL_GETBOMBS;
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
            request.id = OML.APICALL_SETBOMBS;
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
            request.id = OML.APICALL_GETCOINS;
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
            request.id = OML.APICALL_SETCOINS;
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
            request.id = OML.APICALL_GETSTAT;
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
            request.id = OML.APICALL_SETSTAT;
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
}
