#pragma once
#include "afxcmn.h"


// deliver_dlg �Ի���

class deliver_dlg : public CDialog
{
	DECLARE_DYNAMIC(deliver_dlg)

public:
	deliver_dlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~deliver_dlg();

// �Ի�������
	enum { IDD = IDD_DELIVER };
virtual BOOL OnInitDialog();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_progress;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
//	afx_msg void OnBnClickedButton1();
};
