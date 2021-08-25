using System.Runtime.InteropServices;

namespace Net6ClassLib
{
    public static class Net6Lib
    {
        [DllImport("C:\\ATLNet6\\Debug\\ATLNet6.dll", CharSet = CharSet.Unicode, EntryPoint = "?EventProxy2@CATLNet6Object@@QAEXJ@Z")]
        public static extern void EventProxy2(long newResult);

        public static double Addition()
        {
            EventProxy2(42);
            return 4 + 4;
        }


        // entry points for .NET runtime calls
        public delegate double CalculationDelegate();
    }
}
