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
            get { return new API_GetKeysCall(_OML.Connection, Handle).Call(); }

            set { new API_SetKeysCall(_OML.Connection, Handle, value).Call(); }
        }

        public int Bombs
        {
            get { return new API_GetBombsCall(_OML.Connection, Handle).Call(); }

            set { new API_SetBombsCall(_OML.Connection, Handle, value).Call(); }
        }

        public int Coins
        {
            get { return new API_GetCoinsCall(_OML.Connection, Handle).Call(); }

            set { new API_SetCoinsCall(_OML.Connection, Handle, value).Call();  }
        }

        public PointF Position
        {
            get { return new API_GetPositionCall(_OML.Connection, Handle).Call(); }
            set { new API_SetPositionCall(_OML.Connection, Handle, value).Call(); }
        }

        public void HpUp(int amount)
        {
            new API_HpUpCall(_OML.Connection, Handle, amount).Call();
        }

        public void HpDown(int amount)
        {
            new API_HpUpCall(_OML.Connection, Handle, -amount).Call();
        }

        public int GetStat(PlayerStat stat)
        {
            return new API_GetStatCall(_OML.Connection, Handle, stat).Call();
        }

        public void SetStat(PlayerStat stat, int amount)
        {
            new API_SetStatCall(_OML.Connection, Handle, amount, stat).Call();
        }

        public void AddCollectible(int itemid)
        {
            new API_AddCollectibleCall(_OML.Connection, Handle, itemid).Call();
        }

        public void AddCollectible(string itemname)
        {
            int id = _OML.GetItemID(itemname);
            new API_AddCollectibleCall(_OML.Connection, Handle, id).Call();
        }

        public bool HasItem(int itemID)
        {
            return new API_HasItemCall(_OML.Connection, Handle, itemID).Call();
        }

        public bool HasItem(string name)
        {
            int id = _OML.GetItemID(name);
            return new API_HasItemCall(_OML.Connection, Handle, id).Call();
        }
    }
    public class API
    {
        public static Entity SpawnItem(Player player, int itemID)
        {
            if (!_OML.IsValidItemID(itemID))
            {
                Console.WriteLine("\r\n[ERROR] invalid item id");
                return null;
            }
            IntPtr entityHandle = new API_SpawnEntityCall(_OML.Connection, 5, 100, itemID, NormalizePointF(player.Position).x, NormalizePointF(player.Position).y - 1, IntPtr.Zero).Call();
            return new Entity(entityHandle);
        }
        public static Entity SpawnItem(Player player, string itemname)
        {
            int itemID = _OML.GetItemID(itemname);
            if (itemID == -1)
            {
                Console.WriteLine("\r\n[ERROR] invalid item name");
                return null;
            }
            IntPtr entityHandle = new API_SpawnEntityCall(_OML.Connection, 5, 100, itemID, NormalizePointF(player.Position).x, NormalizePointF(player.Position).y - 1, IntPtr.Zero).Call();
            return new Entity(entityHandle);
        }
        public static Entity SpawnEntity(int entityID, int variant, int subtype, float x, float y, IntPtr parentHandle)
        {
            IntPtr entityHandle = new API_SpawnEntityCall(_OML.Connection, entityID, variant, subtype, x, y, parentHandle).Call();
            return new Entity(entityHandle);
        }
        public static void GotoFloor(Floor floor)
        {
            new API_GotoFloorCall(_OML.Connection, (uint)floor).Call();
        }
        public static void Teleport(int roomid)
        {
            new API_TeleportCall(_OML.Connection, roomid).Call();
        }
        public static PointF NormalizePointF(PointF pf)
        {
            PointF result = new PointF();
            result.x = (pf.x - 120) / 40;
            result.y = (pf.y - 160) / 40;
            return result;
        }
    }
}
