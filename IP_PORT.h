#pragma once
#include "afxcmn.h"


// CIP_PORT �Ի���

class CIP_PORT : public CDialog
{
	DECLARE_DYNAMIC(CIP_PORT)

public:
	CIP_PORT(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CIP_PORT();

// �Ի�������
	enum { IDD = IDD_IP_PORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnIpPortSetting();
	afx_msg void OnBnIpPortOk();
	CIPAddressCtrl m_IP;
};
