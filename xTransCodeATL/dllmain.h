// dllmain.h: 模块类的声明。

class CxTransCodeATLModule : public ATL::CAtlDllModuleT< CxTransCodeATLModule >
{
public :
	DECLARE_LIBID(LIBID_xTransCodeATLLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_XTRANSCODEATL, "{08D2B466-072B-4E69-9B54-4C8866C60843}")
};

extern class CxTransCodeATLModule _AtlModule;
