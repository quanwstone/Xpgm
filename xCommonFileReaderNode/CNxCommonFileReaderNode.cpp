// xFFSourceNode.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include "CNxCommonFileReaderNode.h"
#include <conio.h>

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

	
#ifdef __cplusplus
#if __cplusplus
}

#endif
#endif /* __cplusplus */ 

const unsigned long kulNsdAudioSamplesMaxBufferSize = 16016;

// This is the constructor of a class that has been exported.
// see xFFSourceNode.h for the class definition

CNxCommonFileReaderNode::CNxCommonFileReaderNode

(
	wchar_t *in_pwszNodeName,
	ENsTreeNodeType     in_eNsTreeNodeType,
	HRESULT            *out_phr,
	BOOL				bUseFFmpeg
	)
	:CNxBaseNode(in_pwszNodeName, in_eNsTreeNodeType, out_phr),
	m_oAttachedObjectAllocator(L"AttachedObject Allocator ", out_phr),
	m_oPoolManagerAttachedObject(L"Pool Manager for AttachedObject", 10, (INsPoolAllocator *)&m_oAttachedObjectAllocator)
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
	m_sPoolSampleDescription.sAudioSampleDescription.size = sizeof(SNsCreateAudioSamplesDescription);
	m_sPoolSampleDescription.sAudioSampleDescription.bHasFadeIn = false;
	m_sPoolSampleDescription.sAudioSampleDescription.bHasFadeOut = false;
	m_sPoolSampleDescription.sAudioSampleDescription.eMemoryLocation = keNsMemoryLocationHost;
	m_sPoolSampleDescription.sAudioSampleDescription.ePolarity = keNsPolarityProgressiveFrame;
	m_sPoolSampleDescription.sAudioSampleDescription.sWaveFormat.size = sizeof(SNsaWaveFormatInfo);
	m_sPoolSampleDescription.sAudioSampleDescription.sWaveFormat.eChannelType = keNsChannelTypeMono;
	m_sPoolSampleDescription.sAudioSampleDescription.sWaveFormat.eDataType = keNsAudioDataTypePCM;
	m_sPoolSampleDescription.sAudioSampleDescription.sWaveFormat.ulSamplesPerSec = 48000;
	m_sPoolSampleDescription.sAudioSampleDescription.sWaveFormat.ulBitsPerSample = 24;
	m_sPoolSampleDescription.sAudioSampleDescription.sWaveFormat.ulValidBitsPerSample = 16;

	m_sPoolSampleDescription.sAudioSampleDescription.ulMaxBufferSize = kulNsdAudioSamplesMaxBufferSize;
	
	m_hAddNewTaskEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hFinishedReadEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_HandleFrameStatus = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_ui64SendVideoCount = 0;
	m_ui64SendAudioCount = 0;


	m_poThreadOutput = NULL;
	m_nThreadParalleCount = 1;

	for (int i = 0; i < m_nThreadParalleCount; i++)
	{
		m_mapDecodeTask[i] = new CNxAVCDecodeTask();

	}

	m_pFile = NULL;
	m_ui64TaskIndex = 0;
	m_bColorBar = false;

	m_ui64VideoFirstFramePosition = 525332;
	
	m_mapAudioFirstSamplePosition[0] = 1786900;
	m_mapAudioFirstSamplePosition[1] = 1788948;
	m_mapAudioFirstSamplePosition[2] = 1790996;
	m_mapAudioFirstSamplePosition[3] = 1793044;
	m_mapAudioFirstSamplePosition[4] = 1795092;
	m_mapAudioFirstSamplePosition[5] = 1797140;
	m_mapAudioFirstSamplePosition[6] = 1799188;
	m_mapAudioFirstSamplePosition[7] = 1801236;

	m_bUseFFMPEGReader = bUseFFmpeg;
	
	_cprintf("m_bUseFFMPEGReader = %d\n",m_bUseFFMPEGReader);
	m_ui64VideoFrameSize = 1260032;
	m_ui64AudioSampleSize = 1920;
	m_ui64CurrentReadedPosition = 0;
}

