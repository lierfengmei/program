// A5_Read.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "program_balise.h"
#include "A5_Read.h"
#include "Comm_Balise_ts.h"
#include "afxwin.h"

// CA5_Read �Ի���

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


// CA5_Read ��Ϣ�������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

	//A4����������
    ch[pos++]=0x12;
	ch[pos++]=0x00;
	ch[pos++]=0x00;

	len=pos;
	SendCommand(len,&ch[0]);

	//��������
	Sleep(500);
	ReceiveData();

	if(ReadBufLen>0)
	{
		if(ArrangeData())
		{
			//���ı�ʶ
			temp=Read_Tele[0];
			switch(temp)
			{
			case 0x20:
				ss="A5�����ޱ���";
				break;
			case 0x21:
				ss="A5�����б���";
				break;
			}
			m_telegram.AddString(ss);

			if(temp==0x21)
			{
				//��������
				Analyse_Tele_Data();

				ss="";
				//len=128;
				//a5_read[0]=128; //???????
			
				//��ʾ����
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

		//�Ƚ�
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
			if(m_same) m_result.SetWindowText("һ��");
			else m_result.SetWindowText("��һ��");
		}
	}
	else
	{
		ss="A5������ͨ�����ݡ�";
		m_telegram.AddString(ss);
	}
}


void CA5_Read::OnBnClickedButton_OpenFile()
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

void CA5_Read::Analyse_Tele_Data()
{
	int len;

	len=128;
	a5_read[0]=len; //???????

	//ע������Ĵ浽a5_read[]�����У�
	//a5_read[0]:�������ݳ���
	//a5_read[1] ~ [128]: ��������
	
	//ԭʼ������Read_Tele[]��
	//Read_Tele[0]:Ϊ���ձ�ʶ
	//Read_Tele[1]~....[512]:����

	//��������
	
}


BOOL CA5_Read::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_Static_Silent.SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_SILENT)));

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
