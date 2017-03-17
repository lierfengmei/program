// ReadManufacture.cpp : 实现文件
//

#include "stdafx.h"
#include "program_balise.h"
#include "ReadManufacture.h"
#include "Comm_Balise_ts.h"

// CReadManufacture 对话框

IMPLEMENT_DYNAMIC(CReadManufacture, CDialog)
CReadManufacture::CReadManufacture(CWnd* pParent /*=NULL*/)
	: CDialog(CReadManufacture::IDD, pParent)
{
}

CReadManufacture::~CReadManufacture()
{
}

void CReadManufacture::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_1, m_edit1);
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
	DDX_Control(pDX, IDC_EDIT3, m_edit3);
	DDX_Control(pDX, IDC_EDIT4, m_edit4);
	DDX_Control(pDX, IDC_EDIT5, m_edit5);
	DDX_Control(pDX, IDC_EDIT6, m_edit6);
	DDX_Control(pDX, IDC_EDIT1, m_Result);
}


BEGIN_MESSAGE_MAP(CReadManufacture, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButtonRead)
END_MESSAGE_MAP()


// CReadManufacture 消息处理程序

void CReadManufacture::OnBnClickedButtonRead()
{
	// TODO: 在此添加控件通知处理程序代码
	byte ch[1024];
	int pos=0;

	CString ss;
	int len,len_data;
	byte temp;

	Clear_Dispaly();

    ch[pos++]=0x13;
	ch[pos++]=0x00;
	ch[pos++]=0x00;

	ch[pos++]=0x5f;

	len_data=pos-3;
	ch[1]=(byte)(len_data & 0xff);
	len_data>>=8;
	ch[2]=(byte)(len_data & 0xff);

	len=pos;

	SendCommand(len,&ch[0]);

	//*************************************************
	//接收数据
	Read_Tele_Len=0;
	Sleep(500);
	ReceiveData(READLEN_A5_READ_MANU);

	if(ReadBufLen>0)
	{
		if(ArrangeData(ORDER_A5_READ_MANU))
		{
			//标识
			ss="";
			temp=Read_Tele[0];
			switch(temp)
			{
			case 0x02:
				MessageBox("读操作失败：数据通信校验错误！","错误",MB_OK);
				return;
			case 0x12:
				MessageBox("读操作失败：握手信号错误！","错误",MB_OK);
				return;
			case 0x20:
				MessageBox("读操作失败：未读取到信息！","错误",MB_OK);
				return;
			case 0x21:
				ss="读取数据正常。";
				m_Result.SetWindowText(ss);

				Analyse_Manu_Data();
				break;
			default:
				MessageBox("读操作失败：数据分析错误！","错误",MB_OK);
				return;
			}
		}
	}
	else 
	{
		MessageBox("读操作失败：通信未建立！","错误",MB_OK);
	}
	//**********************************************************

}

BOOL CReadManufacture::Analyse_Manu_Data()
{
/*	CStdioFile m_readtele;
	CFileException ex;
	CString m_readtele_name;
	CTime  CurrentTime;
	CString str;
	int i;
	char aa;

	CurrentTime=CTime::GetCurrentTime();
	int hour=CurrentTime.GetHour();

	//打开文件
	m_readtele_name="ReadManufacturelog_";
	m_readtele_name+=CurrentTime.Format("%Y_%m_%d_%H_%M_%S");
	m_readtele_name+=".txt";

	if(!m_readtele.Open(m_readtele_name,CFile::modeCreate | CFile::modeWrite ,&ex))
		return FALSE;
	
	str="Reader log file\n";
	Print_Log(&m_readtele,str);

	str="telegram read at ";
	str+=CurrentTime.Format("%Y年%m月%d日%H:%M:%S\n\n");
	Print_Log(&m_readtele,str);

	for(i=0;i<512;i++)
	{
		if(Read_Tele[i+1]<=0x20 || Read_Tele[i+1]>=0x7f) aa=' ';
		else aa=Read_Tele[i+1];
		
		str.Format("%c",aa);
		Print_Log(&m_readtele,str);
	}

	for(i=0;i<5;i++)
	{
		str="\n";
		Print_Log(&m_readtele,str);
	}

	for(i=0;i<128;i++)
	{
		if(Read_Tele[i+3]<=0x20 || Read_Tele[i+3]>=0x7f) aa=' ';
		else aa=Read_Tele[i+3];

		str.Format("%c",aa);
		Print_Log(&m_readtele,str);
	}

	m_readtele.Close();
*/

	CString str,ss;
	int i;
	char aa;

	ss="";
	for(i=0;i<4;i++)
	{
		aa=Read_Tele[i+5];
		str.Format("%c",aa);
		ss+=str;
	}
	m_edit1.SetWindowText(ss);

	ss="";
	for(i=0;i<13;i++)
	{
		aa=Read_Tele[i+5+6];
		str.Format("%c",aa);
		ss+=str;
	}
	m_edit2.SetWindowText(ss);

	ss="";
	for(i=0;i<10;i++)
	{
		aa=Read_Tele[i+5+6+19];
		str.Format("%c",aa);
		ss+=str;
	}
	m_edit3.SetWindowText(ss);

	ss="";
	for(i=0;i<2;i++)
	{
		aa=Read_Tele[i+5+6+19+14];
		str.Format("%c",aa);
		ss+=str;
	}
	m_edit4.SetWindowText(ss);

	ss="";
	for(i=0;i<8;i++)
	{
		aa=Read_Tele[i+5+6+19+14+16];
		str.Format("%c",aa);
		ss+=str;
	}
	m_edit5.SetWindowText(ss);

	ss="";
	for(i=0;i<2;i++)
	{
		aa=Read_Tele[i+5+6+19+14+16+8];
		str.Format("%c",aa);
		ss+=str;
	}
	m_edit6.SetWindowText(ss);

	return TRUE;

}


void CReadManufacture::Clear_Dispaly()
{
	CString ss;

	ss="";
	m_edit1.SetWindowText(ss);
	m_edit2.SetWindowText(ss);
	m_edit3.SetWindowText(ss);
	m_edit4.SetWindowText(ss);
	m_edit5.SetWindowText(ss);
	m_edit6.SetWindowText(ss);
	m_Result.SetWindowText(ss);
}