// LeuWriteDefaultTele.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "program_balise.h"
#include "LeuWriteDefaultTele.h"
#include ".\leuwritedefaulttele.h"
#include "Comm_Balise_ts.h"
#include "Wait.h"

// CLeuWriteDefaultTele �Ի���

IMPLEMENT_DYNAMIC(CLeuWriteDefaultTele, CDialog)
CLeuWriteDefaultTele::CLeuWriteDefaultTele(CWnd* pParent /*=NULL*/)
	: CDialog(CLeuWriteDefaultTele::IDD, pParent)
{
}

CLeuWriteDefaultTele::~CLeuWriteDefaultTele()
{
}

void CLeuWriteDefaultTele::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_filename[0]);
	DDX_Control(pDX, IDC_EDIT2, m_filename[1]);
	DDX_Control(pDX, IDC_EDIT3, m_filename[2]);

	DDX_Control(pDX, IDC_EDIT4, m_Compare);
}


BEGIN_MESSAGE_MAP(CLeuWriteDefaultTele, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_WRITE, OnBnClickedButtonWrite)
	ON_BN_CLICKED(IDC_BUTTON_OPENFILE1, OnBnClickedButtonOpenfile1)
	ON_BN_CLICKED(IDC_BUTTON_OPENFILE2, OnBnClickedButtonOpenfile2)
	ON_BN_CLICKED(IDC_BUTTON_OPENFILE3, OnBnClickedButtonOpenfile3)
	ON_BN_CLICKED(IDC_BUTTON_READ_TPC, OnBnClickedButtonReadTpc)
	ON_BN_CLICKED(IDC_BUTTON_READ_TE1, OnBnClickedButtonReadTe1)
	ON_BN_CLICKED(IDC_BUTTON_READ_TSE, OnBnClickedButtonReadTse)
END_MESSAGE_MAP()


// CLeuWriteDefaultTele ��Ϣ�������

void CLeuWriteDefaultTele::OnBnClickedButtonWrite()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int i;
	CString filename[3];
	BOOL readflag=TRUE;

	CWait m_dlgWait;

	m_dlgWait.Create(IDD_DIALOG_WAIT,NULL);  
	m_dlgWait.ShowWindow(SW_SHOW);
	m_dlgWait.UpdateWindow();
	CWaitCursor wait;

	for(i=0;i<3;i++)
	{
		m_send_len[i]=0;
		m_filename[i].GetWindowText(filename[i]);
		if(filename[i]=="")
		{
			MessageBox("�޷���bin�ļ�","���ļ�����",MB_OK);
			return ;
		}
		if(i==0)	
		{
			InitBinFile(filename[i],m_send_tpc,&m_send_len[i]);
			if(m_send_len[0]!=20016) readflag=FALSE;
		}
		else if(i==1)
		{
			InitBinFile(filename[i],m_send_te,&m_send_len[i]);
			if(m_send_len[1]!=65536) readflag=FALSE;
		}
		else
		{
			InitBinFile(filename[i],m_send_tse,&m_send_len[i]);
			if(m_send_len[2]!=64) readflag=FALSE;
		}

		if(!readflag)
		{
			MessageBox("�����ļ���ʽ����ȷ��������ѡ���ļ���","����",MB_OK);
			return ;
		}
	}

	//TPC
	if(!Send_TPC_TE1_TSE(0x01)) return;
	
	//TE1
	if(!Send_TPC_TE1_TSE(0x02)) return;

	//TSE
	if(!Send_TPC_TE1_TSE(0x00)) return;
	Sleep(15000);

	//TSE
	if(!Send_TPC_TE1_TSE(0x00)) return;
	Sleep(15000);

	//TSE
	if(!Send_TPC_TE1_TSE(0x00)) return;
	Sleep(15000);

	MessageBox("OK��д������ȷ��","��ȷ",MB_OK);
	return ;
}

