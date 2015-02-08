using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OML
{
    public class Wrappers
    {
        public static void SetStat_Wrapper(object[] _params)
        {
            ((Player)_params[0]).SetStat((PlayerStat)_params[1], (int)_params[2]);
        }
        public static void SpawnItem_Wrapper(object[] _params)
        {
            if (_params[1].GetType() == typeof(int))
                API.SpawnItem((Player)_params[0], (int)_params[1]);
            else if (_params[1].GetType() == typeof(string))
                API.SpawnItem((Player)_params[0], (string)_params[1]);
        }
        public static void SpawnEntity_Wrapper(object[] _params)
        {
            API.SpawnEntity((int)_params[0], (int)_params[1], (int)_params[2], (float)_params[3], (float)_params[4], (IntPtr)_params[5]);
        }
        public static void Teleport_Wrapper(object[] _params)
        {
            API.Teleport((int)_params[0]);
        }
        public static void JumpFloor_Wrapper(object[] _params)
        {
            API.GotoFloor((Floor)_params[0]);
        }
        public static void AddCollectible_Wrapper(object[] _params)
        {
            if (_params[1].GetType() == typeof(int))
                ((Player)_params[0]).AddCollectible((int)_params[1]);
            else if (_params[1].GetType() == typeof(string))
                ((Player)_params[0]).AddCollectible((string)_params[1]);
        }
        public static void SetInv_Wrapper(object[] _params)
        {
            switch((PlayerInv)_params[1])
            {
                case PlayerInv.Coins:
                    ((Player)_params[0]).Coins = (int)_params[2];
                    break;
                case PlayerInv.Bombs:
                    ((Player)_params[0]).Bombs = (int)_params[2];
                    break;
                case PlayerInv.Keys:
                    ((Player)_params[0]).Keys = (int)_params[2];
                    break;
            }
        }
        public static void SetPlayerPosition_Wrapper(object[] _params)
        {
            ((Player)_params[0]).Position = new PointF { x = (float)_params[1], y = (float)_params[2] };
        }
    }
}