CNxCommonFileReaderNode::~CNxCommonFileReaderNode()
{
	if (m_HandleFrameStatus != NULL) {
		CloseHandle(m_HandleFrameStatus);
	}
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
HRESULT CNxCommonFileReaderNode::SetFileName(char *in_pszFileName)
{
	HRESULT hr = NS_NOERROR;
	memcpy_s(m_szFileName, _MAX_PATH_R, in_pszFileName, _MAX_PATH_R);
	
	return hr;
}
HRESULT CNxCommonFileReaderNode::SetFileName2(const wchar_t *in_pwszFileName, const wchar_t *in_pwszCam)
{
	HRESULT hr = NS_NOERROR;
	
	memset(m_szCamName,0,256);
	m_bColorBar = false;
	int nLength = lstrlenW(in_pwszFileName);

	if (nLength < 3)
	{
		m_bColorBar = true;
		ConvertFromWCHARToTCHAR(L"C:\\ColorBar.mxf", m_szFileName, _MAX_PATH_R);
		ConvertFromWCHARToTCHAR(in_pwszCam,m_szCamName,256);
	}
	else
	{
		ConvertFromWCHARToTCHAR(in_pwszFileName, m_szFileName, _MAX_PATH_R);
	}
	//Set AVCDecode Info
	for (int i = 0; i < m_nThreadParalleCount; i++)
	{
		m_mapDecodeTask[i]->SetDrawCamInfo(m_bColorBar, m_szCamName);
	}
	//获取文件句柄
//	errno_t error = fopen_s(&m_pFile, m_szFileName, "rb");
	m_pFile = _fsopen(m_szFileName, "rb", _SH_DENYNO);
	if (m_pFile == NULL) {
		hr = NS_E_FAIL;
		NS_LOG_WSTRING_FORMAT(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, keMessage, (L" CNxCommonFileReaderNode::SetFileName2 Fopen_s Failed FileName= ,%S", m_szFileName));
	}
	return hr;
}


HRESULT CNxCommonFileReaderNode::SetReadTask(uint64_t in_ui64Start, uint64_t in_ui64End)
{
	m_ui64Start = in_ui64Start;
	m_ui64End = in_ui64End;
	m_ui64CurrentReadedPosition = m_ui64Start;

	m_bNewReadTask = true;

	SetEvent(m_HandleFrameStatus);

	SetEvent(m_hAddNewTaskEvent);

	ResetEvent(m_hFinishedReadEvent);

	return NS_NOERROR;
}

HRESULT CNxCommonFileReaderNode::SetTask(uint64_t in_ui64Start, uint64_t in_ui64End)
{
	m_ui64Start = in_ui64Start;
	m_ui64End = in_ui64End;
	m_ui64CurrentReadedPosition = m_ui64Start;

	m_bNewReadTask = true;

	SetEvent(m_HandleFrameStatus);

	SetEvent(m_hAddNewTaskEvent);
	
	ResetEvent(m_hFinishedReadEvent);

	return NS_NOERROR;
}


HRESULT CNxCommonFileReaderNode::SetOutputFormat(ENsSurfaceFormat in_eSurfaceFormat)
{
	HRESULT hr = NS_NOERROR;
	m_eSurfaceFormatOutput = in_eSurfaceFormat;

	
	return hr;

}


static int refcount = 0;

static int open_codec_context(int *stream_idx,
	AVCodecContext **dec_ctx, AVFormatContext *fmt_ctx, enum AVMediaType type)
{
	int ret, stream_index;
	AVStream *st;
	AVCodec *dec = NULL;
	AVDictionary *opts = NULL;

	ret = av_find_best_stream(fmt_ctx, type, -1, -1, NULL, 0);
	if (ret < 0) {
		//fprintf(stderr, "Could not find %s stream in input file '%s'\n",
		//	av_get_media_type_string(type), src_filename);
		return ret;
	}
	else {
		stream_index = ret;
		st = fmt_ctx->streams[stream_index];

		/* find decoder for the stream */
		dec = avcodec_find_decoder(st->codecpar->codec_id);
		if (!dec) {
			fprintf(stderr, "Failed to find %s codec\n",
				av_get_media_type_string(type));
			return AVERROR(EINVAL);
		}

		/* Allocate a codec context for the decoder */
		*dec_ctx = avcodec_alloc_context3(dec);
		if (!*dec_ctx) {
			fprintf(stderr, "Failed to allocate the %s codec context\n",
				av_get_media_type_string(type));
			return AVERROR(ENOMEM);
		}

		/* Copy codec parameters from input stream to output codec context */
		if ((ret = avcodec_parameters_to_context(*dec_ctx, st->codecpar)) < 0) {
			fprintf(stderr, "Failed to copy %s codec parameters to decoder context\n",
				av_get_media_type_string(type));
			return ret;
		}
		/* Init the decoders, with or without reference counting */
		av_dict_set(&opts, "refcounted_frames", refcount ? "1" : "0", 0);
		if ((ret = avcodec_open2(*dec_ctx, dec, &opts)) < 0) {
			fprintf(stderr, "Failed to open %s codec\n",
				av_get_media_type_string(type));
			return ret;
		}

		(*dec_ctx)->thread_count = 2;
		(*dec_ctx)->thread_type = FF_THREAD_FRAME;

		*stream_idx = stream_index;
	}

	return 0;
}


static int open_codec_context_audio(int *stream_idx,
	AVCodecContext **dec_ctx, AVFormatContext *fmt_ctx, enum AVMediaType type, int stream_src_idx)
{
	int ret, stream_index;
	AVStream *st;
	AVCodec *dec = NULL;
	AVDictionary *opts = NULL;

	ret = stream_src_idx;

	if (ret < 0) {
		//fprintf(stderr, "Could not find %s stream in input file '%s'\n",
		//	av_get_media_type_string(type), src_filename);
		return ret;
	}
	else {
		stream_index = ret;
		st = fmt_ctx->streams[stream_index];

		/* find decoder for the stream */
		dec = avcodec_find_decoder(st->codecpar->codec_id);
		if (!dec) {
			fprintf(stderr, "Failed to find %s codec\n",
				av_get_media_type_string(type));
			return AVERROR(EINVAL);
		}

		/* Allocate a codec context for the decoder */
		*dec_ctx = avcodec_alloc_context3(dec);
		if (!*dec_ctx) {
			fprintf(stderr, "Failed to allocate the %s codec context\n",
				av_get_media_type_string(type));
			return AVERROR(ENOMEM);
		}

		/* Copy codec parameters from input stream to output codec context */
		if ((ret = avcodec_parameters_to_context(*dec_ctx, st->codecpar)) < 0) {
			fprintf(stderr, "Failed to copy %s codec parameters to decoder context\n",
				av_get_media_type_string(type));
			return ret;
		}

		/* Init the decoders, with or without reference counting */
		av_dict_set(&opts, "refcounted_frames", refcount ? "1" : "0", 0);
		if ((ret = avcodec_open2(*dec_ctx, dec, &opts)) < 0) {
			fprintf(stderr, "Failed to open %s codec\n",
				av_get_media_type_string(type));
			return ret;
		}
		*stream_idx = stream_index;
	}

	return 0;
}

#define NS_UP_ALIGN(N, STEP)	(((N) + (STEP) - 1) / (STEP) * (STEP))

int CNxCommonFileReaderNode::YUV422Planer10bitLEToYUV4228bit(
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


int CNxCommonFileReaderNode::YUV422Planer8bitLEToYUV4228bit(
	unsigned long ulWidth, unsigned long ulHeight,
	unsigned long ulSrcYPitch, BYTE* pSrcY,
	unsigned long ulSrcUPitch, BYTE* pSrcU,
	unsigned long ulSrcVPitch, BYTE* pSrcV,
	unsigned long ulDstPitch, BYTE* pDst
	)
{
	unsigned long ulYLineSize = ulWidth;

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

			__m128i uv8BitL = _mm_unpacklo_epi8(uRAW, vRAW);

			__m128i uv8BitH = _mm_unpackhi_epi8(uRAW, vRAW);


			__m128i yuyv8BitLL = _mm_unpacklo_epi8(yRAW1, uv8BitL);

			__m128i yuyv8BitLH = _mm_unpackhi_epi8(yRAW1, uv8BitL);

			__m128i yuyv8BitHL = _mm_unpacklo_epi8(yRAW2, uv8BitH);
			__m128i yuyv8BitHH = _mm_unpackhi_epi8(yRAW2, uv8BitH);

			_mm_store_si128((__m128i*)pDstCurrent, yuyv8BitLL);
			_mm_store_si128(((__m128i*)pDstCurrent) + 1, yuyv8BitLH);
			_mm_store_si128(((__m128i*)pDstCurrent) + 2, yuyv8BitHL);
			_mm_store_si128(((__m128i*)pDstCurrent) + 3, yuyv8BitHH);

			pY = pY + 32;
			pV = pV + 16;
			pU = pU + 16;
			pDstCurrent = pDstCurrent + 64;
		}

		pYLine += ulSrcYPitch;
		pULine += ulSrcUPitch;
		pVLine += ulSrcVPitch;
		pDstLine += ulDstPitch;
	}

	return 0;
};

