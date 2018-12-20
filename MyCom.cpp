#include "stdafx.h"
#include "Mycom.h"
#include "resource.h"		// 务必添加资源文件，否则编译不过
#include "MenuTestDoc.h"
#include "MainFrm.h"
#include "MenuTestView.h"

#include "json/json.h"

#ifdef CLIENT_HTTP
#include "HttpClient.h"

CHttpClient http_client;
/*
 *  示例代码：   （https请求到阿里物联网后台）
 *  	string ret;
 *		http_client.HttpPost("https://oapi.dingtalk.com/robot/send?access_token=5de05218ebb592ecf7027f07a972cfd89f892231370d64b9d6c5f9052b5bc360",
 *		"{\"msgtype\": \"text\",\"text\": {\"content\": \"mfc -> hello world!\"}}",ret);
*/
#endif

#ifdef CLIENT_HTTP
BOOL http_json_post(const char* str_json);
char str_json[8*1024] = {0};
#endif

#ifdef DATA_BACE
#include "CPFile.h"
extern CPData *p_cp_data;
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////
BOOL CMyCom::bOpen = FALSE;
#define ID_TIMER	100
extern void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime );
extern int fg_client_socket_conect_ok;
// 构造
CMyCom::CMyCom(LPCSTR Com, int BaudRate)
{
	RdIndex = 0;
	WrIndex = 0;

	//  初始化ACK帧 ： 0xfa + 1(字节数) + 0x55(CMD) + 0x55(checksum) + 0xf0     
	ack[0] = UART_START;ack[1] = 2;ack[2] = MIAN_CMD_ACK;ack[3] = 0;ack[4] = UART_END;
	// 配置串口
	SetupMyCom(Com, BaudRate);
/*
	Cmd[2] = 0;
	Cmd[3] = 0x11;
	Cmd[4] = 0x22;
	Cmd[5] = 0x33;
	Cmd[6] = 0x44;
	Cmd[7] = 7;
	Cmd[8] = 0;
	Cmd[9] = 0;
	Cmd[10] = 20;
	Cmd[11] = 2;
	Cmd[12] = 00;
	Cmd[13] = 28;
	Cmd[14] = 1;
	Cmd[15] = 1000 >> 8;
	Cmd[16] = 1000 & 0xff;
	Cmd[17] = 3;
	Cmd[18] = 0;
	Cmd[19] = 30;
	Cmd[20] = 4;
	Cmd[21] = 0;
	Cmd[22] = 1;
	Cmd[23] = 60;
	Cmd[24] = 0;
	Cmd[25] = 1;
	Cmd[26] = 120;
	Cmd[27] = 1000 >> 8;
	Cmd[28] = 1000 & 0xff;
*/
}

CMyCom::~CMyCom()
{
	CloseHandle(hEventR);
	CloseHandle(hEventW);
	CloseHandle(hComDev);
	CloseHandle(m_handle);
	CloseHandle(wait_handle);
	CloseHandle(com_handle);
	CloseHandle(jindutiao_handle);
}

// uart初始化  （串口参数配置部分以外的系统级部分）
BOOL CMyCom::uart_init()
{
	// 创建异步读写事件
	hEventR = CreateEvent(NULL,TRUE,FALSE,NULL);
	hEventW = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_handle = CreateEvent(NULL,FALSE,FALSE,NULL);			// 主线程请求发送信号(自动复位事件状态，初始状态未触发)
	wait_handle = CreateEvent(NULL,FALSE,TRUE,NULL);		// 临界区 串口接收的两个线程公共部分数据区的同步 （初始状态触发）
	com_handle = CreateEvent(NULL,FALSE,TRUE,NULL);			// 串口资源临界区
	jindutiao_handle = CreateEvent(NULL,FALSE,TRUE,NULL);	// 进度条资源临界区

	// 创建串口检测线程
	CWinThread* pThread = AfxBeginThread(CommWatchProc, this);
	// 串口数据命令执行线程
	CWinThread* pThreadRevCmd = AfxBeginThread(RevComProc, this);
	// 发送异步线程
	CWinThread* pThreadSend = AfxBeginThread(SendProc, this);

	SetTimer(NULL, ID_TIMER, 200, TimerProc);	// WinAPI 重发定时器

	return TRUE;
}

DWORD CMyCom::ReSetComPort(LPCSTR Com)
{
	// 关闭原有串口
	bOpen = FALSE;Sleep(20);
	if (hComDev != INVALID_HANDLE_VALUE) {
		CloseHandle(hComDev);Sleep(20);
	}
	// 打开新COM
	return SetupMyCom(Com, UART_BAUD_RATE);
}

BOOL CMyCom::SetupMyCom(LPCSTR Com, int BaudRate)

