// Category.cpp : implementation file
//

#include "stdafx.h"
#include "Card.h"
#include "Category.h"
#include "Input.h"

// CCategory dialog

IMPLEMENT_DYNAMIC(CCategory, CDialog)

CCategory::CCategory(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CCategory::IDD, pParent)
{
	m_db = NULL;
}

CCategory::~CCategory()
{
}

void CCategory::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_list);
}


BEGIN_MESSAGE_MAP(CCategory, CResizableDialog)
	ON_BN_CLICKED(IDC_CATE_ADD, &CCategory::OnBnClickedCateAdd)
	ON_BN_CLICKED(IDC_CATE_REMOVE, &CCategory::OnBnClickedCateRemove)
	ON_BN_CLICKED(IDC_CATE_RENAME, &CCategory::OnBnClickedCateRename)
	ON_LBN_DBLCLK(IDC_LIST2, &CCategory::OnLbnDblclkList2)
END_MESSAGE_MAP()

BOOL CCategory::OnInitDialog()
{
	CResizableDialog::OnInitDialog();

	if (m_db == NULL)
		return FALSE;
	Load();
    AddAnchor(IDC_LIST2, TOP_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_CATE_ADD, BOTTOM_LEFT);
    AddAnchor(IDC_CATE_REMOVE, BOTTOM_LEFT);
    AddAnchor(IDC_CATE_RENAME, BOTTOM_LEFT);
    AddAnchor(IDOK, BOTTOM_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// CCategory message handlers

void CCategory::OnBnClickedCateAdd()
{
	CInput	input;

	input.Set("New Category Name", "");
	int res = input.DoModal();
	if (res == IDCANCEL)
		return;

	if (input.m_name == "" || input.m_name[0] == '_' ||
		input.m_name[0] == '-' || input.m_name[0] == ' ')
	{
		MessageBox("Invalid name. ", "Error");
		return;
	}

	m_db->CreateCategory(input.m_name);
	Load();
	m_list.SetCurSel(m_list.FindStringExact(0, input.m_name));
}

void CCategory::OnBnClickedCateRemove()
{
	int		sel = m_list.GetCurSel();

	if (sel <= 0)
		return;

	int		res = MessageBox("Do you really want to remove?", "Warning",
				MB_YESNO);

	if (res == IDNO)
		return;

	CString s;

	m_list.GetText(sel, s);
	m_db->DeleteCategory(s);
	Load();
}

void CCategory::OnBnClickedCateRename()
{
	CInput	input;
	CString	s;

	int		sel = m_list.GetCurSel();

	if (sel <= 0)
		return;

	m_list.GetText(sel, s);
	input.Set("Rename Category Name", s);
	int res = input.DoModal();
	if (res == IDCANCEL)
		return;

	if (input.m_name == "" || input.m_name[0] == '_' ||
		input.m_name[0] == '-' || input.m_name[0] == ' ')
	{
		MessageBox("Invalid name. ", "Error");
		return;
	}
	if (s == input.m_name)
		return;

	m_db->RenameCategory(s, input.m_name);
	Load();
	m_list.SetCurSel(m_list.FindStringExact(0, input.m_name));
}

void CCategory::Load()
{
	m_list.ResetContent();
	CNames	categories;
	m_db->GetCategoryList(categories);
	for(int i = 0; i < (int) categories.GetSize(); i++)
	{
		int n = m_list.AddString(categories[i].name);
		m_list.SetItemData(n, categories[i].id);
	}
}
void CCategory::OnLbnDblclkList2()
{
	OnBnClickedCateRename();
}
