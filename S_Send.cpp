// S_Send.cpp : 实现文件
//

#include "stdafx.h"
#include "program_balise.h"
#include "S_Send.h"
#include "Comm_Balise_ts.h"
#include ".\s_send.h"

UINT Send_Multi(LPVOID lpParam);

// CS_Send 对话框

IMPLEMENT_DYNAMIC(CS_Send, CDialog)
CS_Send::CS_Send(CWnd* pParent /*=NULL*/)
	: CDialog(CS_Send::IDD, pParent)
{
}

CS_Send::~CS_Send()
{
}

void CS_Send::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_filename[0]);
	DDX_Control(pDX, IDC_EDIT2, m_filename[1]);
	DDX_Control(pDX, IDC_EDIT3, m_filename[2]);
	DDX_Control(pDX, IDC_EDIT4, m_filename[3]);
	DDX_Control(pDX, IDC_BUTTON1, m_Start);
	DDX_Control(pDX, IDC_BUTTON3, m_Stop);
}


BEGIN_MESSAGE_MAP(CS_Send, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton_OneSend)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton_Stop)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton_OpenFile1)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton_OpenFile2)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton_OpenFile3)
	ON_BN_CLICKED(IDC_BUTTON7, OnBnClickedButton_OpenFile4)
END_MESSAGE_MAP()


// CS_Send 消息处理程序

void CS_Send::OnBnClickedButton_OneSend()
{
	// TODO: 在此添加控件通知处理程序代码

	if(SendTelegram())
	{
		m_Start.EnableWindow(FALSE);
		m_Stop.EnableWindow(TRUE);
	}
	else
	{
		m_Start.EnableWindow(TRUE);
		m_Stop.EnableWindow(FALSE);
	}
}

void CS_Send::OnBnClickedButton_Stop()
{
	// TODO: 在此添加控件通知处理程序代码
	StopSend(1);
}

BOOL CS_Send::SendTelegram()
{
	int len,len_data;
	byte ch[1024];
	int pos=0;
	byte temp;
	int i,j;
	CString filename[4];
	//unsigned int crc;

	for(i=0;i<4;i++)
	{
		m_filename[i].GetWindowText(filename[i]);
		if(filename[i]=="")
		{
			MessageBox("无法打开报文文件","打开文件错误",MB_OK);
			return FALSE;
		}
		if(!InitTelegram(filename[i],&s_send[i][0]))
		{
			MessageBox("报文文件格式不正确，请重新选择文件！","错误",MB_OK);
			return FALSE;
		}
	}

	//发送命令
    ch[pos++]=0x20;
	ch[pos++]=0x00;//
	ch[pos++]=0x00;//

	//头
/*	ch[pos++]=0x01;
	ch[pos++]=0x80;
	ch[pos++]=0x7f;
	ch[pos++]=0xff;
	ch[pos++]=0xff;
	ch[pos++]=0xff;

	//体
	for(i=0;i<4;i++) ch[pos++]=0xaa;
	ch[pos++]=0x02;
	ch[pos++]=0x0c;
	for(i=0;i<4;i++) ch[pos++]=0xc3;
	for(i=0;i<4;i++) ch[pos++]=0x3c;
*/
	//报文
	for(i=0;i<4;i++)
	{
		ch[pos++]=0x01;
		ch[pos++]=0x00;
		ch[pos++]=(byte)(i+1);

		for(j=0;j<s_send[i][0];j++)
		{
			ch[pos++]=s_send[i][j+1];
		}
	}

	//CRC
/*	crc=cal_crc(&ch[3],544);

	ch[pos++]=crc & 0xff;
	crc>>=8;
	ch[pos++]=crc & 0xff;
*/
	//长度
	len_data=pos-3;
	ch[1]=(byte)(len_data & 0xff);
	len_data>>=8;
	ch[2]=(byte)(len_data & 0xff);

	len=pos;
	SendCommand(len,&ch[0]);

	//接收数据
	Read_Tele_Len=0;
	ReceiveData(READLEN_LEU_SEND_TSD);

	if(ReadBufLen>0)
	{
		if(ArrangeData(ORDER_LEU_SEND_TSD))
		{
			//
			temp=Read_Tele[0];
			if(temp==0x02)
			{
				MessageBox("操作失败：数据通信校验错误！","错误",MB_OK);
				return FALSE;
			}
		}
	}
	else
	{
		MessageBox("操作失败：通信未建立！","错误",MB_OK);
		return FALSE;
	}

	return TRUE;
}

void CS_Send::StopSend(int type)
{
	int len;
	byte ch[1024];
	int pos=0;
	byte temp;

	//发送命令
    ch[pos++]=0x21;
	ch[pos++]=0x00;
	ch[pos++]=0x00;

	len=pos;
	SendCommand(len,&ch[0]);

	//接收数据
	Read_Tele_Len=0;
	ReceiveData(READLEN_LEU_STOP_TSD);

	if(type==1)
	{
		if(ReadBufLen>0)
		{
			if(ArrangeData(ORDER_LEU_STOP_TSD))
			{
				//
				temp=Read_Tele[0];
				if(temp==0x02)
					MessageBox("接收数据不正常","提示",MB_OK);
			}
		}
	
		m_Start.EnableWindow(TRUE);
		m_Stop.EnableWindow(FALSE);
	}
}

void CS_Send::OnBnClickedButton_OpenFile1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	CFileFind	filefind;
	CString filename;
	CString filepath;

	CFileDialog filedlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						"报文文件 (*.tgm)|*.tgm|",this);

	if(filedlg.DoModal()!=IDOK) return;

	filepath=filedlg.GetPathName();
	filename=filedlg.GetFileName();

	m_filename[0].SetWindowText(filepath);

}

void CS_Send::OnBnClickedButton_OpenFile2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	CFileFind	filefind;
	CString filename;
	CString filepath;

	CFileDialog filedlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						"报文文件 (*.tgm)|*.tgm|",this);

	if(filedlg.DoModal()!=IDOK) return;

	filepath=filedlg.GetPathName();
	filename=filedlg.GetFileName();

	m_filename[1].SetWindowText(filepath);
}

void CS_Send::OnBnClickedButton_OpenFile3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	CFileFind	filefind;
	CString filename;
	CString filepath;

	CFileDialog filedlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						"报文文件 (*.tgm)|*.tgm|",this);

	if(filedlg.DoModal()!=IDOK) return;

	filepath=filedlg.GetPathName();
	filename=filedlg.GetFileName();

	m_filename[2].SetWindowText(filepath);
}

void CS_Send::OnBnClickedButton_OpenFile4()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	CFileFind	filefind;
	CString filename;
	CString filepath;

	CFileDialog filedlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						"报文文件 (*.tgm)|*.tgm|",this);

	//str="D:\\";
	//filedlg.m_ofn.hwndOwner =m_hWnd;
	//if(filefind.FindFile(str)==TRUE)
		//filedlg.m_ofn.lpstrInitialDir =str;

	if(filedlg.DoModal()!=IDOK) return;

	filepath=filedlg.GetPathName();
	filename=filedlg.GetFileName();

	m_filename[3].SetWindowText(filepath);
}



void CS_Send::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	StopSend(0);
	CDialog::OnCancel();
}

void CS_Send::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	StopSend(0);
	CDialog::OnOK();
}

BOOL CS_Send::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Start.EnableWindow(TRUE);
	m_Stop.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
