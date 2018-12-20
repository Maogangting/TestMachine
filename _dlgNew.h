#if !defined(AFX__DLGNEW_H__153ADD0A_5A02_4E38_8B26_8C6021B3578E__INCLUDED_)
#define AFX__DLGNEW_H__153ADD0A_5A02_4E38_8B26_8C6021B3578E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// _dlgNew.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// C_dlgNew dialog
#include "Tab1.h"
#include "Tab2.h"
#include "afxwin.h"
class C_dlgNew : public CDialog
{
// Construction
public:
	C_dlgNew(CWnd* pParent = NULL);   // standard constructor
CTab1 m_dlg1;  
CTab2 m_dlg2;  
  
CDialog * m_paDlg[2];  
int m_nCurTab; 
// Dialog Data
	//{{AFX_DATA(C_dlgNew)
	enum { IDD = IDD_DIALOG_NEW };
	CTabCtrl	m_Tab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C_dlgNew)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

virtual BOOL OnInitDialog();
protected:

	// Generated message map functions
	//{{AFX_MSG(C_dlgNew)
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNerProjSettingOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_save_name;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnFileImport();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX__DLGNEW_H__153ADD0A_5A02_4E38_8B26_8C6021B3578E__INCLUDED_)
