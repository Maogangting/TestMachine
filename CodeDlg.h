#pragma once


// CCodeDlg 对话框

class CCodeDlg : public CDialog
{
	DECLARE_DYNAMIC(CCodeDlg)

public:
	CCodeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCodeDlg();

// 对话框数据
	enum { IDD = IDD_CODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnCodeOk();
	afx_msg void OnBnClickedCancel();
};
