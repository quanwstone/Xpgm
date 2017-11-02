// CNxTransCode.cpp : CNxTransCode 的实现

#include "stdafx.h"
#include "CNxTransCode.h"


// CNxTransCode

CNxTransCode::CNxTransCode()
{
	m_ppAudioReaderNode = NULL;
	m_pJResourceManagerCreator = NULL;
	m_pJINsdResourceManager = NULL;
	m_bInitialize = false;

	//m_eENsWrapperFileTypeIDL = keNsWrapperFileTypeIDL_MXF_DNxHR;
	m_eENsWrapperFileTypeIDL = keNsWrapperFileTypeIDL_MXF_XAVC_MC;
	m_bUseFFmpeg = true;
	m_bSoftEncode = true;
	memset(m_buf_guid,0,64);
}


void CNxTransCode::FinalRelease()
{
	m_pTopology->Stop();

	delete m_pTopology;

	m_pJResourceManagerCreator = NULL;
	m_pJINsdResourceManager = NULL;

}
// convert unicode to TCHAR
BOOL inline ConvertFromWCHARToTCHAR(LPCWSTR pszWCHAR, LPTSTR pszTCHAR, int nMax)
{
#ifdef _UNICODE
	wcsncpy(pszTCHAR, pszWCHAR, nMax);
	pszTCHAR[nMax - 1] = L'\0';
	return TRUE;
#else
	int nRet = WideCharToMultiByte(CP_THREAD_ACP, 0, pszWCHAR, -1, (LPSTR)pszTCHAR, nMax, NULL, NULL);
	return (nRet>0);
#endif
}

HRESULT CNxTransCode::TransCode_Begin(
	/* [in] */ BSTR in_bstrFile
	)
{
	HRESULT hr = NS_NOERROR;

	wcscpy_s(m_wszTransCodeOutputFile, in_bstrFile);

	if (m_pJResourceManagerCreator == NULL)
	{
		_get_guid();
		HRESULT hr = CoCreateInstance(CLSID_NsdResourceManagerCreator,
			NULL,
			CLSCTX_INPROC,
			IID_INsdResourceManagerCreator,
			(void **)&m_pJResourceManagerCreator);
		NSD_SAFE_REPORT_ERROR(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, hr,
			"CNxTransCode::TransCode_Begin, Create CLSID_NsdResourceManagerCreator Failed!", true);

		SNsResolutionInfo  sResolutionInfo;
		sResolutionInfo.size = sizeof(SNsResolutionInfo);

		sResolutionInfo.b3DVideo = false;
		sResolutionInfo.eAspectRatio = keNsAspectRatio_16_9;
		sResolutionInfo.eFrameRate = keNsFrameRate50;
		sResolutionInfo.eScanMode = keNsScanModeProgressive;
		sResolutionInfo.eColorSpace = keNsColorSpaceYUV;
		sResolutionInfo.ulComponentBitCount = 8;
		sResolutionInfo.ulWidth = 3840;
		sResolutionInfo.ulHeight = 2160;
		sResolutionInfo.eAudioSampleBitCount = keNsAudioSampleBitCount16In16;
		sResolutionInfo.eAudioSamplingFrequency = keNsAudioSamplingFrequency48000;

		hr = m_pJResourceManagerCreator->CreateResourceManager(sResolutionInfo, NULL, keNsResourceProfileFullSet, &m_pJINsdResourceManager);
		NSD_SAFE_REPORT_ERROR(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, hr,
			"CNxTransCode::TransCode_Begin, Create CreateResourceManager Failed!", true);

		//writer node.
		m_pWriterNode = new CNxWriterNode(L"WriterNode", keNsTreeNodeTypeRoot, &hr);
		m_pWriterNode->SetResourceManager(m_pJINsdResourceManager);


		if (m_eENsWrapperFileTypeIDL == keNsWrapperFileTypeIDL_MXF_DNxHR)
		{
			m_pWriterNode->SetWrapperFileType(keNsWrapperFileType_MXF_DNxHR);
		}
		else if (m_eENsWrapperFileTypeIDL == keNsWrapperFileTypeIDL_MXF_XAVC_MC)
		{
			m_pWriterNode->SetWrapperFileType(keNsWrapperFileType_MXF_XAVC_MC);
		}

	}

	hr = m_pWriterNode->SetFileName(m_wszTransCodeOutputFile);
	NSD_SAFE_REPORT_ERROR(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, hr,
		"CNxTransCode::TransCode_Begin, Create SetFileName Failed!", true);


	ConvertFromWCHARToTCHAR(m_wszTransCodeOutputFile, m_szLogString, _MAX_PATH_R);

	NS_LOG_WSTRING_FORMAT(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, keMessage, (L"TransCode_Begin PGM:%S", m_szLogString));
	
	return hr;
}

