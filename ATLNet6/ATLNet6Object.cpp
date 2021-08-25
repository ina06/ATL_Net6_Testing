// ATLNet6Object.cpp : Implementation of CATLNet6Object

/********************************************************************************************
 * Imports
 ********************************************************************************************/
// needed for COM exposure
#include "pch.h"
#include "ATLNet6Object.h"

// needed for hosting .NET runtime
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <iostream>

// Provided by the AppHost NuGet package and installed as an SDK pack 
#include "nethost.h"

// Header files copied from https://github.com/dotnet/core-setup
#include "coreclr_delegates.h"
#include "hostfxr.h"

// defines and imports depending on current os
#ifdef _WIN32
#include <Windows.h>

#define STR(s) L ## s
#define CH(c) L ## c
#define DIR_SEPARATOR L'\\'

#endif


/********************************************************************************************
 * Global variables and forward definitions for hosting the .NET runtime
 ********************************************************************************************/
namespace
{
    // Globals to hold hostfxr exports
    hostfxr_initialize_for_runtime_config_fn init_fptr;
    hostfxr_get_runtime_delegate_fn get_delegate_fptr;
    hostfxr_close_fn close_fptr;

    // Function pointers for .NET core assembly functions
    double (*addition)(void);

    // Forward definitions
    load_assembly_and_get_function_pointer_fn get_dotnet_load_assembly(const char_t* assembly);
    void* load_library(const char_t*);
    void* get_export(void*, const char*);
}

using string_t = std::basic_string<char_t>;

 /********************************************************************************************
  * Function used to load and activate .NET Core
  ********************************************************************************************/
namespace
{
    // Forward declarations
    void* load_library(const char_t*);
    void* get_export(void*, const char*);

#ifdef _WIN32
    void* load_library(const char_t* path)
    {
        HMODULE h = ::LoadLibraryW(path);
        assert(h != nullptr);
        return (void*)h;
    }
    void* get_export(void* h, const char* name)
    {
        void* f = ::GetProcAddress((HMODULE)h, name);
        assert(f != nullptr);
        return f;
    }
#endif

    // Load and initialize .NET Core and get desired function pointer for scenario
    load_assembly_and_get_function_pointer_fn get_dotnet_load_assembly(const char_t* config_path)
    {
        // Load .NET Core
        void* load_assembly_and_get_function_pointer = nullptr;
        hostfxr_handle cxt = nullptr;
        int rc = init_fptr(config_path, nullptr, &cxt);
        if (rc != 0 || cxt == nullptr)
        {
            std::cerr << "Init failed: " << std::hex << std::showbase << rc << std::endl;
            close_fptr(cxt);
            return nullptr;
        }

        rc = get_delegate_fptr(
            cxt,
            hdt_load_assembly_and_get_function_pointer,
            &load_assembly_and_get_function_pointer);
        if (rc != 0 || load_assembly_and_get_function_pointer == nullptr)
            std::cerr << "Get delegate failed: " << std::hex << std::showbase << rc << std::endl;

        close_fptr(cxt);
        return (load_assembly_and_get_function_pointer_fn)load_assembly_and_get_function_pointer;
    }
}

/********************************************************************************************
 * Initialization of function pointers to .NET Core methods
 ********************************************************************************************/