BOOL CLeuWriteDefaultTele::Send_TPC_TE1_TSE(byte type)
{
	//��������
	int len,len_data;
	byte ch[300][500];
	int pos=0;
	byte temp;
	int i,j;
	int send_delay;
	byte tpc_fill[4]={0x5a,0x00,0x00,0x00};

	WORD frame_allnum;
	WORD frame_cur;
	DWORD data_pos;
	DWORD type_alllen;
	WORD repeat_frame;

	//��������
	switch(type)
	{
	case 0x01:
		frame_allnum=(WORD)((m_send_len[0]+12)/224)+1;
		type_alllen=m_send_len[0];
		break;
	case 0x02:
		frame_allnum=(WORD)((m_send_len[1]+12)/224)+1;
		type_alllen=m_send_len[1];
		break;
	case 0x00:
		frame_allnum=(WORD)((m_send_len[2]+12)/224)+1;
		type_alllen=m_send_len[2];
		break;
	default:
		return FALSE;
	}
	frame_cur=1;
	data_pos=0;

	do{
		//��������
		pos=0;
		ch[frame_cur][pos++]=0x25;
		ch[frame_cur][pos++]=0x00;//
		ch[frame_cur][pos++]=0x00;//

		ch[frame_cur][pos++]=type;//
		ch[frame_cur][pos++]=0x00;
		ch[frame_cur][pos++]=(byte)(frame_cur & 0xff);//��ǰ֡��
		ch[frame_cur][pos++]=(byte)((frame_cur>>8) & 0xff);
		ch[frame_cur][pos++]=(byte)(frame_allnum & 0xff);//��֡��
		ch[frame_cur][pos++]=(byte)((frame_allnum>>8) & 0xff);

		if(frame_cur==1)
		{
			for(i=0;i<12;i++)
			{
				ch[frame_cur][pos++]=0x00;
			}
		}

		do{
			if(data_pos<type_alllen)
			{
				switch(type)
				{
				case 0x01:
					ch[frame_cur][pos++]=m_send_tpc[data_pos];
					break;
				case 0x02:
					ch[frame_cur][pos++]=m_send_te[data_pos];
					break;
				case 0x00:
					ch[frame_cur][pos++]=m_send_tse[data_pos];
					break;
				}
				data_pos++;
			}
			else
			{
				break;
			}
		}while(pos<(230+3));

		if(pos<(230+3))
		{
			j=0;
			do{
				if(type==0x01)
				{
					ch[frame_cur][pos++]=tpc_fill[j++]; //�� 0x5a 0x00 0x00 0x00
					if(j==4) j=0;
				}
				else ch[frame_cur][pos++]=0x00;//��0

			}while(pos<(230+3));
		}

		//����
		len_data=pos-3;
		ch[frame_cur][1]=(byte)(len_data & 0xff);
		len_data>>=8;
		ch[frame_cur][2]=(byte)(len_data & 0xff);

		
		len=pos;
		SendCommand(len,&ch[frame_cur][0]);

		//��������
		send_delay=0;
		do{
			Read_Tele_Len=0;
			ReceiveData(READLEN_LEU_WRITE_DEFAULTTELE);

			if(ReadBufLen>0)
			{
				if(ArrangeData(ORDER_LEU_WRITE_DEFAULTTELE))
				{
					//
					temp=Read_Tele[0];
					if(temp==0x02 )
					{
						MessageBox("����ʧ�ܣ�����ͨ��У�����","����",MB_OK);
						return FALSE;
					}
					else if(temp==0x12)
					{
						MessageBox("����ʧ�ܣ������źŲ�������","����",MB_OK);
						return FALSE;
					}
					else if(temp==0x30)
						break;
					else if((temp==0x11) && (((Read_Tele[3] + Read_Tele[4]*256)!=frame_cur) || ((Read_Tele[5] + Read_Tele[6]*256)!=frame_allnum)))
					{
						MessageBox("�������ݴ���","����",MB_OK);
						return FALSE;
					}
					else if(temp==0x13 || temp==0x11)
						break;
				}
			}

			if((type==0x01 || type==0x02) && (frame_cur==1 || frame_cur==frame_allnum))   Sleep(1000);
			else if((type==0x00) && (frame_cur==frame_allnum))   Sleep(1000);

			send_delay++;
		}while(send_delay<40);
		
		if(send_delay>=40)
		{
			MessageBox("����ʧ�ܣ�ͨ��δ������","����",MB_OK);
			return FALSE;
		}

		if(temp==0x30) break;

		frame_cur++;
	}while(frame_cur<=frame_allnum);

	//�ش�
	frame_cur=0;
	while(Read_Tele[0]==0x30)
	{
		repeat_frame=(Read_Tele[1] + Read_Tele[2]*256);

		//len
		SendCommand(len,&ch[repeat_frame][0]);

		frame_cur++;

		//��������
		send_delay=0;
		do{
			Read_Tele_Len=0;
			ReceiveData(READLEN_LEU_WRITE_DEFAULTTELE);

			if(ReadBufLen>0)
			{
				if(ArrangeData(ORDER_LEU_WRITE_DEFAULTTELE))
				{
					//
					temp=Read_Tele[0];
					if(temp==0x02 )
					{
						MessageBox("����ʧ�ܣ�����ͨ��У�����","����",MB_OK);
						return FALSE;
					}
					else if(temp==0x12 )
					{
						MessageBox("����ʧ�ܣ������źŲ�������","����",MB_OK);
						return FALSE;
					}
					else if(temp==0x30)
						break;
					else if(temp==0x13 || temp==0x11)
						break;
				}
			}
			
			Sleep(1000);
			send_delay++;
		}while(send_delay<40);
		
		if(send_delay>=40)
		{
			MessageBox("����ʧ�ܣ�ͨ��δ������","����",MB_OK);
			return FALSE;
		}

		if(temp==0x13) break;
	};

	return TRUE;
}


