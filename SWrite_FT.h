#pragma once
#include "afxwin.h"


// CSWrite_FT �Ի���

class CSWrite_FT : public CDialog
{
	DECLARE_DYNAMIC(CSWrite_FT)

public:
	CSWrite_FT(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSWrite_FT();

// �Ի�������
	enum { IDD = IDD_DIALOG_LEU_WRITE_FT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonWriteFt();

	BOOL Load_Time();
	
	__time64_t send_time;

	CEdit m_edit_time[6];
	virtual BOOL OnInitDialog();
protected:
	int m_time_year;
	int m_time_month;
	int m_time_day;
	int m_time_hour;
	int m_time_miniute;
	int m_time_second;

};
