// MenuTestView.cpp : implementation of the CMenuTestView class
//

#include "stdafx.h"
#include "MenuTest.h"

#include "MenuTestDoc.h"
#include "MenuTestView.h"
#include "MainFrm.h"
#include "_dlgNew.h"
#include "math.h"
#include "MyCom.h"

#ifdef SYS_THREAD_POOL
	#include "atlutil.h"
	#include "tasks.h"
	#include "worker.h"

	CThreadPool<CMyWorker> pool;
	CTaskArray    tasks;
	extern BOOL thread_pool_init();
	extern void ClearPoolAndTask();
#endif

extern CMyCom MyCom;
extern CList<CString ,CString&> histrory_list;
extern UINT SocketClientRevProc(LPVOID pParam);
extern UINT SocketConnectProc(LPVOID pParam);

#if (defined MGT_SOCKET) || (defined MGT_SOCKET_CLIENT)
#include "winsock2.h"   
#pragma comment(lib, "ws2_32.lib")
#endif

#ifdef DATA_BACE
#include "CPFile.h"
CPData *p_cp_data = NULL;
_ConnectionPtr pConnection;
int ado_open_ok= 1;
#endif

extern int total_item_num;

CStdioFile local_excel_file;		// 无数据库情况下的测试记录本地EXCEL文件保存

#ifdef _DEBUG
CString ng_debug_data_record[1] = 
{"Insert into TianYouShou values ('20180108 12:18:32','天佑M370','0x00112200','风扇启动电压','2180 mv','二档转速', '3100','限流', '20.3','欠压', 'OK','NTC保护', 'OK', '电流', '20.3','刘芳芳','','')"
};
#endif

#ifdef MGT_SOCKET 


extern UINT SocketListenProc(LPVOID pParam);
extern UINT FuncToClient(LPVOID pParam);
//static	int		client_num = 0;


#define BUF_SIZE 1024
SOCKET          sServer;        //服务器套接字
SOCKET          sClients[MAX_USER_NUM];  //客户端套接字 ,最多支持200个客户端连线                    
SOCKADDR_IN     addrServ;      //服务器地址 

#endif

#ifdef MGT_SOCKET_CLIENT
#ifndef MGT_SOCKET
#define			BUF_SIZE 1024
#endif
WSADATA         wsd;            //WSADATA变量       
SOCKET          sHost;          //服务器套接字      
SOCKADDR_IN     servAddr;       //服务器地址        
char            buf[BUF_SIZE];  //接收数据缓冲区    
char            bufRecv[BUF_SIZE];
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenuTestView

IMPLEMENT_DYNCREATE(CMenuTestView, CFormView)

BEGIN_MESSAGE_MAP(CMenuTestView, CFormView)
	//{{AFX_MSG_MAP(CMenuTestView)
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonToTestBoard)
	ON_CBN_SELCHANGE(IDC_TYPE, OnSelchangeType)
	ON_BN_CLICKED(IDC_BUTTON2, OnReSetting)
	ON_WM_TIMER()
#ifdef MGT_SOCKET
	ON_BN_CLICKED(IDC_BUTTON3, OnBnqqSend)
#endif
#ifdef MGT_SOCKET_CLIENT
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton1)
#endif
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
//	ON_NOTIFY(HDN_ITEMCLICK, 0, &CMenuTestView::OnHdnItemclickList2)
ON_BN_CLICKED(IDC_RADIO1, &CMenuTestView::OnBnClickedRadio1)
ON_BN_CLICKED(IDC_RADIO_CHENGCE, &CMenuTestView::OnBnClickedRadioChengce)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuTestView construction/destruction

CMenuTestView::CMenuTestView()
	: CFormView(CMenuTestView::IDD)
{
	//{{AFX_DATA_INIT(CMenuTestView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here

}

CMenuTestView::~CMenuTestView()
{
}

void CMenuTestView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMenuTestView)
	DDX_Control(pDX, IDC_EDIT1, m_CurrProj);
	DDX_Control(pDX, IDC_TYPE, m_TestType);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT3, m_Sever_Send);
	DDX_Control(pDX, IDC_DISP, m_topc_disp);
	DDX_Control(pDX, IDC_LIST2, m_main_list);
}

BOOL CMenuTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}
//extern int send_program_code(int program_type);
// 系统初始化总函数
void CMenuTestView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

