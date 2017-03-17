

// THIS IS A FILE FOR CD96 MEASURE METER SOFTWARE.
// COMMUNICATION THAT COMPUTER AND CD96-2
// author: ZZF
// date:   2005.01.24
// CD96表调试使用

#include "stdafx.h"
#include "vfw.h"
#include "comm_balise_ts.h"
#include <afx.h>
#include <string.h>
#include <atltime.h>
#include "resource.h"
#include "Const.h"
#include <setupapi.h>

#pragma comment(lib,"Setupapi.lib")

//端口用变量
HANDLE hCom;
//char ComStr[20]="COM4";
CString ComStr="COM4";

//log文件
CStdioFile logfile;

//读入缓冲区
byte ReadBuf[MAXREADBUF];
DWORD ReadBufLen;

byte Read_Tele[100000];
DWORD Read_Tele_Len;
BOOL Read_Tele_End_Flag;

//初始化COM口
BOOL InitComm()
{
	COMMTIMEOUTS TimeOut;
	DCB dcb;
	DWORD ErrCode;
	COMSTAT Stat;

	CFile ff;
	
	//***************************************
	//Windows 2000下，指定Com口时使用
/*	char* pFileName = "com.txt";
	char TempBuf[5]="";
	char buf[256];
	GetModuleFileName(NULL,buf,256);
	*(strrchr(buf, '\\') + 1) = '\0';
	strcat(buf,pFileName);

	if(!ff.Open(buf,CFile::modeRead)){
		strncpy(ComStr,"COM4",5);
	}
	else{
		ff.Read(TempBuf,4);
		strncpy(ComStr,TempBuf,4);
		ff.Close();
	}
	//*****************************************
*/
	//************************************
	//无用
/*    char completeDeviceName[64] = ""; 

    strcat (completeDeviceName,
            "\\\\.\\?"
            );

    strcat (completeDeviceName,
		    "\\com4"
		    );

	//strncpy(ComStr,"com4",5);//ComStr
*/
	//*************************************

	//创建句柄
	hCom=CreateFile("com3",GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);//FILE_FLAG_OVERLAPPED FILE_ATTRIBUTE_NORMAL|
	if(hCom==INVALID_HANDLE_VALUE) return FALSE;
	
	//设置最大读，写缓冲区
	SetupComm(hCom,MAXREADBUF,MAXWRITEBUF);
	if(!GetCommState(hCom,&dcb)) return FALSE;

	//设置状态参数
	dcb.BaudRate=CBR_38400;//CBR_19200;//
	dcb.ByteSize=8;
	dcb.Parity=NOPARITY;
	dcb.StopBits=TWOSTOPBITS;
	dcb.fDtrControl=DTR_CONTROL_DISABLE;
	dcb.fOutxCtsFlow=false;
	dcb.fOutxDsrFlow=false;
	dcb.fRtsControl=RTS_CONTROL_ENABLE;
	dcb.fAbortOnError=false;
	dcb.fOutX=false;
	dcb.fInX=false;
	if(!SetCommState(hCom,&dcb)) return FALSE;

	//设置延时参数
	if(!GetCommTimeouts(hCom,&TimeOut)) return FALSE;
	TimeOut.ReadIntervalTimeout=100;//0xFFFFFFFF;
	TimeOut.ReadTotalTimeoutMultiplier=2;//;//0
	TimeOut.ReadTotalTimeoutConstant=500;//10000;
	TimeOut.WriteTotalTimeoutMultiplier=2;//2;
	TimeOut.WriteTotalTimeoutConstant=100;//0;
	if(!SetCommTimeouts(hCom,&TimeOut)) return FALSE;

	COMMPROP Prop;
	if(!GetCommProperties(hCom,&Prop)) return FALSE;
	//清缓冲区
	if(!PurgeComm(hCom,PURGE_TXCLEAR|PURGE_RXCLEAR)) return FALSE;

	//清传输错误
	if(!ClearCommError(hCom,&ErrCode,&Stat)) return FALSE;

	return TRUE;
}

BOOL WriteComm(HANDLE &hCom,void* lpBuffer,DWORD nNumberOfBytesToWrite)
{
	DWORD ErrCodeWrite;
	COMSTAT StatWrite;
	DWORD dwBytesSent=0;
	DWORD dwBytesWritten;
 
	//清错误
	if(!ClearCommError(hCom,&ErrCodeWrite,&StatWrite)) return FALSE;

	//写数据
	if(!WriteFile(hCom,lpBuffer,nNumberOfBytesToWrite,&dwBytesWritten,NULL))
		return FALSE;

	if(!ClearCommError(hCom,&ErrCodeWrite,&StatWrite)) return FALSE;
	return TRUE;
}

DWORD ReadComm(HANDLE &hCom,void* lpBuffer,DWORD nNumberOfBytesToRead)
{
	DWORD ErrCodeRead;
	COMSTAT StatRead;
	DWORD dwLength=0;

	if(!ClearCommError(hCom,&ErrCodeRead,&StatRead)) return 0;

	dwLength=nNumberOfBytesToRead;
	if(dwLength>0)
	{
		if(!ReadFile(hCom,lpBuffer,dwLength,&dwLength,NULL)) 
		{
			return 0;
		}
	}

	if(!ClearCommError(hCom,&ErrCodeRead,&StatRead)) return 0;
	return (dwLength);
}

BOOL CloseComm(HANDLE &hCom)
{
	SetCommMask(hCom,0);

	return CloseHandle(hCom);
}

//发送命令
BOOL SendCommand(int len,byte send[])
{
	byte pCmd[2048];
	DWORD n1;
	int pos=0,i;
	UINT total=0;

	//清发送缓冲区
	if(!PurgeComm(hCom,PURGE_TXCLEAR)) return FALSE;
	//清接收缓冲区
	if(!PurgeComm(hCom,PURGE_RXCLEAR)) return FALSE;

	for(pos=0;pos<2048;pos++) pCmd[pos]=0x00;
	
	pos=0;
	pCmd[pos++]=0xff;
	pCmd[pos++]=0x00;

	for(i=0;i<len;i++) pCmd[pos++]=send[i];

	for(i=0;i<(len+2);i++) total += pCmd[i];

	total &= 0xffff;
	pCmd[pos++]=byte(total & 0x0ff);
	total>>=8;
	pCmd[pos++]=byte(total & 0x0ff);
	
	pCmd[pos++]=0x55;
	pCmd[pos++]=0xaa;

	n1=pos;

	/*for(pos=0;pos<24;pos++)     
	{
		pCmd[pos++]=0x55;
		pCmd[pos]=0xaa;
	}*/
		

	//写Comm口
	if(!WriteComm(hCom,pCmd,n1)) return FALSE;

	return TRUE;
}

void ClearData()
{
	int i;
	for(i=0;i<MAXREADBUF;i++)  ReadBuf[i]=0x00;
}

//整理接收数据
BOOL ArrangeData(byte order)
{
	int i;
	int pos;
	int num=0;
	UINT len_data=0;
	WORD total=0,total1=0;
	WORD count=0,allcount=0;
 
	pos=0;
	do{
		if((ReadBuf[pos]== 0xff) && ReadBuf[pos+1]==0x00)
		{
			len_data=UINT(ReadBuf[pos+4] *256) + ReadBuf[pos+3];

			num=2+1+2+len_data+2+2+pos;
			if(num<=(int)ReadBufLen)
			{
				if(ReadBuf[num-2] == 0x55 && ReadBuf[num-1]==0xaa)
				{
					total=0;
					for(i=pos;i<(num-4);i++) total+=ReadBuf[i];
					total1=ReadBuf[num-3] *256 + ReadBuf[num-4];
					if(total==total1)
					{
						for(i=0;i<(int)len_data;i++)
							Read_Tele[Read_Tele_Len+i]=ReadBuf[pos+5+i];
						Read_Tele_Len+=len_data;

						if(order==ORDER_LEU_READ)
						{
							//判断标识
							if(ReadBuf[pos+5]!=0x21) return FALSE;
							//判断总数
							count=ReadBuf[pos+5+3] + ReadBuf[pos+5+4] * 256;
							allcount=ReadBuf[pos+5+5] + ReadBuf[pos+5+6] * 256;
							if(count==allcount) 
								Read_Tele_End_Flag=TRUE; //结束
						}
						return TRUE;
					}
				}
			}
		}

		pos++;
	}while(pos<(int)(ReadBufLen-1));
	
	return FALSE;
}

//数据接收
BOOL ReceiveData(int readlen)
{
	DWORD byteRead;
	byte *lpReadBuf;
	BOOLEAN bRet=TRUE;
	int i=0;

	ReadBufLen=0;
	lpReadBuf=ReadBuf;
	for(int i=0;i<MAXREADBUF;i++)  ReadBuf[i]=0x00;
	
	if(!SetCommMask(hCom,EV_RXCHAR)) return FALSE;
	
	byteRead=ReadComm(hCom,lpReadBuf,readlen);
	if(byteRead>0)
	{
		ReadBufLen=byteRead;
	}
	else bRet=FALSE;

	SetCommMask(hCom,0);
	return bRet;
}

int chartoint(unsigned char chr, unsigned char *cint)
{
	unsigned char cTmp;
	cTmp=chr-48;
	if(cTmp>=0&&cTmp<=9)
	{
		*cint=cTmp;
		return 0;
	}
	cTmp=chr-65;
	if(cTmp>=0&&cTmp<=5)
	{
		*cint=(cTmp+10);
		return 0;
	}
	cTmp=chr-97;
	if(cTmp>=0&&cTmp<=5)
	{
		*cint=(cTmp+10);
		return 0;
	}
	return 1;
}

int strtodata(unsigned char *str, unsigned char *data, int len, int flag)
{
	unsigned char cTmp=0;
	int i=0;
	for(int j=0;j<len;j++)
	{
		if(chartoint(str[i++],&cTmp))
			return 1;
		data[j]=cTmp;
		if(chartoint(str[i++],&cTmp))
			return 1;
		data[j]=(data[j]<<4)+cTmp;
		if(flag==1)
			i++;
	}
	return 0;
}

