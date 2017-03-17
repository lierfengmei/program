#pragma once
#include "afxwin.h"


// CA5_Read 对话框

class CA5_Read : public CDialog
{
	DECLARE_DYNAMIC(CA5_Read)

public:
	CA5_Read(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CA5_Read();

// 对话框数据
	enum { IDD = IDD_DIALOG_A5_READ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	
public:
	afx_msg void OnBnClickedButtonA5Read();

	void Clear_Display();

	CListBox m_telegram;
	CEdit m_telegramfile;
	CEdit m_result;

	byte a5_read[129];
	byte a5_send[129];

	afx_msg void OnBnClickedButton_OpenFile();

	void Analyse_Tele_Data();
	CStatic m_Static_Silent;
	CStatic m_Static_Functional;
	

	virtual BOOL OnInitDialog();
};
