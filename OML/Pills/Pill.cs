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
            IsGood = isGood;
        }

        public bool IsGood;
        public abstract void OnUse(Player player);
    }
}
