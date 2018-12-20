// MenuTest.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MenuTest.h"

#include "MainFrm.h"
#include "MenuTestDoc.h"
#include "MenuTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenuTestApp

BEGIN_MESSAGE_MAP(CMenuTestApp, CWinApp)
	//{{AFX_MSG_MAP(CMenuTestApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuTestApp construction

CMenuTestApp::CMenuTestApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMenuTestApp object

CMenuTestApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMenuTestApp initialization

BOOL CMenuTestApp::InitInstance()
{
#ifndef _DEBUG
	if (!FirstInstance())
		return FALSE;	//已经有实例存在了，退出
#endif
	AfxEnableControlContainer();

#ifdef DATA_BACE
	if (!AfxOleInit()) {
		AfxMessageBox("初始化 OLE DLL失败 ！");
		return FALSE;
	}
#endif

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
//	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
//	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMenuTestDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CMenuTestView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
#ifdef WINDOW_MAX
	m_nCmdShow=SW_MAXIMIZE;		//最大化窗口
#endif
	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
#ifndef _DEBUG
CodeDlg.DoModal();
#endif
	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	AfxGetMainWnd()->SetWindowText("士腾科技智能测试上位机软件 20180817");

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnStartUp();
//	afx_msg void OnLogin();
//	afx_msg void OnBnSetCodeOk();
//	afx_msg void OnComSet();
//	afx_msg void On32786();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
//	ON_COMMAND(ID_START_UP, &CAboutDlg::OnStartUp)
//	ON_COMMAND(ID_32780, &CAboutDlg::OnLogin)
//ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnSetCodeOk)
//	ON_COMMAND(ID_COM_SET, &CAboutDlg::OnComSet)
//	ON_COMMAND(ID_32786, &CAboutDlg::On32786)
END_MESSAGE_MAP()

// App command to run the dialog
void CMenuTestApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CMenuTestApp message handlers 

BOOL CMenuTestApp::FirstInstance()
{
  CWnd *pWndPrev, *pWndChild; 
  //根据主窗口类名和主窗口名判断是否已经有实例存在了
  if (pWndPrev = CWnd::FindWindow(NULL,"士腾科技智能测试上位机软件 v1.0"))//#32770是默认的对话框类名字
  {
	  //如果存在就将其激活，并显示出来	  
	  pWndChild = pWndPrev->GetLastActivePopup(); 	  
	  //如果是最小化的就还原窗口
	  if (pWndPrev->IsIconic()) 
		  pWndPrev->ShowWindow(SW_RESTORE);      	  
	  pWndChild->SetForegroundWindow();         	 
	  return FALSE;                             
  }
  else
	  return TRUE;
}
//
//void CAboutDlg::OnComSet()
//{
//	// TODO: 在此添加命令处理程序代码
//	AfxMessageBox("COM select ...");
//}
//
//
//void CAboutDlg::On32786()
//{
//	// TODO: 在此添加命令处理程序代码
//}
