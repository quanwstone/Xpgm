// xFFSourceNode.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "CNxAVCDecodeTask.h"
#include"songproperty.h"
#include"xLogger.h"
#include <iostream>

#ifdef __cplusplus
#if __cplusplus

extern "C" {
#endif
#endif /* __cplusplus */ 
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
#include <libavutil/imgutils.h>
#include <libavutil/samplefmt.h>
#include <libavutil/timestamp.h>
#include <libavutil/error.h>
#include <libavformat/avformat.h>
#include<libswscale/swscale.h>//

#ifdef __cplusplus
#if __cplusplus
}

#endif
#endif /* __cplusplus */ 

//using namespace Gdiplus;
//GdiplusStartupInput g_gdiplusStartupInput;
//ULONG_PTR g_gdiplusToken;

CNxAVCDecodeTask::CNxAVCDecodeTask()
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

	m_ui64DecodeSendFrameCount = 0;

	m_ui64ReceiveFrameCountFromDecode = 0;

	m_poThread = NULL;
	m_ui64Input = 0;
	m_ui64Output = 0;

	m_bDrawCamFlag = false;
//	GdiplusStartup(&g_gdiplusToken, &g_gdiplusStartupInput, NULL);
}

CNxAVCDecodeTask::~CNxAVCDecodeTask()
{
}

HRESULT CNxAVCDecodeTask::SetTask(uint64_t in_ui64Start, uint64_t in_ui64End)
{
	m_ui64Start = in_ui64Start;
	m_ui64End = in_ui64End;

	m_ui64ProcessFrameCount = in_ui64Start;
	m_ui64ReceiveFrameCountFromDecode = in_ui64Start;
	ResetEvent(m_hFinishedReadEvent);
	return NS_NOERROR;
}


#define NS_UP_ALIGN(N, STEP)	(((N) + (STEP) - 1) / (STEP) * (STEP))

