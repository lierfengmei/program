#pragma once
#include "afxwin.h"


// CBtmDlg 对话框

class CBtmDlg : public CDialog
{
	DECLARE_DYNAMIC(CBtmDlg)

public:
	CBtmDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBtmDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_BTM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