{
	DCB dcb;
//	COMMTIMEOUTS timeouts;

	if(bOpen)return FALSE;            // 串口被占用
	// 打开COM4
	hComDev = CreateFile(Com, GENERIC_READ|GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	if(hComDev == INVALID_HANDLE_VALUE)
	{
#ifndef _DEBUG
		MessageBox(NULL, _T("串口端口配置无效!!"), _T("TRACE"), MB_OK);
#endif
		return FALSE;                    // 打开端口失败
	}

	// 设置超时控制
//	SetCommTimeouts(hComDev, &timeouts);
	// 设置接收缓冲区和输出缓冲区的大小
	SetupComm(hComDev, CMD_SIZE_MAX, 1024);
	// 获取缺省的DCB结构的值
	GetCommState(hComDev, &dcb);

	// 设定波特率为115200 bps
	dcb.BaudRate = BaudRate;
	// 设定无奇偶校验
	dcb.fParity = NOPARITY;
	// 设定数据位为8
	dcb.ByteSize = 8;
	// 设定一个停止位
	dcb.StopBits = ONESTOPBIT;

	// 设定相应监视串口的错误和接收到字符两种事件响应
	SetCommMask(hComDev, EV_ERR|EV_RXCHAR);
	// 设置串行设备控制参数
	SetCommState(hComDev, &dcb);
	// 设置参数表示设备已打开
	bOpen = TRUE;
	
	return TRUE;
}

// 数据发送  
// 数据发送利用WriteFile函数实现。对于同步I/O操作，它的最后一个参数可为
// NULL；而对异步I/O操作，它的最后一个参数必需是一个指向OVERLAPPED结构的指针，
// 通过OVERLAPPED结构来获得当前的操作状态。
// lpSndBuffer为发送数据缓冲区指针，  
// dwBytesToWrite为将要发送的字节长度
BOOL CMyCom::WriteMyCom(LPVOID lpSndBuffer, DWORD dwBytesToWrite)
{

	BOOL bWriteState;
	DWORD dwBytesWritten;				// 实际发送的字节数
	OVERLAPPED olRead;
	memset(&olRead,0,sizeof(OVERLAPPED));
	olRead.hEvent = hEventW;


	if(!bOpen) {TRACE("Send Err 0");return FALSE;}				// 串口未打开
	bWriteState = WriteFile(hComDev, lpSndBuffer, dwBytesToWrite, &dwBytesWritten, &olRead);
	if(!bWriteState) 
	{
		if (GetLastError() == ERROR_IO_PENDING)	// 返回ERROR_IO_PENDING,表明串口正在进行读操作 
		{
			if (WaitForSingleObject(olRead.hEvent,2000) == WAIT_OBJECT_0)
			{
				return TRUE;
			}

		}
		//TRACE("Send Err 1");
		return FALSE;
	}

	return TRUE;
}

// 数据接收  
// 接收数据的任务由ReadFile函数完成。该函数从串口接收缓冲区中读取数据，
// 读取数据前，先用ClearCommError函数获得接收缓冲区中的字节数。接收
// 数据时，同步
// 和异步读取的差别同发送数据是一样的。  
// lpInBuffer为接收数据的缓冲区指针，
// dwBytesToRead为准备读取的数据长度(即字节数)
DWORD CMyCom::ReadMyCom(LPVOID lpInBuffer, DWORD dwBytesToRead)
{
	BOOL bReadState = FALSE;
	COMSTAT ComStat; // 串行设备状态结构
	DWORD dwBytesRead, dwErrorFlags;
	OVERLAPPED olRead;
	memset(&olRead,0,sizeof(OVERLAPPED));
	olRead.hEvent = hEventR;


	if(!bOpen)return 0; // 串口未打开
	// 用于读取串行设备的当前状态
	ClearCommError(hComDev, &dwErrorFlags, &ComStat);
	// 设定应该读取的数据长度
	dwBytesRead = min(dwBytesToRead, ComStat.cbInQue);
	
	// 读取数据  
	if(0 == dwBytesRead) return 0;
	bReadState = ReadFile(hComDev, lpInBuffer, dwBytesRead, &dwBytesRead, &olRead);
	if(!bReadState) // 返回数据失败(可能异步还未完成)
	{
		if (GetLastError()==ERROR_IO_PENDING)	// 返回ERROR_IO_PENDING,表明串口正在进行读操作 
		{
			if (WaitForSingleObject(olRead.hEvent,2000) == WAIT_OBJECT_0)
			{
				if (GetOverlappedResult(hComDev, &olRead, &dwBytesRead,FALSE))
					return dwBytesRead;
				else return 0;
			}

		}
		return 0;
	}

	// 返回所读取的数据
	return dwBytesRead;
}

extern int fg_chengce;
extern int ConvertRecvBuf(char *buf, int num);
// 串口监视线程
UINT CMyCom::CommWatchProc(LPVOID pParam)
{
	// pThread->m_bAutoDelete = TRUE;
	HWND hWnd = (HWND)pParam;
	UINT8 rbuf[CMD_SIZE_MAX];	// 用于接受串口数据
	DWORD read;					// 保存串口缓冲区中的数据
	DWORD dwEventMask = 0;		// 发生的事件
	CMyCom *pMyCom = (CMyCom *)pParam;
	char COM_chengce[128];int chengce_sum = 0;

	while(1)				// 串口已被成功打开
	{
		if (pMyCom->bOpen) {/*
			WaitCommEvent(pMyCom->hComDev, &dwEventMask, NULL);
			if((dwEventMask & EV_RXCHAR) == EV_RXCHAR)
*/			{
				// 接收到字符，在此加入语句进行处理
				read = pMyCom->ReadMyCom(rbuf);
				if(read > 0)
				{  
#ifdef AUTO_TOOL_18V_TEST
					if (fg_chengce)	{
						// 成测一次性将串口读完
						do{
							memcpy(COM_chengce + chengce_sum, rbuf, read);
							chengce_sum += read;
							Sleep(100);
						}while((read = pMyCom->ReadMyCom(rbuf)) > 0);
						memcpy(rbuf, COM_chengce, chengce_sum);
						read = chengce_sum;
						chengce_sum = 0;
						read = ConvertRecvBuf((char*)rbuf, read);	// 如果是成测，转换收到的字符串，使之符合协议要求格式
						if (read == 0) continue;
					}
#endif
					// 命令缓冲
					pMyCom->DataToRevBuf(rbuf, read);
					//if (!fgClose)
					{
	#ifdef DEBUG_CHAR
						for(int i=0; i<(int)read; i++)
						{	
							if (0 == kk){kk = 1;RevDat.GetBufferSetLength(1024*1024);}	// 设置调试区缓冲大小}
							// Debug Dvd串口上发信息
							RevDat.Format(L"%s %.2X",RevDat, rbuf[i]);
							if (++count % 20 == 0) RevDat.Format(L"%s %s",RevDat, L"\r\n");
						}
						if (pView)	{	// 等待窗口创建完毕
							pView->m_DVDInfo.SetWindowText(RevDat);
							pView->m_DVDInfo.LineScroll(pView->m_DVDInfo.GetLineCount());//滚动条置底
 						}
	#endif
					}
				}
			}/*
			if((dwEventMask & EV_ERR) == EV_ERR)
			{
				MessageBox(NULL, _T("ComError"), _T("TRACE"), MB_OK);
			}
	*/
		}

		Sleep(20);
	}
	return 1;
}
#define RESEND_NUM	5
//UINT8 CmdBak[DSA_BUF_LENTH];
UINT8 ResendTime = 0;
UINT8 ResendNum = 0;

#define init_ack		0
#define get_ack			1
#define time_over_ack	2
#define wait_ack	{while(init_ack == fg_ack) Sleep(50);}
int fg_ack = get_ack;
UINT8 *pLastFlame = NULL;
int LastFlameNum = 0;

// 发送命令  // 本函数为可重入函数
int CMyCom::UartSendCmd(UINT8 *sentbuf, int bytes_num, char re_send)		// bytes_num  总字节数（含首位0xfa,0xf0）
{
#ifdef AUTO_TOOL_18V_TEST
	return 1;	// 目前电动工具上位机不发送指令 20180609
#endif

#ifndef SIMULATE_TOPC

	TRACE("发送\r\n");
	int i;
	UINT8 ret = 2;

#ifdef _NO_ACK
	re_send = 1;
#endif
	if (0 == re_send) {
		// 备份，以便重发
		pLastFlame = sentbuf;
		LastFlameNum = bytes_num;
	}

	WaitForSingleObject(com_handle, INFINITE);			// 串口资源的保护区

	sentbuf[0] = UART_START;
	UINT8 check = 0;
	for (i=1;i<bytes_num - 2;i++)
	{
		check ^= sentbuf[i];
	}
	sentbuf[bytes_num - 2] = check;
	sentbuf[bytes_num - 1] = UART_END;	//end
	
	WriteMyCom(sentbuf, bytes_num);
	
	SetEvent(com_handle);

#ifndef _DEBUG
		if (ret == 2)	return 2;
#endif

	// 备份此次发送的数据
	//memcpy(CmdBak, sentbuf, DSA_BUF_LENTH);
	//ResendTime = 0;
	//for (int j = 0; j < 16; j++)
	//	TRACE("%d  ", sentbuf[j]);
	//TRACE("\r\n");

	if (0 == re_send) {
		fg_ack = init_ack;
		wait_ack
		if (time_over_ack == fg_ack) return 2;		// ack 超时
	}
#endif
	return 1;
}

int uart_pos = 0;
int pos = 0;
#ifdef CODE_PART
	extern int send_program_code(int program_type);
	extern PARA_SETTING   setting;
	extern unsigned char fg_normal_program;
	extern int send_bace_param_code(PARA_SETTING * psetting);
	extern int send_normal_code();
#endif

UINT CMyCom::jindutiao_func(LPVOID pParam)
{
	CMyCom *pCom = (CMyCom *)pParam;
	WaitForSingleObject(pCom->jindutiao_handle, INFINITE);			// 进度条资源的保护区
	pCurrView->Deliver_Dlg.DoModal();
	SetEvent(pCom->jindutiao_handle);

	return 1;
}
int fg_moudle_item_debug = 0;	// debug topc使用
// 发送线程
UINT CMyCom::SendProc(LPVOID pParam)
{
	int ret = 0;
	CMyCom *pCom = (CMyCom *)pParam;
	while (1) {

		WaitForSingleObject(pCom->m_handle, INFINITE);
			
		// 启动进度条
		uart_pos = 0;
		CWinThread* pThread = AfxBeginThread(jindutiao_func, pParam);
#ifndef SIMULATE_TOPC
		// 开始UART传输
		// 1，基本参数部分传输
		if (2 == send_bace_param_code(& setting)) {
			AfxMessageBox("通信故障！");
			continue;
		}
		uart_pos = 10;
		// 2 定制部分 start 时序传输
		ret = send_program_code(MIAN_CMD_PROGRAM_START);
		if (0 == ret) {
			AfxMessageBox("定制逻辑【启动】部分语法问题！");
			continue;
		}else if (2 == ret) {
			AfxMessageBox("通信故障！");
			continue;
		}
		uart_pos = 30;
		// 3 定制部分 end 时序传输
		ret = send_program_code(MIAN_CMD_PROGRAM_STOP);
		if (0 == ret) {
			AfxMessageBox("定制逻辑【停机】部分语法问题！");
			continue;
		}else if (2 == ret) {
			AfxMessageBox("通信故障！");
			continue;
		}
#endif
		uart_pos = 50;
		// 2, 定制部分传输
		if (0 == fg_normal_program) {
			// 4 定制部分编程时序传输
			ret = send_program_code(MIAN_CMD_PROGRAM_OTHER);
			if (0 == ret) {
				AfxMessageBox("定制逻辑【测试】部分语法问题！");
				continue;
			}else if (2 == ret) {
				AfxMessageBox("通信故障！");
				continue;
			}
			uart_pos = 100;
#ifdef SIMULATE_TOPC
			fg_moudle_item_debug = 1;
#endif
		}else {
			// 5，常规可选部分传输
			if (2 == send_normal_code()) {
				AfxMessageBox("通信故障！");
				continue;
			}
			uart_pos = 100;
		}
		Sleep(50);
	}

	return 1;
}
/*
DWORD CMyCom::RequestSend(UINT8 *sentbuf, int bytes_num)
{
	SendBufArea[wr].SendBufNum = bytes_num;
	memcpy(SendBufArea[wr].SendBufArea, sentbuf, bytes_num);
	if (++wr >= SEND_BUF_NUM) wr = 0;

	return 1;
}
*/

extern CMyCom MyCom;
int loop_times = 0;
int re_send_times = 0;
void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime )
{
	if (init_ack == fg_ack) {
		if (++loop_times > 5) {
			loop_times = 0;
			if (++re_send_times < 3) {
				TRACE("重发\r\n");
				MyCom.UartSendCmd(pLastFlame, LastFlameNum, 1);
			}else {
				fg_ack = time_over_ack;
				re_send_times = 0;
				//AfxMessageBox("发送超时！");
			}
		}
	}else {
		re_send_times = 0;
		loop_times = 0;
	}
}

