// program_balise.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "program_balise.h"
#include "MainFrm.h"

#include "program_baliseDoc.h"
#include "program_baliseView.h"

#include "Comm_Balise_ts.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cprogram_baliseApp

BEGIN_MESSAGE_MAP(Cprogram_baliseApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// Cprogram_baliseApp ����

Cprogram_baliseApp::Cprogram_baliseApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� Cprogram_baliseApp ����

Cprogram_baliseApp theApp;

// Cprogram_baliseApp ��ʼ��

BOOL Cprogram_baliseApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

	CWinApp::InitInstance();

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ����� MRU��
	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(Cprogram_baliseDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(Cprogram_baliseView));
	AddDocTemplate(pDocTemplate);
	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// �������ں�׺ʱ�ŵ��� DragAcceptFiles��
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand  ֮����

	//if(!Open_LogFile()) m_pMainWnd->MessageBox("�޷���log �ļ�!","����",MB_OK);

	if(!Start_ResetUsbDev(TRUE))
		m_pMainWnd->MessageBox("�����豸����1","����",MB_OK);
	else
	{
		//m_pMainWnd->MessageBox("�����豸�ɹ�","�ɹ�",MB_OK);
		Sleep(1000);
	//	Get_ComDev();

		if(!InitComm())
		{
			//m_pMainWnd->MessageBox("��ʼ��COM�ڴ���","����",MB_OK);
			m_pMainWnd->MessageBox("�����豸����2","����",MB_OK);
			Print_Log(&logfile,"�����豸����\n");
		}
		else
		{
			//m_pMainWnd->MessageBox("��ʼ��COM�ڳɹ���","������ʾ",MB_OK);
			m_pMainWnd->MessageBox("�����豸�ɹ�","�ɹ�",MB_OK);
			Print_Log(&logfile,"�����豸�ɹ�\n");
		}
	}

	return TRUE;
}



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void Cprogram_baliseApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// Cprogram_baliseApp ��Ϣ�������


int Cprogram_baliseApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	if(hCom)
	{
		CloseComm(hCom);
		Print_Log(&logfile,"�ر�COM�ڳɹ�\n");
	}

	//Close_LogFile();

	if(!Start_ResetUsbDev(FALSE))
		;//m_pMainWnd->MessageBox("ͣ���豸����","����",MB_OK);
	else ;//m_pMainWnd->MessageBox("ͣ���豸�ɹ�","�ɹ�",MB_OK);

	return CWinApp::ExitInstance();
}

