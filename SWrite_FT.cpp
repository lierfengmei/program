// SWrite_FT.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "program_balise.h"
#include "SWrite_FT.h"
#include ".\swrite_ft.h"
#include "Comm_Balise_ts.h"
#include <sys/timeb.h>
#include <time.h>

// CSWrite_FT �Ի���

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


// CSWrite_FT ��Ϣ�������

void CSWrite_FT::OnBnClickedButtonWriteFt()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int len,len_data;
	byte ch[1024];
	int pos=0;
	byte temp;
	int i;
	int count;

	//����
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

	//����
	len_data=pos-3;
	ch[1]=(byte)(len_data & 0xff);
	len_data>>=8;
	ch[2]=(byte)(len_data & 0xff);

	len=pos;
	SendCommand(len,&ch[0]);

	count=0;
	do{
		//��������
		Read_Tele_Len=0;
		ReceiveData(READLEN_LEU_WRITE_FT);

		if(ReadBufLen>0)
		{
			if(ArrangeData(ORDER_LEU_WRITE_FT))
			{
				//���ı�ʶ
				temp=Read_Tele[0];
				if(temp==0x02)
				{
					MessageBox("����ʧ�ܣ�����ͨ��У�����","����",MB_OK);
					return ;
				}
				else if(temp==0x12)
				{
					MessageBox("����ʧ�ܣ������źŲ�������","����",MB_OK);
					return ;
				}
				else if(temp==0x11)
				{
					MessageBox("�����ɹ�","�ɹ�",MB_OK);
					return ;
				}
				else
				{
					MessageBox("�������ݸ�ʽ����","����",MB_OK);
					return ;
				}
			}
		}

		count++;
		Sleep(1000);
	}while(count<10);
	
	if(count>=10)
	{
		MessageBox("����ʧ�ܣ�ͨ��δ������","����",MB_OK);
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
		MessageBox("��Ч����ݣ�����������!\n\n��ݵ���Ч��Χ1970 ~ 3000��","����",MB_OK);
		m_edit_time[0].SetFocus();
		return FALSE;
	}

	m_edit_time[1].GetWindowText(str);
	m_time_month=atoi(str);

	if(m_time_month<1 || m_time_month>12) 
	{
		MessageBox("��Ч���·ݣ�����������!\n\n�·ݵ���Ч��Χ1 ~ 12��","����",MB_OK);
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
			MessageBox("��Ч�����ڣ�����������!\n\n���ڵ���Ч��Χ1 ~ 29��","����",MB_OK);
			m_edit_time[2].SetFocus();
			return FALSE;
		}
	}
	else if((m_time_month==2) && (m_time_day<1 || m_time_day>28))
	{
		MessageBox("��Ч�����ڣ�����������!\n\n���ڵ���Ч��Χ1 ~ 28��","����",MB_OK);
		m_edit_time[2].SetFocus();
		return FALSE;
	}
	
	if(m_time_month==1 || m_time_month==3 || m_time_month==5 || m_time_month==7 || m_time_month==8 || m_time_month==10 || m_time_month==12)
	{
		if(m_time_day<1 || m_time_day>31)
		{
			MessageBox("��Ч�����ڣ�����������!\n\n���ڵ���Ч��Χ1 ~ 31��","����",MB_OK);
			m_edit_time[2].SetFocus();
			return FALSE;
		}
	}
    else if(m_time_day<1 || m_time_day>30) 
	{
		MessageBox("��Ч�����ڣ�����������!\n\n���ڵ���Ч��Χ1 ~ 30��","����",MB_OK);
		m_edit_time[2].SetFocus();
		return FALSE;
	}

	m_edit_time[3].GetWindowText(str);
	m_time_hour=atoi(str);
	if(m_time_hour<0 || m_time_hour>23) 
	{
		MessageBox("��Ч��Сʱ������������!\n\nСʱ����Ч��Χ0 ~ 23��","����",MB_OK);
		m_edit_time[3].SetFocus();
		return FALSE;
	}

	m_edit_time[4].GetWindowText(str);
	m_time_miniute=atoi(str);
	if(m_time_miniute<0 || m_time_miniute>59) 
	{
		MessageBox("��Ч�ķ��ӣ�����������!\n\n���ӵ���Ч��Χ0 ~ 59��","����",MB_OK);
		m_edit_time[4].SetFocus();
		return FALSE;
	}

	m_edit_time[5].GetWindowText(str);
	m_time_second=atoi(str);
	if(m_time_second<0 || m_time_second>59) 
	{
		MessageBox("��Ч���룬����������!\n\n�����Ч��Χ0 ~ 59��","����",MB_OK);
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

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
	// �쳣: OCX ����ҳӦ���� FALSE
}


