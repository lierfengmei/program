// SRead_SC.cpp : 实现文件
//

#include "stdafx.h"
#include "program_balise.h"
#include "SRead_SC.h"
#include ".\sread_sc.h"
#include "Comm_Balise_ts.h"
#include "Wait.h"
#include <time.h>

// CSRead_SC 对话框

IMPLEMENT_DYNAMIC(CSRead_SC, CDialog)
CSRead_SC::CSRead_SC(CWnd* pParent /*=NULL*/)
	: CDialog(CSRead_SC::IDD, pParent)
{
}

CSRead_SC::~CSRead_SC()
{
}

void CSRead_SC::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSRead_SC, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_READ_SC, OnBnClickedButtonReadSc)
	ON_BN_CLICKED(IDC_BUTTON_READ_THM, OnBnClickedButtonReadThm)
	ON_BN_CLICKED(IDC_BUTTON_READ_THE, OnBnClickedButtonReadThe)
END_MESSAGE_MAP()


// CSRead_SC 消息处理程序

void CSRead_SC::OnBnClickedButtonReadSc()
{
	// TODO: 在此添加控件通知处理程序代码
	int len,len1,len_data;
	byte ch[1024];
	int pos=0;
	int i;
	CString ss,ss1;
	byte temp;
	CString filename;
	CWait m_dlgWait;

	m_dlgWait.Create(IDD_DIALOG_WAIT,NULL);  
	m_dlgWait.ShowWindow(SW_SHOW);
	m_dlgWait.UpdateWindow();
	CWaitCursor wait;
	
	//读发送命令
	pos=0;
	ch[pos++]=0x22;
	ch[pos++]=0x00;
	ch[pos++]=0x00;
	ch[pos++]=ORDER_LEU_SC;

	//长度
	len_data=pos-3;
	ch[1]=(byte)(len_data & 0xff);
	len_data>>=8;
	ch[2]=(byte)(len_data & 0xff);

	len=pos;
	SendCommand(len,&ch[0]);

	//接收数据
	i=0;
	Read_Tele_Len=0;
	Read_Tele_End_Flag=FALSE;
	do{
		Sleep(200);
		ReceiveData(READLEN_LEU_READ_SC);

		if(ReadBufLen>0)
		{
			if(ArrangeData(ORDER_LEU_READ))
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
					MessageBox("读操作失败:未读取到数据!","错误",MB_OK);
					return;
					break;
				}
				
				//***********************************************
				//分析报文
				len1=230;
				if(temp==0x21)
				{
					Copy_UserTele(ch,&Read_Tele[1],len1);
					if(!Anaylse_SC(ch,len1))
					{
						MessageBox("分析 System Configuration 数据错误","错误",MB_OK);
						return ;
					}
					else
					{
						MessageBox("分析 System Configuration 数据正确","成功",MB_OK);
						return ;
					}
				}
			}
			else
			{
				MessageBox("接收数据格式错误","错误",MB_OK);
				return;
			}
			//*************************************************
		}
		i++;
	}while(i<80);

	if(i>=80)
	{
		MessageBox("读操作失败：通信未建立！","错误",MB_OK);
		return ;
	}
/*
	byte ch[]={
 0x0A, 0x00, 0x01, 0x00, 0x01,0x00,
 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE1, 0x04, 0x00, 0x00,//?3 word
 0x56, 0x34, 0x30, 0x34, 0x14, 0x3E, 0x5C, 0xDF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//sw
 0x03, 0x00, 0x00, 0x00,//hw
 0x03, 0x00, 0x00, 0x0D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x17, 0x00, 0x00, 0x00,//CRTE
 0x07, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x01, 0x01,//SLEB
 0x01, 0x00, 0x00, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x16, 0x06, 0x16, 0x06,//SERB
 0x44, 0xE5, 0xC9, 0xCA, 0x05, 0x55, 0x55, 0x55, 0xA0, 0xC6, 0xA6, 0xDC, 0xA0, 0xC6, 0xA6, 0xDC,
 0x44, 0xE5, 0xC9, 0xCA, 0x24, 0x44, 0x44, 0x44, 0x3E, 0xD6, 0xF4, 0xC8, 0x3E, 0xD6, 0xF4, 0xC8,
 0x44, 0xE5, 0xC9, 0xCA, 0x1A, 0xBC, 0xDE, 0xEE, 0x4B, 0x92, 0xD5, 0x46, 0x4B, 0x92, 0xD5, 0x46,
 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x21, 0x54, 0x81, 0xB1, 0x21, 0x54, 0x81, 0xB1,
 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x21, 0x54, 0x81, 0xB1, 0x21, 0x54, 0x81, 0xB1,
 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x21, 0x54, 0x81, 0xB1, 0x21, 0x54, 0x81, 0xB1,
 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x21, 0x54, 0x81, 0xB1, 0x21, 0x54, 0x81, 0xB1,
 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x21, 0x54, 0x81, 0xB1, 0x21, 0x54, 0x81, 0xB1,
 0x01, 0xF1, 0x00, 0x01, 0x04, 0x5E, 0x00, 0x01,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	
    int len1=230;

	Anaylse_SC(ch,len1);
*/	return;
}

