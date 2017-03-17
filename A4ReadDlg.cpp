// A4ReadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "program_balise.h"
#include "A4ReadDlg.h"
#include "Comm_Balise_ts.h"
#include "Const_d.h"
#include ".\a4readdlg.h"

// CA4ReadDlg 对话框

IMPLEMENT_DYNAMIC(CA4ReadDlg, CDialog)
CA4ReadDlg::CA4ReadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CA4ReadDlg::IDD, pParent)
{
}

CA4ReadDlg::~CA4ReadDlg()
{
}

void CA4ReadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_telegram);
	DDX_Control(pDX, IDC_LIST2, m_usertelegram);
	DDX_Control(pDX, IDC_LIST4, m_header);
	DDX_Control(pDX, IDC_RADIO_A4, m_A4Read);
	DDX_Control(pDX, IDC_RADIO_A5, m_A5Read);
	DDX_Control(pDX, IDC_STATIC_FUNCTIONAL, m_Functional_Bmp);
	DDX_Control(pDX, IDC_STATIC_SILENT, m_Silent_Bmp);
	DDX_Control(pDX, IDC_EDIT14,m_result);
	DDX_Control(pDX, IDC_EDIT13,m_telegramfile);
}


BEGIN_MESSAGE_MAP(CA4ReadDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_A4_READ, OnBnClickedButtonA4Read)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton_OpenFile)
	ON_LBN_SELCHANGE(IDC_LIST1, OnLbnSelchangeList1)
END_MESSAGE_MAP()


// CA4ReadDlg 消息处理程序
BOOL CA4ReadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//m_telegramfile.SetWindowText("telegram.txt");

	m_A4Read.SetCheck(BST_CHECKED);
	m_A5Read.SetCheck(BST_UNCHECKED);

	m_Functional_Bmp.SetResultBmp(0);
	m_Silent_Bmp.SetResultBmp(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}


