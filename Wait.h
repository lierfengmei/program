#pragma once


// CWait 对话框

class CWait : public CDialog
{
	DECLARE_DYNAMIC(CWait)

public:
	CWait(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWait();

// 对话框数据
	enum { IDD = IDD_DIALOG_WAIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	//CWaitCursor wc;
};
