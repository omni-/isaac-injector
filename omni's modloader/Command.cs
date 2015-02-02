using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace OML
{
    public class Command
    {
        public Action<object[]> callback;
        public string cmdusage;
        public List<Type> typeinfo;

        public Command(Action<object[]> callback, string cmdusage, List<Type> typeinfo)
        {
            this.callback = callback;
            this.cmdusage = cmdusage;
            this.typeinfo = typeinfo;
        }
    }
}
