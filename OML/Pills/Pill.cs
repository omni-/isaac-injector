using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OML.Pills
{
    public abstract class Pill
    {
        public abstract void OnUse(Player player);
    }
}