static int get_format_from_sample_fmt(const char **fmt,
enum AVSampleFormat sample_fmt)
{
	int i;
	struct sample_fmt_entry {
		enum AVSampleFormat sample_fmt; const char *fmt_be, *fmt_le;
	} sample_fmt_entries[] = {
		{ AV_SAMPLE_FMT_U8,  "u8",    "u8" },
		{ AV_SAMPLE_FMT_S16, "s16be", "s16le" },
		{ AV_SAMPLE_FMT_S32, "s32be", "s32le" },
		{ AV_SAMPLE_FMT_FLT, "f32be", "f32le" },
		{ AV_SAMPLE_FMT_DBL, "f64be", "f64le" },
	};
	*fmt = NULL;

	for (i = 0; i < FF_ARRAY_ELEMS(sample_fmt_entries); i++) {
		struct sample_fmt_entry *entry = &sample_fmt_entries[i];
		if (sample_fmt == entry->sample_fmt) {
			*fmt = AV_NE(entry->fmt_be, entry->fmt_le);
			return 0;
		}
	}

	fprintf(stderr,
		"sample format %s is not supported as output format\n",
		av_get_sample_fmt_name(sample_fmt));
	return -1;
}

HRESULT CNxCommonFileReaderNode::WaitReadTaskFinished()
{
	DWORD dwWaitCode = WaitForSingleObject(m_hFinishedReadEvent, INFINITE);
	return NS_NOERROR;
}

HRESULT CNxCommonFileReaderNode::SetReaderType(ENsCommonReaderType in_eCommonReaderType)
{
	m_eCommonReaderType = in_eCommonReaderType;
	return NS_NOERROR;
}

//only for keNsCommonReaderTypeAudio

