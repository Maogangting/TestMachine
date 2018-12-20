// SetCode.cpp : 实现文件
//

#include "stdafx.h"
#include "MenuTest.h"
#include "SetCode.h"


// CSetCode 对话框

IMPLEMENT_DYNAMIC(CSetCode, CDialog)

CSetCode::CSetCode(CWnd* pParent /*=NULL*/)
	: CDialog(CSetCode::IDD, pParent)
{

}

CSetCode::~CSetCode()
{
}

void CSetCode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSetCode, CDialog)
	ON_BN_CLICKED(IDOK, &CSetCode::OnBnSetCodeOk)
END_MESSAGE_MAP()


// CSetCode 消息处理程序
extern CString strCode;
void CSetCode::OnBnSetCodeOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString codeTemp = "";
	GetDlgItem(IDC_EDIT1)->GetWindowText(codeTemp);

	if (codeTemp == strCode) {
		//AfxMessageBox("登录成功！");
		GetDlgItem(IDC_EDIT8)->GetWindowText(codeTemp);	
		int len = codeTemp.GetLength();
		if (len >= 6 && len <= 10) 						// 对新密码进行合法性检验
		{
			// 此时 codeTemp 为新密码
			CStdioFile file;

			strCode = codeTemp;
			if(file.Open(CODE_FILE,CStdioFile::modeCreate|CStdioFile::modeNoTruncate|CStdioFile::modeWrite))//创建/打开文件
			{
				file.WriteString(strCode);//写入内容
				file.Close();//关闭输出文件
			}
			AfxMessageBox("密码修改成功！");
		}else {
			AfxMessageBox("新密码长度出错！");
			GetDlgItem(IDC_EDIT1)->SetWindowText("");
			GetDlgItem(IDC_EDIT8)->SetWindowText("");
			return;
		}
		
	}else {
		AfxMessageBox("原密码错误！");
		GetDlgItem(IDC_EDIT1)->SetWindowText("");
		GetDlgItem(IDC_EDIT8)->SetWindowText("");
		return;
	}

	OnOK();
}
