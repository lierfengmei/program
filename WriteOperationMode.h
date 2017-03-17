#pragma once
#include "afxwin.h"
#include "BmpStatic.h"

// CWriteOperationMode 对话框

class CWriteOperationMode : public CDialog
{
	DECLARE_DYNAMIC(CWriteOperationMode)

public:
	CWriteOperationMode(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWriteOperationMode();

// 对话框数据
	enum { IDD = IDD_DIALOG_WRITE_MODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonWrite();
	CEdit m_Result;
	CButton m_Silent;
	CButton m_Functional;
	virtual BOOL OnInitDialog();

	CBmpStatic m_Result_Bmp;
};
