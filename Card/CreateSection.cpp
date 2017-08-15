// CreateSection.cpp : implementation file
//

#include "stdafx.h"
#include "Card.h"
#include "CreateSection.h"


// CCreateSection dialog

IMPLEMENT_DYNAMIC(CCreateSection, CDialog)

CCreateSection::CCreateSection(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateSection::IDD, pParent)
	, m_dictMode(FALSE)
	, m_name(_T(""))
{

}

CCreateSection::~CCreateSection()
{
}

void CCreateSection::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_dictMode);
	DDX_Text(pDX, IDC_EDIT1, m_name);
}


BEGIN_MESSAGE_MAP(CCreateSection, CDialog)
END_MESSAGE_MAP()


// CCreateSection message handlers

BOOL CCreateSection::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
