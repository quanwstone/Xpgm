
// Test_xTransCodeNodeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Test_xTransCodeNode.h"
#include "Test_xTransCodeNodeDlg.h"
#include "afxdialogex.h"
//#include <conio.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
	
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTest_xTransCodeNodeDlg �Ի���



CTest_xTransCodeNodeDlg::CTest_xTransCodeNodeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TEST_xTransCodeNODE_DIALOG, pParent)
{
//	AllocConsole();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pVideoReaderNode = NULL;
}

void CTest_xTransCodeNodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_LiveWindow, m_LiveWindow);
	DDX_Control(pDX, IDC_CHECK2, m_CheckButton);
}

BEGIN_MESSAGE_MAP(CTest_xTransCodeNodeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CTest_xTransCodeNodeDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_Finish, &CTest_xTransCodeNodeDlg::OnBnClickedButtonFinish)
	ON_BN_CLICKED(IDC_BUTTON_SELECTED_OUTPUT_FILE, &CTest_xTransCodeNodeDlg::OnBnClickedButtonSelectedOutputFile)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_INPUT, &CTest_xTransCodeNodeDlg::OnBnClickedButtonSelectInput)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CTest_xTransCodeNodeDlg::OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_INPUT_AUDIO, &CTest_xTransCodeNodeDlg::OnBnClickedButtonSelectInputAudio)
	ON_BN_CLICKED(IDCANCEL, &CTest_xTransCodeNodeDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_MIX_INPUT, &CTest_xTransCodeNodeDlg::OnBnClickedButtonSelectMixInput)
	ON_BN_CLICKED(IDC_BUTTON2_Current, &CTest_xTransCodeNodeDlg::OnBnClickedButton2Current)
	ON_BN_CLICKED(IDC_BUTTON3_Stop, &CTest_xTransCodeNodeDlg::OnBnClickedButton3Stop)
END_MESSAGE_MAP()