void CLeuWriteDefaultTele::OnBnClickedButtonOpenfile1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	CFileFind	filefind;
	CString filename;
	CString filepath;

	CFileDialog filedlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						"bin�ļ� (*.bin)|*.bin|",this);

	if(filedlg.DoModal()!=IDOK) return;

	filepath=filedlg.GetPathName();
	filename=filedlg.GetFileName();

	m_filename[0].SetWindowText(filepath);
}

void CLeuWriteDefaultTele::OnBnClickedButtonOpenfile2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	CFileFind	filefind;
	CString filename;
	CString filepath;

	CFileDialog filedlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						"bin�ļ� (*.bin)|*.bin|",this);

	if(filedlg.DoModal()!=IDOK) return;

	filepath=filedlg.GetPathName();
	filename=filedlg.GetFileName();

	m_filename[1].SetWindowText(filepath);
}

void CLeuWriteDefaultTele::OnBnClickedButtonOpenfile3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	CFileFind	filefind;
	CString filename;
	CString filepath;

	CFileDialog filedlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						"bin�ļ� (*.bin)|*.bin|",this);

	if(filedlg.DoModal()!=IDOK) return;

	filepath=filedlg.GetPathName();
	filename=filedlg.GetFileName();

	m_filename[2].SetWindowText(filepath);
}

void CLeuWriteDefaultTele::InitBinFile(CString ff,byte send[],DWORD* datalen)
{
	CStdioFile m_tele;
	CFileException ex1;
	byte pbuf[1024];
	DWORD len;
	int i=0;
	int num;

	if(!m_tele.Open(ff,CFile::modeRead | CFile::typeBinary,&ex1))
		return ;

	len=0;
	do{
		num=m_tele.Read(pbuf,1024);
		if( num>0)
		{
			for(i=0;i<num;i++)
			{
				send[len]=pbuf[i];
				len++;
			}
		}
	}while(num>0);

	*datalen=len;

	m_tele.Close();
}	

void CLeuWriteDefaultTele::OnBnClickedButtonReadTpc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString filename;
	BOOL readflag=TRUE;

	m_Compare.SetWindowText("");

	m_send_len[0]=0;
	m_filename[0].GetWindowText(filename);
	if(filename=="")
	{
		MessageBox("�޷���bin�ļ�","���ļ�����",MB_OK);
		return ;
	}

	InitBinFile(filename,m_send_tpc,&m_send_len[0]);
	if(m_send_len[0]!=20016) readflag=FALSE;

	if(!readflag)
	{
		MessageBox("�����ļ���ʽ����ȷ��������ѡ���ļ���","����",MB_OK);
		return ;
	}
	
	if(!Read_TPC_TE_TSE(ORDER_LEU_TPC)) return;

	//�Ƚ�
	if(!Compare(0x01)) m_Compare.SetWindowText("TPC ���ݲ�һ��!");
	else m_Compare.SetWindowText("TPC ����һ�¡�");


}