int strtodata(CString source_str,  DWORD *data, int len,int flag)
{
	CString str=source_str;
	unsigned char cTmp=0;
	int i=0;
	char ch[256];

    int strlen=str.GetLength();
	strncpy(ch,str,strlen);

	for(int j=0;j<len;j++)
	{
		//4字节
		if(chartoint(ch[i++],&cTmp))
			return 1;
		data[j]=cTmp;

		if(chartoint(ch[i++],&cTmp))
			return 1;
		data[j]=(data[j]<<4)+cTmp;

		if(chartoint(ch[i++],&cTmp))
			return 1;
		data[j]=(data[j]<<4)+cTmp;

		if(chartoint(ch[i++],&cTmp))
			return 1;
		data[j]=(data[j]<<4)+cTmp;

		if(chartoint(ch[i++],&cTmp))
			return 1;
		data[j]=(data[j]<<4)+cTmp;

		if(chartoint(ch[i++],&cTmp))
			return 1;
		data[j]=(data[j]<<4)+cTmp;

		if(chartoint(ch[i++],&cTmp))
			return 1;
		data[j]=(data[j]<<4)+cTmp;

		if(chartoint(ch[i++],&cTmp))
			return 1;
		data[j]=(data[j]<<4)+cTmp;

		if(flag==1)
			i++;
	}
	return 0;
}

BOOL InitTelegram(CString ff,byte send[])
{
	CFile file;
	char pbuf[1000];
	char pbuf1[1000];
	UINT nBytesRead;
	int i;
	CFileException ex;
	char ch[5];
	int j,k;
	int m;
	BOOL m_find=FALSE;
	int len;

	if(!file.Open(ff,CFile::modeRead,&ex))
		return FALSE;
	
	file.SeekToBegin();
	do{
		nBytesRead=file.Read(pbuf,1);
		
		if(nBytesRead<=0) break;

		if(pbuf[0]=='.')
		{
			nBytesRead=file.Read(pbuf,4);
			if((pbuf[0]=='T') && (pbuf[1]=='G') && (pbuf[2]=='M') && ((pbuf[3]=='L') || (pbuf[3]=='S')))
			{
				m_find=TRUE;
				if(pbuf[3]=='L') len=256;
				else if(pbuf[3]=='S') len=86;
				break;
			}
		}
	}while(nBytesRead>0);

	if(m_find)
	{
		k=0;
		do{
			nBytesRead=file.Read(pbuf,1);
			if(nBytesRead>0)
			{
				for(i=0;i<(int)nBytesRead;i++)
				{
					if((pbuf[i]>=0x30 && pbuf[i]<=0x39) || (pbuf[i]>=0x41 && pbuf[i]<=0x46) ||(pbuf[i]>=0x61 && pbuf[i]<=0x66))
					{
						strncpy(&pbuf1[k],&pbuf[i],1);
						k++;
					}
					else if(pbuf[i]!=' ' && pbuf[i]!=0x0d && pbuf[i]!=0x0a)
					{
						if(k<len) m_find=FALSE;
						break;
					}
				}
				if(m_find==FALSE) break;
			}
			else
			{
				if(k<len) m_find=FALSE;
				break;
			}
		}while(k<len);

		if(m_find)
		{
			j=0;
			m=0;
			while(j<(k-1))
			{
				strncpy(&ch[0],&pbuf1[j],2);
				m++;
				j+=2;
                strtodata((unsigned char*)ch,&send[m],1,0);
				if(m==(len/2)) break;
			};
			send[0]=m;
		}
	}

	file.Close();

	return m_find;
}

unsigned int cal_crc(unsigned char *ptr,unsigned int len)
{
	unsigned char i;
	unsigned int crc=0;

	while(len--!=0)
	{
		for(i=0x80;i!=0;i/=2)
		{
			if((crc & 0x8000)!=0)
			{
				crc*=2;
				crc^=0x0811;
			}
			else crc*=2;
			if((*ptr & i)!=0) crc^=0x0811;
		}
		ptr++;
	}

	return(crc);
}

ULONG crc32_table[256];
ULONG Reflect(ULONG ref,byte ch)
{
	ULONG value=0;

	//交换bit0 和 bit7, bit1 和 bit6，.....
	for(int i=1;i<(ch+1);i++)
	{
		if(ref & 1) value|= (1<<(ch-i));
		ref>>=1;
	}

	return value;
}

void init_crc32_table()
{
	ULONG crc,temp;

	for(int i=0;i<0xff;i++)
	{
		temp=Reflect(i,8);
		crc32_table[i]=temp<<24;

		for(int j=0;j<8;j++)
		{
			ULONG t1,t2;
			ULONG flag=crc32_table[i] & 0x80000000;

			t1=(crc32_table[i]<<1);
			if(flag==0) t2=0;
			else t2=0x04c11db7;

			crc32_table[i]= t1 ^ t2;
		}
		crc=crc32_table[i];
		crc32_table[i]=Reflect(crc32_table[i],32);
	}

}

ULONG cal_crc32(byte* data,ULONG len)
{
	ULONG oldcrc32;
	ULONG crc32;
	ULONG oldcrc;
	UINT charcnt;

	byte c,t;

	oldcrc32=0x00000000;

	charcnt=0;

	while(len--)
	{
		t=(byte)((oldcrc32>>24) & 0xff);
		oldcrc=crc32_table[t];
		c=data[charcnt];

		oldcrc32=(oldcrc32<<8) | c;

		oldcrc32=(oldcrc32 ^ oldcrc);

		charcnt++;
	}

	crc32=oldcrc32;

	return crc32;
}


BOOL Open_LogFile()
{
	CFileException ex;
	CString ff;
	CTime  CurrentTime;
	CString str;

	CurrentTime=CTime::GetCurrentTime();
	int hour=CurrentTime.GetHour();

	ff=CurrentTime.Format("%Y-%m-%d-%H-%M-%S");
	ff+=".txt";

	if(!logfile.Open(ff,CFile::modeCreate |CFile::modeNoTruncate| CFile::modeWrite ,&ex))
		return FALSE;
	
	str=CurrentTime.Format("开始----%Y年%m月%d日%H点%M分%S秒\n\n");

	Print_Log(&logfile,"应答器维护-----版本1.00\n");
	Print_Log(&logfile,"------------\n");
	Print_Log(&logfile,"Log File\n");
    Print_Log(&logfile,"------------\n\n");
	Print_Log(&logfile,str);

	return TRUE;
}

BOOL Close_LogFile()
{
	if(logfile) logfile.Close();

	return TRUE;
}

//向文件中写字符内容
void Print_Log(CStdioFile* ptr_file,CString ss)
{
	CFileException ex;

	if(ptr_file->m_hFile != CFile::hFileNull)
	{
		ptr_file->SeekToEnd();
		ptr_file->WriteString(ss);
	}
}


//数据向左移位
//data[]:数据缓冲区
//len_data:要移动的数据个数
//len_shift_bit:每个字节移动的位数;0~8
void left_shift_bit(byte data[],int len_data,int len_shift_bit)
{
	int i;
	byte temp;

	if(len_shift_bit<0) len_shift_bit=0;
	else if(len_shift_bit>8) len_shift_bit=8;

	for(i=0;i<len_data;i++)
	{
		temp=data[i]<<len_shift_bit;
		if( i != (len_data-1) ) temp+=(data[i+1]>>(8-len_shift_bit));

		temp&=0xff;
		data[i]=temp;
	}
}

//数据向左移位，并复制数组
//data[]:数据源数组
//len_data:数据源数组长度
//dup_mul:目标数组是源数组的倍数，即复制 dup_mul-1倍数据。
//len_shift_bits:每个字节移动的位数;0~8
//total_len:返回数组有效数据的总长度。
void left_shift_bits(byte data[],int len_data,int dup_mul,int len_shift_bits,int* total_len)
{
	int j,k;
	byte temp,temp1;
	int del_byte=0;
	byte last_data;

	if(len_shift_bits<0) len_shift_bits=0;
	else if(len_shift_bits>8) len_shift_bits=8;

	//保存源数组最后一个数。
	last_data=data[len_data-1];

	for(k=1;k<dup_mul;k++)
	{
		//复制
		for(j=0;j<len_data;j++)
		{
			data[len_data*k+j-del_byte]=data[j];
		}
		data[len_data*(k+1)-1-del_byte]=last_data;
	
		//处理:第一个数有多少位移到上次最后一个数据
		temp1=data[len_data*k-del_byte];
		temp1>>=(8-(len_shift_bits*k)%8);
		
		//上次最后一个数据留下的位
		temp=0xff;
		temp<<=(len_shift_bits*k)%8;//移出
		data[len_data*k-del_byte-1] &=temp;

		//得到上次最后一个数据
		data[len_data*k-del_byte-1] |=temp1;

		//后边所有数据移位
		left_shift_bit(&data[len_data*k-del_byte],len_data,(len_shift_bits*k)%8);

		del_byte=(k*len_shift_bits)/8;
	}

	del_byte=(dup_mul*len_shift_bits)/8;
	(*total_len)=len_data*dup_mul-del_byte;
}