//	send_program_code(0);		// debug
	// 0， 字体初始化
	m_font.CreateFont(
	   24,                        // nHeight
	   0,                         // nWidth
	   0,                         // nEscapement
	   0,                         // nOrientation
	   FW_LIGHT,                 // nWeight
	   FALSE,                     // bItalic
	   FALSE,                     // bUnderline
	   0,                         // cStrikeOut
	   ANSI_CHARSET,              // nCharSet
	   OUT_DEFAULT_PRECIS,        // nOutPrecision
	   CLIP_DEFAULT_PRECIS,       // nClipPrecision
	   DEFAULT_QUALITY,           // nQuality
	   DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
	   "微软雅黑");                 // lpszFacename
	GetDlgItem(IDC_BUTTON1)->SetFont(&m_font);
	GetDlgItem(IDC_BUTTON2)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC1)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC2)->SetFont(&m_font);
	GetDlgItem(IDC_TYPE)->SetFont(&m_font);
	GetDlgItem(IDC_EDIT1)->SetFont(&m_font);

	m_font2.CreateFont(
	   26,                        // nHeight
	   0,                         // nWidth
	   0,                         // nEscapement
	   0,                         // nOrientation
	   FW_SEMIBOLD,                 // nWeight
	   FALSE,                     // bItalic
	   FALSE,                     // bUnderline
	   0,                         // cStrikeOut
	   ANSI_CHARSET,              // nCharSet
	   OUT_DEFAULT_PRECIS,        // nOutPrecision
	   CLIP_DEFAULT_PRECIS,       // nClipPrecision
	   DEFAULT_QUALITY,           // nQuality
	   DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
	   "微软雅黑");                 // lpszFacename
	GetDlgItem(IDC_STATIC3)->SetFont(&m_font2);
	GetDlgItem(IDC_STATIC4)->SetFont(&m_font2);
	GetDlgItem(IDC_STATIC_RECORD)->SetFont(&m_font2);

	m_font3.CreateFont(
	   18,                        // nHeight
	   0,                         // nWidth
	   0,                         // nEscapement
	   0,                         // nOrientation
	   FW_NORMAL,                 // nWeight
	   FALSE,                     // bItalic
	   FALSE,                     // bUnderline
	   0,                         // cStrikeOut
	   ANSI_CHARSET,              // nCharSet
	   OUT_DEFAULT_PRECIS,        // nOutPrecision
	   CLIP_DEFAULT_PRECIS,       // nClipPrecision
	   DEFAULT_QUALITY,           // nQuality
	   DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
	   "宋体");                 // lpszFacename
	m_topc_disp.SetFont(&m_font3);
	GetDlgItem(IDC_LIST2)->SetFont(&m_font3);

	//1， 文件数据库初始化，文件读取历史纪录
	m_TestType.SetCurSel(0);	// 默认园林工具
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
		
	CStdioFile File;
	CString strTemp = "";
	if(File.Open(HIS_FILE,CStdioFile::modeRead))//创建/打开文件
	{
		while(!feof(File.m_pStream))
		{
			File.ReadString(strTemp);strTemp.Remove('\r');
			((CMainFrame*)AfxGetMainWnd())->OnCommand(0,(long)(&strTemp));
		}
	}

#ifdef LOCAL_EXCEL_SAVE
	if(local_excel_file.Open(LOCAL_EXCEL_FILE,CStdioFile::modeCreate|CStdioFile::modeNoTruncate|CStdioFile::modeWrite)) {//创建/打开文件
		local_excel_file.SeekToEnd();	// 追加
	}
#endif

	// 2，串口初始化  （这部分初始化放到尽量靠后位置）
	if (MyCom.uart_init() == FALSE) {
		AfxMessageBox("Uart init ERR !");
		//return;
	}

	

	// 初始化主界面测试记录列表
	DWORD dwStyle = m_main_list.GetExtendedStyle();     
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
	//dwStyle |= LVS_REPORT;
    m_main_list.SetExtendedStyle(dwStyle);
	//total_item_num = 6;
	re_build_main_list(total_item_num, MAIN_LIST);


	// 3, 数据库初始化
#ifdef DATA_BACE
	CString strSRC;
    HRESULT hr;
    try
    {
        hr = pConnection.CreateInstance(__uuidof(Connection));
        //m_pConnection->CursorLocation=adUseClient;
		strSRC="Provider=SQLOLEDB;Server=(local);Database=try_db;UID=sa;PWD=abc#123";
        if(SUCCEEDED(hr))
        { 
			//pConnection->put_ConnectionTimeout(1);		// 设置超时时间
            hr = pConnection->Open(_bstr_t(strSRC),"","",adModeUnknown);
			if(FAILED(hr))
			{
				AfxMessageBox("打开数据库失败！");
				pConnection->Close();
				AfxGetApp()->m_pMainWnd->PostMessage(WM_CLOSE);
				return ;
			}
        }else {
			AfxMessageBox("数据库连接失败！");
			AfxGetApp()->m_pMainWnd->PostMessage(WM_CLOSE);
			return;
		}
    }
 
    catch(_com_error e)    //捕捉异常     
    {
        CString errormessage;
        errormessage.Format("连接数据库失败!\r\n错误信息:%s",e.ErrorMessage());
        AfxMessageBox(errormessage);//显示错误信息
		ado_open_ok = 0;
		AfxGetApp()->m_pMainWnd->PostMessage(WM_CLOSE);
		return;
    }
	if (ado_open_ok)	p_cp_data = new CPData(pConnection);

