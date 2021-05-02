// Paraformat.cpp : implementation file
//

#include "stdafx.h"
#include "LineSpacing.h"


// CLineSpacing dialog

IMPLEMENT_DYNAMIC(CLineSpacing, CDialog)

CLineSpacing::CLineSpacing(CWnd* pParent /*=NULL*/)
	: CDialog(CLineSpacing::IDD, pParent)
{

}

CLineSpacing::~CLineSpacing()
{
}

void CLineSpacing::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO1, m_space);
}


BEGIN_MESSAGE_MAP(CLineSpacing, CDialog)
    ON_CBN_SELCHANGE(IDC_COMBO1, &CLineSpacing::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CLineSpacing message handlers

BOOL CLineSpacing::OnInitDialog()
{
    CDialog::OnInitDialog();

	m_space.ResetContent();
	m_space.AddString("1");
	m_space.AddString("1.25");
	m_space.AddString("1.5");
	m_space.AddString("1.75");
	m_space.AddString("2");
    m_value = 360;
	m_space.SetCurSel(2);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CLineSpacing::OnCbnSelchangeCombo1()
{
    int     i = m_space.GetCurSel();
    CString s;

    if (i < 0)
        return;

    m_space.GetLBText(i, s);  
    double k = atof(s);
    m_value =(int)( k * 240.0); 
}