CString UserTelegram_Description(int list_serial,int pos)
{
	CString m_header[10]={"Q_UPDOWN","M_VERSION","Q_MEDIA","N_PIG","N_TOTAL",
						"M_DUP","M_MCOUNT","NID_C","NID_BG","Q_LINK"};

	CString m_etcs5[19]={"NID_PACKET","Q_DIR","L_PACKET","Q_SCALE",
		"D_LINK","Q_NEWCOUNTRY","NID_C","NID_BG","Q_LINKORIENTATION","Q_LINKREADCTION","Q_LINKACC",
		"N_ITER","D_LINK","Q_NEWCOUNTRY","NID_C","NID_BG","Q_LINKORIENTATION","Q_LINKREACTION","Q_LINKACC"};
	
	CString m_etcs21[11]={"NID_PACKET","Q_DIR","L_PACKET","Q_SCALE",
		"D_GRADIENT","Q_GDIR","G_A",
		"N_ITER","D_GRADIENT","Q_GDIR","G_A"};

	CString m_etcs27[17]={"NID_PACKET","Q_DIR","L_PACKET","Q_SCALE",
		"D_LEVELTR","V_STATIC","Q_FRONT","N_ITER","NC_DIFF","V_DIFF",
		"N_ITER","D_STATIC","V_STATIC","Q_FRONT","N_ITER","NC_DIFF","V_DIFF"};

	CString m_etcs41[12]={"NID_PACKET","Q_DIR","L_PACKET","Q_SCALE",
		"D_LEVELTR","M_LEVELTR","NID_STM","L_ACKLEVELTR",
		"N_ITER","M_LEVELTR","NID_STM","L_ACKLEVELTR"};

	CString m_etcs68[13]={"NID_PACKET","Q_DIR","L_PACKET","Q_SCALE",
		"Q_TRACKINIT","D_TRACKINIT","D_TRACKCOND","L_TRACKCOND","M_TRACKCOND",
		"N_ITER","D_TRACKCOND","L_TRACKCOND","M_TRACKCOND"};

	CString m_etcs132[4]={"NID_PACKET","Q_DIR","L_PACKET","Q_ASPECT"};

	CString m_etcs254[3]={"NID_PACKET","Q_DIR","L_PACKET"};


	CString m_etcs44[4]={"NID_PACKET","Q_DIR","L_PACKET","NID_XUSER"};

	CString m_ctcs1[11]={"Q_DIR","L_PACKET","Q_SCALE",
		"D_SIGNAL",
		"NID_SIGNAL","NID_FREQUENCY","L_SECTION",
		"N_ITER","NID_SIGNAL","NID_FREQUENCY","L_SECTION"};

	CString m_ctcs2[13]={"Q_DIR","L_PACKET","Q_SCALE",
		"L_TSRarea",
		"D_TSR","L_TSR","Q_FRONT","V_TSR",
		"N_ITER","D_TSR","L_TSR","Q_FRONT","V_TSR"};

	CString m_ctcs3[5]={"Q_DIR","L_PACKET","Q_SCALE",
		"D_STARTREVERSE","L_REVERSEAREA"};

	CString m_ctcs4[5]={"Q_DIR","L_PACKET","Q_SCALE",
		"D_TURNOUT","V_TURNOUT"};

	int len,len1,len2;
	int i;
	CString null_s="";

	//计算长度
	len2=0;
	switch(list_serial)
	{
	case 0:
		for(i=0;i<9;i++)
		{
			len=m_header[i].GetLength();
			if(len>len2) len2=len;
		}

		len2+=2;
		break;
	case 5:
	case 21:
	case 27:
	case 41:
	case 44:
	case 68:
	case 132:
	case 254:
		for(i=0;i<19;i++)
		{
			len=m_etcs5[i].GetLength();
			if(len>len2) len2=len;
		}
		for(i=0;i<11;i++)
		{
			len=m_etcs21[i].GetLength();
			if(len>len2) len2=len;
		}
		for(i=0;i<17;i++)
		{
			len=m_etcs27[i].GetLength();
			if(len>len2) len2=len;
		}
		for(i=0;i<12;i++)
		{
			len=m_etcs41[i].GetLength();
			if(len>len2) len2=len;
		}
		for(i=0;i<4;i++)
		{
			len=m_etcs44[i].GetLength();
			if(len>len2) len2=len;
		}
		for(i=0;i<13;i++)
		{
			len=m_etcs68[i].GetLength();
			if(len>len2) len2=len;
		}
		for(i=0;i<4;i++)
		{
			len=m_etcs132[i].GetLength();
			if(len>len2) len2=len;
		}
		for(i=0;i<3;i++)
		{
			len=m_etcs254[i].GetLength();
			if(len>len2) len2=len;
		}
		len2+=2;
		break;
	case 1:
	case 2:
	case 3:
	case 4:
		for(i=0;i<11;i++)
		{
			len=m_ctcs1[i].GetLength();
			if(len>len2) len2=len;
		}
		for(i=0;i<13;i++)
		{
			len=m_ctcs2[i].GetLength();
			if(len>len2) len2=len;
		}
		for(i=0;i<5;i++)
		{
			len=m_ctcs3[i].GetLength();
			if(len>len2) len2=len;
		}
		for(i=0;i<5;i++)
		{
			len=m_ctcs4[i].GetLength();
			if(len>len2) len2=len;
		}
		len2+=2;
		break;
	};

	switch(list_serial)
	{
	case 0://报文信息帧
		if(pos>=10 || pos<0) return null_s;

        len=m_header[pos].GetLength();
		len1=len2-len;
		for(i=0;i<len1;i++)	m_header[pos]+=" ";

		return m_header[pos];

		break;
	case 5://ETCS-5
		if(pos>=19 || pos<0) return null_s;

        len=m_etcs5[pos].GetLength();
		len1=len2-len;
		for(i=0;i<len1;i++)	m_etcs5[pos]+=" ";
		m_etcs5[pos]="\t"+m_etcs5[pos]+": ";

		return m_etcs5[pos];

		break;
	case 21://ETCS-21
		if(pos>=11 || pos<0) return null_s;

        len=m_etcs21[pos].GetLength();
		len1=len2-len;
		for(i=0;i<len1;i++)	m_etcs21[pos]+=" ";
		m_etcs21[pos]="\t"+m_etcs21[pos]+": ";

		return m_etcs21[pos];
		break;
	case 27://ETCS-27
		if(pos>=17 || pos<0) return null_s;

        len=m_etcs27[pos].GetLength();
		len1=len2-len;
		for(i=0;i<len1;i++)	m_etcs27[pos]+=" ";
		m_etcs27[pos]="\t"+m_etcs27[pos]+": ";

		return m_etcs27[pos];
		break;
	case 41://ETCS-41
		if(pos>=12 || pos<0) return null_s;

        len=m_etcs41[pos].GetLength();
		len1=len2-len;
		for(i=0;i<len1;i++)	m_etcs41[pos]+=" ";
		m_etcs41[pos]="\t"+m_etcs41[pos]+": ";

		return m_etcs41[pos];
		break;
	case 44://ETCS-44
		if(pos>=4 || pos<0) return null_s;

        len=m_etcs44[pos].GetLength();
		len1=len2-len;
		for(i=0;i<len1;i++)	m_etcs44[pos]+=" ";
		m_etcs44[pos]="\t"+m_etcs44[pos]+": ";

		return m_etcs44[pos];
		break;
	case 68://ETCS-68
		if(pos>=4 || pos<0) return null_s;

        len=m_etcs68[pos].GetLength();
		len1=len2-len;
		for(i=0;i<len1;i++)	m_etcs68[pos]+=" ";
		m_etcs68[pos]="\t"+m_etcs68[pos]+": ";

		return m_etcs68[pos];
		break;
	case 132://ETCS-132
		if(pos>=4 || pos<0) return null_s;

        len=m_etcs132[pos].GetLength();
		len1=len2-len;
		for(i=0;i<len1;i++)	m_etcs132[pos]+=" ";
		m_etcs132[pos]="\t"+m_etcs132[pos]+": ";

		return m_etcs132[pos];
		break;
	case 254://ETCS-254
		if(pos>=3 || pos<0) return null_s;

        len=m_etcs254[pos].GetLength();
		len1=len2-len;
		for(i=0;i<len1;i++)	m_etcs254[pos]+=" ";
		m_etcs254[pos]="\t"+m_etcs254[pos]+": ";

		return m_etcs254[pos];
		break;
	case 1://CTCS-1
		if(pos>=11 || pos<0) return null_s;

        len=m_ctcs1[pos].GetLength();
		len1=len2-len;
		for(i=0;i<len1;i++)	m_ctcs1[pos]+=" ";
		m_ctcs1[pos]="\t\t"+m_ctcs1[pos]+": ";

		return m_ctcs1[pos];
		break;
	case 2://CTCS-2
		if(pos>=13 || pos<0) return null_s;

        len=m_ctcs2[pos].GetLength();
		len1=len2-len;
		for(i=0;i<len1;i++)	m_ctcs2[pos]+=" ";
		m_ctcs2[pos]="\t\t"+m_ctcs2[pos]+": ";

		return m_ctcs2[pos];
		break;
	case 3://CTCS-3
		if(pos>=5 || pos<0) return null_s;

        len=m_ctcs3[pos].GetLength();
		len1=len2-len;
		for(i=0;i<len1;i++)	m_ctcs3[pos]+=" ";
		m_ctcs3[pos]="\t\t"+m_ctcs3[pos]+": ";

		return m_ctcs3[pos];
		break;
	case 4://CTCS-4
		if(pos>=5 || pos<0) return null_s;

        len=m_ctcs4[pos].GetLength();
		len1=len2-len;
		for(i=0;i<len1;i++)	m_ctcs4[pos]+=" ";
		m_ctcs4[pos]="\t\t"+m_ctcs4[pos]+": ";

		return m_ctcs4[pos];
		break;
	}
	return null_s;
}

//从数组中按 Bit 取数据
//usertele[]：报文信息数组
//len_array：报文信息数组长度
//get_bit_len：取0~15位数据
DWORD Get_Data_From_Byte(byte usertele[],int len_array,int get_bit_len)
{
	DWORD get_data;
	byte temp,temp1;
	byte and_data;
	int i;

	get_data=0;
	if(get_bit_len<0 || get_bit_len>15) return get_data;

	temp1=0;
	temp=0;
	if(get_bit_len>=8 && get_bit_len<=15)
	{
		temp1=usertele[0];
		left_shift_bit(usertele,len_array,8);

		get_bit_len-=8;
	}

	and_data=0;
	for(i=0;i<get_bit_len;i++)
	{
		and_data<<=1;
		and_data|=0x01;
	}

	if(get_bit_len>0)
	{
		temp=(usertele[0]>>(8-get_bit_len));
		temp&=and_data;
		get_data=temp;

		left_shift_bit(usertele,len_array,get_bit_len);

		get_data+=(DWORD)(temp1<<get_bit_len);
	}
	else get_data+=(DWORD)(temp1);

	return get_data;
}

