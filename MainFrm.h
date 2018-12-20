// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__245C2721_F388_44C9_8864_ADD0BE582A7F__INCLUDED_)
#define AFX_MAINFRM_H__245C2721_F388_44C9_8864_ADD0BE582A7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "_dlgNew.h"
#include "SetCode.h"
#include "IP_PORT.h"
//#include "C_NG.h"
#include "NG_DLG.h"

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
C_dlgNew  new_dlg; 
//CCodeDlg  CodeDlg;
CSetCode  SetCodeDlg;
CIP_PORT  ip_port;
//C_NG		ng_record;
// Operations

CNG_DLG  ng_record;
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
//public:
//	void SetStateBarServerState(CString state);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
public:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNetSetting();
	afx_msg void Ontest1();
	afx_msg void OnFileOpen();
	afx_msg void OnNewProj();
	afx_msg void OnSetCode();
	afx_msg void ng_disp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnIpPortSetting();
//	afx_msg void OnTestResult();
	afx_msg void On32785();
	afx_msg void On32786();
	afx_msg void On32787();
	afx_msg void On32788();
	afx_msg void On32789();
	afx_msg void On32790();
	afx_msg void On32791();
	afx_msg void On32792();
	afx_msg void On32793();
	afx_msg void On32794();
	afx_msg void OnUpdate32785(CCmdUI *pCmdUI);
	afx_msg void OnUpdate32786(CCmdUI *pCmdUI);
	afx_msg void OnUpdate32787(CCmdUI *pCmdUI);
	afx_msg void OnUpdate32788(CCmdUI *pCmdUI);
	afx_msg void OnUpdate32789(CCmdUI *pCmdUI);
	afx_msg void OnUpdate32790(CCmdUI *pCmdUI);
	afx_msg void OnUpdate32791(CCmdUI *pCmdUI);
	afx_msg void OnUpdate32792(CCmdUI *pCmdUI);
	afx_msg void OnUpdate32793(CCmdUI *pCmdUI);
	afx_msg void OnUpdate32794(CCmdUI *pCmdUI);
	afx_msg void On32797();
	afx_msg void On32798();
	afx_msg void OnUpdate32798(CCmdUI *pCmdUI);
	afx_msg void OnUpdate1(CCmdUI *pCmdUI);
	afx_msg void OnLogOut();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__245C2721_F388_44C9_8864_ADD0BE582A7F__INCLUDED_)