DWORD WINAPI FuncNext(void *param) {
	
	CoInitialize(NULL);
	CTest_xTransCodeNodeDlg *dlg = (CTest_xTransCodeNodeDlg*)param;

	int nTrimIn = 0;
	int nTrimOut = 0;

	CString strTrimIn;
	dlg->GetDlgItem(IDC_EDIT_TRIMIN)->GetWindowText(strTrimIn);
	nTrimIn = atoi(strTrimIn);
	CString strTrimOut;
	dlg->GetDlgItem(IDC_EDIT_TRIMOUT)->GetWindowText(strTrimOut);
	nTrimOut = atoi(strTrimOut);
	CStringArray arryAudios;
	CString strOutputFile;
	dlg->GetDlgItem(IDC_EDIT_OUT_FILE)->GetWindowText(strOutputFile);

	CString strInputFileVideo;
	dlg->GetDlgItem(IDC_EDIT_INPUT_VIDEO_FILE)->GetWindowText(strInputFileVideo);

	CString strInputMixFileVideo;
	dlg->GetDlgItem(IDC_EDIT_INPUT_MIX_VIDEO_FILE)->GetWindowText(strInputMixFileVideo);

	CString strInputCam;
	dlg->GetDlgItem(IDC_EDIT1_Cam)->GetWindowText(strInputCam);

	CString strInputFileAudio1;
	dlg->GetDlgItem(IDC_EDIT_INPUT_AUDIO1_FILE)->GetWindowText(strInputFileAudio1);

	if (!strInputFileAudio1.IsEmpty())
	{
		arryAudios.Add(strInputFileAudio1);
	}

	CString strInputFileAudio2;
	dlg->GetDlgItem(IDC_EDIT_INPUT_AUDIO2_FILE)->GetWindowText(strInputFileAudio2);

	if (!strInputFileAudio2.IsEmpty())
	{
		arryAudios.Add(strInputFileAudio2);
	}


	CString strInputFileAudio3;
	dlg->GetDlgItem(IDC_EDIT_INPUT_AUDIO3_FILE)->GetWindowText(strInputFileAudio3);

	if (!strInputFileAudio3.IsEmpty())
	{
		arryAudios.Add(strInputFileAudio3);
	}

	CString strInputFileAudio4;
	dlg->GetDlgItem(IDC_EDIT_INPUT_AUDIO4_FILE)->GetWindowText(strInputFileAudio4);

	if (!strInputFileAudio4.IsEmpty())
	{
		arryAudios.Add(strInputFileAudio4);
	}


	CString strInputFileAudio5;
	dlg->GetDlgItem(IDC_EDIT_INPUT_AUDIO5_FILE)->GetWindowText(strInputFileAudio5);

	if (!strInputFileAudio5.IsEmpty())
	{
		arryAudios.Add(strInputFileAudio5);
	}

	CString strInputFileAudio6;
	dlg->GetDlgItem(IDC_EDIT_INPUT_AUDIO6_FILE)->GetWindowText(strInputFileAudio6);

	if (!strInputFileAudio6.IsEmpty())
	{
		arryAudios.Add(strInputFileAudio6);
	}

	CString strInputFileAudio7;
	dlg->GetDlgItem(IDC_EDIT_INPUT_AUDIO7_FILE)->GetWindowText(strInputFileAudio7);

	if (!strInputFileAudio7.IsEmpty())
	{
		arryAudios.Add(strInputFileAudio7);
	}

	CString strInputFileAudio8;
	dlg->GetDlgItem(IDC_EDIT_INPUT_AUDIO8_FILE)->GetWindowText(strInputFileAudio8);


	if (!strInputFileAudio8.IsEmpty())
	{
		arryAudios.Add(strInputFileAudio8);
	}

	HRESULT hr = NS_NOERROR;
	hr = dlg->m_pTransCode->TransCode_Begin(strOutputFile.AllocSysString());

	hr = dlg->m_pTransCode->TransElement_Begin();
	NSD_SAFE_REPORT_ERROR(keLogPkgTest, 0, hr,
		"CTest_xTransCodeNodeDlg::OnBnClickedButtonStart,TransElement_Begin Failed!", true);

	CString strMixOffsetB, strMixOffsetE;
	dlg->GetDlgItem(IDC_EDIT2_Offset_B)->GetWindowText(strMixOffsetB);
	dlg->GetDlgItem(IDC_EDIT3_Offset_E)->GetWindowText(strMixOffsetE);
	double offsetB = atof(strMixOffsetB.GetBuffer());
	double offsetE = atof(strMixOffsetE.GetBuffer());

	if (dlg->IsDlgButtonChecked(IDC_CHECK_MIX) == BST_CHECKED)
	{

		CString strMixTrimIn;
		dlg->GetDlgItem(IDC_EDIT_MIXV_TRIMIN)->GetWindowText(strMixTrimIn);
		CString strMixTrimOut;
		dlg->GetDlgItem(IDC_EDIT_MIXV_TRIMOUT)->GetWindowText(strMixTrimOut);
		int nMixTrimIn = atoi(strMixTrimIn.GetBuffer());
		int nMixTrimOut = atoi(strMixTrimOut.GetBuffer());
		if (nMixTrimOut < nMixTrimIn)
		{
			AfxMessageBox("�����������Mix In�����С��Out�㣡");
			ASSERT(0);
		}

		hr = dlg->m_pTransCode->TransElement_AddVideoFile(strInputMixFileVideo.AllocSysString(), nMixTrimIn, nMixTrimOut, strInputCam.AllocSysString());
		NSD_SAFE_REPORT_ERROR(keLogPkgTest, 0, hr,
			"CTest_xTransCodeNodeDlg::OnBnClickedButtonStart,TransElement_AddVideoFile Failed!", true);
	}

	hr = dlg->m_pTransCode->TransElement_AddVideoFile(strInputFileVideo.AllocSysString(), nTrimIn, nTrimOut,strInputCam.AllocSysString());
	NSD_SAFE_REPORT_ERROR(keLogPkgTest, 0, hr,
		"CTest_xTransCodeNodeDlg::OnBnClickedButtonStart,TransElement_AddVideoFile Failed!", true);
	dlg->m_pTransCode->TransElement_SetMixSchedule(offsetB, offsetE);

	for (int i = 0; i < arryAudios.GetCount(); i++)
	{
		hr = dlg->m_pTransCode->TransElement_AddAudioFile(arryAudios[i].AllocSysString(), nTrimIn, nTrimOut);
		NSD_SAFE_REPORT_ERROR(keLogPkgTest, 0, hr,
			"CTest_xTransCodeNodeDlg::OnBnClickedButtonStart,TransElement_AddAudioFile Failed!", true);
	}

	int iCheck = dlg->m_CheckButton.GetCheck();
	hr = dlg->m_pTransCode->TransElement_End(iCheck);
	NSD_SAFE_REPORT_ERROR(keLogPkgTest, 0, hr,
		"CTest_xTransCodeNodeDlg::OnBnClickedButtonStart,TransElement_End Failed!", true);
	CoUninitialize();
	hr = dlg->m_pTransCode->TransCode_End();
	AfxMessageBox("Fineshed.");
	return 0;
}
DWORD WINAPI Func(void *param)
{
	CoInitialize(NULL);
	CTest_xTransCodeNodeDlg *dlg = (CTest_xTransCodeNodeDlg*)param;
	HRESULT hr;
	CStringArray arryAudios;
	CString strOutputFile;
	dlg->GetDlgItem(IDC_EDIT_OUT_FILE)->GetWindowText(strOutputFile);

	CString strInputFileVideo;
	dlg->GetDlgItem(IDC_EDIT_INPUT_VIDEO_FILE)->GetWindowText(strInputFileVideo);
	CString strInputMixFileVideo;
	dlg->GetDlgItem(IDC_EDIT_INPUT_MIX_VIDEO_FILE)->GetWindowText(strInputMixFileVideo);

	CString strInputFileAudio1;
	dlg->GetDlgItem(IDC_EDIT_INPUT_AUDIO1_FILE)->GetWindowText(strInputFileAudio1);
	CString strInputCam;
	dlg->GetDlgItem(IDC_EDIT1_Cam)->GetWindowTextA(strInputCam);
	if (!strInputFileAudio1.IsEmpty())
	{
		arryAudios.Add(strInputFileAudio1);
	}

	CString strInputFileAudio2;
	dlg->GetDlgItem(IDC_EDIT_INPUT_AUDIO2_FILE)->GetWindowText(strInputFileAudio2);

	if (!strInputFileAudio2.IsEmpty())
	{
		arryAudios.Add(strInputFileAudio2);
	}


	CString strInputFileAudio3;
	dlg->GetDlgItem(IDC_EDIT_INPUT_AUDIO3_FILE)->GetWindowText(strInputFileAudio3);

	if (!strInputFileAudio3.IsEmpty())
	{
		arryAudios.Add(strInputFileAudio3);
	}

	CString strInputFileAudio4;
	dlg->GetDlgItem(IDC_EDIT_INPUT_AUDIO4_FILE)->GetWindowText(strInputFileAudio4);

	if (!strInputFileAudio4.IsEmpty())
	{
		arryAudios.Add(strInputFileAudio4);
	}

	CString strInputFileAudio5;
	dlg->GetDlgItem(IDC_EDIT_INPUT_AUDIO5_FILE)->GetWindowText(strInputFileAudio5);

	if (!strInputFileAudio5.IsEmpty())
	{
		arryAudios.Add(strInputFileAudio5);
	}

	CString strInputFileAudio6;
	dlg->GetDlgItem(IDC_EDIT_INPUT_AUDIO6_FILE)->GetWindowText(strInputFileAudio6);

	if (!strInputFileAudio6.IsEmpty())
	{
		arryAudios.Add(strInputFileAudio6);
	}

	CString strInputFileAudio7;
	dlg->GetDlgItem(IDC_EDIT_INPUT_AUDIO7_FILE)->GetWindowText(strInputFileAudio7);

	if (!strInputFileAudio7.IsEmpty())
	{
		arryAudios.Add(strInputFileAudio7);
	}

	CString strInputFileAudio8;
	dlg->GetDlgItem(IDC_EDIT_INPUT_AUDIO8_FILE)->GetWindowText(strInputFileAudio8);

	if (!strInputFileAudio8.IsEmpty())
	{
		arryAudios.Add(strInputFileAudio8);
	}
	CString strTrimIn;
	dlg->GetDlgItem(IDC_EDIT_TRIMIN)->GetWindowText(strTrimIn);

	CString strTrimOut;
	dlg->GetDlgItem(IDC_EDIT_TRIMOUT)->GetWindowText(strTrimOut);

	int nTrimIn = atoi(strTrimIn.GetBuffer());
	int nTrimOut = atoi(strTrimOut.GetBuffer());
	dlg->m_OutStart = nTrimOut;
	if (dlg ->IsDlgButtonChecked(IDC_CHECK_XAVC_MC) == BST_CHECKED)
	{

		hr = dlg->m_pTransCode->TransCode_SetWrapperFileType(keNsWrapperFileTypeIDL_MXF_XAVC_MC);
		NSD_SAFE_REPORT_ERROR(keLogPkgTest, 0, hr,
			"CTest_xTransCodeNodeDlg::OnBnClickedButtonStart,TransElement_AddVideoFile Failed!", true);
	}


	hr = dlg->m_pTransCode->TransCode_Begin(strOutputFile.AllocSysString());
	NSD_SAFE_REPORT_ERROR(keLogPkgTest, 0, hr,
		"CTest_xTransCodeNodeDlg::OnBnClickedButtonStart,TransCode_Begin Failed!", true);


	hr = dlg->m_pTransCode->TransElement_Begin();
	NSD_SAFE_REPORT_ERROR(keLogPkgTest, 0, hr,
		"CTest_xTransCodeNodeDlg::OnBnClickedButtonStart,TransElement_Begin Failed!", true);

	CString strMixOffsetB, strMixOffsetE;
	dlg->GetDlgItem(IDC_EDIT2_Offset_B)->GetWindowText(strMixOffsetB);
	dlg->GetDlgItem(IDC_EDIT3_Offset_E)->GetWindowText(strMixOffsetE);
	double offsetB = atof(strMixOffsetB.GetBuffer());
	double offsetE = atof(strMixOffsetE.GetBuffer());

	if (dlg ->IsDlgButtonChecked(IDC_CHECK_MIX) == BST_CHECKED)
	{
		CString strMixTrimIn;
		dlg->GetDlgItem(IDC_EDIT_MIXV_TRIMIN)->GetWindowText(strMixTrimIn);
		CString strMixTrimOut;
		dlg->GetDlgItem(IDC_EDIT_MIXV_TRIMOUT)->GetWindowText(strMixTrimOut);
		int nMixTrimIn = atoi(strMixTrimIn.GetBuffer());
		int nMixTrimOut = atoi(strMixTrimOut.GetBuffer());
		if (nMixTrimOut < nMixTrimIn)
		{
			AfxMessageBox("�����������Mix In�����С��Out�㣡");
			ASSERT(0);
		}
		hr = dlg->m_pTransCode->TransElement_AddVideoFile(strInputMixFileVideo.AllocSysString(), nMixTrimIn, nMixTrimOut, strInputCam.AllocSysString());
		NSD_SAFE_REPORT_ERROR(keLogPkgTest, 0, hr,
			"CTest_xTransCodeNodeDlg::OnBnClickedButtonStart,TransElement_AddVideoFile Failed!", true);
	}

	hr = dlg->m_pTransCode->TransElement_AddVideoFile(strInputFileVideo.AllocSysString(), nTrimIn, nTrimOut, strInputCam.AllocSysString());
	NSD_SAFE_REPORT_ERROR(keLogPkgTest, 0, hr,
		"CTest_xTransCodeNodeDlg::OnBnClickedButtonStart,TransElement_AddVideoFile Failed!", true);

	dlg->m_pTransCode->TransElement_SetMixSchedule(offsetB, offsetE);

	for (int i = 0; i < arryAudios.GetCount(); i++)
	{
		hr = dlg->m_pTransCode->TransElement_AddAudioFile(arryAudios[i].AllocSysString(), nTrimIn, nTrimOut);
		NSD_SAFE_REPORT_ERROR(keLogPkgTest, 0, hr,
			"CTest_xTransCodeNodeDlg::OnBnClickedButtonStart,TransElement_AddAudioFile Failed!", true);
	}
	int iCheck = dlg->m_CheckButton.GetCheck();
	hr = dlg->m_pTransCode->TransElement_End(iCheck);
	NSD_SAFE_REPORT_ERROR(keLogPkgTest, 0, hr,
		"CTest_xTransCodeNodeDlg::OnBnClickedButtonStart,TransElement_End Failed!", true);
	hr = dlg->m_pTransCode->TransCode_End();
	AfxMessageBox("Fineshed.");
	CoUninitialize();
	
	return 0;
}
// CTest_xTransCodeNodeDlg ��Ϣ�������

