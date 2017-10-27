// xFFSourceNode.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "CNxCompositorNode.h"
#include <iostream>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */ 

	
#ifdef __cplusplus
#if __cplusplus
}

#endif
#endif /* __cplusplus */ 

unsigned long kulNsdAudioSamplesMaxBufferSize = 16016;


CNxCompositorNode::CNxCompositorNode

(
	wchar_t *in_pwszNodeName,
	ENsTreeNodeType     in_eNsTreeNodeType,
	HRESULT            *out_phr
	)
	:CNxBaseNode(in_pwszNodeName, in_eNsTreeNodeType, out_phr)
{


	m_sPoolSurfaceDescription.eContentAllocationType = keNsdAVContentAllocationTypeNormal;
	m_sPoolSurfaceDescription.eMemoryLocation = keNsMemoryLocationHost;
	m_sPoolSurfaceDescription.eMemoryUsage = keNsdMemoryUsageProcessing;
	m_sPoolSurfaceDescription.eScanMode = keNsScanModeProgressive;
	m_sPoolSurfaceDescription.ulCardID = 0;
	m_sPoolSurfaceDescription.ulPoolID = 0;
	m_sPoolSurfaceDescription.ulHeight = 2160;
	m_sPoolSurfaceDescription.ulWidth = 3840;
	m_sPoolSurfaceDescription.ePolarity = keNsPolarityProgressiveFrame;
	m_sPoolSurfaceDescription.ulComponentBitCount = 8;
	m_sPoolSurfaceDescription.eSurfaceFormat = keNsSurfaceFormatYUYV422;


	ZeroMemory(&m_sPoolSampleDescription, sizeof(m_sPoolSampleDescription));

	m_sPoolSampleDescription.iPoolID = 0;
	m_sPoolSampleDescription.ulCardID = 0;
	m_sPoolSampleDescription.eContentAllocationType = keNsdAVContentAllocationTypeNormal;
	m_sPoolSampleDescription.eMemoryUsage = keNsdMemoryUsageProcessing;
	m_sPoolSampleDescription.sAudioSampleDescription.bHasFadeIn = false;
	m_sPoolSampleDescription.sAudioSampleDescription.bHasFadeOut = false;
	m_sPoolSampleDescription.sAudioSampleDescription.eMemoryLocation = keNsMemoryLocationHost;
	m_sPoolSampleDescription.sAudioSampleDescription.ePolarity = keNsPolarityProgressiveFrame;
	m_sPoolSampleDescription.sAudioSampleDescription.sWaveFormat.eChannelType = keNsChannelTypeMono;
	m_sPoolSampleDescription.sAudioSampleDescription.sWaveFormat.eDataType = keNsAudioDataTypePCM;
	m_sPoolSampleDescription.sAudioSampleDescription.sWaveFormat.ulSamplesPerSec = 48000;
	m_sPoolSampleDescription.sAudioSampleDescription.sWaveFormat.ulBitsPerSample = 32;
	m_sPoolSampleDescription.sAudioSampleDescription.sWaveFormat.ulValidBitsPerSample = 16;

	m_sPoolSampleDescription.sAudioSampleDescription.ulMaxBufferSize = kulNsdAudioSamplesMaxBufferSize;
	
	m_ui64CompositerCount = 0;

}

CNxCompositorNode::~CNxCompositorNode()
{
}

HRESULT CNxCompositorNode::SetOutputFormat(ENsSurfaceFormat in_eSurfaceFormat)
{
	HRESULT hr = NS_NOERROR;
	m_eSurfaceFormatOutput = in_eSurfaceFormat;
	return hr;
}


int TransationProc(
	unsigned long ulWidth, unsigned long ulHeight,
	unsigned long ulSrcAPitch, BYTE* pSrcA,
	unsigned long ulSrcBPitch, BYTE* pSrcB,
	double        dAProgress,
	unsigned long ulDstPitch, BYTE* pDst
	)
{
	unsigned long ulALineSize = ulSrcAPitch;

	BYTE* pALine   = pSrcA;
	BYTE* pBLine   = pSrcB;
	BYTE* pDstLine = pDst;

	BYTE* pDstCurrent = pDstLine;

	BYTE* pA = pALine;
	BYTE* pB = pBLine;

	for (; ulHeight > 0; ulHeight--)
	{
		pDstCurrent = pDstLine;
		pA = pALine;
		pB = pBLine;

		while (pA < pALine + ulALineSize)
		{
			//loop 1

			__m128i src128Ai = _mm_load_si128((__m128i*)pA);
			__m128i src128Bi = _mm_load_si128((__m128i*)pB);

			__m128 src128As = _mm_cvtepi32_ps(src128Ai);
			__m128 src128Bs = _mm_cvtepi32_ps(src128Bi);

			pA = pA+ 16;
			pB = pB + 16;
			pDstCurrent = pDstCurrent + 16;
		}

		pALine += ulSrcAPitch;
		pBLine += ulSrcBPitch;

		pDstLine += ulDstPitch;
	}

	return 0;
};

