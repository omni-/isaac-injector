using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace omni_s_modloader
{
    class Program
    {
        static void Main(string[] args)
        {
            Injector i = new Injector();
            Console.WriteLine(i.Inject("isaac-ng"));
            Console.ReadKey();
        }
    }
}
