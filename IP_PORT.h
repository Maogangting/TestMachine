#pragma once
#include "afxcmn.h"


// CIP_PORT 对话框

class CIP_PORT : public CDialog
{
	DECLARE_DYNAMIC(CIP_PORT)

public:
	CIP_PORT(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CIP_PORT();

// 对话框数据
	enum { IDD = IDD_IP_PORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnIpPortSetting();
	afx_msg void OnBnIpPortOk();
	CIPAddressCtrl m_IP;
};
