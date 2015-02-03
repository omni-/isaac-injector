using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OML
{
    public static class PillDictionary
    {
        private static readonly List<Pill> PillDict = new List<Pill>();

        /// <summary>
        /// Adds a pill to the dictionary
        /// </summary>
        /// <param name="p"></param>
        /// <returns>The id of the new pill</returns>
        public static int AddPill(Pill p)
        {
            // TODO: Actually add it to REAL isaac
            // By this I mean, add it to the drop table.
            // Also I told Cjreek there would be a TODO in here

            PillDict.Add(p);

            // Range: -1 to -infinity
            int pillId = -1 * (PillDict.Count); // In case we need to use this data elsewhere
            return pillId;
        }

        /// <summary>
        /// Gets a pill by id
        /// </summary>
        /// <param name="id"></param>
        /// <returns></returns>
        public static Pill GetPill(int id)
        {
            // Valid id range: -1 to -PillDict.Count

            // Translate to: 0 to -(PillDict.Count - 1)
            int idx = id + 1;

            // idx is now: 0 to PillDict.Count - 1
            idx = idx * -1;

            // Check to ensure it's within bounds, if not it's an invalid pill
            if (idx < 0 || idx >= PillDict.Count)
                return null;

            return PillDict[idx];
        }
    }
}
