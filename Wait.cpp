// Wait.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "program_balise.h"
#include "Wait.h"
#include ".\wait.h"


// CWait �Ի���

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


// CWait ��Ϣ�������

BOOL CWait::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//CWaitCursor wait;
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
