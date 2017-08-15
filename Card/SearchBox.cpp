// SearchBox.cpp : implementation file
//

#include "stdafx.h"
#include "Card.h"
#include "SearchBox.h"
#include "afxdialogex.h"


// CSearchBox dialog

IMPLEMENT_DYNAMIC(CSearchBox, CDialog)

CSearchBox::CSearchBox(CWnd* pParent /*=NULL*/)
	: CDialog(CSearchBox::IDD, pParent)
    , m_string(_T(""))
{

}

CSearchBox::~CSearchBox()
{
}

void CSearchBox::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_STRING, m_string);
}


BEGIN_MESSAGE_MAP(CSearchBox, CDialog)
END_MESSAGE_MAP()


// CSearchBox message handlers
