// CodeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MenuTest.h"
#include "CodeDlg.h"


// CCodeDlg 对话框

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
CString op_name = "士腾-罗新君";
CString factory_name = "杭州士腾";

BOOL CCodeDlg::OnInitDialog()
{
	BOOL ret = CDialog::OnInitDialog();
	GetDlgItem(IDC_USER)->SetWindowText(op_name);
	return ret;
}

// CCodeDlg 消息处理程序

void CCodeDlg::OnBnCodeOk()
{
	// TODO: 在此添加控件通知处理程序代码

	static int input_code_num = 0;

	CString codeTemp = "";
	GetDlgItem(IDC_EDIT1)->GetWindowText(codeTemp);

	CStdioFile File;
	if(File.Open(CODE_FILE,CStdioFile::modeRead))//创建/打开文件
	{
		File.ReadString(strCode);
	}

	// 获取加工厂
	GetDlgItem(IDC_FACTROY_NAME)->GetWindowText(factory_name);
	if (factory_name.Trim() == "") {
		AfxMessageBox("工厂名不能为空！");
		return;
	}

	if (codeTemp == strCode) {
		//AfxMessageBox("登录成功！");
		GetDlgItem(IDC_USER)->GetWindowText(op_name);	// 记录操作人员
	}else {
		AfxMessageBox("密码错误，请重新输入！");
		GetDlgItem(IDC_EDIT1)->SetWindowText("");
		if (++input_code_num > 3) {
			AfxMessageBox("请向管理员获取密码，再见！");
			OnBnClickedCancel();
		}
		return;
	}

	OnOK();
}

void CCodeDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxGetApp()->m_pMainWnd->PostMessage(WM_CLOSE);

	OnCancel();
}