#ifdef SYS_SHOW_TEST
	//((CMainFrame*)AfxGetApp()->m_pMainWnd)->ng_record.select_DB_for_list("","",0,0,&m_main_list, 0);
	//CString sql_string = "select TOP 10 * from TianYouShou ORDER BY 日期 DESC";
	//GetDlgItem(IDC_LIST2)->SendMessage(WM_VSCROLL,SB_BOTTOM,0);
#endif

#if 0//def _DEBUG	
	p_cp_data->Execute("Insert into TianYouShou values ('2018/01/04','大叶富士华割草机','0x00112200','电流', '28','刘芳芳')");
	p_cp_data->Execute("delete from TianYouShou");

	for (int i = 0; i < 100; i++)
		p_cp_data->Execute(ng_debug_data_record[0]);
#endif

#endif// DATA_BACE

	//3， Socket套接字初始化
#ifdef MGT_SOCKET
	InitServerSocket();
#ifdef SYS_THREAD_POOL
	if (thread_pool_init() == FALSE) {
		MessageBox("thread_pool_init error!!!");
		ASSERT(0);
	}
#endif
#endif
	
#ifdef MGT_SOCKET_CLIENT
	qq_client_init();
#else
#endif

	

#ifdef MGT_CLOCK
	// 5， 其他初始化

#if  !defined(MGT_SOCKET) || defined(_DEBUG)
	// 隐藏控件
	GetDlgItem(IDC_EDIT3)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON3)->ShowWindow(FALSE);
#endif
#if	 !defined(MGT_SOCKET_CLIENT) || defined(_DEBUG)
	// 隐藏控件
	GetDlgItem(IDC_EDIT4)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON4)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON5)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
#endif
#endif

	SetTimer(2, 1000, NULL);
#ifdef MGT_SOCKET_CLIENT
	CWinThread* pThread = AfxBeginThread(SocketConnectProc, this);
#endif

#ifdef AUTO_TOOL_18V_TEST
	// 初测/成测 单选框
	CButton* radio=(CButton*)GetDlgItem(IDC_RADIO_CHUCE);
	radio->SetCheck(1);
#else
	GetDlgItem(IDC_RADIO_CHUCE)->ShowWindow(FALSE);
	GetDlgItem(IDC_RADIO_CHENGCE)->ShowWindow(FALSE);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CMenuTestView printing

BOOL CMenuTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMenuTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMenuTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMenuTestView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CMenuTestView diagnostics

#ifdef _DEBUG
void CMenuTestView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMenuTestView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CMenuTestDoc* CMenuTestView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMenuTestDoc)));
	return (CMenuTestDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMenuTestView message handlers


// 配置信息下发到测试板
//int fgDeliver = 0;

void CMenuTestView::OnButtonToTestBoard() 
{/*
	// TODO: Add your control notification handler code here
	CString file ;//= "F:\\mylove.exl";
	static int k = 0;
	file.Format("F:\\mylove%d.exl", k++);
	((CMainFrame*)AfxGetMainWnd())->OnCommand(0,(long)(&file));		//  add file histroy function
*/
	SetEvent(MyCom.m_handle);       //触发事件
}

//void CMenuTestView::OnSelectType() 
//{
	// TODO: Add your control notification handler code here
	
//}

void CMenuTestView::OnSelchangeType() 
{
	// TODO: Add your control notification handler code here
	int sel = m_TestType.GetCurSel();
}

void CMenuTestView::OnReSetting() 
{
	// TODO: Add your control notification handler code here
	CString buf = "";
	m_CurrProj.GetWindowText(buf);
	if (buf != "") {
		((CMainFrame*)AfxGetMainWnd())->new_dlg.DoModal();
	}
}


#ifdef MGT_SOCKET

int socket_num = 0;
int rev_test_num = 0;
int state_listen = 0;

SOCKET* GetFreeSocket()
{
	for (int i = 0; i < MAX_USER_NUM; i++) {
		if ((int)sClients[i] == 0) {
			socket_num++;
			return &sClients[i];
		}
	}
	return NULL;
}

void DeleteTheSocket(SOCKET *pTheSocket)
{
	*pTheSocket = 0;
	socket_num --;
	ASSERT(socket_num >= 0);
}

BOOL Socket_not_full()
{
	if (socket_num < MAX_USER_NUM) return TRUE;
	else return FALSE;
}
int CMenuTestView::socket_bind_listen()
{
	int retVal;

	//创建套接字
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);               
	if(INVALID_SOCKET == sServer) { 
		::SendMessage(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_wndStatusBar.m_hWnd,SB_SETTEXT,0,(LPARAM)("socket failed！"));// 改用状态栏提示                                 
		return 0;                                                    
	}                                                                   
	//服务器套接字地址                                                 
	addrServ.sin_family = AF_INET;                                     
	addrServ.sin_port = htons(4999);                                   
	addrServ.sin_addr.s_addr = INADDR_ANY; 

	//支持socket复用
	BOOL bReuseaddr = TRUE;
	setsockopt(sServer,SOL_SOCKET ,SO_REUSEADDR,(const char*)&bReuseaddr,sizeof(BOOL));
	//绑定套接字                                                       
	retVal = bind(sServer, (LPSOCKADDR)&addrServ, sizeof(SOCKADDR_IN));
	if(SOCKET_ERROR == retVal) {
		::SendMessage(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_wndStatusBar.m_hWnd,SB_SETTEXT,0,(LPARAM)("bind failed！"));// 改用状态栏提示
		closesocket(sServer);   //关闭套接字                   
		return 0;                                                     
	}
	// 设置接收/发送缓冲区大小
	int nRecvBuf=30*1024;//设置为10K
	setsockopt(sServer,SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int));
	int nSendBuf=30*1024;//设置为10K
	setsockopt(sServer,SOL_SOCKET,SO_SNDBUF,(const char*)&nSendBuf,sizeof(int));
	//开始监听                                                               
	retVal = listen(sServer, 20);                                             
	if(SOCKET_ERROR == retVal){
		closesocket(sServer);   //关闭套接字 
		::SendMessage(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_wndStatusBar.m_hWnd,SB_SETTEXT,0,(LPARAM)("listen failed！"));// 改用状态栏提示
        return 0;                                                           
	}else {
		::SendMessage(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_wndStatusBar.m_hWnd,SB_SETTEXT,0,(LPARAM)("服务器已准备就绪！"));// 改用状态栏提示
	}
	return 1;
}

