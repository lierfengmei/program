// LeuWriteDefaultTele_Bx.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "program_balise.h"
#include "LeuWriteDefaultTele_Bx.h"
#include ".\leuwritedefaulttele_bx.h"
#include "Comm_Balise_ts.h"
#include "Wait.h"

// CLeuWriteDefaultTele_Bx �Ի���

IMPLEMENT_DYNAMIC(CLeuWriteDefaultTele_Bx, CDialog)
CLeuWriteDefaultTele_Bx::CLeuWriteDefaultTele_Bx(CWnd* pParent /*=NULL*/)
	: CDialog(CLeuWriteDefaultTele_Bx::IDD, pParent)
{
}

CLeuWriteDefaultTele_Bx::~CLeuWriteDefaultTele_Bx()
{
}

void CLeuWriteDefaultTele_Bx::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_filename[0]);
	DDX_Control(pDX, IDC_EDIT2, m_filename[1]);
	DDX_Control(pDX, IDC_EDIT3, m_filename[2]);
	DDX_Control(pDX, IDC_EDIT4, m_filename[3]);
	DDX_Control(pDX, IDC_EDIT_INDEX1, m_index[0]);
	DDX_Control(pDX, IDC_EDIT_INDEX2, m_index[1]);
	DDX_Control(pDX, IDC_EDIT_INDEX3, m_index[2]);
	DDX_Control(pDX, IDC_EDIT_INDEX4, m_index[3]);
}


BEGIN_MESSAGE_MAP(CLeuWriteDefaultTele_Bx, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_OPENFILE1, OnBnClickedButtonOpenfile1)
	ON_BN_CLICKED(IDC_BUTTON_OPENFILE2, OnBnClickedButtonOpenfile2)
	ON_BN_CLICKED(IDC_BUTTON_OPENFILE3, OnBnClickedButtonOpenfile3)
	ON_BN_CLICKED(IDC_BUTTON_OPENFILE4, OnBnClickedButtonOpenfile4)
	ON_BN_CLICKED(IDC_BUTTON_WRITE, OnBnClickedButtonWrite)
END_MESSAGE_MAP()


// CLeuWriteDefaultTele_Bx ��Ϣ�������

void CLeuWriteDefaultTele_Bx::OnBnClickedButtonOpenfile1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	CFileFind	filefind;
	CString filename;
	CString filepath;

	CFileDialog filedlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						"tgm�ļ� (*.tgm)|*.tgm|",this);

	if(filedlg.DoModal()!=IDOK) return;

	filepath=filedlg.GetPathName();
	filename=filedlg.GetFileName();

	m_filename[0].SetWindowText(filepath);
}

void CLeuWriteDefaultTele_Bx::OnBnClickedButtonOpenfile2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	CFileFind	filefind;
	CString filename;
	CString filepath;

	CFileDialog filedlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						"tgm�ļ� (*.tgm)|*.tgm|",this);

	if(filedlg.DoModal()!=IDOK) return;

	filepath=filedlg.GetPathName();
	filename=filedlg.GetFileName();

	m_filename[1].SetWindowText(filepath);
}

void CLeuWriteDefaultTele_Bx::OnBnClickedButtonOpenfile3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	CFileFind	filefind;
	CString filename;
	CString filepath;

	CFileDialog filedlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						"tgm�ļ� (*.tgm)|*.tgm|",this);

	if(filedlg.DoModal()!=IDOK) return;

	filepath=filedlg.GetPathName();
	filename=filedlg.GetFileName();

	m_filename[2].SetWindowText(filepath);
}

void CLeuWriteDefaultTele_Bx::OnBnClickedButtonOpenfile4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	CFileFind	filefind;
	CString filename;
	CString filepath;

	CFileDialog filedlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						"tgm�ļ� (*.tgm)|*.tgm|",this);

	if(filedlg.DoModal()!=IDOK) return;

	filepath=filedlg.GetPathName();
	filename=filedlg.GetFileName();

	m_filename[3].SetWindowText(filepath);
}

