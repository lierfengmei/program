#pragma once
#include "afxwin.h"


// CReadManufacture 对话框

class CReadManufacture : public CDialog
{
	DECLARE_DYNAMIC(CReadManufacture)

public:
	CReadManufacture(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CReadManufacture();

// 对话框数据
	enum { IDD = IDD_DIALOG_READMANU };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edit1;
	CEdit m_edit2;
	CEdit m_edit3;
	CEdit m_edit4;
	CEdit m_edit5;
	CEdit m_edit6;
	afx_msg void OnBnClickedButtonRead();
	CEdit m_Result;

	BOOL Analyse_Manu_Data();
	void Clear_Dispaly();
};
