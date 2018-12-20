#include "stdafx.h"

class CMyCom
{
public:
	enum{
		UART_START	= 0xfa,
		UART_END		= 0xf0,
		//SD_UART_START	= 0xf5,
		//TRANCELATE_CHAR = 0X5A,	// 转意字符
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
	DWORD ReSetComPort(LPCSTR);		// 设置串口号
private:
	static UINT CommWatchProc(LPVOID pParam);						// 串口监测线程函数
	static UINT SendProc(LPVOID pParam);							// 发送线程
	static UINT RevComProc(LPVOID pParam);							// 串口命令执行线程
	static UINT jindutiao_func(LPVOID pParam);						// 发送进度条单独线程

	BOOL SetupMyCom(LPCSTR Com, int BaudRate);     // 初始化端口
	BOOL WriteMyCom(LPVOID lpSndBuffer, DWORD dwBytesToWrite);      // 写端口
	DWORD ReadMyCom(LPVOID lpInBuffer, DWORD dwBytesToRead=CMD_SIZE_MAX);      // 读端口
	void DataToRevBuf(UINT8 *RevBuf, int DataNum);

public:
	UINT8	Cmd[REV_FLAME_SIZE_MAX];					// 接收的命令备份
	HANDLE	m_handle;
	HANDLE	wait_handle;
	HANDLE	com_handle;
	HANDLE	jindutiao_handle;
private:
	HANDLE	hComDev;
	HANDLE	hEventR, hEventW;
	UINT8	AfxRevBuf[COM_REV_BUF_SIZE];		// 串口数据缓冲 1M
	int		RdIndex,WrIndex;					// 串口缓冲读写点
	static  BOOL	bOpen;
public:
	enum{
		ACK_FLAME_NUM	= 5,
	};
	UINT8 ack[ACK_FLAME_NUM];

};