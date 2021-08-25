// ATLNet6Object.h : Declaration of the CATLNet6Object

#pragma once
#include "resource.h"       // main symbols

#include "ATLNet6_i.h"
#include "_IATLNet6ObjectEvents_CP.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;


// CATLNet6Object

class ATL_NO_VTABLE CATLNet6Object :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CATLNet6Object, &CLSID_ATLNet6Object>,
	public IConnectionPointContainerImpl<CATLNet6Object>,
	public CProxy_IATLNet6ObjectEvents<CATLNet6Object>,
	public IDispatchImpl<IATLNet6Object, &IID_IATLNet6Object, &LIBID_ATLNet6Lib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{

public:
	CATLNet6Object()
	{
	}

DECLARE_REGISTRY_RESOURCEID(106)


BEGIN_COM_MAP(CATLNet6Object)
	COM_INTERFACE_ENTRY(IATLNet6Object)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CATLNet6Object)
	CONNECTION_POINT_ENTRY(__uuidof(_IATLNet6ObjectEvents))
END_CONNECTION_POINT_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	STDMETHOD(Addition)(DOUBLE *result);
	__declspec(dllexport) void (EventProxy2)(LONG newResult);
};

OBJECT_ENTRY_AUTO(__uuidof(ATLNet6Object), CATLNet6Object)
