// Tag.cpp : implementation file
//

#include "stdafx.h"
#include "Card.h"
#include "Tag.h"
#include "Input.h"
#include "CardView.h"

// CTag dialog

IMPLEMENT_DYNAMIC(CTag, CDialog)

CTag::CTag(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CTag::IDD, pParent)
{
	m_db = NULL;
    m_cardView = NULL;
}

CTag::~CTag()
{
}

void CTag::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_list);
}
 

BEGIN_MESSAGE_MAP(CTag, CResizableDialog)
	ON_BN_CLICKED(IDC_TAG_ADD, &CTag::OnBnClickedTagAdd)
	ON_BN_CLICKED(IDC_TAG_REMOVE, &CTag::OnBnClickedTagRemove)
	ON_BN_CLICKED(IDC_TAG_RENAME, &CTag::OnBnClickedTagRename)
	ON_LBN_DBLCLK(IDC_LIST2, &CTag::OnLbnDblclkList2)
END_MESSAGE_MAP()


// CTag message handlers

BOOL CTag::OnInitDialog()
{
	CResizableDialog::OnInitDialog();

	if (m_db == NULL)
		return FALSE;
	Load();
    AddAnchor(IDC_LIST2, TOP_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_TAG_ADD, BOTTOM_LEFT);
    AddAnchor(IDC_TAG_REMOVE, BOTTOM_LEFT);
    AddAnchor(IDC_TAG_RENAME, BOTTOM_LEFT);
    AddAnchor(IDOK, BOTTOM_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// CTag message handlers

void CTag::OnBnClickedTagAdd()
{
	CInput	input;

	input.Set("New Tag Name", "");
	int res = input.DoModal();
	if (res == IDCANCEL)
		return;

	if (input.m_name == "" || input.m_name[0] == '_' ||
		input.m_name[0] == '-')
	{
		MessageBox("Invalid name. ", "Error");
		return;
	}

	m_db->CreateTag(input.m_name, input.m_preset);
	Load();
	m_list.SetCurSel(m_list.FindStringExact(0, input.m_name));
}

void CTag::OnBnClickedTagRemove()
{
	int		sel = m_list.GetCurSel();

	if (sel < 0)
		return;

	int		res = MessageBox("Do you really want to remove?", "Warning",
				MB_YESNO);

	if (res == IDNO)
		return;

	CString s;

	m_list.GetText(sel, s);
	m_db->DeleteTag(s);
	Load();
    UpdateParentWindow();
}

void CTag::OnBnClickedTagRename()
{
	CInput	input;
	CString	s;

	int		sel = m_list.GetCurSel();

	if (sel < 0)
		return;

	m_list.GetText(sel, s);
	input.Set("Rename Tag Name", s);
	int res = input.DoModal();
	if (res == IDCANCEL)
		return;

	if (input.m_name == "" || input.m_name[0] == '_' ||
		input.m_name[0] == '-')
	{
		MessageBox("Invalid name. ", "Error");
		return;
	}
	if (s == input.m_name)
		return;

	m_db->RenameTag(s, input.m_name);
	Load();
	m_list.SetCurSel(m_list.FindStringExact(0, input.m_name));
    UpdateParentWindow();
}

void CTag::Load()
{
	m_list.ResetContent();
	CNames	names;
	m_db->GetTagList(names);
	for(int i = 0; i < (int) names.GetSize(); i++)
	{
		int n = m_list.AddString(names[i].name);
		m_list.SetItemData(n, names[i].id);
	}
}

void CTag::UpdateParentWindow()
{
    if (!m_cardView)
        return;

    ((CCardView *)m_cardView)->UpdateKeyListView();
}

void CTag::OnLbnDblclkList2()
{
	OnBnClickedTagRename();
}
