#pragma once


// CSetCode �Ի���

class CSetCode : public CDialog
{
	DECLARE_DYNAMIC(CSetCode)

public:
	CSetCode(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetCode();

// �Ի�������
	enum { IDD = IDD_SET_CODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnSetCodeOk();
};
