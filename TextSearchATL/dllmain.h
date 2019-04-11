// dllmain.h : Declaration of module class.

class CTextSearchATLModule : public ATL::CAtlDllModuleT< CTextSearchATLModule >
{
public :
	DECLARE_LIBID(LIBID_TextSearchATLLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_TEXTSEARCHATL, "{B42BF192-9DE0-4F34-A384-840A01A9E299}")
};

extern class CTextSearchATLModule _AtlModule;