//格式化报文数据
//data：要显示的数据
//bits_len：显示长度
CString UserTelegram_Data_Format(DWORD data,int bits_len)
{
	CString ss="";

	if(bits_len>=0 && bits_len<=4)
		ss.Format("%d (%X)\n",data,data);
	else if(bits_len>4 && bits_len<=8)
		ss.Format("%d (%02X)\n",data,data);
	else if(bits_len>8 && bits_len<=12)
		ss.Format("%d (%03X)\n",data,data);
	else ss.Format("%d (%04X)\n",data,data);

	return ss;
}

//分析报文信息帧
//file：写入文件指针
//usertele[]：报文信息数组
//len_array：报文信息数组长度
void Analyse_Usertele_Header(CStdioFile* file,byte usertele[],int len_array)
{
	DWORD temp;
	CString ss,ss1;
	int frame_var_num=10;
	int bit_num[10]={1,7,1,3,3,2,8,10,14,1};
	int i;
	DWORD nid;

	Print_Log(file,"Header balise telegram\n");

	//信息传送的方向；版本编号；信息传输媒介；本应答器在应答器组中的位置；
	//应答器组中所包含的应答器数量；本应答器信息与前后应答器信息的关系；
	//报文计数器；地区编号；应答器（组）编号;应答器的链接关系
	
	for(i=0;i<frame_var_num;i++)
	{
        temp=Get_Data_From_Byte(usertele,len_array,bit_num[i]);
		
		ss=UserTelegram_Data_Format(temp,bit_num[i]);
		ss1=UserTelegram_Description(0,i);
		ss1+=ss;
		Print_Log(file,ss1);

		//2007-6-20f add
		if((i==7) || (i==8))
		{
			if(i==7)//NID_C
			{
				nid=temp;
				temp=((nid>>3) & 0x7f);
				ss = UserTelegram_Data_Format(temp,7);
				ss.Remove('\n');
				ss1="    大区编号:";
				ss1+=ss;

				temp=(nid & 0x07);
				ss = UserTelegram_Data_Format(temp,3);
				ss1+="; 分区编号:";
				ss1+=ss;
				Print_Log(file,ss1);
			}
			else if(i==8) //NID_BG
			{
				nid=temp;
				temp=((nid>>8) & 0x3f);
				ss = UserTelegram_Data_Format(temp,6);
				ss.Remove('\n');
				ss1="    车站编号:";
				ss1+=ss;

				temp=(nid & 0xff);
				ss = UserTelegram_Data_Format(temp,8);
				ss1+="; 应答器编号:";
				ss1+=ss;
				Print_Log(file,ss1);
			}
		}
		//***********************
	};

}

//分析用户信息包：ETCS-5
//file：写入文件指针
//usertele[]：报文信息数组
//len_array：报文信息数组长度
BOOL Analyse_Usertele_Etcs5(CStdioFile* file,byte usertele[],int len_array)
{
	DWORD temp;
	CString ss,ss1;
	int frame_var_num=12;
	int bit_num[12]={8,2,13,2,15,1,10,14,1,2,6,5};
	
	DWORD n_iter;
	int iter_var_num=7;
	int iter_num[7]={15,1,10,14,1,2,6};

	DWORD m_l_packet=-1;
	DWORD len_packet=0;
	bool error_flag=false;
	DWORD error_len=0; 

	int i,j;

	for(i=0;i<frame_var_num;i++)
	{
		//*****************************
		len_packet+=bit_num[i];
		if(i>2)
		{
			if(len_packet>m_l_packet)
			{
				error_flag=true;
				error_len=m_l_packet-(len_packet-bit_num[i]);
				break;
			}
		}
		//*****************************
		temp=Get_Data_From_Byte(usertele,len_array,bit_num[i]);
	
		if(i==0) ss.Format("%d 应答器链接\n",temp);
		else ss=UserTelegram_Data_Format(temp,bit_num[i]);

		ss1=UserTelegram_Description(5,i);
		ss1+=ss;
		Print_Log(file,ss1);

		//********************************
		if((i==5) && (temp!=1)) i++;
		//********************************
		if(i==2) m_l_packet=temp;
		//********************************
	};

	//数量
	if(!error_flag)
	{
		n_iter=temp;
		for(i=0;i<(int)n_iter;i++)
		{
			for(j=0;j<iter_var_num;j++)
			{
				//*****************************
				len_packet+=iter_num[j];
				if(len_packet>m_l_packet)
				{
					error_flag=true;
					error_len=m_l_packet-(len_packet-iter_num[j]);
					break;
				}
				//*****************************

				temp=Get_Data_From_Byte(usertele,len_array,iter_num[j]);
			
				ss=UserTelegram_Data_Format(temp,iter_num[j]);
				ss1=UserTelegram_Description(5,j+frame_var_num);
				ss1+=ss;
				Print_Log(file,ss1);

				//*************************
				if((j==1) && (temp!=1)) j++;
				//*************************
			}
			//********************
			if(error_flag) break;
			//********************
		}
	}
	
	//读掉错误数据***********************
	if(error_flag)
	{
		temp=Get_Data_From_Byte(usertele,len_array,(int)error_len);
	}

	return (!error_flag);
}

//分析用户信息包：ETCS-21
//file：写入文件指针
//usertele[]：报文信息数组
//len_array：报文信息数组长度
BOOL Analyse_Usertele_Etcs21(CStdioFile* file,byte usertele[],int len_array)
{
	DWORD temp;
	CString ss,ss1;
	int frame_var_num=8;
	int bit_num[8]={8,2,13,2,15,1,8,5};
	
	DWORD n_iter;
	int iter_var_num=3;
	int iter_num[3]={15,1,8};

	DWORD m_l_packet=-1;
	DWORD len_packet=0;
	bool error_flag=false;
	DWORD error_len=0; 

	int i,j;

	for(i=0;i<frame_var_num;i++)
	{
		//*****************************
		len_packet+=bit_num[i];
		if(i>2)
		{
			if(len_packet>m_l_packet)
			{
				error_flag=true;
				error_len=m_l_packet-(len_packet-bit_num[i]);
				break;
			}
		}
		//*****************************
        temp=Get_Data_From_Byte(usertele,len_array,bit_num[i]);
		
		if(i==0) ss.Format("%d 线路坡度\n",temp);
		else ss=UserTelegram_Data_Format(temp,bit_num[i]);

		ss1=UserTelegram_Description(21,i);
		ss1+=ss;
		Print_Log(file,ss1);

		//********************************
		if(i==2) m_l_packet=temp;
		//********************************
	};

	//数量
	if(!error_flag)
	{
		n_iter=temp;
		for(i=0;i<(int)n_iter;i++)
		{
			for(j=0;j<iter_var_num;j++)
			{
				//*****************************
				len_packet+=iter_num[j];
				if(len_packet>m_l_packet)
				{
					error_flag=true;
					error_len=m_l_packet-(len_packet-iter_num[j]);
					break;
				}
				//*****************************

				temp=Get_Data_From_Byte(usertele,len_array,iter_num[j]);
			
				ss=UserTelegram_Data_Format(temp,iter_num[j]);
				ss1=UserTelegram_Description(21,j+frame_var_num);
				ss1+=ss;
				Print_Log(file,ss1);
			}
			//*******************
			if(error_flag) break;
			//*******************
		}
	}

	//读掉错误数据***********************
	if(error_flag)
	{
		temp=Get_Data_From_Byte(usertele,len_array,(int)error_len);
	}

	return (!error_flag);
}

//分析用户信息包：ETCS-27
//file：写入文件指针
//usertele[]：报文信息数组
//len_array：报文信息数组长度
BOOL Analyse_Usertele_Etcs27(CStdioFile* file,byte usertele[],int len_array)
{
	DWORD temp;
	CString ss,ss1;
	int frame_var_num=8;
	int bit_num[8]={8,2,13,2,15,7,1,5};
	
	DWORD n_iter1;
	int iter_var_num1=2;
	int iter_num1[2]={4,7};

	DWORD n_iter2;
	int iter_var_num2=4;
	int iter_num2[4]={15,7,1,5};

	DWORD n_iter3;
	int iter_var_num3=2;
	int iter_num3[2]={4,7};

	DWORD m_l_packet=-1;
	DWORD len_packet=0;
	bool error_flag=false;
	DWORD error_len=0; 

	int i,j,m,n;

	for(i=0;i<frame_var_num;i++)
	{
		//*****************************
		len_packet+=bit_num[i];
		if(i>2)
		{
			if(len_packet>m_l_packet)
			{
				error_flag=true;
				error_len=m_l_packet-(len_packet-bit_num[i]);
				break;
			}
		}
		//*****************************

        temp=Get_Data_From_Byte(usertele,len_array,bit_num[i]);
		
		if(i==0) ss.Format("%d 线路速度\n",temp);
		else ss=UserTelegram_Data_Format(temp,bit_num[i]);

		ss1=UserTelegram_Description(27,i);
		ss1+=ss;
		Print_Log(file,ss1);

		//********************************
		if(i==2) m_l_packet=temp;
		//********************************
	};

	//数量
	if(!error_flag)
	{
		n_iter1=temp;
		for(i=0;i<(int)n_iter1;i++)
		{
			for(j=0;j<iter_var_num1;j++)
			{
				//*****************************
				len_packet+=iter_num1[j];
				if(len_packet>m_l_packet)
				{
					error_flag=true;
					error_len=m_l_packet-(len_packet-iter_num1[j]);
					break;
				}
				//*****************************

				temp=Get_Data_From_Byte(usertele,len_array,iter_num1[j]);
			
				ss=UserTelegram_Data_Format(temp,iter_num1[j]);
				ss1=UserTelegram_Description(27,j+frame_var_num);
				ss1+=ss;
				Print_Log(file,ss1);
			}
			//******************
			if(error_flag) break;
			//******************
		}
	}

	//
	if(!error_flag)
	{
		//*****************************
		len_packet+=5;
		if(len_packet>m_l_packet)
		{
			error_flag=true;
			error_len=m_l_packet-(len_packet-5);
		}
		else
		{
		//*****************************
			temp=Get_Data_From_Byte(usertele,len_array,5);
			
			ss=UserTelegram_Data_Format(temp,5);
			ss1=UserTelegram_Description(27,frame_var_num + iter_var_num1);
			ss1+=ss;
			Print_Log(file,ss1);
		}
	}

	if(!error_flag)
	{
		n_iter2=temp;
		for(i=0;i<(int)n_iter2;i++)
		{
			for(j=0;j<iter_var_num2;j++)
			{
				//*****************************
				len_packet+=iter_num2[j];
				if(len_packet>m_l_packet)
				{
					error_flag=true;
					error_len=m_l_packet-(len_packet-iter_num2[j]);
					break;
				}
				//*****************************
				temp=Get_Data_From_Byte(usertele,len_array,iter_num2[j]);
			
				ss=UserTelegram_Data_Format(temp,iter_num2[j]);
				ss1=UserTelegram_Description(27,frame_var_num + iter_var_num1 +1 +j);
				ss1+=ss;
				Print_Log(file,ss1);
			}
			//******************
			if(error_flag) break;
			//******************

			n_iter3=temp;
			for(m=0;m<(int)n_iter3;m++)
			{
				for(n=0;n<iter_var_num3;n++)
				{
					//*****************************
					len_packet+=iter_num3[n];
					if(len_packet>m_l_packet)
					{
						error_flag=true;
						error_len=m_l_packet-(len_packet-iter_num3[n]);
						break;
					}
					//*****************************
					temp=Get_Data_From_Byte(usertele,len_array,iter_num3[n]);
				
					ss=UserTelegram_Data_Format(temp,iter_num3[n]);
					ss1=UserTelegram_Description(27,frame_var_num + iter_var_num1 +1 +iter_var_num2 +n);
					ss1+=ss;
					Print_Log(file,ss1);
				}
				//******************
				if(error_flag) break;
				//******************
			}
			//******************
			if(error_flag) break;
			//******************
		}
	}
	//读掉错误数据***********************
	if(error_flag)
	{
		temp=Get_Data_From_Byte(usertele,len_array,(int)error_len);
	}

	return (!error_flag);
	
}

