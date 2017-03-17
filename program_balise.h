// program_balise.h : program_balise 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // 主符号


// Cprogram_baliseApp:
// 有关此类的实现，请参阅 program_balise.cpp
//

class Cprogram_baliseApp : public CWinApp
{
public:
	Cprogram_baliseApp();


// 重写
public:
	virtual BOOL InitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern Cprogram_baliseApp theApp;
