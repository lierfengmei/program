#pragma once


// CLeuWriteDefaultTele_Bx �Ի���

class CLeuWriteDefaultTele_Bx : public CDialog
{
	DECLARE_DYNAMIC(CLeuWriteDefaultTele_Bx)

public:
	CLeuWriteDefaultTele_Bx(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLeuWriteDefaultTele_Bx();

// �Ի�������
	enum { IDD = IDD_DIALOG_LEU_WRITETELE_BX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOpenfile1();
	afx_msg void OnBnClickedButtonOpenfile2();
	afx_msg void OnBnClickedButtonOpenfile3();
	afx_msg void OnBnClickedButtonOpenfile4();

	CEdit m_filename[4];
	CEdit m_index[4];
	DWORD m_send_len;
	byte m_send[4][130]; 
	byte m_index_data[4];
	byte m_teledata[600];
	afx_msg void OnBnClickedButtonWrite();
	BOOL Arrange_tele();
	virtual BOOL OnInitDialog();
	BOOL Send_Tele();
};