BOOL CTest_xTransCodeNodeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	bButtonFlag = true;
	HRESULT hr = ::CoCreateInstance(CLSID_NxTransCode, NULL, CLSCTX_INPROC_SERVER,
		IID_INxTransCode, (LPVOID*)&m_pTransCode);

	hr = m_pTransCode->AddRef();
	GetDlgItem(IDC_EDIT_TRIMIN)->SetWindowText("0");
	CString strTrimOut;
	GetDlgItem(IDC_EDIT_TRIMOUT)->SetWindowText("500");

	GetDlgItem(IDC_EDIT_MIXV_TRIMIN)->SetWindowText("0");
	GetDlgItem(IDC_EDIT_MIXV_TRIMOUT)->SetWindowText("500");

	//GetDlgItem(IDC_EDIT_TRIMIN)->SetWindowText("750");
	//CString strTrimOut;
	//GetDlgItem(IDC_EDIT_TRIMOUT)->SetWindowText("850");

	//GetDlgItem(IDC_EDIT_MIXV_TRIMIN)->SetWindowText("750");
	//GetDlgItem(IDC_EDIT_MIXV_TRIMOUT)->SetWindowText("850");


	GetDlgItem(IDC_EDIT_OUT_FILE)->SetWindowText("D:\\out.mxf");

	/*GetDlgItem(IDC_EDIT_INPUT_VIDEO_FILE)->SetWindowText("\\\\192.168.0.102\\D\\MCEContent\\20170426140434\\High\\��Ŀ01_20170426140434_1.mxf");
	GetDlgItem(IDC_EDIT_INPUT_MIX_VIDEO_FILE)->SetWindowText("\\\\192.168.0.102\\D\\MCEContent\\20170426140434\\High\\��Ŀ01_20170426140434_1.mxf");
	GetDlgItem(IDC_EDIT_INPUT_AUDIO1_FILE)->SetWindowText("\\\\192.168.0.102\\D\\MCEContent\\20170426140434\\High\\��Ŀ01_20170426140434_100.mxf");
	GetDlgItem(IDC_EDIT_INPUT_AUDIO2_FILE)->SetWindowText("\\\\192.168.0.102\\D\\MCEContent\\20170426140434\\High\\��Ŀ01_20170426140434_101.mxf");
	GetDlgItem(IDC_EDIT_INPUT_AUDIO3_FILE)->SetWindowText("\\\\192.168.0.102\\D\\MCEContent\\20170426140434\\High\\��Ŀ01_20170426140434_102.mxf");
	GetDlgItem(IDC_EDIT_INPUT_AUDIO4_FILE)->SetWindowText("\\\\192.168.0.102\\D\\MCEContent\\20170426140434\\High\\��Ŀ01_20170426140434_103.mxf");
	GetDlgItem(IDC_EDIT_INPUT_AUDIO5_FILE)->SetWindowText("\\\\192.168.0.102\\D\\MCEContent\\20170426140434\\High\\��Ŀ01_20170426140434_104.mxf");
	GetDlgItem(IDC_EDIT_INPUT_AUDIO6_FILE)->SetWindowText("\\\\192.168.0.102\\D\\MCEContent\\20170426140434\\High\\��Ŀ01_20170426140434_105.mxf");
	GetDlgItem(IDC_EDIT_INPUT_AUDIO7_FILE)->SetWindowText("\\\\192.168.0.102\\D\\MCEContent\\20170426140434\\High\\��Ŀ01_20170426140434_106.mxf");
	GetDlgItem(IDC_EDIT_INPUT_AUDIO8_FILE)->SetWindowText("\\\\192.168.0.102\\D\\MCEContent\\20170426140434\\High\\��Ŀ01_20170426140434_107.mxf");*/

	//CString strFileName = "G:\\XAVC_10bit_REC709.MXF";
	CString strFileName = "D:\\quanwei\\�����_.mxf";
	GetDlgItem(IDC_EDIT_INPUT_VIDEO_FILE)->SetWindowText(strFileName);
	GetDlgItem(IDC_EDIT_INPUT_MIX_VIDEO_FILE)->SetWindowText(strFileName);
	GetDlgItem(IDC_EDIT_INPUT_AUDIO1_FILE)->SetWindowText(strFileName);
	GetDlgItem(IDC_EDIT_INPUT_AUDIO2_FILE)->SetWindowText(strFileName);
	GetDlgItem(IDC_EDIT_INPUT_AUDIO3_FILE)->SetWindowText(strFileName);
	GetDlgItem(IDC_EDIT_INPUT_AUDIO4_FILE)->SetWindowText(strFileName);
	GetDlgItem(IDC_EDIT_INPUT_AUDIO5_FILE)->SetWindowText(strFileName);
	GetDlgItem(IDC_EDIT_INPUT_AUDIO6_FILE)->SetWindowText(strFileName);
	GetDlgItem(IDC_EDIT_INPUT_AUDIO7_FILE)->SetWindowText(strFileName);
	GetDlgItem(IDC_EDIT_INPUT_AUDIO8_FILE)->SetWindowText(strFileName);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTest_xTransCodeNodeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTest_xTransCodeNodeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTest_xTransCodeNodeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CTest_xTransCodeNodeDlg::CheckPaths()
{
	CString strOutputFile;
	GetDlgItem(IDC_EDIT_OUT_FILE)->GetWindowText(strOutputFile);

	CString strInputFileVideo;
	GetDlgItem(IDC_EDIT_INPUT_VIDEO_FILE)->GetWindowText(strInputFileVideo);
	CString strInputFileAudio1;
	GetDlgItem(IDC_EDIT_INPUT_AUDIO1_FILE)->GetWindowText(strInputFileAudio1);


	CString strTrimIn;
	GetDlgItem(IDC_EDIT_TRIMIN)->GetWindowText(strTrimIn);

	CString strTrimOut;
	GetDlgItem(IDC_EDIT_TRIMOUT)->GetWindowText(strTrimOut);

	if (strOutputFile.IsEmpty() || strInputFileVideo.IsEmpty() || strInputFileAudio1.IsEmpty() )
	{
		AfxMessageBox("�������������������ȷ�ļ���");
		return false;
	}


	if (strTrimIn.IsEmpty() || strTrimOut.IsEmpty())
	{
		AfxMessageBox("�������������������ȷIn  , Out �㣡");
		return false;
	}

	int nTrimIn = atoi(strTrimIn.GetBuffer());
	int nTrimOut = atoi(strTrimOut.GetBuffer());

	if (nTrimOut <= nTrimIn)
	{
		AfxMessageBox("�����������In�����С��Out�㣡");

		return false;
	}


	return true;


}
void CTest_xTransCodeNodeDlg::OnBnClickedButtonStart()
{
	HRESULT hr = NS_NOERROR;
	CreateThread(NULL, 0, Func, this, 0, NULL);
}
/*
void CTest_xTransCodeNodeDlg::OnBnClickedButtonStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//video

	HRESULT hr = NS_NOERROR;

	NsSetLogFilePath("c:\\");

	//if (!CheckPaths())
	//{
	//	return;
	//}

	if (m_pVideoReaderNode != NULL)
		return;

	CreateThread(NULL, 0, Func, this, 0, NULL);
	CStringArray arryAudios;
	CString strOutputFile;
	GetDlgItem(IDC_EDIT_OUT_FILE)->GetWindowText(strOutputFile);

	CString strInputFileVideo;
	GetDlgItem(IDC_EDIT_INPUT_VIDEO_FILE)->GetWindowText(strInputFileVideo);
	CString strInputMixFileVideo;
	GetDlgItem(IDC_EDIT_INPUT_MIX_VIDEO_FILE)->GetWindowText(strInputMixFileVideo);

	CString strInputFileAudio1;
	GetDlgItem(IDC_EDIT_INPUT_AUDIO1_FILE)->GetWindowText(strInputFileAudio1);
	CString strInputCam;
	GetDlgItem(IDC_EDIT1_Cam)->GetWindowTextA(strInputCam);
	if (!strInputFileAudio1.IsEmpty())
	{
		arryAudios.Add(strInputFileAudio1);
	}

	CString strInputFileAudio2;
	GetDlgItem(IDC_EDIT_INPUT_AUDIO2_FILE)->GetWindowText(strInputFileAudio2);

	if (!strInputFileAudio2.IsEmpty())
	{
		arryAudios.Add(strInputFileAudio2);
	}


	CString strInputFileAudio3;
	GetDlgItem(IDC_EDIT_INPUT_AUDIO3_FILE)->GetWindowText(strInputFileAudio3);

	if (!strInputFileAudio3.IsEmpty())
	{
		arryAudios.Add(strInputFileAudio3);
	}

	CString strInputFileAudio4;
	GetDlgItem(IDC_EDIT_INPUT_AUDIO4_FILE)->GetWindowText(strInputFileAudio4);

	if (!strInputFileAudio4.IsEmpty())
	{
		arryAudios.Add(strInputFileAudio4);
	}

	CString strInputFileAudio5;
	GetDlgItem(IDC_EDIT_INPUT_AUDIO5_FILE)->GetWindowText(strInputFileAudio5);

	if (!strInputFileAudio5.IsEmpty())
	{
		arryAudios.Add(strInputFileAudio5);
	}

	CString strInputFileAudio6;
	GetDlgItem(IDC_EDIT_INPUT_AUDIO6_FILE)->GetWindowText(strInputFileAudio6);

	if (!strInputFileAudio6.IsEmpty())
	{
		arryAudios.Add(strInputFileAudio6);
	}

	CString strInputFileAudio7;
	GetDlgItem(IDC_EDIT_INPUT_AUDIO7_FILE)->GetWindowText(strInputFileAudio7);

	if (!strInputFileAudio7.IsEmpty())
	{
		arryAudios.Add(strInputFileAudio7);
	}

	CString strInputFileAudio8;
	GetDlgItem(IDC_EDIT_INPUT_AUDIO8_FILE)->GetWindowText(strInputFileAudio8);

	if (!strInputFileAudio8.IsEmpty())
	{
		arryAudios.Add(strInputFileAudio8);
	}
	CString strTrimIn;
	GetDlgItem(IDC_EDIT_TRIMIN)->GetWindowText(strTrimIn);

	CString strTrimOut;
	GetDlgItem(IDC_EDIT_TRIMOUT)->GetWindowText(strTrimOut);

	int nTrimIn = atoi(strTrimIn.GetBuffer());
	int nTrimOut = atoi(strTrimOut.GetBuffer());
	m_OutStart = nTrimOut;
	if (this->IsDlgButtonChecked(IDC_CHECK_XAVC_MC) == BST_CHECKED)
	{

		hr = m_pTransCode->TransCode_SetWrapperFileType(keNsWrapperFileTypeIDL_MXF_XAVC_MC);
		NSD_SAFE_REPORT_ERROR(keLogPkgTest, 0, hr,
			"CTest_xTransCodeNodeDlg::OnBnClickedButtonStart,TransElement_AddVideoFile Failed!", true);
	}
	

	hr = m_pTransCode->TransCode_Begin(strOutputFile.AllocSysString());
	NSD_SAFE_REPORT_ERROR(keLogPkgTest, 0, hr,
		"CTest_xTransCodeNodeDlg::OnBnClickedButtonStart,TransCode_Begin Failed!", true);


	hr = m_pTransCode->TransElement_Begin();
	NSD_SAFE_REPORT_ERROR(keLogPkgTest, 0, hr,
		"CTest_xTransCodeNodeDlg::OnBnClickedButtonStart,TransElement_Begin Failed!", true);

	CString strMixOffsetB, strMixOffsetE;
	GetDlgItem(IDC_EDIT2_Offset_B)->GetWindowText(strMixOffsetB);
	GetDlgItem(IDC_EDIT3_Offset_E)->GetWindowText(strMixOffsetE);
	int offsetB = atoi(strMixOffsetB.GetBuffer());
	int offsetE = atoi(strMixOffsetE.GetBuffer());

	if(this->IsDlgButtonChecked(IDC_CHECK_MIX) == BST_CHECKED)
	{
		CString strMixTrimIn;
		GetDlgItem(IDC_EDIT_MIXV_TRIMIN)->GetWindowText(strMixTrimIn);
		CString strMixTrimOut;
		GetDlgItem(IDC_EDIT_MIXV_TRIMOUT)->GetWindowText(strMixTrimOut);
		int nMixTrimIn = atoi(strMixTrimIn.GetBuffer());
		int nMixTrimOut = atoi(strMixTrimOut.GetBuffer());
		if (nMixTrimOut < nMixTrimIn)
		{
			AfxMessageBox("�����������Mix In�����С��Out�㣡");
			ASSERT(0);
		}
		hr = m_pTransCode->TransElement_AddVideoFile(strInputMixFileVideo.AllocSysString(), nMixTrimIn, nMixTrimOut, offsetB,strInputCam.AllocSysString());
		NSD_SAFE_REPORT_ERROR(keLogPkgTest, 0, hr,
			"CTest_xTransCodeNodeDlg::OnBnClickedButtonStart,TransElement_AddVideoFile Failed!", true);
	}

	hr = m_pTransCode->TransElement_AddVideoFile(strInputFileVideo.AllocSysString(), nTrimIn, nTrimOut, offsetE, strInputCam.AllocSysString());
	NSD_SAFE_REPORT_ERROR(keLogPkgTest, 0, hr,
		"CTest_xTransCodeNodeDlg::OnBnClickedButtonStart,TransElement_AddVideoFile Failed!", true);


	for (int i = 0; i < arryAudios.GetCount(); i++)
	{
		hr = m_pTransCode->TransElement_AddAudioFile(arryAudios[i].AllocSysString(),nTrimIn, nTrimOut);
		NSD_SAFE_REPORT_ERROR(keLogPkgTest, 0, hr,
			"CTest_xTransCodeNodeDlg::OnBnClickedButtonStart,TransElement_AddAudioFile Failed!", true);
	}
	int iCheck = m_CheckButton.GetCheck();
	hr = m_pTransCode->TransElement_End(iCheck);
	NSD_SAFE_REPORT_ERROR(keLogPkgTest, 0, hr,
		"CTest_xTransCodeNodeDlg::OnBnClickedButtonStart,TransElement_End Failed!", true);

	AfxMessageBox("Fineshed.");
}*/