BOOL CSRead_SC::Anaylse_SC(byte data[],int len)
{
	CStdioFile m_readtele;
	CFileException ex;
	CString m_readtele_name;
	CTime  CurrentTime;
	CString str;
	CString file_direct;
	char buf[256];
	BOOL bRet=TRUE,bTempRet=TRUE;
	
	int pos;
	int i,j;

	GetModuleFileName(NULL,buf,256);
	*(strrchr(buf, '\\') + 1) = '\0';
	file_direct.Format("%s",buf);
	
	CurrentTime=CTime::GetCurrentTime();
	int hour=CurrentTime.GetHour();

	//打开文件
	m_readtele_name=file_direct;
	m_readtele_name+="SCreaderlog_";
	m_readtele_name+=CurrentTime.Format("%Y_%m_%d_%H_%M_%S");
	m_readtele_name+=".txt";

	if(!m_readtele.Open(m_readtele_name,CFile::modeCreate | CFile::modeWrite ,&ex))
		return FALSE;
	
	str="System Configureation Data of Buffer Transfer Messages reader log file\n";
	Print_Log(&m_readtele,str);

	str="-------------------------\n";
	Print_Log(&m_readtele,str);

	str="Status Output File\n";
	Print_Log(&m_readtele,str);

	str="-------------------------\n";
	Print_Log(&m_readtele,str);

	str="Uploading of : -> SC ";
	str+=CurrentTime.Format("%Y年%m月%d日%H:%M:%S\n\n");
	Print_Log(&m_readtele,str);

	byte ch[16];
	byte temp;
	CString str_temp;
	int board_num;

	//输出上传数据
	str="";
	for(i=0;i<len;i++)
	{
		str_temp.Format("%02X ",data[i]);
		str+=str_temp;
	}
	str+="\n\n\n";
	Print_Log(&m_readtele,str);


	pos=0;
	//无用数据
	for(i=0;i<18;i++) ch[0]=data[pos++];

	//Software configuration
	str="generic encoder, software version: ";

	for(i=0;i<4;i++)
	{
		ch[i]=data[pos++];
		str_temp.Format("%c",ch[i]);
		str+=str_temp;
	}
	str+=", program checksum ";
	for(i=0;i<4;i++)
	{
		ch[i]=data[pos++];
		str_temp.Format("%02X",ch[i]);
		str+=str_temp;
	}
	str+="\n";
	Print_Log(&m_readtele,str);

	str="running : ";
	ch[0]=data[pos++];
	if(ch[0]==0x00)
		str+="Remote mode\n\n";
	else if(ch[0]=0x01)
		str+="Nominal mode\n\n";
	else str+="\n\n";
	Print_Log(&m_readtele,str);

	for(i=0;i<7;i++) pos++;

	//Hardware configuration
	ch[0]=data[pos++];
	board_num=ch[0];

	str.Format("%i",ch[0]);
	str+=" board(s):\n\n";
	Print_Log(&m_readtele,str);

	for(i=0;i<3;i++) pos++;

	for(i=0;i<board_num;i++)
	{
		for(j=0;j<16;j++) ch[j]=data[pos++];

		switch(ch[0])
		{
		case 0x01:
		case 0x81:
			str=" output board type ";
			str_temp.Format("%02X",ch[0]);
			str+=str_temp;

			str+=" , GID: ";
			for(j=4;j<12;j++)
			{
				str_temp.Format("%02X",ch[j]);
				str+=str_temp;
			}
			str+="\n\n";
			str+=" outputs configuration\n\n";
			str+="               PCC |  PT  |  PC6 |  LO  |  LCC |\n";
			Print_Log(&m_readtele,str);

			for(j=0;j<4;j++)
			{
				temp=ch[15-j];
				str.Format("    output %i:   ",j+1);
				if((temp & 0x10)>>4)
				{
					str+="X";
				}
				else
				{
					str+=" ";
				}
				str+="     ";

				if((temp & 0x08)>>3)
					str+="X";
				else str+=" ";
				str+="       ";

				if((temp & 0x04)>>2)
					str+="X";
				else str+=" ";
				str+="      ";

				if((temp & 0x02)>>1)
					str+="X";
				else str+=" ";
				str+="      ";

				if(temp & 0x01)
					str+="X";
				else str+=" ";
				str+="\n";

				Print_Log(&m_readtele,str);
			};
			break;
		case 0x03:
			str="  CRTE board 1 Mb/s type ";
			str_temp.Format("%02X",ch[0]);
			str+=str_temp;

			str+=" h/w version: ";
			str_temp.Format("%02X",ch[12]);
			str+=str_temp;

			str+=", GID: ";
			for(j=4;j<12;j++)
			{
				str_temp.Format("%02X",ch[j]);
				str+=str_temp;
			}
			str+="\n";
			Print_Log(&m_readtele,str);

			break;
		case 0x07:
			str="  SLEB board type ";
			str_temp.Format("%02X",ch[0]);
			str+=str_temp;

			str+=" , GID: ";
			for(j=4;j<12;j++)
			{
				str_temp.Format("%02X",ch[j]);
				str+=str_temp;
			}
			str+="\n";
			str+="               Activity |  Processed  |  Used | \n";
			Print_Log(&m_readtele,str);

			for(j=0;j<4;j++)
			{
				temp=ch[15-j];
				str.Format("    link %i:        ",j+1);

				if((temp & 0x04)>>2)
					str+="X";
				else str+=" ";
				str+="           ";

				if((temp & 0x02)>>1)
					str+="X";
				else str+=" ";
				str+="          ";

				if(temp & 0x01)
					str+="X";
				else str+=" ";
				str+="\n";

				Print_Log(&m_readtele,str);
			};
			break;
		default:
			break;
		}
		str="\n";
		Print_Log(&m_readtele,str);
	}

	//Parameters Tables Configuration
	str="PARAMETER TABLES:\n\n";
	Print_Log(&m_readtele,str);

	str="            TABLE ID        CRC1    CRC2\n\n";
	Print_Log(&m_readtele,str);

	CString par_str[8]={"TPC","TSE","TE1","TE2","TE3","TE4","TE5","TE6"};
	for(i=0;i<8;i++)
	{
		str="  ";
		str+=par_str[i];
		str+=": ";

		for(j=0;j<16;j++) ch[j]=data[pos++];

		for(j=0;j<16;j++)
		{
			str_temp.Format("%02X",ch[j]);
			str+=str_temp;

			if(((j+1)%4)==0) str+=" ";
		}
		str+="\n";
		Print_Log(&m_readtele,str);
	}

	str="\n";
	Print_Log(&m_readtele,str);

	//History Tables information
	str="HISTORY INFO\n";
	Print_Log(&m_readtele,str);

	for(i=0;i<8;i++) ch[i]=data[pos++];

	str="  THM: ";
	str_temp.Format("%i",(int)(ch[0] *256 +ch[1]));
	str+=str_temp;
	str+=" records\n";
	Print_Log(&m_readtele,str);

	str="  THE: ";
	str_temp.Format("%i",(int)(ch[4] *256 +ch[5]));
	str+=str_temp;
	str+=" records\n";
	Print_Log(&m_readtele,str);

	m_readtele.Close();

	return TRUE;
}

