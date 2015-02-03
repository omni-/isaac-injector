using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OML
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

        public int Coins
        {
            get { return new API_GetCoinsCall(OML.Connection, Handle).Call(); }

            set { new API_SetCoinsCall(OML.Connection, Handle, value).Call();  }
        }

        public void HpUp(int amount)
        {
            new API_HpUpCall(OML.Connection, Handle, amount).Call();
        }

        public void HpDown(int amount)
        {
            new API_HpUpCall(OML.Connection, Handle, -amount).Call();
        }

        public void GetStat(PlayerStat stat)
        {
            new API_GetStatCall(OML.Connection, Handle, stat).Call();
        }

        public void SetStat(PlayerStat stat, int amount)
        {
            new API_SetStatCall(OML.Connection, Handle, amount, stat).Call();
        }
    }
    public class API
    {
        public static Entity SpawnItem(int itemID, float x = 0, float y = 0)
        {
            IntPtr entityHandle = new API_SpawnEntityCall(OML.Connection, 5, 100, itemID, x, y, IntPtr.Zero).Call();
            return new Entity(entityHandle);
        }
        public static Entity SpawnEntity(int entityID, int variant, int subtype, float x, float y, IntPtr parentHandle)
        {
            IntPtr entityHandle = new API_SpawnEntityCall(OML.Connection, entityID, variant, subtype, x, y, parentHandle).Call();
            return new Entity(entityHandle);
        }

        public static void GotoFloor(Floor floor)
        {
            new API_GotoFloorCall(OML.Connection, (uint)floor).Call();
        }
    }
}
