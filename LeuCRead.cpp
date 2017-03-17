// LeuCRead.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "program_balise.h"
#include "LeuCRead.h"
#include ".\leucread.h"
#include "Comm_Balise_ts.h"

// CLeuCRead �Ի���

IMPLEMENT_DYNAMIC(CLeuCRead, CDialog)
CLeuCRead::CLeuCRead(CWnd* pParent /*=NULL*/)
	: CDialog(CLeuCRead::IDD, pParent)
{
}

CLeuCRead::~CLeuCRead()
{
}

void CLeuCRead::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_C_1, m_Select_1);
	DDX_Control(pDX, IDC_RADIO_C_2, m_Select_2);
	DDX_Control(pDX, IDC_RADIO_C_3, m_Select_3);
	DDX_Control(pDX, IDC_RADIO_C_4, m_Select_4);
	DDX_Control(pDX, IDC_LIST1, m_telegram);
	DDX_Control(pDX, IDC_LIST2, m_usertelegram);
	DDX_Control(pDX, IDC_LIST4, m_header);
	DDX_Control(pDX, IDC_EDIT14,m_result);
	DDX_Control(pDX, IDC_EDIT13,m_telegramfile);

	DDX_Control(pDX, IDC_EDIT2, m_C4_Flag);
	DDX_Control(pDX, IDC_EDIT1, m_C6_Energy);
}


BEGIN_MESSAGE_MAP(CLeuCRead, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_OPENFILE, OnBnClickedButtonOpenfile)
	ON_BN_CLICKED(IDC_BUTTON_C4_READ, OnBnClickedButtonC4Read)
END_MESSAGE_MAP()


// CLeuCRead ��Ϣ�������

void CLeuCRead::OnBnClickedButtonOpenfile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	CFileFind	filefind;
	CString filename;
	CString filepath;

	CFileDialog filedlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						"�����ļ� (*.tgm)|*.tgm|",this);

	if(filedlg.DoModal()!=IDOK) return;

	filepath=filedlg.GetPathName();
	filename=filedlg.GetFileName();

	m_telegramfile.SetWindowText(filepath);

}

