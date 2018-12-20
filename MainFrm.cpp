// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MenuTest.h"

#include "MainFrm.h"
#include "Tab1.h"
#include "MyCom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int fgNew = 0;
extern PARA_SETTING   setting;
extern PARA_SETTING   new_setting;
/////////////////////////////////////////////////////////////////////////////
// CMainFrame
#include "MenuTestDoc.h"
#include "MenuTestView.h"

extern BOOL /*CTab2::*/LoadFile(CString CodeFileName, PARA_SETTING *file_setting);
extern void ClearSetting(PARA_SETTING *set);

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_NEWMENU, OnNetSetting)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_NEW_PROJ, OnNewProj)
	ON_COMMAND(ID_32780, OnSetCode)
	ON_COMMAND(ID_START_UP, ng_disp)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_32781, &CMainFrame::OnIpPortSetting)
//	ON_COMMAND(ID_32782, &CMainFrame::OnTestResult)
	ON_COMMAND(ID_32785, &CMainFrame::On32785)
	ON_COMMAND(ID_32786, &CMainFrame::On32786)
	ON_COMMAND(ID_32787, &CMainFrame::On32787)
	ON_COMMAND(ID_32788, &CMainFrame::On32788)
	ON_COMMAND(ID_32789, &CMainFrame::On32789)
	ON_COMMAND(ID_32790, &CMainFrame::On32790)
	ON_COMMAND(ID_32791, &CMainFrame::On32791)
	ON_COMMAND(ID_32792, &CMainFrame::On32792)
	ON_COMMAND(ID_32793, &CMainFrame::On32793)
	ON_COMMAND(ID_32794, &CMainFrame::On32794)
	ON_UPDATE_COMMAND_UI(ID_32785, &CMainFrame::OnUpdate32785)
	ON_UPDATE_COMMAND_UI(ID_32786, &CMainFrame::OnUpdate32786)
	ON_UPDATE_COMMAND_UI(ID_32787, &CMainFrame::OnUpdate32787)
	ON_UPDATE_COMMAND_UI(ID_32788, &CMainFrame::OnUpdate32788)
	ON_UPDATE_COMMAND_UI(ID_32789, &CMainFrame::OnUpdate32789)
	ON_UPDATE_COMMAND_UI(ID_32790, &CMainFrame::OnUpdate32790)
	ON_UPDATE_COMMAND_UI(ID_32791, &CMainFrame::OnUpdate32791)
	ON_UPDATE_COMMAND_UI(ID_32792, &CMainFrame::OnUpdate32792)
	ON_UPDATE_COMMAND_UI(ID_32793, &CMainFrame::OnUpdate32793)
	ON_UPDATE_COMMAND_UI(ID_32794, &CMainFrame::OnUpdate32794)
	ON_COMMAND(ID_32797, &CMainFrame::On32797)
	ON_COMMAND(ID_32798, &CMainFrame::On32798)
	ON_UPDATE_COMMAND_UI(ID_32798, &CMainFrame::OnUpdate32798)
	ON_COMMAND(ID_LOG_OUT, &CMainFrame::OnLogOut)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
	ID_INDICATOR_NET_STATE
};

CList<HISTROY_FILE ,HISTROY_FILE&> histrory_list;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
/*	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
*/
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
/*	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
*/
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style = WS_OVERLAPPED | WS_CAPTION |WS_THICKFRAME
| WS_SYSMENU | WS_MAXIMIZE | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
/*
	int xSize=::GetSystemMetrics(SM_CXSCREEN);
	int ySize=::GetSystemMetrics(SM_CYSCREEN);
	cs.cx=xSize * 53/100;
	cs.cy=ySize * 73/100;
	cs.x=(xSize-cs.cx)/2;
	cs.y=(ySize-cs.cy)/2;
*/
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnNetSetting() 
{
	fgNew = 2;
	ClearSetting(&new_setting);
	new_dlg.DoModal();	
}

void CMainFrame::OnSetCode()
{
	// TODO: 在此添加命令处理程序代码
	//CodeDlg.DoModal();
	// 更改密码
	SetCodeDlg.DoModal();
}  

void CMainFrame::ng_disp()
{
	// TODO: 在此添加命令处理程序代码

	// 启动测试，将窗口上其他空间置无效灰色
	// 。。。
	ng_record.DoModal();

}


void CMainFrame::Ontest1() 
{

}

void CMainFrame::OnFileOpen() 
{
	// TODO: Add your command handler code here
	return;
}