void CSRead_SC::OnBnClickedButtonReadThm()
{
	// TODO: 在此添加控件通知处理程序代码
	int len,len_data;
	byte ch[1024];
	int pos=0;
	int i;
	CString ss,ss1;
	byte temp;
	CString filename;
	CWait m_dlgWait;

	m_dlgWait.Create(IDD_DIALOG_WAIT,NULL);  
	m_dlgWait.ShowWindow(SW_SHOW);
	m_dlgWait.UpdateWindow();
	CWaitCursor wait;
	
	//读发送命令
	pos=0;
	ch[pos++]=0x22;
	ch[pos++]=0x00;
	ch[pos++]=0x00;
	ch[pos++]=ORDER_LEU_THM;

	//长度
	len_data=pos-3;
	ch[1]=(byte)(len_data & 0xff);
	len_data>>=8;
	ch[2]=(byte)(len_data & 0xff);

	len=pos;
	SendCommand(len,&ch[0]);

	//接收数据
	i=0;
	Read_Tele_Len=0;
	Read_Tele_End_Flag=FALSE;
	do{
		Sleep(100);
		ReceiveData(READLEN_LEU_READ_THM);

		if(ReadBufLen>0)
		{
			if(!ArrangeData(ORDER_LEU_READ)) 
				break;
		}
		else
		{
			i++;
		}
	}while(!Read_Tele_End_Flag && (i<15));

	if(Read_Tele_Len>0)
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
			MessageBox("读操作失败:未读取到数据!","错误",MB_OK);
			return;
			break;
		}
		
		//***********************************************
		//分析报文
		if(temp==0x21)
		{
			if(!Anaylse_THM(Read_Tele,Read_Tele_Len))
			{
				MessageBox("分析 Maintenance History Table 数据错误","错误",MB_OK);
				return ;
			}
			else
			{
				MessageBox("分析 Maintenance History Table 数据正确","成功",MB_OK);
				return ;
			}
		}
		else
		{
			MessageBox("接收数据格式错误","错误",MB_OK);
			return;
		}
	}
	else
	{
		MessageBox("读操作失败：通信未建立！","错误",MB_OK);
		return ;
	}

	return;
}

