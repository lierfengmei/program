// BaliseCMeas.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "program_balise.h"
#include "BaliseCMeas.h"
#include "BmpStatic.h"
#include "Comm_Balise_ts.h"

// CBaliseCMeas �Ի���

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


// CBaliseCMeas ��Ϣ�������

BOOL CBaliseCMeas::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_Result_Bmp.SetResultBmp(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CBaliseCMeas::OnBnClickedButton_OpenFile()
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

void CBaliseCMeas::OnBnClickedButtonMeasC()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		MessageBox("�޷��򿪱����ļ�,\n��ѡ�����ļ���","����",MB_OK);
		return;
	}

	//����ԭʼ����
	for(i=0;i<(c5_orig[0]+1);i++) c5_send[i]=c5_orig[i];

	//�̱��ģ�43�ֽڣ����3����Чλ��
	if(c5_send[0]==43)
	{
		left_shift_bits(&c5_send[1],c5_send[0],3,3,&total_len);
		c5_send[0]=(byte)(total_len & 0xff);
	}

	//��ʾԭʼ����
	//******************************************
	m_telegram.AddString("C�ӿڴ��䱨��");
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

	//��������
	//******************************************

	//******************************************

	//����д����
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
				//ss="��������������";
				//flag=true;
				break;
			case 0x02:
				ss="д����ʧ�ܣ�����ͨ��У�����";
				flag=false;
				break;
			case 0x20:
				ss="A4��ʧ�ܣ�δ�������ģ�";
				flag=false;
				break;
			case 0x21: //�̱���
			case 0x22: //������
				ss="C �ӿڹ���������";
				flag=true;
				break;
			case 0x30:
				ss="C �ӿڹ��ܹ��ϣ�";
				flag=false;
				break;
			default:
				break;
			}
			if(temp==0x21 || temp==0x22)
			{
				//c4��ʶ
				c4_flag=Read_Tele[1];
				if(c4_flag==0) m_Have_C4.SetWindowText("����Ӧ������C4�ӿڡ�");
				else if(c4_flag==1) m_Have_C4.SetWindowText("����Ӧ������C4�ӿڡ�");

				//����
				if(temp==0x21) len=43;
				if(temp==0x22) len=128;
				a4_read[0]=len;
				for(i=0;i<len;i++)
					a4_read[i+1]=Read_Tele[i+2];

				//�Ƚϱ���
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
					ss="C �ӿڹ���������";
					flag=true;
				}
				else 
				{
					ss="C �ӿڣ��������ݲ�һ�£�";
					flag=false;
				}
			}
		}
	}
	else 
	{
		ss="д����ʧ�ܣ�ͨ��δ������";
		flag=false;
	}

	//*********************************************

	//��ʾ����
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
