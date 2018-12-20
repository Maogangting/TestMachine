// MenuTest.h : main header file for the MENUTEST application
//

#if !defined(AFX_MENUTEST_H__9DAA3309_0158_4E59_96A3_66F75735E7C8__INCLUDED_)
#define AFX_MENUTEST_H__9DAA3309_0158_4E59_96A3_66F75735E7C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "CodeDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CMenuTestApp:
// See MenuTest.cpp for the implementation of this class
//

class CMenuTestApp : public CWinApp
{
public:
	CMenuTestApp();
protected:
	BOOL FirstInstance();
public:
CCodeDlg CodeDlg;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenuTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMenuTestApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENUTEST_H__9DAA3309_0158_4E59_96A3_66F75735E7C8__INCLUDED_)
