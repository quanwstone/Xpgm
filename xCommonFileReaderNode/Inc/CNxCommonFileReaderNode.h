#pragma once

#ifdef XCOMMONFILEREADERNODE_EXPORTS
#define XCOMMONFILEREADERNODE_API __declspec(dllexport)
#else
#define XCOMMONFILEREADERNODE_API __declspec(dllimport)
#endif

#define _MAX_PATH_R 2048
#include "xAPI.h"
#include "xLogger.h"
#include "CNxBaseNode.h"
#include "CNsTransationParameter.h"
// This class is exported from the XCOMMONFILEREADERNODE.dll
#include "CNxAVCDecodeTask.h"

struct SNxFileCache
{
	char szFileName[2048];
	void * fmt_ctx;
};
class XCOMMONFILEREADERNODE_API CNxCommonFileReaderNode : public CNxBaseNode
{
public:
	CNxCommonFileReaderNode(
		wchar_t *in_pwszNodeName,
		ENsTreeNodeType     in_eNsTreeNodeType,
		HRESULT            *out_phr,
		BOOL				bUseFFmpeg);


	/** Destructor of the class. *
	<b>Remark:</b><ul><li>None</ul>
	*/

	virtual ~CNxCommonFileReaderNode();

	virtual HRESULT __cdecl  ThreadNodeProcessLoop();

public:
	HRESULT SetFileName(char *in_pszFileName);
	HRESULT SetFileName2(const wchar_t *in_pwszFileName,const wchar_t *in_pwszCam=L"");
	HRESULT SetReaderType(ENsCommonReaderType in_eCommonReaderType);
	
	//only for keNsCommonReaderTypeAudio
	HRESULT SetAudioStreamIndex(int in_nAudioStreamIndex);

	HRESULT SetReadTask(uint64_t in_ui64Start, uint64_t in_ui64End);
	HRESULT SetTask(uint64_t in_ui64Start, uint64_t in_ui64End);
	HRESULT SetOutputFormat(ENsSurfaceFormat in_eSurfaceFormat);


	HRESULT WaitReadTaskFinished();
	HRESULT GetAttachedObject(INsTransationParameter **out_ppAttachedObject);

	HRESULT GetFileDurationInFrames(char *in_pszFileName,uint64_t *out_pui64Frames);
	HRESULT GetCurrentPositionInFrames(uint64_t *out_pui64CurrentPosition);
	
	HRESULT SetFramesPraseStatus(BOOL in_bStatus);

	HRESULT __cdecl  ThreadOutputLoop();
protected:

	int YUV422Planer10bitLEToYUV4228bit(
		unsigned long ulWidth, unsigned long ulHeight,
		unsigned long ulSrcYPitch, BYTE* pSrcY,
		unsigned long ulSrcUPitch, BYTE* pSrcU,
		unsigned long ulSrcVPitch, BYTE* pSrcV,
		unsigned long ulDstPitch, BYTE* pDst
	);
	int YUV422Planer8bitLEToYUV4228bit(
		unsigned long ulWidth, unsigned long ulHeight,
		unsigned long ulSrcYPitch, BYTE* pSrcY,
		unsigned long ulSrcUPitch, BYTE* pSrcU,
		unsigned long ulSrcVPitch, BYTE* pSrcV,
		unsigned long ulDstPitch, BYTE* pDst
	);

	HRESULT ThreadNodeSeekProcessLoop();

private:
	bool  JudgeReadFrameInFile(uint64_t read_size,int read_len);//判断读取的字节是否存在于文件中
	SNsdPoolSurfaceDescription m_sPoolSurfaceDescription;
	SNsdPoolSampleDescription m_sPoolSampleDescription;
	char m_szFileName[_MAX_PATH_R];
	char m_szCamName[256];
	ENsSurfaceFormat m_eSurfaceFormatOutput;

	uint64_t m_ui64Start;
	uint64_t m_ui64End;

	uint64_t m_ui64CurrentReadedPosition;
	volatile bool m_bNewReadTask;
	HANDLE    m_hAddNewTaskEvent;
	HANDLE    m_hFinishedReadEvent;
	ENsCommonReaderType m_eCommonReaderType;
	int m_nAudioStreamIndex;
		
	CNsdPoolElementAllocator<CNsTransationParameter> m_oAttachedObjectAllocator;
	CNsPoolManager m_oPoolManagerAttachedObject;

	uint64_t m_ui64SendVideoCount;
	uint64_t m_ui64SendAudioCount;
	std::vector<SNxFileCache*> m_vecFileCache;
	//CNsCriticalSection m_csCriticalSection;

	std::map<unsigned long, CNxAVCDecodeTask*, std::less<unsigned long> > m_mapDecodeTask;

	int m_nThreadParalleCount;
	uint64_t m_ui64OutputIndex;
	CNsCOMThread* m_poThreadOutput;
	uint64_t m_ui64TaskIndex;
	bool m_bColorBar;

	uint64_t m_ui64VideoFirstFramePosition;
	std::map<int, uint64_t> m_mapAudioFirstSamplePosition;
	//read file
	FILE *m_pFile;

	uint64_t m_ui64VideoFrameSize;
	uint64_t m_ui64AudioSampleSize;

	bool m_bUseFFMPEGReader;
	HANDLE  m_HandleFrameStatus;
public:
	static unsigned int __stdcall _outputhread(void* in_pThreadParameter);
};
