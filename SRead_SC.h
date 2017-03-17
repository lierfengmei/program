#pragma once

#include "Wait.h"

// CSRead_SC 对话框

class CSRead_SC : public CDialog
{
	DECLARE_DYNAMIC(CSRead_SC)

public:
	CSRead_SC(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSRead_SC();

// 对话框数据
	enum { IDD = IDD_DIALOG_S_READ_SC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonReadSc();

	BOOL Anaylse_SC(byte data[],int len);
	BOOL Anaylse_THM(byte data[],int len);
	BOOL Anaylse_THE(byte data[],int len);

	//CWait m_dlgWait;
	afx_msg void OnBnClickedButtonReadThm();
	afx_msg void OnBnClickedButtonReadThe();

	CString Message_THM(int message);
};