HRESULT  CNxTransCode::TransElement_Begin()
{
	HRESULT hr = NS_NOERROR;
	m_wszvectorAudioFiles.clear();
	m_wszvectorVideoInfos.clear();

	NS_LOG_WSTRING_FORMAT(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, keMessage,(L"TransElement_Begin # GUID::%S",m_buf_guid));
	return hr;
}

HRESULT  CNxTransCode::TransElement_AddVideoFile(
	/* [in] */ BSTR in_bstrFile,
	/* [in] */ unsigned long in_ulIn,
	/* [in] */ unsigned long in_ulOut,
	/* [in] */ BSTR in_bstrCam
	)
{
	HRESULT hr = NS_NOERROR;
	int nIndex = m_wszvectorVideoInfos.size();
	if (nIndex >= 2)
	{
		NS_LOG_WSTRING_FORMAT(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, keMessage, (L"TransElement_AddVideoFile nIndex >= 2!"));
		return hr;
	}

	m_aVideoIn[nIndex] = in_ulIn;
	m_aVideoOut[nIndex] = in_ulOut;
	
	std::wstring wsztemp(in_bstrFile);
	std::wstring wszCam(in_bstrCam);

	//m_wszvectorVideoFiles.push_back(wsztemp);
	SNxVideoInfo strvideinfo;
	strvideinfo.wstrCam = wszCam;
	strvideinfo.wstrVideoFile = wsztemp;
	m_wszvectorVideoInfos.push_back(strvideinfo);

	ConvertFromWCHARToTCHAR(in_bstrFile, m_szLogString, _MAX_PATH_R);

	//m_aVideoIn[nIndex] = in_ulIn;
	//m_aVideoOut[nIndex] = in_ulOut;
	//std::wstring wsztemp(L"E:/MCEContent/20170616125930/High/节目01_20170616125930_3.MXF");
	//m_wszvectorVideoFiles.push_back(wsztemp);

	//ConvertFromWCHARToTCHAR(L"E:\\MCEContent\\20170616125930\\High\\节目01_20170616125930_3.MXF", m_szLogString, _MAX_PATH_R);

	NS_LOG_WSTRING_FORMAT(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, keMessage, (L"TransElement_AddVideoFile [%d,%d),%S,# GUID::%S", in_ulIn, in_ulOut, m_szLogString,m_buf_guid));
	//hr = TransElement_AddAudioFile(in_bstrFile, in_ulIn, in_ulOut);

	return hr;
}

