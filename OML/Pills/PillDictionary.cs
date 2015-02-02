using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OML.Pills
{
    public static class PillDictionary
    {
        protected static List<Pill> PillDictionary = new List<Pill>();
        protected static int StartIndex = 28;

        public static int AddPill(Pill p)
        {
            //TODO: Actually add it to REAL isaac
            PillDictionary.Add(p);
            return StartIndex + PillDictionary.Count;
        }

        public static Pill GetPill(int id)
        {
            int idx = id - StartIndex;

            if (idx < 0 || idx >= PillDictionary.Count)
                return null;

            return PillDictionary[idx];
        }
    }
}
