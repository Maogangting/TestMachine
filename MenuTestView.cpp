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

CStdioFile local_excel_file;		// �����ݿ�����µĲ��Լ�¼����EXCEL�ļ�����

#ifdef _DEBUG
CString ng_debug_data_record[1] = 
{"Insert into TianYouShou values ('20180108 12:18:32','����M370','0x00112200','����������ѹ','2180 mv','����ת��', '3100','����', '20.3','Ƿѹ', 'OK','NTC����', 'OK', '����', '20.3','������','','')"
};
#endif

#ifdef MGT_SOCKET 


extern UINT SocketListenProc(LPVOID pParam);
extern UINT FuncToClient(LPVOID pParam);
//static	int		client_num = 0;


#define BUF_SIZE 1024
SOCKET          sServer;        //�������׽���
SOCKET          sClients[MAX_USER_NUM];  //�ͻ����׽��� ,���֧��200���ͻ�������                    
SOCKADDR_IN     addrServ;      //��������ַ 

#endif

#ifdef MGT_SOCKET_CLIENT
#ifndef MGT_SOCKET
#define			BUF_SIZE 1024
#endif
WSADATA         wsd;            //WSADATA����       
SOCKET          sHost;          //�������׽���      
SOCKADDR_IN     servAddr;       //��������ַ        
char            buf[BUF_SIZE];  //�������ݻ�����    
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
// ϵͳ��ʼ���ܺ���
void CMenuTestView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

//	send_program_code(0);		// debug
	// 0�� �����ʼ��
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
	   "΢���ź�");                 // lpszFacename
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
	   "΢���ź�");                 // lpszFacename
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
	   "����");                 // lpszFacename
	m_topc_disp.SetFont(&m_font3);
	GetDlgItem(IDC_LIST2)->SetFont(&m_font3);

	//1�� �ļ����ݿ��ʼ�����ļ���ȡ��ʷ��¼
	m_TestType.SetCurSel(0);	// Ĭ��԰�ֹ���
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
		
	CStdioFile File;
	CString strTemp = "";
	if(File.Open(HIS_FILE,CStdioFile::modeRead))//����/���ļ�
	{
		while(!feof(File.m_pStream))
		{
			File.ReadString(strTemp);strTemp.Remove('\r');
			((CMainFrame*)AfxGetMainWnd())->OnCommand(0,(long)(&strTemp));
		}
	}

#ifdef LOCAL_EXCEL_SAVE
	if(local_excel_file.Open(LOCAL_EXCEL_FILE,CStdioFile::modeCreate|CStdioFile::modeNoTruncate|CStdioFile::modeWrite)) {//����/���ļ�
		local_excel_file.SeekToEnd();	// ׷��
	}
#endif

	// 2�����ڳ�ʼ��  ���ⲿ�ֳ�ʼ���ŵ���������λ�ã�
	if (MyCom.uart_init() == FALSE) {
		AfxMessageBox("Uart init ERR !");
		//return;
	}

	

	// ��ʼ����������Լ�¼�б�
	DWORD dwStyle = m_main_list.GetExtendedStyle();     
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
	//dwStyle |= LVS_REPORT;
    m_main_list.SetExtendedStyle(dwStyle);
	//total_item_num = 6;
	re_build_main_list(total_item_num, MAIN_LIST);


	// 3, ���ݿ��ʼ��
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
			//pConnection->put_ConnectionTimeout(1);		// ���ó�ʱʱ��
            hr = pConnection->Open(_bstr_t(strSRC),"","",adModeUnknown);
			if(FAILED(hr))
			{
				AfxMessageBox("�����ݿ�ʧ�ܣ�");
				pConnection->Close();
				AfxGetApp()->m_pMainWnd->PostMessage(WM_CLOSE);
				return ;
			}
        }else {
			AfxMessageBox("���ݿ�����ʧ�ܣ�");
			AfxGetApp()->m_pMainWnd->PostMessage(WM_CLOSE);
			return;
		}
    }
 
    catch(_com_error e)    //��׽�쳣     
    {
        CString errormessage;
        errormessage.Format("�������ݿ�ʧ��!\r\n������Ϣ:%s",e.ErrorMessage());
        AfxMessageBox(errormessage);//��ʾ������Ϣ
		ado_open_ok = 0;
		AfxGetApp()->m_pMainWnd->PostMessage(WM_CLOSE);
		return;
    }
	if (ado_open_ok)	p_cp_data = new CPData(pConnection);

