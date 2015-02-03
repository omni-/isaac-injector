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
            if (_params.Length >= 3)
                API.SpawnItem((int)_params[0], (float)_params[1], (float)_params[2]);
            else
                API.SpawnItem((int)_params[0]);
        }
        public static void SpawnEntity_Wrapper(object[] _params)
        {
            API.SpawnEntity((int)_params[0], (int)_params[1], (int)_params[2], (float)_params[3], (float)_params[4], (IntPtr)_params[5]);
        }
        public static void Teleport_Wrapper(object[] _params)
        {
            API.Teleport((int)_params[0]);
        }
    }
}
