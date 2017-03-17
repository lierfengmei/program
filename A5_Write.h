#pragma once
#include "afxwin.h"
#include "BmpStatic.h"

// CA5_Write 对话框

class CA5_Write : public CDialog
{
	DECLARE_DYNAMIC(CA5_Write)

public:
	CA5_Write(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CA5_Write();

// 对话框数据
	enum { IDD = IDD_DIALOG_A5_WRITE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit m_telegramfile;
	afx_msg void OnBnClickedButton_A5Write();
	CListBox m_telegram;

	void Clear_Display();
	void A5_Arrange_Tele(byte *buff);
	BOOL A4_Read_Tele();

	byte a5_send[130];
	byte a5_orig[130];
	byte a4_read[130];
	afx_msg void OnBnClickedButton_OpenFile();
	CBmpStatic m_Result_Bmp;
	CEdit m_Result;
};
