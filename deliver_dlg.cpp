// deliver_dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MenuTest.h"
#include "deliver_dlg.h"


// deliver_dlg 对话框

IMPLEMENT_DYNAMIC(deliver_dlg, CDialog)

deliver_dlg::deliver_dlg(CWnd* pParent /*=NULL*/)
	: CDialog(deliver_dlg::IDD, pParent)
{

}

deliver_dlg::~deliver_dlg()
{
}

void deliver_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
}


BEGIN_MESSAGE_MAP(deliver_dlg, CDialog)
	ON_WM_TIMER()
	ON_WM_CLOSE()
//	ON_BN_CLICKED(IDC_BUTTON1, &deliver_dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// deliver_dlg 消息处理程序
extern int pos;
BOOL deliver_dlg::OnInitDialog()
{
	BOOL ret = CDialog::OnInitDialog();

	SetTimer(1, 100, NULL);
	m_progress.SetRange(0,100);
	m_progress.SetPos(pos);
	//GetDlgItem(IDOK)->EnableWindow(FALSE);

	return ret;
}
//extern int fgDeliver;
extern int uart_pos;
void deliver_dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent) 
	{
	case 1:
		//if (0 == fgDeliver && pos <80) {	// 自动到80%
		//	pos += 10;
		//	m_progress.SetPos(pos);
		//	break;
		//}else if (1 == fgDeliver){
		//	pos = 100;
			m_progress.SetPos(uart_pos);
			if (uart_pos == 100)	PostMessage(WM_CLOSE);		// 自动消失进度条
		//	GetDlgItem(IDOK)->EnableWindow(TRUE);
		//	KillTimer(1);
		/*}*/
	default: break;
	}
	CDialog::OnTimer(nIDEvent);
}


void deliver_dlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	KillTimer(1);
	CDialog::OnClose();
}
/*
extern int fg_ack;
extern  int loop_times ;
	extern  int re_send_times ;
void deliver_dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	loop_times = 0;re_send_times = 0;fg_ack = 1;
	TRACE("~~~~~~\r\n");
}
*/
