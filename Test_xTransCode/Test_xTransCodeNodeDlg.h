
// Test_xTransCodeNodeDlg.h : 头文件
//

#pragma once

#include "CNxCommonFileReaderNode.h"
#include "CNxCommonCodecNode.h"
#include "CNxDNxHDCodecNode.h"
#include "CNxWriterNode.h"
#include "CNxDirectSoundOutputNode.h"
#include "CNxMuxerNode.h"

//extern "C" {
//#include "xTransCodeATL_i.h"
//}
#include "xTransCodeATL_i.h"
#include "afxwin.h"
// CTest_xTransCodeNodeDlg 对话框
class CTest_xTransCodeNodeDlg : public CDialogEx
{
// 构造
public:
	CTest_xTransCodeNodeDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEST_xTransCodeNODE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonFinish();

private:
	CNxCommonFileReaderNode *m_pVideoReaderNode;
	CNxCommonFileReaderNode *m_pAudioReaderNode;
	CNxCommonFileReaderNode *m_pAudioReaderNode2;
public:
	afx_msg void OnBnClickedButtonSelectedOutputFile();
	afx_msg void OnBnClickedButtonSelectInput();

	bool CheckPaths();
	int m_OutStart;
	INxTransCode *m_pTransCode;
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnBnClickedButtonSelectInputAudio();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonSelectMixInput();
	CStatic m_LiveWindow;
	CButton m_CheckButton;
	afx_msg void OnBnClickedButton2Current();
	afx_msg void OnBnClickedButton3Stop();
	bool bButtonFlag;
};