HRESULT CNxCompositorNode::SetMixSchedule(double ul_offsetbegin, double ul_offsetend)
{
	HRESULT hr = NS_NOERROR;
	if (ul_offsetend <= ul_offsetbegin)
	{
		return NS_E_FAIL;
	}
	m_ulOffsetBegin = ul_offsetbegin;
	m_ulOffsetEnd = ul_offsetend;
	
	return hr;
}

HRESULT CNxCompositorNode::ThreadNodeProcessLoop()
{
	HRESULT hr = NS_NOERROR;
	try {
		bool bFirst = false;
		TNsSmartPtr<INsAVContent> *ppJAVContentInput = new TNsSmartPtr<INsAVContent>[2];

		double dProgress = 0.0;
		while (true)
		{

			WaitPauseEvent();
			//{

				//CNsAutoLock lock(&m_csSignalInputAVContents);
				//if (m_mapQueueSignalInputAVContents[0]->size() <= 0)
				//{
				//	Sleep(10);
				//	continue;
				//}

				//if (m_mapQueueSignalInputAVContents[1]->size() <= 0)
				//{
				//	Sleep(10);
				//	continue;
				//}
			//}


			for (int i = 0; i < 2; i++)
			{
				ppJAVContentInput[i] = m_mapQueueSignalInputAVContents[i]->front();
			}


			bool bAllReady = true;
			for (int i = 0; i < 2; i++)
			{
				if (ppJAVContentInput[i] == NULL)
					bAllReady = false;
			}


			if (!bAllReady)
				continue;
		
			if (dProgress >= 1.0)
			{
				dProgress = 1.0;
			}

			TNsSmartPtr<INsSurface> pJSurfaceInputA;
			TNsSmartPtr<INsSurface> pJSurfaceInputB;
			hr = ppJAVContentInput[0]->QueryInterface(IID_INsSurface, (void**)&pJSurfaceInputA);
			hr = ppJAVContentInput[1]->QueryInterface(IID_INsSurface, (void**)&pJSurfaceInputB);

			SNsSurfaceDescription sSurfaceDescriptionA;
			sSurfaceDescriptionA.size = sizeof(sSurfaceDescriptionA);
			hr = pJSurfaceInputA->GetSurfaceDescription(&sSurfaceDescriptionA);


			m_sPoolSurfaceDescription.ulHeight = sSurfaceDescriptionA.ulHeight;
			m_sPoolSurfaceDescription.ulWidth = sSurfaceDescriptionA.ulWidth;

			TNsSmartPtr<INsSurface> pJSurfaceOutput = NULL;
			hr = m_pJResourceManager->GetSurface(m_sPoolSurfaceDescription, &pJSurfaceOutput, INFINITE);

			if (hr == NS_E_OUT_OF_POOL_MEMORY)
			{
				NS_LOG_WSTRING_FORMAT(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, keMessage, (L"CNxCompositorNode::ThreadNodeProcessLoop NS_E_OUT_OF_POOL_MEMORY Failed!!"));

				continue;
			}
			SNsLockSurfaceDescription sSurfaceInputA;
			pJSurfaceInputA->Lock(0, keNsFaceTypeFront, &sSurfaceInputA);

			SNsLockSurfaceDescription sSurfaceInputB;
			pJSurfaceInputB->Lock(0, keNsFaceTypeFront, &sSurfaceInputB);

			SNsLockSurfaceDescription sSurfaceOutput;
			pJSurfaceOutput->Lock(0, keNsFaceTypeFront, &sSurfaceOutput);

			TNsSmartPtr<INsTransationParameter> pJTransationParameter = NULL;

			ppJAVContentInput[0]->GetCustomObject(&IID_INsTransationParameter, (IUnknown**)&pJTransationParameter);

			if (pJTransationParameter != NULL)
			{
				double dTransationParam = 0;
				pJTransationParameter->GetTransationParam(&dTransationParam);

				dProgress = dTransationParam;

				//NS_LOG_WSTRING_FORMAT(keLogPkgxGeneralNode, keLogPkgxGeneralNodeFuncGeneral, keMessage, (L"dTransationParam = %f", dProgress));

				hr = ppJAVContentInput[0]->DetachCustomObject(&IID_INsTransationParameter);
				NSD_SAFE_REPORT_ERROR(keLogPkgxGeneralNode, keLogPkgxGeneralNodeFuncGeneral, hr, " can not DetachCustomObject ", true);
			}

			double dOffsetB = (double) m_ulOffsetBegin / 100;
			double dOffsetE = (double)m_ulOffsetEnd / 100;

			double dAprogress = 1.0 - dOffsetB - (dOffsetE - dOffsetB)*dProgress;
			double dBprogress = dOffsetB + (dOffsetE - dOffsetB)*dProgress;

			unsigned long ulHeight = 2160;
			unsigned long ulWidth = 3840;

			//NS_LOG_WSTRING_FORMAT(keLogPkgxGeneralNode, keLogPkgxGeneralNodeFuncGeneral, keMessage, (L"sSurfaceInputA.uiWidth = %d,sSurfaceInputA.uiHight = %d", sSurfaceInputA.ulWidth, sSurfaceInputA.ulHeight));

			for (int h = 0; h < ulHeight; h++)
			{
				for (int x = 0; x < ulWidth; x++)
				{
					*((BYTE*)sSurfaceOutput.pBuffer + h*ulWidth*2 + 2 * x + 0) = *((BYTE*)sSurfaceInputA.pBuffer + h*sSurfaceDescriptionA.ulWidth * 2 + 2 * x + 0)*dAprogress + *((BYTE*)sSurfaceInputB.pBuffer + h*ulWidth * 2 + 2 * x + 0)*dBprogress;
					*((BYTE*)sSurfaceOutput.pBuffer + h*ulWidth * 2 + 2 * x + 1) = *((BYTE*)sSurfaceInputA.pBuffer + h*sSurfaceDescriptionA.ulWidth * 2 + 2 * x + 1)*dAprogress + *((BYTE*)sSurfaceInputB.pBuffer + h*ulWidth * 2 + 2 * x + 1)*dBprogress;
				}
			}

			//static int ncount = 0;
			//if (ncount == 20)
			//{
			//	FILE *pFile;
			//	fopen_s(&pFile, "g:\\c.yuv", "wb");
			//	fwrite(sSurfaceOutput.pBuffer, 1, 3840 * 2160 * 2, pFile);
			//	fclose(pFile);
			//}
			//ncount++;

			//if (m_ui64CompositerCount == 0)
			//{
			//	if (pFile != NULL)
			//	{
			//		fclose(pFile);
			//	}
			//	static int ncount = 0;
			//	char temp[200];
			//	sprintf_s(temp, "e:\\composited%.5d.yuv", ncount);
			//	fopen_s(&pFile, temp, "wb");
			//	ncount++;
			//}

			//if(pFile != NULL)
			//{
			//	fwrite(sSurfaceOutput.pBuffer, 1, 3840 * 2160, pFile);
			//}
			//m_ui64CompositerCount++;

			//if (m_ui64CompositerCount == 500)
			//	m_ui64CompositerCount = 0;

			pJSurfaceOutput->Unlock(0, keNsFaceTypeFront);
			pJSurfaceInputA->Unlock(0, keNsFaceTypeFront);
			pJSurfaceInputB->Unlock(0, keNsFaceTypeFront);

			pJSurfaceOutput->ChangeUserBufferDataSize(ulHeight*ulWidth*2);
			/*char temp[255];
			sprintf_s(temp, "xavc,CNxCompositorNode::ThreadNodeProcessLoop sSurfaceInputA.ulSlicePitchInBytes is %d", sSurfaceInputA.ulSlicePitchInBytes);
			OutputDebugStringA(temp);*/

			/*NS_LOG_WSTRING_FORMAT(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, keMessage, (L"sSurfaceInputA.ulSlicePitchInBytes = %d", sSurfaceInputA.ulSlicePitchInBytes));*/

			TNsSmartPtr<INsAVContent> pJAVContentOutput;
			pJSurfaceOutput->QueryInterface(IID_INsAVContent, (void**)&pJAVContentOutput);
			hr = SetOutputAVContent(pJAVContentOutput);
			NSD_SAFE_REPORT_ERROR(keLogPkgxCommonDecodeNode, 0, hr,
				"CNxCommonCodecNode::ThreadNodeProcessLoop,SetOutputAVContent Failed!", true);


			for (int i = 0; i < 2; i++)
			{
				ppJAVContentInput[i]->SignalReadCompletion();
			}

			dProgress +=1.0/50;

			for (int i = 0; i < 2; i++)
			{
				m_mapQueueSignalInputAVContents[i]->pop_front();
			}
		}

	}
	catch (...)
	{
		ASSERT(FALSE);
	}
	return hr;
}