int YUV422Planer10bitLEToYUV4228bit(
	unsigned long ulWidth, unsigned long ulHeight,
	unsigned long ulSrcYPitch, BYTE* pSrcY,
	unsigned long ulSrcUPitch, BYTE* pSrcU,
	unsigned long ulSrcVPitch, BYTE* pSrcV,
	unsigned long ulDstPitch, BYTE* pDst
	)
{
	unsigned long ulYLineSize = NS_UP_ALIGN(ulWidth, 32) * 2;

	BYTE* pYLine = pSrcY;
	BYTE* pULine = pSrcU;
	BYTE* pVLine = pSrcV;
	BYTE* pDstLine = pDst;


	BYTE* pDstCurrent = pDstLine;
	BYTE* Output4 = pYLine;
	BYTE* pY = pYLine;
	BYTE* pU = pULine;
	BYTE* pV = pVLine;

	for (; ulHeight > 0; ulHeight--)
	{
		pDstCurrent = pDstLine;
		pY = pYLine;
		pU = pULine;
		pV = pVLine;

		while (pY < pYLine + ulYLineSize)
		{
			//loop 1
			__m128i yRAW1 = _mm_load_si128((__m128i*)pY);
			__m128i yRAW2 = _mm_load_si128(((__m128i*)pY) + 1);
			__m128i uRAW = _mm_load_si128((__m128i*)pU);
			__m128i vRAW = _mm_load_si128((__m128i*)pV);

			__m128i y8BitLoose1 = _mm_srli_epi16(yRAW1, 2);							//Y0  00  Y1  00  Y2  00  Y3  00  Y4  00  Y5  00  Y6  00  Y7  00
			__m128i y8BitLoose2 = _mm_srli_epi16(yRAW2, 2);							//Y8  00  Y9  00  Y10 00  Y11 00  Y12 00  Y13 00  Y14 00  Y15 00
			__m128i y8Bit = _mm_packus_epi16(y8BitLoose1, y8BitLoose2);					//Y0  Y1  Y2  Y3  Y4  Y5  Y6  Y7  Y8  Y9  Y10 Y11 Y12 Y13 Y14 Y15

			__m128i u8BitLoose = _mm_srli_epi16(uRAW, 2);								//U0  00  U2  00  U4  00  U6  00  U8  00  U10 00  U12 00  U14 00
			__m128i v8BitLoose = _mm_srli_epi16(vRAW, 2);								//V1  00  V3  00  V5  00  V7  00  V9  00  V11 00  V13 00  V15 00

			v8BitLoose = _mm_slli_si128(v8BitLoose, 1);									//00  V1  00  V3  00  V5  00  V7  00  V9  00  V11 00  V13 00  V15
			__m128i uv8Bit = _mm_or_si128(v8BitLoose, u8BitLoose);						//U0  V1  U2  V3  U4  V5  U6  V7  U8  V9  U10 V11 U12 V13 U14 V15

			__m128i yuv8Bit1 = _mm_unpacklo_epi8(y8Bit, uv8Bit);							//Y0  U0  Y1  V1  Y2  U2  Y3  V3  Y4  U4  Y5  V5  Y6  U6  Y7  V7
			__m128i yuv8Bit2 = _mm_unpackhi_epi8(y8Bit, uv8Bit);							//Y8  U8  Y9  V9  Y10 Y10 Y11 V11 Y12 U12 Y13 V13 Y14 U14 Y15 V15
			
			_mm_store_si128((__m128i*)pDstCurrent, yuv8Bit1);
			_mm_store_si128(((__m128i*)pDstCurrent) + 1, yuv8Bit2);

			pY = pY + 32;
			pV = pV + 16;
			pU = pU + 16;
			pDstCurrent = pDstCurrent + 32;
		}

		pYLine += ulSrcYPitch;
		pULine += ulSrcUPitch;
		pVLine += ulSrcVPitch;
		pDstLine += ulDstPitch;
	}

	return 0;


};
//Test_WriteBMP
void CNxAVCDecodeTask::ConstructBih(int nWidth, int nHeight, BITMAPINFOHEADER& bih)
{
	int widthStep = (((nWidth * 24) + 31) & (~31)) / 8;

	bih.biSize = 40;       // header size  
	bih.biWidth = nWidth;
	bih.biHeight = -nHeight;
	bih.biPlanes = 1;
	bih.biBitCount = 24;     // RGB encoded, 24 bit  
	bih.biCompression = BI_RGB;   // no compression 非压缩  
	bih.biSizeImage = widthStep*nHeight * 3;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;
}
void CNxAVCDecodeTask::ContructBhh(int nWidth, int nHeight, BITMAPFILEHEADER& bhh)
{
	int widthStep = (((nWidth * 24) + 31) & (~31)) / 8; //每行实际占用的大小（每行都被填充到一个4字节边界）  
	bhh.bfType = ((WORD)('M' << 8) | 'B');  //'BM'  
	bhh.bfSize = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + widthStep * nHeight;
	bhh.bfReserved1 = 0;
	bhh.bfReserved2 = 0;
	bhh.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);
}
void  CNxAVCDecodeTask::SetDrawCamInfo(bool bColorBar, char *chCam)
{
	memset(m_chCamName,0,256);
	m_bDrawCamFlag = bColorBar;
	memcpy(m_chCamName, chCam, strlen(chCam) + 1);
}
HRESULT CNxAVCDecodeTask::ThreadNodeProcessLoop()
{
	HRESULT hr = NS_NOERROR;


	SNxSongProperty sSongProperty;
	//sSongProperty.strFontFace = sSongProperty.vFonts[1]->strFaceName;
	sSongProperty.Red = 0;
	sSongProperty.Green = 0;
	sSongProperty.Blue = 255;
	//sSongProperty.dFontSize = 0.3;
	//sSongProperty.dPositionX = 0.1;
	//sSongProperty.dPositionY = 0.1;
	//sSongProperty.nOutlineWeight = 0;

	// TODO: 在此添加控件通知处理程序代码
	unsigned char *pRGB;
	pRGB = new unsigned char[3840 * 2160 * 4];
	memset(pRGB, 0, 3840 * 2160 * 4);

	try {
		AVFrame *frame = NULL;
		AVPacket pkt;

		AVFrame *pFrameYUVtoRGB;
		pFrameYUVtoRGB = av_frame_alloc(); //初始化存储对象

		bool bFirst = false;

		av_init_packet(&pkt);
		pkt.data = NULL;
		pkt.size = 0;
		frame = av_frame_alloc();
		if (!frame) {
			fprintf(stderr, "Could not allocate frame\n");

		}
		//Test_WriteBMP
		//FILE *pFile;
		//BITMAPINFOHEADER bih;
		//ConstructBih(3840, 2160, bih);
		//BITMAPFILEHEADER bhh;
		//ContructBhh(3840, 2160, bhh);
		//char buf[256] = {0};
		//int num = 0;
		AVFrame *pFrameRgbTemp = av_frame_alloc();
		AVFrame *pFrameYUV = av_frame_alloc();

		struct SwsContext *img_convert_ctx = sws_getContext(3840, 2160,
			AV_PIX_FMT_YUV422P10LE, 3840, 2160,
			AV_PIX_FMT_BGR24, SWS_BICUBIC, NULL, NULL, NULL);

		unsigned char *out_buffer;							  //内存分配 计算出输出内存大小
		out_buffer = (unsigned char *)av_malloc(
			av_image_get_buffer_size(AV_PIX_FMT_BGR24, 3840, 2160, 1));
		
		av_image_fill_arrays(pFrameYUVtoRGB->data, pFrameYUVtoRGB->linesize, out_buffer,
			AV_PIX_FMT_BGR24, 3840, 2160,1);

		struct SwsContext *img_convert_yuv = sws_getContext(3840, 2160,
			AV_PIX_FMT_BGR24, 3840, 2160,
			AV_PIX_FMT_YUV422P10LE, SWS_BICUBIC, NULL, NULL, NULL);

		unsigned char *out_buffer_yuv = (unsigned char *)av_malloc(
			av_image_get_buffer_size(AV_PIX_FMT_YUV422P10LE, 3840,
				2160, 1));

		av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, out_buffer_yuv,
			AV_PIX_FMT_YUV422P10LE, 3840, 2160, 1);
		//
		while (true)
		{

			TNsSmartPtr<INsAVContent> pJAVContentInput = m_mapQueueSignalThreadInputAVContents[0]->front();
			m_mapQueueSignalThreadInputAVContents[0]->pop_front();

			TNsSmartPtr<INsSurface>  pJSurfaceInput;

			hr = pJAVContentInput->QueryInterface(IID_INsSurface, (void**)&pJSurfaceInput);

			AVCodecContext *video_dec_ctx = (AVCodecContext *)m_pDecoder;
			//
			SNsLockSurfaceDescription sSurfaceInput;
			pJSurfaceInput->Lock(0, keNsFaceTypeFront, &sSurfaceInput);

			pkt.data = (uint8_t*)sSurfaceInput.pBuffer;
			pkt.size = sSurfaceInput.ulSlicePitchInBytes;
			pkt.duration = 1;
			pkt.pts = 0;
			pkt.dts = 0;
			pkt.flags = 1;
			int ret = avcodec_send_packet(video_dec_ctx, &pkt);
			if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF)
			{
				NS_LOG_WSTRING_FORMAT(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, keMessage, (L"CNxCommonFileReaderNode::ThreadNodeProcessLoop avcodec_send_packet Failed!!"));
				//av_packet_unref(&pkt);

				continue;
			}

			//从解码器返回解码输出数据  
			ret = avcodec_receive_frame(video_dec_ctx, frame);
			//av_packet_unref(&pkt);
			if (ret < 0 && ret != AVERROR_EOF)
			{
				av_packet_unref(&pkt);
				NS_LOG_WSTRING_FORMAT(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, keMessage, (L"CNxCommonFileReaderNode::ThreadNodeProcessLoop avcodec_receive_frame Failed!!"));

				return FALSE;
			}
			else
			{
				TNsSmartPtr<INsSurface> pJSurface = NULL;
				hr = m_pJResourceManager->GetSurface(m_sPoolSurfaceDescription, &pJSurface, INFINITE);

				if (hr == NS_E_OUT_OF_POOL_MEMORY)
				{
					Sleep(1000);
					NS_LOG_WSTRING_FORMAT(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, keMessage, (L"CNxCommonFileReaderNode::ThreadNodeProcessLoop NS_E_OUT_OF_POOL_MEMORY Failed!!"));

					//ASSERT(FALSE);
					continue;
				}

				SNsLockSurfaceDescription sSurface;

				pJSurface->Lock(0, keNsFaceTypeFront, &sSurface);
				int width = 3840;
				int height = 2160;
				if (m_bDrawCamFlag) {//true DrawCamFlag
					//yuv->RGB
					sws_scale(img_convert_ctx,
						(const uint8_t* const *)frame->data, frame->linesize,
						0, video_dec_ctx->height, pFrameYUVtoRGB->data,
						pFrameYUVtoRGB->linesize);//转成rgb
					 //Draw RGB
					sSongProperty.DrawCG(3840, 2160, m_chCamName, (void**)pFrameYUVtoRGB->data[0], pRGB);
					//sprintf_s(buf, "%s%d%s", "d:\\quanwei\\bmp\\", num, ".bmp");
					//fopen_s(&pFile, buf, "wb");//
					//fwrite((LPSTR)&bhh, 1, sizeof(BITMAPFILEHEADER), pFile);//bmp
					//fwrite((LPSTR)&bih, 1, sizeof(BITMAPINFOHEADER), pFile);
					//fwrite(pRGB,1,3840*2160*3,pFile);									 //
					//fclose(pFile);
					//num++;
					//RGB->yuv
					pFrameRgbTemp->data[0] = (uint8_t*)pRGB;
					pFrameRgbTemp->linesize[0] = 3840 * 3;
					sws_scale(img_convert_yuv,
						(const uint8_t* const *)pFrameRgbTemp->data, pFrameRgbTemp->linesize,
						0, 2160, pFrameYUV->data,
						pFrameYUV->linesize);

					YUV422Planer10bitLEToYUV4228bit(width, height,
						pFrameYUV->linesize[0], pFrameYUV->data[0],
						pFrameYUV->linesize[1], pFrameYUV->data[1],
						pFrameYUV->linesize[2], pFrameYUV->data[2],
						width * 2, (BYTE*)sSurface.pBuffer
						);
				}
				else {//no need DrawCam
					YUV422Planer10bitLEToYUV4228bit(width, height,
						frame->linesize[0], frame->data[0],
						frame->linesize[1], frame->data[1],
						frame->linesize[2], frame->data[2],
						width * 2, (BYTE*)sSurface.pBuffer
						);
				}
				
				//av_frame_unref(frame);

				pJSurface->ChangeUserBufferDataSize(width*height * 2);

				pJSurface->ChangeFormat(keNsSurfaceFormatYUYV422);


				pJSurface->Unlock(0, keNsFaceTypeFront);

				TNsSmartPtr<INsAVContent> pJAVContent;
				pJSurface->QueryInterface(IID_INsAVContent, (void**)&pJAVContent);


				m_mapQueueSignalThreadOutputAVContents[0]->push_back(pJAVContent);
				
			}
			pJSurfaceInput->Unlock(0, keNsFaceTypeFront);
			av_packet_unref(&pkt);
			av_frame_unref(frame);
			pJAVContentInput->SignalReadCompletion();
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
	}
	return hr;
}


