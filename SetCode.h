#pragma once


// CSetCode 对话框

class CSetCode : public CDialog
{
	DECLARE_DYNAMIC(CSetCode)

public:
	CSetCode(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetCode();

// 对话框数据
	enum { IDD = IDD_SET_CODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnSetCodeOk();
};