HRESULT  CNxTransCode::TransElement_AddAudioFile(
	/* [in] */ BSTR in_bstrFile,
	/* [in] */ unsigned long in_ulIn,
	/* [in] */ unsigned long in_ulOut)
{
	HRESULT hr = NS_NOERROR;
	std::wstring wsztemp(in_bstrFile);
	int nIndex = m_wszvectorAudioFiles.size();

	//if (nIndex >= 2)
	//{
	//	NS_LOG_WSTRING_FORMAT(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, keMessage, (L"TransElement_AddAudioFile nIndex >= 8"));
	//	return hr;
	//}
	m_aAudioIn[nIndex] = in_ulIn;
	m_aAudioOut[nIndex] = in_ulOut;
	m_wszvectorAudioFiles.push_back(wsztemp);

	ConvertFromWCHARToTCHAR(in_bstrFile, m_szLogString, _MAX_PATH_R);

	//std::wstring wsztemp(L"E:/MCEContent/20170616125930/High/节目01_20170616125930_3.MXF");
	//int nIndex = m_wszvectorAudioFiles.size();

	//if (nIndex >= 8)
	//{
	//	NS_LOG_WSTRING_FORMAT(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, keMessage, (L"TransElement_AddAudioFile nIndex >= 8"));
	//	return hr;
	//}
	//m_aAudioIn[nIndex] = in_ulIn;
	//m_aAudioOut[nIndex] = in_ulOut;
	//m_wszvectorAudioFiles.push_back(wsztemp);

	//ConvertFromWCHARToTCHAR(L"E:\\MCEContent\\20170616125930\\High\\节目01_20170616125930_3.MXF", m_szLogString, _MAX_PATH_R);

	//
	NS_LOG_WSTRING_FORMAT(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, keMessage, (L"TransElement_AddAudioFile [%d,%d),%S#GUID::%S", in_ulIn, in_ulOut, m_szLogString,m_buf_guid));
	return hr;
}
HRESULT CNxTransCode::TransElement_SetMixSchedule(
	/* [in] */ double in_ulMixStart,
	/* [in] */ double in_ulMixEnd
	)
{
	HRESULT hr = NS_NOERROR;

	m_aMixSchedule[0] = in_ulMixStart;
	m_aMixSchedule[1] = in_ulMixEnd;
	NS_LOG_WSTRING_FORMAT(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, keMessage, (L"TransElement_SetMixSchedule [%f,%f)#GUID::%S", in_ulMixStart, in_ulMixEnd,m_buf_guid));
	return hr;
}
HRESULT CNxTransCode::TransElement_GetCurrentPositionInFrames(/*[out]*/ULONGLONG* out_iCurrentFramePosition)
{
	HRESULT hr = NS_NOERROR;
	if (m_ppVideoReaderNode == NULL || m_ppVideoReaderNode[0] == NULL) {
		
		*out_iCurrentFramePosition = 0;

		return hr;
	}
	ULONGLONG iCurrentFramePosition = 0;
	m_ppVideoReaderNode[0]->GetCurrentPositionInFrames(&iCurrentFramePosition);
	*out_iCurrentFramePosition  = iCurrentFramePosition;
	return hr;
}
HRESULT CNxTransCode::TransElement_SetFramesReaderStatus(/*[in]*/BOOL in_bStatus)
{
	HRESULT hr = NS_NOERROR;

	if (m_ppVideoReaderNode == NULL || m_ppVideoReaderNode[0] == NULL) {

		NSD_SAFE_REPORT_ERROR(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, NS_E_FAIL,
			(L"CNxTransCode::TransElement_SetFramesReaderStatus,m_ppVideoReaderNode is NULL!#GUID::%S",m_buf_guid), true);
		return NS_E_FAIL;
	}
	m_ppVideoReaderNode[0]->SetFramesPraseStatus(in_bStatus);
	m_ppVideoReaderNode[1]->SetFramesPraseStatus(in_bStatus);
	
	NS_LOG_WSTRING_FORMAT(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, keMessage, (L"TransElement_SetFramesReaderStatus: %d#GUID::%S", in_bStatus,m_buf_guid));
	return hr;
}
HRESULT  CNxTransCode::_execute_transcode()
{

	HRESULT hr = NS_NOERROR;
	int nAudios = m_wszvectorAudioFiles.size();
	int nVideos = m_wszvectorVideoInfos.size();

	bool bNeedMix = false;
	bool bUseCodec = true;

	if (nVideos == 2)
		bNeedMix = true;

	if (!bNeedMix)
	{
		m_pTopology->Stop();

		m_pTopology->UnLinkAll();
		m_pTopology->RemoveAll();

		hr = m_ppVideoReaderNode[0]->SetFileName2(m_wszvectorVideoInfos[0].wstrVideoFile.c_str(), m_wszvectorVideoInfos[0].wstrCam.c_str());
		NSD_SAFE_REPORT_ERROR(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, hr,
			(L"CNxTransCode::_execute_transcode,m_ppVideoReaderNode[0] SetFileName2 Failed!#GUID::%S",m_buf_guid), true);

		if (bUseCodec)
		{
			m_ppVideoReaderNode[0]->SetOutputFormat(keNsSurfaceFormatYUY2);
		}
		else
		{
			m_ppVideoReaderNode[0]->SetOutputFormat(keNsSurfaceFormatAVC);
		}


		hr = m_pTopology->AddNode(m_ppVideoReaderNode[0]);
		hr = m_pTopology->AddNode(m_pMuxerNode);

		for (int i = 0; i < nAudios; i++)
		{
			hr = m_pTopology->AddNode(m_ppAudioReaderNode[i]);
		}
		hr = m_pTopology->AddNode(m_pWriterNode);

		//Link Topology
		//reader(A/V) - > muxer

		if (bUseCodec)
		{
			hr = m_pTopology->AddNode(m_pEncodeNode);

			m_pEncodeNode->ConnectToChildNode(0, keNsChildUsageDontCare, m_ppVideoReaderNode[0]);
			m_pMuxerNode->ConnectToChildNode(0, keNsChildUsageDontCare,m_pEncodeNode);
		}
		else
		{
			m_pMuxerNode->ConnectToChildNode(0, keNsChildUsageDontCare, m_ppVideoReaderNode[0]);
		}

		for (int i = 0; i < nAudios; i++)
		{
			m_pMuxerNode->ConnectToChildNode(i + 1, keNsChildUsageDontCare, m_ppAudioReaderNode[i]);
		}

		//muxer - > mxf writer
		m_pWriterNode->ConnectToChildNode(0, keNsChildUsageDontCare, m_pMuxerNode);



		m_pTopology->Run();

		//Set Read Task.
		m_ppVideoReaderNode[0]->SetReadTask(m_aVideoIn[0], m_aVideoOut[0]);
		
		hr = m_pWriterNode->SetTask(m_aVideoIn[0], m_aVideoOut[0]);
		NSD_SAFE_REPORT_ERROR(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, hr,
			"CNxTransCode::_execute_transcode,m_pWriterNode SetTask Failed!", true);
		if (bUseCodec)
		{

			if (m_bSoftEncode)
			{
				((CNxAVCEncodeMCNode *)m_pEncodeNode)->SetTask(m_aVideoIn[0], m_aVideoOut[0]);
			}
		}

		for (int i = 0; i < nAudios; i++)
		{
			m_ppAudioReaderNode[i]->SetAudioStreamIndex(i + 1);
			m_ppAudioReaderNode[i]->SetReadTask(m_aAudioIn[i], m_aAudioOut[i]);
	
		}

	}
	else
	{
		m_pTopology->Stop();

		m_pTopology->UnLinkAll();
		m_pTopology->RemoveAll();

		hr = m_ppVideoReaderNode[0]->SetFileName2(m_wszvectorVideoInfos[0].wstrVideoFile.c_str(), m_wszvectorVideoInfos[0].wstrCam.c_str());
		NSD_SAFE_REPORT_ERROR(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, hr,
			(L"CNxTransCode::_execute_transcode,m_ppVideoReaderNode[0] SetFileName2 Failed!#GUID::%S",m_buf_guid), true);

		m_ppVideoReaderNode[0]->SetOutputFormat(keNsSurfaceFormatYUY2);

		hr = m_ppVideoReaderNode[1]->SetFileName2(m_wszvectorVideoInfos[1].wstrVideoFile.c_str(), m_wszvectorVideoInfos[1].wstrCam.c_str());
		NSD_SAFE_REPORT_ERROR(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, hr,
			(L"CNxTransCode::_execute_transcode,m_ppVideoReaderNode[1] SetFileName2 Failed!#GUID::%S",m_buf_guid), true);

		m_ppVideoReaderNode[1]->SetOutputFormat(keNsSurfaceFormatYUY2);


		hr = m_pTopology->AddNode(m_pWriterNode);
		hr = m_pTopology->AddNode(m_pMuxerNode);
		hr = m_pTopology->AddNode(m_pEncodeNode);
		hr = m_pTopology->AddNode(m_pCompositorNode);

		hr = m_pTopology->AddNode(m_ppVideoReaderNode[0]);
		hr = m_pTopology->AddNode(m_ppVideoReaderNode[1]);
		for (int i = 0; i < nAudios; i++)
		{
			hr = m_pTopology->AddNode(m_ppAudioReaderNode[i]);
		}

		//Link Topology
		//reader(A/V) - > muxer
		m_pCompositorNode->ConnectToChildNode(0, keNsChildUsageDontCare, m_ppVideoReaderNode[0]);
		m_pCompositorNode->ConnectToChildNode(1, keNsChildUsageDontCare, m_ppVideoReaderNode[1]);

		hr = m_pCompositorNode->SetMixSchedule(m_aMixSchedule[0], m_aMixSchedule[1]);
		NSD_SAFE_REPORT_ERROR(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, hr,
			(L"CNxTransCode::_execute_transcode,SetMixOffset Failed!#GUID::%S",m_buf_guid), true);

		m_pEncodeNode->ConnectToChildNode(0, keNsChildUsageDontCare, m_pCompositorNode);
		m_pMuxerNode->ConnectToChildNode(0, keNsChildUsageDontCare, m_pEncodeNode);
		for (int i = 0; i < nAudios; i++)
		{
			m_pMuxerNode->ConnectToChildNode(i + 1, keNsChildUsageDontCare, m_ppAudioReaderNode[i]);
		}
		//muxer - > mxf writer
		m_pWriterNode->ConnectToChildNode(0, keNsChildUsageDontCare, m_pMuxerNode);



		m_pTopology->Run();
		//Set Read Task.
		hr = m_ppVideoReaderNode[0]->SetReadTask(m_aVideoIn[0], m_aVideoOut[0]);
		hr = m_ppVideoReaderNode[1]->SetReadTask(m_aVideoIn[1], m_aVideoOut[1]);
		//Set Writer Task.
		hr = m_pWriterNode->SetTask(m_aVideoIn[0], m_aVideoOut[0]);
		NSD_SAFE_REPORT_ERROR(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, hr,
			"CNxTransCode::_execute_transcode,m_pWriterNode SetTask Failed!", true);


		if (m_bSoftEncode)
		{
			((CNxAVCEncodeMCNode *)m_pEncodeNode)->SetTask(m_aVideoIn[1], m_aVideoOut[1]);
		}

		for (int i = 0; i < nAudios; i++)
		{
			m_ppAudioReaderNode[i]->SetAudioStreamIndex(i + 1);

			m_ppAudioReaderNode[i]->SetReadTask(m_aAudioIn[i], m_aAudioOut[i]);
			
		}
	}

	return hr;
}