//分析用户信息包：ETCS-41
//file：写入文件指针
//usertele[]：报文信息数组
//len_array：报文信息数组长度
BOOL Analyse_Usertele_Etcs41(CStdioFile* file,byte usertele[],int len_array)
{
	DWORD temp;
	CString ss,ss1;
	int frame_var_num=9;
	int bit_num[9]={8,2,13,2,15,3,8,15,5};
	
	DWORD n_iter;
	int iter_var_num=3;
	int iter_num[3]={3,8,15};

	DWORD m_l_packet=-1;
	DWORD len_packet=0;
	bool error_flag=false;
	DWORD error_len=0; 

	int i,j;

	for(i=0;i<frame_var_num;i++)
	{
 		//*****************************
		len_packet+=bit_num[i];
		if(i>2)
		{
			if(len_packet>m_l_packet)
			{
				error_flag=true;
				error_len=m_l_packet-(len_packet-bit_num[i]);
				break;
			}
		}
		//*****************************

		temp=Get_Data_From_Byte(usertele,len_array,bit_num[i]);
		
		if(i==0) ss.Format("%d 等级转换\n",temp);
		else ss=UserTelegram_Data_Format(temp,bit_num[i]);

		ss1=UserTelegram_Description(41,i);
		ss1+=ss;
		Print_Log(file,ss1);

		//********************************
		if(i==2) m_l_packet=temp;
		//********************************
	};

	//数量
	if(!error_flag)
	{
		n_iter=temp;
		for(i=0;i<(int)n_iter;i++)
		{
			for(j=0;j<iter_var_num;j++)
			{
				//*****************************
				len_packet+=iter_num[j];
				if(len_packet>m_l_packet)
				{
					error_flag=true;
					error_len=m_l_packet-(len_packet-iter_num[j]);
					break;
				}
				//*****************************

				temp=Get_Data_From_Byte(usertele,len_array,iter_num[j]);
			
				ss=UserTelegram_Data_Format(temp,iter_num[j]);
				ss1=UserTelegram_Description(41,j+frame_var_num);
				ss1+=ss;
				Print_Log(file,ss1);
			}
			//********************
			if(error_flag) break;
			//********************
		}
	}

	//读掉错误数据***********************
	if(error_flag)
	{
		temp=Get_Data_From_Byte(usertele,len_array,(int)error_len);
	}

	return (!error_flag);

}

//分析用户信息包：ETCS-68
//file：写入文件指针
//usertele[]：报文信息数组
//len_array：报文信息数组长度
BOOL Analyse_Usertele_Etcs68(CStdioFile* file,byte usertele[],int len_array)
{
	DWORD temp;
	CString ss,ss1;
	int frame_var_num=10;
	int bit_num[10]={8,2,13,2,1,15,15,15,4,5};
	DWORD m_q_trackinit=-1;
	
	DWORD n_iter;
	int iter_var_num=3;
	int iter_num[3]={15,15,4};

	DWORD m_l_packet=-1;
	DWORD len_packet=0;
	bool error_flag=false;
	DWORD error_len=0; 

	int i,j;

	for(i=0;i<frame_var_num;i++)
	{
 		//*****************************
		len_packet+=bit_num[i];
		if(i>2)
		{
			if(len_packet>m_l_packet)
			{
				error_flag=true;
				error_len=m_l_packet-(len_packet-bit_num[i]);
				break;
			}
		}
		//*****************************

        temp=Get_Data_From_Byte(usertele,len_array,bit_num[i]);
		
		if(i==0) ss.Format("%d 特殊区段\n",temp);
		else ss=UserTelegram_Data_Format(temp,bit_num[i]);

		ss1=UserTelegram_Description(68,i);
		ss1+=ss;
		Print_Log(file,ss1);

		//**********************************
		if(i==4)
		{
			m_q_trackinit=temp;
			if(m_q_trackinit==0) i++;
		}
		else if(i==5)
		{
			if(m_q_trackinit==1) i+=3;
		}
		//**********************************

		//********************************
		if(i==2) m_l_packet=temp;
		//********************************

	};

	//数量
	if(!error_flag)
	{
		n_iter=temp;
		for(i=0;(i<((int)n_iter) && (m_q_trackinit==0));i++) //*************
		{
			for(j=0;j<iter_var_num;j++)
			{
				//*****************************
				len_packet+=iter_num[j];
				if(len_packet>m_l_packet)
				{
					error_flag=true;
					error_len=m_l_packet-(len_packet-iter_num[j]);
					break;
				}
				//*****************************

				temp=Get_Data_From_Byte(usertele,len_array,iter_num[j]);
			
				ss=UserTelegram_Data_Format(temp,iter_num[j]);
				ss1=UserTelegram_Description(68,j+frame_var_num);
				ss1+=ss;
				Print_Log(file,ss1);
			}
			//********************
			if(error_flag) break;
			//********************
		}
	}
	//读掉错误数据***********************
	if(error_flag)
	{
		temp=Get_Data_From_Byte(usertele,len_array,(int)error_len);
	}

	return (!error_flag);

}

//分析用户信息包：ETCS-132
//file：写入文件指针
//usertele[]：报文信息数组
//len_array：报文信息数组长度
BOOL Analyse_Usertele_Etcs132(CStdioFile* file,byte usertele[],int len_array)
{
	DWORD temp;
	CString ss,ss1;
	int frame_var_num=4;
	int bit_num[4]={8,2,13,1};

	DWORD m_l_packet=-1;
	DWORD len_packet=0;
	bool error_flag=false;
	DWORD error_len=0; 

	int i;

	for(i=0;i<frame_var_num;i++)
	{
 		//*****************************
		len_packet+=bit_num[i];
		if(i>2)
		{
			if(len_packet>m_l_packet)
			{
				error_flag=true;
				error_len=m_l_packet-(len_packet-bit_num[i]);
				break;
			}
		}
		//*****************************

        temp=Get_Data_From_Byte(usertele,len_array,bit_num[i]);
		
		if(i==0) ss.Format("%d 调车危险\n",temp);
		else ss=UserTelegram_Data_Format(temp,bit_num[i]);

		ss1=UserTelegram_Description(132,i);
		ss1+=ss;
		Print_Log(file,ss1);

		//********************************
		if(i==2) m_l_packet=temp;
		//********************************
	}

	//读掉错误数据***********************
	if(error_flag)
	{
		temp=Get_Data_From_Byte(usertele,len_array,(int)error_len);
	}

	return (!error_flag);

}

//分析用户信息包：ETCS-254
//file：写入文件指针
//usertele[]：报文信息数组
//len_array：报文信息数组长度
BOOL Analyse_Usertele_Etcs254(CStdioFile* file,byte usertele[],int len_array)
{
	DWORD temp;
	CString ss,ss1;
	int frame_var_num=3;
	int bit_num[3]={8,2,13};

	int i;

	for(i=0;i<frame_var_num;i++)
	{
        temp=Get_Data_From_Byte(usertele,len_array,bit_num[i]);
		
		if(i==0) ss.Format("%d Default Balise Info\n",temp);
		else ss=UserTelegram_Data_Format(temp,bit_num[i]);

		ss1=UserTelegram_Description(254,i);
		ss1+=ss;
		Print_Log(file,ss1);
	}

	return TRUE;
}


