// program_balise.h : program_balise Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // ������


// Cprogram_baliseApp:
// �йش����ʵ�֣������ program_balise.cpp
//

class Cprogram_baliseApp : public CWinApp
{
public:
	Cprogram_baliseApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern Cprogram_baliseApp theApp;
