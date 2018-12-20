#include "afxwin.h"
#if !defined(AFX_TAB2_H__99E63FFE_CDFD_42BD_93C7_EB6FC399CE20__INCLUDED_)
#define AFX_TAB2_H__99E63FFE_CDFD_42BD_93C7_EB6FC399CE20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Tab2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTab2 dialog

class CTab2 : public CDialog
{
// Construction
public:
	CTab2(CWnd* pParent = NULL);   // standard constructor
//	BOOL LoadFile(CString CodeFileName, PARA_SETTING *file_setting);
// Dialog Data
	//{{AFX_DATA(CTab2)
	enum { IDD = IDD_DIALOG2 };
	CEdit	m_dzbiancheng;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTab2)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
virtual BOOL OnInitDialog();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTab2)
	afx_msg void OnChangedzBiancheng();
	afx_msg void OnChangedzstart();
	afx_msg void OnChangedzstop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_dzstart;
	CEdit m_dzstop;
	//afx_msg void OnBnFileImport();
	afx_msg void OnBnClickedCheckFlag();
	afx_msg void OnBnClickedCheckCurrent();
	afx_msg void OnBnClickedCheckSpeed();
	afx_msg void OnBnClickedCheckLowVol();
	afx_msg void OnBnClickedCheckLowVolEnd();
	afx_msg void OnBnClickedCheckDir();
	afx_msg void OnBnClickedCheckTime();
	afx_msg void OnBnClickedCheckBrake();
	afx_msg void OnBnClickedCheckNtc1();
	afx_msg void OnBnClickedCheckNtc2();
	afx_msg void OnBnClickedCheckNtc3();
	afx_msg void OnBnClickedCheckNtc4();
	afx_msg void OnBnClickedAllSel();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TAB2_H__99E63FFE_CDFD_42BD_93C7_EB6FC399CE20__INCLUDED_)
