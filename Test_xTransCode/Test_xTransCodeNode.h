
// Test_xTransCodeNode.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTest_xTransCodeNodeApp: 
// �йش����ʵ�֣������ Test_xTransCodeNode.cpp
//

class CTest_xTransCodeNodeApp : public CWinApp
{
public:
	CTest_xTransCodeNodeApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTest_xTransCodeNodeApp theApp;