HRESULT CNxAVCDecodeTask::NodifyFrame(void* in_pDecode)
{
	HRESULT hr = NS_NOERROR;


	return  hr;
}


unsigned int CNxAVCDecodeTask::_taskthread(void* in_pThreadParameter)
{
	CNxAVCDecodeTask *pThis = (CNxAVCDecodeTask*)in_pThreadParameter;
	try
	{
		pThis->ThreadNodeProcessLoop();
	}
	catch (...)
	{
		NSD_SAFE_REPORT_ERROR(keLogPkgxTree, keLogPkgxTreeFuncGeneric, NS_E_FAIL,
			"CNxAVCDecodeTask::_taskthread: -> ThreadNodeProcessLoop() - Unhandled exception!", true);
	}

	return 0;
}

HRESULT CNxAVCDecodeTask::SetResourceManager(INsdResourceManager *in_pResourceManager)
{
	m_pJResourceManager = in_pResourceManager;
	m_mapQueueSignalThreadInputAVContents[0] = new CNsSignalQueue(10);
	m_mapQueueSignalThreadOutputAVContents[0] = new CNsSignalQueue(10);
	if (m_poThread == NULL)
	{
		m_poThread = new CNsCOMThread;
		m_poThread->Create(_taskthread, this, false, CNsThread::ms_kuiDefaultStackSize, true);
	}

	return NS_NOERROR;
}
TNsSmartPtr<INsAVContent> CNxAVCDecodeTask::GetDecodeFrame()
{
	TNsSmartPtr<INsAVContent>  pJAVContent = m_mapQueueSignalThreadOutputAVContents[0]->front();
	m_mapQueueSignalThreadOutputAVContents[0]->pop_front();
	m_ui64Output++;
	return pJAVContent;
}

HRESULT CNxAVCDecodeTask::DecodeFrame(INsAVContent *in_pAVContent)
{
	m_mapQueueSignalThreadInputAVContents[0]->push_back(in_pAVContent);
	m_ui64Input ++;
	

	return NS_NOERROR;
}