//分析用户信息包：CTCS-1
//file：写入文件指针
//usertele[]：报文信息数组
//len_array：报文信息数组长度
BOOL Analyse_Usertele_Ctcs1(CStdioFile* file,byte usertele[],int len_array)
{
	DWORD temp;
	CString ss,ss1;
	int frame_var_num=8;
	int bit_num[8]={2,13,2,15,4,5,15,5};
	
	DWORD n_iter;
	int iter_var_num=3;
	int iter_num[3]={4,5,15};

	DWORD m_l_packet=-1;
	DWORD len_packet=0;
	bool error_flag=false;
	DWORD error_len=0; 

	int i,j;

	for(i=0;i<frame_var_num;i++)
	{
		//*****************************
		len_packet+=bit_num[i];
		if(i>1)
		{
			if(len_packet>m_l_packet)
			{
				error_flag=true;
				error_len=m_l_packet-(len_packet-bit_num[i]);
				break;
			}
		}
		//*****************************
        temp=Get_Data_From_Byte(usertele,len_array,bit_num[i]);
		
		ss=UserTelegram_Data_Format(temp,bit_num[i]);

		ss1=UserTelegram_Description(1,i);
		ss1+=ss;
		Print_Log(file,ss1);

		//********************************
		if(i==1) m_l_packet=temp;
		//********************************
	};

	//数量
	if(!error_flag)
	{
		n_iter=temp;
		for(i=0;i<(int)n_iter;i++)
		{
			for(j=0;j<iter_var_num;j++)
			{
				//*****************************
				len_packet+=iter_num[j];
				if(len_packet>m_l_packet)
				{
					error_flag=true;
					error_len=m_l_packet-(len_packet-iter_num[j]);
					break;
				}
				//*****************************
				temp=Get_Data_From_Byte(usertele,len_array,iter_num[j]);
			
				ss=UserTelegram_Data_Format(temp,iter_num[j]);
				ss1=UserTelegram_Description(1,j+frame_var_num);
				ss1+=ss;
				Print_Log(file,ss1);
			}
			//********************
			if(error_flag) break;
			//********************
		}
	}

	//读掉错误数据***********************
	if(error_flag)
	{
		temp=Get_Data_From_Byte(usertele,len_array,(int)error_len);
	}

	return (!error_flag);

}

//分析用户信息包：CTCS-2
//file：写入文件指针
//usertele[]：报文信息数组
//len_array：报文信息数组长度
BOOL Analyse_Usertele_Ctcs2(CStdioFile* file,byte usertele[],int len_array)
{
	DWORD temp;
	CString ss,ss1;
	int frame_var_num=9;
	int bit_num[9]={2,13,2,15,15,15,1,7,5};
	
	DWORD n_iter;
	int iter_var_num=4;
	int iter_num[4]={15,15,1,7};

	DWORD m_l_packet=-1;
	DWORD len_packet=0;
	bool error_flag=false;
	DWORD error_len=0; 

	int i,j;

	for(i=0;i<frame_var_num;i++)
	{
		//*****************************
		len_packet+=bit_num[i];
		if(i>1)
		{
			if(len_packet>m_l_packet)
			{
				error_flag=true;
				error_len=m_l_packet-(len_packet-bit_num[i]);
				break;
			}
		}
		//*****************************
        temp=Get_Data_From_Byte(usertele,len_array,bit_num[i]);
		
		ss=UserTelegram_Data_Format(temp,bit_num[i]);

		ss1=UserTelegram_Description(2,i);
		ss1+=ss;
		Print_Log(file,ss1);
		//********************************
		if(i==1) m_l_packet=temp;
		//********************************
	};

	//数量
	if(!error_flag)
	{
		n_iter=temp;
		for(i=0;i<(int)n_iter;i++)
		{
			for(j=0;j<iter_var_num;j++)
			{
				//*****************************
				len_packet+=iter_num[j];
				if(len_packet>m_l_packet)
				{
					error_flag=true;
					error_len=m_l_packet-(len_packet-iter_num[j]);
					break;
				}
				//*****************************
				temp=Get_Data_From_Byte(usertele,len_array,iter_num[j]);
			
				ss=UserTelegram_Data_Format(temp,iter_num[j]);
				ss1=UserTelegram_Description(2,j+frame_var_num);
				ss1+=ss;
				Print_Log(file,ss1);
			}
			//********************
			if(error_flag) break;
			//********************
		}
	}
	//读掉错误数据***********************
	if(error_flag)
	{
		temp=Get_Data_From_Byte(usertele,len_array,(int)error_len);
	}

	return (!error_flag);
}

//分析用户信息包：CTCS-3
//file：写入文件指针
//usertele[]：报文信息数组
//len_array：报文信息数组长度
BOOL Analyse_Usertele_Ctcs3(CStdioFile* file,byte usertele[],int len_array)
{
	DWORD temp;
	CString ss,ss1;
	int frame_var_num=5;
	int bit_num[5]={2,13,2,15,15};
	
	DWORD m_l_packet=-1;
	DWORD len_packet=0;
	bool error_flag=false;
	DWORD error_len=0; 

	int i;

	for(i=0;i<frame_var_num;i++)
	{
 		//*****************************
		len_packet+=bit_num[i];
		if(i>1)
		{
			if(len_packet>m_l_packet)
			{
				error_flag=true;
				error_len=m_l_packet-(len_packet-bit_num[i]);
				break;
			}
		}
		//*****************************
       temp=Get_Data_From_Byte(usertele,len_array,bit_num[i]);
		
		ss=UserTelegram_Data_Format(temp,bit_num[i]);

		ss1=UserTelegram_Description(3,i);
		ss1+=ss;
		Print_Log(file,ss1);

		//********************************
		if(i==1) m_l_packet=temp;
		//********************************
	};

	//读掉错误数据***********************
	if(error_flag)
	{
		temp=Get_Data_From_Byte(usertele,len_array,(int)error_len);
	}

	return (!error_flag);
}

//分析用户信息包：CTCS-4
//file：写入文件指针
//usertele[]：报文信息数组
//len_array：报文信息数组长度
BOOL Analyse_Usertele_Ctcs4(CStdioFile* file,byte usertele[],int len_array)
{
	DWORD temp;
	CString ss,ss1;
	int frame_var_num=5;
	int bit_num[5]={2,13,2,15,7};
	
	DWORD m_l_packet=-1;
	DWORD len_packet=0;
	bool error_flag=false;
	DWORD error_len=0; 

	int i;

	for(i=0;i<frame_var_num;i++)
	{
  		//*****************************
		len_packet+=bit_num[i];
		if(i>1)
		{
			if(len_packet>m_l_packet)
			{
				error_flag=true;
				error_len=m_l_packet-(len_packet-bit_num[i]);
				break;
			}
		}
		//*****************************
       temp=Get_Data_From_Byte(usertele,len_array,bit_num[i]);
		
		ss=UserTelegram_Data_Format(temp,bit_num[i]);
		
		ss1=UserTelegram_Description(4,i);
		ss1+=ss;
		Print_Log(file,ss1);
	
		//********************************
		if(i==1) m_l_packet=temp;
		//********************************
	}
	//读掉错误数据***********************
	if(error_flag)
	{
		temp=Get_Data_From_Byte(usertele,len_array,(int)error_len);
	}

	return (!error_flag);
}

//分析用户信息包：ETCS-44
//file：写入文件指针
//usertele[]：报文信息数组
//len_array：报文信息数组长度
BOOL Analyse_Usertele_Etcs44(CStdioFile* file,byte usertele[],int len_array)
{
	DWORD temp;
	CString ss,ss1;
	int frame_var_num=4;
	int bit_num[4]={8,2,13,9};
	CString ctcs_ss[5]={"\n"," 轨道区段\n"," 临时限速\n"," 区间反向运行\n"," 大号码岔道\n"};
	
	int i;
	int len;

	BOOL bRet=TRUE;

	DWORD nid_xuser;

	for(i=0;i<frame_var_num;i++)
	{
        temp=Get_Data_From_Byte(usertele,len_array,bit_num[i]);
		
		if(i==0) ss.Format("%d CTCS数据\n",temp);
		else if(i==(frame_var_num-1)) 
		{
			ss=UserTelegram_Data_Format(temp,bit_num[i]);
			if(temp>0 && temp<5)
			{
				len=ss.GetLength();
				ss.Delete(len-1,1);

				ss+=ctcs_ss[temp];
			}
		}
		else ss=UserTelegram_Data_Format(temp,bit_num[i]);

		ss1=UserTelegram_Description(44,i);
		ss1+=ss;
		Print_Log(file,ss1);
	}

	nid_xuser=temp;
	switch(nid_xuser)
	{
	case 1:
		bRet=Analyse_Usertele_Ctcs1(file,usertele,len_array);
		break;
	case 2:
		bRet=Analyse_Usertele_Ctcs2(file,usertele,len_array);
		break;
	case 3:
		bRet=Analyse_Usertele_Ctcs3(file,usertele,len_array);
		break;
	case 4:
		bRet=Analyse_Usertele_Ctcs4(file,usertele,len_array);
		break;
	default:
		bRet=FALSE;
		break;
	}

	return bRet;
}

//分析用户信息包：ETCS-255
//file：写入文件指针
//usertele[]：报文信息数组
//len_array：报文信息数组长度
BOOL Analyse_Usertele_Etcs255(CStdioFile* file)
{
	CString ss;
	byte temp=0xff;
	
	ss.Format("%X: END TELEGRAM\n",temp);
	Print_Log(file,ss);

	return TRUE;
}