UINT SocketListenProc(LPVOID pParam)
{
	WSADATA         wsd;            //WSADATA变量
	SOCKET			*pSocket = NULL;
	CMenuTestView* pView = (CMenuTestView*)pParam;

	//初始化套结字动态库                                               
	if (WSAStartup(MAKEWORD(2,2), &wsd) != 0) {
		::SendMessage(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_wndStatusBar.m_hWnd,SB_SETTEXT,0,(LPARAM)("WSAStartup failed！"));// 改用状态栏提示
		return 0;                                                      
	}                                                                  
	while(1)
	{	 
		if (Socket_not_full()) {
			// listen ...
			if (0 == state_listen) {
				if (pView->socket_bind_listen() == 0)
					return 0;
				state_listen = 1;	// 监听成功
			}
			//接受客户端请求                                                         
			sockaddr_in addrClient;
			int addrClientlen = sizeof(addrClient);
			SOCKET socket_temp = accept(sServer,(sockaddr FAR*)&addrClient, &addrClientlen);
			if(INVALID_SOCKET == socket_temp) { 
				continue;
			}
			pSocket = GetFreeSocket();
			*pSocket = socket_temp;
			// 创建监听线程
#ifdef SYS_THREAD_POOL
			CTaskBase* pTask = CreateTasks(tasks, 1, (void*)pSocket);	// 添加线程池任务
			if (pTask != NULL) {
                pool.QueueRequest( (CMyWorker::RequestType) pTask );
			}
#else
			CWinThread* pThread = AfxBeginThread(FuncToClient, (LPVOID)pSocket);
#endif
		}else {
			if (state_listen) {
				closesocket(sServer);   //关闭套接字
				state_listen = 0;
			}
		}
		Sleep(20);
	}
	
	return 1;
}

UINT FuncToClient(LPVOID pParam)
{
	char            *buf;			//接收数据缓冲区
	int             retVal;         //返回值
	SOCKET			sClient = *(SOCKET *)pParam;
	int				beeting = 0;
	SYSTEMTIME		st_last;

	buf = (char *)malloc(BUF_SIZE);
	if (buf == NULL) return 0;

	GetLocalTime(&st_last);	// 初始化 初始时间

	int nNetTimeout=1000;//1秒，
	//设置发送超时
	setsockopt(sClient,SOL_SOCKET,SO_SNDTIMEO, (char *)&nNetTimeout,sizeof(int));
	//设置接收超时
	setsockopt(sClient,SOL_SOCKET,SO_RCVTIMEO, (char *)&nNetTimeout,sizeof(int));

	while(true){
		//接收客户端数据                                                     
		ZeroMemory(buf, BUF_SIZE);
		retVal = recv(sClient, buf, BUF_SIZE, 0);
		int err_no = WSAGetLastError();
		if(retVal < 0 && err_no == WSAETIMEDOUT /*(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)*/) {//这几种错误码，认为连接是正常的，继续接收
			//继续接收数据
			if (++beeting >= 20) {
				break;	// 无响应20s表示客户端已经异常断线
			}
			continue;
		}else if(SOCKET_ERROR == retVal || 0 == retVal) {
			// 客户端已断开
			break;
		}else if (buf != ""){
			// 正常收到数据
			CString rev(buf);
			rev.Replace(beat_string, "");
			if (rev == "") { // 收到心跳包 正常每秒一次收到
				beeting = 0;
			}else {
				GetLocalTime(&st_last);	// 记录最后一次数据收到时间
				//cout << "客户端发送的数据: " << buf <<endl; 
				pCurrView->GetDlgItem(IDC_EDIT3)->SetWindowText(rev);
				//CString len;len.Format("%d\r\n", retVal);
				//TRACE(len);
				// 存入数据库     
#ifdef DATA_BACE
				p_cp_data->Execute(rev);
#endif	 
#if defined(SYS_SHOW_TEST) && defined(SYS_SERVER)		// 演示用
				pCurrView->disp_current_test_info_line(rev);
#endif
		
				// 5，更新状态栏测试数量
				rev_test_num++;
				CString str_num = "";
				str_num.Format("当前测试数量：%d", rev_test_num);
				::SendMessage(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_wndStatusBar.m_hWnd,SB_SETTEXT,4,(LPARAM)str_num.GetBuffer());// 改用状态栏提示
			}
			// 超时无数据关闭socket
			if (Socket_not_full() == FALSE) { // SOCKET已饱和情况下
				SYSTEMTIME	st;
				GetLocalTime(&st);
				if (st.wHour < st_last.wHour) st.wHour += 24;	// 第二天
				if ((st.wHour - st_last.wHour) * 60 + (st.wMinute - st_last.wMinute) >= 180)	{	// 按分钟数计算
					TRACE("超时！");
					break;	// 关闭socket
				}
			}
		}
	}                                                                        
	      
	//退出线程                                                                                                      
	closesocket(sClient);   //关闭套接字                                     
	//WSACleanup();         //释放套接字资源;                                
	DeleteTheSocket((SOCKET*)pParam);
	free(buf);

	return 1;  
}
int CMenuTestView::InitServerSocket()
{
	// 初始化Socket集合
	ZeroMemory(sClients, MAX_USER_NUM * sizeof(SOCKET));
	// 创建服务器主监听线程
	CWinThread* pThread = AfxBeginThread(SocketListenProc, this);
	return 1;
}