#ifdef SYS_SHOW_TEST
	//((CMainFrame*)AfxGetApp()->m_pMainWnd)->ng_record.select_DB_for_list("","",0,0,&m_main_list, 0);
	//CString sql_string = "select TOP 10 * from TianYouShou ORDER BY ���� DESC";
	//GetDlgItem(IDC_LIST2)->SendMessage(WM_VSCROLL,SB_BOTTOM,0);
#endif

#if 0//def _DEBUG	
	p_cp_data->Execute("Insert into TianYouShou values ('2018/01/04','��Ҷ��ʿ����ݻ�','0x00112200','����', '28','������')");
	p_cp_data->Execute("delete from TianYouShou");

	for (int i = 0; i < 100; i++)
		p_cp_data->Execute(ng_debug_data_record[0]);
#endif

#endif// DATA_BACE

	//3�� Socket�׽��ֳ�ʼ��
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
	// 5�� ������ʼ��

#if  !defined(MGT_SOCKET) || defined(_DEBUG)
	// ���ؿؼ�
	GetDlgItem(IDC_EDIT3)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON3)->ShowWindow(FALSE);
#endif
#if	 !defined(MGT_SOCKET_CLIENT) || defined(_DEBUG)
	// ���ؿؼ�
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
	// ����/�ɲ� ��ѡ��
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


// ������Ϣ�·������԰�
//int fgDeliver = 0;

void CMenuTestView::OnButtonToTestBoard() 
{/*
	// TODO: Add your control notification handler code here
	CString file ;//= "F:\\mylove.exl";
	static int k = 0;
	file.Format("F:\\mylove%d.exl", k++);
	((CMainFrame*)AfxGetMainWnd())->OnCommand(0,(long)(&file));		//  add file histroy function
*/
	SetEvent(MyCom.m_handle);       //�����¼�
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

	//�����׽���
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);               
	if(INVALID_SOCKET == sServer) { 
		::SendMessage(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_wndStatusBar.m_hWnd,SB_SETTEXT,0,(LPARAM)("socket failed��"));// ����״̬����ʾ                                 
		return 0;                                                    
	}                                                                   
	//�������׽��ֵ�ַ                                                 
	addrServ.sin_family = AF_INET;                                     
	addrServ.sin_port = htons(4999);                                   
	addrServ.sin_addr.s_addr = INADDR_ANY; 

	//֧��socket����
	BOOL bReuseaddr = TRUE;
	setsockopt(sServer,SOL_SOCKET ,SO_REUSEADDR,(const char*)&bReuseaddr,sizeof(BOOL));
	//���׽���                                                       
	retVal = bind(sServer, (LPSOCKADDR)&addrServ, sizeof(SOCKADDR_IN));
	if(SOCKET_ERROR == retVal) {
		::SendMessage(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_wndStatusBar.m_hWnd,SB_SETTEXT,0,(LPARAM)("bind failed��"));// ����״̬����ʾ
		closesocket(sServer);   //�ر��׽���                   
		return 0;                                                     
	}
	// ���ý���/���ͻ�������С
	int nRecvBuf=30*1024;//����Ϊ10K
	setsockopt(sServer,SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int));
	int nSendBuf=30*1024;//����Ϊ10K
	setsockopt(sServer,SOL_SOCKET,SO_SNDBUF,(const char*)&nSendBuf,sizeof(int));
	//��ʼ����                                                               
	retVal = listen(sServer, 20);                                             
	if(SOCKET_ERROR == retVal){
		closesocket(sServer);   //�ر��׽��� 
		::SendMessage(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_wndStatusBar.m_hWnd,SB_SETTEXT,0,(LPARAM)("listen failed��"));// ����״̬����ʾ
        return 0;                                                           
	}else {
		::SendMessage(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_wndStatusBar.m_hWnd,SB_SETTEXT,0,(LPARAM)("��������׼��������"));// ����״̬����ʾ
	}
	return 1;
}

