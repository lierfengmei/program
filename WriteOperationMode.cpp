// WriteOperationMode.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BmpStatic.h"
#include "program_balise.h"
#include "WriteOperationMode.h"
#include "Comm_Balise_ts.h"

// CWriteOperationMode �Ի���

IMPLEMENT_DYNAMIC(CWriteOperationMode, CDialog)
CWriteOperationMode::CWriteOperationMode(CWnd* pParent /*=NULL*/)
	: CDialog(CWriteOperationMode::IDD, pParent)
{
}

CWriteOperationMode::~CWriteOperationMode()
{
}

void CWriteOperationMode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Result);
	DDX_Control(pDX, IDC_RADIO_SILENT, m_Silent);
	DDX_Control(pDX, IDC_RADIO_FUNCTIONAL, m_Functional);
	DDX_Control(pDX, IDC_STATIC3, m_Result_Bmp);
}


BEGIN_MESSAGE_MAP(CWriteOperationMode, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_WRITE, OnBnClickedButtonWrite)
END_MESSAGE_MAP()


// CWriteOperationMode ��Ϣ�������

void CWriteOperationMode::OnBnClickedButtonWrite()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	byte mode[2]={0x5d,0x5c};
	byte ch[1024];
	int pos=0;
	int check;
	int i;
	CString ss="";
	int len,len_data;
	byte temp;
	bool flag=false;

	if(m_Silent.GetCheck()==BST_CHECKED) check=0;
	else if(m_Functional.GetCheck()==BST_CHECKED) check=1;
	else return;

	m_Result.SetWindowText("");
	m_Result.UpdateWindow();
	
	m_Result_Bmp.SetResultBmp(0);

	ch[pos++]=0x11;
	ch[pos++]=0x00;
	ch[pos++]=0x00;

	ch[pos++]=mode[check];

	len_data=pos-3;
	ch[1]=(byte)(len_data & 0xff);
	len_data>>=8;
	ch[2]=(byte)(len_data & 0xff);

	len=pos;

	for(i=0;i<5;i++)
	{
		SendCommand(len,&ch[0]);
		//��������
		Read_Tele_Len=0;
		Sleep(500);
		ReceiveData(READLEN_A5_WRITE_MODE);

		if(ReadBufLen>0)
		{
			if(ArrangeData(ORDER_A5_WRITE_MODE))
			{
				//��ʶ
				ss="";
				temp=Read_Tele[0];
				if(temp==0x02)
				{
					ss="д����ʧ�ܣ�����ͨ��У�����";
					flag=false;
					break;
				}
				else if(temp==0x12)
				{
					ss="д����ʧ�ܣ������źŴ���";
					flag=false;
					break;
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
			break;
		}
	}

	m_Result.SetWindowText(ss);

	if(flag)
	{
		m_Result_Bmp.SetResultBmp(1);
	}
	else 
	{
		m_Result_Bmp.SetResultBmp(2);
	}
    
}

BOOL CWriteOperationMode::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_Silent.SetCheck(BST_CHECKED);
	m_Functional.SetCheck(BST_UNCHECKED);
	
	m_Result_Bmp.SetResultBmp(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
