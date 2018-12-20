#pragma once
#include "afxcmn.h"


// deliver_dlg 对话框

class deliver_dlg : public CDialog
{
	DECLARE_DYNAMIC(deliver_dlg)

public:
	deliver_dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~deliver_dlg();

// 对话框数据
	enum { IDD = IDD_DELIVER };
virtual BOOL OnInitDialog();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_progress;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
//	afx_msg void OnBnClickedButton1();
};
