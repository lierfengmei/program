#pragma once
#include "afxwin.h"
#include "BmpStatic.h"

// CA4ReadDlg 对话框

class CA4ReadDlg : public CDialog
{
	DECLARE_DYNAMIC(CA4ReadDlg)

public:
	CA4ReadDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CA4ReadDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_READ_TELE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonA4Read();
	CListBox m_telegram;
	CListBox m_usertelegram;
	CListBox m_header;

	CEdit m_telegramfile;
	CEdit m_result;
	virtual BOOL OnInitDialog();

	//void InitTelegram(CString ff);
	void Analyse_User_info_frame();
	void Clear_Display();
	//int Analyse_A5_Tele_Data(byte* scr_data,byte* des_tele,byte* user_tele);
	//void left_shift(unsigned int *temp,int num);

	byte a4_read[129];
	byte a4_send[129];
	byte usertele[104];
	byte temp_tele[129];
	byte temp_usertele[104];
	afx_msg void OnBnClickedButton_OpenFile();

	CButton m_A4Read;
	CButton m_A5Read;

	CBmpStatic m_Functional_Bmp;
	CBmpStatic m_Silent_Bmp;

	afx_msg void OnLbnSelchangeList1();
};
