// BaliseCMeas.cpp : 实现文件
//

#include "stdafx.h"
#include "program_balise.h"
#include "BaliseCMeas.h"
#include "BmpStatic.h"
#include "Comm_Balise_ts.h"

// CBaliseCMeas 对话框

IMPLEMENT_DYNAMIC(CBaliseCMeas, CDialog)
CBaliseCMeas::CBaliseCMeas(CWnd* pParent /*=NULL*/)
	: CDialog(CBaliseCMeas::IDD, pParent)
{
}

CBaliseCMeas::~CBaliseCMeas()
{
}

void CBaliseCMeas::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT13, m_telegramfile);
	DDX_Control(pDX, IDC_LIST1, m_telegram);
	DDX_Control(pDX, IDC_EDIT1, m_Result);
	DDX_Control(pDX, IDC_STATIC4, m_Result_Bmp);
	DDX_Control(pDX, IDC_EDIT2, m_Have_C4);
}


BEGIN_MESSAGE_MAP(CBaliseCMeas, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton_OpenFile)
	ON_BN_CLICKED(IDC_BUTTON_MEAS_C, OnBnClickedButtonMeasC)
END_MESSAGE_MAP()


// CBaliseCMeas 消息处理程序

BOOL CBaliseCMeas::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Result_Bmp.SetResultBmp(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CBaliseCMeas::OnBnClickedButton_OpenFile()
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

void CBaliseCMeas::OnBnClickedButtonMeasC()
{
	// TODO: 在此添加控件通知处理程序代码
	int len,len_data;
	byte ch[1024];
	int pos=0;
	int i;
	CString ss,ss1;
	byte temp;
	CString filename;
	int total_len;
	bool flag=false;
	BOOL m_same=FALSE;

	Clear_Display();

	m_Result_Bmp.SetResultBmp(0);
	
	m_Result.SetWindowText("");
	m_Result.UpdateWindow();
	m_Have_C4.SetWindowText("");
	m_Have_C4.UpdateWindow();

	m_telegramfile.GetWindowText(filename);
	if(!InitTelegram(filename,&c5_orig[0]))
	{
		MessageBox("无法打开报文文件,\n请选择报文文件！","错误",MB_OK);
		return;
	}

	//复制原始数据
	for(i=0;i<(c5_orig[0]+1);i++) c5_send[i]=c5_orig[i];

	//短报文：43字节，最后3个无效位。
	if(c5_send[0]==43)
	{
		left_shift_bits(&c5_send[1],c5_send[0],3,3,&total_len);
		c5_send[0]=(byte)(total_len & 0xff);
	}

	//显示原始报文
	//******************************************
	m_telegram.AddString("C接口传输报文");
	ss="";
	for(i=0;i<c5_orig[0];i++)
	{
		ss1.Format("%02X  ",c5_orig[i+1]);
		ss+=ss1;
		if((i+1)%16==0)
		{
			m_telegram.AddString(ss);
			ss="";
		}
	}
	m_telegram.AddString(ss);
	//******************************************

	//报文整理
	//******************************************

	//******************************************

	//发送写命令
	//******************************************
	ch[pos++]=0x40;
	ch[pos++]=0x00;//
	ch[pos++]=0x00;//

	for(i=0;i<128;i++) ch[pos++]=c5_send[i+1];

	len_data=pos-3;
	ch[1]=(byte)(len_data & 0xff);
	len_data>>=8;
	ch[2]=(byte)(len_data & 0xff);

	len=pos;

	SendCommand(len,&ch[0]);

	//***************************************

	//**************************************
	Read_Tele_Len=0;
	Sleep(1000);
	ReceiveData(READLEN_C_SEND_TELE);

	if(ReadBufLen>0)
	{
		if(ArrangeData(ORDER_C_SEND_TELE))
		{
			temp=Read_Tele[0];
			switch(temp)
			{
			case 0x01:
				//ss="接收数据正常。";
				//flag=true;
				break;
			case 0x02:
				ss="写操作失败：数据通信校验错误！";
				flag=false;
				break;
			case 0x20:
				ss="A4读失败：未读到报文！";
				flag=false;
				break;
			case 0x21: //短报文
			case 0x22: //长报文
				ss="C 接口功能正常。";
				flag=true;
				break;
			case 0x30:
				ss="C 接口功能故障！";
				flag=false;
				break;
			default:
				break;
			}
			if(temp==0x21 || temp==0x22)
			{
				//c4标识
				c4_flag=Read_Tele[1];
				if(c4_flag==0) m_Have_C4.SetWindowText("被测应答器无C4接口。");
				else if(c4_flag==1) m_Have_C4.SetWindowText("被测应答器有C4接口。");

				//报文
				if(temp==0x21) len=43;
				if(temp==0x22) len=128;
				a4_read[0]=len;
				for(i=0;i<len;i++)
					a4_read[i+1]=Read_Tele[i+2];

				//比较报文
				if(temp==0x21) 
				{
					a4_read[43]&=0xf8;
					c5_orig[43]&=0xf8;
				}
				else if(temp==0x22)
				{
					a4_read[128]&=0xfe;
					c5_orig[128]&=0xfe;
				}

				m_same=TRUE;
				for(i=0;i<len;i++)
				{
					if(c5_orig[i+1]!=a4_read[i+1])
					{
						m_same=FALSE;
						break;
					}
				}
				if(m_same)
				{
					ss="C 接口功能正常。";
					flag=true;
				}
				else 
				{
					ss="C 接口：报文数据不一致！";
					flag=false;
				}
			}
		}
	}
	else 
	{
		ss="写操作失败：通信未建立！";
		flag=false;
	}

	//*********************************************

	//显示结论
	//*********************************************
	m_Result.SetWindowText(ss);
	if(flag) 
	{
		m_Result_Bmp.SetResultBmp(1);
	}
	else 
	{
		m_Result_Bmp.SetResultBmp(2);
	}

    //****************************************
}

void CBaliseCMeas::Clear_Display()
{
	m_telegram.ResetContent();
	for(int i=0;i<129;i++)
	{
		c5_send[i]=0x00;
	}
	c4_flag=0;
}
