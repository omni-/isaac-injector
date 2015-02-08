using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace OML
{
    internal class Loader
    {
        public static bool init()
        {
            if (!Directory.Exists("Plugins"))
                Directory.CreateDirectory("Plugins");
            if (!Directory.Exists("res"))
                Directory.CreateDirectory("res");
            return true;
        }
        public static List<OMLPlugin> GetPlugins()
        {
            string[] dlls = Directory.GetFiles("Plugins", "*.dll").Where(x => x != "Plugins\\OML.dll").ToArray<string>();
            List<Assembly> assemblies = new List<Assembly>();
            foreach (string dll in dlls)
                assemblies.Add(Assembly.LoadFile(Path.GetFullPath(dll)));
            List<OMLPlugin> plugins = new List<OMLPlugin>();
            foreach (Assembly a in assemblies)
                if (a != null)
                    foreach (Type t in a.GetTypes())
                        if (t.IsSubclassOf(typeof(OMLPlugin)))
                            plugins.Add((OMLPlugin)Activator.CreateInstance(t));
            return plugins;
        }
    }
}