void CLeuWriteDefaultTele_Bx::OnBnClickedButtonWrite()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int i;
	CString filename[4];
	BOOL readflag=TRUE;

	CWait m_dlgWait;

	m_dlgWait.Create(IDD_DIALOG_WAIT,NULL);  
	m_dlgWait.ShowWindow(SW_SHOW);
	m_dlgWait.UpdateWindow();
	CWaitCursor wait;

	for(i=0;i<4;i++)
	{
		m_filename[i].GetWindowText(filename[i]);
		if(filename[i]=="")
		{
			MessageBox("�޷���tgm�����ļ�","���ļ�����",MB_OK);
			return ;
		}
		if(!InitTelegram(filename[i],&m_send[i][0]))
		{
			MessageBox("�޷��򿪱����ļ�,\n��ѡ�����ļ���","����",MB_OK);
			return;
		}
		if(m_send[i][0]!=128) readflag=FALSE;

		if(!readflag)
		{
			MessageBox("�����ļ���ʽ����ȷ��������ѡ���ļ���","����",MB_OK);
			return ;
		}
	}

	if(!Arrange_tele())
	{
		MessageBox("������������ȷ�����������룡","����",MB_OK);
		return ;
	}

	//
	if(!Send_Tele()) return;
	Sleep(15000);


	MessageBox("OK��д������ȷ��","��ȷ",MB_OK);
	return ;

}

BOOL CLeuWriteDefaultTele_Bx::Arrange_tele()
{
	int i,j;
	CString str[4];
	DWORD index[4];
	DWORD crc;
	byte temp;

/*	for(i=0;i<4;i++)
	{
		m_index[i].GetWindowText(str[i]);
		str[i].MakeUpper();

		strtodata(str[i],&index[i],1,0);
		temp=(byte)(index[i]>>16);
		if(temp>0x3f) return FALSE;
	}*/
	
	int pos=0;

	for(i=0;i<4;i++)
	{
	//	m_teledata[pos++]=(byte)((index[i] >>16) & 0xff);
	//	m_teledata[pos++]=(byte)((index[i]>>8) & 0xff);
	//	m_teledata[pos++]=(byte)(index[i] & 0xff);

		for(j=0;j<m_send[i][0];j++)
			m_teledata[pos++]=m_send[i][j+1];
	}

	crc=cal_crc32(m_teledata,pos);
	m_teledata[pos++]=(byte)((crc & 0xff000000) >>24);
	m_teledata[pos++]=(byte)((crc & 0x00ff0000) >>16);
	m_teledata[pos++]=(byte)((crc & 0x0000ff00) >>8);
	m_teledata[pos++]=(byte)(crc & 0x000000ff);

	m_send_len=pos;

	return TRUE;
}

BOOL CLeuWriteDefaultTele_Bx::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	for(int i=0;i<4;i++)
	{
		m_index[i].SetLimitText(6);
		m_index[i].SetWindowText("");
		m_filename[i].SetWindowText("");
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

BOOL CLeuWriteDefaultTele_Bx::Send_Tele()
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
	frame_allnum=(WORD)((m_send_len+12)/224)+1;
	type_alllen=m_send_len;
	frame_cur=1;
	data_pos=0;

	do{
		//��������
		pos=0;
		ch[frame_cur][pos++]=0x25;
		ch[frame_cur][pos++]=0x00;//
		ch[frame_cur][pos++]=0x00;//

		ch[frame_cur][pos++]=0x0c;//type
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
				ch[frame_cur][pos++]=m_teledata[data_pos];
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
				ch[frame_cur][pos++]=tpc_fill[j++]; //�� 0x5a 0x00 0x00 0x00
				if(j==4) j=0;
				
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

			if((frame_cur==1 || frame_cur==frame_allnum))   Sleep(1000);
			
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
