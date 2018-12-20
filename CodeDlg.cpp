// CodeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MenuTest.h"
#include "CodeDlg.h"


// CCodeDlg �Ի���

IMPLEMENT_DYNAMIC(CCodeDlg, CDialog)

CCodeDlg::CCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCodeDlg::IDD, pParent)
{

}

CCodeDlg::~CCodeDlg()
{
}

void CCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCodeDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CCodeDlg::OnBnCodeOk)
	ON_BN_CLICKED(IDCANCEL, &CCodeDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

CString strCode = "121121";
CString op_name = "ʿ��-���¾�";
CString factory_name = "����ʿ��";

BOOL CCodeDlg::OnInitDialog()
{
	BOOL ret = CDialog::OnInitDialog();
	GetDlgItem(IDC_USER)->SetWindowText(op_name);
	return ret;
}

// CCodeDlg ��Ϣ�������

void CCodeDlg::OnBnCodeOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	static int input_code_num = 0;

	CString codeTemp = "";
	GetDlgItem(IDC_EDIT1)->GetWindowText(codeTemp);

	CStdioFile File;
	if(File.Open(CODE_FILE,CStdioFile::modeRead))//����/���ļ�
	{
		File.ReadString(strCode);
	}

	// ��ȡ�ӹ���
	GetDlgItem(IDC_FACTROY_NAME)->GetWindowText(factory_name);
	if (factory_name.Trim() == "") {
		AfxMessageBox("����������Ϊ�գ�");
		return;
	}

	if (codeTemp == strCode) {
		//AfxMessageBox("��¼�ɹ���");
		GetDlgItem(IDC_USER)->GetWindowText(op_name);	// ��¼������Ա
	}else {
		AfxMessageBox("����������������룡");
		GetDlgItem(IDC_EDIT1)->SetWindowText("");
		if (++input_code_num > 3) {
			AfxMessageBox("�������Ա��ȡ���룬�ټ���");
			OnBnClickedCancel();
		}
		return;
	}

	OnOK();
}

void CCodeDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AfxGetApp()->m_pMainWnd->PostMessage(WM_CLOSE);

	OnCancel();
}