UINT SocketListenProc(LPVOID pParam)
{
	WSADATA         wsd;            //WSADATA����
	SOCKET			*pSocket = NULL;
	CMenuTestView* pView = (CMenuTestView*)pParam;

	//��ʼ���׽��ֶ�̬��                                               
	if (WSAStartup(MAKEWORD(2,2), &wsd) != 0) {
		::SendMessage(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_wndStatusBar.m_hWnd,SB_SETTEXT,0,(LPARAM)("WSAStartup failed��"));// ����״̬����ʾ
		return 0;                                                      
	}                                                                  
	while(1)
	{	 
		if (Socket_not_full()) {
			// listen ...
			if (0 == state_listen) {
				if (pView->socket_bind_listen() == 0)
					return 0;
				state_listen = 1;	// �����ɹ�
			}
			//���ܿͻ�������                                                         
			sockaddr_in addrClient;
			int addrClientlen = sizeof(addrClient);
			SOCKET socket_temp = accept(sServer,(sockaddr FAR*)&addrClient, &addrClientlen);
			if(INVALID_SOCKET == socket_temp) { 
				continue;
			}
			pSocket = GetFreeSocket();
			*pSocket = socket_temp;
			// ���������߳�
#ifdef SYS_THREAD_POOL
			CTaskBase* pTask = CreateTasks(tasks, 1, (void*)pSocket);	// ����̳߳�����
			if (pTask != NULL) {
                pool.QueueRequest( (CMyWorker::RequestType) pTask );
			}
#else
			CWinThread* pThread = AfxBeginThread(FuncToClient, (LPVOID)pSocket);
#endif
		}else {
			if (state_listen) {
				closesocket(sServer);   //�ر��׽���
				state_listen = 0;
			}
		}
		Sleep(20);
	}
	
	return 1;
}

UINT FuncToClient(LPVOID pParam)
{
	char            *buf;			//�������ݻ�����
	int             retVal;         //����ֵ
	SOCKET			sClient = *(SOCKET *)pParam;
	int				beeting = 0;
	SYSTEMTIME		st_last;

	buf = (char *)malloc(BUF_SIZE);
	if (buf == NULL) return 0;

	GetLocalTime(&st_last);	// ��ʼ�� ��ʼʱ��

	int nNetTimeout=1000;//1�룬
	//���÷��ͳ�ʱ
	setsockopt(sClient,SOL_SOCKET,SO_SNDTIMEO, (char *)&nNetTimeout,sizeof(int));
	//���ý��ճ�ʱ
	setsockopt(sClient,SOL_SOCKET,SO_RCVTIMEO, (char *)&nNetTimeout,sizeof(int));

	while(true){
		//���տͻ�������                                                     
		ZeroMemory(buf, BUF_SIZE);
		retVal = recv(sClient, buf, BUF_SIZE, 0);
		int err_no = WSAGetLastError();
		if(retVal < 0 && err_no == WSAETIMEDOUT /*(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)*/) {//�⼸�ִ����룬��Ϊ�����������ģ���������
			//������������
			if (++beeting >= 20) {
				break;	// ����Ӧ20s��ʾ�ͻ����Ѿ��쳣����
			}
			continue;
		}else if(SOCKET_ERROR == retVal || 0 == retVal) {
			// �ͻ����ѶϿ�
			break;
		}else if (buf != ""){
			// �����յ�����
			CString rev(buf);
			rev.Replace(beat_string, "");
			if (rev == "") { // �յ������� ����ÿ��һ���յ�
				beeting = 0;
			}else {
				GetLocalTime(&st_last);	// ��¼���һ�������յ�ʱ��
				//cout << "�ͻ��˷��͵�����: " << buf <<endl; 
				pCurrView->GetDlgItem(IDC_EDIT3)->SetWindowText(rev);
				//CString len;len.Format("%d\r\n", retVal);
				//TRACE(len);
				// �������ݿ�     
#ifdef DATA_BACE
				p_cp_data->Execute(rev);
#endif	 
#if defined(SYS_SHOW_TEST) && defined(SYS_SERVER)		// ��ʾ��
				pCurrView->disp_current_test_info_line(rev);
#endif
		
				// 5������״̬����������
				rev_test_num++;
				CString str_num = "";
				str_num.Format("��ǰ����������%d", rev_test_num);
				::SendMessage(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_wndStatusBar.m_hWnd,SB_SETTEXT,4,(LPARAM)str_num.GetBuffer());// ����״̬����ʾ
			}
			// ��ʱ�����ݹر�socket
			if (Socket_not_full() == FALSE) { // SOCKET�ѱ��������
				SYSTEMTIME	st;
				GetLocalTime(&st);
				if (st.wHour < st_last.wHour) st.wHour += 24;	// �ڶ���
				if ((st.wHour - st_last.wHour) * 60 + (st.wMinute - st_last.wMinute) >= 180)	{	// ������������
					TRACE("��ʱ��");
					break;	// �ر�socket
				}
			}
		}
	}                                                                        
	      
	//�˳��߳�                                                                                                      
	closesocket(sClient);   //�ر��׽���                                     
	//WSACleanup();         //�ͷ��׽�����Դ;                                
	DeleteTheSocket((SOCKET*)pParam);
	free(buf);

	return 1;  
}
int CMenuTestView::InitServerSocket()
{
	// ��ʼ��Socket����
	ZeroMemory(sClients, MAX_USER_NUM * sizeof(SOCKET));
	// �����������������߳�
	CWinThread* pThread = AfxBeginThread(SocketListenProc, this);
	return 1;
}