int initializeCoreReferences() {
    // Load nethost.dll
#ifdef _WIN32
    // Path to nethost.dll location for 32 bit build
    const wchar_t* nethostPath = L"C:\\Program Files (x86)\\dotnet\\packs\\Microsoft.NETCore.App.Host.win-x86\\6.0.0-preview.6.21352.12\\runtimes\\win-x86\\native\\nethost.dll";
    const wchar_t* nethostDirectory = L"C:\Program Files\dotnet\packs\Microsoft.NETCore.App.Host.win-x64\6.0.0-preview.6.21352.12\runtimes\win-x86\native";
#endif 

    // might be needed to set the directory back to origin
    TCHAR NPath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, NPath);

    SetCurrentDirectory(nethostDirectory);

    // Load nethost.dll with provided path
    HMODULE handleNethost{ LoadLibrary(nethostPath)};

    if (handleNethost == NULL) {
        assert(false && "Failure: loading nethost.dll");
        return EXIT_FAILURE;
    }

    // Get get_hostfxr_path
    auto get_hostfxr_path_handle{ reinterpret_cast<decltype (&get_hostfxr_path)>(GetProcAddress(handleNethost, "get_hostfxr_path")) };
    if (get_hostfxr_path_handle == NULL) {
        assert(false && "Failure: getting address of the get_hostfxr_path");
        return EXIT_FAILURE;
    }


    // Pre-allocate a large buffer for the path to hostfxr
    char_t buffer[MAX_PATH];
    size_t buffer_size = sizeof(buffer) / sizeof(char_t);

    // Locate hostfxr.dll
    int result{ get_hostfxr_path_handle(buffer, &buffer_size, nullptr) };

    if (result != 0) {
        assert(false && "Failure: getting address of the get_hostfxr_path");
        return EXIT_FAILURE;
    }

    // Load hostfxr.dll
    void* lib = load_library(buffer);

    // Get desired exports
    init_fptr = (hostfxr_initialize_for_runtime_config_fn)get_export(lib, "hostfxr_initialize_for_runtime_config");
    get_delegate_fptr = (hostfxr_get_runtime_delegate_fn)get_export(lib, "hostfxr_get_runtime_delegate");
    close_fptr = (hostfxr_close_fn)get_export(lib, "hostfxr_close");


    if (!(init_fptr && get_delegate_fptr && close_fptr))
    {
        assert(false && "Failure: load_hostfxr()");
        return EXIT_FAILURE;
    }
    
    load_assembly_and_get_function_pointer_fn load_assembly_and_get_function_pointer = nullptr;

    // Path to runtimeconfig.json file 
    const char_t* config_path = STR("C:\\ATLNet6\\runtimeconfig.json");

    load_assembly_and_get_function_pointer = get_dotnet_load_assembly(config_path);

    assert(load_assembly_and_get_function_pointer != nullptr && "Failure: get_dotnet_load_assembly()");

    // Load managed assembly and get function pointer to a managed method
    // Set params to get assembly
   
    // Path to Net6 library dll
    const string_t dotnetlib_path = STR("C:\\ATLNet6\\Net6ClassLib\\bin\\Debug\\net6.0\\Net6ClassLib.dll");
    const char_t* dotnet_type = STR("Net6ClassLib.Net6Lib, Net6ClassLib");
    const char_t* dotnet_type_method;
    const char_t* dotnet_delegate_name;

    // Function pointer to managed delegates
    // Initialize
    addition = nullptr;

    // GET FUNCTION POINTERS FOR .NET ASSEMBLY FUNCTIONS
    dotnet_type_method = STR("Addition");
    dotnet_delegate_name = STR("Net6ClassLib.Net6Lib+CalculationDelegate, Net6ClassLib");
    int rcAddition = load_assembly_and_get_function_pointer(
        dotnetlib_path.c_str(),
        dotnet_type,
        dotnet_type_method,
        dotnet_delegate_name,
        nullptr,
        (void**)&addition);

    assert(rcAddition == 0 && addition != nullptr && "Failure: load_assembly_and_get_function_pointer() for addition");
    
    return EXIT_SUCCESS;
}

/********************************************************************************************
 * Functions to be exposed to COM
 ********************************************************************************************/
 // CATLNet6Object
STDMETHODIMP CATLNet6Object::Addition(DOUBLE *result)
{
    initializeCoreReferences();
  
    *result = addition();

    return S_OK;
}


/********************************************************************************************
 * Proxy functions to call event firing functions
 ********************************************************************************************/
__declspec(dllexport) void CATLNet6Object::EventProxy2(LONG newResult) {

    HRESULT error = Fire_CalcResultChanged(newResult);

}