void CSRead_SC::OnBnClickedButtonReadThe()
{
	// TODO: 在此添加控件通知处理程序代码
	int len,len_data;
	byte ch[1024];
	int pos=0;
	int i;
	CString ss,ss1;
	byte temp;
	CString filename;
	CWait m_dlgWait;

	m_dlgWait.Create(IDD_DIALOG_WAIT,NULL);  
	m_dlgWait.ShowWindow(SW_SHOW);
	m_dlgWait.UpdateWindow();
	CWaitCursor wait;
	
	//读发送命令
	pos=0;
	ch[pos++]=0x22;
	ch[pos++]=0x00;
	ch[pos++]=0x00;
	ch[pos++]=ORDER_LEU_THE;

	//长度
	len_data=pos-3;
	ch[1]=(byte)(len_data & 0xff);
	len_data>>=8;
	ch[2]=(byte)(len_data & 0xff);

	len=pos;
	SendCommand(len,&ch[0]);
	Sleep(500);

	//接收数据
	i=0;
	Read_Tele_Len=0;
	Read_Tele_End_Flag=FALSE;
	do{
		Sleep(100);
		ReceiveData(READLEN_LEU_READ_THE);

		if(ReadBufLen>0)
		{
			if(!ArrangeData(ORDER_LEU_READ))
				break;
		}
		else 
		{
			i++;
		}
	}while(!Read_Tele_End_Flag && (i<15));

	if(Read_Tele_Len>0)
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
			MessageBox("读操作失败:未读取到数据!","错误",MB_OK);
			return;
			break;
		}
		
		//***********************************************
		//分析报文
		if(temp==0x21)
		{
			if(!Anaylse_THE(Read_Tele,Read_Tele_Len))
			{
				MessageBox("分析 Eurotelegram History Table 数据错误","错误",MB_OK);
				return ;
			}
			else
			{
				MessageBox("分析 Eurotelegram History Table 数据正确","成功",MB_OK);
				return ;
			}
		}
		else
		{
			MessageBox("接收数据格式错误","错误",MB_OK);
			return;
		}
	}
	else
	{
		MessageBox("读操作失败：通信未建立！","错误",MB_OK);
		return;
	}

	return;
}


BOOL CSRead_SC::Anaylse_THM(byte data[],int len)
{
	CStdioFile m_readtele;
	CFileException ex;
	CString m_readtele_name;
	CTime  CurrentTime;
	CString str;
	CString file_direct;
	char buf[256];
	BOOL bRet=TRUE,bTempRet=TRUE;
	
	int pos;
	int i,j;

	GetModuleFileName(NULL,buf,256);
	*(strrchr(buf, '\\') + 1) = '\0';
	file_direct.Format("%s",buf);
	
	CurrentTime=CTime::GetCurrentTime();
	int hour=CurrentTime.GetHour();

	//打开文件
	m_readtele_name=file_direct;
	m_readtele_name+="THMreaderlog_";
	m_readtele_name+=CurrentTime.Format("%Y_%m_%d_%H_%M_%S");
	m_readtele_name+=".txt";

	if(!m_readtele.Open(m_readtele_name,CFile::modeCreate | CFile::modeWrite ,&ex))
		return FALSE;
	
	str="THM(Maintenance History Table) Data of Buffer Transfer Messages reader log file\n";
	Print_Log(&m_readtele,str);

	str="-------------------------\n";
	Print_Log(&m_readtele,str);

	str="Status Output File\n";
	Print_Log(&m_readtele,str);

	str="-------------------------\n";
	Print_Log(&m_readtele,str);

	str="Uploading of : -> THM ";
	str+=CurrentTime.Format("%Y年%m月%d日%H:%M:%S\n\n");
	Print_Log(&m_readtele,str);

	byte ch[32];
	CString str_temp;
	DWORD cycles=0x00000000,cycles_old;
	//DWORD absolute;
	WORD message_nr;
	int jump;
	__time64_t rectime,temptime;
	struct tm *gmt;


	//输出上传数据
	str="";
	j=0;
	for(i=0;i<len;i++)
	{
		if(j==0) i++;
		str_temp.Format("%02X ",data[i]);
		str+=str_temp;
		j++;
		if((j%16)==0) str+="\n";
		if(j==230) 
		{
			j=0;
			str+="\n\n";
			Print_Log(&m_readtele,str);
			str="";
		}
	}
	str="\n\n";
	Print_Log(&m_readtele,str);


	str="------------------------------------------------------------------------------------------------------------\n";
	str+=" Rec Nb |Posix Date| Cycles  |  absolute date     |     TSE ident    | message nr |  message interpretation \n";
	str+="------------------------------------------------------------------------------------------------------------\n";
	Print_Log(&m_readtele,str);

	pos=0;
	jump=0;
	//无用数据
	for(i=0;i<(1+6+12);i++)
	{
		ch[0]=data[pos++];
		jump++;
	}

	do{
		//读取8个字：32字节
		for(i=0;i<32;i++)
		{
			ch[i]=data[pos++];
			jump++;
			if(jump==231)
			{
				pos+=(1+6);
				jump=7;
			}
			if(pos>len) break;
		}
		if(pos>len) break;

		//分析数据
		if(ch[0]==0x00)//有效记录
		{
			str="";
			
			cycles_old=cycles;
			cycles=(DWORD)((ch[8]<<24) + (ch[9]<<16) + (ch[10]<<8) + (ch[11]));

			if((cycles_old>0x00) && (cycles==0x00))
				str+=" cycle counter reset\n";

			//Rec Nb
			
			for(i=0;i<4;i++)
			{
				str_temp.Format("%02X",ch[i]);
				str+=str_temp;
			}
			str+="|";

			//Posix Date
			rectime=0;
			str+=" ";
			for(i=4;i<8;i++)
			{
				str_temp.Format("%02X",ch[i]);
				str+=str_temp;
			}
			str+=" |";
			rectime=__time64_t((ch[4]<<24) + (ch[5]<<16) + (ch[6]<<8) + ch[7]);

			//Cycles  
			str+=" ";
			for(i=8;i<12;i++)
			{
				str_temp.Format("%02X",ch[i]);
				str+=str_temp;
			}
			str+="|";

			temptime=__time64_t(cycles * 204.8f /1000);
			rectime+=temptime;//+8*3600;

			gmt = _gmtime64( &rectime );

			//absolute date
			str+=" ";
			str_temp.Format("%02i/",gmt->tm_mday);//日
			str+=str_temp;

			str_temp.Format("%02i/",gmt->tm_mon+1);//月
			str+=str_temp;

			str_temp.Format("%04i-",gmt->tm_year+1900);//年
			str+=str_temp;

			str_temp.Format("%02i-",gmt->tm_hour);//时
			str+=str_temp;

			str_temp.Format("%02i-",gmt->tm_min);//分
			str+=str_temp;

			str_temp.Format("%02i",gmt->tm_sec);//秒
			str+=str_temp;

			str+="|";

/*			absolute=(DWORD)(cycles * 204.8f /1000);
			str+=" ";
			str+="01/01/1970";
			str_temp.Format("%02i",((DWORD)(absolute/3600) & 0xff)); //时
			str+= "-" + str_temp;
			absolute%=3600;
			str_temp.Format("%02i",((DWORD)(absolute/60) & 0xff)); //分
			str+= "-" + str_temp;
			absolute%=60;
			str_temp.Format("%02i",(absolute & 0xff)); //秒
			str+= "-" + str_temp;
			str+="|";
*/
			//TSE ident
			for(i=12;i<16;i++)
			{
				str_temp.Format("%02X",ch[i]);
				str+=str_temp;
			}
			str+=" ";
		
			str_temp.Format("%02X",ch[16]);
			str+=str_temp;
			str+=" ";

			for(i=17;i<20;i++)
			{
				str_temp.Format("%02X",ch[i]);
				str+=str_temp;
			}
			str+="|";

			//message nr
			str+="    ";
			for(i=20;i<22;i++)
			{
				str_temp.Format("%02X",ch[i]);
				str+=str_temp;
			}
			str+="    |";
			message_nr=(WORD)((ch[20]<<8) + ch[21]);

			//message interpretation
			str+=" ";
			str+=Message_THM(message_nr);
			str+="\n";
	
			Print_Log(&m_readtele,str);
		}
	}while(pos<len);

	m_readtele.Close();

	return TRUE;
}