// ����������
void CMenuTestView::OnBnqqSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	CString server_send;
	m_Sever_Send.GetWindowText(server_send);
	//USES_CONVERSION;
	char* sendbuf=(char*)server_send.GetBuffer(server_send.GetLength());
	for (int i = 0; i < MAX_USER_NUM; i++) {
		if ((int)sClients[i] != 0) {
			// �������ͣ�ֱ�����緢����Ϸ��� ���߳�ʱ
			send(sClients[i], sendbuf, (int)strlen(sendbuf), 0);
			// ���ͷ��ذ��� 
			/* if (��ʱ���ش��� > n��) {
				�ж�Ϊͨ����·�쳣���ر�ͨ����·Socket.
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
	//��ʼ���׽��ֶ�̬��                                
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
	//�����׽���                                        
	sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  
	if(INVALID_SOCKET == sHost) {
		fg_client_socket_conect_ok = 0;
		MessageBox(_T("create socket failed!"), _T("TRACE"), MB_OK);                   
		return FALSE;                                     
	}                                                   
	//���÷�������ַ                                    
	servAddr.sin_family =AF_INET;                       
	servAddr.sin_addr.s_addr = inet_addr(IP);  
	servAddr.sin_port = htons((short)atoi(Port));             
	int nServAddlen  = sizeof(servAddr);
	//// ���ý���/���ͻ�������С
	int nRecvBuf=30*1024;//����Ϊ10K
	setsockopt(sHost,SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int));
	int nSendBuf=30*1024;//����Ϊ10K
	setsockopt(sHost,SOL_SOCKET,SO_SNDBUF,(const char*)&nSendBuf,sizeof(int));
	// ���÷���/���ճ�ʱ
	int nNetTimeout=1000;//1�룬
	//setsockopt(sHost,SOL_SOCKET,SO_SNDTIMEO, (char *)&nNetTimeout,sizeof(int));
	setsockopt(sHost,SOL_SOCKET,SO_RCVTIMEO, (char *)&nNetTimeout,sizeof(int));
	
	//���ӷ�����   
    retVal=connect(sHost,(LPSOCKADDR)&servAddr, sizeof(servAddr));    
    if(SOCKET_ERROR == retVal) {
		::SendMessage(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_wndStatusBar.m_hWnd,SB_SETTEXT,0,(LPARAM)("Զ�̷���������ʧ�ܣ�"));// ����״̬����ʾ
        closesocket(sHost);		//�ر��׽���   
		fg_client_socket_conect_ok = 0;		// 1,�����������ݿ�洢��2��������ʱ���ӷ�����ģʽ
		return FALSE;
	}
	//GetDlgItem(IDC_EDIT4)->SetWindowText(_T("connect ok !"));
	::SendMessage(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_wndStatusBar.m_hWnd,SB_SETTEXT,0,(LPARAM)("Զ�̷��������ӳɹ���"));// ����״̬����ʾ
	// ������OK��־
	fg_client_socket_conect_ok = 1;
	// �趨��������ʱ��
	SetTimer(3, 1000, NULL);
	// ���������߳�
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//socket_conect();
}


UINT SocketClientRevProc(LPVOID pParam)
{
	CMenuTestView*		pView = (CMenuTestView*)pParam;
	
	while(1) {
		int retVal = recv(sHost, bufRecv,BUF_SIZE , 0);     // ���շ������˵����� 
		int err_no = WSAGetLastError();
		if(retVal < 0 && err_no == WSAETIMEDOUT /*(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)*/) {//�⼸�ִ����룬��Ϊ�����������ģ���������
			continue;
		}else if(SOCKET_ERROR == retVal || 0 == retVal/*��������ֹ*/) {
			closesocket(sHost);								//�ر��׽���   
			//WSACleanup();									//�ͷ��׽�����Դ
			pView->KillTimer(3);
			Sleep(100); //��Կ���ռ��������õ�����
			fg_client_socket_conect_ok = 0;					// �������жϱ�־��1�������������ݿⱣ�棻2��������ʱ���ӷ�����ģʽ
			return 0;
		}else {
			bufRecv[retVal] = 0;
			CString client_rev(bufRecv);
			pView->GetDlgItem(IDC_EDIT4)->SetWindowText(client_rev);
		}
	}
}

