// A5_Write.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "program_balise.h"
#include "A5_Write.h"
#include "Comm_Balise_ts.h"
#include "a5_write.h"
#include "BmpStatic.h"

// CA5_Write �Ի���

IMPLEMENT_DYNAMIC(CA5_Write, CDialog)
CA5_Write::CA5_Write(CWnd* pParent /*=NULL*/)
	: CDialog(CA5_Write::IDD, pParent)
{
}

CA5_Write::~CA5_Write()
{
}

void CA5_Write::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT13, m_telegramfile);
	DDX_Control(pDX, IDC_LIST1, m_telegram);
	DDX_Control(pDX, IDC_STATIC4, m_Result_Bmp);
	DDX_Control(pDX, IDC_EDIT1, m_Result);
}


BEGIN_MESSAGE_MAP(CA5_Write, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton_A5Write)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton_OpenFile)
END_MESSAGE_MAP()


// CA5_Write ��Ϣ�������

BOOL CA5_Write::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	m_Result_Bmp.SetResultBmp(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}

void CA5_Write::OnBnClickedButton_A5Write()
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

	Clear_Display();

	m_Result_Bmp.SetResultBmp(0);
	
	m_Result.SetWindowText("");
	m_Result.UpdateWindow();

	m_telegramfile.GetWindowText(filename);
	if(!InitTelegram(filename,&a5_orig[0]))
	{
		MessageBox("�޷��򿪱����ļ�,\n��ѡ�����ļ���","����",MB_OK);
		return;
	}

	//����ԭʼ����
	for(i=0;i<(a5_orig[0]+1);i++) a5_send[i]=a5_orig[i];

	//�̱��ģ�43�ֽڣ����3����Чλ��
	if(a5_send[0]==43)
	{
		left_shift_bits(&a5_send[1],a5_send[0],3,3,&total_len);
		a5_send[0]=(byte)(total_len & 0xff);
	}

	//��ʾԭʼ����
	//******************************************
	m_telegram.AddString("A5д����");
	ss="";
	for(i=0;i<a5_orig[0];i++)
	{
		ss1.Format("%02X  ",a5_orig[i+1]);
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
	A5_Arrange_Tele(&a5_send[1]);

	//******************************************

	//����д����
	//******************************************
	ch[pos++]=0x10;
	ch[pos++]=0x00;//
	ch[pos++]=0x00;//

	ch[pos++]=0x55;
	ch[pos++]=0xff;
	
	for(i=0;i<128;i++) ch[pos++]=a5_send[i+1];

	len_data=pos-3;
	ch[1]=(byte)(len_data & 0xff);
	len_data>>=8;
	ch[2]=(byte)(len_data & 0xff);

	len=pos;

	SendCommand(len,&ch[0]);

	//***************************************

	//**************************************
	Read_Tele_Len=0;
	ReceiveData(READLEN_A5_WRITE_TELE);

	if(ReadBufLen>0)
	{
		if(ArrangeData(ORDER_A5_WRITE_TELE))
		{
			temp=Read_Tele[0];
			if(temp==0x02)
			{
				ss="д����ʧ�ܣ�����ͨ��У�����";
				flag=false;
			}
			else if(temp==0x12)
			{
				ss="д����ʧ�ܣ������źŴ���";
				flag=false;
			}
			else
			{
				ss="д����ģʽ��д��������";
				flag=true;
			}
		}
	}
	else 
	{
		ss="д����ʧ�ܣ�ͨ��δ������";
		flag=false;
	}

	//ͨ��A4��ȡд��ı��ģ��ж�д���Ƿ���ȷ��
	//*********************************************
	if(flag)
	{
		Sleep(1000);
		if(!A4_Read_Tele())
		{
			flag=false;
			ss="д����ʧ�ܣ�ͨ��A4�ض����Ĵ���";
		}
		else
		{
			flag=true;
			ss="д�����ɹ���";
		}
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

void CA5_Write::Clear_Display()
{
	m_telegram.ResetContent();
	for(int i=0;i<129;i++)
	{
		a5_send[i]=0x00;
	}
}

void CA5_Write::OnBnClickedButton_OpenFile()
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


void CA5_Write::A5_Arrange_Tele(byte *buff)
{
	byte res_buff[128];
	byte tel_xor[128]={
		0x7f, 0xd7, 0x44, 0xfc, 0x60, 0xe1, 0xdd, 0xc8, 0xd0, 0x89, 0x5a, 0x2d, 0xcd, 0xc7, 0xe9, 0x29,  
		0xce, 0x09, 0x81, 0x0c, 0xc7, 0x1a, 0xb3, 0xc7, 0x4b, 0xc5, 0xc0, 0x75, 0xf9, 0x84, 0x1b, 0xa7,  
		0x69, 0x50, 0x03, 0x0d, 0x20, 0x2e, 0xa1, 0x43, 0xc2, 0x5c, 0xa5, 0xb3, 0x96, 0x3d, 0xc2, 0xfe,  
		0x49, 0xbd, 0xea, 0x45, 0x4a, 0x22, 0xf4, 0x67, 0x7d, 0x0d, 0x82, 0xc2, 0xaf, 0x3f, 0xba, 0xd4,  
		0x79, 0x0e, 0x4c, 0xaa, 0x8a, 0x36, 0xa9, 0xe6, 0xb2, 0x82, 0x93, 0xd9, 0x38, 0x84, 0xe8, 0x3d,  
		0x60, 0x12, 0x47, 0xc1, 0x92, 0xcd, 0x96, 0x9f, 0x2e, 0xf0, 0x35, 0xc5, 0x62, 0x99, 0xf7, 0xf8, 
		0x63, 0x2f, 0xb5, 0xed, 0xd9, 0x9a, 0x68, 0xe6, 0x41, 0x18, 0x9a, 0x9b, 0x7c, 0x48, 0x5a, 0xde, 
		0x57, 0xce, 0xab, 0x6d, 0x02, 0xbb, 0x62, 0x3b, 0x1b, 0xf6, 0x1f, 0x57, 0x3d, 0x31, 0x64, 0x40,};


	unsigned int clr_num,i,j,m,temp,temp1,s_temp;

	m=0;
	for(;;)
	{
		for(i=0;i<128;i++) res_buff[i]=buff[i]^tel_xor[i];

		clr_num=0;
		for(i=0;i<1024;i++)
		{
			temp=res_buff[i/8];
			temp1=0x01<<(7-i%8);
			if((temp&temp1)==0) clr_num++;
			else clr_num=0;
			if(clr_num>=11) break;
		}
		if(i!=1024)
		{
			i-=8;
			temp=i/8;
			if(temp==0) temp=1;
			temp*=8;
			if(m>=2) temp=1;
			for(i=0;i<temp;i++)
			{
				temp1=buff[127]&0x01;
				if((buff[0]&0x80)!=0) buff[127]|=0x01;
				else buff[127]&=~0x01;
				for(j=0;j<128;j++)
				{
					s_temp=buff[j]<<1;
					if(j!=127) s_temp+=buff[j+1]>>7;
					s_temp&=0xff;
					buff[j]=s_temp;
				}
				if(temp1!=0) buff[127]|=0x01;
			}
			m++;
		}
		else break;
	}

	for(i=0;i<128;i++) buff[i] ^= tel_xor[i];

	return;
}

BOOL CA5_Write::A4_Read_Tele()
{
	int len;
	byte ch[1024];
	int pos=0;
	int i;
	byte temp;
	BOOL m_same;

	pos=0;
	ch[pos++]=0x01;
	ch[pos++]=0x00;
	ch[pos++]=0x00;

	len=pos;
	SendCommand(len,&ch[0]);

	//��������
	Read_Tele_Len=0;
	Sleep(500);
	ReceiveData(READLEN_A4_READ_TELE);

	if(ReadBufLen>0)
	{
		if(ArrangeData(ORDER_A4_READ_TELE))
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
			case 0x21:
				//ss="A4�����̱��ġ�";
				break;
			case 0x22:
				//ss="A4���������ġ�";
				break;
			default:
				MessageBox("������ʧ��:����!","����",MB_OK);
				return FALSE;
				break;
			}
			len=0;
			if(temp==0x21) len=43;
			else if(temp==0x22) len=128;
			a4_read[0]=len; 
			
			for(i=0;i<len;i++)
			{
				a4_read[i+1]=Read_Tele[i+2];
			}
			//************************************************
			

			//************************************************
			//�Ƚ�
			if(temp==0x21 || temp==0x22)
			{
				if(temp==0x21) 
				{
					a4_read[43]&=0xf8;
					a5_orig[43]&=0xf8;
				}
				else if(temp==0x22)
				{
					a4_read[128]&=0xfe;
					a5_orig[128]&=0xfe;
				}

				len=a4_read[0];
				m_same=TRUE;
				for(i=0;i<len;i++)
				{
					if(a5_orig[i+1]!=a4_read[i+1])
					{
						m_same=FALSE;
						break;
					}
				}
			}
			//***********************************************
		}
	}
	else 
	{
		MessageBox("������ʧ�ܣ�ͨ��δ������","����",MB_OK);

		return FALSE;
	}

	return m_same;

}