CString CSRead_SC::Message_THM(int message)
{
	CString str_mess;

	switch(message)
	{
	case 0x0001:
		str_mess="Reboot requested";
		break;
	case 0x0003:
		str_mess="Remote mode --> Init mode";
		break;
	case 0x0007:
		str_mess="Init mode --> Nominal mode 4";
		break;
	case 0x0008:
		str_mess="Init mode --> Remote mode";
		break;
	case 0x0020:
		str_mess="Configuration has changed (TSE CRC signature)";
		break;
	case 0x0031:
		str_mess="BSD (remote signaling data) frame acquisition error (Bad 16-bits CRC)";
		break;
	case 0x0047:
		str_mess="Parameters are not valid";
		break;
	case 0x0048:
		str_mess="Initialization failed";
		break;
	case 0x0100:
		str_mess="correct SBT sent by MPT";
		break;
	case 0x0102:
		str_mess="first correct request sent by MPT";
		break;
	case 0x0105:
		str_mess="EBT sent by MPT";
		break;
	case 0x0312:
		str_mess="@1Mb/s : The encoder will never be FIP bus arbitrator (Not BA)";
		break;
	case 0x0400:
		str_mess="Activity on links A and B";
		break;
	case 0x0401:
		str_mess="Activity on link A, no activity on link B";
		break;
	case 0x0403:
		str_mess="No activity on both links";
		break;
	default:
		str_mess="";
		break;
	}
	return str_mess;
}

