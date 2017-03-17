// program_balise.cpp : 定义应用程序的类行为。
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
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// Cprogram_baliseApp 构造

Cprogram_baliseApp::Cprogram_baliseApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 Cprogram_baliseApp 对象

Cprogram_baliseApp theApp;

// Cprogram_baliseApp 初始化

BOOL Cprogram_baliseApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControls()。否则，将无法创建窗口。
	InitCommonControls();

	CWinApp::InitInstance();

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项（包括 MRU）
	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(Cprogram_baliseDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(Cprogram_baliseView));
	AddDocTemplate(pDocTemplate);
	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// 仅当存在后缀时才调用 DragAcceptFiles，
	//  在 SDI 应用程序中，这应在 ProcessShellCommand  之后发生

	//if(!Open_LogFile()) m_pMainWnd->MessageBox("无法打开log 文件!","错误",MB_OK);

	if(!Start_ResetUsbDev(TRUE))
		m_pMainWnd->MessageBox("启用设备错误1","错误",MB_OK);
	else
	{
		//m_pMainWnd->MessageBox("启用设备成功","成功",MB_OK);
		Sleep(1000);
	//	Get_ComDev();

		if(!InitComm())
		{
			//m_pMainWnd->MessageBox("初始化COM口错误","错误",MB_OK);
			m_pMainWnd->MessageBox("启用设备错误2","错误",MB_OK);
			Print_Log(&logfile,"启用设备错误\n");
		}
		else
		{
			//m_pMainWnd->MessageBox("初始化COM口成功！","操作提示",MB_OK);
			m_pMainWnd->MessageBox("启用设备成功","成功",MB_OK);
			Print_Log(&logfile,"启用设备成功\n");
		}
	}

	return TRUE;
}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// 用于运行对话框的应用程序命令
void Cprogram_baliseApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// Cprogram_baliseApp 消息处理程序


int Cprogram_baliseApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	if(hCom)
	{
		CloseComm(hCom);
		Print_Log(&logfile,"关闭COM口成功\n");
	}

	//Close_LogFile();

	if(!Start_ResetUsbDev(FALSE))
		;//m_pMainWnd->MessageBox("停用设备错误","错误",MB_OK);
	else ;//m_pMainWnd->MessageBox("停用设备成功","成功",MB_OK);

	return CWinApp::ExitInstance();
}

