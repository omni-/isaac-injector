using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace omni_s_modloader
{
    public class Injector
    {
        // privileges
        const int PROCESS_CREATE_THREAD = 0x0002;
        const int PROCESS_QUERY_INFORMATION = 0x0400;
        const int PROCESS_VM_OPERATION = 0x0008;
        const int PROCESS_VM_WRITE = 0x0020;
        const int PROCESS_VM_READ = 0x0010;

        // used for memory allocation
        const uint MEM_COMMIT = 0x00001000;
        const uint MEM_RESERVE = 0x00002000;
        const uint PAGE_READWRITE = 4;

        public hResult Inject(string procName)
        {
            try
            {
                Process proc = Process.GetProcessesByName(procName)[0];
                IntPtr handle = NativeMethods.OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, false, proc.Id);
                IntPtr loadlibaddr = NativeMethods.GetProcAddress(NativeMethods.GetModuleHandle("kernel32.dll"), "LoadLibraryA");
                string dll = Path.GetFullPath("isaac-injector.dll");
                IntPtr malloc = NativeMethods.VirtualAllocEx(handle, IntPtr.Zero, (uint)((dll.Length + 1) * Marshal.SizeOf(typeof(char))), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
                UIntPtr bytesWritten;
                NativeMethods.WriteProcessMemory(handle, malloc, Encoding.Default.GetBytes(dll), (uint)((dll.Length + 1) * Marshal.SizeOf(typeof(char))), out bytesWritten);
                NativeMethods.CreateRemoteThread(handle, IntPtr.Zero, 0, loadlibaddr, malloc, 0, IntPtr.Zero);
            }
            catch (IndexOutOfRangeException) { return hResult.ProcNotFound; }
            catch (Exception ex) { return hResult.Error; }
            return hResult.Success;
        }
    }

    public enum hResult
    {
        ProcNotFound,
        Error,
        Success
    }
}
