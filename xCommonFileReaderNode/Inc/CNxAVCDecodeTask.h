#pragma once

#ifdef XAVCDECODEMCNODE_EXPORTS
#define XAVCDECODEMCNODE_API __declspec(dllexport)
#else
#define XAVCDECODEMCNODE_API __declspec(dllimport)
#endif

#include "xAPI.h"
#include "xLogger.h"
#include "CNxBaseNode.h"
// This class is exported from the XAVCDECODEMCNODE.dll
class  CNxAVCDecodeTask 
{
public:
	CNxAVCDecodeTask();

	virtual ~CNxAVCDecodeTask();

	virtual HRESULT __cdecl  ThreadNodeProcessLoop();
	
	HRESULT SetTask(uint64_t in_ui64Start, uint64_t in_ui64End);
	HRESULT SetResourceManager(INsdResourceManager *in_pResourceManage);
	TNsSmartPtr<INsAVContent> GetDecodeFrame();

	HRESULT DecodeFrame(INsAVContent *in_pAVContent);
public:
	static unsigned int __stdcall _taskthread(void* in_pThreadParameter);
	void  SetDrawCamInfo(bool bColorBar, char *chCam);
protected:
	HRESULT NodifyFrame(void* in_pDecode);
	HRESULT Flush(void *pDecode);
private:
	SNsdPoolSurfaceDescription m_sPoolSurfaceDescription;
	
	uint64_t m_ui64DecodeSendFrameCount;

	uint64_t m_ui64ReceiveFrameCountFromDecode;

	CNsCOMThread* m_poThreadGetDecodedFrame;
	uint64_t m_ui64ProcessFrameCount;

	uint64_t m_ui64Start;
	uint64_t m_ui64End;
	HANDLE    m_hFinishedReadEvent;

	CNsCOMThread* m_poThread;
	TNsSmartPtr<INsdResourceManager>  m_pJResourceManager;
	

	uint64_t m_ui64Input;
	uint64_t m_ui64Output;
	//Test_WriteFile_BMP
	void ConstructBih(int nWidth, int nHeight, BITMAPINFOHEADER& bih);
	void ContructBhh(int nWidth, int nHeight, BITMAPFILEHEADER& bhh);

	bool m_bDrawCamFlag;
	char m_chCamName[256];
public:
	std::map<unsigned long, CNsSignalQueue*, std::less<unsigned long> > m_mapQueueSignalThreadInputAVContents;
	std::map<unsigned long, CNsSignalQueue*, std::less<unsigned long> > m_mapQueueSignalThreadOutputAVContents;
	void *m_pDecoder;
};
