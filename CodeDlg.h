#pragma once


// CCodeDlg �Ի���

class CCodeDlg : public CDialog
{
	DECLARE_DYNAMIC(CCodeDlg)

public:
	CCodeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCodeDlg();

// �Ի�������
	enum { IDD = IDD_CODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnCodeOk();
	afx_msg void OnBnClickedCancel();
};
