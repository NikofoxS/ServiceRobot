
// �ƶ������˻�е��.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// C�ƶ������˻�е��App: 
// �йش����ʵ�֣������ �ƶ������˻�е��.cpp
//

class CnlApp : public CWinApp
{
public:
	CnlApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CnlApp theApp;