CString file_selected = "";		// get file histroy cstring
int top = 0;
// file_histoy_init()
// {
//	// read txt into struct
// }


/*   还有个BUG：
**   1,  动态菜单要第二个开始
*/

/*   本软件中重要技巧为：
**   重写了OnCommond()函数，并巧妙利用WPARAM wParam, LPARAM lParam 两个参数。从而实现了全部功能。
**   详见：virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
*/
 BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	 //static k = 0;
	static int ID2 = ID_BACE;
	HISTROY_FILE his_file;

/////////////////////////////////////
	//获取顶级菜单
    CMenu* pMenu=GetMenu();
    //获取1级菜单
    CMenu* pTestMenu=pMenu->GetSubMenu(0);
    //添加一个2级弹出菜单，将"文件"菜单项作为该菜单的子菜单
	if (top == 0)
		pTestMenu->AppendMenu(MF_POPUP,(UINT)pTestMenu->m_hMenu,"历史记录 >>");
    //添加n级菜单
    CMenu* pnMenu=pTestMenu->GetSubMenu(1);  
    if(pnMenu==NULL) return FALSE;

/////////////////////////////////////
	 if (0 == wParam)  {
		//histroy_file[top].histroy = *(CString *)lParam;
		//histroy_file[top].ID = ID2++;
		 
		his_file.histroy = *(CString *)lParam;
		his_file.ID = ID2++;

		histrory_list.AddHead(his_file);
		(pnMenu)->InsertMenu(0, MF_BYPOSITION | MF_STRING, (UINT)his_file.ID,his_file.histroy);	

		top++;
		return TRUE;
	 }else if (wParam >= ID_BACE && ID_BACE < ID_BACE + HIS_FILE_NUM) {
		//file_selected = histroy_file[wParam - ID_BACE].histroy;
		POSITION pos = histrory_list.GetHeadPosition(); // pos == 0;
		POSITION pos_old;

		pos_old = pos;
		for (int i = 0; i < histrory_list.GetCount(); i++) {
			if (wParam == histrory_list.GetNext(pos).ID){
				his_file.histroy = histrory_list.GetAt(pos_old).histroy;
				his_file.ID = histrory_list.GetAt(pos_old).ID;
				histrory_list.RemoveAt(pos_old);
				histrory_list.AddHead(his_file);
				(pnMenu)->DeleteMenu(wParam, MF_BYCOMMAND);
				(pnMenu)->InsertMenu(0, MF_BYPOSITION | MF_STRING, (UINT)his_file.ID,his_file.histroy);

				file_selected = his_file.histroy;
			}
			pos_old = pos;
		}

		TRACE(file_selected);
		pCurrView->m_CurrProj.SetWindowText(file_selected);
		pCurrView->GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
		pCurrView->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);

		//2, 读取文件配置信息
		LoadFile(file_selected, &setting);
	 }
	 else {
		 CFrameWnd::OnCommand(wParam, lParam);
		 //TRACE("\r\n normal manu !");
	 }

	 return TRUE;
}
extern PARA_SETTING new_setting;
extern void ClearSetting(PARA_SETTING *set);
void CMainFrame::OnNewProj() 
{
	// TODO: Add your command handler code here
	fgNew = 1;
	ClearSetting(&new_setting);
	new_dlg.DoModal();
}

void CMainFrame::OnIpPortSetting()
{
	// TODO: 在此添加命令处理程序代码
#ifdef MGT_SOCKET_CLIENT
	ip_port.DoModal();
#endif
}
/*
void CMainFrame::OnTestResult()
{
	
}
*/
// 默认选中 COM1
int fg_com[11] = {0,0,0,0,1,0,0,0,0,0,0};

int g_com_selected = 4;
extern CMyCom MyCom; 
void CMainFrame::On32785()					// COM1
{
	// TODO: 在此添加命令处理程序代码
	if (g_com_selected != 1) {
		fg_com[g_com_selected] = 0;
		g_com_selected = 1;
		fg_com[g_com_selected] = 1;
		MyCom.ReSetComPort("COM1");
	}
}


void CMainFrame::On32786()					// COM2
{
	if (g_com_selected != 2) {
		// TODO: 在此添加命令处理程序代码
		fg_com[g_com_selected] = 0;
		g_com_selected = 2;
		fg_com[g_com_selected] = 1;
		MyCom.ReSetComPort("COM2");
	}
}


void CMainFrame::On32787()					// COM3
{
	if (g_com_selected != 3) {
		// TODO: 在此添加命令处理程序代码
		fg_com[g_com_selected] = 0;
		g_com_selected = 3;
		fg_com[g_com_selected] = 1;
		MyCom.ReSetComPort("COM3");
	}
}


