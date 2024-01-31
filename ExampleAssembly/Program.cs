using System;
using System.Reflection;

namespace ExampleAssembly
{
    internal class Program
    {
        static void Main()
        {
            Type exitClass = typeof(System.Environment);
            string exitName = "Exit";
            BindingFlags exitBinding = BindingFlags.Static | BindingFlags.Public;

            MethodInfo exitInfo = exitClass.GetMethod(exitName, exitBinding);
            RuntimeMethodHandle exitRtHandle = exitInfo.MethodHandle;
            IntPtr exitPtr = exitRtHandle.GetFunctionPointer();

            Console.WriteLine("[M] Exit function pointer: 0x{0:X16}", exitPtr.ToInt64());

            System.Environment.Exit(0);

            Console.WriteLine("[M] Survived exit!");
        }
    }
}
