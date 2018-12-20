// MenuTestDoc.cpp : implementation of the CMenuTestDoc class
//

#include "stdafx.h"
#include "MenuTest.h"

#include "MenuTestDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenuTestDoc

IMPLEMENT_DYNCREATE(CMenuTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CMenuTestDoc, CDocument)
	//{{AFX_MSG_MAP(CMenuTestDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuTestDoc construction/destruction

CMenuTestDoc::CMenuTestDoc()
{
	// TODO: add one-time construction code here

}

CMenuTestDoc::~CMenuTestDoc()
{
}

BOOL CMenuTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMenuTestDoc serialization

void CMenuTestDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMenuTestDoc diagnostics

#ifdef _DEBUG
void CMenuTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMenuTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMenuTestDoc commands