// 服务器发送
void CMenuTestView::OnBnqqSend()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CString server_send;
	m_Sever_Send.GetWindowText(server_send);
	//USES_CONVERSION;
	char* sendbuf=(char*)server_send.GetBuffer(server_send.GetLength());
	for (int i = 0; i < MAX_USER_NUM; i++) {
		if ((int)sClients[i] != 0) {
			// 阻塞发送，直至网络发送完毕返回 或者超时
			send(sClients[i], sendbuf, (int)strlen(sendbuf), 0);
			// 发送返回按段 
			/* if (超时返回次数 > n次) {
				判定为通信链路异常，关闭通信链路Socket.
			}
			*/
		}
	}
}


#endif

#ifdef MGT_SOCKET_CLIENT

extern CString IP;
extern CString Port;

BOOL CMenuTestView::qq_client_init()
{                                                    
	//初始化套结字动态库                                
	if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)           
	{                                                   
		//cout << "WSAStartup failed!" << endl;
		MessageBox(_T("WSAStartup failed!"), _T("TRACE"), MB_OK);
		return FALSE;                                      
	}
	return TRUE;
}

int fg_client_socket_conect_ok = 0;
BOOL CMenuTestView::socket_client_connect()
{
	int	retVal;
	//创建套接字                                        
	sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  
	if(INVALID_SOCKET == sHost) {
		fg_client_socket_conect_ok = 0;
		MessageBox(_T("create socket failed!"), _T("TRACE"), MB_OK);                   
		return FALSE;                                     
	}                                                   
	//设置服务器地址                                    
	servAddr.sin_family =AF_INET;                       
	servAddr.sin_addr.s_addr = inet_addr(IP);  
	servAddr.sin_port = htons((short)atoi(Port));             
	int nServAddlen  = sizeof(servAddr);
	//// 设置接收/发送缓冲区大小
	int nRecvBuf=30*1024;//设置为10K
	setsockopt(sHost,SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int));
	int nSendBuf=30*1024;//设置为10K
	setsockopt(sHost,SOL_SOCKET,SO_SNDBUF,(const char*)&nSendBuf,sizeof(int));
	// 设置发送/接收超时
	int nNetTimeout=1000;//1秒，
	//setsockopt(sHost,SOL_SOCKET,SO_SNDTIMEO, (char *)&nNetTimeout,sizeof(int));
	setsockopt(sHost,SOL_SOCKET,SO_RCVTIMEO, (char *)&nNetTimeout,sizeof(int));
	
	//连接服务器   
    retVal=connect(sHost,(LPSOCKADDR)&servAddr, sizeof(servAddr));    
    if(SOCKET_ERROR == retVal) {
		::SendMessage(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_wndStatusBar.m_hWnd,SB_SETTEXT,0,(LPARAM)("远程服务器连接失败！"));// 改用状态栏提示
        closesocket(sHost);		//关闭套接字   
		fg_client_socket_conect_ok = 0;		// 1,开启本地数据库存储；2，启动定时连接服务器模式
		return FALSE;
	}
	//GetDlgItem(IDC_EDIT4)->SetWindowText(_T("connect ok !"));
	::SendMessage(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_wndStatusBar.m_hWnd,SB_SETTEXT,0,(LPARAM)("远程服务器连接成功！"));// 改用状态栏提示
	// 置连接OK标志
	fg_client_socket_conect_ok = 1;
	// 设定心跳包定时器
	SetTimer(3, 1000, NULL);
	// 创建接收线程
	CWinThread* pThread = AfxBeginThread(SocketClientRevProc, this);
	GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
	return TRUE;
}

