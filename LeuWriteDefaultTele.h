#pragma once
#include "afxwin.h"


// CLeuWriteDefaultTele 对话框

class CLeuWriteDefaultTele : public CDialog
{
	DECLARE_DYNAMIC(CLeuWriteDefaultTele)

public:
	CLeuWriteDefaultTele(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLeuWriteDefaultTele();

// 对话框数据
	enum { IDD = IDD_DIALOG_LEU_WRITETELE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonWrite();
	CEdit m_filename[3];
	afx_msg void OnBnClickedButtonOpenfile1();
	afx_msg void OnBnClickedButtonOpenfile2();
	afx_msg void OnBnClickedButtonOpenfile3();

	void InitBinFile(CString ff,byte send[],DWORD* len);

	DWORD m_send_len[3];
	byte m_send_tpc[25000];
	byte m_send_te[70000];
	byte m_send_tse[100];

	byte m_read[70000];

	BOOL Send_TPC_TE1_TSE(byte type);
	afx_msg void OnBnClickedButtonReadTpc();
	afx_msg void OnBnClickedButtonReadTe1();
	afx_msg void OnBnClickedButtonReadTse();

	BOOL Read_TPC_TE_TSE(byte type);

	BOOL Anaylse_Data(byte data[],DWORD len,int type);

	BOOL Compare(byte type);

	CEdit m_Compare;
};