HRESULT CNxCommonFileReaderNode::SetAudioStreamIndex(int in_nAudioStreamIndex)
{
	m_nAudioStreamIndex = in_nAudioStreamIndex;
	return NS_NOERROR;
}
bool CNxCommonFileReaderNode::JudgeReadFrameInFile(uint64_t read_size, int read_len)
{
	if (NULL == m_pFile) {
		NSD_SAFE_REPORT_ERROR(keLogPkgxGeneralNode, keLogPkgxGeneralNodeFuncGeneral, NS_E_FAIL,
			"CNxCommonFileReaderNode::JudgeReadFrameInFile,m_pFile == NULL!", true);
		return false;
	}
	uint64_t file_size = _fseeki64(m_pFile, 0, SEEK_END);
	file_size = _ftelli64(m_pFile);
	if (read_size + read_len >file_size) {
		//开始轮询
		while (read_size + read_len >file_size) {
			_fseeki64(m_pFile, 0, SEEK_END);
			file_size = _ftelli64(m_pFile);
			_cprintf("JudgeReadFrameInFile#file_size = %lld,read_size=%lld\r\n",file_size, read_size + read_len);
			Sleep(1000);
		}
	}
	return true;
}
HRESULT CNxCommonFileReaderNode::ThreadNodeSeekProcessLoop()
{
	HRESULT hr = NS_NOERROR;

	try {

		for (int i = 0; i < m_nThreadParalleCount; i++)
		{
			m_mapDecodeTask[i]->SetResourceManager(m_pJResourceManager);
			m_mapDecodeTask[i]->m_pDecoder = NULL;

		}


		if (m_poThreadOutput == NULL)
		{
			m_poThreadOutput = new CNsCOMThread;
			m_poThreadOutput->Create(_outputhread, this, false, CNsThread::ms_kuiDefaultStackSize, true);
		}


		AVFormatContext *fmt_ctx = NULL;
		AVCodecContext *video_dec_ctx = NULL;
		AVCodecContext *audio_dec_ctx = NULL;
		int width, height;
		enum AVPixelFormat pix_fmt;
		enum AVSampleFormat     sample_fmt;
		AVStream *video_stream = NULL;
		AVStream *audio_stream = NULL;

		int video_stream_idx = -1, audio_stream_idx = -1;

		int video_frame_count = 0;
		int audio_frame_count = 0;

		
		int ret = 0, got_frame;

		bool juge_flag = false;
		/* register all formats and codecs */
		av_register_all();

		/* initialize packet, set data to NULL, let the demuxer fill it */

		unsigned long ulSleep = 0;
		/* read frames from the file */

		uint64_t uiFileSize = 0;
		while (true)
		{
			WaitPauseEvent();
			DWORD dwWaitCode = WaitForSingleObject(m_hAddNewTaskEvent, INFINITE);
			if (m_bNewReadTask)
			{
				m_bNewReadTask = false;

				if (avformat_open_input(&fmt_ctx, m_szFileName, NULL, NULL) < 0) {
					fprintf(stderr, "Could not open source file %s\n", m_szFileName);
					exit(1);
				}

				/* retrieve stream information */
				if (avformat_find_stream_info(fmt_ctx, NULL) < 0) {
					fprintf(stderr, "Could not find stream information\n");
					exit(1);
				}

				if (m_eCommonReaderType == keNsCommonReaderTypeVideo)
				{

					for (int i = 0; i < m_nThreadParalleCount; i++)
					{

						AVCodecContext *video_dec_ctx = NULL;

						if (open_codec_context(&video_stream_idx, &video_dec_ctx, fmt_ctx, AVMEDIA_TYPE_VIDEO) >= 0) {
							video_stream = fmt_ctx->streams[video_stream_idx];
							width = video_dec_ctx->width;
							height = video_dec_ctx->height;
							pix_fmt = video_dec_ctx->pix_fmt;

							m_sPoolSurfaceDescription.ulHeight = height;
							m_sPoolSurfaceDescription.ulWidth = width;
						}
						else
						{
							video_dec_ctx = NULL;
						}

						if (m_mapDecodeTask[i]->m_pDecoder != NULL)
						{
							avcodec_free_context((AVCodecContext **)&(m_mapDecodeTask[i]->m_pDecoder));
						}
						m_mapDecodeTask[i]->m_pDecoder = video_dec_ctx;
					}

					if (!video_stream) {
						NSD_SAFE_REPORT_ERROR(keLogPkgxGeneralNode, keLogPkgxGeneralNodeFuncGeneral, NS_E_FAIL,
							"CNxCommonFileReaderNode::ThreadNodeProcessLoop,video_stream == NULL!", true);
						ret = 1;
						goto end;
					}

					if (video_stream)
					{
						uint64_t ui64SeekPos = m_ui64Start;

						if (m_bColorBar)
						{
							ui64SeekPos = 0;
						}


					}
				}

				if (m_eCommonReaderType == keNsCommonReaderTypeAudio)
				{
					//if (open_codec_context(&audio_stream_idx, &audio_dec_ctx, fmt_ctx, AVMEDIA_TYPE_AUDIO) >= 0) {
					if (open_codec_context_audio(&audio_stream_idx, &audio_dec_ctx, fmt_ctx, AVMEDIA_TYPE_AUDIO, m_nAudioStreamIndex) >= 0) {

						audio_stream_idx = m_nAudioStreamIndex;
						audio_stream = fmt_ctx->streams[audio_stream_idx];
						NS_LOG_WSTRING_FORMAT(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, keMessage, (L"Audio Reader  File stream count = %d", fmt_ctx->nb_streams));
						NS_LOG_WSTRING_FORMAT(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, keMessage, (L"Audio Reader  audio_stream_idx = %d", audio_stream_idx));

						sample_fmt = audio_dec_ctx->sample_fmt;
					}
					else
					{
						audio_dec_ctx = NULL;
					}

					if (!audio_stream) {
						NSD_SAFE_REPORT_ERROR(keLogPkgxGeneralNode, keLogPkgxGeneralNodeFuncGeneral, NS_E_FAIL,
							"CNxCommonFileSegmentReaderNode::ThreadNodeProcessLoop,audio_stream == NULL!", true);
						ret = 1;
						goto end;
					}
				}
			}

			bool bReadHit = false;

			while (true) {
	
				WaitForSingleObject(m_HandleFrameStatus, INFINITE);
				//video
				if (m_eCommonReaderType == keNsCommonReaderTypeVideo)
				{

					//18432 8 channel audio data size.
					uint64_t ui64PosAtFile = m_ui64VideoFirstFramePosition + m_ui64CurrentReadedPosition*(1260032 + 18432);

					if (m_bColorBar)
					{
						ui64PosAtFile = m_ui64VideoFirstFramePosition;
					}
					//判断文件大小是否包含需要读取的数据帧,线程挂起
					juge_flag = JudgeReadFrameInFile(ui64PosAtFile, 1260032);
					if (juge_flag == false) {//
						break;
					}
					int nResult = _fseeki64(m_pFile, ui64PosAtFile, SEEK_SET);

					if (nResult != 0)
					{
						NSD_SAFE_REPORT_ERROR(keLogPkgxCommonDecodeNode, 0, NS_E_FAIL,
							"CNxAVCFileSeekReaderNode::ThreadNodeProcessLoop,fseek Failed!", true);
					}

					TNsSmartPtr<INsSurface> pJSurface = NULL;

					hr = m_pJResourceManager->GetSurface(m_sPoolSurfaceDescription, &pJSurface, INFINITE);
					if (hr == NS_E_OUT_OF_POOL_MEMORY)
					{
						continue;
					}

					SNsLockSurfaceDescription sSurface;
					pJSurface->Lock(0, keNsFaceTypeFront, &sSurface);
					size_t readsize = fread_s(sSurface.pBuffer, m_ui64VideoFrameSize, 1, m_ui64VideoFrameSize, m_pFile);
					pJSurface->Unlock(0, keNsFaceTypeFront);

					TNsSmartPtr<INsAVContent> pJAVContent;
					pJSurface->QueryInterface(IID_INsAVContent, (void**)&pJAVContent);

					hr = pJSurface->ChangeFormat(m_eSurfaceFormatOutput);

					hr = pJSurface->ChangeUserBufferDataSize(m_ui64VideoFrameSize);

					if ((m_eSurfaceFormatOutput == keNsSurfaceFormatDNX_145_1080i)
						|| (m_eSurfaceFormatOutput == keNsSurfaceFormatDNX_HQ)
						|| (m_eSurfaceFormatOutput == keNsSurfaceFormatProRes_422_HQ)
						|| (m_eSurfaceFormatOutput == keNsSurfaceFormatAVC)
						)
					{
						NS_LOG_WSTRING_FORMAT(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, keMessage, (L"CNxCommonFileReaderNode::ThreadNodeProcessLoop read one frame"));
						hr = SetOutputAVContent(pJAVContent);
					}
					else if (m_eSurfaceFormatOutput == keNsSurfaceFormatYUY2/*keNsSurfaceFormatYUYV422*/)
					{//video
						m_mapDecodeTask[m_ui64TaskIndex%m_nThreadParalleCount]->DecodeFrame(pJAVContent);
						m_ui64TaskIndex++;
					}
				}

				//audio
				if (m_eCommonReaderType == keNsCommonReaderTypeAudio)
				{

					unsigned long  ulBitsPerSample = m_ui64AudioSampleSize / 960 * 8;

					TNsSmartPtr<INsAudioSamples> pJAudioSamples = NULL;
					m_sPoolSampleDescription.sAudioSampleDescription.sWaveFormat.ulBitsPerSample = ulBitsPerSample;
					m_sPoolSampleDescription.sAudioSampleDescription.sWaveFormat.ulValidBitsPerSample = ulBitsPerSample;
					hr = m_pJResourceManager->GetAudioSamples(m_sPoolSampleDescription, &pJAudioSamples, INFINITE);
					if (hr == NS_E_OUT_OF_POOL_MEMORY)
					{
						continue;
					}

					char *pAudioBuffer;
					unsigned long ulAudioBufferLength;
					pJAudioSamples->GetBufferAndLength((void**)&pAudioBuffer, &ulAudioBufferLength);

					uint64_t ui64PosAtFile = m_mapAudioFirstSamplePosition[m_nAudioStreamIndex-1] + m_ui64CurrentReadedPosition*(1920 + 1276544);

					if (m_bColorBar)
					{
						ui64PosAtFile = m_mapAudioFirstSamplePosition[m_nAudioStreamIndex - 1];
					}

					////判断文件大小是否包含需要读取的数据帧,线程挂起
					juge_flag = JudgeReadFrameInFile(ui64PosAtFile, 1920);
					if (juge_flag == false) {//
						break;
					}

					int nResult = _fseeki64(m_pFile, ui64PosAtFile, SEEK_SET);

					if (nResult != 0)
					{
						NSD_SAFE_REPORT_ERROR(keLogPkgxCommonDecodeNode, 0, NS_E_FAIL,
							"CNxAVCFileSeekReaderNode::ThreadNodeProcessLoop,fseek Failed!", true);
					}

					size_t readsize = fread_s(pAudioBuffer, m_ui64AudioSampleSize, 1, m_ui64AudioSampleSize, m_pFile);


					pJAudioSamples->SetLength(m_ui64AudioSampleSize);
					TNsSmartPtr<INsAVContent> pJAVContent;
					pJAudioSamples->QueryInterface(IID_INsAVContent, (void**)&pJAVContent);


					hr = SetOutputAVContent(pJAVContent);

					m_ui64SendAudioCount++;

				}

				m_ui64CurrentReadedPosition++;

				if (m_ui64CurrentReadedPosition == m_ui64End)
				{
					if (video_dec_ctx != NULL)
						avcodec_free_context(&video_dec_ctx);

					if (audio_dec_ctx != NULL)
						avcodec_free_context(&audio_dec_ctx);

					if (fmt_ctx != NULL)
						avformat_close_input(&fmt_ctx);

					ResetEvent(m_hAddNewTaskEvent);
					SetEvent(m_hFinishedReadEvent);

					break;
				}
			}
		}
	end:
		if (video_dec_ctx != NULL)
			avcodec_free_context(&video_dec_ctx);

		if (audio_dec_ctx != NULL)
			avcodec_free_context(&audio_dec_ctx);

	}

	catch (...)
	{
		ASSERT(FALSE);
	}
	return hr;
}

