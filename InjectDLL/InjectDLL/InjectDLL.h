// InjectDLL.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CInjectDLLApp:
// �йش����ʵ�֣������ InjectDLL.cpp
//

class CInjectDLLApp : public CWinApp
{
public:
	CInjectDLLApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CInjectDLLApp theApp;