BOOL CMenuTestView::client_send(CString buf)		// ����Ӧ���ǣ� (char *, int len)
{
	int	retVal;         //����ֵ
	char* sendbuf=(char*)buf.GetBuffer(buf.GetLength());
	retVal = send(sHost, sendbuf, strlen(sendbuf), 0);  
	if (SOCKET_ERROR == retVal || 0 == retVal/*��������ֹ*/)  
	{  
	   //cout << "send failed!" << endl; 
	   MessageBox(_T("send failed!"), _T("TRACE"), MB_OK);
	   closesocket(sHost); //�ر��׽���   
	   //WSACleanup();       //�ͷ��׽�����Դ  
	   return FALSE;
	} 
	return TRUE;
}
void CMenuTestView::OnBnClickedButton1()	// send
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���������������   
   ZeroMemory(buf, BUF_SIZE);  
   //cout << " ���������������:  ";  
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	CFormView::OnClose();
}
CRect Rect(500,70,750,240);
//CString moni_string = "yqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqy";  // ������
void CMenuTestView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nIDEvent) 
	{
	case 2:
		//TRACE("ok\r\n");
#ifdef MGT_SOCKET_CLIENT
//		client_send(moni_string);	// ������
#endif
#ifdef MGT_CLOCK		
		InvalidateRect(&Rect,true);
		UpdateWindow();
#endif
		break;
	case 3:
#ifdef MGT_SOCKET_CLIENT
		client_send(beat_string);  // ������������ÿ����
#endif
		break;
	default: break;
	}
	CFormView::OnTimer(nIDEvent);
}

void CMenuTestView::OnDestroy()
{
	CFormView::OnDestroy();
	// TODO: �ڴ˴������Ϣ����������
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

	// ɾ��������
	plist_control->DeleteAllItems();
	// ɾ��������
	while(plist_control->DeleteColumn(0));
	// ���¹�����
	plist_control->InsertColumn(0, "���", LVCFMT_LEFT,50);
    plist_control->InsertColumn(1, "����", LVCFMT_LEFT,150);
	plist_control->InsertColumn(2, "��Ʒ��", LVCFMT_LEFT,200);
    plist_control->InsertColumn(3, "ɨ��ID", LVCFMT_LEFT,150);

	for (int i = 0; i < l_total_item_num; i++) {
		CString strtest_item_n;
		strtest_item_n.Format("������%d", i+1);
		plist_control->InsertColumn(FIRST_COL_NUM + i*2, strtest_item_n, LVCFMT_LEFT,150);
		plist_control->InsertColumn(FIRST_COL_NUM + 1 + i*2, "����ֵ", LVCFMT_LEFT,150);
	}

    plist_control->InsertColumn(l_total_item_num*2 + FIRST_COL_NUM, "��������", LVCFMT_LEFT,150);
    plist_control->InsertColumn(l_total_item_num*2 + FIRST_COL_NUM+1, "����ֵ", LVCFMT_LEFT,100); 
	plist_control->InsertColumn(l_total_item_num*2 + FIRST_COL_NUM+2, "����Ա", LVCFMT_LEFT,100);
	plist_control->InsertColumn(l_total_item_num*2 + FIRST_COL_NUM+3, "�ӹ���", LVCFMT_LEFT,100);
	plist_control->InsertColumn(l_total_item_num*2 + FIRST_COL_NUM+4, "����", LVCFMT_LEFT,100);
	// ע�� �������� ����ʾ
}

