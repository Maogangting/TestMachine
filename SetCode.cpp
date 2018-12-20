// SetCode.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MenuTest.h"
#include "SetCode.h"


// CSetCode �Ի���

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


// CSetCode ��Ϣ�������
extern CString strCode;
void CSetCode::OnBnSetCodeOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString codeTemp = "";
	GetDlgItem(IDC_EDIT1)->GetWindowText(codeTemp);

	if (codeTemp == strCode) {
		//AfxMessageBox("��¼�ɹ���");
		GetDlgItem(IDC_EDIT8)->GetWindowText(codeTemp);	
		int len = codeTemp.GetLength();
		if (len >= 6 && len <= 10) 						// ����������кϷ��Լ���
		{
			// ��ʱ codeTemp Ϊ������
			CStdioFile file;

			strCode = codeTemp;
			if(file.Open(CODE_FILE,CStdioFile::modeCreate|CStdioFile::modeNoTruncate|CStdioFile::modeWrite))//����/���ļ�
			{
				file.WriteString(strCode);//д������
				file.Close();//�ر�����ļ�
			}
			AfxMessageBox("�����޸ĳɹ���");
		}else {
			AfxMessageBox("�����볤�ȳ���");
			GetDlgItem(IDC_EDIT1)->SetWindowText("");
			GetDlgItem(IDC_EDIT8)->SetWindowText("");
			return;
		}
		
	}else {
		AfxMessageBox("ԭ�������");
		GetDlgItem(IDC_EDIT1)->SetWindowText("");
		GetDlgItem(IDC_EDIT8)->SetWindowText("");
		return;
	}

	OnOK();
}
