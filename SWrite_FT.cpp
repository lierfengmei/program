// SWrite_FT.cpp : 实现文件
//

#include "stdafx.h"
#include "program_balise.h"
#include "SWrite_FT.h"
#include ".\swrite_ft.h"
#include "Comm_Balise_ts.h"
#include <sys/timeb.h>
#include <time.h>

// CSWrite_FT 对话框

IMPLEMENT_DYNAMIC(CSWrite_FT, CDialog)
CSWrite_FT::CSWrite_FT(CWnd* pParent /*=NULL*/)
	: CDialog(CSWrite_FT::IDD, pParent)
	, m_time_year(0)
	, m_time_month(0)
	, m_time_day(0)
	, m_time_hour(0)
	, m_time_miniute(0)
	, m_time_second(0)
{
}

CSWrite_FT::~CSWrite_FT()
{
}

void CSWrite_FT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit_time[0]);
	DDX_Control(pDX, IDC_EDIT2, m_edit_time[1]);
	DDX_Control(pDX, IDC_EDIT3, m_edit_time[2]);
	DDX_Control(pDX, IDC_EDIT4, m_edit_time[3]);
	DDX_Control(pDX, IDC_EDIT5, m_edit_time[4]);
	DDX_Control(pDX, IDC_EDIT6, m_edit_time[5]);
}


BEGIN_MESSAGE_MAP(CSWrite_FT, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_FT, OnBnClickedButtonWriteFt)
END_MESSAGE_MAP()


// CSWrite_FT 消息处理程序

void CSWrite_FT::OnBnClickedButtonWriteFt()
{
	// TODO: 在此添加控件通知处理程序代码
	int len,len_data;
	byte ch[1024];
	int pos=0;
	byte temp;
	int i;
	int count;

	//命令
	pos=0;
	ch[pos++]=0x26;
	ch[pos++]=0x00;
	ch[pos++]=0x00;

	if(!Load_Time()) return ;
	for(i=0;i<4;i++)
	{
		ch[pos++]=(byte)(send_time & 0xff);
		send_time>>=8;
	}

	//长度
	len_data=pos-3;
	ch[1]=(byte)(len_data & 0xff);
	len_data>>=8;
	ch[2]=(byte)(len_data & 0xff);

	len=pos;
	SendCommand(len,&ch[0]);

	count=0;
	do{
		//接收数据
		Read_Tele_Len=0;
		ReceiveData(READLEN_LEU_WRITE_FT);

		if(ReadBufLen>0)
		{
			if(ArrangeData(ORDER_LEU_WRITE_FT))
			{
				//报文标识
				temp=Read_Tele[0];
				if(temp==0x02)
				{
					MessageBox("操作失败：数据通信校验错误！","错误",MB_OK);
					return ;
				}
				else if(temp==0x12)
				{
					MessageBox("操作失败：握手信号不正常！","错误",MB_OK);
					return ;
				}
				else if(temp==0x11)
				{
					MessageBox("操作成功","成功",MB_OK);
					return ;
				}
				else
				{
					MessageBox("接收数据格式错误","错误",MB_OK);
					return ;
				}
			}
		}

		count++;
		Sleep(1000);
	}while(count<10);
	
	if(count>=10)
	{
		MessageBox("操作失败：通信未建立！","错误",MB_OK);
		return ;
	}

	return ;
}

