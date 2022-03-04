# ATL_Net6_Testing

The purpose of this project is to test out a way to expose C# .NET 6.0 library functionality to different COM clients. To be able to provide a tlb together with the COM server an C++ ATL project is used which accesses the C# functions using a costum .NET host.

This approach does currently work for functions. However, COM events are not working properly at the moment. 
