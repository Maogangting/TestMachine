// deliver_dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MenuTest.h"
#include "deliver_dlg.h"


// deliver_dlg �Ի���

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


// deliver_dlg ��Ϣ�������
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nIDEvent) 
	{
	case 1:
		//if (0 == fgDeliver && pos <80) {	// �Զ���80%
		//	pos += 10;
		//	m_progress.SetPos(pos);
		//	break;
		//}else if (1 == fgDeliver){
		//	pos = 100;
			m_progress.SetPos(uart_pos);
			if (uart_pos == 100)	PostMessage(WM_CLOSE);		// �Զ���ʧ������
		//	GetDlgItem(IDOK)->EnableWindow(TRUE);
		//	KillTimer(1);
		/*}*/
	default: break;
	}
	CDialog::OnTimer(nIDEvent);
}


void deliver_dlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	KillTimer(1);
	CDialog::OnClose();
}
/*
extern int fg_ack;
extern  int loop_times ;
	extern  int re_send_times ;
void deliver_dlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	loop_times = 0;re_send_times = 0;fg_ack = 1;
	TRACE("~~~~~~\r\n");
}
*/