void CTest_xTransCodeNodeDlg::OnBnClickedButtonFinish()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	HRESULT hr = NS_NOERROR;

	hr = m_pTransCode->TransCode_End();
	NSD_SAFE_REPORT_ERROR(keLogPkgTest, 0, hr,
		"CTest_xTransCodeNodeDlg::OnBnClickedButtonFinish,TransElement_End Failed!", true);
}

void CTest_xTransCodeNodeDlg::OnBnClickedButtonSelectedOutputFile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString strExt(_T("*"));
	CString strFilter(_T("Output�ļ�(*.mxf)|*.mxf||"));

	CFileDialog filedialog(FALSE, strExt, NULL, OFN_PATHMUSTEXIST, strFilter, this);

	if (filedialog.DoModal() != IDOK)
		return;

	GetDlgItem(IDC_EDIT_OUT_FILE)->SetWindowText(filedialog.GetPathName());
}

void CTest_xTransCodeNodeDlg::OnBnClickedButtonSelectInput()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strExt(_T("*"));
	CString strFilter(_T("Input�ļ�(*.mxf)|*.mxf||"));

	CFileDialog filedialog(TRUE, strExt, NULL, OFN_FILEMUSTEXIST, strFilter, this);

	if (filedialog.DoModal() != IDOK)
		return;

	GetDlgItem(IDC_EDIT_INPUT_VIDEO_FILE)->SetWindowText(filedialog.GetPathName());
}