HRESULT CNxTransCode::_get_guid()
{
	HRESULT hr = NS_NOERROR;

	GUID guid;
	hr = CoCreateGuid(&guid);
	if (hr != S_OK)
	{
		NSD_SAFE_REPORT_ERROR(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, NS_E_FAIL,
			"CNxTransCode::_get_guid,CoCreateGuid Failed!", true);
		return NS_E_FAIL;
	}
	
	_stprintf_s(
		m_buf_guid, 
		"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1],
		guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);

	NS_LOG_WSTRING_FORMAT(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, keMessage, (L"CNxTransCode#GUID::,%S", m_buf_guid));
	return hr;
}
HRESULT  CNxTransCode::_init()
{
	HRESULT hr = NS_NOERROR;

	int nAudios = m_wszvectorAudioFiles.size();
	int nVideos = m_wszvectorVideoInfos.size();
	
	NS_LOG_WSTRING_FORMAT(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, keMessage, (L"CNxTransCode::Init#GUID::%S", m_buf_guid));

	m_pTopology = new CNxTopology(L"Topology");
	m_ppVideoReaderNode = new CNxCommonFileReaderNode*[2];

	for (int i = 0; i < 2; i++)
	{
		m_ppVideoReaderNode[i] = new CNxCommonFileReaderNode(L"VideoReaderNode", keNsTreeNodeTypeRoot, &hr,m_bUseFFmpeg);
		m_ppVideoReaderNode[i]->SetResourceManager(m_pJINsdResourceManager);
		//m_ppVideoReaderNode[i]->SetOutputFormat(keNsSurfaceFormatDNX_145_1080i);
		m_ppVideoReaderNode[i]->SetOutputFormat(keNsSurfaceFormatAVC);
		m_ppVideoReaderNode[i]->SetReaderType(keNsCommonReaderTypeVideo);
	}
	m_ppAudioReaderNode = new CNxCommonFileReaderNode*[nAudios];
	for (int i = 0; i < nAudios; i++)
	{
		m_ppAudioReaderNode[i] = new CNxCommonFileReaderNode(L"AudioReaderNode", keNsTreeNodeTypeRoot, &hr, m_bUseFFmpeg);
		m_ppAudioReaderNode[i]->SetFileName2(m_wszvectorAudioFiles[i].c_str());
		m_ppAudioReaderNode[i]->SetResourceManager(m_pJINsdResourceManager);
		m_ppAudioReaderNode[i]->SetReaderType(keNsCommonReaderTypeAudio);
	}
	//muxer node.
	m_pMuxerNode = new CNxMuxerNode(L"MuxerNode", keNsTreeNodeTypeRoot, &hr);
	m_pMuxerNode->SetResourceManager(m_pJINsdResourceManager);
	m_pMuxerNode->SetAudioChannelCount(nAudios);

	m_pCompositorNode = new CNxCompositorNode(L"CompositorNode", keNsTreeNodeTypeRoot, &hr);
	m_pCompositorNode->SetResourceManager(m_pJINsdResourceManager);

	if (m_eENsWrapperFileTypeIDL == keNsWrapperFileTypeIDL_MXF_DNxHR)
	{
		//m_pEncodeNode = new CNxDNxHDCodecNode(L"DNxHDEncodeNode", keNsTreeNodeTypeRoot, &hr);
		//( (CNxDNxHDCodecNode *)m_pEncodeNode )->SetOutputFormat(keNsSurfaceFormatDNX_HQ);
	}
	else if (m_eENsWrapperFileTypeIDL == keNsWrapperFileTypeIDL_MXF_XAVC_MC)
	{
		if (m_bSoftEncode)
		{
			m_pEncodeNode = new CNxAVCEncodeMCNode(L"XAVCNode", keNsTreeNodeTypeRoot, &hr);
			((CNxAVCEncodeMCNode *)m_pEncodeNode)->SetOutputFormat(keNsSurfaceFormatAVC);
		}
		else
		{
			m_pEncodeNode = new CNxM264XAVCCodecNode(L"XAVCNode", keNsTreeNodeTypeRoot, &hr);
			((CNxM264XAVCCodecNode *)m_pEncodeNode)->SetOutputFormat(keNsSurfaceFormatAVC);

		}
	}

	
	m_pEncodeNode->SetResourceManager(m_pJINsdResourceManager);

	

	return NS_NOERROR;
}