BOOL CLeuWriteDefaultTele::Read_TPC_TE_TSE(byte type)
{
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
	
	byte order;
	int readlen;
	order=ORDER_LEU_READ;
	switch(type)
	{
	case 0x01:
		readlen=READLEN_LEU_READ_TPC;
		break;
	case 0x02:
		readlen=READLEN_LEU_READ_TE1;
		break;
	case 0x00:
		readlen=READLEN_LEU_READ_TSE;
		break;
	}
	//����������
	pos=0;
	ch[pos++]=order;
	ch[pos++]=0x00;
	ch[pos++]=0x00;
	ch[pos++]=type;

	//����
	len_data=pos-3;
	ch[1]=(byte)(len_data & 0xff);
	len_data>>=8;
	ch[2]=(byte)(len_data & 0xff);

	len=pos;
	SendCommand(len,&ch[0]);

	//��������
	i=0;
	Read_Tele_Len=0;
	Read_Tele_End_Flag=FALSE;
	do{
		Sleep(100);
		ReceiveData(readlen);

		if(ReadBufLen>0)
		{
			if(!ArrangeData(order)) 
				break;
		}
		else
		{
			i++;
		}
	}while(!Read_Tele_End_Flag && (i<30));

	if(Read_Tele_Len>0)
	{
		//************************************************
		//���ı�ʶ
		temp=Read_Tele[0];
		switch(temp)
		{
		case 0x02:
			MessageBox("������ʧ�ܣ�����ͨ��У�����","����",MB_OK);
			return FALSE;
			break;
		case 0x20:
			MessageBox("������ʧ��:δ��ȡ������!","����",MB_OK);
			return FALSE;
			break;
		}
		
		//***********************************************
		//��������
		if(temp==0x21)
		{
			Anaylse_Data(Read_Tele,Read_Tele_Len,type);
		}
		else
		{
			MessageBox("�������ݸ�ʽ����","����",MB_OK);
			return FALSE;
		}
	}
	else
	{
		MessageBox("������ʧ�ܣ�ͨ��δ������","����",MB_OK);
		return FALSE;
	}

	return TRUE;

}

void CLeuWriteDefaultTele::OnBnClickedButtonReadTe1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString filename;
	BOOL readflag=TRUE;

	m_Compare.SetWindowText("");

	m_send_len[1]=0;
	m_filename[1].GetWindowText(filename);
	if(filename=="")
	{
		MessageBox("�޷���bin�ļ�","���ļ�����",MB_OK);
		return ;
	}

	InitBinFile(filename,m_send_te,&m_send_len[1]);
	if(m_send_len[1]!=65536) readflag=FALSE;

	if(!readflag)
	{
		MessageBox("�����ļ���ʽ����ȷ��������ѡ���ļ���","����",MB_OK);
		return ;
	}
	
	if(!Read_TPC_TE_TSE(ORDER_LEU_TE1)) return;

	//�Ƚ�
	if(!Compare(0x02)) m_Compare.SetWindowText("TE1 ���ݲ�һ��!");
	else m_Compare.SetWindowText("TE1 ����һ�¡�");

}

void CLeuWriteDefaultTele::OnBnClickedButtonReadTse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString filename;
	BOOL readflag=TRUE;

	m_Compare.SetWindowText("");

	m_send_len[2]=0;
	m_filename[2].GetWindowText(filename);
	if(filename=="")
	{
		MessageBox("�޷���bin�ļ�","���ļ�����",MB_OK);
		return ;
	}

	InitBinFile(filename,m_send_tse,&m_send_len[2]);
	if(m_send_len[2]!=64) readflag=FALSE;

	if(!readflag)
	{
		MessageBox("�����ļ���ʽ����ȷ��������ѡ���ļ���","����",MB_OK);
		return ;
	}
	
	if(!Read_TPC_TE_TSE(ORDER_LEU_TSE)) return;

	//�Ƚ�
	if(!Compare(0x00)) m_Compare.SetWindowText("TSE ���ݲ�һ��!");
	else m_Compare.SetWindowText("TSE ����һ�¡�");
}