void CA4ReadDlg::OnBnClickedButtonA4Read()
{
	// TODO: 在此添加控件通知处理程序代码
	int len,len1,len_data;
	byte ch[1024];
	int pos=0;
	int i;
	CString ss,ss1;
	byte temp,temp1;
	CString filename;
	BOOL m_same;
	//BOOL m_nofile;
	int check;
	UINT long_or_short;

	Clear_Display();

	m_telegramfile.GetWindowText(filename);
	if(filename!="")	
	{
		if(!InitTelegram(filename,&a4_send[0])) 
		{
			MessageBox("报文文件格式不正确，\n请重新选择文件！","错误",MB_OK);
			return;
		}
	}

	if(m_A4Read.GetCheck()==BST_CHECKED) check=0;
	else if(m_A5Read.GetCheck()==BST_CHECKED) check=1;
	else return;

	//读发送命令
	if(check==0) //A4读
	{
		pos=0;
		ch[pos++]=ORDER_A4_READ_TELE;
		ch[pos++]=0x00;
		ch[pos++]=0x00;

		len=pos;
		SendCommand(len,&ch[0]);

		//接收数据
		Read_Tele_Len=0;
		Sleep(500);
		ReceiveData(READLEN_A4_READ_TELE);

		if(ReadBufLen>0)
		{
			if(ArrangeData(ORDER_A4_READ_TELE))
			{
				//************************************************
				//报文标识
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
					ss="A4读：短报文。";
					break;
				case 0x22:
					ss="A4读：长报文。";
					break;
				}
				m_telegram.AddString(ss);

				ss="";
				len=0;
				if(temp==0x21) len=43;
				else if(temp==0x22) len=128;
				a4_read[0]=len; //???????
				
				//显示报文
				for(i=0;i<len;i++)
				{
					a4_read[i+1]=Read_Tele[i+2];
					ss1.Format("%02x  ",Read_Tele[i+2]);
					ss+=ss1;
					if(i>0 && (i+1)%16==0)
					{
						m_telegram.AddString(ss);
						ss="";
					}

				}
				m_telegram.AddString(ss);
				//************************************************
				
				//***********************************************
				//用户报文标识
				temp1=Read_Tele[1];
				switch(temp1)
				{
				case 0x20:
					ss="A4读：无用户报文。";
					break;
				case 0x21:
					ss="A4读：短用户报文。";
					break;
				case 0x22:
					ss="A4读：长用户报文。";
					break;
				}
				m_usertelegram.AddString(ss);

				ss="";
				len1=0;
				if(temp1==0x21) len1=27;
				else if(temp1==0x22) len1=104;
				
				for(i=0;i<len1;i++)
				{
					usertele[i]=Read_Tele[i+2+len];
					ss1.Format("%02x  ",Read_Tele[i+2+len]);
					ss+=ss1;
					if(i>0 && (i+1)%16==0)
					{
						m_usertelegram.AddString(ss);
						ss="";
					}
				}
				m_usertelegram.AddString(ss);
				//******************************************************

				//******************************************************
				Copy_UserTele(temp_usertele,usertele,len1);
				Analyse_Usertele_Header(&(this->m_header),temp_usertele,len1);
				//******************************************************
				Copy_UserTele(temp_tele,&a4_read[1],a4_read[0]);
				Copy_UserTele(temp_usertele,usertele,len1);
				if(!Analyse_Usertele_Info(ANALYSE_USERTELE_BALISE_A4,temp_usertele,len1,temp_tele,a4_read[0]))
					MessageBox("分析用户报文错误！","错误",MB_OK);
			}

			//************************************************
			//比较
			if(filename!="" && (temp==0x21 || temp==0x22))
			{
				if(temp==0x21) 
				{
					a4_send[43]&=0xf8;
				}

				len=a4_read[0];
				m_same=TRUE;
				for(i=0;i<len;i++)
				{
					if(a4_send[i+1]!=a4_read[i+1])
					{
						m_same=FALSE;
						break;
					}
				}
				if(m_same) m_result.SetWindowText("一致");
				else m_result.SetWindowText("不一致");
			}
			//*************************************************
		}
		else 
		{
			MessageBox("读操作失败：通信未建立！","错误",MB_OK);
		}
	}
	else if(check==1)//A5
	{
		pos=0;
		ch[pos++]=ORDER_A5_READ_TELE;
		ch[pos++]=0x00;
		ch[pos++]=0x00;
		ch[pos++]=0x5e;

		len_data=pos-3;
		ch[1]=(byte)(len_data & 0xff);
		len_data>>=8;
		ch[2]=(byte)(len_data & 0xff);

		len=pos;
		SendCommand(len,&ch[0]);

		//接收数据
		Read_Tele_Len=0;
		Sleep(500);
		ReceiveData(READLEN_A5_READ_TELE);

		if(ReadBufLen>0)
		{
			if(ArrangeData(ORDER_A5_READ_TELE))
			{
				//************************************************
				//报文标识
				temp=Read_Tele[0];
				switch(temp)
				{
				case 0x02:
					MessageBox("读操作失败：数据通信校验错误！","错误",MB_OK);
					return;
					break;
				case 0x12:
					MessageBox("读操作失败：握手信号不正常！","错误",MB_OK);
					return;
					break;
				case 0x20:
					MessageBox("读操作失败:未读取到报文!","错误",MB_OK);
					return;
					break;
				case 0x21:
					ss="A5读：有报文。";
					break;
				default:
					MessageBox("读操作失败：数据通信校验错误！","错误",MB_OK);
					return;
				}
				m_telegram.AddString(ss);

				//分析报文
				if(Analyse_Tele_Data_Get_TeleAndUsertele(&Read_Tele[3],a4_read,usertele,&long_or_short)!=0)
				{
					MessageBox("分析报文错误","错误",MB_OK);
					return ;
				}

				for(i=128;i<510;i++)//add by peng
				{
					temp=Read_Tele[i+3];
					temp1=0;
					if(i==254)
					{
						if(temp!=0x55 && temp!=0xaa) temp1=1;
					}
					else if(i==508 || i==509)
					{
						if(temp!=0) temp1=2;
					}
					else if((i&0x02)==0)
					{
						if(temp!=0xff) temp1=3;
					}
					else if((i&0x02)!=0)
					{
						if(temp!=0) temp1=3;
					}
					if(temp1!=0)
					{
						MessageBox("A5读其他信息错误","错误",MB_OK);
						return ;
					}
				}

				//显示报文
				if(long_or_short==0) ss="A5读：长报文。";
				else ss="A5读：短报文。";
				m_telegram.AddString(ss);

                ss="";
				len=a4_read[0];

				for(i=0;i<len;i++)
				{
					ss1.Format("%02x  ",a4_read[i+1]);
					ss+=ss1;
					if(i>0 && (i+1)%16==0)
					{
						m_telegram.AddString(ss);
						ss="";
					}

				}
				m_telegram.AddString(ss);
				//************************************************
				
				//***********************************************
				//用户报文标识
				if(long_or_short==0) ss="A5读：长用户报文。";
				else ss="A5读：短用户报文。";
				m_usertelegram.AddString(ss);

				ss="";
				if(long_or_short==0) len1=104;
				else len1=27;
				
				for(i=0;i<len1;i++)
				{
					ss1.Format("%02x  ",usertele[i]);
					ss+=ss1;
					if(i>0 && (i+1)%16==0)
					{
						m_usertelegram.AddString(ss);
						ss="";
					}
				}
				m_usertelegram.AddString(ss);
				//******************************************************

				//******************************************************
				Copy_UserTele(temp_usertele,usertele,len1);
				Analyse_Usertele_Header(&(this->m_header),temp_usertele,len1);
				//******************************************************
				Copy_UserTele(temp_tele,&a4_read[1],a4_read[0]);
				Copy_UserTele(temp_usertele,usertele,len1);
				if(!Analyse_Usertele_Info(ANALYSE_USERTELE_BALISE_A5,temp_usertele,len1,temp_tele,a4_read[0]))
					MessageBox("分析用户报文错误！","错误",MB_OK);
			}
			else
			{
				MessageBox("读操作失败：数据通信校验错误！","错误",MB_OK);
				return;
			}

			//************************************************
			//比较
			if(filename!="")
			{
				if(long_or_short==1) 
				{
					a4_send[43]&=0xf8;
				}

				len=a4_read[0];
				m_same=TRUE;
				for(i=0;i<len;i++)
				{
					if(a4_send[i+1]!=a4_read[i+1])
					{
						m_same=FALSE;
						break;
					}
				}
				if(m_same) m_result.SetWindowText("一致");
				else m_result.SetWindowText("不一致");
			}
			//*************************************************

			//*************************************************
			if(Read_Tele[1+256]==0x55) 
				m_Functional_Bmp.SetResultBmp(1);
			else if(Read_Tele[1+256]==0xaa)
				m_Silent_Bmp.SetResultBmp(1);
			//*************************************************
		}
	}
}

