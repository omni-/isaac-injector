using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OML.API.Calls;

namespace OML.Types
{
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
        public Player(IntPtr handle)
            : base(handle)
        {

        }

        public int Keys
        {
            get { return new API_GetKeysCall(OML.Connection, Handle).Call(); }

            set { new API_SetKeysCall(OML.Connection, Handle, value).Call(); }
        }

        public int Bombs
        {
            get { return new API_GetBombsCall(OML.Connection, Handle).Call(); }

            set { new API_SetBombsCall(OML.Connection, Handle, value).Call(); }
        }

        public void HpUp(int amount)
        {
            new API_HpUpCall(OML.Connection, Handle, amount).Call();
        }

        public void HpDown(int amount)
        {
            new API_HpUpCall(OML.Connection, Handle, -amount).Call();
        }
    }
}
