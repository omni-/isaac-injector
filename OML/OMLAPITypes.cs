using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace OML
{
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
    public class Command
    {
        public Action<object[]> callback;
        public string cmdusage;
        private List<Type> _typeinfo;

        public List<Type> typeinfo
        {
            get
            {
                return _typeinfo;
            }
            private set { }
        }
        public bool optionalargs;

        public Command(Action<object[]> callback, string cmdusage, List<Type> typeinfo, bool optionalargs)
        {
            this.callback = callback;
            this.cmdusage = cmdusage;
            _typeinfo = typeinfo;
            this.optionalargs = optionalargs;
        }
    }
}
