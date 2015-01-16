using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace omni_s_modloader
{
    public class Loader
    {
        public bool init()
        {
            if (!Directory.Exists("Plugins"))
                Directory.CreateDirectory("Plugins");
            return true;
        }
    }
}
