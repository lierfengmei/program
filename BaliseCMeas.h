#pragma once
#include "afxwin.h"
#include "BmpStatic.h"

// CBaliseCMeas 对话框

class CBaliseCMeas : public CDialog
{
	DECLARE_DYNAMIC(CBaliseCMeas)

public:
	CBaliseCMeas(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBaliseCMeas();

// 对话框数据
	enum { IDD = IDD_DIALOG_BALISE_C };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton_OpenFile();
	CEdit m_telegramfile;
	CListBox m_telegram;
	afx_msg void OnBnClickedButtonMeasC();
	CEdit m_Result;
	CBmpStatic m_Result_Bmp;
	CEdit m_Have_C4;

	void Clear_Display();

	byte c5_send[130];
	byte c5_orig[130];
	byte a4_read[130];
	byte c4_flag;

};
