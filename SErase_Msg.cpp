// SErase_Msg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "program_balise.h"
#include "SErase_Msg.h"
#include ".\serase_msg.h"
#include "Comm_Balise_ts.h"

// CSErase_Msg �Ի���

IMPLEMENT_DYNAMIC(CSErase_Msg, CDialog)
CSErase_Msg::CSErase_Msg(CWnd* pParent /*=NULL*/)
	: CDialog(CSErase_Msg::IDD, pParent)
{
}

CSErase_Msg::~CSErase_Msg()
{
}

void CSErase_Msg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSErase_Msg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ERASE_TSE, OnBnClickedButtonEraseTse)
	ON_BN_CLICKED(IDC_BUTTON_ERASE_TPC, OnBnClickedButtonEraseTpc)
	ON_BN_CLICKED(IDC_BUTTON_ERASE_TE1, OnBnClickedButtonEraseTe1)
	ON_BN_CLICKED(IDC_BUTTON_ERASE_THM, OnBnClickedButtonEraseThm)
	ON_BN_CLICKED(IDC_BUTTON_ERASE_THE, OnBnClickedButtonEraseThe)
	ON_BN_CLICKED(IDC_BUTTON_ERASE_TE3, OnBnClickedButtonEraseTe3)
	ON_BN_CLICKED(IDC_BUTTON_ERASE_TE2, OnBnClickedButtonEraseTe2)
	ON_BN_CLICKED(IDC_BUTTON_ERASE_TE4, OnBnClickedButtonEraseTe4)
	ON_BN_CLICKED(IDC_BUTTON_ERASE_TE5, OnBnClickedButtonEraseTe5)
	ON_BN_CLICKED(IDC_BUTTON_ERASE_TE6, OnBnClickedButtonEraseTe6)
END_MESSAGE_MAP()


// CSErase_Msg ��Ϣ�������

void CSErase_Msg::OnBnClickedButtonEraseTse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Erase_Message(ORDER_LEU_TSE);
}

void CSErase_Msg::OnBnClickedButtonEraseTpc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Erase_Message(ORDER_LEU_TPC);
}


void CSErase_Msg::OnBnClickedButtonEraseThm()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Erase_Message(ORDER_LEU_THM);
}

void CSErase_Msg::OnBnClickedButtonEraseThe()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Erase_Message(ORDER_LEU_THE);
}

void CSErase_Msg::OnBnClickedButtonEraseTe1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Erase_Message(ORDER_LEU_TE1);
}

void CSErase_Msg::OnBnClickedButtonEraseTe2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Erase_Message(ORDER_LEU_TE2);
}

void CSErase_Msg::OnBnClickedButtonEraseTe3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Erase_Message(ORDER_LEU_TE3);
}


void CSErase_Msg::OnBnClickedButtonEraseTe4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Erase_Message(ORDER_LEU_TE4);
}

void CSErase_Msg::OnBnClickedButtonEraseTe5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Erase_Message(ORDER_LEU_TE5);
}

void CSErase_Msg::OnBnClickedButtonEraseTe6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Erase_Message(ORDER_LEU_TE6);
}


BOOL CSErase_Msg::Erase_Message(int type)
{
	int len,len_data;
	byte ch[1024];
	int pos=0;
	byte temp;
	int count;

	//����
	pos=0;
	ch[pos++]=0x24;
	ch[pos++]=0x00;
	ch[pos++]=0x00;
	ch[pos++]=type;

	//����
	len_data=pos-3;
	ch[1]=(byte)(len_data & 0xff);
	len_data>>=8;
	ch[2]=(byte)(len_data & 0xff);

	len=pos;
	Read_Tele_Len=0;
	SendCommand(len,&ch[0]);

	count=0;
	do{
		//��������
		Read_Tele_Len=0;
		ReceiveData(READLEN_LEU_ERASE);

		if(ReadBufLen>0)
		{
			if(ArrangeData(ORDER_LEU_ERASE))
			{
				//���ı�ʶ
				temp=Read_Tele[0];
				if(temp==0x02)
				{
					MessageBox("����ʧ�ܣ�����ͨ��У�����","����",MB_OK);
					return FALSE;
				}
				else if(temp==0x12)
				{
					MessageBox("����ʧ�ܣ������źŲ�������","����",MB_OK);
					return FALSE;
				}
				else if(temp==0x11)
				{
					MessageBox("�����ɹ�","�ɹ�",MB_OK);
					return TRUE;
				}
				else
				{
					MessageBox("�������ݸ�ʽ����","����",MB_OK);
					return FALSE;
				}
			}
		}

		Sleep(500);
		count++;
	}while(count<30);

	if(count>29)
	{
		MessageBox("����ʧ�ܣ�ͨ��δ������","����",MB_OK);
		return FALSE;
	}

	return TRUE;
}