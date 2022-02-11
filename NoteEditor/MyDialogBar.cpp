// MyDialogBar.cpp : implementation file
//

#include "pch.h"
#include "NoteEditor.h"
#include "afxdialogex.h"
#include "MyDialogBar.h"


// MyDialogBar dialog

IMPLEMENT_DYNAMIC(MyDialogBar, CDialogBar)

MyDialogBar::MyDialogBar()
    : CDialogBar()
{

}

MyDialogBar::~MyDialogBar()
{
}

void MyDialogBar::DoDataExchange(CDataExchange* pDX)
{
    CDialogBar::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO1, m_prop);
    DDX_Control(pDX, IDC_COMBO2, m_tag);
    DDX_Control(pDX, IDC_STATIC1, m_keys);
    // DDX_Control(pDX, IDC_BUTTON1, m_cur_status);
    DDX_Control(pDX, IDC_EDIT1, m_keys_editor);
}


void  MyDialogBar::Init()
{
	UpdateData(FALSE);

    m_keys.SetWindowText("AAAAAAA");
}

BEGIN_MESSAGE_MAP(MyDialogBar, CDialogBar)
//	ON_WM_LBUTTONDBLCLK()
ON_STN_DBLCLK(IDC_STATIC1, &MyDialogBar::OnStnDblclickStatic1)
ON_EN_KILLFOCUS(IDC_EDIT1, &MyDialogBar::OnEnKillfocusEdit1)
END_MESSAGE_MAP()


// MyDialogBar message handlers



void MyDialogBar::OnStnDblclickStatic1()
{
    // TODO: Add your control notification handler code here
    CString s;
    
    m_keys.GetWindowText(s);
    m_keys.ShowWindow(SW_HIDE);
    m_keys_editor.SetWindowText(s);
    m_keys_editor.ShowWindow(SW_SHOW);
    m_keys_editor.SetFocus();
}


void MyDialogBar::OnEnKillfocusEdit1()
{
    // TODO: Add your control notification handler code here
    CString s;

    m_keys_editor.GetWindowText(s);
    m_keys.SetWindowText(s);
    m_keys_editor.ShowWindow(SW_HIDE);
    m_keys.ShowWindow(SW_SHOW);
}