void CTest_xTransCodeNodeDlg::OnBnClickedButtonSelectInputAudio()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strExt(_T("*"));
	CString strFilter(_T("Input�ļ�(*.mxf)|*.mxf||"));

	CFileDialog filedialog(TRUE, strExt, NULL, OFN_FILEMUSTEXIST, strFilter, this);

	if (filedialog.DoModal() != IDOK)
		return;

	GetDlgItem(IDC_EDIT_INPUT_AUDIO1_FILE)->SetWindowText(filedialog.GetPathName());
}
void CTest_xTransCodeNodeDlg::OnBnClickedButtonNext()
{
	HRESULT hr = NS_NOERROR;
	CreateThread(NULL, 0, FuncNext, this, 0, NULL);
}
/*
void CTest_xTransCodeNodeDlg::OnBnClickedButtonNext()
{
	int nTrimIn = 0;
	int nTrimOut = 0;

		CString strTrimIn;
		GetDlgItem(IDC_EDIT_TRIMIN)->GetWindowText(strTrimIn);
		nTrimIn = atoi(strTrimIn);
		CString strTrimOut;
		GetDlgItem(IDC_EDIT_TRIMOUT)->GetWindowText(strTrimOut);
		nTrimOut = atoi(strTrimOut);
		CStringArray arryAudios;
		CString strOutputFile;
		GetDlgItem(IDC_EDIT_OUT_FILE)->GetWindowText(strOutputFile);

		CString strInputFileVideo;
		GetDlgItem(IDC_EDIT_INPUT_VIDEO_FILE)->GetWindowText(strInputFileVideo);

		CString strInputMixFileVideo;
		GetDlgItem(IDC_EDIT_INPUT_MIX_VIDEO_FILE)->GetWindowText(strInputMixFileVideo);

		CString strInputCam;
		GetDlgItem(IDC_EDIT1_Cam)->GetWindowText(strInputCam);

		CString strInputFileAudio1;
		GetDlgItem(IDC_EDIT_INPUT_AUDIO1_FILE)->GetWindowText(strInputFileAudio1);

		if (!strInputFileAudio1.IsEmpty())
		{
			arryAudios.Add(strInputFileAudio1);
		}

		CString strInputFileAudio2;
		GetDlgItem(IDC_EDIT_INPUT_AUDIO2_FILE)->GetWindowText(strInputFileAudio2);

		if (!strInputFileAudio2.IsEmpty())
		{
			arryAudios.Add(strInputFileAudio2);
		}


		CString strInputFileAudio3;
		GetDlgItem(IDC_EDIT_INPUT_AUDIO3_FILE)->GetWindowText(strInputFileAudio3);

		if (!strInputFileAudio3.IsEmpty())
		{
			arryAudios.Add(strInputFileAudio3);
		}

		CString strInputFileAudio4;
		GetDlgItem(IDC_EDIT_INPUT_AUDIO4_FILE)->GetWindowText(strInputFileAudio4);

		if (!strInputFileAudio4.IsEmpty())
		{
			arryAudios.Add(strInputFileAudio4);
		}


		CString strInputFileAudio5;
		GetDlgItem(IDC_EDIT_INPUT_AUDIO5_FILE)->GetWindowText(strInputFileAudio5);

		if (!strInputFileAudio5.IsEmpty())
		{
			arryAudios.Add(strInputFileAudio5);
		}

		CString strInputFileAudio6;
		GetDlgItem(IDC_EDIT_INPUT_AUDIO6_FILE)->GetWindowText(strInputFileAudio6);

		if (!strInputFileAudio6.IsEmpty())
		{
			arryAudios.Add(strInputFileAudio6);
		}

		CString strInputFileAudio7;
		GetDlgItem(IDC_EDIT_INPUT_AUDIO7_FILE)->GetWindowText(strInputFileAudio7);

		if (!strInputFileAudio7.IsEmpty())
		{
			arryAudios.Add(strInputFileAudio7);
		}

		CString strInputFileAudio8;
		GetDlgItem(IDC_EDIT_INPUT_AUDIO8_FILE)->GetWindowText(strInputFileAudio8);


		if (!strInputFileAudio8.IsEmpty())
		{
			arryAudios.Add(strInputFileAudio8);
		}

		HRESULT hr = NS_NOERROR;

		hr = m_pTransCode->TransElement_Begin();
		NSD_SAFE_REPORT_ERROR(keLogPkgTest, 0, hr,
			"CTest_xTransCodeNodeDlg::OnBnClickedButtonStart,TransElement_Begin Failed!", true);

		CString strMixOffsetB, strMixOffsetE;
		GetDlgItem(IDC_EDIT2_Offset_B)->GetWindowText(strMixOffsetB);
		GetDlgItem(IDC_EDIT3_Offset_E)->GetWindowText(strMixOffsetE);
		int offsetB = atoi(strMixOffsetB.GetBuffer());
		int offsetE = atoi(strMixOffsetE.GetBuffer());

		if (this->IsDlgButtonChecked(IDC_CHECK_MIX) == BST_CHECKED)
		{

			CString strMixTrimIn;
			GetDlgItem(IDC_EDIT_MIXV_TRIMIN)->GetWindowText(strMixTrimIn);
			CString strMixTrimOut;
			GetDlgItem(IDC_EDIT_MIXV_TRIMOUT)->GetWindowText(strMixTrimOut);
			int nMixTrimIn = atoi(strMixTrimIn.GetBuffer());
			int nMixTrimOut = atoi(strMixTrimOut.GetBuffer());
			if (nMixTrimOut < nMixTrimIn)
			{
				AfxMessageBox("�����������Mix In�����С��Out�㣡");
				ASSERT(0);
			}

			hr = m_pTransCode->TransElement_AddVideoFile(strInputMixFileVideo.AllocSysString(), nMixTrimIn, nMixTrimOut, offsetB, strInputCam.AllocSysString());
			NSD_SAFE_REPORT_ERROR(keLogPkgTest, 0, hr,
				"CTest_xTransCodeNodeDlg::OnBnClickedButtonStart,TransElement_AddVideoFile Failed!", true);
		}

		hr = m_pTransCode->TransElement_AddVideoFile(strInputFileVideo.AllocSysString(), nTrimIn, nTrimOut, offsetE, strInputCam.AllocSysString());
		NSD_SAFE_REPORT_ERROR(keLogPkgTest, 0, hr,
			"CTest_xTransCodeNodeDlg::OnBnClickedButtonStart,TransElement_AddVideoFile Failed!", true);

		for (int i = 0; i < arryAudios.GetCount(); i++)
		{
			hr = m_pTransCode->TransElement_AddAudioFile(arryAudios[i].AllocSysString(), nTrimIn, nTrimOut);
			NSD_SAFE_REPORT_ERROR(keLogPkgTest, 0, hr,
				"CTest_xTransCodeNodeDlg::OnBnClickedButtonStart,TransElement_AddAudioFile Failed!", true);
		}

		int iCheck = m_CheckButton.GetCheck();
		hr = m_pTransCode->TransElement_End(iCheck);
		NSD_SAFE_REPORT_ERROR(keLogPkgTest, 0, hr,
			"CTest_xTransCodeNodeDlg::OnBnClickedButtonStart,TransElement_End Failed!", true);

	AfxMessageBox("Fineshed.");
}*/




