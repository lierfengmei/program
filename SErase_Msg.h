#pragma once


// CSErase_Msg �Ի���

class CSErase_Msg : public CDialog
{
	DECLARE_DYNAMIC(CSErase_Msg)

public:
	CSErase_Msg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSErase_Msg();

// �Ի�������
	enum { IDD = IDD_DIALOG_LEU_ERASE_BTM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonEraseTse();
	afx_msg void OnBnClickedButtonEraseTpc();
	afx_msg void OnBnClickedButtonEraseTe1();
	afx_msg void OnBnClickedButtonEraseThm();
	afx_msg void OnBnClickedButtonEraseThe();
	afx_msg void OnBnClickedButtonEraseTe3();
	afx_msg void OnBnClickedButtonEraseTe2();
	afx_msg void OnBnClickedButtonEraseTe4();
	afx_msg void OnBnClickedButtonEraseTe5();
	afx_msg void OnBnClickedButtonEraseTe6();

	BOOL Erase_Message(int type);
};
