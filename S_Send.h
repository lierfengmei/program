#pragma once
#include "afxwin.h"


// CS_Send �Ի���

class CS_Send : public CDialog
{
	DECLARE_DYNAMIC(CS_Send)

public:
	CS_Send(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CS_Send();

// �Ի�������
	enum { IDD = IDD_DIALOG_S_SEND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton_OneSend();
	afx_msg void OnBnClickedButton_Stop();
	afx_msg void OnBnClickedButton_OpenFile1();
	CEdit m_filename[4];
	afx_msg void OnBnClickedButton_OpenFile2();
	afx_msg void OnBnClickedButton_OpenFile3();
	afx_msg void OnBnClickedButton_OpenFile4();

	BOOL SendTelegram();
	byte s_send[4][129];
	void StopSend(int type);

protected:
	virtual void OnCancel();
	virtual void OnOK();
public:
	CButton m_Start;
	CButton m_Stop;
	virtual BOOL OnInitDialog();
};