HRESULT CNxCommonFileReaderNode::ThreadNodeProcessLoop()
{
	HRESULT hr = NS_NOERROR;

	if (!m_bUseFFMPEGReader)
	{
		ThreadNodeSeekProcessLoop();
		return hr;
	}
	try {

		for (int i = 0; i < m_nThreadParalleCount; i++)
		{
			m_mapDecodeTask[i]->SetResourceManager(m_pJResourceManager);
			m_mapDecodeTask[i]->m_pDecoder = NULL;

		}


		if (m_poThreadOutput == NULL)
		{
			m_poThreadOutput = new CNsCOMThread;
			m_poThreadOutput->Create(_outputhread, this, false, CNsThread::ms_kuiDefaultStackSize, true);
		}


		AVFormatContext *fmt_ctx = NULL;
		AVCodecContext *video_dec_ctx = NULL;
		AVCodecContext *audio_dec_ctx = NULL;
		int width, height;
		enum AVPixelFormat pix_fmt;
		enum AVSampleFormat     sample_fmt;
		AVStream *video_stream = NULL;
		AVStream *audio_stream = NULL;
		const char *src_filename = NULL;
		const char *video_dst_filename = NULL;
		const char *audio_dst_filename = NULL;
		FILE *video_dst_file = NULL;
		FILE *audio_dst_file = NULL;


		int      video_dst_linesize[4];
		int video_dst_bufsize;

		int video_stream_idx = -1, audio_stream_idx = -1;
		AVFrame *frame = NULL;
		AVPacket pkt;
		int video_frame_count = 0;
		int audio_frame_count = 0;



		int ret = 0, got_frame;
		//src_filename = "T:\Capture0008.mov";
		//src_filename = "T:\Capture0008.mxf";
		//src_filename = "T:\Capture0008_100.mxf";

		video_dst_filename = "v.yuv";
		audio_dst_filename = "a.pcm";


		//memcpy_s(m_szFileName, _MAX_PATH, src_filename, _MAX_PATH);


		/* register all formats and codecs */
		av_register_all();

		/* initialize packet, set data to NULL, let the demuxer fill it */
		av_init_packet(&pkt);
		pkt.data = NULL;
		pkt.size = 0;


		unsigned long ulSleep = 0;
		/* read frames from the file */

		frame = av_frame_alloc();
		if (!frame) {
			fprintf(stderr, "Could not allocate frame\n");
			ret = AVERROR(ENOMEM);
			goto end;
		}

		uint64_t uiFileSize = 0;
		while (true)
		{
			WaitPauseEvent();
			DWORD dwWaitCode = WaitForSingleObject(m_hAddNewTaskEvent, INFINITE);

			if (m_bNewReadTask)
			{
				m_bNewReadTask = false;
				
				if (avformat_open_input(&fmt_ctx, m_szFileName, NULL, NULL) < 0) {
					fprintf(stderr, "Could not open source file %s\n", m_szFileName);
					exit(1);
				}

				/* retrieve stream information */
				if (avformat_find_stream_info(fmt_ctx, NULL) < 0) {
					fprintf(stderr, "Could not find stream information\n");
					exit(1);
				}

				if (m_eCommonReaderType == keNsCommonReaderTypeVideo)
				{

					for (int i = 0; i < m_nThreadParalleCount; i++)
					{

						AVCodecContext *video_dec_ctx = NULL;

						if (open_codec_context(&video_stream_idx, &video_dec_ctx, fmt_ctx, AVMEDIA_TYPE_VIDEO) >= 0) {
							video_stream = fmt_ctx->streams[video_stream_idx];
							width = video_dec_ctx->width;
							height = video_dec_ctx->height;
							pix_fmt = video_dec_ctx->pix_fmt;

							m_sPoolSurfaceDescription.ulHeight = height;
							m_sPoolSurfaceDescription.ulWidth = width;
						}
						else
						{
							video_dec_ctx = NULL;
						}

						if (m_mapDecodeTask[i]->m_pDecoder != NULL)
						{
							avcodec_free_context((AVCodecContext **)&(m_mapDecodeTask[i]->m_pDecoder));
						}
						m_mapDecodeTask[i]->m_pDecoder = video_dec_ctx;
					}

					if (!video_stream) {
						NSD_SAFE_REPORT_ERROR(keLogPkgxGeneralNode, keLogPkgxGeneralNodeFuncGeneral, NS_E_FAIL,
							"CNxCommonFileReaderNode::ThreadNodeProcessLoop,video_stream == NULL!", true);
						ret = 1;
						goto end;
					}

					if (video_stream)
					{
						uint64_t ui64SeekPos = m_ui64Start;

						if (m_bColorBar)
						{
							ui64SeekPos = 0;
						}
						ret = av_seek_frame(fmt_ctx, video_stream_idx, ui64SeekPos,AVSEEK_FLAG_FRAME);
						if (ret < 0)
						{
							ASSERT(0);
						}


					}
				}

				if (m_eCommonReaderType == keNsCommonReaderTypeAudio)
				{
					//if (open_codec_context(&audio_stream_idx, &audio_dec_ctx, fmt_ctx, AVMEDIA_TYPE_AUDIO) >= 0) {
					if (open_codec_context_audio(&audio_stream_idx, &audio_dec_ctx, fmt_ctx, AVMEDIA_TYPE_AUDIO, m_nAudioStreamIndex) >= 0) {


						audio_stream_idx = m_nAudioStreamIndex;
						audio_stream = fmt_ctx->streams[audio_stream_idx];
						NS_LOG_WSTRING_FORMAT(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, keMessage, (L"Audio Reader  File stream count = %d", fmt_ctx->nb_streams));
						NS_LOG_WSTRING_FORMAT(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, keMessage, (L"Audio Reader  audio_stream_idx = %d", audio_stream_idx));

						sample_fmt = audio_dec_ctx->sample_fmt;
					}
					else
					{
						audio_dec_ctx = NULL;
					}

					if (!audio_stream) {
						NSD_SAFE_REPORT_ERROR(keLogPkgxGeneralNode, keLogPkgxGeneralNodeFuncGeneral, NS_E_FAIL,
							"CNxCommonFileSegmentReaderNode::ThreadNodeProcessLoop,audio_stream == NULL!", true);
						ret = 1;
						goto end;
					}

					if (audio_stream)
					{

					}

					// this code for seek by video,because the audio dont seek!
					if (open_codec_context(&video_stream_idx, &video_dec_ctx, fmt_ctx, AVMEDIA_TYPE_VIDEO) >= 0) {
						video_stream = fmt_ctx->streams[video_stream_idx];

					}
					else
					{
						video_dec_ctx = NULL;
					}

					if (!video_stream) {
						NSD_SAFE_REPORT_ERROR(keLogPkgxGeneralNode, keLogPkgxGeneralNodeFuncGeneral, NS_E_FAIL,
							"CNxCommonFileSegmentReaderNode::ThreadNodeProcessLoop,video_stream == NULL!", true);
						ret = 1;
						goto end;
					}

					if (video_stream)
					{

						uint64_t ui64SeekPos = m_ui64Start;

						if (m_bColorBar)
						{
							ui64SeekPos = 0;
						}

						ret = av_seek_frame(fmt_ctx, video_stream_idx, ui64SeekPos, AVSEEK_FLAG_FRAME);
						if (ret < 0)
						{
							ASSERT(0);
						}
					}

					video_stream_idx = -1;
				}
			}

			pkt.data = NULL;
			pkt.size = 0;

			bool bReadHit = false;


			while (av_read_frame(fmt_ctx, &pkt) >= 0) {
				bReadHit = false;
				char temp[200];
				sprintf_s(temp, "xSolution,CNxCommonFileReaderNode::ThreadNodeProcessLoop  pkt.stream_index = %d", pkt.stream_index);
				OutputDebugString(temp);

				WaitForSingleObject(m_HandleFrameStatus, INFINITE);
				
				if ((m_eSurfaceFormatOutput == keNsSurfaceFormatDNX_145_1080i)
					|| (m_eSurfaceFormatOutput == keNsSurfaceFormatDNX_HQ)
					|| (m_eSurfaceFormatOutput == keNsSurfaceFormatProRes_422_HQ)
					|| (m_eSurfaceFormatOutput == keNsSurfaceFormatAVC)
					)
				{
					NS_LOG_WSTRING_FORMAT(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, keMessage, (L"CNxCommonFileReaderNode::ThreadNodeProcessLoop read one frame"));

					if (pkt.stream_index == video_stream_idx)
					{
						bReadHit = true;

						TNsSmartPtr<INsSurface> pJSurface = NULL;

						hr = m_pJResourceManager->GetSurface(m_sPoolSurfaceDescription, &pJSurface, INFINITE);
						if (hr == NS_E_OUT_OF_POOL_MEMORY)
						{
							continue;
						}

						SNsLockSurfaceDescription sSurface;

						pJSurface->Lock(0, keNsFaceTypeFront, &sSurface);
						memcpy(sSurface.pBuffer, pkt.data, pkt.size);
						pJSurface->Unlock(0, keNsFaceTypeFront);

						TNsSmartPtr<INsAVContent> pJAVContent;
						pJSurface->QueryInterface(IID_INsAVContent, (void**)&pJAVContent);

						hr = pJSurface->ChangeFormat(m_eSurfaceFormatOutput);

						hr = pJSurface->ChangeUserBufferDataSize(pkt.size);

						hr = SetOutputAVContent(pJAVContent);

		
						if (m_bColorBar)
						{
							uint64_t ui64SeekPos = 0;
							ret = av_seek_frame(fmt_ctx, video_stream_idx, ui64SeekPos, AVSEEK_FLAG_FRAME);
						}

						
						//m_ui64SendVideoCount++;

					}
					ret = pkt.size;
				}
				else if (pkt.stream_index == video_stream_idx)
				{//video
					NS_LOG_WSTRING_FORMAT(keLogPkgxTransCodeATL, keLogPkgxTransCodeATLFuncGeneral, keMessage, (L"CNxCommonFileReaderNode::ThreadNodeProcessLoop read one frame"));


					bReadHit = true;


					TNsSmartPtr<INsSurface> pJSurface = NULL;

					hr = m_pJResourceManager->GetSurface(m_sPoolSurfaceDescription, &pJSurface, INFINITE);
					if (hr == NS_E_OUT_OF_POOL_MEMORY)
					{
						continue;
					}

					SNsLockSurfaceDescription sSurface;

					pJSurface->Lock(0, keNsFaceTypeFront, &sSurface);
					memcpy(sSurface.pBuffer, pkt.data, pkt.size);
					pJSurface->Unlock(0, keNsFaceTypeFront);

					TNsSmartPtr<INsAVContent> pJAVContent;
					pJSurface->QueryInterface(IID_INsAVContent, (void**)&pJAVContent);

					hr = pJSurface->ChangeFormat(m_eSurfaceFormatOutput);

					hr = pJSurface->ChangeUserBufferDataSize(pkt.size);

					m_mapDecodeTask[m_ui64TaskIndex%m_nThreadParalleCount]->DecodeFrame(pJAVContent);
					m_ui64TaskIndex++;

					ret = pkt.size;

					if (m_bColorBar)
					{
						uint64_t ui64SeekPos = 0;
						ret = av_seek_frame(fmt_ctx, video_stream_idx, ui64SeekPos, AVSEEK_FLAG_FRAME);
					}

				}
				else if (pkt.stream_index == audio_stream_idx)
				{//audio
				 /* decode audio frame */


					uiFileSize += pkt.size;

					bReadHit = true;
					ret = avcodec_send_packet(audio_dec_ctx, &pkt);
					if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF)
					{
						av_packet_unref(&pkt);
						return FALSE;
					}
					//从解码器返回解码输出数据  
					ret = avcodec_receive_frame(audio_dec_ctx, frame);
					if (ret < 0 && ret != AVERROR_EOF)
					{
						av_packet_unref(&pkt);
						return FALSE;
					}
					else
					{

						//size_t unpadded_linesize = frame->nb_samples * av_get_bytes_per_sample((AVSampleFormat)frame->format);

						//if (unpadded_linesize != 3840)
						//{

						//	pkt.size = 0;
						//	pkt.data = NULL;
						//	continue;
						//}

						//int nAudioSamplesPerVideoUnit = 48000 / 50;


						//int nVideoUnitsPerAudioFrame = frame->nb_samples / nAudioSamplesPerVideoUnit;

						//for (int i = 0; i < nVideoUnitsPerAudioFrame; i++)
						//{
						//	TNsSmartPtr<INsAudioSamples> pJAudioSamples = NULL;

						//	if (AV_SAMPLE_FMT_S16 == sample_fmt)
						//	{
						//		m_sPoolSampleDescription.sAudioSampleDescription.sWaveFormat.ulBitsPerSample = 16;
						//		m_sPoolSampleDescription.sAudioSampleDescription.sWaveFormat.ulValidBitsPerSample = 16;
						//		hr = m_pJResourceManager->GetAudioSamples(m_sPoolSampleDescription, &pJAudioSamples, 0);
						//	}
						//	else if (AV_SAMPLE_FMT_S32 == sample_fmt)
						//	{
						//		m_sPoolSampleDescription.sAudioSampleDescription.sWaveFormat.ulBitsPerSample = 32;
						//		m_sPoolSampleDescription.sAudioSampleDescription.sWaveFormat.ulValidBitsPerSample = 32;

						//		hr = m_pJResourceManager->GetAudioSamples(m_sPoolSampleDescription, &pJAudioSamples, 0);
						//	}
						//	else
						//	{
						//		hr = m_pJResourceManager->GetAudioSamples(m_sPoolSampleDescription, &pJAudioSamples, 0);
						//	}

						//	if (hr == NS_E_OUT_OF_POOL_MEMORY)
						//	{
						//		continue;
						//	}

						//	char *pAudioBuffer;
						//	unsigned long ulAudioBufferLength;
						//	pJAudioSamples->GetBufferAndLength((void**)&pAudioBuffer, &ulAudioBufferLength);

						//	//if (unpadded_linesize > ulAudioBufferLength)
						//	//{
						//	//	continue;
						//	//}

						//	//unpadded_linesize = 3840;
						//	memcpy(pAudioBuffer, frame->extended_data[0] + i*ulAudioBufferLength, ulAudioBufferLength);

						//	pJAudioSamples->SetLength(ulAudioBufferLength);

						//	TNsSmartPtr<INsAVContent> pJAVContent;
						//	pJAudioSamples->QueryInterface(IID_INsAVContent, (void**)&pJAVContent);

						//	hr = SetOutputAVContent(pJAVContent);
						//	m_ui64CurrentReadedPosition++;

						//	if (m_ui64End == m_ui64CurrentReadedPosition)
						//	{
						//		break;
						//	}
						//}


						//m_ui64CurrentReadedPosition = m_ui64CurrentReadedPosition - 1;

						//ret = pkt.size;


						size_t unpadded_linesize = frame->nb_samples * av_get_bytes_per_sample((AVSampleFormat)frame->format);


						TNsSmartPtr<INsAudioSamples> pJAudioSamples = NULL;

						if (AV_SAMPLE_FMT_S16 == sample_fmt)
						{
							m_sPoolSampleDescription.sAudioSampleDescription.sWaveFormat.ulBitsPerSample = 16;
							m_sPoolSampleDescription.sAudioSampleDescription.sWaveFormat.ulValidBitsPerSample = 16;
							hr = m_pJResourceManager->GetAudioSamples(m_sPoolSampleDescription, &pJAudioSamples, 0);
						}
						else if (AV_SAMPLE_FMT_S32 == sample_fmt)
						{
							m_sPoolSampleDescription.sAudioSampleDescription.sWaveFormat.ulBitsPerSample = 32;
							m_sPoolSampleDescription.sAudioSampleDescription.sWaveFormat.ulValidBitsPerSample = 32;

							hr = m_pJResourceManager->GetAudioSamples(m_sPoolSampleDescription, &pJAudioSamples, INFINITE);
						}
						else
						{
							hr = m_pJResourceManager->GetAudioSamples(m_sPoolSampleDescription, &pJAudioSamples, INFINITE);
						}

						if (hr == NS_E_OUT_OF_POOL_MEMORY)
						{
							continue;
						}

						char *pAudioBuffer;
						unsigned long ulAudioBufferLength;
						pJAudioSamples->GetBufferAndLength((void**)&pAudioBuffer, &ulAudioBufferLength);

						memcpy(pAudioBuffer, frame->extended_data[0], unpadded_linesize);

						pJAudioSamples->SetLength(unpadded_linesize);

						TNsSmartPtr<INsAVContent> pJAVContent;
						pJAudioSamples->QueryInterface(IID_INsAVContent, (void**)&pJAVContent);

						hr = SetOutputAVContent(pJAVContent);

						m_ui64SendAudioCount++;

						ret = pkt.size;

						if (m_bColorBar)
						{
							uint64_t ui64SeekPos = 0;
							ret = av_seek_frame(fmt_ctx, 0, ui64SeekPos, AVSEEK_FLAG_FRAME);
						}


					}

				}

				ret = pkt.size;

				if (ret < 0)
					break;

				pkt.data += ret;
				pkt.size -= ret;
				av_packet_free_side_data(&pkt);
				av_packet_unref(&pkt);

				if (bReadHit)
				{
					m_ui64CurrentReadedPosition++;

					if (m_ui64CurrentReadedPosition == m_ui64End)
					{
						if (video_dec_ctx != NULL)
							avcodec_free_context(&video_dec_ctx);

						if (audio_dec_ctx != NULL)
							avcodec_free_context(&audio_dec_ctx);

						if (fmt_ctx != NULL)
							avformat_close_input(&fmt_ctx);

						ResetEvent(m_hAddNewTaskEvent);

						SetEvent(m_hFinishedReadEvent);
						break;
					}
				}

			}

		}

		/* flush cached frames */
		av_packet_unref(&pkt);
		pkt.data = NULL;
		pkt.size = 0;

		if (video_stream) {
			printf("Play the output video file with the command:\n"
				"ffplay -f rawvideo -pix_fmt %s -video_size %dx%d %s\n",
				av_get_pix_fmt_name(pix_fmt), width, height,
				video_dst_filename);
		}

		if (audio_stream) {
			enum AVSampleFormat sfmt = audio_dec_ctx->sample_fmt;
			int n_channels = audio_dec_ctx->channels;
			const char *fmt;

			if (av_sample_fmt_is_planar(sfmt)) {
				const char *packed = av_get_sample_fmt_name(sfmt);
				printf("Warning: the sample format the decoder produced is planar "
					"(%s). This example will output the first channel only.\n",
					packed ? packed : "?");
				sfmt = av_get_packed_sample_fmt(sfmt);
				n_channels = 1;
			}

			if ((ret = get_format_from_sample_fmt(&fmt, sfmt)) < 0)
				goto end;

			printf("Play the output audio file with the command:\n"
				"ffplay -f %s -ac %d -ar %d %s\n",
				fmt, n_channels, audio_dec_ctx->sample_rate,
				audio_dst_filename);
		}

	end:
		if (video_dec_ctx != NULL)
			avcodec_free_context(&video_dec_ctx);

		if (audio_dec_ctx != NULL)
			avcodec_free_context(&audio_dec_ctx);

		//if (fmt_ctx != NULL)
		//	avformat_close_input(&fmt_ctx);

		if (video_dst_file)
			fclose(video_dst_file);
		if (audio_dst_file)
			fclose(audio_dst_file);
		av_frame_free(&frame);

	}
	catch (...)
	{
		ASSERT(FALSE);
	}
	return hr;
}


