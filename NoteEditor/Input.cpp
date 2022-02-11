// Input.cpp : implementation file
//

#include "pch.h"
#include "NoteEditor.h"
#include "afxdialogex.h"
#include "Input.h"


// Input dialog

IMPLEMENT_DYNAMIC(Input, CDialogEx)

Input::Input(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INPUT, pParent)
	, m_name(_T(""))
{
	m_title = _T("Input");
}

Input::~Input()
{
}

void Input::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_name);
}


BEGIN_MESSAGE_MAP(Input, CDialogEx)
END_MESSAGE_MAP()


// Input message handlers


BOOL Input::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowText(m_title);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void Input::Set(const CString& title, const CString& name)
{
	m_title = title;
	m_name = name;
}