void CA4ReadDlg::Analyse_User_info_frame()
{
 /*   byte data[104]={
	0x90, 0x12, 0x7F, 0x04, 0x61, 0x01, 0xFF, 0x90, 0x0B, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC,};

	byte data1[104]={
	0x90, 0x12, 0x7F, 0xA1, 0x20, 0x82, 0xC5, 0x50, 0x6F, 0x20, 0x00, 0x00, 0x27, 0x1E, 0xFF, 0x02, 
	0x06, 0xCC, 0x04, 0x04, 0x4C, 0x01, 0x09, 0xC5, 0x02, 0x04, 0x4C, 0x03, 0x07, 0xD0, 0x00, 0x12,
	0x5E, 0xFF, 0x1B, 0x41, 0x1C, 0x80, 0x00, 0x40, 0x00, 0xC7, 0x60, 0x18, 0x80, 0x29, 0x9A, 0x00,
	0x10, 0xC2, 0x7F, 0x80, 0xB1, 0x0B, 0xE8, 0x05, 0x0B, 0x32, 0x00, 0x00, 0xC2, 0x06, 0xBD, 0x86,
	0x30, 0x6A, 0xE6, 0x10, 0x6A, 0x46, 0x30, 0x6A, 0x86, 0x10, 0x70, 0x82, 0x30, 0x7F, 0x64, 0x10,
	0xB0, 0xEC, 0x30, 0x35, 0xC2, 0x10, 0x2E, 0xE8, 0x30, 0x4B, 0xC4, 0x10, 0x87, 0x8C, 0x30, 0x6C,
	0xE6, 0x10, 0x6A, 0x50, 0x88, 0x06, 0x1F, 0xFC};
	
	for(int i=0;i<104;i++) usertele[i]=data1[i];
*/
/*	byte temp,temp1;
	CString ss;
	int pos;

	//信息传送的方向
	pos=0;
	temp=(usertele[pos] & 0x80)>>7;
	ss.Format("%d",temp);
	m_Updown.SetWindowText(ss);

	//版本编号
	temp=usertele[pos] & 0x7f;
	ss.Format("%d",temp);
	m_Version.SetWindowText(ss);

	//信息传输媒介
	pos++;
	temp=(usertele[pos] & 0x80)>>7;
	ss.Format("%d",temp);
	m_Media.SetWindowText(ss);

	//本应答器在应答器组中的位置
	temp=(usertele[pos] & 0x70)>>4;
	ss.Format("%d",temp);
	m_Pig.SetWindowText(ss);

	//应答器组中所包含的应答器数量
	temp=(usertele[pos] & 0x0e)>>1;
	ss.Format("%d",temp);
	m_Total.SetWindowText(ss);

	//本应答器信息与前后应答器信息的关系
	temp=(usertele[pos] & 0x01)<<1;
	pos++;
	temp1=(usertele[pos] & 0x80)>>7;
	temp+=temp1;
	ss.Format("%d",temp);
	m_Dup.SetWindowText(ss);

	//报文计数器
	temp=(usertele[pos] & 0x7f)<<1;
	pos++;
	temp1=(usertele[pos] & 0x80)>>7;
	temp+=temp1;
	ss.Format("%d",temp);
	m_Mcount.SetWindowText(ss);

	//大区编号
	temp=usertele[pos] & 0x7f;
	ss.Format("%d",temp);
	m_Nid_cd.SetWindowText(ss);

	//分区编号
	pos++;
	temp=(usertele[pos] & 0xe0)>>5;
	ss.Format("%d",temp);
	m_Nid_cf.SetWindowText(ss);

	//车站编号
	temp=(usertele[pos] & 0x1f)<<1;
	pos++;
	temp1=(usertele[pos] & 0x80)>>7;
	temp+=temp1;
	ss.Format("%d",temp);
	m_Nid_bgc.SetWindowText(ss);

	//应答器编号
	temp=(usertele[pos] & 0x7f)<<1;
	pos++;
	temp1=(usertele[pos] & 0x80)>>7;
	temp+=temp1;
	ss.Format("%d",temp);
	m_Nid_bgy.SetWindowText(ss);

	//应答器的链接关系
	temp=(usertele[pos] & 0x40)>>6;
	ss.Format("%d",temp);
	m_Link.SetWindowText(ss);

	Analyse_Usertele_Info(&usertele[0],104);
*/	
}

