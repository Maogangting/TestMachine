// MenuTestDoc.h : interface of the CMenuTestDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MENUTESTDOC_H__7D9894E2_820D_4212_A51E_7E6B32156CA6__INCLUDED_)
#define AFX_MENUTESTDOC_H__7D9894E2_820D_4212_A51E_7E6B32156CA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMenuTestDoc : public CDocument
{
protected: // create from serialization only
	CMenuTestDoc();
	DECLARE_DYNCREATE(CMenuTestDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenuTestDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMenuTestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMenuTestDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENUTESTDOC_H__7D9894E2_820D_4212_A51E_7E6B32156CA6__INCLUDED_)
