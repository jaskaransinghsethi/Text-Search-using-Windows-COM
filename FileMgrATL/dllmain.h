// dllmain.h : Declaration of module class.

class CFileMgrATLModule : public ATL::CAtlDllModuleT< CFileMgrATLModule >
{
public :
	DECLARE_LIBID(LIBID_FileMgrATLLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_FILEMGRATL, "{8CB9F7D8-4879-4ED1-A632-E2007C0DE75C}")
};

extern class CFileMgrATLModule _AtlModule;
