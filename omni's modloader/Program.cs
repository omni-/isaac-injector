using System;
using System.Collections.Generic;
using System.Diagnostics;

namespace OML
{
    public class Program
    {
        public const string version = "v1.0";

        public static bool verbose = false;

        static void Main(string[] args)
        {
            Console.Title = "omni's modloader - " + version;

            Console.Write("[INFO] initializing omni's modloader {0}...", version);

            //check for verbose switch
            verbose = new List<string>(args).Contains("-v");

            bool result = Loader.init();
            Console.Write(result ? " success" : "\r\n[ERROR] failed. exiting.");
            if (!result) return;

            Injector i = new Injector();
            hResult hresult;
            Process proc = i.Inject("isaac-ng", out hresult);

            Console.WriteLine("\r\n\r\n[INFO] injection result: " + hresult);
            if (hresult != hResult.Error && proc != null)
            {
                Handler h = new Handler();
                h.Handle(proc);
            }
            Console.WriteLine("\r\nprocess exited/error encountered. press any key to exit...");
            Console.ReadKey();
        }
    }
}
