// A5_Read.cpp : 实现文件
//

#include "stdafx.h"
#include "program_balise.h"
#include "A5_Read.h"
#include "Comm_Balise_ts.h"
#include "afxwin.h"

// CA5_Read 对话框

IMPLEMENT_DYNAMIC(CA5_Read, CDialog)
CA5_Read::CA5_Read(CWnd* pParent /*=NULL*/)
	: CDialog(CA5_Read::IDD, pParent)
{
}

CA5_Read::~CA5_Read()
{
}

void CA5_Read::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TELEGRAM, m_telegram);
	DDX_Control(pDX, IDC_EDIT13, m_telegramfile);
	DDX_Control(pDX, IDC_EDIT14, m_result);
	DDX_Control(pDX, IDC_STATIC1, m_Static_Silent);
}


BEGIN_MESSAGE_MAP(CA5_Read, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_A5_READ, OnBnClickedButtonA5Read)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton_OpenFile)
END_MESSAGE_MAP()


// CA5_Read 消息处理程序
void CA5_Read::Clear_Display()
{
	m_telegram.ResetContent();
	m_result.SetWindowText("");
	for(int i=0;i<129;i++)
	{
		a5_send[i]=0x00;
		a5_read[i]=0x00;
	}
}

void CA5_Read::OnBnClickedButtonA5Read()
{
	// TODO: 在此添加控件通知处理程序代码
	int len;
	byte ch[1024];
	int pos=0;
	int i;
	CString ss,ss1;
	byte temp;
	CString filename;
	BOOL m_same;
    
	Clear_Display();

	m_telegramfile.GetWindowText(filename);
	if(filename!="") InitTelegram(filename,&a5_send[0]);

	//A4读发送命令
    ch[pos++]=0x12;
	ch[pos++]=0x00;
	ch[pos++]=0x00;

	len=pos;
	SendCommand(len,&ch[0]);

	//接收数据
	Sleep(500);
	ReceiveData();

	if(ReadBufLen>0)
	{
		if(ArrangeData())
		{
			//报文标识
			temp=Read_Tele[0];
			switch(temp)
			{
			case 0x20:
				ss="A5读：无报文";
				break;
			case 0x21:
				ss="A5读：有报文";
				break;
			}
			m_telegram.AddString(ss);

			if(temp==0x21)
			{
				//分析数据
				Analyse_Tele_Data();

				ss="";
				//len=128;
				//a5_read[0]=128; //???????
			
				//显示报文
				for(i=0;i<len;i++)
				{
					//a5_read[i+1]=Read_Tele[i+1];
					ss1.Format("%02x  ",a5_read[i+1]);
					ss+=ss1;
					if(i>0 && (i+1)%16==0)
					{
						m_telegram.AddString(ss);
						ss="";
					}
				}
				m_telegram.AddString(ss);
			}
		}

		//比较
		if(filename!="" && temp==0x21) 
		{
			len=a5_read[0];
			m_same=TRUE;
			for(i=0;i<len;i++)
			{
				if(a5_send[i+1]!=a5_read[i+1])
				{
					m_same=FALSE;
					break;
				}
			}
			if(m_same) m_result.SetWindowText("一致");
			else m_result.SetWindowText("不一致");
		}
	}
	else
	{
		ss="A5读：无通信数据。";
		m_telegram.AddString(ss);
	}
}


void CA5_Read::OnBnClickedButton_OpenFile()
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

	m_telegramfile.SetWindowText(filepath);

}

void CA5_Read::Analyse_Tele_Data()
{
	int len;

	len=128;
	a5_read[0]=len; //???????

	//注：最后报文存到a5_read[]数组中，
	//a5_read[0]:报文数据长度
	//a5_read[1] ~ [128]: 报文数据
	
	//原始数据在Read_Tele[]中
	//Read_Tele[0]:为接收标识
	//Read_Tele[1]~....[512]:数据

	//分析报文
	
}


BOOL CA5_Read::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_Static_Silent.SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_SILENT)));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
