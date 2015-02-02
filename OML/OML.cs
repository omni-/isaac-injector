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
        public virtual void OnPlayerCardUse(Player player, int cardID, ref bool handled)
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
        public virtual void OnGameUpdate()
        {
        }
        public virtual void OnPlayerUpdate(Player player)
        {
        }

    }

    public static class OML
    {
        public static API_ConnectionInfo Connection = null;

        public const int PLAYER_EVENT_TAKEPILL       = 0x00;
        public const int PLAYER_EVENT_ADDCOLLECTIBLE = 0x01;
        public const int GAME_EVENT_SPAWNENTITY      = 0x02;
        public const int PLAYER_EVENT_HPUP           = 0x03;
        public const int PLAYER_EVENT_HPDOWN         = 0x04;
        public const int PLAYER_EVENT_ADDSOULHEARTS  = 0x05;
        public const int ENEMY_EVENT_SHOOTTEARS      = 0x06;
        public const int PLAYER_EVENT_CHANGEROOM     = 0x07;
        public const int GAME_EVENT_UPDATE           = 0x08;
        public const int PLAYER_EVENT_UPDATE         = 0x09;
        public const int PLAYER_EVENT_USECARD        = 0x0A;


        public const uint APICALL_NULL               = 0x00;

        public const uint APICALL_HPUP               = 0x01;
        public const uint APICALL_ADDSOULHEARTS      = 0x02;
        public const uint APICALL_GETKEYS            = 0x03;
        public const uint APICALL_SETKEYS            = 0x04;
        public const uint APICALL_GETBOMBS           = 0x05;
        public const uint APICALL_SETBOMBS           = 0x06;
        public const uint APICALL_GETCOINS           = 0x07;
        public const uint APICALL_SETCOINS           = 0x08;
        public const uint APICALL_GETTRINKET         = 0x09;
        public const uint APICALL_SETTRINKET         = 0x0A;
        public const uint APICALL_ADDCOLLECTIBLE     = 0x0B;
        public const uint APICALL_HASITEM            = 0x0C;
        public const uint APICALL_GETPOSITION        = 0x0D;
        public const uint APICALL_SETPOSITION        = 0x0E;
        public const uint APICALL_TELEPORT           = 0x0F;
        public const uint APICALL_GETSTAT            = 0x10;
        public const uint APICALL_SETSTAT            = 0x11;

        public const uint APICALL_END                = 0xFFFFFFFF;

        public static Player GetPlayer(IntPtr handle)
        {
            return new Player(handle);
        }
    }
    
    public enum PlayerStat
    {
        Speed,
        Range,
        FireRate,
        ShotSpeed,
        Damage,
        Luck
    }
}


