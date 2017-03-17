#pragma once
#include "afxwin.h"


// CLeuCRead 对话框

class CLeuCRead : public CDialog
{
	DECLARE_DYNAMIC(CLeuCRead)

public:
	CLeuCRead(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLeuCRead();

// 对话框数据
	enum { IDD = IDD_DIALOG_LEU_C };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CButton m_Select_1;
	CButton m_Select_2;
	CButton m_Select_3;
	CButton m_Select_4;

	CEdit m_telegramfile;
	CEdit m_result;

	CListBox m_telegram;
	CListBox m_usertelegram;
	CListBox m_header;

	byte c_read[129];
	byte c_send[129];
	byte usertele[104];
	byte temp_tele[129];
	byte temp_usertele[104];

	afx_msg void OnBnClickedButtonOpenfile();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonC4Read();
	CEdit m_C4_Flag;
	CEdit m_C6_Energy;

	void Clear_Display();
	int Get_Check();
};