BOOL CLeuCRead::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_Select_1.SetCheck(BST_CHECKED);
	m_Select_2.SetCheck(BST_UNCHECKED);
	m_Select_3.SetCheck(BST_UNCHECKED);
	m_Select_4.SetCheck(BST_UNCHECKED);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CLeuCRead::OnBnClickedButtonC4Read()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int len,len1,len_data;
	byte ch[1024];
	int pos=0;
	int i;
	CString ss,ss1;
	byte temp,temp1;
	CString filename;
	BOOL m_same;
	int check;
	UINT tele_flag_long_short;
	float energy=0;

	Clear_Display();

	m_telegramfile.GetWindowText(filename);
	if(filename!="")
	{
		if(!InitTelegram(filename,&c_send[0]))
		{
			MessageBox("�����ļ���ʽ����ȷ��������ѡ���ļ���","����",MB_OK);
			return;
		}
	}

	check=Get_Check();
	if(check==-1)
	{
		MessageBox("��ѡ��Ҫ��ȡ�� C �ӿںš�","����",MB_OK);
        return ;
	}

	//����������
	pos=0;
	ch[pos++]=0x50;
	ch[pos++]=0x00;
	ch[pos++]=0x00;
	ch[pos++]=(byte)check;

	len_data=pos-3;
	ch[1]=(byte)(len_data & 0xff);
	len_data>>=8;
	ch[2]=(byte)(len_data & 0xff);

	len=pos;
	SendCommand(len,&ch[0]);

	//��������
	Read_Tele_Len=0;
	Sleep(500);
	ReceiveData(READLEN_LEU_C_READ_TELE);

	if(ReadBufLen>0)
	{
		if(ArrangeData(ORDER_LEU_C_READ_TELE))
		{
			//************************************************
			//���ı�ʶ
			temp=Read_Tele[0];
			switch(temp)
			{
			case 0x02:
				MessageBox("������ʧ�ܣ�����ͨ��У�����","����",MB_OK);
				return;
				break;
			case 0x20:
				MessageBox("������ʧ��:δ��ȡ������!","����",MB_OK);
				//return;
				break;
			case 0x21:
				ss="C1�����̱��ġ�";
				m_telegram.AddString(ss);
				break;
			case 0x22:
				ss="C1���������ġ�";
				m_telegram.AddString(ss);
				break;
			}
			

			ss="";
			len=0;
			if(temp==0x21) len=43;
			else if(temp==0x22) len=128;
			c_read[0]=len; //???????
			
			//��ʾ����
			for(i=0;i<len;i++)
			{
				c_read[i+1]=Read_Tele[i+4];
				ss1.Format("%02x  ",Read_Tele[i+4]);
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
			//��������
			if(temp!=0x20)
			{
				if(Analyse_Tele_Data_Get_TeleAndUsertele(&Read_Tele[4],c_read,usertele,&tele_flag_long_short)!=0)
				{
					MessageBox("�������Ĵ���","����",MB_OK);
					//return ;
				}
			}

			//�û����ı�ʶ
			ss="";
			if(temp==0x21) ss="C1�������û����ġ�";
			else if(temp==0x22) ss="C1�������û����ġ�";
			m_usertelegram.AddString(ss);

			ss="";
			len1=0;
			if(temp==0x22) len1=104;
			else if(temp==0x21) len1=27;
			
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
			if(temp!=0x20)
			{
				Copy_UserTele(temp_usertele,usertele,len1);
				Analyse_Usertele_Header(&(this->m_header),temp_usertele,len1);

				//**********************************************************
				Copy_UserTele(temp_tele,&c_read[1],c_read[0]);
				Copy_UserTele(temp_usertele,usertele,len1);
				if(!Analyse_Usertele_Info(check+ANALYSE_USERTELE_LEU_C1,temp_usertele,len1,temp_tele,c_read[0]))
					MessageBox("�����û����Ĵ���","����",MB_OK);
			}
			//******************************************************

			//************************************************
			//�Ƚ�
			if(filename!="" && (temp==0x21 || temp==0x22))
			{
				if(temp==0x21) 
				{
					c_send[43]&=0xf8;
				}

				len=c_read[0];
				m_same=TRUE;
				for(i=0;i<len;i++)
				{
					if(c_send[i+1]!=c_read[i+1])
					{
						m_same=FALSE;
						break;
					}
				}
				if(m_same) m_result.SetWindowText("һ��");
				else m_result.SetWindowText("��һ��");

				//�ж�C4����
				temp1=Read_Tele[1];
				if(temp1==1) m_C4_Flag.SetWindowText("LEU��C4�ӿ�");
				else m_C4_Flag.SetWindowText("LEUû��C4�ӿ�");

			}
			//��ʾC6�ӿ�����
			if(temp==0x20 || temp==0x21 || temp==0x22)
			{
				energy=(float)((Read_Tele[3]<<8) + Read_Tele[2])*0.0089225f;
				ss.Format("%0.2f",energy);
				m_C6_Energy.SetWindowText(ss);
			}
		}
		//*************************************************
	}
	else 
	{
		MessageBox("������ʧ�ܣ�ͨ��δ������","����",MB_OK);
	}

	return;
}

void CLeuCRead::Clear_Display()
{
	m_telegram.ResetContent();
	m_usertelegram.ResetContent();
	m_header.ResetContent();

	m_result.SetWindowText("");
	m_C4_Flag.SetWindowText("");
	m_C6_Energy.SetWindowText("");

	for(int i=0;i<129;i++)
	{
		c_send[i]=0x00;
		c_read[i]=0x00;
	}

}

int CLeuCRead::Get_Check()
{
	int check=-1;

	if(m_Select_1.GetCheck()==BST_CHECKED) check=0;
	else if(m_Select_2.GetCheck()==BST_CHECKED) check=1;
	else if(m_Select_3.GetCheck()==BST_CHECKED) check=2;
	else if(m_Select_4.GetCheck()==BST_CHECKED) check=3;

	return check;
}