UINT SocketConnectProc(LPVOID pParam)
{
	CMenuTestView* pView = (CMenuTestView*)pParam;
	while(1) {
		if (0 == fg_client_socket_conect_ok) {
			TRACE("connect ...\r\n");
			if (pView->socket_client_connect() == FALSE) 
				;//return;		//conect ...
		}
		Sleep(20);
	}
	return 1;
}
void CMenuTestView::OnBnClickedButton2()   // connect
{
	// TODO: 在此添加控件通知处理程序代码
	//socket_conect();
}


UINT SocketClientRevProc(LPVOID pParam)
{
	CMenuTestView*		pView = (CMenuTestView*)pParam;
	
	while(1) {
		int retVal = recv(sHost, bufRecv,BUF_SIZE , 0);     // 接收服务器端的数据 
		int err_no = WSAGetLastError();
		if(retVal < 0 && err_no == WSAETIMEDOUT /*(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)*/) {//这几种错误码，认为连接是正常的，继续接收
			continue;
		}else if(SOCKET_ERROR == retVal || 0 == retVal/*连接已终止*/) {
			closesocket(sHost);								//关闭套接字   
			//WSACleanup();									//释放套接字资源
			pView->KillTimer(3);
			Sleep(100); //针对空闲占用情况，让道不抢
			fg_client_socket_conect_ok = 0;					// 置网络中断标志，1，启动本地数据库保存；2，开启定时连接服务器模式
			return 0;
		}else {
			bufRecv[retVal] = 0;
			CString client_rev(bufRecv);
			pView->GetDlgItem(IDC_EDIT4)->SetWindowText(client_rev);
		}
	}
}

BOOL CMenuTestView::client_send(CString buf)		// 参数应该是： (char *, int len)
{
	int	retVal;         //返回值
	char* sendbuf=(char*)buf.GetBuffer(buf.GetLength());
	retVal = send(sHost, sendbuf, strlen(sendbuf), 0);  
	if (SOCKET_ERROR == retVal || 0 == retVal/*连接已终止*/)  
	{  
	   //cout << "send failed!" << endl; 
	   MessageBox(_T("send failed!"), _T("TRACE"), MB_OK);
	   closesocket(sHost); //关闭套接字   
	   //WSACleanup();       //释放套接字资源  
	   return FALSE;
	} 
	return TRUE;
}
void CMenuTestView::OnBnClickedButton1()	// send
{
	// TODO: 在此添加控件通知处理程序代码
	//向服务器发送数据   
   ZeroMemory(buf, BUF_SIZE);  
   //cout << " 向服务器发送数据:  ";  
   //cin >> buf;
	CString str_client;
	GetDlgItem(IDC_EDIT4)->GetWindowText(str_client);
	//USES_CONVERSION;
 
	client_send(str_client);
}

#endif
extern void SaveHistroyFile();
void CMenuTestView::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CFormView::OnClose();
}
CRect Rect(500,70,750,240);
//CString moni_string = "yqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqy";  // 测试用
void CMenuTestView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent) 
	{
	case 2:
		//TRACE("ok\r\n");
#ifdef MGT_SOCKET_CLIENT
//		client_send(moni_string);	// 测试用
#endif
#ifdef MGT_CLOCK		
		InvalidateRect(&Rect,true);
		UpdateWindow();
#endif
		break;
	case 3:
#ifdef MGT_SOCKET_CLIENT
		client_send(beat_string);  // 发送心跳包，每秒钟
#endif
		break;
	default: break;
	}
	CFormView::OnTimer(nIDEvent);
}

void CMenuTestView::OnDestroy()
{
	CFormView::OnDestroy();
	// TODO: 在此处添加消息处理程序代码
	SaveHistroyFile();
#ifdef LOCAL_EXCEL_SAVE
	local_excel_file.Close();
#endif

#ifdef DATA_BACE
	delete p_cp_data;
#endif
}