HRESULT CNxCommonFileReaderNode::GetAttachedObject(INsTransationParameter **out_ppAttachedObject)
{
	HRESULT hr = NOERROR;

	TNsSmartPtr<IUnknown> pJUnk;
	TNsSmartPtr<INsTransationParameter> pJNsAttachedObject;

	hr = m_oPoolManagerAttachedObject.GetElement(&pJUnk);
	NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgxGeneralNode, keLogPkgxGeneralNodeFuncGeneral,
		hr, "CNxCommonFileReaderNode::GetAttachedObject : Failed get an element .", true);

	// Get custom object interface
	hr = pJUnk->QueryInterface(IID_INsTransationParameter, (void **)(&pJNsAttachedObject));
	NSD_SAFE_REPORT_ERROR_RETURN(keLogPkgxGeneralNode, keLogPkgxGeneralNodeFuncGeneral,
		hr, "CNxCommonFileReaderNode::GetAttachedObject : Failed to query IID_INsdAttachedAudioSamples interface", true);

	*out_ppAttachedObject = pJNsAttachedObject;
	(*out_ppAttachedObject)->AddRef();

	return hr;
}

HRESULT CNxCommonFileReaderNode::GetFileDurationInFrames(char *in_pszFileName, uint64_t *out_pui64Frames)
{
	HRESULT hr = NS_NOERROR;

	*out_pui64Frames = 0;

	AVFormatContext *fmt_ctx = NULL;
	AVCodecContext *video_dec_ctx = NULL;
	AVCodecContext *audio_dec_ctx = NULL;
	int width, height;
	enum AVPixelFormat pix_fmt;
	enum AVSampleFormat     sample_fmt;
	AVStream *video_stream = NULL;
	AVStream *audio_stream = NULL;
	const char *src_filename = NULL;
	const char *video_dst_filename = NULL;
	const char *audio_dst_filename = NULL;
	FILE *video_dst_file = NULL;
	FILE *audio_dst_file = NULL;

	
	int      video_dst_linesize[4];
	int video_dst_bufsize;

	int video_stream_idx = -1, audio_stream_idx = -1;
	AVFrame *frame = NULL;
	AVPacket pkt;
	int video_frame_count = 0;
	int audio_frame_count = 0;



	int ret = 0, got_frame;




	/* register all formats and codecs */
	av_register_all();


	if (avformat_open_input(&fmt_ctx, in_pszFileName, NULL, NULL) < 0) {
		fprintf(stderr, "Could not open source file %s\n", m_szFileName);
		return hr;
	}

	/* retrieve stream information */
	if (avformat_find_stream_info(fmt_ctx, NULL) < 0) {
		fprintf(stderr, "Could not find stream information\n");
		return hr;
	}

	if (open_codec_context(&video_stream_idx, &video_dec_ctx, fmt_ctx, AVMEDIA_TYPE_VIDEO) >= 0) {
		video_stream = fmt_ctx->streams[video_stream_idx];

		*out_pui64Frames = video_stream->duration;
	}

	if (video_dec_ctx != NULL)
		avcodec_free_context(&video_dec_ctx);

	if (fmt_ctx != NULL)
		avformat_close_input(&fmt_ctx);


	return hr;
}

