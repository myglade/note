// Input.cpp : implementation file
//

#include "stdafx.h"
#include "Card.h"
#include "Input.h"


// CInput dialog

IMPLEMENT_DYNAMIC(CInput, CDialog)

CInput::CInput(CWnd* pParent /*=NULL*/)
	: CDialog(CInput::IDD, pParent)
	, m_name(_T("")), m_title(_T("Input"))
{

}

CInput::~CInput()
{
}

void CInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_name);
}


BEGIN_MESSAGE_MAP(CInput, CDialog)
END_MESSAGE_MAP()


// CInput message handlers
BOOL CInput::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(m_title);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CInput::Set(CString title, CString name)
{
	m_title = title;
	m_name = name;
}