BOOL Analyse_Usertele_Info(int read_interface,byte usertele[],int len_usertele,byte tele[],int len_tele)
{
	CStdioFile m_readtele;
	CFileException ex;
	CString m_readtele_name;
	CTime  CurrentTime;
	CString str,temp_str;
	byte m_packet;
	CString file_direct;
	char buf[256];
	BOOL bRet=TRUE,bTempRet=TRUE;
	int i;
	
	GetModuleFileName(NULL,buf,256);
	*(strrchr(buf, '\\') + 1) = '\0';
	file_direct.Format("%s",buf);
	
	CurrentTime=CTime::GetCurrentTime();
	int hour=CurrentTime.GetHour();

	//打开文件
	m_readtele_name=file_direct;
	switch(read_interface)
	{
	case ANALYSE_USERTELE_BALISE_A4:
		m_readtele_name+="A4";
		break;
	case ANALYSE_USERTELE_BALISE_A5:
        m_readtele_name+="A5";
		break;
	case ANALYSE_USERTELE_LEU_C1:
        m_readtele_name+="C1";
		break;
	case ANALYSE_USERTELE_LEU_C2:
        m_readtele_name+="C2";
		break;
	case ANALYSE_USERTELE_LEU_C3:
        m_readtele_name+="C3";
		break;
	case ANALYSE_USERTELE_LEU_C4:
        m_readtele_name+="C4";
		break;
	default:
		return FALSE;
	}

	m_readtele_name+="readerlog_";
	m_readtele_name+=CurrentTime.Format("%Y_%m_%d_%H_%M_%S");
	m_readtele_name+=".txt";

	if(!m_readtele.Open(m_readtele_name,CFile::modeCreate | CFile::modeWrite ,&ex))
		return FALSE;
	
	str="Telegram reader log file\n";
	Print_Log(&m_readtele,str);

	str="telegram read at ";
	str+=CurrentTime.Format("%Y年%m月%d日%H:%M:%S\n\n");
	Print_Log(&m_readtele,str);

	//报文数据
	str=".FILETYPE Balise_Telegrams\n";
	Print_Log(&m_readtele,str);
	
	str="";
	if(len_tele==128)	str=".TGML ";
	else if(len_tele==43) str=".TGMS ";

	for(i=0;i<len_tele;i++)
	{
		temp_str.Format("%02X ",tele[i]);
		str+=temp_str;
		if((i+1)%16==0) str+="\n";
	}
	if(len_tele==43) str+="\n";
	str+="\n";
	Print_Log(&m_readtele,str);

	//用户报文数据
	str=".FILETYPE User_Data\n";
	Print_Log(&m_readtele,str);
	
	str="";
	if(len_usertele==104)	str=".DATAL ";
	else if(len_usertele==27) str=".DATAS ";

	for(i=0;i<len_usertele;i++)
	{
		temp_str.Format("%02X ",usertele[i]);
		str+=temp_str;
		if((i+1)%16==0) str+="\n";
	}
	str+="\n\n";
	Print_Log(&m_readtele,str);

	//解析
	Print_Log(&m_readtele,"Extracting telegrams\n");

	//分析信息帧头
	Analyse_Usertele_Header(&m_readtele,usertele,len_usertele);

	bRet=TRUE;
	do{
		m_packet=usertele[0];
		switch(m_packet)
		{
		case 5://ETCS-5
			bTempRet=Analyse_Usertele_Etcs5(&m_readtele,usertele,len_usertele);
			break;
		case 21://ETCS-21
			bTempRet=Analyse_Usertele_Etcs21(&m_readtele,usertele,len_usertele);
			break;
		case 27://ETCS-27
			bTempRet=Analyse_Usertele_Etcs27(&m_readtele,usertele,len_usertele);
			break;
		case 41://ETCS-41
			bTempRet=Analyse_Usertele_Etcs41(&m_readtele,usertele,len_usertele);
			break;
		case 44://ETCS-44
			bTempRet=Analyse_Usertele_Etcs44(&m_readtele,usertele,len_usertele);
			break;
		case 68://ETCS-68
			bTempRet=Analyse_Usertele_Etcs68(&m_readtele,usertele,len_usertele);
			break;
		case 132://ETCS-132
			bTempRet=Analyse_Usertele_Etcs132(&m_readtele,usertele,len_usertele);
			break;
		case 254://缺省
			bTempRet=Analyse_Usertele_Etcs254(&m_readtele,usertele,len_usertele);
			break;
		case 255://结束
			bTempRet=Analyse_Usertele_Etcs255(&m_readtele);
			break;
		default:
			bTempRet=FALSE;
			break;
		}
		bRet&=bTempRet;
		if(!bRet) break;
	}while(m_packet!=0xff);//结束

	m_readtele.Close();

	return bRet;
}


//对数组每字节进行异或
void Make_Xor(unsigned int *scr_buff,unsigned int *var_buff1,int num)
{
	int i;

	for(i=0;i<num;i++) scr_buff[i]^=var_buff1[i];

}

//分析报文信息帧
//file：写入文件指针
//usertele[]：报文信息数组
//len_array：报文信息数组长度
void Analyse_Usertele_Header(CListBox* list,byte usertele[],int len_array)
{
	DWORD temp;
	CString ss,ss1;
	int frame_var_num=10;
	int bit_num[10]={1,7,1,3,3,2,8,10,14,1};
	int i;
	DWORD nid;

	//信息传送的方向；版本编号；信息传输媒介；本应答器在应答器组中的位置；
	//应答器组中所包含的应答器数量；本应答器信息与前后应答器信息的关系；
	//报文计数器；地区编号；应答器（组）编号;应答器的链接关系
	
	for(i=0;i<frame_var_num;i++)
	{
        temp=Get_Data_From_Byte(usertele,len_array,bit_num[i]);
		
		ss=UserTelegram_Data_Format(temp,bit_num[i]);
		ss.Remove('\n');
		ss1=UserTelegram_Description(0,i);
		ss1+=ss;
		list->AddString(ss1);

		//2007-6-20f add
		if((i==7) || (i==8))
		{
			if(i==7)//NID_C
			{
				nid=temp;
				temp=((nid>>3) & 0x7f);
				ss = UserTelegram_Data_Format(temp,7);
				ss.Remove('\n');
				ss1="    大区编号:";
				ss1+=ss;

				temp=(nid & 0x07);
				ss = UserTelegram_Data_Format(temp,3);
				ss.Remove('\n');
				ss1+="; 分区编号:";
				ss1+=ss;
				list->AddString(ss1);
			}
			else if(i==8) //NID_BG
			{
				nid=temp;
				temp=((nid>>8) & 0x3f);
				ss = UserTelegram_Data_Format(temp,6);
				ss.Remove('\n');
				ss1="    车站编号:";
				ss1+=ss;

				temp=(nid & 0xff);
				ss = UserTelegram_Data_Format(temp,8);
				ss.Remove('\n');
				ss1+="; 应答器编号:";
				ss1+=ss;
				list->AddString(ss1);
			}
		}
		//***********************
	};

}

//对num个数向左移一位
void left_shift_onebit(unsigned int *temp,int num)
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
}

//分析数据得到报文和用户报文
//原始数据在scr_data[]中
//scr_data[0~127]

//最后报文存到des_tele[]数组中，
//des_tele[0]:报文数据长度
//des_tele[1] ~ [128]: 报文数据

//user_p_tele：用户报文数组
//long_or_short:输出的长短用户标识 0 长报文，1 短报文

