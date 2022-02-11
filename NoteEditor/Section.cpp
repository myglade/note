// Section.cpp : implementation file
//

#include "pch.h"
#include "NoteEditor.h"
#include "afxdialogex.h"
#include "Section.h"
#include "Input.h"

// Section dialog

IMPLEMENT_DYNAMIC(Section, CDialogEx)

Section::Section(CWnd* pParent /*=nullptr*/)
	: CResizableDialog(IDD_SECTION, pParent)
{

}

Section::~Section()
{
}

void Section::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_list);
	DDX_Control(pDX, IDC_SECTION_UP, m_move_up_bt);
	DDX_Control(pDX, IDC_SECTION_DOWN, m_move_down_bt);
	DDX_Control(pDX, IDC_SECTION_RENAME, m_rename_bt);
	DDX_Control(pDX, IDC_SECTION_REMOVE, m_remove_bt);
}


BEGIN_MESSAGE_MAP(Section, CResizableDialog)
	ON_BN_CLICKED(IDC_SECTION_ADD, &Section::OnBnClickedSectionAdd)
	ON_BN_CLICKED(IDC_SECTION_REMOVE, &Section::OnBnClickedSectionRemove)
	ON_BN_CLICKED(IDC_SECTION_RENAME, &Section::OnBnClickedSectionRename)
	ON_LBN_DBLCLK(IDC_LIST2, &Section::OnLbnDblclkList2)
	ON_BN_CLICKED(IDC_SECTION_UP, &Section::OnBnClickedSectionUp)
	ON_BN_CLICKED(IDC_SECTION_DOWN, &Section::OnBnClickedSectionDown)
	ON_LBN_SELCHANGE(IDC_LIST2, &Section::OnLbnSelchangeList)
END_MESSAGE_MAP()


// Section message handlers


BOOL Section::OnInitDialog()
{
	CResizableDialog::OnInitDialog();

    AddAnchor(IDC_LIST2, TOP_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_SECTION_ADD, BOTTOM_LEFT);
    AddAnchor(IDC_SECTION_REMOVE, BOTTOM_LEFT);
    AddAnchor(IDC_SECTION_RENAME, BOTTOM_LEFT);
    AddAnchor(IDOK, BOTTOM_RIGHT);

	m_rename_bt.EnableWindow(FALSE);
	m_move_up_bt.EnableWindow(FALSE);
	m_move_down_bt.EnableWindow(FALSE);
	m_remove_bt.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void Section::OnBnClickedSectionAdd()
{
	Input	input;

	input.Set("New Section Name", "");
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


void Section::OnBnClickedSectionRemove()
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


void Section::OnBnClickedSectionRename()
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


void Section::OnLbnDblclkList2()
{
	OnBnClickedSectionRename();
}


void Section::OnBnClickedSectionUp()
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


void Section::OnBnClickedSectionDown()
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


void Section::OnLbnSelchangeList()
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
