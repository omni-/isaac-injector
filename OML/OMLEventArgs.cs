using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OML
{
    public class OnPlayerAddCollectibleEventArgs : EventArgs
    {
        public Player player;
        public int a2;
        public int id;
        public int a4;

        public OnPlayerAddCollectibleEventArgs(Player player, int a2, int id, int a4)
        {
            this.player = player;
            this.a2 = a2;
            this.id = id;
            this.a4 = a4;
        }
    }

    public class OnEntitySpawnEventArgs : EventArgs
    {
        public PointF velocity;
        public PointF position;
        public int entityID;
        public int variant;
        public int subtype;
        public Entity parent;

        public OnEntitySpawnEventArgs(PointF velocity, PointF position, int entityID, int variant, int subtype, Entity parent)
        {
            this.velocity = velocity;
            this.position = position;
            this.entityID = entityID;
            this.variant = variant;
            this.subtype = subtype;
            this.parent = parent;
        }
    }

    public class OnPlayerCardUseEventArgs : EventArgs
    {
        public Player player;
        public int cardID;
        
        public OnPlayerCardUseEventArgs(Player player, int cardID)
        {
            this.player = player;
            this.cardID = cardID;
        }
    }

    public class OnPlayerPillUseEventArgs : EventArgs
    {
        public Player player;
        public int pillID;

        public OnPlayerPillUseEventArgs(Player player, int pillID)
        {
            this.player = player;
            this.pillID = pillID;
        }
    }

    public class OnPlayerHealthModifiedEventArgs : EventArgs
    {
        public Player player;
        public int amount;

        public OnPlayerHealthModifiedEventArgs(Player player, int amount)
        {
            this.player = player;
            this.amount = amount;
        }
    }

    public class OnSoulHeartsAddedEventArgs : EventArgs
    {
        public Player player;
        public int amount;

        public OnSoulHeartsAddedEventArgs(Player player, int amount)
        {
            this.player = player;
            this.amount = amount;
        }
    }

    public class OnRoomChangeEventArgs : EventArgs
    {
        public int newRoom;

        public OnRoomChangeEventArgs(int newRoom)
        {
            this.newRoom = newRoom;
        }
    }

    public class OnEnemyTearShotEventArgs : EventArgs
    {
        public PointF velocity;
        public PointF position;
        public Entity sourceEntity;
        public int pattern;
        public TearInfo tearInfo;

        public OnEnemyTearShotEventArgs(PointF velocity, PointF position, Entity sourceEntity, int pattern, TearInfo tearInfo)
        {
            this.velocity = velocity;
            this.position = position;
            this.sourceEntity = sourceEntity;
            this.pattern = pattern;
            this.tearInfo = tearInfo;
        }
    }

    public class OnGameUpdateEventArgs : EventArgs
    {
    }

    public class OnGotoFloorEventArgs : EventArgs
    {
        public Floor nextFloor;

        public OnGotoFloorEventArgs(Floor nextFloor)
        {
            this.nextFloor = nextFloor;
        }
    }

    public class OnPlayerUpdateEventArgs : EventArgs
    {
        public Player player;
        
        public OnPlayerUpdateEventArgs(Player player)
        {
            this.player = player;
        }
    }
}