BOOL CSRead_SC::Anaylse_THE(byte data[],int len)
{
	CStdioFile m_readtele;
	CFileException ex;
	CString m_readtele_name;
	CTime  CurrentTime;
	CString str;
	CString file_direct;
	char buf[256];
	BOOL bRet=TRUE,bTempRet=TRUE;
	
	int pos;
	int i,j;

	GetModuleFileName(NULL,buf,256);
	*(strrchr(buf, '\\') + 1) = '\0';
	file_direct.Format("%s",buf);
	
	CurrentTime=CTime::GetCurrentTime();
	int hour=CurrentTime.GetHour();

	//打开文件
	m_readtele_name=file_direct;
	m_readtele_name+="THEreaderlog_";
	m_readtele_name+=CurrentTime.Format("%Y_%m_%d_%H_%M_%S");
	m_readtele_name+=".txt";

	if(!m_readtele.Open(m_readtele_name,CFile::modeCreate | CFile::modeWrite ,&ex))
		return FALSE;
	
	str="THE(Eurotelegram History Table) Data of Buffer Transfer Messages reader log file\n";
	Print_Log(&m_readtele,str);

	str="-------------------------\n";
	Print_Log(&m_readtele,str);

	str="Status Output File\n";
	Print_Log(&m_readtele,str);

	str="-------------------------\n";
	Print_Log(&m_readtele,str);

	str="Uploading of : -> THE ";
	str+=CurrentTime.Format("%Y年%m月%d日%H:%M:%S\n\n");
	Print_Log(&m_readtele,str);

	byte ch[32];
	CString str_temp;
	DWORD cycles=0x00000000,cycles_old;
	DWORD absolute;
	WORD message_nr;
	CString mess[3][21];
	int mess_cur;
	byte index;
	byte pt;
	float time;
	CString dot=" .                     ";
	CString question="   ???        ";
	CString str_null="              ";
	int jump;
	DWORD cycles_start[4];

	__time64_t rectime,temptime;
	struct tm *gmt;

	//输出上传数据
	str="";
	j=0;
	for(i=0;i<len;i++)
	{
		if(j==0) i++;
		str_temp.Format("%02X ",data[i]);
		str+=str_temp;
		j++;
		if((j%16)==0) str+="\n";
		if(j==230) 
		{
			j=0;
			str+="\n\n";
			Print_Log(&m_readtele,str);
			str="";
		}
	}
	str="\n\n";
	Print_Log(&m_readtele,str);


	str= "-------------------------------------------------------------------------------------------------------------------------------------------------------------------------|\n";
	str+=" Rec Nb |  Posix |    Absolute       | Cycles |    TSE ident     |                     Eurotelegram number & duration                                                    |\n";
	str+="        |  Date  |     Date          |elapsed |                  |          output 1       |          output 2       |          output 3       |          output 4       |\n";
	str+="        |        |           HH MM SS|        |                  |   tel    DD HH MM SS   P|   tel    DD HH MM SS   P|   tel    DD HH MM SS   P|   tel    DD HH MM SS   P|\n";
	str+="-------------------------------------------------------------------------------------------------------------------------------------------------------------------------|\n";
	Print_Log(&m_readtele,str);

	for(i=0;i<3;i++)
	{
		for(j=0;j<21;j++) mess[i][j]="";
	}
	mess_cur=0;
	
	str="";

	pos=0;
	jump=0;
	//无用数据
	for(i=0;i<(1+6+12);i++)
	{
		ch[0]=data[pos++];
		jump++;
	}

	do{
		//读取8个字：32字节
		for(i=0;i<32;i++)
		{
			ch[i]=data[pos++];
			jump++;
			if(jump==231)
			{
				pos+=(1+6);
				jump=7;
			}
			if(pos>len) break;
		}
		if(pos>len) break;

		//分析数据
		if(ch[0]==0x00)//有效记录
		{
			cycles_old=cycles;
			cycles=(DWORD)((ch[8]<<24) + (ch[9]<<16) + (ch[10]<<8) + (ch[11]));

			if((cycles_old>0x00) && (cycles==0x00))
			{
				str="";
                for(i=0;i<5;i++) str+=mess[1][i];
				for(i=5;i<7;i++) str+=mess[1][i];
				str+=question;
				for(i=8;i<11;i++) str+=mess[1][i];
				str+=question;
				for(i=12;i<15;i++) str+=mess[1][i];
				str+=question;
				for(i=16;i<19;i++) str+=mess[1][i];
				str+=question;
				str+=mess[1][20];
				str+="\n";

				str+=" cycle counter reset\n";

				Print_Log(&m_readtele,str);

				for(i=0;i<3;i++)
					for(j=0;j<21;j++) mess[i][j]="";
				mess_cur=0;
				str="";
			}
			//Rec Nb
			mess[mess_cur][0]="";
			for(i=0;i<4;i++)
			{
				str_temp.Format("%02X",ch[i]);
				mess[mess_cur][0]+=str_temp;
			}
			mess[mess_cur][0]+="|";

			//Posix Date
			mess[mess_cur][1]="";
			for(i=4;i<8;i++)
			{
				str_temp.Format("%02X",ch[i]);
				mess[mess_cur][1]+=str_temp;
			}
			mess[mess_cur][1]+="|";

			rectime=__time64_t((ch[4]<<24) + (ch[5]<<16) + (ch[6]<<8) + ch[7]);

			//Cycles  
			mess[mess_cur][3]="";
			for(i=8;i<12;i++)
			{
				str_temp.Format("%02X",ch[i]);
				mess[mess_cur][3]+=str_temp;
			}
			mess[mess_cur][3]+="|";

			temptime=__time64_t(cycles * 204.8f /1000);
			rectime+=temptime;//+8*3600;

			gmt = _gmtime64( &rectime );
			
			//absolute date
			mess[mess_cur][2]="";

			str_temp.Format("%02i/",gmt->tm_mday);//日
			mess[mess_cur][2]+=str_temp;

			str_temp.Format("%02i/",gmt->tm_mon+1);//月
			mess[mess_cur][2]+=str_temp;

			str_temp.Format("%04i-",gmt->tm_year+1900);//年
			mess[mess_cur][2]+=str_temp;

			str_temp.Format("%02i-",gmt->tm_hour);//时
			mess[mess_cur][2]+=str_temp;

			str_temp.Format("%02i-",gmt->tm_min);//分
			mess[mess_cur][2]+=str_temp;

			str_temp.Format("%02i",gmt->tm_sec);//秒
			mess[mess_cur][2]+=str_temp;

			mess[mess_cur][2]+="|";

/*			mess[mess_cur][2]="";
			absolute=(DWORD)(cycles * 204.8f /1000);
			mess[mess_cur][2]+="01/01/1970";
			str_temp.Format("%02i",((DWORD)(absolute/3600) & 0xff)); //时
			mess[mess_cur][2]+= "-" + str_temp;
			absolute%=3600;
			str_temp.Format("%02i",((DWORD)(absolute/60) & 0xff)); //分
			mess[mess_cur][2]+= "-" + str_temp;
			absolute%=60;
			str_temp.Format("%02i",(absolute & 0xff)); //秒
			mess[mess_cur][2]+= "-" + str_temp;
			mess[mess_cur][2]+="|";
*/
			//TSE ident
			mess[mess_cur][4]="";
			for(i=12;i<16;i++)
			{
				str_temp.Format("%02X",ch[i]);
				mess[mess_cur][4]+=str_temp;
			}
			mess[mess_cur][4]+=" ";
		
			str_temp.Format("%02X",ch[16]);
			mess[mess_cur][4]+=str_temp;
			mess[mess_cur][4]+=" ";

			for(i=17;i<20;i++)
			{
				str_temp.Format("%02X",ch[i]);
				mess[mess_cur][4]+=str_temp;
			}
			mess[mess_cur][4]+="|";

			//tel 1 p
			pt=ch[20];
			for(i=0;i<4;i++)
			{
				mess[mess_cur][8+i*4]=" ";
				if(pt & 0x01) mess[mess_cur][8+i*4]+="X";
				else mess[mess_cur][8+i*4]+=" ";
                pt>>=1;
				mess[mess_cur][8+i*4]+="|";
			}

			//identifiers
			index=((ch[29] & 0xfc)>>2);
			str_temp.Format("%03i",index);
			mess[mess_cur][5]=str_temp;

			index=((ch[29] & 0x03)<<4) + ((ch[30] & 0xf0)>>4);
			str_temp.Format("%03i",index);
			mess[mess_cur][9]=str_temp;

			index=((ch[30] & 0x0f)<<2) + ((ch[31] & 0xc0)>>6);
			str_temp.Format("%03i",index);
			mess[mess_cur][13]=str_temp;

			index=ch[31] & 0x3f;
			str_temp.Format("%03i",index);
			mess[mess_cur][17]=str_temp;

			//index
			for(i=0;i<4;i++)
			{
				message_nr=(WORD)(ch[25+i] *256) + ch[21+i];
				mess[mess_cur][6+4*i]=" ";
				str_temp.Format("%05i",message_nr);
				mess[mess_cur][6+4*i]+=str_temp;
			}

			//计算开始执行时间
			if(mess_cur==0)
			{
				for(i=0;i<4;i++) cycles_start[i]=cycles;
			}
			else
			{
				if( (mess[mess_cur][5]!=mess[mess_cur-1][5]) || (mess[mess_cur][6]!=mess[mess_cur-1][6]))
					cycles_start[0]=cycles_old;

				if( (mess[mess_cur][9]!=mess[mess_cur-1][9]) || (mess[mess_cur][10]!=mess[mess_cur-1][10]))
					cycles_start[1]=cycles_old;

				if( (mess[mess_cur][13]!=mess[mess_cur-1][13]) || (mess[mess_cur][14]!=mess[mess_cur-1][14]))
					cycles_start[2]=cycles_old;

				if( (mess[mess_cur][17]!=mess[mess_cur-1][17]) || (mess[mess_cur][18]!=mess[mess_cur-1][18]))
					cycles_start[3]=cycles_old;
			}

			//D H M S 计算执行时间
			if(cycles > cycles_old) 
			{
				for(i=0;i<4;i++)
				{
					mess[mess_cur][7+i*4]=" ";
					time=(float)((cycles-cycles_start[i]) * 204.8f /1000);

					absolute=(DWORD)(time/(3600*24)) & 0xff;
					if(absolute>9)	str_temp.Format("%02i",absolute);//天
					else str_temp.Format(" %01i",absolute);
					mess[mess_cur][7+i*4]+=str_temp;
					
					mess[mess_cur][7+i*4]+=" ";
					time-=(absolute*24);
					absolute=((DWORD)(time/3600)) & 0xff;
					if(absolute>9) str_temp.Format("%02i",absolute);//时
					else str_temp.Format(" %01i",absolute);
					mess[mess_cur][7+i*4]+=str_temp;

                    mess[mess_cur][7+i*4]+=" ";
					time-=(absolute*3600);
					absolute=((DWORD)(time/60)) & 0xff;
					if(absolute>9) str_temp.Format("%02i",absolute);//分
					else str_temp.Format(" %01i",absolute);
					mess[mess_cur][7+i*4]+=str_temp;

                    mess[mess_cur][7+i*4]+=" ";
					time-=(absolute*60);
					if(time>=10.0f) str_temp.Format("%2.1f",time); //秒
					else str_temp.Format(" %1.1f",time);
					mess[mess_cur][7+i*4]+=str_temp;
				}
			}
			else
			{
				for(i=0;i<4;i++)
					mess[mess_cur][7+i*4]="              ";
			}

			if(mess_cur==0)
			{
/*				str="";
				for(j=0;j<21;j++) str+=mess[0][j];
				str+="\n";
				Print_Log(&m_readtele,str);
*/				mess_cur++;
			}
			else if(mess_cur==1)
			{
				str="";
				for(j=0;j<7;j++) str+=mess[0][j];

				if((mess[0][5].Compare(mess[1][5])==0) && (mess[0][6].Compare(mess[1][6])==0))
					str+=str_null;
				else str+=mess[1][7];

				for(j=8;j<11;j++) str+=mess[0][j];
				if((mess[0][9].Compare(mess[1][9])==0) && (mess[0][10].Compare(mess[1][10])==0))
					str+=str_null;
				else str+=mess[1][11];

				for(j=12;j<15;j++) str+=mess[0][j];
				if((mess[0][13].Compare(mess[1][13])==0) && (mess[0][14].Compare(mess[1][14])==0))
					str+=str_null;
				else str+=mess[1][15];

				for(j=16;j<19;j++) str+=mess[0][j];
				if((mess[0][17].Compare(mess[1][17])==0) && (mess[0][18].Compare(mess[1][18])==0))
					str+=str_null;
				else str+=mess[1][19];

				str+=mess[0][20];

				str+="\n";
				Print_Log(&m_readtele,str);

				mess_cur++;
			}
			else if(mess_cur==2)
			{
				str="";
				for(i=0;i<5;i++) str+=mess[1][i];

				if((mess[0][5].Compare(mess[1][5])==0) && (mess[0][5].Compare(mess[2][5])==0) && (mess[0][6].Compare(mess[1][6])==0) && (mess[0][6].Compare(mess[2][6])==0))
					str+=dot;
				else if((mess[1][5].Compare(mess[2][5])==0) && (mess[1][6].Compare(mess[2][6])==0))
				{
					str+=mess[1][5];
					str+=mess[1][6];
					str+=str_null;
				}
				else
				{
					str+=mess[1][5];
					str+=mess[1][6];
				
					str+=mess[2][7];
				}
				str+=mess[1][8];
				

				if((mess[0][9].Compare(mess[1][9])==0) && (mess[0][9].Compare(mess[2][9])==0) && (mess[0][10].Compare(mess[1][10])==0) && (mess[0][10].Compare(mess[2][10])==0))
					str+=dot;
				else if((mess[1][9].Compare(mess[2][9])==0) && (mess[1][10].Compare(mess[2][10])==0))
				{
					str+=mess[1][9];
					str+=mess[1][10];

					str+=str_null;
				}
				else
				{
					str+=mess[1][9];
					str+=mess[1][10];
				
					str+=mess[2][11];
				}
				str+=mess[1][12];
				

				if((mess[0][13].Compare(mess[1][13])==0) && (mess[0][13].Compare(mess[2][13])==0) && (mess[0][14].Compare(mess[1][14])==0) && (mess[0][14].Compare(mess[2][14])==0))
					str+=dot;
				else if((mess[1][13].Compare(mess[2][13])==0) && (mess[1][14].Compare(mess[2][14])==0))
				{
					str+=mess[1][13];
					str+=mess[1][14];

					str+=str_null;
				}
				else
				{
					str+=mess[1][13];
					str+=mess[1][14];
				
					str+=mess[2][15];
				}
				str+=mess[1][16];
				

				if((mess[0][17].Compare(mess[1][17])==0) && (mess[0][17].Compare(mess[2][17])==0) && (mess[0][18].Compare(mess[1][18])==0) && (mess[0][18].Compare(mess[2][18])==0))
					str+=dot;
				else if((mess[1][17].Compare(mess[2][17])==0) && (mess[1][18].Compare(mess[2][18])==0))
				{
					str+=mess[1][17];
					str+=mess[1][18];

					str+=str_null;
				}
				else
                {
					str+=mess[1][17];
					str+=mess[1][18];
				
					str+=mess[2][19];
				}
				str+=mess[1][20];

				str+="\n";
				Print_Log(&m_readtele,str);

				for(i=0;i<21;i++) mess[0][i]=mess[1][i];
				for(i=0;i<21;i++) mess[1][i]=mess[2][i];
				mess_cur=2;
			}
		}
	}while(pos<len);

	if(mess_cur>0)
	{
		str="";
        for(i=0;i<5;i++) str+=mess[1][i];
		for(i=5;i<7;i++) str+=mess[1][i];
		str+=question;
		for(i=8;i<11;i++) str+=mess[1][i];
		str+=question;
		for(i=12;i<15;i++) str+=mess[1][i];
		str+=question;
		for(i=16;i<19;i++) str+=mess[1][i];
		str+=question;
		str+=mess[1][20];
		str+="\n";

		//str+=" cycle counter reset\n";

		Print_Log(&m_readtele,str);

		for(i=0;i<3;i++)
			for(j=0;j<21;j++) mess[i][j]="";
		mess_cur=0;
	}

	m_readtele.Close();

	return TRUE;
}