//返回值：0 正常；非0 ：错误
int Analyse_Tele_Data_Get_TeleAndUsertele(byte* scr_data,byte* des_tele,byte* user_tele,UINT* long_or_short_flag)
{
	unsigned long l_temp;
	unsigned int i,j,len,tel_start,temp,temp1,zm_or_fm,sb_temp,s_temp;
	unsigned int tel_buff[128],user_p_tele[256];
	unsigned int flx[2]={0xdb,0xe0};	//
	unsigned int fsx[2]={0xb5,0x60};	//
	
	unsigned int glx[10]={0xb8,0x87,0x39,0xa7,0xa2,0xed,0x52,0x3b,0xa1,0x30};//last 4 bit no use
	unsigned int gsx[10]={0x9f,0x79,0x0c,0x2f,0xef,0x7c,0xa4,0xa3,0xc4,0xb0};//last 4 bit no use
	
	int tel_error;
	UINT long_or_short;

	len=128;
	des_tele[0]=(BYTE)(len & 0xff); //???????

	//分析报文
	tel_error=0;
	for(i=0;i<128;i++) tel_buff[i]=scr_data[i];
	tel_buff[127]&=~0x01;

	for(i=0;i<1013;i++)
	{
		if((tel_buff[0] & 0x80)!=0) Make_Xor(tel_buff,flx,2);
		left_shift_onebit(tel_buff,128);
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
		for(i=0;i<128;i++) tel_buff[i]=scr_data[i];
		if(tel_start!=0)
		{
			tel_start=1023-tel_start;

			if((tel_buff[127]&0x01)!=0) temp=1;
			else temp=0;
			for(i=0;i<tel_start;i++)
			{
				if((tel_buff[0] & 0x80)!=0) tel_buff[127]|=0x01;
				else tel_buff[127]&=~0x01;
				left_shift_onebit(tel_buff,128);
			}
			if(temp!=0) tel_buff[127]|=0x01;
		}
		for(i=0;i<128;i++) des_tele[i+1]=tel_buff[i];
		tel_buff[127]&=~0x01;

		//检查是否可被g(x)整除
		for(i=0;i<948;i++)
		{
			if((tel_buff[0] & 0x80)!=0) Make_Xor(tel_buff,glx,10);
			left_shift_onebit(tel_buff,128);
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
				user_p_tele[i]=bit11_to_10[temp];
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

		for(i=0;i<128;i++) tel_buff[i]=scr_data[i];
		tel_buff[42]&=~0x07;

		for(i=0;i<331;i++)
		{
			if((tel_buff[0] & 0x80)!=0) Make_Xor(tel_buff,fsx,2);
			left_shift_onebit(tel_buff,43);
		}

		tel_start=tel_buff[0]*256+tel_buff[1];
		tel_start>>=6;

		tel_start=ssfx[tel_start];
		if(tel_start>=341)
		{
			tel_error=0x11;
			return tel_error;
		}

		for(i=0;i<128;i++) tel_buff[i]=scr_data[i];
		if(tel_start!=0)
		{
			tel_start=1023-tel_start;

			if((tel_buff[127]&0x01)!=0) temp=1;
			else temp=0;
			for(i=0;i<tel_start;i++)
			{
				if((tel_buff[0] & 0x80)!=0) tel_buff[127]|=0x01;
				else tel_buff[127]&=~0x01;
				left_shift_onebit(tel_buff,128);
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
			left_shift_onebit(tel_buff,43);
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
				user_p_tele[i]=bit11_to_10[temp];
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
		for(i=0;i<83-long_or_short*62;i++) user_p_tele[i]^=0x3ff;
	}

	//解扰
	l_temp=2801775573;
	l_temp*=sb_temp;
	l_temp&=0xffffffff;
	s_temp=l_temp;


	for(i=0;i<83-long_or_short*62;i++)
	{
		temp=0;
		temp1=user_p_tele[i];
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
		user_p_tele[i]=temp;
	}
	//恢复第一个10位
	temp=0;
	for(i=1;i<83-long_or_short*62;i++) temp+=user_p_tele[i];
	temp=user_p_tele[0]-temp;
	user_p_tele[0]=temp&0x3ff;

	for(i=0;i<83;i++)
	{
		user_p_tele[103-i]=user_p_tele[82-i];
	}
	for(i=0;i<104;i++) user_p_tele[i]&=0x3ff;


	for(i=0;i<104;i++)
	{
		temp=i*8;
		j=temp/10;
		temp=temp%10;
		if(j==82) j=user_p_tele[j+21]<<10;
		else j=(user_p_tele[j+21]<<10)+user_p_tele[j+21+1];
		j>>=(12-temp);
		user_p_tele[i]=(j&0xff);
	}
	if(long_or_short==0) user_p_tele[103]&=0xfc;
	else
	{
		user_p_tele[26]&=0xc0;
		for(i=27;i<104;i++) user_p_tele[i]=0;
	}

	*(long_or_short_flag)=long_or_short;
	for(i=0;i<104;i++) user_tele[i]=user_p_tele[i] & 0xff;
	return 0;
}

//***********************************************************
//启动或关闭usb设备
BOOL Start_ResetUsbDev(BOOL start_stop)
{
	IN LPTSTR HardwareId = new TCHAR[MAX_PATH];

	HDEVINFO hDevInfo = INVALID_HANDLE_VALUE ;
	SP_DEVINFO_DATA spDevInfoData ;
	BOOL ret = true;

	//硬件ID，可以在设备管理器里面找到
	wsprintf(HardwareId, "USB\\Vid_0451&Pid_3410");

	//调用ddk函数

	DWORD i ;
	BOOL Found = false ;

	//访问系统的硬件库
	hDevInfo = SetupDiGetClassDevs(NULL, "USB", NULL,DIGCF_ALLCLASSES | DIGCF_PRESENT);
	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		ret=false;
	}

	if(ret)
	{
		//枚举硬件，获得需要的接口
		spDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
		for (i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &spDevInfoData); i++)
		{
			DWORD DataT ;
			LPTSTR p, buffer = NULL ;
			DWORD buffersize = 0 ;

			//获得硬件的属性值
			while (!SetupDiGetDeviceRegistryProperty(hDevInfo, &spDevInfoData,
					SPDRP_HARDWAREID, &DataT, (PBYTE) buffer, buffersize,
					&buffersize))
			{
				if (GetLastError() == ERROR_INVALID_DATA)
				{
					//不存在HardwareID. Continue.
					break ;
				}
				else if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
				{
					//buffer size不对.
					if (buffer)
						LocalFree(buffer);
					buffer = (char *) LocalAlloc(LPTR, buffersize);
				}
				else
				{
					//未知错误
					ret=false;
					break;
				}
			}

			if(!ret) break;

			if (GetLastError() == ERROR_INVALID_DATA)
				continue ;

			//比较
			for (p = buffer;*p && (p < &buffer[buffersize]);p += lstrlen(p) + sizeof(TCHAR))
			{
				if (!_tcscmp(HardwareId, p))
				{
					//找到
					Found = TRUE ;
					break ;
				}
			}

			if (buffer)		LocalFree(buffer);

			//如果相等
			if (Found)
			{
	/*			SP_PROPCHANGE_PARAMS spPropChangeParams ;

				spPropChangeParams.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
				//改 spPropChangeParams.ClassInstallHeader.InstallFunction 可以对设备进行重启，停止，启动等操作
				spPropChangeParams.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE ;
				spPropChangeParams.Scope = DICS_FLAG_GLOBAL ;

				spPropChangeParams.StateChange = DICS_PROPCHANGE  ;

				//
				if (!SetupDiSetClassInstallParams(hDevInfo, &spDevInfoData,
						(SP_CLASSINSTALL_HEADER *) &spPropChangeParams,
						sizeof(spPropChangeParams)))
				{
					goto cleanup_DeviceInfo;
				}

				if (!SetupDiCallClassInstaller(DIF_PROPERTYCHANGE, hDevInfo,
						&spDevInfoData))
				{
					goto cleanup_DeviceInfo;
				} 
	*/			
				SP_PROPCHANGE_PARAMS spPropChangeParams ;

				spPropChangeParams.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
				//改 spPropChangeParams.ClassInstallHeader.InstallFunction 可以对设备进行重启，停止，启动等操作
				spPropChangeParams.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE ;
				spPropChangeParams.Scope = DICS_FLAG_GLOBAL ;

				//改此参数可
				//DICS_ENABLE：启用设备
				//DICS_DISABLE：停用设备
				if(start_stop) 
					spPropChangeParams.StateChange = DICS_ENABLE ;//启用
				else spPropChangeParams.StateChange = DICS_DISABLE ; //停用

				//
				if (!SetupDiSetClassInstallParams(hDevInfo, &spDevInfoData,
						(SP_CLASSINSTALL_HEADER *) &spPropChangeParams,
						sizeof(spPropChangeParams)))
				{
					ret=false;
					break;
				}

				if (!SetupDiCallClassInstaller(DIF_PROPERTYCHANGE, hDevInfo,
						&spDevInfoData))
				{
					ret=false;
					break;
				} 
			
				//卸载设备
	/*			SP_REMOVEDEVICE_PARAMS spRemoveDeviceParams;

				spRemoveDeviceParams.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
				spRemoveDeviceParams.ClassInstallHeader.InstallFunction = DIF_REMOVE;//DIF_PROPERTYCHANGE ;
				spRemoveDeviceParams.Scope = DICS_FLAG_GLOBAL ;

				//spPropChangeParams.StateChange = DICS_DISABLE  ;

				//
				if (!SetupDiSetClassInstallParams(hDevInfo, &spDevInfoData,
						(SP_CLASSINSTALL_HEADER *) &spRemoveDeviceParams,
						sizeof(spRemoveDeviceParams)))
				{
					goto cleanup_DeviceInfo;
				}

				if (!SetupDiCallClassInstaller(DIF_REMOVE, hDevInfo,
						&spDevInfoData))
				{
					goto cleanup_DeviceInfo;
				} 
	*/			

	/*			//启用非PNP设备
				SP_UNREMOVEDEVICE_PARAMS spUnremovedeviceParams ;

				spUnremovedeviceParams.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
				spUnremovedeviceParams.ClassInstallHeader.InstallFunction = DIF_UNREMOVE ;
				spUnremovedeviceParams.Scope = DI_UNREMOVEDEVICE_CONFIGSPECIFIC ;

				//
				if (!SetupDiSetClassInstallParams(hDevInfo, &spDevInfoData,
						(SP_CLASSINSTALL_HEADER *) &spUnremovedeviceParams,
						sizeof(spUnremovedeviceParams)))
				{
					goto cleanup_DeviceInfo;
				}

				if (!SetupDiCallClassInstaller(DIF_UNREMOVE, hDevInfo,
						&spDevInfoData))
				{
					goto cleanup_DeviceInfo;
				} 
	*/			

				ret = true;
				break ;
			}
		}
	}

	//退出时，清理工作环境
	if (hDevInfo != INVALID_HANDLE_VALUE)
			SetupDiDestroyDeviceInfoList(hDevInfo);

	delete[] HardwareId;

	return (ret & Found);
}

//得到Com口端口号
//此功能Win2000无法使用
BOOL Get_ComDev()
{
	IN LPTSTR HardwareId = new TCHAR[MAX_PATH];

	HDEVINFO hDevInfo = INVALID_HANDLE_VALUE ;
	SP_DEVINFO_DATA spDevInfoData ;
	BOOL ret = true;
	CString ss; 

	//硬件ID，可以在设备管理器里面找到
	wsprintf(HardwareId, "umpport\\VID_0451_341A_com");

	//调用ddk函数

	DWORD i ;
	BOOL Found = false ;

	//访问系统的硬件库
	hDevInfo = SetupDiGetClassDevs(NULL, "umpport", NULL,DIGCF_ALLCLASSES | DIGCF_PRESENT);
	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		ret=false;
	}

	if(ret)
	{
		//枚举硬件，获得需要的接口
		spDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
		for (i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &spDevInfoData); i++)
		{
			DWORD DataT ;
			LPTSTR p, buffer = NULL ;
			DWORD buffersize = 0 ;

			//获得硬件的属性值
			while (!SetupDiGetDeviceRegistryProperty(hDevInfo, &spDevInfoData,
					//SPDRP_HARDWAREID, &DataT, (PBYTE) buffer, buffersize,
					SPDRP_FRIENDLYNAME, &DataT, (PBYTE) buffer, buffersize,
					&buffersize))
			{
				if (GetLastError() == ERROR_INVALID_DATA)
				{
					//不存在HardwareID. Continue.
					break ;
				}
				else if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
				{
					//buffer size不对.
					if (buffer)
						LocalFree(buffer);
					buffer = (char *) LocalAlloc(LPTR, buffersize);
				}
				else
				{
					//未知错误
					ret=false;
					break;
				}
			}

			if(!ret) break;

			if (GetLastError() == ERROR_INVALID_DATA)
				continue ;

			//比较
			for (p = buffer;*p && (p < &buffer[buffersize]);p += lstrlen(p) + sizeof(TCHAR))
			{
				if (!_tcscmp(HardwareId, p))
				{
					//找到
					Found = TRUE ;
					break ;
				}
			}

			CString ss1;
			ss.Format("%s",buffer);
			ss1=ss.Right(7);
			ss1.Remove('(');
			ss1.Remove(')');
			ss1.Remove(' ');
			ComStr=ss1;
			Found=TRUE;

			if (buffer)		LocalFree(buffer);

			//如果相等
			if (Found)
			{
				ret = true;
				break ;
			}
		}
	}

	//退出时，清理工作环境
	if (hDevInfo != INVALID_HANDLE_VALUE)
			SetupDiDestroyDeviceInfoList(hDevInfo);

	delete[] HardwareId;

	return ret & Found;
}

//复制用户报文数据
void Copy_UserTele(byte des[],byte source[],int len_array)
{
	int i;
	
	for(i=0;i<len_array;i++)
		des[i]=source[i];
}