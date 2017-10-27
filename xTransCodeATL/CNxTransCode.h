// CNxTransCode.h : CNxTransCode ������

#pragma once
#include "resource.h"       // ������



#include "xTransCodeATL_i.h"
#include "_INxTransCodeEvents_CP.h"

#include "xAPI.h"
#include "xIDs.h"
#include "xLogger.h"
#include "CNxLiveWindowNode.h"
#include "CNxCommonFileReaderNode.h"

#include "CNxWriterNode.h"
#include "CNxDirectSoundOutputNode.h"
#include "CNxMuxerNode.h"
#include "CNxCompositorNode.h"
#include "CNxTopology.h"

#include "CNxM264XAVCCodecNode.h"
#include "CNxAVCEncodeMCNode.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CNxTransCode

typedef struct VideoInfo {
	std::wstring wstrVideoFile;
	std::wstring wstrCam;
}SNxVideoInfo;

class ATL_NO_VTABLE CNxTransCode :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CNxTransCode, &CLSID_NxTransCode>,
	public IConnectionPointContainerImpl<CNxTransCode>,
	public CProxy_INxTransCodeEvents<CNxTransCode>,
	public IDispatchImpl<INxTransCode, &IID_INxTransCode, &LIBID_xTransCodeATLLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CNxTransCode();

DECLARE_REGISTRY_RESOURCEID(IDR_NXTRANSCODE)


BEGIN_COM_MAP(CNxTransCode)
	COM_INTERFACE_ENTRY(INxTransCode)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CNxTransCode)
	CONNECTION_POINT_ENTRY(__uuidof(_INxTransCodeEvents))
END_CONNECTION_POINT_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease();

public:

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE TransCode_Begin(
		/* [in] */ BSTR in_bstrFile);

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE TransElement_Begin();

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE TransElement_AddVideoFile(
		/* [in] */ BSTR in_bstrFile,
		/* [in] */ unsigned long in_ulIn,
		/* [in] */ unsigned long in_ulOut,
		/* [in] */ BSTR in_bstrCam);

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE TransElement_AddAudioFile(
		/* [in] */ BSTR in_bstrFile,
		/* [in] */ unsigned long in_ulIn,
		/* [in] */ unsigned long in_ulOut);

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE TransElement_End(BOOL bUseFFmpeg);

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE TransCode_End();

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE TransCode_SetWrapperFileType(
		/* [in] */ ENsWrapperFileTypeIDL in_eWrapperFileTypeIDL);

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE TransElement_GetCurrentPositionInFrames(/*[out]*/ULONGLONG* out_iCurrentFramePosition);

	virtual /* [id] */HRESULT STDMETHODCALLTYPE TransElement_SetFramesReaderStatus(/*[in]*/BOOL in_bStatus);

	virtual /* [id] */HRESULT TransElement_SetMixSchedule
		(
			/* [in] */ double in_ulMixStart,
			/* [in] */ double in_ulMixEnd
			);
private:

	TNsSmartPtr<INsdResourceManagerCreator> m_pJResourceManagerCreator;
	TNsSmartPtr<INsdResourceManager> m_pJINsdResourceManager;

	CNxTopology *m_pTopology;


	

	CNxCommonFileReaderNode **m_ppVideoReaderNode;
	CNxCommonFileReaderNode **m_ppAudioReaderNode;
	CNxBaseNode *m_pEncodeNode;
	CNxWriterNode *m_pWriterNode;
	CNxCompositorNode *m_pCompositorNode;
	CNxMuxerNode *m_pMuxerNode;

	std::vector<std::wstring>	m_wszvectorAudioFiles;
//	std::vector<std::wstring>m_wszvectorVideoFiles;
	std::vector<SNxVideoInfo>	m_wszvectorVideoInfos;
	wchar_t       m_wszTransCodeOutputFile[_MAX_PATH_R];

	unsigned long m_aVideoIn[2];
	unsigned long m_aVideoOut[2];
	double m_aMixSchedule[2];

	unsigned long m_aAudioIn[8];
	unsigned long m_aAudioOut[8];

	char m_szLogString[_MAX_PATH_R];

	bool         m_bInitialize;
	ENsWrapperFileTypeIDL m_eENsWrapperFileTypeIDL;
	bool         m_bSoftEncode;
	
	bool		 m_bUseFFmpeg;
private:
	HRESULT _init();

	HRESULT _execute_transcode();
};

OBJECT_ENTRY_AUTO(__uuidof(NxTransCode), CNxTransCode)
