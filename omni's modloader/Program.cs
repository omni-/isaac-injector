using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace omni_s_modloader
{
    class Program
    {
        public const string version = "v1.0";

        static void Main(string[] args)
        {
            Console.Title = "omni's modloader - " + version;
            Loader l = new Loader();
            Console.Write("[INFO]Initializing omni's modloader {0}...", version);
            bool result = l.init();
            Console.Write(result ? " success" : "\r\n[ERROR]Failed. Exiting.");
            if (!result)
                return;
            Injector i = new Injector();
            Console.WriteLine("\r\n\r\n[INFO]Injection result: " + i.Inject("isaac-ng"));
            Console.ReadKey();
        }
    }
}
