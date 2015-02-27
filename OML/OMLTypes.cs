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
    public enum ItemType
    {
        passive = 1,
        trinket,
        active, 
        familiar
    }
    public enum ItemPool
    {
        treasure,
        shop,
        boss,
        devil, 
        angel,
        secret,
        library,
        goldenChest,
        redChest,
        beggar,
        demonBeggar,
        curse,
        keyMaster,
        bossrush,
        dungeon
    }
    public abstract class Item
    {
        public string Name;
        public string PickupText;
        public ItemType Type;
        public ItemPool Pool;
        public int Weight;
        public float DecreaseBy;
        public float RemoveOn;
        public int id;
        /// <summary>
        /// 1 for special 0 for not
        /// </summary>
        public int Special;
        public int Recharge;
        public int dmgbuff;
        public int speedbuff;
        public int rangebuff;
        public float tearbuff;
        public int luckbuff;
        public int shotspeedbuff;
        public string cache;
        public string ResDir;
        public string AnimName;
        public string CostumeName;
        public OMLResource resource;

        public virtual void OnTearHit(Entity target)
        {
        }
        public virtual void OnEnemyDeath(Entity enemy)
        {
        }
        public virtual void OnShootTear(Entity tear)
        {
        }
        public virtual void OnPlayerMove(Player p)
        {
        }
        public virtual void OnEnemyContact(/*int a1, int a2, float damage, int a4, Entity sourceEntity,*/ Player player)
        {
        }
        public virtual void OnPlayerAddCollectible(Player player, int itemID, int charges, int a4)
        {
        }
        public virtual void OnEntitySpawn(PointF velocity, PointF position, int entityID, int variant, int subtype, Entity parent)
        {
        }
        public virtual void OnPlayerCardUse(Player player, int cardID, ref bool handled)
        {
        }
        public virtual void OnPlayerPillUse(Player player, int pillID, ref bool handled)
        {
            Pill p = PillDictionary.GetPill(pillID);

            if (p != null)
            {
                p.OnUse(player);
                handled = true;
            }

            else
                handled = false;
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
        public virtual void OnGameUpdate()
        {
        }
        public virtual void OnGotoFloor(Floor nextFloor)
        {
        }
        public virtual void OnPlayerUpdate(Player player)
        {
        }
        public virtual void OnPickup(Player player)
        {
            new API_AddCostumeCall(_OML.Connection, player.Handle, "resources/gfx/characters/" + AnimName).Call();
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
                return null;
            IntPtr entityHandle = new API_SpawnEntityCall(_OML.Connection, 5, 100, itemID, NormalizePointF(player.Position).x, NormalizePointF(player.Position).y - 1, IntPtr.Zero).Call();
            return new Entity(entityHandle);
        }
        public static Entity SpawnItem(Player player, string itemname)
        {
            int itemID = _OML.GetItemID(itemname);
            if (itemID == 0)
                return null;
            IntPtr entityHandle = new API_SpawnEntityCall(_OML.Connection, 5, 100, itemID, NormalizePointF(player.Position).x, NormalizePointF(player.Position).y - 1, IntPtr.Zero).Call();
            return new Entity(entityHandle);
        }
        public static Entity SpawnEntity(int entityID, int variant, int subtype, float x, float y)
        {
            IntPtr entityHandle = new API_SpawnEntityCall(_OML.Connection, entityID, variant, subtype, x, y, IntPtr.Zero).Call();
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
        //public static void AddCustomItem(int id)
        //{
            //new API_AddCustomItemCall(_OML.Connection, id).Call();
        //}
    }
}