// 串口数据入缓冲
void CMyCom::DataToRevBuf(UINT8 *RevBuf, int DataNum)
{
	WaitForSingleObject(wait_handle, INFINITE);

	if (WrIndex + DataNum >= COM_REV_BUF_SIZE)
	{
		memcpy(AfxRevBuf, AfxRevBuf + RdIndex, WrIndex - RdIndex);
		WrIndex -= RdIndex;
		RdIndex = 0;
	}
	if (WrIndex + DataNum < COM_REV_BUF_SIZE) {
		memcpy(AfxRevBuf + WrIndex, RevBuf, DataNum);
		WrIndex += DataNum;
	}

	SetEvent(wait_handle);
}

// 0x5a 0x01 => 0xfa
// 0x5a 0x02 => 0xf0
// 0x5a 0x03 => 0x5a
//UINT8 data[] = {NULL,CMyCom::UART_START, CMyCom::UART_END, CMyCom::TRANCELATE_CHAR};
// 串口读出命令
DWORD CMyCom::ReadCmd()
{
	int CRC = 0;
	int frame_length = 0;		// 适用不定长帧
	DWORD ret = 0;

	WaitForSingleObject(wait_handle, INFINITE);

	while (RdIndex < WrIndex && AfxRevBuf[RdIndex] != UART_START) RdIndex++;		// 找命令头
	if (RdIndex +1 < WrIndex) {
		// 找到命令头
		frame_length = AfxRevBuf[RdIndex + 1] +3;									// 不定长帧实际长度
		if (WrIndex >= RdIndex + frame_length) {
			for (int i = 1; i < frame_length - 2; i++)
				CRC ^= AfxRevBuf[RdIndex + i];
			if (AfxRevBuf[RdIndex + frame_length - 1] == UART_END
				&& CRC == AfxRevBuf[RdIndex + frame_length - 2]) {

				memcpy(Cmd, AfxRevBuf + RdIndex, frame_length);					// 收到一条完整命令
				RdIndex += frame_length;
				ret = 1;
				goto RET_CMD;
			}
			RdIndex += frame_length;			
		}
	}

RET_CMD:
	SetEvent(wait_handle);
	return ret;
}

