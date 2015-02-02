using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OML.Pills
{
    public static class PillDictionary
    {
        private static readonly List<Pill> PillDict = new List<Pill>();
        private static readonly int StartIndex = 28;

        public static int AddPill(Pill p)
        {
            //TODO: Actually add it to REAL isaac
            PillDict.Add(p);
            return StartIndex + PillDict.Count;
        }

        public static Pill GetPill(int id)
        {
            int idx = id - StartIndex;

            if (idx < 0 || idx >= PillDict.Count)
                return null;

            return PillDict[idx];
        }
    }
}
