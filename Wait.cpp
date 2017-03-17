// Wait.cpp : 实现文件
//

#include "stdafx.h"
#include "program_balise.h"
#include "Wait.h"
#include ".\wait.h"


// CWait 对话框

IMPLEMENT_DYNAMIC(CWait, CDialog)
CWait::CWait(CWnd* pParent /*=NULL*/)
	: CDialog(CWait::IDD, pParent)
{
}

CWait::~CWait()
{
}

void CWait::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWait, CDialog)
END_MESSAGE_MAP()


// CWait 消息处理程序

BOOL CWait::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//CWaitCursor wait;
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