HRESULT CNxCommonFileReaderNode::GetCurrentPositionInFrames(uint64_t *out_pui64CurrentPosition)
{
	HRESULT hr = NS_NOERROR;
	uint64_t uiCurrentReadedPositionTemp = 0;
	
	uiCurrentReadedPositionTemp = m_ui64CurrentReadedPosition;

	if (uiCurrentReadedPositionTemp < m_ui64Start) {
		uiCurrentReadedPositionTemp = m_ui64Start;
	}
	*out_pui64CurrentPosition = uiCurrentReadedPositionTemp - m_ui64Start;
	
	return hr;
}
HRESULT CNxCommonFileReaderNode::SetFramesPraseStatus(BOOL in_bStatus)
{
	HRESULT hr = NS_NOERROR;
	if (in_bStatus == TRUE) {//Stop
		if(m_HandleFrameStatus != NULL)
			ResetEvent(m_HandleFrameStatus);
		
	}
	else {//Continue
		if (m_HandleFrameStatus != NULL)
			SetEvent(m_HandleFrameStatus);
	}
	return hr;
}

unsigned int CNxCommonFileReaderNode::_outputhread(void* in_pThreadParameter)
{
	CNxCommonFileReaderNode *pThis = (CNxCommonFileReaderNode*)in_pThreadParameter;
	try
	{
		pThis->ThreadOutputLoop();
	}
	catch (...)
	{
		NSD_SAFE_REPORT_ERROR(keLogPkgxTree, keLogPkgxTreeFuncGeneric, NS_E_FAIL,
			"CNxAVCDecodeTask::_taskthread: -> ThreadNodeProcessLoop() - Unhandled exception!", true);
	}

	return 0;
}

