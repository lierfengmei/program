// WriteOperationMode.cpp : 实现文件
//

#include "stdafx.h"
#include "BmpStatic.h"
#include "program_balise.h"
#include "WriteOperationMode.h"
#include "Comm_Balise_ts.h"

// CWriteOperationMode 对话框

IMPLEMENT_DYNAMIC(CWriteOperationMode, CDialog)
CWriteOperationMode::CWriteOperationMode(CWnd* pParent /*=NULL*/)
	: CDialog(CWriteOperationMode::IDD, pParent)
{
}

CWriteOperationMode::~CWriteOperationMode()
{
}

void CWriteOperationMode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Result);
	DDX_Control(pDX, IDC_RADIO_SILENT, m_Silent);
	DDX_Control(pDX, IDC_RADIO_FUNCTIONAL, m_Functional);
	DDX_Control(pDX, IDC_STATIC3, m_Result_Bmp);
}


BEGIN_MESSAGE_MAP(CWriteOperationMode, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_WRITE, OnBnClickedButtonWrite)
END_MESSAGE_MAP()


// CWriteOperationMode 消息处理程序

void CWriteOperationMode::OnBnClickedButtonWrite()
{
	// TODO: 在此添加控件通知处理程序代码
	byte mode[2]={0x5d,0x5c};
	byte ch[1024];
	int pos=0;
	int check;
	int i;
	CString ss="";
	int len,len_data;
	byte temp;
	bool flag=false;

	if(m_Silent.GetCheck()==BST_CHECKED) check=0;
	else if(m_Functional.GetCheck()==BST_CHECKED) check=1;
	else return;

	m_Result.SetWindowText("");
	m_Result.UpdateWindow();
	
	m_Result_Bmp.SetResultBmp(0);

	ch[pos++]=0x11;
	ch[pos++]=0x00;
	ch[pos++]=0x00;

	ch[pos++]=mode[check];

	len_data=pos-3;
	ch[1]=(byte)(len_data & 0xff);
	len_data>>=8;
	ch[2]=(byte)(len_data & 0xff);

	len=pos;

	for(i=0;i<5;i++)
	{
		SendCommand(len,&ch[0]);
		//接收数据
		Read_Tele_Len=0;
		Sleep(500);
		ReceiveData(READLEN_A5_WRITE_MODE);

		if(ReadBufLen>0)
		{
			if(ArrangeData(ORDER_A5_WRITE_MODE))
			{
				//标识
				ss="";
				temp=Read_Tele[0];
				if(temp==0x02)
				{
					ss="写操作失败：数据通信校验错误";
					flag=false;
					break;
				}
				else if(temp==0x12)
				{
					ss="写操作失败：握手信号错误";
					flag=false;
					break;
				}
				else
				{
					ss="写操作模式：写入正常。";
					flag=true;
				}
			}
		}
		else
		{
			ss="写操作失败：通信未建立！";
			flag=false;
			break;
		}
	}

	m_Result.SetWindowText(ss);

	if(flag)
	{
		m_Result_Bmp.SetResultBmp(1);
	}
	else 
	{
		m_Result_Bmp.SetResultBmp(2);
	}
    
}

BOOL CWriteOperationMode::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Silent.SetCheck(BST_CHECKED);
	m_Functional.SetCheck(BST_UNCHECKED);
	
	m_Result_Bmp.SetResultBmp(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
