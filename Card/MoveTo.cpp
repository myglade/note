// MoveTo.cpp : implementation file
//

#include "stdafx.h"
#include "Card.h"
#include "MoveTo.h"
#include "afxdialogex.h"


// CMoveTo dialog

IMPLEMENT_DYNAMIC(CMoveTo, CDialog)

CMoveTo::CMoveTo(CWnd* pParent /*=NULL*/)
	: CDialog(CMoveTo::IDD, pParent)
{

    m_number = 1;
}

CMoveTo::~CMoveTo()
{
}

void CMoveTo::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_STRING, m_number);
    DDX_Control(pDX, IDC_STRING, m_numberWnd);
}


BEGIN_MESSAGE_MAP(CMoveTo, CDialog)
END_MESSAGE_MAP()


BOOL CMoveTo::OnInitDialog()
{
    CDialog::OnInitDialog();

    CString s;

    s.Format("Move To (1 - %d)", m_max);
    this->SetWindowText(s);
    m_numberWnd.SetFocus();

    return true;
}