void CTest_xTransCodeNodeDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}

void CTest_xTransCodeNodeDlg::OnBnClickedButtonSelectMixInput()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString strExt(_T("*"));
	CString strFilter(_T("Input�ļ�(*.mxf)|*.mxf||"));
	
	CFileDialog filedialog(TRUE, strExt, NULL, OFN_FILEMUSTEXIST, strFilter, this);
	
	if (filedialog.DoModal() != IDOK)
		return;
	
	GetDlgItem(IDC_EDIT_INPUT_MIX_VIDEO_FILE)->SetWindowText(filedialog.GetPathName());
}


void CTest_xTransCodeNodeDlg::OnBnClickedButton2Current()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ULONGLONG ulPosition;
	m_pTransCode->TransElement_GetCurrentPositionInFrames(&ulPosition);
	CString str;
	str.Format("%I64d", ulPosition);
	GetDlgItem(IDC_EDIT1_Current_Fram)->SetWindowText(str);
}


void CTest_xTransCodeNodeDlg::OnBnClickedButton3Stop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (bButtonFlag)
	{
		m_pTransCode->TransElement_SetFramesReaderStatus(TRUE);
		GetDlgItem(IDC_BUTTON3_Stop)->SetWindowText("Continue");
		bButtonFlag = false;
	}
	else {
		bButtonFlag = true;
		m_pTransCode->TransElement_SetFramesReaderStatus(FALSE);
		GetDlgItem(IDC_BUTTON3_Stop)->SetWindowText("Stop");
	}
		
}
