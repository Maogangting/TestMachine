// MenuTestView.h : interface of the CMenuTestView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MENUTESTVIEW_H__73269A3C_3043_43F2_B601_CC87DFD7F8F0__INCLUDED_)
#define AFX_MENUTESTVIEW_H__73269A3C_3043_43F2_B601_CC87DFD7F8F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "_dlgNew.h"
#include "afxwin.h"
#include "deliver_dlg.h"
#include "MenuTestDoc.h"
#include "afxcmn.h"
#include "ListCtrl_Sortable.h"


class CMenuTestView : public CFormView
{
protected: // create from serialization only
	CMenuTestView();
	DECLARE_DYNCREATE(CMenuTestView)

public:
	//{{AFX_DATA(CMenuTestView)
	enum { IDD = IDD_MENUTEST_FORM };
	CEdit	m_CurrProj;
	CComboBox	m_TestType;
	//}}AFX_DATA

	deliver_dlg Deliver_Dlg;
	
// Attributes
public:
	CMenuTestDoc* GetDocument();
#ifdef MGT_SOCKET
	int InitServerSocket();
	int socket_bind_listen();
	//void OnBnqqSend();
#endif
public:
	void re_build_main_list(int total_item_num, int list_control_type);
	void disp_current_test_info_line(CString sql_string);
// Operations
public:
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenuTestView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMenuTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
// ²Ù×÷
public:
#ifdef MGT_SOCKET_CLIENT
	BOOL qq_client_init();
	BOOL socket_client_connect();
	BOOL client_send(CString buf);
#endif
protected:
	CFont m_font;	// ×ÖÌå
	CFont m_font2;	// ×ÖÌå
	CFont m_font3;
// Generated message map functions
protected:
	//{{AFX_MSG(CMenuTestView)
	afx_msg void OnButtonToTestBoard();
	afx_msg void OnSelchangeType();
	afx_msg void OnReSetting();
	afx_msg void OnBnqqSend();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_Sever_Send;
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CEdit m_topc_disp;
	CListCtrl_Sortable m_main_list;
	afx_msg void OnPaint();
//	afx_msg void OnHdnItemclickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadioChengce();
};

#ifndef _DEBUG  // debug version in MenuTestView.cpp
inline CMenuTestDoc* CMenuTestView::GetDocument()
   { return (CMenuTestDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENUTESTVIEW_H__73269A3C_3043_43F2_B601_CC87DFD7F8F0__INCLUDED_)