void CMenuTestView::disp_current_test_info_line(CString sql_string)
{
	// ...�����ַ���������ӵ��������б�����ʾ
	// ���磺string == '������ĥ.','(null)','MCU����10�ź�','1','������','',''
	int index1 = 0,index2 = 0, j = 1;CString str = "";
	int iRow = pCurrView->m_main_list.GetItemCount();
	//���
	str.Format("%d", iRow);
	pCurrView->m_main_list.InsertItem(iRow, str);
	// ����
	//str = sql_string.Mid(sql_string.Find("'") +1, 8);
	//str.Insert(4,"/");
	//str.Insert(7,"/");
	pCurrView->m_main_list.SetItemText(iRow, 1,str);
#ifdef LOCAL_EXCEL_SAVE
	local_excel_file.WriteString("\n");//����
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
			local_excel_file.WriteString(str + "\t");//д������,������Ԫ��
#endif
		}else break;
		index1 = index2 + 1;
		if (index1 >= sql_string.GetLength()) break;
		if (str != "") j++;	// ����item�����ֶ�
	}
	pCurrView->GetDlgItem(IDC_LIST2)->SendMessage(WM_VSCROLL,SB_BOTTOM,0);
}

CRect Rect_pic_ok; //(800,350,876,424);
int fg_test_result_ok = 0;
void CMenuTestView::OnPaint()
{   // ���´�����ʢ���������ϳ����������Լ��ʼǱ���OK�ģ�֤�������ݡ�
/*	CImage image;
	//CPaintDC dc(this);        
    if(!image.IsNull()) image.Destroy();
	if (1 == fg_test_result_ok)
		image.Load("res/ok2.png");
	else if (2 == fg_test_result_ok)
		image.Load("res/fail2.png");
    //if(!image.IsNull()) img.Draw(dc.m_hDC, Rect_pic_ok.left, Rect_pic_ok.top);

	GetDlgItem(IDC_PIC_XY)->GetClientRect(&Rect_pic_ok);	//���pictrue�ؼ����ڵľ������� 
	if (fg_test_result_ok != 0) {
		int cx = image.GetWidth();							//��ȡͼƬ���   
		int cy = image.GetHeight();							//��ȡͼƬ�߶�   
		GetDlgItem(IDC_PIC_XY)->GetWindowRect(&Rect_pic_ok);//�����ھ���ѡ�е�picture�ؼ���   
		ScreenToClient(&Rect_pic_ok);						//���ͻ���ѡ�е�Picture�ؼ���ʾ�ľ���������   
		GetDlgItem(IDC_PIC_XY)->MoveWindow(Rect_pic_ok.left, Rect_pic_ok.top, cx, cy, TRUE);//�������ƶ���Picture�ؼ���ʾ�ľ�������   
		CWnd *pWnd=GetDlgItem(IDC_PIC_XY);					//���pictrue�ؼ����ڵľ��   
		pWnd->GetClientRect(&Rect_pic_ok);					//���pictrue�ؼ����ڵľ�������   
		CDC *pDC=pWnd->GetDC();								//���pictrue�ؼ���DC   
		image.Draw(pDC->m_hDC, Rect_pic_ok);				//��ͼƬ����Picture�ؼ���ʾ�ľ�������   
		ReleaseDC(pDC);										//�ͷ�picture�ؼ���DC
		TRACE("draw!\r\n");
	}
*/
#ifdef MGT_CLOCK
	CPaintDC dc(this); // device context for painting
	CPaintDC *pDC = &dc;
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CFormView::OnPaint()
	//��ȡ�ͻ���
	 //RECT Rect;
	 //GetClientRect(&Rect);
	 //������Բ����λ��
	 int CenterX=(Rect.right - Rect.left)/2;
	 int CenterY=(Rect.bottom - Rect.top)/2;
	 //ȡ��ǰʱ��
	 CTime Time=CTime::GetCurrentTime();
	 CString str;
	 int i,x,y;
	 CSize size;
	 //����һ֧��ɫ�ıʣ���������Բ
	 CPen Pen(PS_SOLID,3,RGB(0,0,0));
	 //���õ�ǰ���ʣ���������ǰ�Ļ���
	 CPen *OldPen=pDC->SelectObject(&Pen);
	 //����������Բ
	 pDC->Ellipse(&Rect);
	 CBrush brush(RGB(240,240,240));//�����Լ���Ҫ�����ɫ
	 CBrush *oldbrush;
	 oldbrush = pDC->SelectObject(&brush);//ѡ�µĻ�ˢ
	 pDC->Ellipse(&Rect);
	 pDC->SelectObject(oldbrush);
	 double Radians;
	 //����������ɫΪ��ɫ
	 pDC->SetTextColor(RGB(255,0,0));
	 for(i=1;i<=12;i++)
	 {
	  //��ʽ���ӵ�ֵ
	  str.Format("%d",i);
	  size=pDC->GetTextExtent(str,str.GetLength());
	  Radians=(double)i*6.28/12.0;
	  //�����ӵ���õ�λ��
	  x=CenterX-(size.cx/2)+(int)((double)(CenterX-20)*sin(Radians))+Rect.left;
	  y=CenterY-(size.cy/2)-(int)((double)(CenterY-20)*cos(Radians)) + Rect.top;
	  //�����ӵ�
	  pDC->TextOut(x,y,str);
	 }
	 //����ʱ��ָ��ļн�
	 Radians=(double)Time.GetHour()+(double)Time.GetMinute()/60.0+(double)Time.GetSecond()/3600.0;
	 Radians *=6.28/12.0;
	 //����ʱ��ָ�뻭��
	 CPen HourPen(PS_SOLID,5,RGB(255,0,0));
	 pDC->SelectObject(&HourPen);
	 //����ʱ��ָ��
	 pDC->MoveTo(CenterX+Rect.left,CenterY+ Rect.top);
	 pDC->LineTo(CenterX+Rect.left+(int)((double)(CenterX/3)*sin(Radians)),CenterY+ Rect.top-(int)((double)(CenterY/3)*cos(Radians)));
	 Radians=(double)Time.GetMinute()+(double)Time.GetSecond()/60.0;
	 Radians *=6.28/60.0;
	 //��������ָ�뻭��
	 CPen MinutePen(PS_SOLID,3,RGB(0,0,255));
	 pDC->SelectObject(&MinutePen);
	 //���Ʒ���ָ��
	 pDC->MoveTo(CenterX+Rect.left,CenterY+ Rect.top);
	 pDC->LineTo((int)(CenterX+Rect.left+(int)((double)(CenterX*2)/3)*sin(Radians)),(int)(CenterY+ Rect.top-(int)((double)(CenterY*2/3)*cos(Radians))));
	 Radians=(double)Time.GetSecond();
	 Radians *=6.28/60.0;
	 //��������ָ�뻭��
	 CPen SecondPen(PS_SOLID,1,RGB(0,255,0));
	 pDC->SelectObject(&SecondPen);
	 //���Ʒ���ָ��
	 pDC->MoveTo(CenterX+Rect.left,CenterY+ Rect.top);
	 pDC->LineTo((int)(CenterX+Rect.left+(int)((double)(CenterX*4)/5)*sin(Radians)),(int)(CenterY+ Rect.top-(int)((double)(CenterY*4/5)*cos(Radians))));
	 pDC->SelectObject(OldPen);
#endif

	CFormView::OnPaint();
}

#ifdef SYS_THREAD_POOL

#define THREADPOOL_SIZE				MAX_USER_NUM	// �߳�������������� ���
#define SOCKET_THREAD_STASK_SIZE	(64 * 1024)	// �߳�ջ��С 128K
//#define TATOL_TAST_NUM	20

BOOL thread_pool_init()
{
    HRESULT hr = pool.Initialize((void*)321, THREADPOOL_SIZE, SOCKET_THREAD_STASK_SIZE);
    if( !SUCCEEDED( hr ) ) return FALSE;
	return TRUE;
}

// ��δ���� �����̽����Զ��ջ���Դ��
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	fg_chengce = 0;
	//TRACE("0\r\n");
}


void CMenuTestView::OnBnClickedRadioChengce()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	fg_chengce = 1;
	//TRACE("1\r\n");
}