BOOL CLeuWriteDefaultTele::Anaylse_Data(byte data[],DWORD len,int type)
{
	int target_len=0;
	int target_cur=0;
	int source_cur=0;
	int i;
	int num_per_frame;

	target_cur=0;
	source_cur=0;
	do{
		source_cur+=1;
		if((data[source_cur+2] + data[source_cur+3]*256) ==0x01)
		{
			source_cur+=(6+12);
			num_per_frame=224-12;
		}
		else
		{
			source_cur+=6;
			num_per_frame=224;
		}

		for(i=0;i<num_per_frame;i++)
		{
			m_read[target_cur++]=data[source_cur++];
		}
	}while(source_cur<(int)len);

	target_len=target_cur;

	//����д���ļ�
	CStdioFile m_readtele;
	CFileException ex;
	CString m_readtele_name;
	CTime  CurrentTime;
	CString str;
	CString file_direct;
	char buf[256];
	BOOL bRet=TRUE,bTempRet=TRUE;
	
	int j;

	GetModuleFileName(NULL,buf,256);
	*(strrchr(buf, '\\') + 1) = '\0';
	file_direct.Format("%s",buf);
	
	CurrentTime=CTime::GetCurrentTime();
	int hour=CurrentTime.GetHour();

	//���ļ�
	m_readtele_name=file_direct;
	switch(type)
	{
	case 0x01:
        m_readtele_name+="TPCreaderlog_";
		break;
	case 0x02:
		m_readtele_name+="TE1readerlog_";
		break;
	case 0x00:
		m_readtele_name+="TSEreaderlog_";
		break;
	}
	m_readtele_name+=CurrentTime.Format("%Y_%m_%d_%H_%M_%S");
	m_readtele_name+=".txt";

	if(!m_readtele.Open(m_readtele_name,CFile::modeCreate | CFile::modeWrite ,&ex))
		return FALSE;
	
	switch(type)
	{
	case 0x01:
		str="Common Parameter Table Data of Buffer Transfer Messages reader log file\n";
		break;
	case 0x02:
		str="Eurotelegram Table 1 Data of Buffer Transfer Messages reader log file\n";
		break;
	case 0x00:
		str="Selection Table Data of Buffer Transfer Messages reader log file\n";
		break;
	}
	Print_Log(&m_readtele,str);

	str="-------------------------\n";
	Print_Log(&m_readtele,str);

	str="Status Output File\n";
	Print_Log(&m_readtele,str);

	str="-------------------------\n";
	Print_Log(&m_readtele,str);

	switch(type)
	{
	case 0x01:
        str="Uploading of : -> TPC ";
		break;
	case 0x02:
		str="Uploading of : -> TE1 ";
		break;
	case 0x00:
		str="Uploading of : -> TSE ";
		break;
	}
	str+=CurrentTime.Format("%Y��%m��%d��%H:%M:%S\n\n");
	Print_Log(&m_readtele,str);

	CString str_temp;
	

	//����ϴ�����
	int output_len;
	switch(type)
	{
	case 0x01:
		output_len=20016;
		break;
	case 0x02:
		output_len=65536;
		break;
	case 0x00:
		output_len=64;
		break;
	}
	if(target_len<output_len)  output_len=target_len;

	//���ݳ���
	str.Format("���ݳ���%i\n",output_len);
	Print_Log(&m_readtele,str);

    str="";
	j=0;
	for(i=0;i<output_len;i++)
	{
		str_temp.Format("%02X ",m_read[i]);
		str+=str_temp;
		j++;
		if(j==16)
		{
			str+="\n";
			j=0;
		}
	}
	str+="\n\n\n";
	Print_Log(&m_readtele,str);

	m_readtele.Close();

	return TRUE;

}

BOOL CLeuWriteDefaultTele::Compare(byte type)
{
	DWORD len;

	if(type==0x01) len=20016;
	else if(type==0x02) len=65536;
	else if(type==0x00) len=64;
	else return FALSE;

	DWORD i;

	for(i=0;i<len;i++)
	{
		switch(type)
		{
		case 0x01:
			if(m_send_tpc[i]!=m_read[i]) 
				return FALSE;
			break;
		case 0x02:
			if(m_send_te[i]!=m_read[i]) 
				return FALSE;
			break;
		case 0x00:
			if(m_send_tse[i]!=m_read[i]) 
				return FALSE;
			break;
		}
	}

	return TRUE;
}
