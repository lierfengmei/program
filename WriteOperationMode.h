#pragma once
#include "afxwin.h"
#include "BmpStatic.h"

// CWriteOperationMode �Ի���

class CWriteOperationMode : public CDialog
{
	DECLARE_DYNAMIC(CWriteOperationMode)

public:
	CWriteOperationMode(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWriteOperationMode();

// �Ի�������
	enum { IDD = IDD_DIALOG_WRITE_MODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonWrite();
	CEdit m_Result;
	CButton m_Silent;
	CButton m_Functional;
	virtual BOOL OnInitDialog();

	CBmpStatic m_Result_Bmp;
};
