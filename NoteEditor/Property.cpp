// Category.cpp : implementation file
//

#include "pch.h"
#include "NoteEditor.h"
#include "afxdialogex.h"
#include "Property.h"
#include "Input.h"

// Category dialog

IMPLEMENT_DYNAMIC(Property, CDialog)

Property::Property(CWnd* pParent /*=nullptr*/)
	: CResizableDialog(IDD_PROPERTY, pParent)
{

}

Property::~Property()
{
}

void Property::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_list);
	DDX_Control(pDX, IDC_PROP_REMOVE, m_remove_bt);
	DDX_Control(pDX, IDC_PROP_RENAME, m_rename_bt);
	DDX_Control(pDX, IDC_PROP_UP, m_move_up_bt);
	DDX_Control(pDX, IDC_PROP_DOWN, m_move_down_bt);
}


BEGIN_MESSAGE_MAP(Property, CResizableDialog)
	ON_BN_CLICKED(IDC_PROP_ADD, &Property::OnBnClickedPropAdd)
	ON_BN_CLICKED(IDC_PROP_REMOVE, &Property::OnBnClickedPropRemove)
	ON_BN_CLICKED(IDC_PROP_RENAME, &Property::OnBnClickedPropRename)
	ON_LBN_DBLCLK(IDC_LIST2, &Property::OnLbnDblclkList2)
	ON_BN_CLICKED(IDC_PROP_UP, &Property::OnBnClickedPropUp)
	ON_BN_CLICKED(IDC_PROP_DOWN, &Property::OnBnClickedPropDown)
	ON_LBN_SELCHANGE(IDC_LIST2, &Property::OnLbnSelchangeList)
END_MESSAGE_MAP()


// Category message handlers


BOOL Property::OnInitDialog()
{
	CResizableDialog::OnInitDialog();

    AddAnchor(IDC_LIST2, TOP_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_PROP_ADD, BOTTOM_LEFT);
    AddAnchor(IDC_PROP_REMOVE, BOTTOM_LEFT);
    AddAnchor(IDC_PROP_RENAME, BOTTOM_LEFT);
    AddAnchor(IDOK, BOTTOM_RIGHT);

	m_rename_bt.EnableWindow(FALSE);
	m_move_up_bt.EnableWindow(FALSE);
	m_move_down_bt.EnableWindow(FALSE);
	m_remove_bt.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void Property::OnBnClickedPropAdd()
{
	Input	input;

	input.Set("New Property Name", "");
	int res = (int)input.DoModal();
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


void Property::OnBnClickedPropRemove()
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


void Property::OnBnClickedPropRename()
{
	Input	input;
	CString	s;
	int		sel = m_list.GetCurSel();

	if (sel < 0)
		return;

	m_list.GetText(sel, s);
	input.Set("Rename Property Name", s);
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


void Property::OnLbnDblclkList2()
{
	OnBnClickedPropRename();
}


void Property::OnBnClickedPropUp()
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


void Property::OnBnClickedPropDown()
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


void Property::OnLbnSelchangeList()
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
