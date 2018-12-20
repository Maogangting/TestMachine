// IP_PORT.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MenuTest.h"
#include "MainFrm.h"
#include "MenuTestView.h"
#include "IP_PORT.h"


// CIP_PORT �Ի���

IMPLEMENT_DYNAMIC(CIP_PORT, CDialog)

CIP_PORT::CIP_PORT(CWnd* pParent /*=NULL*/)
	: CDialog(CIP_PORT::IDD, pParent)
{

}

CIP_PORT::~CIP_PORT()
{
}

void CIP_PORT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_IP);
}

extern CString IP,Port;
BOOL CIP_PORT::OnInitDialog()
{
	BOOL ret = CDialog::OnInitDialog();

	GetDlgItem(IDC_IPADDRESS1)->SetWindowText(IP);
	GetDlgItem(IDC_EDIT8)->SetWindowText(Port);
	return ret;
}

BEGIN_MESSAGE_MAP(CIP_PORT, CDialog)
	//ON_COMMAND(ID_32781, &CIP_PORT::OnIpPortSetting)
	ON_BN_CLICKED(IDOK, &CIP_PORT::OnBnIpPortOk)
END_MESSAGE_MAP()


// CIP_PORT ��Ϣ�������
/*
void CIP_PORT::OnIpPortSetting()
{
	// TODO: �ڴ���������������
	ip_port.
}*/
CString IP = "192.168.3.3";
CString Port = "4999";
BYTE field1,field2,field3,field4;
void CIP_PORT::OnBnIpPortOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strTemp = "";
	// ���IP��port�������ȷ��
	m_IP.GetAddress(field1,field2,field3,field4);
	IP.Format("%d.%d.%d.%d", field1,field2,field3,field4);
	GetDlgItem(IDC_EDIT8)->GetWindowText(Port);
#ifdef MGT_SOCKET_CLIENT
	//���´����ͻ���SOCKET
	//pCurrView->qq_client_init();
	//pCurrView->GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);
	//pCurrView->socket_conect();
#endif
	CDialog::OnOK();
}
