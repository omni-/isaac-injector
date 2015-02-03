using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OML
{
    public abstract class Pill
    {
        public Pill(bool isGood)
        {
            this.isGood = isGood;
        }

        public bool isGood;
        public abstract void OnUse(Player player);
    }
}
