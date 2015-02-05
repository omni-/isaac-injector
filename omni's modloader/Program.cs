using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading;

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

            OML.Init();
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
                h.proc = proc;
                Thread handlerThread = new Thread(new ThreadStart(h.Handle));
                handlerThread.Start();

                while (!handlerThread.IsAlive) ; // Wait for Thread to be started

                // Listen for Player input/commands
                bool canEnterCommands = false;
                bool waitForAbort = false;
                while (handlerThread.IsAlive)
                {
                    if (!canEnterCommands)
                    {
                        canEnterCommands = Interlocked.CompareExchange(ref h.IsWorking, 1, 1) == 1;
                        if (canEnterCommands)
                            Console.WriteLine();
                    }
                    else
                    if (!waitForAbort)
                    {
                        Console.Write("<OML> ");
                        string cmd = Console.ReadLine();

                        if (cmd.Equals("exit"))
                        {
                            Interlocked.Increment(ref h.Abort);
                            waitForAbort = true;
                        }
                        else
                            h.commandQueue.Enqueue(cmd);
                    }
                }
            }
            Console.Write("\r\nprocess exited/error encountered. press any key to exit...");
            Console.ReadKey();
        }
    }
}