HRESULT  CNxTransCode::TransElement_End(BOOL bUseFFmpeg)
{
	HRESULT hr = NS_NOERROR;
	int nAudios = m_wszvectorAudioFiles.size();
	int nVideos = m_wszvectorVideoInfos.size();

	NS_LOG_WSTRING_FORMAT(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, keMessage, (L"TransElement End#GUID::%S.",m_buf_guid));

	if (nVideos > 2 || nVideos < 0)
	{
		NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral,NS_E_FAIL,
			(L"CNxTransCode::TransElement_End, Create nVideos != 1 Failed!#GUID::%S",m_buf_guid), true);
	}

	if (nAudios == 0)
	{
		NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, NS_E_FAIL,
			(L"CNxTransCode::TransElement_End, Create nAudios == 0 Failed!#GUID::%S",m_buf_guid), true);
	}
	m_bUseFFmpeg = bUseFFmpeg;
	if (!m_bInitialize)
	{
		_init();
		m_bInitialize = true;
	}

	_execute_transcode();
	
	for (int i = 0; i < nVideos; i++)
	{
		hr = m_ppVideoReaderNode[i]->WaitReadTaskFinished();
		NSD_SAFE_REPORT_ERROR(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, hr,
			"CNxTransCode::TransElement_End, WaitReadTaskFinished Failed!", true);
	}

	for (int i = 0; i < nAudios; i++)
	{
		//暂时不顾音频,只考虑视频
		hr = m_ppAudioReaderNode[i]->WaitReadTaskFinished();

		hr = NS_NOERROR;
		NSD_SAFE_REPORT_ERROR(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, hr,
			"CNxTransCode::TransElement_End, WaitReadTaskFinished Failed!", true);
	}

	m_pWriterNode->WaitWriteTaskFinished();

	NS_LOG_WSTRING_FORMAT(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, keMessage, (L"TransElement Success.#GUID::%S",m_buf_guid));


	return hr;
}


HRESULT  CNxTransCode::TransCode_End()
{
	HRESULT hr = NS_NOERROR;

	if (m_pWriterNode != NULL)
	{
		hr = m_pWriterNode->FinishedWrite();
		NSD_SAFE_REPORT_ERROR(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, hr,
			"CNxTransCode::TransCode_End, FinishedWrite Failed!", true);
	}

	NS_LOG_WSTRING_FORMAT(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, keMessage, (L"TransCode Complete.#GUID::%S",m_buf_guid));
	return hr;
}


HRESULT  CNxTransCode::TransCode_SetWrapperFileType(
	/* [in] */ ENsWrapperFileTypeIDL in_eWrapperFileTypeIDL)
{
	HRESULT hr = NS_NOERROR;

	m_eENsWrapperFileTypeIDL = in_eWrapperFileTypeIDL;

	return hr;
}