void CA4ReadDlg::Clear_Display()
{
	m_telegram.ResetContent();
	m_usertelegram.ResetContent();
	m_header.ResetContent();

	m_result.SetWindowText("");

	m_Functional_Bmp.SetResultBmp(0);
	m_Silent_Bmp.SetResultBmp(0);

	for(int i=0;i<129;i++)
	{
		a4_send[i]=0x00;
		a4_read[i]=0x00;
	}
}

void CA4ReadDlg::OnBnClickedButton_OpenFile()
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

/*
int CA4ReadDlg::Analyse_A5_Tele_Data(byte* scr_data,byte* des_tele,byte* user_tele)
{
	unsigned long l_temp;
	unsigned int i,j,len,tel_start,temp,temp1,zm_or_fm,sb_temp,s_temp;
	unsigned int tel_buff[128];
	unsigned int flx[2]={0xdb,0xe0};	//
	unsigned int fsx[2]={0xb5,0x60};	//
	
	unsigned int glx[10]={0xb8,0x87,0x39,0xa7,0xa2,0xed,0x52,0x3b,0xa1,0x30};//last 4 bit no use
	unsigned int gsx[10]={0x9f,0x79,0x0c,0x2f,0xef,0x7c,0xa4,0xa3,0xc4,0xb0};//last 4 bit no use


	len=128;
	des_tele[0]=(BYTE)(len & 0xff); //???????

	//注：最后报文存到a5_read[]数组中，
	//a5_read[0]:报文数据长度
	//a5_read[1] ~ [128]: 报文数据
	
	//原始数据在Read_Tele[]中
	//Read_Tele[0]:为接收标识
	//Read_Tele[1]:0xff
	//Read_Tele[2]:0xeb
	//Read_Tele[3]~....[512]:数据

	//分析报文
	tel_error=0;
	for(i=0;i<128;i++) tel_buff[i]=scr_data[i+3];
	tel_buff[127]&=~0x01;

	for(i=0;i<1013;i++)
	{
		if((tel_buff[0] & 0x80)!=0) Make_Xor(tel_buff,flx,2);
		left_shift(tel_buff,128);
	}

	tel_start=tel_buff[0]*256+tel_buff[1];
	tel_start>>=6;
	
	if(tel_start!=0)
	{			//长报文
		long_or_short=0;
		if(tel_start==0x1ee)
		{		//长码不正确
			tel_error=1;
			return tel_error;
		}
		tel_start=slfx[tel_start];
		for(i=0;i<128;i++) tel_buff[i]=Read_Tele[i+3];
		if(tel_start!=0)
		{
			tel_start=1023-tel_start;

			if((tel_buff[127]&0x01)!=0) temp=1;
			else temp=0;
			for(i=0;i<tel_start;i++)
			{
				if((tel_buff[0] & 0x80)!=0) tel_buff[127]|=0x01;
				else tel_buff[127]&=~0x01;
				left_shift(tel_buff,128);
			}
			if(temp!=0) tel_buff[127]|=0x01;
		}
		for(i=0;i<128;i++) des_tele[i+1]=tel_buff[i];
		tel_buff[127]&=~0x01;

		//检查是否可被g(x)整除
		for(i=0;i<948;i++)
		{
			if((tel_buff[0] & 0x80)!=0) Make_Xor(tel_buff,glx,10);
			left_shift(tel_buff,128);
		}
		temp=0;
		for(i=0;i<10;i++) temp+=tel_buff[i];
		if(temp!=0)
		{			//长码不正确
			tel_error=2;
			return tel_error;
		}

		//检查11位有效
		for(i=0;i<128;i++) tel_buff[i]=des_tele[i+1];
		tel_buff[127]&=~0x01;

		for(i=0;i<93;i++)
		{
			temp=(i*11)/8;
			if(i!=92) temp=(tel_buff[temp]<<16)+(tel_buff[temp+1]<<8)+tel_buff[temp+2];
			else temp=(tel_buff[temp]<<16)+(tel_buff[temp+1]<<8);
			temp=temp<<((i*11)%8);
			temp>>=13;
			temp&=0x7ff;
			if(bit11_to_10[temp]>=1024)
			{				//长码不正确
				tel_error=3;
				return tel_error;
			}
			if(i<83)
			{
				user_tele[i]=bit11_to_10[temp];
			}
			else if(i==83)
			{
				if((temp & 0x0700)==0x0100) zm_or_fm=0;
				else if((temp & 0x0700)==0x0600) zm_or_fm=1;
				else
				{					//长码不正确
					tel_error=4;
					return tel_error;
				}
				sb_temp=temp & 0xff;
				sb_temp<<=4;
			}
			else if(i==84) sb_temp+=(temp>>7);
		}

	}
	else
	{			//短报文
		long_or_short=1;

		for(i=0;i<128;i++) tel_buff[i]=Read_Tele[i+3];
		tel_buff[42]&=~0x07;

		for(i=0;i<331;i++)
		{
			if((tel_buff[0] & 0x80)!=0) Make_Xor(tel_buff,fsx,2);
			left_shift(tel_buff,43);
		}

		tel_start=tel_buff[0]*256+tel_buff[1];
		tel_start>>=6;

		tel_start=ssfx[tel_start];
		if(tel_start>=341)
		{
			tel_error=0x11;
			return tel_error;
		}

		for(i=0;i<128;i++) tel_buff[i]=Read_Tele[i+3];
		if(tel_start!=0)
		{
			tel_start=1023-tel_start;

			if((tel_buff[127]&0x01)!=0) temp=1;
			else temp=0;
			for(i=0;i<tel_start;i++)
			{
				if((tel_buff[0] & 0x80)!=0) tel_buff[127]|=0x01;
				else tel_buff[127]&=~0x01;
				left_shift(tel_buff,128);
			}
			if(temp!=0) tel_buff[127]|=0x01;
		}
		for(i=0;i<128;i++) des_tele[i+1]=tel_buff[i];
		for(i=0;i<341;i++)
		{
			temp=tel_buff[i/8];
			temp=(temp<<(i%8)&0x80);

			j=i+341;
			temp1=tel_buff[j/8];
			temp1=(temp1<<(j%8)&0x80);
			if(temp1!=temp)
			{
				tel_error=0x15;
				return tel_error;
			}
			j=i+682;
			temp1=tel_buff[j/8];
			temp1=(temp1<<(j%8)&0x80);
			if(temp1!=temp)
			{
				tel_error=0x15;
				return tel_error;
			}
		}
		tel_buff[42]&=~0x07;

		//检查是否可被g(x)整除
		for(i=0;i<266;i++)
		{
			if((tel_buff[0] & 0x80)!=0) Make_Xor(tel_buff,gsx,10);
			left_shift(tel_buff,43);
		}
		temp=0;
		for(i=0;i<10;i++) temp+=tel_buff[i];
		if(temp!=0)
		{			//短码不正确
			tel_error=0x12;
			return tel_error;
		}

		//检查11位有效
		for(i=0;i<128;i++) tel_buff[i]=des_tele[i+1];
		tel_buff[42]&=~0x07;

		for(i=0;i<31;i++)
		{
			temp=(i*11)/8;
			if(i!=30) temp=(tel_buff[temp]<<16)+(tel_buff[temp+1]<<8)+tel_buff[temp+2];
			else temp=(tel_buff[temp]<<16)+(tel_buff[temp+1]<<8);
			temp=temp<<((i*11)%8);
			temp>>=13;
			temp&=0x7ff;
			if(bit11_to_10[temp]>=1024)
			{				//短码不正确
				tel_error=0x13;
				return tel_error;
			}
			if(i<21)
			{
				user_tele[i]=bit11_to_10[temp];
			}
			else if(i==21)
			{
				if((temp & 0x0700)==0x0100) zm_or_fm=0;
				else if((temp & 0x0700)==0x0600) zm_or_fm=1;
				else
				{					//短码不正确
					tel_error=0x14;
					return tel_error;
				}
				sb_temp=temp & 0xff;
				sb_temp<<=4;
			}
			else if(i==22) sb_temp+=(temp>>7);
		}
	}

	if(zm_or_fm!=0)
	{
		sb_temp^=0xfff;
		for(i=0;i<83-long_or_short*62;i++) user_tele[i]^=0x3ff;
	}

	//解扰
	l_temp=2801775573;
	l_temp*=sb_temp;
	l_temp&=0xffffffff;
	s_temp=l_temp;


	for(i=0;i<83-long_or_short*62;i++)
	{
		temp=0;
		temp1=user_tele[i];
		for(j=0;j<10;j++)
		{
			temp<<=1;
			if((s_temp & 0x80000000)!=0)
			{
				if((temp1 & 0x0200)==0) temp|=0x01;
			}
			else
			{
				if((temp1 & 0x0200)!=0) temp|=0x01;
			}
			s_temp<<=1;
			if((temp1 & 0x0200)!=0)
			{
				s_temp^=0xea000001;
			}
			s_temp&=0xffffffff;
			temp1<<=1;
		}
		user_tele[i]=temp;
	}
	//恢复第一个10位
	temp=0;
	for(i=1;i<83-long_or_short*62;i++) temp+=user_tele[i];
	temp=user_tele[0]-temp;
	user_tele[0]=temp&0x3ff;

	for(i=0;i<83;i++)
	{
		user_tele[103-i]=user_tele[82-i];
	}
	for(i=0;i<104;i++) user_tele[i]&=0x3ff;


	for(i=0;i<104;i++)
	{
		temp=i*8;
		j=temp/10;
		temp=temp%10;
		if(j==82) j=user_tele[j+21]<<10;
		else j=(user_tele[j+21]<<10)+user_tele[j+21+1];
		j>>=(12-temp);
		user_tele[i]=(j&0xff);
	}
	if(long_or_short==0) user_tele[103]&=0xfc;
	else
	{
		user_tele[26]&=0xc0;
		for(i=27;i<104;i++) user_tele[i]=0;
	}

	return 0;
}

void CA4ReadDlg::left_shift(unsigned int *temp,int num)
{
	int i;
	unsigned int temp1;

	for(i=0;i<num;i++)
	{
		temp1=temp[i]<<1;
		if(i!=num-1) temp1+=(temp[i+1]>>7);
		temp1&=0xff;
		temp[i]=temp1;
	}
	}*/
void CA4ReadDlg::OnLbnSelchangeList1()
{
	// TODO: 在此添加控件通知处理程序代码
}
