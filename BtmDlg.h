#pragma once
#include "afxwin.h"


// CBtmDlg �Ի���

class CBtmDlg : public CDialog
{
	DECLARE_DYNAMIC(CBtmDlg)

public:
	CBtmDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBtmDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_BTM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedButton_Start_Btm();
	afx_msg void OnBnClickedButton_Stop_Btm();
	virtual BOOL OnInitDialog();
	CButton m_StartBtm;
	CButton m_StopBtm;
	CListBox m_telelist;
	CListBox m_usertelelist;

	byte btm_read[129];
	byte usertele[104];

	void Clear_Display();

	void StopSend(int type);

protected:
	virtual void OnOK();
	virtual void OnCancel();
};