void CMainFrame::On32788()					// COM4
{
	if (g_com_selected != 4) {
		// TODO: 在此添加命令处理程序代码
		fg_com[g_com_selected] = 0;
		g_com_selected = 4;
		fg_com[g_com_selected] = 1;
		MyCom.ReSetComPort("COM4");
	}
}


void CMainFrame::On32789()					// COM5
{
	if (g_com_selected != 5) {
		// TODO: 在此添加命令处理程序代码
		fg_com[g_com_selected] = 0;
		g_com_selected = 5;
		fg_com[g_com_selected] = 1;
		MyCom.ReSetComPort("COM5");
	}
}


void CMainFrame::On32790()					// COM6
{
	if (g_com_selected != 6) {
		// TODO: 在此添加命令处理程序代码
		fg_com[g_com_selected] = 0;
		g_com_selected = 6;
		fg_com[g_com_selected] = 1;
		MyCom.ReSetComPort("COM6");
	}
}


void CMainFrame::On32791()					// COM7
{
	if (g_com_selected != 7) {
		// TODO: 在此添加命令处理程序代码
		fg_com[g_com_selected] = 0;
		g_com_selected = 7;
		fg_com[g_com_selected] = 1;
		MyCom.ReSetComPort("COM7");
	}
}

	
void CMainFrame::On32792()					// COM8
{
	if (g_com_selected != 8) {
		// TODO: 在此添加命令处理程序代码
		fg_com[g_com_selected] = 0;
		g_com_selected = 8;
		fg_com[g_com_selected] = 1;
		MyCom.ReSetComPort("COM8");
	}
}


void CMainFrame::On32793()					// COM9
{
	if (g_com_selected != 9) {
		// TODO: 在此添加命令处理程序代码
		fg_com[g_com_selected] = 0;
		g_com_selected = 9;
		fg_com[g_com_selected] = 1;
		MyCom.ReSetComPort("COM9");
	}
}


void CMainFrame::On32794()					// COM10
{
	if (g_com_selected != 10) {
		// TODO: 在此添加命令处理程序代码
		fg_com[g_com_selected] = 0;
		g_com_selected = 10;
		fg_com[g_com_selected] = 1;
		MyCom.ReSetComPort("COM10");
	}
}


void CMainFrame::OnUpdate32785(CCmdUI *pCmdUI)			// 1
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(fg_com[1]? true : false);

}


void CMainFrame::OnUpdate32786(CCmdUI *pCmdUI)			//2
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(fg_com[2]? true : false);
}


void CMainFrame::OnUpdate32787(CCmdUI *pCmdUI)			//3
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(fg_com[3]? true : false);
}


void CMainFrame::OnUpdate32788(CCmdUI *pCmdUI)			//4
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(fg_com[4]? true : false);
}


void CMainFrame::OnUpdate32789(CCmdUI *pCmdUI)			//5
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(fg_com[5]? true : false);
}


void CMainFrame::OnUpdate32790(CCmdUI *pCmdUI)			//6
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(fg_com[6]? true : false);
}


void CMainFrame::OnUpdate32791(CCmdUI *pCmdUI)			//7
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(fg_com[7]? true : false);
}


void CMainFrame::OnUpdate32792(CCmdUI *pCmdUI)			//8
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(fg_com[8]? true : false);
}


void CMainFrame::OnUpdate32793(CCmdUI *pCmdUI)			//9
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(fg_com[9]? true : false);
}


void CMainFrame::OnUpdate32794(CCmdUI *pCmdUI)			//10
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(fg_com[10]? true : false);
}


void CMainFrame::On32797()				// 统计功能
{
	// TODO: 在此添加命令处理程序代码
}

int all_data_sel = 1;
void CMainFrame::On32798()				// 测试全数据保存
{
	// TODO: 在此添加命令处理程序代码
	all_data_sel ^= 1;
}


void CMainFrame::OnUpdate32798(CCmdUI *pCmdUI)	// 测试全数据保存
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(all_data_sel? true : false);
}
/*
void CMainFrame::SetStateBarServerState(CString state)
{
	m_wndStatusBar.SetPaneText(4, state);
}
*/

void CMainFrame::OnLogOut()
{
	// TODO: 在此添加命令处理程序代码
	if (IDOK == AfxMessageBox("确定要退出吗？", MB_OKCANCEL)) {
		((CMenuTestApp*)AfxGetApp())->CodeDlg.DoModal();
	}
}