BOOL CSWrite_FT::Load_Time()
{
	
	//_time64( &ltime );

	//struct tm *today;
	CTime  CurrentTime;
	__time64_t ltime;
	CString str;
	bool leap_year;

	m_edit_time[0].GetWindowText(str);
	m_time_year=atoi(str);
	if(m_time_year<1970 || m_time_year>2100) 
	{
		MessageBox("无效的年份，请重新输入!\n\n年份的有效范围1970 ~ 3000。","错误",MB_OK);
		m_edit_time[0].SetFocus();
		return FALSE;
	}

	m_edit_time[1].GetWindowText(str);
	m_time_month=atoi(str);

	if(m_time_month<1 || m_time_month>12) 
	{
		MessageBox("无效的月份，请重新输入!\n\n月份的有效范围1 ~ 12。","错误",MB_OK);
		m_edit_time[1].SetFocus();
		return FALSE;
	}

	m_edit_time[2].GetWindowText(str);
	m_time_day=atoi(str);
	if(m_time_year%100==0)
	{
		if(m_time_year%400==0) leap_year=true;
		else leap_year=false;
	}
	else
	{
		if(m_time_year%4==0) leap_year=true;
		else leap_year=false;
	}	
	if(leap_year && (m_time_month==2))
	{
		if(m_time_day<1 || m_time_day>29)
		{
			MessageBox("无效的日期，请重新输入!\n\n日期的有效范围1 ~ 29。","错误",MB_OK);
			m_edit_time[2].SetFocus();
			return FALSE;
		}
	}
	else if((m_time_month==2) && (m_time_day<1 || m_time_day>28))
	{
		MessageBox("无效的日期，请重新输入!\n\n日期的有效范围1 ~ 28。","错误",MB_OK);
		m_edit_time[2].SetFocus();
		return FALSE;
	}
	
	if(m_time_month==1 || m_time_month==3 || m_time_month==5 || m_time_month==7 || m_time_month==8 || m_time_month==10 || m_time_month==12)
	{
		if(m_time_day<1 || m_time_day>31)
		{
			MessageBox("无效的日期，请重新输入!\n\n日期的有效范围1 ~ 31。","错误",MB_OK);
			m_edit_time[2].SetFocus();
			return FALSE;
		}
	}
    else if(m_time_day<1 || m_time_day>30) 
	{
		MessageBox("无效的日期，请重新输入!\n\n日期的有效范围1 ~ 30。","错误",MB_OK);
		m_edit_time[2].SetFocus();
		return FALSE;
	}

	m_edit_time[3].GetWindowText(str);
	m_time_hour=atoi(str);
	if(m_time_hour<0 || m_time_hour>23) 
	{
		MessageBox("无效的小时，请重新输入!\n\n小时的有效范围0 ~ 23。","错误",MB_OK);
		m_edit_time[3].SetFocus();
		return FALSE;
	}

	m_edit_time[4].GetWindowText(str);
	m_time_miniute=atoi(str);
	if(m_time_miniute<0 || m_time_miniute>59) 
	{
		MessageBox("无效的分钟，请重新输入!\n\n分钟的有效范围0 ~ 59。","错误",MB_OK);
		m_edit_time[4].SetFocus();
		return FALSE;
	}

	m_edit_time[5].GetWindowText(str);
	m_time_second=atoi(str);
	if(m_time_second<0 || m_time_second>59) 
	{
		MessageBox("无效的秒，请重新输入!\n\n秒的有效范围0 ~ 59。","错误",MB_OK);
		m_edit_time[5].SetFocus();
		return FALSE;
	}

	CTime ct(m_time_year,m_time_month,m_time_day,m_time_hour,m_time_miniute,m_time_second);

	ltime=ct.GetTime();

	send_time=ltime+(8*3600);

/*	today = _localtime64( &ltime );
	
	//_ftime64(today);

	//ltime=_time64( today );
*/	

	return TRUE;
}

BOOL CSWrite_FT::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	CTime  CurrentTime;
	int ddd;
	CString str;
	int len[6]={4,2,2,2,2,2};

	for(int i=0;i<6;i++)
	{
		m_edit_time[i].SetLimitText(len[i]);
	}

	CurrentTime=CTime::GetCurrentTime();

	ddd=CurrentTime.GetYear();
	str.Format("%d",ddd);
	m_edit_time[0].SetWindowText(str);
	
	ddd=CurrentTime.GetMonth();
	str.Format("%d",ddd);
	m_edit_time[1].SetWindowText(str);

	ddd=CurrentTime.GetDay();
	str.Format("%d",ddd);
	m_edit_time[2].SetWindowText(str);

	ddd=CurrentTime.GetHour();
	str.Format("%d",ddd);
	m_edit_time[3].SetWindowText(str);

	ddd=CurrentTime.GetMinute();
	str.Format("%d",ddd);
	m_edit_time[4].SetWindowText(str);

	ddd=CurrentTime.GetSecond();
	str.Format("%d",ddd);
	m_edit_time[5].SetWindowText(str);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


