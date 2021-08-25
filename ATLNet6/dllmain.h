// dllmain.h : Declaration of module class.

class CATLNet6Module : public ATL::CAtlDllModuleT< CATLNet6Module >
{
public :
	DECLARE_LIBID(LIBID_ATLNet6Lib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ATLNET6, "{68169deb-692e-48dd-97b7-2c126609f543}")
};

extern class CATLNet6Module _AtlModule;