void CMenuTestView::re_build_main_list(int l_total_item_num, int list_control_type)
{
	CListCtrl_Sortable *plist_control;
	if (list_control_type == MAIN_LIST) {
		plist_control = &m_main_list;
	}else if (list_control_type == NG_LIST) {
		plist_control = &(((CMainFrame*)AfxGetApp()->m_pMainWnd)->ng_record.m_ng_list);
	}

	// 删除所有行
	plist_control->DeleteAllItems();
	// 删除所有列
	while(plist_control->DeleteColumn(0));
	// 重新构造列
	plist_control->InsertColumn(0, "序号", LVCFMT_LEFT,50);
    plist_control->InsertColumn(1, "日期", LVCFMT_LEFT,150);
	plist_control->InsertColumn(2, "产品名", LVCFMT_LEFT,200);
    plist_control->InsertColumn(3, "扫码ID", LVCFMT_LEFT,150);

	for (int i = 0; i < l_total_item_num; i++) {
		CString strtest_item_n;
		strtest_item_n.Format("测试项%d", i+1);
		plist_control->InsertColumn(FIRST_COL_NUM + i*2, strtest_item_n, LVCFMT_LEFT,150);
		plist_control->InsertColumn(FIRST_COL_NUM + 1 + i*2, "测试值", LVCFMT_LEFT,150);
	}

    plist_control->InsertColumn(l_total_item_num*2 + FIRST_COL_NUM, "不良类型", LVCFMT_LEFT,150);
    plist_control->InsertColumn(l_total_item_num*2 + FIRST_COL_NUM+1, "不良值", LVCFMT_LEFT,100); 
	plist_control->InsertColumn(l_total_item_num*2 + FIRST_COL_NUM+2, "操作员", LVCFMT_LEFT,100);
	plist_control->InsertColumn(l_total_item_num*2 + FIRST_COL_NUM+3, "加工厂", LVCFMT_LEFT,100);
	plist_control->InsertColumn(l_total_item_num*2 + FIRST_COL_NUM+4, "其他", LVCFMT_LEFT,100);
	// 注： 测试条数 不显示
}

void CMenuTestView::disp_current_test_info_line(CString sql_string)
{
	// ...解析字符串，并添加到主界面列表中显示
	// 例如：string == '车王角磨.','(null)','MCU引脚10信号','1','刘芳芳','',''
	int index1 = 0,index2 = 0, j = 1;CString str = "";
	int iRow = pCurrView->m_main_list.GetItemCount();
	//序号
	str.Format("%d", iRow);
	pCurrView->m_main_list.InsertItem(iRow, str);
	// 日期
	//str = sql_string.Mid(sql_string.Find("'") +1, 8);
	//str.Insert(4,"/");
	//str.Insert(7,"/");
	pCurrView->m_main_list.SetItemText(iRow, 1,str);
#ifdef LOCAL_EXCEL_SAVE
	local_excel_file.WriteString("\n");//换行
#endif
	while (1) {
		index1 = sql_string.Find("'",index1);
		if (index1 == -1) break;
		index1 = index1 + 1;
		index2 = sql_string.Find("'",index1);
		if (index2 != -1) {
			str = sql_string.Mid(index1, index2 - index1);
			pCurrView->m_main_list.SetItemText(iRow, j,str);
#ifdef LOCAL_EXCEL_SAVE
			str.Remove('\r');str.Remove('\n');
			local_excel_file.WriteString(str + "\t");//写入内容,并换单元格
#endif
		}else break;
		index1 = index2 + 1;
		if (index1 >= sql_string.GetLength()) break;
		if (str != "") j++;	// 过滤item兼容字段
	}
	pCurrView->GetDlgItem(IDC_LIST2)->SendMessage(WM_VSCROLL,SB_BOTTOM,0);
}

