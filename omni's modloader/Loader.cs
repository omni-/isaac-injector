using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace omni_s_modloader
{
    public class Loader
    {
        public static bool init()
        {
            if (!Directory.Exists("Plugins"))
                Directory.CreateDirectory("Plugins");
            return true;
        }
        public static List<OMLPlugin> GetPlugins()
        {
            string[] dlls = Directory.GetFiles("Plugins", "*.dll");
            List<Assembly> assemblies = new List<Assembly>();
            foreach (string dll in dlls)
                assemblies.Add(Assembly.LoadFile(dll));
            List<OMLPlugin> plugins = new List<OMLPlugin>();
            foreach (Assembly a in assemblies)
                if (a != null)
                    foreach (Type t in a.GetTypes())
                        plugins.Add((OMLPlugin)Activator.CreateInstance(t));
            return plugins;
        }
    }
}