HRESULT CNxCommonFileReaderNode::ThreadOutputLoop()
{
	HRESULT hr = NS_NOERROR;

	uint64_t ui64TempReadedPosition = 0;
	try {

		m_ui64OutputIndex = 0;
		while (true)
		{
			TNsSmartPtr<INsAVContent> pJAVContent = m_mapDecodeTask[m_ui64OutputIndex%m_nThreadParalleCount]->GetDecodeFrame();

			TNsSmartPtr<INsTransationParameter> pIAttachedObject = NULL;
			hr = GetAttachedObject(&pIAttachedObject);
			NSD_SAFE_REPORT_ERROR(keLogPkgxGeneralNode, keLogPkgxGeneralNodeFuncGeneral, hr,
				"CNxCommonFileReaderNode::ThreadNodeProcessLoop,GetAttachedObject Failed!", true);
			
			double	dTransationParam = (double)ui64TempReadedPosition/*(m_ui64CurrentReadedPosition - m_ui64Start)*/ / (m_ui64End - m_ui64Start -1);

			pIAttachedObject->SetTransationParam(dTransationParam);

			hr = pJAVContent->AttachCustomObject(&IID_INsTransationParameter, (IUnknown *)pIAttachedObject);
			NSD_SAFE_REPORT_ERROR(keLogPkgxGeneralNode, keLogPkgxGeneralNodeFuncGeneral, hr,
				"CNxCommonFileReaderNode::ThreadNodeProcessLoop,AttachCustomObject Failed!", true);

			SetOutputAVContent(pJAVContent);

			m_ui64OutputIndex++;
			ui64TempReadedPosition++;

			if (ui64TempReadedPosition == (m_ui64End - m_ui64Start)) {
				ui64TempReadedPosition = 0;
			}
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
	}
	return hr;
}