CRect Rect_pic_ok; //(800,350,876,424);
int fg_test_result_ok = 0;
void CMenuTestView::OnPaint()
{   // 以下代码在盛斌工作电脑上出现死机，自己笔记本是OK的，证明不兼容。
/*	CImage image;
	//CPaintDC dc(this);        
    if(!image.IsNull()) image.Destroy();
	if (1 == fg_test_result_ok)
		image.Load("res/ok2.png");
	else if (2 == fg_test_result_ok)
		image.Load("res/fail2.png");
    //if(!image.IsNull()) img.Draw(dc.m_hDC, Rect_pic_ok.left, Rect_pic_ok.top);

	GetDlgItem(IDC_PIC_XY)->GetClientRect(&Rect_pic_ok);	//获得pictrue控件所在的矩形区域 
	if (fg_test_result_ok != 0) {
		int cx = image.GetWidth();							//获取图片宽度   
		int cy = image.GetHeight();							//获取图片高度   
		GetDlgItem(IDC_PIC_XY)->GetWindowRect(&Rect_pic_ok);//将窗口矩形选中到picture控件上   
		ScreenToClient(&Rect_pic_ok);						//将客户区选中到Picture控件表示的矩形区域内   
		GetDlgItem(IDC_PIC_XY)->MoveWindow(Rect_pic_ok.left, Rect_pic_ok.top, cx, cy, TRUE);//将窗口移动到Picture控件表示的矩形区域   
		CWnd *pWnd=GetDlgItem(IDC_PIC_XY);					//获得pictrue控件窗口的句柄   
		pWnd->GetClientRect(&Rect_pic_ok);					//获得pictrue控件所在的矩形区域   
		CDC *pDC=pWnd->GetDC();								//获得pictrue控件的DC   
		image.Draw(pDC->m_hDC, Rect_pic_ok);				//将图片画到Picture控件表示的矩形区域   
		ReleaseDC(pDC);										//释放picture控件的DC
		TRACE("draw!\r\n");
	}
*/
#ifdef MGT_CLOCK
	CPaintDC dc(this); // device context for painting
	CPaintDC *pDC = &dc;
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CFormView::OnPaint()
	//获取客户区
	 //RECT Rect;
	 //GetClientRect(&Rect);
	 //计算椭圆中心位置
	 int CenterX=(Rect.right - Rect.left)/2;
	 int CenterY=(Rect.bottom - Rect.top)/2;
	 //取当前时间
	 CTime Time=CTime::GetCurrentTime();
	 CString str;
	 int i,x,y;
	 CSize size;
	 //创建一支黄色的笔，用来画椭圆
	 CPen Pen(PS_SOLID,3,RGB(0,0,0));
	 //设置当前画笔，并记下以前的画笔
	 CPen *OldPen=pDC->SelectObject(&Pen);
	 //绘制钟面椭圆
	 pDC->Ellipse(&Rect);
	 CBrush brush(RGB(240,240,240));//根据自己需要填充颜色
	 CBrush *oldbrush;
	 oldbrush = pDC->SelectObject(&brush);//选新的画刷
	 pDC->Ellipse(&Rect);
	 pDC->SelectObject(oldbrush);
	 double Radians;
	 //设置字体颜色为红色
	 pDC->SetTextColor(RGB(255,0,0));
	 for(i=1;i<=12;i++)
	 {
	  //格式化钟点值
	  str.Format("%d",i);
	  size=pDC->GetTextExtent(str,str.GetLength());
	  Radians=(double)i*6.28/12.0;
	  //计算钟点放置的位置
	  x=CenterX-(size.cx/2)+(int)((double)(CenterX-20)*sin(Radians))+Rect.left;
	  y=CenterY-(size.cy/2)-(int)((double)(CenterY-20)*cos(Radians)) + Rect.top;
	  //绘制钟点
	  pDC->TextOut(x,y,str);
	 }
	 //计算时钟指针的夹角
	 Radians=(double)Time.GetHour()+(double)Time.GetMinute()/60.0+(double)Time.GetSecond()/3600.0;
	 Radians *=6.28/12.0;
	 //创建时钟指针画笔
	 CPen HourPen(PS_SOLID,5,RGB(255,0,0));
	 pDC->SelectObject(&HourPen);
	 //绘制时钟指针
	 pDC->MoveTo(CenterX+Rect.left,CenterY+ Rect.top);
	 pDC->LineTo(CenterX+Rect.left+(int)((double)(CenterX/3)*sin(Radians)),CenterY+ Rect.top-(int)((double)(CenterY/3)*cos(Radians)));
	 Radians=(double)Time.GetMinute()+(double)Time.GetSecond()/60.0;
	 Radians *=6.28/60.0;
	 //创建分钟指针画笔
	 CPen MinutePen(PS_SOLID,3,RGB(0,0,255));
	 pDC->SelectObject(&MinutePen);
	 //绘制分钟指针
	 pDC->MoveTo(CenterX+Rect.left,CenterY+ Rect.top);
	 pDC->LineTo((int)(CenterX+Rect.left+(int)((double)(CenterX*2)/3)*sin(Radians)),(int)(CenterY+ Rect.top-(int)((double)(CenterY*2/3)*cos(Radians))));
	 Radians=(double)Time.GetSecond();
	 Radians *=6.28/60.0;
	 //创建秒钟指针画笔
	 CPen SecondPen(PS_SOLID,1,RGB(0,255,0));
	 pDC->SelectObject(&SecondPen);
	 //绘制分钟指针
	 pDC->MoveTo(CenterX+Rect.left,CenterY+ Rect.top);
	 pDC->LineTo((int)(CenterX+Rect.left+(int)((double)(CenterX*4)/5)*sin(Radians)),(int)(CenterY+ Rect.top-(int)((double)(CenterY*4/5)*cos(Radians))));
	 pDC->SelectObject(OldPen);
#endif

	CFormView::OnPaint();
}

#ifdef SYS_THREAD_POOL

#define THREADPOOL_SIZE				MAX_USER_NUM	// 线程数和最大连接数 相等
#define SOCKET_THREAD_STASK_SIZE	(64 * 1024)	// 线程栈大小 128K
//#define TATOL_TAST_NUM	20

BOOL thread_pool_init()
{
    HRESULT hr = pool.Initialize((void*)321, THREADPOOL_SIZE, SOCKET_THREAD_STASK_SIZE);
    if( !SUCCEEDED( hr ) ) return FALSE;
	return TRUE;
}

// 暂未调用 （进程结束自动收回资源）
void ClearPoolAndTask()
{
	CTaskBase* pTask = NULL;
	// Clean up the tasks and shutdown the thread pool
    for (int i = 0; i < tasks.GetSize(); i++ ) {
        pTask = tasks[ i ];
        ATLASSERT( NULL != pTask );
        delete pTask;
    }
 
    // Shutdown the thread pool
    pool.Shutdown();
}
#endif

extern int fg_chengce;
void CMenuTestView::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	fg_chengce = 0;
	//TRACE("0\r\n");
}


void CMenuTestView::OnBnClickedRadioChengce()
{
	// TODO: 在此添加控件通知处理程序代码
	fg_chengce = 1;
	//TRACE("1\r\n");
}
