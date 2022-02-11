// Tag.cpp : implementation file
//

#include "pch.h"
#include "NoteEditor.h"
#include "afxdialogex.h"
#include "Tag.h"
#include "Input.h"

// Tag dialog

IMPLEMENT_DYNAMIC(Tag, CDialogEx)

Tag::Tag(CWnd* pParent /*=nullptr*/)
	: CResizableDialog(IDD_TAG, pParent)
{

}

Tag::~Tag()
{
}

void Tag::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_list);
	DDX_Control(pDX, IDC_TAG_REMOVE, m_remove_bt);
	DDX_Control(pDX, IDC_TAG_RENAME, m_rename_bt);
	DDX_Control(pDX, IDC_TAG_UP, m_move_up_bt);
	DDX_Control(pDX, IDC_TAG_DOWN, m_move_down_bt);
}


BEGIN_MESSAGE_MAP(Tag, CResizableDialog)
	ON_BN_CLICKED(IDC_TAG_ADD, &Tag::OnBnClickedTagAdd)
	ON_BN_CLICKED(IDC_TAG_REMOVE, &Tag::OnBnClickedTagRemove)
	ON_BN_CLICKED(IDC_TAG_RENAME, &Tag::OnBnClickedTagRename)
	ON_LBN_DBLCLK(IDC_LIST2, &Tag::OnLbnDblclkList2)
	ON_BN_CLICKED(IDC_TAG_UP, &Tag::OnBnClickedTagUp)
	ON_BN_CLICKED(IDC_TAG_DOWN, &Tag::OnBnClickedTagDown)
	ON_LBN_SELCHANGE(IDC_LIST2, &Tag::OnLbnSelchangeList)
END_MESSAGE_MAP()


// Tag message handlers


BOOL Tag::OnInitDialog()
{
	CResizableDialog::OnInitDialog();

    AddAnchor(IDC_LIST2, TOP_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_TAG_ADD, BOTTOM_LEFT);
    AddAnchor(IDC_TAG_REMOVE, BOTTOM_LEFT);
    AddAnchor(IDC_TAG_RENAME, BOTTOM_LEFT);
    AddAnchor(IDOK, BOTTOM_RIGHT);

	m_rename_bt.EnableWindow(FALSE);
	m_move_up_bt.EnableWindow(FALSE);
	m_move_down_bt.EnableWindow(FALSE);
	m_remove_bt.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void Tag::OnBnClickedTagAdd()
{
	Input	input;
	 
	input.Set("New Tag Name", "");
	int res = (int) input.DoModal();
	if (res == IDCANCEL)
		return;

	if (input.m_name == "" || input.m_name[0] == '_' ||
		input.m_name[0] == '-')
	{
		MessageBox("Invalid name. ", "Error");
		return;
	}

	//m_list.AddString(input.m_name);
	m_list.InsertString(-1, input.m_name);
}


void Tag::OnBnClickedTagRemove()
{
	int		sel = m_list.GetCurSel();

	if (sel < 0)
		return;

	auto	res = MessageBox("Do you really want to remove?", "Warning",
		MB_YESNO);
	if (res == IDNO)
		return;

	m_list.DeleteString(sel);
	m_rename_bt.EnableWindow(FALSE);
	m_move_up_bt.EnableWindow(FALSE);
	m_move_down_bt.EnableWindow(FALSE);
	m_remove_bt.EnableWindow(FALSE);
}


void Tag::OnBnClickedTagRename()
{
	Input	input;
	CString	s;
	int		sel = m_list.GetCurSel();

	if (sel < 0)
		return;

	m_list.GetText(sel, s);
	input.Set("Rename Section Name", s);
	auto res = input.DoModal();
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

	auto data = m_list.GetItemData(sel);
	m_list.DeleteString(sel);
	m_list.InsertString(sel, input.m_name);
	m_list.SetItemData(sel, data);
}


void Tag::OnLbnDblclkList2()
{
	OnBnClickedTagRename();
}


void Tag::OnBnClickedTagUp()
{
	int		sel = m_list.GetCurSel();

	if (sel < 0 || sel == 0)
		return;

	auto prev = sel - 1;

	CString s;
	m_list.GetText(prev, s);
	auto data = m_list.GetItemData(prev);

	m_list.DeleteString(prev);
	m_list.InsertString(sel, s);
	m_list.SetItemData(sel, data);
	m_list.SetCurSel(prev);
}


void Tag::OnBnClickedTagDown()
{
	int		sel = m_list.GetCurSel();

	if (sel < 0 || sel == m_list.GetCount() - 1)
		return;

	auto next = sel + 1;

	CString s;
	m_list.GetText(sel, s);
	auto data = m_list.GetItemData(sel);

	m_list.DeleteString(sel);
	m_list.InsertString(next, s);
	m_list.SetItemData(next, data);
	m_list.SetCurSel(next);
}


void Tag::OnLbnSelchangeList()
{
	int		sel = m_list.GetCurSel();

	if (sel < 0) {
		m_rename_bt.EnableWindow(FALSE);
		m_move_up_bt.EnableWindow(FALSE);
		m_move_down_bt.EnableWindow(FALSE);
		return;
	}
	m_rename_bt.EnableWindow(TRUE);
	m_move_up_bt.EnableWindow(TRUE);
	m_move_down_bt.EnableWindow(TRUE);
	m_remove_bt.EnableWindow(TRUE);

	if (sel == 0) {
		m_move_up_bt.EnableWindow(FALSE);
	}

	if (sel == m_list.GetCount() - 1) {
		m_move_down_bt.EnableWindow(FALSE);
	}

}
