#include "stdafx.h"

class CMyCom
{
public:
	enum{
		UART_START	= 0xfa,
		UART_END		= 0xf0,
		//SD_UART_START	= 0xf5,
		//TRANCELATE_CHAR = 0X5A,	// ת���ַ�
		COM_REV_BUF_SIZE= 10*1024,			// 10K
		CMD_SIZE_MAX	= 128,
		//BUF_SIZE		= 16,
		REV_FLAME_SIZE_MAX	= 256
	};

public:
	CMyCom(LPCSTR Com, int BaudRate);
	~CMyCom();
	
	
	BOOL uart_init();
	DWORD ReadCmd();
//	DWORD RequestSend(UINT8 *sentbuf, int bytes_num);
	int UartSendCmd(UINT8 *sentbuf,int bytes_num, char re_send);
	DWORD ReSetComPort(LPCSTR);		// ���ô��ں�
private:
	static UINT CommWatchProc(LPVOID pParam);						// ���ڼ���̺߳���
	static UINT SendProc(LPVOID pParam);							// �����߳�
	static UINT RevComProc(LPVOID pParam);							// ��������ִ���߳�
	static UINT jindutiao_func(LPVOID pParam);						// ���ͽ����������߳�

	BOOL SetupMyCom(LPCSTR Com, int BaudRate);     // ��ʼ���˿�
	BOOL WriteMyCom(LPVOID lpSndBuffer, DWORD dwBytesToWrite);      // д�˿�
	DWORD ReadMyCom(LPVOID lpInBuffer, DWORD dwBytesToRead=CMD_SIZE_MAX);      // ���˿�
	void DataToRevBuf(UINT8 *RevBuf, int DataNum);

public:
	UINT8	Cmd[REV_FLAME_SIZE_MAX];					// ���յ������
	HANDLE	m_handle;
	HANDLE	wait_handle;
	HANDLE	com_handle;
	HANDLE	jindutiao_handle;
private:
	HANDLE	hComDev;
	HANDLE	hEventR, hEventW;
	UINT8	AfxRevBuf[COM_REV_BUF_SIZE];		// �������ݻ��� 1M
	int		RdIndex,WrIndex;					// ���ڻ����д��
	static  BOOL	bOpen;
public:
	enum{
		ACK_FLAME_NUM	= 5,
	};
	UINT8 ack[ACK_FLAME_NUM];

};