// TOPC部分

//	1，	限流
//	2,	限流公差
//  3,	转速
//	4， 转速公差
//	5,	欠压
//	6，	欠压公差
//	7,	欠压恢复
//	8,	欠压恢复公差
//	9,	转向
//	10,	软启时间
//	11,	软启时间公差
//	12,	刹车
//	13,	NTC高温截至
//	14,	NTC高温恢复
//	15,	NTC低温截至
//	16,	NTC低温恢复
//	17，NTC公差
//	18，电机极对数
//const CString test_item_string[] = {"电流", "转速", "欠压", "欠压恢复", "转向", "启动时间","刹车", "NTC高温截至", "NTC高温恢复", "NTC低温截至", "NTC低温恢复", ""};
CString total_disp_info = "";
#define ITEM_MODLE_NUM_MAX	256
extern CString op_name;
extern CString factory_name;
extern CString file_selected;
extern ITEM_CONTENT item_content[];
extern int total_item_num;
extern CString topc_data_report(const TEST_INFO *pInfo);
typedef struct {
	CString type;
	CString value;
}MODLE_ITEM;
MODLE_ITEM modle_item[ITEM_MODLE_NUM_MAX];
MODLE_ITEM ng_item_record;
int modle_num = 0;
unsigned char debug_pakect[256] = {
	/*0xFA,0x34,0x01,'N','Y','1','6','1','1','3','0','0','1','6','-','0','0','9','3','0',0,0,0,0x03,0xC7,0x84,0x08,0x00,0x00,0xC7,0x84,0x08,0x00,0x00,0x01,0xA5,0x0C,0x00,0x80,0x00,0xCD,0x00,0x00,0x00,/ ** /0x01,0xD6,0x0C,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x01,0x07,0x0C,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x01,0x13,0x0C,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0xDD,0xF0*/
0xFA, 0x44, 0x01, 0x4E,0x59,0x31,0x37,0x31,0x31,0x33,0x36,0x33,0x33,0x2D,0x30,0x32,0x30,0x37,0x38,0x0D,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x01,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x80,0xC0,0xF0 
};
//当前测试数量
int test_num = 0;
int ng_num = 0;
extern CRect Rect_pic_ok;
//extern int fg_test_result_ok;
UINT CMyCom::RevComProc(LPVOID pParam)
{
	  while (1) /**/{
		CMyCom *pCom = (CMyCom *)pParam;
#ifdef SIMULATE_TOPC
		if (fg_moudle_item_debug) {fg_moudle_item_debug = 0;
#else
		if (pCom->ReadCmd()/*1*/) {
#endif
			// 收到一条正常命令
			// 解析并执行Cmd
			CString sql_save_string = "";
			CString disp_info = "";
#ifdef SIMULATE_TOPC
			TEST_INFO *pInfo = (TEST_INFO *)debug_pakect;
#else
			TEST_INFO *pInfo = (TEST_INFO *)pCom->Cmd;
#endif
			// send ACK
			if (pInfo->to_pc_type != MIAN_CMD_ACK)	pCom->UartSendCmd(pCom->ack, ACK_FLAME_NUM, 1);			// 反馈 ACK

			switch (pInfo->to_pc_type)
			{
			case TEST_GOOD:														// 良品
				//break;
			case TEST_NG:														// 不良品
				disp_info += topc_data_report(pInfo);
				if (disp_info == "") break;	// topc_data_report返回 ""，异常
				pCurrView->GetDlgItem(IDC_RESULT)->SetWindowText(pInfo->to_pc_type == TEST_GOOD? "成功" : "失败");
				// 构造数据
				{
				CTime t = CTime::GetCurrentTime(); //获取系统日期
				int d=t.GetDay(); //获得几号
				int y=t.GetYear(); //获取年份
				int m=t.GetMonth(); //获取当前月份
				int sys_date = y*10000 + m*100 + d*1;
				CString date_time = "";
				date_time.Format("%d %d%d:%d%d:%d%d", sys_date, t.GetHour()/10,t.GetHour()%10, t.GetMinute()/10,t.GetMinute()%10, t.GetSecond()/10, t.GetSecond()%10);
				int start = file_selected.ReverseFind('\\');
				int end = file_selected.ReverseFind('.');
				CString name = file_selected.Mid(start +1,end - start - 1);

				sql_save_string.Format("Insert into TianYouShou values ('%s', '%s','%s'", date_time,name,pInfo->saoma_id);
#ifdef CLIENT_HTTP
				Json::Value root;
				root["data_type"] = Json::Value("testdata2server");
				Json::Value test_items;  // 测试项部分Json组合
				test_items["date_time"] = Json::Value(date_time.GetBuffer());
				test_items["project_name"] = Json::Value(name.GetBuffer());
				test_items["saoma_id"] = Json::Value(pInfo->saoma_id);
#endif
				CString temp;
				for (int i = 0;i < DB_SEGMENT_ITEM_NUMMAX; i++) {
					temp = sql_save_string;
					if (i < modle_num) {
						sql_save_string.Format("%s, '%s', '%s'", temp,modle_item[i].type, modle_item[i].value);//实测项
#ifdef CLIENT_HTTP
						test_items[modle_item[i].type] = Json::Value(modle_item[i].value.GetBuffer());
#endif
					}else if (i < total_item_num){
						sql_save_string.Format("%s, '%s', '%s'", temp, "--", "--");	// '--'表示遇到NG之后未测项
#ifdef CLIENT_HTTP
						test_items["--"] = Json::Value("--");
#endif
					}else {
						sql_save_string.Format("%s, '%s', '%s'", temp, "", ""); // 冗余项（兼容考虑）
#ifdef CLIENT_HTTP
						//test_items[""] = Json::Value("");
#endif
					}
				}
				temp = sql_save_string;
				sql_save_string.Format("%s,'%s','%s','%s',%d,'%s','')", temp,ng_item_record.type,ng_item_record.value,op_name, total_item_num, factory_name);
#ifdef CLIENT_HTTP
				test_items["NG_item"] = Json::Value(ng_item_record.type.GetBuffer());
				test_items["NG_item_value"] = Json::Value(ng_item_record.value.GetBuffer());
				test_items["op_name"] = Json::Value(op_name.GetBuffer());
				test_items["total_item_num"] = Json::Value(total_item_num);
				test_items["factory_name"] = Json::Value(factory_name.GetBuffer());

				root["test_items"] = test_items;

				Json::ValueType type = root.type();
				Json::FastWriter fast_writer;
				strcpy_s(str_json, fast_writer.write(root).c_str());	// 此处，只能拷贝，不能指针赋值

				TRACE("\r\n JSON : %s",str_json);
#endif

#ifdef DATA_BACE				
				// 1，本地存储
				if (p_cp_data != NULL) p_cp_data->Execute(sql_save_string);
#endif

#ifdef SYS_CLEINT
				// 2，本地列表显示
				pCurrView->disp_current_test_info_line(sql_save_string);
#endif
				// 3，外网发送到远程服务器
#ifdef MGT_SOCKET_CLIENT
				// 外网发送  sql_save_string  表明和本地数据库存储模式和方案都一摸一样
				if (fg_client_socket_conect_ok) pCurrView->client_send(sql_save_string);
				else {
					// 本地暂存 (功能待完善)。。。
					// ...
				}
#elif defined(CLIENT_HTTP)
				// 通过 HTTP (POST)请求，以 JSON格式发送测试数据到服务器
				http_json_post(str_json);
#endif
				// 4，本地测试信息块显示
				disp_info += (pInfo->to_pc_type == 0? "测试结果：良品\r\n\r\n" : "测试结果：不良品\r\n\r\n");
				if (total_disp_info.GetLength() > 100000) total_disp_info = "";// 清字符串
				total_disp_info += disp_info;				
				pCurrView->m_topc_disp.SetWindowText(total_disp_info);
				pCurrView->GetDlgItem(IDC_DISP)->SendMessage(WM_VSCROLL,SB_BOTTOM,0);
				// 5，更新状态栏测试数量
				{
					test_num++;		// 总数
					if (pInfo->to_pc_type) ng_num++;	// 不良数
					CString str_num = "";
					str_num.Format("当前测试数量：%d  不良数量：%d", test_num, ng_num);
					::SendMessage(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_wndStatusBar.m_hWnd,SB_SETTEXT,4,(LPARAM)str_num.GetBuffer());// 改用状态栏提示
				}
				// 6,更新测试结果图标
				//pCurrView->InvalidateRect(&Rect_pic_ok,true);
				//pCurrView->UpdateWindow();
				}
				break;
			case SELFCHECK_FAULT:												// 系统自检故障
				break;
			case MIAN_CMD_ACK:
				loop_times = 0;
				re_send_times = 0;
				fg_ack = get_ack;
				break;
			default:
				break;
			}
		}// if (pCom->ReadCmd())
		Sleep(50);
	}// while(1)
	return 1;
}

#ifdef AUTO_TOOL_18V_TEST

	// item_value 用于第一个数据，item_reserv（低15位）用于第二个数据
	// 欠压是要测上下限两组数据的
	// 15bit最大值 表示电机不停
	// dd_text[]表填上固定文字
	typedef struct {
		char data_num;	// 代表一项测试几个数据:0,1,2
		CString title1;	// 固定文字1
		CString title2;	// 固定文字2
		CString unit;	// 单位
	}diandong_dispinfo_text;

	// 本数组此处初始化值可以随意，后续会通过读入文件将真正值赋值到以下数组。具体见函数GetDiandongToolTestItem()
	diandong_dispinfo_text chuce_text[20] = {
		{0, "", "", ""},
		{0, "", "", ""},
		{0, "", "", ""},
		{0, "", "", ""},
		{0, "", "", ""},
		{0, "", "", ""},
		{1, "", "", ""},
		{1, "", "", ""},
		{1, "", "", ""},
		{1, "", "", ""},
	};
	diandong_dispinfo_text chengce_text[20] = {{0, "", "", ""}, {0,"","",""}};	// 成测使用

	int GetDiandongToolTestItem(CString& str_comment, int num)
	{
		int index0 = 0, index1 = 0;
		CString str_temp = "";

		index0 = str_comment.Find("{");
		if (index0 == -1) return 0;
		index1 = str_comment.Find(",");
		if (index1 == -1) return 0;
		str_temp = str_comment.Mid(index0 + 1, index1 - index0 - 1);
		// data_num
		chuce_text[num].data_num = _ttoi(str_temp);
		// string: title1,title2,unit
		for (int i = 0; i < 3; i++) {
			index0 = index1;
			index1 = str_comment.Find(",", index1 + 1);
			if (index1 == -1) return 0;
			str_temp = str_comment.Mid(index0 + 1, index1 - index0 - 1);
			(&chuce_text[num].title1)[i] = str_temp.Trim('\"');
		}
		// 删减出真正的注释字符串
		index0 = str_comment.Find("{");
		str_comment.Delete(index0, str_comment.GetLength() - index0);
		return 1;
	}
#endif

CString topc_data_report(const TEST_INFO *pInfo)
{
	CString str_item_type = "";
	CString str_item_value = "";
	CString ret_str_item_info = "";

	modle_num = 0;
	ng_item_record.type = "OK";
	ng_item_record.value= "OK";

	ret_str_item_info.Format("扫码ID:  %s\r\n",pInfo->saoma_id);

	for (int i = 0; i < pInfo->item_num; i++) {
		// 值
#ifndef AUTO_TOOL_18V_TEST		// 园林工具
		str_item_type = item_content[i].item_content;	// 字段
		if(pInfo->item_info[i].item_type == FAULT_CURRENT) {
			str_item_value.Format("%.1f A",pInfo->item_info[i].item_value / 10.0);
			if (pInfo->item_info[i].item_reserv & 0x8000) str_item_value += "  (NG)";
		}else if (pInfo->item_info[i].item_type > FAULT_DZ_AD(0) && item_content[i].item_content.Find("压") != -1) {
			str_item_value.Format("%d mv",pInfo->item_info[i].item_value);
			if (pInfo->item_info[i].item_reserv & 0x8000) str_item_value += "  (NG)";
		}else if (pInfo->item_info[i].item_type == FAULT_SPEED && item_content[i].item_content.Find("速") != -1){
			str_item_value.Format("%d RPM",pInfo->item_info[i].item_value);
			if (pInfo->item_info[i].item_reserv & 0x8000) str_item_value += "  (NG)";
		}else if (pInfo->item_info[i].item_type == FAULT_POWER && item_content[i].item_content.Find("功率") != -1) {
			str_item_value.Format("%d w",pInfo->item_info[i].item_value);
			if (pInfo->item_info[i].item_reserv & 0x8000) str_item_value += "  (NG)";
		}else {
			str_item_value = (pInfo->item_info[i].item_reserv & 0x8000)? "NG" : "OK";
		}
#else							// 电动工具
		if (fg_chengce && pInfo->item_num > 1) {AfxMessageBox("请正确选择测试模式：初测？ 或者 成测？"); return "";} // 选了成册，实际初测数据
		diandong_dispinfo_text *dd_text = NULL;
		if (fg_chengce == 0) {
			str_item_type = item_content[i].item_content;	// 字段
			dd_text = chuce_text;
		}else {
			str_item_type = "";	// 字段
			dd_text = chengce_text;
		}

		if (dd_text[i].data_num > 0) {
			str_item_value.Format("%s %d%s",dd_text[i].title1, pInfo->item_info[i].item_value, dd_text[i].unit);
			if (dd_text[i].data_num == 2) {
				CString str_tmp = "";
				str_tmp.Format("%s %d%s",dd_text[i].title2, pInfo->item_info[i].item_reserv & 0x7fff ,dd_text[i].unit);
				str_item_value += "; " + str_tmp;
			}
		}
		else {
			str_item_value = dd_text[i].title1;
		}
		if (fg_chengce == 0) str_item_value += (pInfo->item_info[i].item_reserv & 0x8000)? " NG" : " OK";	// 只针对初测 后缀 NG/OK
#endif
		
		// 数据库数据构建 (目前仅支持测试到不良项，立刻停止测试，上发数据)
		if ((pInfo->item_info[i].item_reserv & 0x8000) || item_content[i].fg_half == 1) {
			modle_item[modle_num].type = str_item_type;
			modle_item[modle_num++].value = str_item_value;
			ret_str_item_info += str_item_type + ":" + str_item_value + " " + "\r\n";
		}
		// 不良项记录
		if (pInfo->item_info[i].item_reserv & 0x8000) {
				ng_item_record.type = str_item_type;
				ng_item_record.value = str_item_value;
		}
	}
	//ret_str_item_info += "\r\n";
	return ret_str_item_info;
}

int fg_chengce = 0;
#ifdef AUTO_TOOL_18V_TEST
//============================   成测数据记录   ===========================
// 测试结果扫码：
// $chengce：OK
// $chengce：NG
// $chengce：xxx NG

char chengce_saoma_ID[SAOMA_ID_LEN] = {0};
CString str_chegncebuliang[] = {"OK","NG", "xxx NG", "xxxx NG", "xxxxx NG", "", "", ""};
// 函数返回值：0：中间数据  >=1: 模拟出一个完整帧，返回帧bytes个数
int ConvertRecvBuf(char *buf, int num)
{
	if (num >= 2) buf[num - 2] = 0;	// 去掉扫码枪末尾的\r\n
	else buf[num] = 0;
	CString str(buf);
	str.MakeLower();
	if (str.Find("$成测") != -1) {	// 上传的是测试结果
		if (chengce_saoma_ID[0] == 0) {
			AfxMessageBox("请先扫码产品流水号，谢谢！");
			return 0;
		}
		// 构建假的帧协议
		TEST_INFO *pchengce_flame = (TEST_INFO *)buf;
		pchengce_flame->frame_head = 0xfa;	//
		pchengce_flame->item_num = 1;
		pchengce_flame->frame_data_num = 3+SAOMA_ID_LEN + pchengce_flame->item_num * sizeof(ITEM_INFO);
		pchengce_flame->to_pc_type = (str.Find("ok") != -1? TEST_GOOD : TEST_NG);
		strcpy_s(pchengce_flame->saoma_id, chengce_saoma_ID);
		pchengce_flame->item_info[0].item_type = 0;
		pchengce_flame->item_info[0].item_value = 0;
		pchengce_flame->item_info[0].item_reserv = pchengce_flame->to_pc_type == TEST_GOOD? 0:0x8000;

		unsigned char checksum = 0;
		for (int i = 1; i <= pchengce_flame->frame_data_num; i++) {
			checksum ^= buf[i];
		}
		((unsigned char *)(&pchengce_flame->item_info[pchengce_flame->item_num]))[0] = checksum;		// checksum
		((unsigned char *)(&pchengce_flame->item_info[pchengce_flame->item_num]))[1] = 0xf0;			//

		chengce_text[0].title1 = str;
		chengce_saoma_ID[0] = 0;				// 清扫码 ID
		return pchengce_flame->frame_data_num + 3;
	}else {										// 上传的是扫码ID
		buf[SAOMA_ID_LEN - 1] = 0;// 截断
		strcpy_s(chengce_saoma_ID, buf);		// 覆盖式
		return 0;
	}
}
#endif
//=============================================================================

#ifdef CLIENT_HTTP
#define POST_URL	"https://oapi.dingtalk.com/robot/send?access_token=5de05218ebb592ecf7027f07a972cfd89f892231370d64b9d6c5f9052b5bc360"
/*
 *  测试数据发送的 JSON 格式 :
 *	{
 *		"data_type": "testdata2server",
 * 		"test_items": {
 * 			"systime": "20180728 15:07:00",
 *			"product_name":"XXXXXXX",
 *			"id":"0320494596"，
 *			... ...
 *		}
 *	}
 *  返回Json格式
 *	{
 *		"errmsg": "ok",
 *		"errcode": 0
 *	}
*/

BOOL http_json_post(const char* str_json)	// str_json 如： "{\"msgtype\": \"text\",\"text\": {\"content\": \"mfc -> hello world!\"}}";
{ 
	string ret;

	http_client.HttpPost(POST_URL,str_json,ret);

	Json::Reader reader;
	Json::Value json_object;
	//const char* json_document = "{\"age\" : 26,\"name\" : \"huchao\"}";
	reader.parse(ret, json_object);
	if (json_object["errcode"] == 0) return TRUE;
	else return FALSE;
}

#endif

/*
 *   Json拼接
*/

/*
Json::Value json_temp;										// 临时对象，供如下代码使用
json_temp["name"] = Json::Value("huchao");
json_temp["age"] = Json::Value(26);
Json::Value root;  // 表示整个 json 对象
root["key_string"] = Json::Value("value_string");			// 新建一个 Key（名为：key_string），赋予字符串值："value_string"。
root["key_number"] = Json::Value(12345);					// 新建一个 Key（名为：key_number），赋予数值：12345。
root["key_boolean"] = Json::Value(false);					// 新建一个 Key（名为：key_boolean），赋予bool值：false。
root["key_double"] = Json::Value(12.345);					// 新建一个 Key（名为：key_double），赋予 double 值：12.345。
root["key_object"] = json_temp;								// 新建一个 Key（名为：key_object），赋予 json::Value 对象值。
root["key_array"].append("array_string");	//数组			// 新建一个 Key（名为：key_array），类型为数组，对第一个元素赋值为字符串："array_string"。
root["key_array"].append(1234);								// 为数组 key_array 赋值，对第二个元素赋值为：1234。
Json::ValueType type = root.type();							// 获得 root 的类型，此处为 objectValue 类型。
Json::FastWriter fast_writer;
TRACE("%s\r\n", fast_writer.write(root).c_str());
Json::StyledWriter styled_writer;
TRACE("%s\r\n", styled_writer.write(root).c_str());
输出结果分别为：
{"key_array":["array_string",1234],"key_boolean":false,"key_double":12.3450,"key_number":12345,"key_object":{"age":26,"name":"huchao"},"key_string":"value_string"}
和
// 字符串格式化
{

   "key_array" : [ "array_string", 1234 ],
   "key_boolean" : false,
   "key_double" : 12.3450,
   "key_number" : 12345,
   "key_object" : {
      "age" : 26,
      "name" : "huchao"
   },
   "key_string" : "value_string"
}
*/

/*
 *   Json 解析
 */

/*
 	Json::Reader reader;
	Json::Value json_object;
	const char* json_document = "{\"age\" : 26,\"name\" : \"huchao\"}";
	if (!reader.parse(json_document, json_object))
		return 0;
	std::cout << json_object["name"] << std::endl;
	std::cout << json_object["age"] << std::endl;

	输出结果为：
		"huchao"
		26
*/ 