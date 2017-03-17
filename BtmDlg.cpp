// BtmDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "program_balise.h"
#include "BtmDlg.h"
#include "Comm_Balise_ts.h"
#include ".\btmdlg.h"

// CBtmDlg 对话框

IMPLEMENT_DYNAMIC(CBtmDlg, CDialog)
CBtmDlg::CBtmDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBtmDlg::IDD, pParent)
{
}

CBtmDlg::~CBtmDlg()
{
}

void CBtmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_StartBtm);
	DDX_Control(pDX, IDC_BUTTON2, m_StopBtm);
	DDX_Control(pDX, IDC_LIST1, m_telelist);
	DDX_Control(pDX, IDC_LIST2, m_usertelelist);
}


BEGIN_MESSAGE_MAP(CBtmDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton_Start_Btm)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton_Stop_Btm)
END_MESSAGE_MAP()



void CBtmDlg::OnBnClickedButton_Start_Btm()
{
	// TODO: 在此添加控件通知处理程序代码
	byte ch[1024];
	int pos=0;
	int len,len1;
	int i;
	CString ss,ss1;
	byte temp;
	UINT tele_flag_long_short;
	int total_len;

	Clear_Display();
	//m_StartBtm.EnableWindow(FALSE);
	m_StopBtm.EnableWindow(TRUE);

	//发送命令模拟 BTM
    ch[pos++]=0x30;
	ch[pos++]=0x00;
	ch[pos++]=0x00;

	len=pos;
	SendCommand(len,&ch[0]);

	//接收数据
	Read_Tele_Len=0;
	ReceiveData(READLEN_START_BTM);

	if(ReadBufLen>0)
	{
		if(ArrangeData(ORDER_START_BTM))
		{
			temp=Read_Tele[0];
			switch(temp)
			{
			case 0x02:
				MessageBox("读操作失败：数据通信校验错误！","错误",MB_OK);
				return;
				break;
			case 0x20:
				MessageBox("读操作失败:未读取到报文!","错误",MB_OK);
				return;
				break;
			case 0x21:
				ss="短报文。";
				m_telelist.AddString(ss);
				break;
			case 0x22:
				ss="长报文。";
				m_telelist.AddString(ss);
				break;
			}
			

			ss="";
			len=0;
			if(temp==0x21) len=43;
			else if(temp==0x22) len=128;
			btm_read[0]=len; //???????
			
			//显示报文
			for(i=0;i<len;i++)
			{
				btm_read[i+1]=Read_Tele[i+1];
				ss1.Format("%02x  ",Read_Tele[i+1]);
				ss+=ss1;
				if((i+1)%16==0)
				{
					m_telelist.AddString(ss);
					ss="";
				}

			}
			m_telelist.AddString(ss);
			//************************************************
			
			//***********************************************
			//分析报文
			if(temp!=0x20)
			{
				if(temp==0x21)
				{
					left_shift_bits(&Read_Tele[1],len,3,3,&total_len);
				}
				if(Analyse_Tele_Data_Get_TeleAndUsertele(&Read_Tele[1],btm_read,usertele,&tele_flag_long_short)!=0)
				{
					MessageBox("分析报文错误","错误",MB_OK);
					return ;
				}
			}

			//用户报文标识
			ss="";
			if(temp==0x21) ss="短用户报文。";
			else if(temp==0x22) ss="长用户报文。";
			m_usertelelist.AddString(ss);

			ss="";
			len1=0;
			if(temp==0x22) len1=104;
			else if(temp==0x21) len1=27;
			
			for(i=0;i<len1;i++)
			{
				ss1.Format("%02x  ",usertele[i]);
				ss+=ss1;
				if(i>0 && (i+1)%16==0)
				{
					m_usertelelist.AddString(ss);
					ss="";
				}
			}
			m_usertelelist.AddString(ss);
			
		}
	}
	else 
	{
		MessageBox("读操作失败：通信未建立！","错误",MB_OK);
		m_StartBtm.EnableWindow(TRUE);
		m_StopBtm.EnableWindow(FALSE);
		return;
	}

	return;
}

void CBtmDlg::OnBnClickedButton_Stop_Btm()
{
	// TODO: 在此添加控件通知处理程序代码
	StopSend(1);
}

BOOL CBtmDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_StartBtm.EnableWindow(TRUE);
	m_StopBtm.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CBtmDlg::Clear_Display()
{
	m_telelist.ResetContent();
	m_usertelelist.ResetContent();

	for(int i=0;i<129;i++)
	{
		btm_read[i]=0x00;
	}

}

void CBtmDlg::StopSend(int type)
{
	int len;
	byte ch[1024];
	int pos=0;
	byte temp;

	//发送命令模拟 BTM
    ch[pos++]=0x31;
	ch[pos++]=0x00;
	ch[pos++]=0x00;

	len=pos;
	SendCommand(len,&ch[0]);

	//接收数据
	Read_Tele_Len=0;
	ReceiveData(READLEN_STOP_BTM);

	if(type==1)
	{
		if(ReadBufLen>0)
		{
			if(ArrangeData(ORDER_STOP_BTM))
			{
				//
				temp=Read_Tele[0];
				if(temp==0x02)
					MessageBox("接收数据不正常","提示",MB_OK);
			}
		}

		m_StartBtm.EnableWindow(TRUE);
		m_StopBtm.EnableWindow(FALSE);
	}
}

void CBtmDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	
	StopSend(0);
	CDialog::OnOK();
}

void CBtmDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	StopSend(0);
	CDialog::OnCancel();
}
