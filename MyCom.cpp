#include "stdafx.h"
#include "Mycom.h"
#include "resource.h"		// ��������Դ�ļ���������벻��
#include "MenuTestDoc.h"
#include "MainFrm.h"
#include "MenuTestView.h"

#include "json/json.h"

#ifdef CLIENT_HTTP
#include "HttpClient.h"

CHttpClient http_client;
/*
 *  ʾ�����룺   ��https���󵽰�����������̨��
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
// ����
CMyCom::CMyCom(LPCSTR Com, int BaudRate)
{
	RdIndex = 0;
	WrIndex = 0;

	//  ��ʼ��ACK֡ �� 0xfa + 1(�ֽ���) + 0x55(CMD) + 0x55(checksum) + 0xf0     
	ack[0] = UART_START;ack[1] = 2;ack[2] = MIAN_CMD_ACK;ack[3] = 0;ack[4] = UART_END;
	// ���ô���
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

// uart��ʼ��  �����ڲ������ò��������ϵͳ�����֣�
BOOL CMyCom::uart_init()
{
	// �����첽��д�¼�
	hEventR = CreateEvent(NULL,TRUE,FALSE,NULL);
	hEventW = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_handle = CreateEvent(NULL,FALSE,FALSE,NULL);			// ���߳��������ź�(�Զ���λ�¼�״̬����ʼ״̬δ����)
	wait_handle = CreateEvent(NULL,FALSE,TRUE,NULL);		// �ٽ��� ���ڽ��յ������̹߳���������������ͬ�� ����ʼ״̬������
	com_handle = CreateEvent(NULL,FALSE,TRUE,NULL);			// ������Դ�ٽ���
	jindutiao_handle = CreateEvent(NULL,FALSE,TRUE,NULL);	// ��������Դ�ٽ���

	// �������ڼ���߳�
	CWinThread* pThread = AfxBeginThread(CommWatchProc, this);
	// ������������ִ���߳�
	CWinThread* pThreadRevCmd = AfxBeginThread(RevComProc, this);
	// �����첽�߳�
	CWinThread* pThreadSend = AfxBeginThread(SendProc, this);

	SetTimer(NULL, ID_TIMER, 200, TimerProc);	// WinAPI �ط���ʱ��

	return TRUE;
}

DWORD CMyCom::ReSetComPort(LPCSTR Com)
{
	// �ر�ԭ�д���
	bOpen = FALSE;Sleep(20);
	if (hComDev != INVALID_HANDLE_VALUE) {
		CloseHandle(hComDev);Sleep(20);
	}
	// ����COM
	return SetupMyCom(Com, UART_BAUD_RATE);
}

BOOL CMyCom::SetupMyCom(LPCSTR Com, int BaudRate)

{
	DCB dcb;
//	COMMTIMEOUTS timeouts;

	if(bOpen)return FALSE;            // ���ڱ�ռ��
	// ��COM4
	hComDev = CreateFile(Com, GENERIC_READ|GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	if(hComDev == INVALID_HANDLE_VALUE)
	{
#ifndef _DEBUG
		MessageBox(NULL, _T("���ڶ˿�������Ч!!"), _T("TRACE"), MB_OK);
#endif
		return FALSE;                    // �򿪶˿�ʧ��
	}

	// ���ó�ʱ����
//	SetCommTimeouts(hComDev, &timeouts);
	// ���ý��ջ�����������������Ĵ�С
	SetupComm(hComDev, CMD_SIZE_MAX, 1024);
	// ��ȡȱʡ��DCB�ṹ��ֵ
	GetCommState(hComDev, &dcb);

	// �趨������Ϊ115200 bps
	dcb.BaudRate = BaudRate;
	// �趨����żУ��
	dcb.fParity = NOPARITY;
	// �趨����λΪ8
	dcb.ByteSize = 8;
	// �趨һ��ֹͣλ
	dcb.StopBits = ONESTOPBIT;

	// �趨��Ӧ���Ӵ��ڵĴ���ͽ��յ��ַ������¼���Ӧ
	SetCommMask(hComDev, EV_ERR|EV_RXCHAR);
	// ���ô����豸���Ʋ���
	SetCommState(hComDev, &dcb);
	// ���ò�����ʾ�豸�Ѵ�
	bOpen = TRUE;
	
	return TRUE;
}

// ���ݷ���  
// ���ݷ�������WriteFile����ʵ�֡�����ͬ��I/O�������������һ��������Ϊ
// NULL�������첽I/O�������������һ������������һ��ָ��OVERLAPPED�ṹ��ָ�룬
// ͨ��OVERLAPPED�ṹ����õ�ǰ�Ĳ���״̬��
// lpSndBufferΪ�������ݻ�����ָ�룬  
// dwBytesToWriteΪ��Ҫ���͵��ֽڳ���
BOOL CMyCom::WriteMyCom(LPVOID lpSndBuffer, DWORD dwBytesToWrite)
{

	BOOL bWriteState;
	DWORD dwBytesWritten;				// ʵ�ʷ��͵��ֽ���
	OVERLAPPED olRead;
	memset(&olRead,0,sizeof(OVERLAPPED));
	olRead.hEvent = hEventW;


	if(!bOpen) {TRACE("Send Err 0");return FALSE;}				// ����δ��
	bWriteState = WriteFile(hComDev, lpSndBuffer, dwBytesToWrite, &dwBytesWritten, &olRead);
	if(!bWriteState) 
	{
		if (GetLastError() == ERROR_IO_PENDING)	// ����ERROR_IO_PENDING,�����������ڽ��ж����� 
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

// ���ݽ���  
// �������ݵ�������ReadFile������ɡ��ú����Ӵ��ڽ��ջ������ж�ȡ���ݣ�
// ��ȡ����ǰ������ClearCommError������ý��ջ������е��ֽ���������
// ����ʱ��ͬ��
// ���첽��ȡ�Ĳ��ͬ����������һ���ġ�  
// lpInBufferΪ�������ݵĻ�����ָ�룬
// dwBytesToReadΪ׼����ȡ�����ݳ���(���ֽ���)
DWORD CMyCom::ReadMyCom(LPVOID lpInBuffer, DWORD dwBytesToRead)
{
	BOOL bReadState = FALSE;
	COMSTAT ComStat; // �����豸״̬�ṹ
	DWORD dwBytesRead, dwErrorFlags;
	OVERLAPPED olRead;
	memset(&olRead,0,sizeof(OVERLAPPED));
	olRead.hEvent = hEventR;


	if(!bOpen)return 0; // ����δ��
	// ���ڶ�ȡ�����豸�ĵ�ǰ״̬
	ClearCommError(hComDev, &dwErrorFlags, &ComStat);
	// �趨Ӧ�ö�ȡ�����ݳ���
	dwBytesRead = min(dwBytesToRead, ComStat.cbInQue);
	
	// ��ȡ����  
	if(0 == dwBytesRead) return 0;
	bReadState = ReadFile(hComDev, lpInBuffer, dwBytesRead, &dwBytesRead, &olRead);
	if(!bReadState) // ��������ʧ��(�����첽��δ���)
	{
		if (GetLastError()==ERROR_IO_PENDING)	// ����ERROR_IO_PENDING,�����������ڽ��ж����� 
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

	// ��������ȡ������
	return dwBytesRead;
}

extern int fg_chengce;
extern int ConvertRecvBuf(char *buf, int num);
// ���ڼ����߳�
UINT CMyCom::CommWatchProc(LPVOID pParam)
{
	// pThread->m_bAutoDelete = TRUE;
	HWND hWnd = (HWND)pParam;
	UINT8 rbuf[CMD_SIZE_MAX];	// ���ڽ��ܴ�������
	DWORD read;					// ���洮�ڻ������е�����
	DWORD dwEventMask = 0;		// �������¼�
	CMyCom *pMyCom = (CMyCom *)pParam;
	char COM_chengce[128];int chengce_sum = 0;

	while(1)				// �����ѱ��ɹ���
	{
		if (pMyCom->bOpen) {/*
			WaitCommEvent(pMyCom->hComDev, &dwEventMask, NULL);
			if((dwEventMask & EV_RXCHAR) == EV_RXCHAR)
*/			{
				// ���յ��ַ����ڴ˼��������д���
				read = pMyCom->ReadMyCom(rbuf);
				if(read > 0)
				{  
#ifdef AUTO_TOOL_18V_TEST
					if (fg_chengce)	{
						// �ɲ�һ���Խ����ڶ���
						do{
							memcpy(COM_chengce + chengce_sum, rbuf, read);
							chengce_sum += read;
							Sleep(100);
						}while((read = pMyCom->ReadMyCom(rbuf)) > 0);
						memcpy(rbuf, COM_chengce, chengce_sum);
						read = chengce_sum;
						chengce_sum = 0;
						read = ConvertRecvBuf((char*)rbuf, read);	// ����ǳɲ⣬ת���յ����ַ�����ʹ֮����Э��Ҫ���ʽ
						if (read == 0) continue;
					}
#endif
					// �����
					pMyCom->DataToRevBuf(rbuf, read);
					//if (!fgClose)
					{
	#ifdef DEBUG_CHAR
						for(int i=0; i<(int)read; i++)
						{	
							if (0 == kk){kk = 1;RevDat.GetBufferSetLength(1024*1024);}	// ���õ����������С}
							// Debug Dvd�����Ϸ���Ϣ
							RevDat.Format(L"%s %.2X",RevDat, rbuf[i]);
							if (++count % 20 == 0) RevDat.Format(L"%s %s",RevDat, L"\r\n");
						}
						if (pView)	{	// �ȴ����ڴ������
							pView->m_DVDInfo.SetWindowText(RevDat);
							pView->m_DVDInfo.LineScroll(pView->m_DVDInfo.GetLineCount());//�������õ�
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

// ��������  // ������Ϊ�����뺯��
int CMyCom::UartSendCmd(UINT8 *sentbuf, int bytes_num, char re_send)		// bytes_num  ���ֽ���������λ0xfa,0xf0��
{
#ifdef AUTO_TOOL_18V_TEST
	return 1;	// Ŀǰ�綯������λ��������ָ�� 20180609
#endif

#ifndef SIMULATE_TOPC

	TRACE("����\r\n");
	int i;
	UINT8 ret = 2;

#ifdef _NO_ACK
	re_send = 1;
#endif
	if (0 == re_send) {
		// ���ݣ��Ա��ط�
		pLastFlame = sentbuf;
		LastFlameNum = bytes_num;
	}

	WaitForSingleObject(com_handle, INFINITE);			// ������Դ�ı�����

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

	// ���ݴ˴η��͵�����
	//memcpy(CmdBak, sentbuf, DSA_BUF_LENTH);
	//ResendTime = 0;
	//for (int j = 0; j < 16; j++)
	//	TRACE("%d  ", sentbuf[j]);
	//TRACE("\r\n");

	if (0 == re_send) {
		fg_ack = init_ack;
		wait_ack
		if (time_over_ack == fg_ack) return 2;		// ack ��ʱ
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
	WaitForSingleObject(pCom->jindutiao_handle, INFINITE);			// ��������Դ�ı�����
	pCurrView->Deliver_Dlg.DoModal();
	SetEvent(pCom->jindutiao_handle);

	return 1;
}
int fg_moudle_item_debug = 0;	// debug topcʹ��
// �����߳�
UINT CMyCom::SendProc(LPVOID pParam)
{
	int ret = 0;
	CMyCom *pCom = (CMyCom *)pParam;
	while (1) {

		WaitForSingleObject(pCom->m_handle, INFINITE);
			
		// ����������
		uart_pos = 0;
		CWinThread* pThread = AfxBeginThread(jindutiao_func, pParam);
#ifndef SIMULATE_TOPC
		// ��ʼUART����
		// 1�������������ִ���
		if (2 == send_bace_param_code(& setting)) {
			AfxMessageBox("ͨ�Ź��ϣ�");
			continue;
		}
		uart_pos = 10;
		// 2 ���Ʋ��� start ʱ����
		ret = send_program_code(MIAN_CMD_PROGRAM_START);
		if (0 == ret) {
			AfxMessageBox("�����߼��������������﷨���⣡");
			continue;
		}else if (2 == ret) {
			AfxMessageBox("ͨ�Ź��ϣ�");
			continue;
		}
		uart_pos = 30;
		// 3 ���Ʋ��� end ʱ����
		ret = send_program_code(MIAN_CMD_PROGRAM_STOP);
		if (0 == ret) {
			AfxMessageBox("�����߼���ͣ���������﷨���⣡");
			continue;
		}else if (2 == ret) {
			AfxMessageBox("ͨ�Ź��ϣ�");
			continue;
		}
#endif
		uart_pos = 50;
		// 2, ���Ʋ��ִ���
		if (0 == fg_normal_program) {
			// 4 ���Ʋ��ֱ��ʱ����
			ret = send_program_code(MIAN_CMD_PROGRAM_OTHER);
			if (0 == ret) {
				AfxMessageBox("�����߼������ԡ������﷨���⣡");
				continue;
			}else if (2 == ret) {
				AfxMessageBox("ͨ�Ź��ϣ�");
				continue;
			}
			uart_pos = 100;
#ifdef SIMULATE_TOPC
			fg_moudle_item_debug = 1;
#endif
		}else {
			// 5�������ѡ���ִ���
			if (2 == send_normal_code()) {
				AfxMessageBox("ͨ�Ź��ϣ�");
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
				TRACE("�ط�\r\n");
				MyCom.UartSendCmd(pLastFlame, LastFlameNum, 1);
			}else {
				fg_ack = time_over_ack;
				re_send_times = 0;
				//AfxMessageBox("���ͳ�ʱ��");
			}
		}
	}else {
		re_send_times = 0;
		loop_times = 0;
	}
}

// ���������뻺��
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
// ���ڶ�������
DWORD CMyCom::ReadCmd()
{
	int CRC = 0;
	int frame_length = 0;		// ���ò�����֡
	DWORD ret = 0;

	WaitForSingleObject(wait_handle, INFINITE);

	while (RdIndex < WrIndex && AfxRevBuf[RdIndex] != UART_START) RdIndex++;		// ������ͷ
	if (RdIndex +1 < WrIndex) {
		// �ҵ�����ͷ
		frame_length = AfxRevBuf[RdIndex + 1] +3;									// ������֡ʵ�ʳ���
		if (WrIndex >= RdIndex + frame_length) {
			for (int i = 1; i < frame_length - 2; i++)
				CRC ^= AfxRevBuf[RdIndex + i];
			if (AfxRevBuf[RdIndex + frame_length - 1] == UART_END
				&& CRC == AfxRevBuf[RdIndex + frame_length - 2]) {

				memcpy(Cmd, AfxRevBuf + RdIndex, frame_length);					// �յ�һ����������
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

// TOPC����

//	1��	����
//	2,	��������
//  3,	ת��
//	4�� ת�ٹ���
//	5,	Ƿѹ
//	6��	Ƿѹ����
//	7,	Ƿѹ�ָ�
//	8,	Ƿѹ�ָ�����
//	9,	ת��
//	10,	����ʱ��
//	11,	����ʱ�乫��
//	12,	ɲ��
//	13,	NTC���½���
//	14,	NTC���»ָ�
//	15,	NTC���½���
//	16,	NTC���»ָ�
//	17��NTC����
//	18�����������
//const CString test_item_string[] = {"����", "ת��", "Ƿѹ", "Ƿѹ�ָ�", "ת��", "����ʱ��","ɲ��", "NTC���½���", "NTC���»ָ�", "NTC���½���", "NTC���»ָ�", ""};
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
//��ǰ��������
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
			// �յ�һ����������
			// ������ִ��Cmd
			CString sql_save_string = "";
			CString disp_info = "";
#ifdef SIMULATE_TOPC
			TEST_INFO *pInfo = (TEST_INFO *)debug_pakect;
#else
			TEST_INFO *pInfo = (TEST_INFO *)pCom->Cmd;
#endif
			// send ACK
			if (pInfo->to_pc_type != MIAN_CMD_ACK)	pCom->UartSendCmd(pCom->ack, ACK_FLAME_NUM, 1);			// ���� ACK

			switch (pInfo->to_pc_type)
			{
			case TEST_GOOD:														// ��Ʒ
				//break;
			case TEST_NG:														// ����Ʒ
				disp_info += topc_data_report(pInfo);
				if (disp_info == "") break;	// topc_data_report���� ""���쳣
				pCurrView->GetDlgItem(IDC_RESULT)->SetWindowText(pInfo->to_pc_type == TEST_GOOD? "�ɹ�" : "ʧ��");
				// ��������
				{
				CTime t = CTime::GetCurrentTime(); //��ȡϵͳ����
				int d=t.GetDay(); //��ü���
				int y=t.GetYear(); //��ȡ���
				int m=t.GetMonth(); //��ȡ��ǰ�·�
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
				Json::Value test_items;  // �������Json���
				test_items["date_time"] = Json::Value(date_time.GetBuffer());
				test_items["project_name"] = Json::Value(name.GetBuffer());
				test_items["saoma_id"] = Json::Value(pInfo->saoma_id);
#endif
				CString temp;
				for (int i = 0;i < DB_SEGMENT_ITEM_NUMMAX; i++) {
					temp = sql_save_string;
					if (i < modle_num) {
						sql_save_string.Format("%s, '%s', '%s'", temp,modle_item[i].type, modle_item[i].value);//ʵ����
#ifdef CLIENT_HTTP
						test_items[modle_item[i].type] = Json::Value(modle_item[i].value.GetBuffer());
#endif
					}else if (i < total_item_num){
						sql_save_string.Format("%s, '%s', '%s'", temp, "--", "--");	// '--'��ʾ����NG֮��δ����
#ifdef CLIENT_HTTP
						test_items["--"] = Json::Value("--");
#endif
					}else {
						sql_save_string.Format("%s, '%s', '%s'", temp, "", ""); // ��������ݿ��ǣ�
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
				strcpy_s(str_json, fast_writer.write(root).c_str());	// �˴���ֻ�ܿ���������ָ�븳ֵ

				TRACE("\r\n JSON : %s",str_json);
#endif

#ifdef DATA_BACE				
				// 1�����ش洢
				if (p_cp_data != NULL) p_cp_data->Execute(sql_save_string);
#endif

#ifdef SYS_CLEINT
				// 2�������б���ʾ
				pCurrView->disp_current_test_info_line(sql_save_string);
#endif
				// 3���������͵�Զ�̷�����
#ifdef MGT_SOCKET_CLIENT
				// ��������  sql_save_string  �����ͱ������ݿ�洢ģʽ�ͷ�����һ��һ��
				if (fg_client_socket_conect_ok) pCurrView->client_send(sql_save_string);
				else {
					// �����ݴ� (���ܴ�����)������
					// ...
				}
#elif defined(CLIENT_HTTP)
				// ͨ�� HTTP (POST)������ JSON��ʽ���Ͳ������ݵ�������
				http_json_post(str_json);
#endif
				// 4�����ز�����Ϣ����ʾ
				disp_info += (pInfo->to_pc_type == 0? "���Խ������Ʒ\r\n\r\n" : "���Խ��������Ʒ\r\n\r\n");
				if (total_disp_info.GetLength() > 100000) total_disp_info = "";// ���ַ���
				total_disp_info += disp_info;				
				pCurrView->m_topc_disp.SetWindowText(total_disp_info);
				pCurrView->GetDlgItem(IDC_DISP)->SendMessage(WM_VSCROLL,SB_BOTTOM,0);
				// 5������״̬����������
				{
					test_num++;		// ����
					if (pInfo->to_pc_type) ng_num++;	// ������
					CString str_num = "";
					str_num.Format("��ǰ����������%d  ����������%d", test_num, ng_num);
					::SendMessage(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_wndStatusBar.m_hWnd,SB_SETTEXT,4,(LPARAM)str_num.GetBuffer());// ����״̬����ʾ
				}
				// 6,���²��Խ��ͼ��
				//pCurrView->InvalidateRect(&Rect_pic_ok,true);
				//pCurrView->UpdateWindow();
				}
				break;
			case SELFCHECK_FAULT:												// ϵͳ�Լ����
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

	// item_value ���ڵ�һ�����ݣ�item_reserv����15λ�����ڵڶ�������
	// Ƿѹ��Ҫ���������������ݵ�
	// 15bit���ֵ ��ʾ�����ͣ
	// dd_text[]�����Ϲ̶�����
	typedef struct {
		char data_num;	// ����һ����Լ�������:0,1,2
		CString title1;	// �̶�����1
		CString title2;	// �̶�����2
		CString unit;	// ��λ
	}diandong_dispinfo_text;

	// ������˴���ʼ��ֵ�������⣬������ͨ�������ļ�������ֵ��ֵ���������顣���������GetDiandongToolTestItem()
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
	diandong_dispinfo_text chengce_text[20] = {{0, "", "", ""}, {0,"","",""}};	// �ɲ�ʹ��

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
		// ɾ����������ע���ַ���
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

	ret_str_item_info.Format("ɨ��ID:  %s\r\n",pInfo->saoma_id);

	for (int i = 0; i < pInfo->item_num; i++) {
		// ֵ
#ifndef AUTO_TOOL_18V_TEST		// ԰�ֹ���
		str_item_type = item_content[i].item_content;	// �ֶ�
		if(pInfo->item_info[i].item_type == FAULT_CURRENT) {
			str_item_value.Format("%.1f A",pInfo->item_info[i].item_value / 10.0);
			if (pInfo->item_info[i].item_reserv & 0x8000) str_item_value += "  (NG)";
		}else if (pInfo->item_info[i].item_type > FAULT_DZ_AD(0) && item_content[i].item_content.Find("ѹ") != -1) {
			str_item_value.Format("%d mv",pInfo->item_info[i].item_value);
			if (pInfo->item_info[i].item_reserv & 0x8000) str_item_value += "  (NG)";
		}else if (pInfo->item_info[i].item_type == FAULT_SPEED && item_content[i].item_content.Find("��") != -1){
			str_item_value.Format("%d RPM",pInfo->item_info[i].item_value);
			if (pInfo->item_info[i].item_reserv & 0x8000) str_item_value += "  (NG)";
		}else if (pInfo->item_info[i].item_type == FAULT_POWER && item_content[i].item_content.Find("����") != -1) {
			str_item_value.Format("%d w",pInfo->item_info[i].item_value);
			if (pInfo->item_info[i].item_reserv & 0x8000) str_item_value += "  (NG)";
		}else {
			str_item_value = (pInfo->item_info[i].item_reserv & 0x8000)? "NG" : "OK";
		}
#else							// �綯����
		if (fg_chengce && pInfo->item_num > 1) {AfxMessageBox("����ȷѡ�����ģʽ�����⣿ ���� �ɲ⣿"); return "";} // ѡ�˳ɲᣬʵ�ʳ�������
		diandong_dispinfo_text *dd_text = NULL;
		if (fg_chengce == 0) {
			str_item_type = item_content[i].item_content;	// �ֶ�
			dd_text = chuce_text;
		}else {
			str_item_type = "";	// �ֶ�
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
		if (fg_chengce == 0) str_item_value += (pInfo->item_info[i].item_reserv & 0x8000)? " NG" : " OK";	// ֻ��Գ��� ��׺ NG/OK
#endif
		
		// ���ݿ����ݹ��� (Ŀǰ��֧�ֲ��Ե����������ֹͣ���ԣ��Ϸ�����)
		if ((pInfo->item_info[i].item_reserv & 0x8000) || item_content[i].fg_half == 1) {
			modle_item[modle_num].type = str_item_type;
			modle_item[modle_num++].value = str_item_value;
			ret_str_item_info += str_item_type + ":" + str_item_value + " " + "\r\n";
		}
		// �������¼
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
//============================   �ɲ����ݼ�¼   ===========================
// ���Խ��ɨ�룺
// $chengce��OK
// $chengce��NG
// $chengce��xxx NG

char chengce_saoma_ID[SAOMA_ID_LEN] = {0};
CString str_chegncebuliang[] = {"OK","NG", "xxx NG", "xxxx NG", "xxxxx NG", "", "", ""};
// ��������ֵ��0���м�����  >=1: ģ���һ������֡������֡bytes����
int ConvertRecvBuf(char *buf, int num)
{
	if (num >= 2) buf[num - 2] = 0;	// ȥ��ɨ��ǹĩβ��\r\n
	else buf[num] = 0;
	CString str(buf);
	str.MakeLower();
	if (str.Find("$�ɲ�") != -1) {	// �ϴ����ǲ��Խ��
		if (chengce_saoma_ID[0] == 0) {
			AfxMessageBox("����ɨ���Ʒ��ˮ�ţ�лл��");
			return 0;
		}
		// �����ٵ�֡Э��
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
		chengce_saoma_ID[0] = 0;				// ��ɨ�� ID
		return pchengce_flame->frame_data_num + 3;
	}else {										// �ϴ�����ɨ��ID
		buf[SAOMA_ID_LEN - 1] = 0;// �ض�
		strcpy_s(chengce_saoma_ID, buf);		// ����ʽ
		return 0;
	}
}
#endif
//=============================================================================

#ifdef CLIENT_HTTP
#define POST_URL	"https://oapi.dingtalk.com/robot/send?access_token=5de05218ebb592ecf7027f07a972cfd89f892231370d64b9d6c5f9052b5bc360"
/*
 *  �������ݷ��͵� JSON ��ʽ :
 *	{
 *		"data_type": "testdata2server",
 * 		"test_items": {
 * 			"systime": "20180728 15:07:00",
 *			"product_name":"XXXXXXX",
 *			"id":"0320494596"��
 *			... ...
 *		}
 *	}
 *  ����Json��ʽ
 *	{
 *		"errmsg": "ok",
 *		"errcode": 0
 *	}
*/

BOOL http_json_post(const char* str_json)	// str_json �磺 "{\"msgtype\": \"text\",\"text\": {\"content\": \"mfc -> hello world!\"}}";
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
 *   Jsonƴ��
*/

/*
Json::Value json_temp;										// ��ʱ���󣬹����´���ʹ��
json_temp["name"] = Json::Value("huchao");
json_temp["age"] = Json::Value(26);
Json::Value root;  // ��ʾ���� json ����
root["key_string"] = Json::Value("value_string");			// �½�һ�� Key����Ϊ��key_string���������ַ���ֵ��"value_string"��
root["key_number"] = Json::Value(12345);					// �½�һ�� Key����Ϊ��key_number����������ֵ��12345��
root["key_boolean"] = Json::Value(false);					// �½�һ�� Key����Ϊ��key_boolean��������boolֵ��false��
root["key_double"] = Json::Value(12.345);					// �½�һ�� Key����Ϊ��key_double�������� double ֵ��12.345��
root["key_object"] = json_temp;								// �½�һ�� Key����Ϊ��key_object�������� json::Value ����ֵ��
root["key_array"].append("array_string");	//����			// �½�һ�� Key����Ϊ��key_array��������Ϊ���飬�Ե�һ��Ԫ�ظ�ֵΪ�ַ�����"array_string"��
root["key_array"].append(1234);								// Ϊ���� key_array ��ֵ���Եڶ���Ԫ�ظ�ֵΪ��1234��
Json::ValueType type = root.type();							// ��� root �����ͣ��˴�Ϊ objectValue ���͡�
Json::FastWriter fast_writer;
TRACE("%s\r\n", fast_writer.write(root).c_str());
Json::StyledWriter styled_writer;
TRACE("%s\r\n", styled_writer.write(root).c_str());
�������ֱ�Ϊ��
{"key_array":["array_string",1234],"key_boolean":false,"key_double":12.3450,"key_number":12345,"key_object":{"age":26,"name":"huchao"},"key_string":"value_string"}
��
// �ַ�����ʽ��
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
 *   Json ����
 */

/*
 	Json::Reader reader;
	Json::Value json_object;
	const char* json_document = "{\"age\" : 26,\"name\" : \"huchao\"}";
	if (!reader.parse(json_document, json_object))
		return 0;
	std::cout << json_object["name"] << std::endl;
	std::cout << json_object["age"] << std::endl;

	������Ϊ��
		"huchao"
		26
*/ 