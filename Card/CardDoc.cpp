// CardDoc.cpp : implementation of the CCardDoc class
//

#include "stdafx.h"
#include "Card.h"

#include "CardDoc.h"
#include "Env.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCardDoc

IMPLEMENT_DYNCREATE(CCardDoc, CDocument)

BEGIN_MESSAGE_MAP(CCardDoc, CDocument)
END_MESSAGE_MAP()


// CCardDoc construction/destruction

CCardDoc::CCardDoc()
{
	// TODO: add one-time construction code here

}

CCardDoc::~CCardDoc()
{
}

BOOL CCardDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}




// CCardDoc serialization

void CCardDoc::Serialize(CArchive& ar)
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


// CCardDoc diagnostics

#ifdef _DEBUG
void CCardDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCardDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCardDoc commands


BOOL CCardDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    if (!CDocument::OnOpenDocument(lpszPathName))
        return FALSE;

    return TRUE;
}
