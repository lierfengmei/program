#pragma once


// CWait �Ի���

class CWait : public CDialog
{
	DECLARE_DYNAMIC(CWait)

public:
	CWait(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWait();

// �Ի�������
	enum { IDD = IDD_DIALOG_WAIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	//CWaitCursor wc;
};
