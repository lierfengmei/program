// SErase_Msg.cpp : 实现文件
//

#include "stdafx.h"
#include "program_balise.h"
#include "SErase_Msg.h"
#include ".\serase_msg.h"
#include "Comm_Balise_ts.h"

// CSErase_Msg 对话框

IMPLEMENT_DYNAMIC(CSErase_Msg, CDialog)
CSErase_Msg::CSErase_Msg(CWnd* pParent /*=NULL*/)
	: CDialog(CSErase_Msg::IDD, pParent)
{
}

CSErase_Msg::~CSErase_Msg()
{
}

void CSErase_Msg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSErase_Msg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ERASE_TSE, OnBnClickedButtonEraseTse)
	ON_BN_CLICKED(IDC_BUTTON_ERASE_TPC, OnBnClickedButtonEraseTpc)
	ON_BN_CLICKED(IDC_BUTTON_ERASE_TE1, OnBnClickedButtonEraseTe1)
	ON_BN_CLICKED(IDC_BUTTON_ERASE_THM, OnBnClickedButtonEraseThm)
	ON_BN_CLICKED(IDC_BUTTON_ERASE_THE, OnBnClickedButtonEraseThe)
	ON_BN_CLICKED(IDC_BUTTON_ERASE_TE3, OnBnClickedButtonEraseTe3)
	ON_BN_CLICKED(IDC_BUTTON_ERASE_TE2, OnBnClickedButtonEraseTe2)
	ON_BN_CLICKED(IDC_BUTTON_ERASE_TE4, OnBnClickedButtonEraseTe4)
	ON_BN_CLICKED(IDC_BUTTON_ERASE_TE5, OnBnClickedButtonEraseTe5)
	ON_BN_CLICKED(IDC_BUTTON_ERASE_TE6, OnBnClickedButtonEraseTe6)
END_MESSAGE_MAP()


// CSErase_Msg 消息处理程序

void CSErase_Msg::OnBnClickedButtonEraseTse()
{
	// TODO: 在此添加控件通知处理程序代码
	Erase_Message(ORDER_LEU_TSE);
}

void CSErase_Msg::OnBnClickedButtonEraseTpc()
{
	// TODO: 在此添加控件通知处理程序代码
	Erase_Message(ORDER_LEU_TPC);
}


void CSErase_Msg::OnBnClickedButtonEraseThm()
{
	// TODO: 在此添加控件通知处理程序代码
	Erase_Message(ORDER_LEU_THM);
}

void CSErase_Msg::OnBnClickedButtonEraseThe()
{
	// TODO: 在此添加控件通知处理程序代码
	Erase_Message(ORDER_LEU_THE);
}

void CSErase_Msg::OnBnClickedButtonEraseTe1()
{
	// TODO: 在此添加控件通知处理程序代码
	Erase_Message(ORDER_LEU_TE1);
}

void CSErase_Msg::OnBnClickedButtonEraseTe2()
{
	// TODO: 在此添加控件通知处理程序代码
	Erase_Message(ORDER_LEU_TE2);
}

void CSErase_Msg::OnBnClickedButtonEraseTe3()
{
	// TODO: 在此添加控件通知处理程序代码
	Erase_Message(ORDER_LEU_TE3);
}


void CSErase_Msg::OnBnClickedButtonEraseTe4()
{
	// TODO: 在此添加控件通知处理程序代码
	Erase_Message(ORDER_LEU_TE4);
}

void CSErase_Msg::OnBnClickedButtonEraseTe5()
{
	// TODO: 在此添加控件通知处理程序代码
	Erase_Message(ORDER_LEU_TE5);
}

void CSErase_Msg::OnBnClickedButtonEraseTe6()
{
	// TODO: 在此添加控件通知处理程序代码
	Erase_Message(ORDER_LEU_TE6);
}


BOOL CSErase_Msg::Erase_Message(int type)
{
	int len,len_data;
	byte ch[1024];
	int pos=0;
	byte temp;
	int count;

	//命令
	pos=0;
	ch[pos++]=0x24;
	ch[pos++]=0x00;
	ch[pos++]=0x00;
	ch[pos++]=type;

	//长度
	len_data=pos-3;
	ch[1]=(byte)(len_data & 0xff);
	len_data>>=8;
	ch[2]=(byte)(len_data & 0xff);

	len=pos;
	Read_Tele_Len=0;
	SendCommand(len,&ch[0]);

	count=0;
	do{
		//接收数据
		Read_Tele_Len=0;
		ReceiveData(READLEN_LEU_ERASE);

		if(ReadBufLen>0)
		{
			if(ArrangeData(ORDER_LEU_ERASE))
			{
				//报文标识
				temp=Read_Tele[0];
				if(temp==0x02)
				{
					MessageBox("操作失败：数据通信校验错误！","错误",MB_OK);
					return FALSE;
				}
				else if(temp==0x12)
				{
					MessageBox("操作失败：握手信号不正常！","错误",MB_OK);
					return FALSE;
				}
				else if(temp==0x11)
				{
					MessageBox("操作成功","成功",MB_OK);
					return TRUE;
				}
				else
				{
					MessageBox("接收数据格式错误","错误",MB_OK);
					return FALSE;
				}
			}
		}

		Sleep(500);
		count++;
	}while(count<30);

	if(count>29)
	{
		MessageBox("操作失败：通信未建立！","错误",MB_OK);
		return FALSE;
	}

	return TRUE;
}