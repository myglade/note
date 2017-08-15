// SearchResult.cpp : implementation file
//

#include "stdafx.h"
#include "SearchResult.h"
#include "CardView.h"
#include "RtfParser/TextListener.h"


// CSearchResult dialog

IMPLEMENT_DYNAMIC(CSearchResult, CDialog)

CSearchResult::CSearchResult(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CSearchResult::IDD, pParent)
{
	m_db = NULL;
}

CSearchResult::~CSearchResult()
{
}

void CSearchResult::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CSearchResult, CResizableDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CSearchResult::OnNMDblclkList1)
END_MESSAGE_MAP()


// CSearchResult message handlers

// CKeyList message handlers
void CSearchResult::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	CCardView *		wnd = (CCardView *) m_parent;

	if (wnd)
	{
		wnd->m_searchView = 0;
	}

	WINDOWPLACEMENT	wp;
	// before it is destroyed, save the position of the window
	wp.length = sizeof wp;

	if (GetWindowPlacement(&wp))
	{
		WriteWindowPlacement("searchresult", &wp);
	}

	DestroyWindow();
//	CDialog::OnOK();
}

void CSearchResult::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	CCardView *		wnd = (CCardView *) m_parent;

	if (wnd)
	{
		wnd->m_searchView = 0;
	}

	WINDOWPLACEMENT	wp;
	// before it is destroyed, save the position of the window
	wp.length = sizeof wp;

	if (GetWindowPlacement(&wp))
	{
		WriteWindowPlacement("searchresult", &wp);
	}

	DestroyWindow();
//	CDialog::OnCancel();
}

BOOL CSearchResult::OnInitDialog()
{
	CResizableDialog::OnInitDialog();

	if (m_db == NULL)
		return FALSE;

	m_list.SetExtendedStyle
	   (m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT );

	m_list.InsertColumn(0, " ", LVCFMT_CENTER, 20);
	m_list.InsertColumn(1, " ", LVCFMT_CENTER, 110);
	m_list.InsertColumn(2, "Content", LVCFMT_LEFT, 1000);

    AddAnchor(IDC_LIST1, TOP_LEFT, BOTTOM_RIGHT);

	WINDOWPLACEMENT wp;
	if (ReadWindowPlacement("searchresult", &wp))
	{
		 SetWindowPlacement(&wp);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSearchResult::FillItem(CString & key, StringMapArray	&result)
{
	LVITEM				lv;
	CString				s; 

	m_list.DeleteAllItems();

	lv.mask = LVIF_TEXT;

	for(int i = 0; i < (int) result.size(); i++)
	{
		s.Format("%d", i);
		lv.iItem = i;
		lv.iSubItem = 0;
		lv.pszText = (LPSTR)(LPCTSTR) s;
		m_list.InsertItem(&lv);

		lv.pszText = (LPSTR)result[i]["key"].c_str();
		lv.iSubItem = 1;
		m_list.SetItem(&lv);

		lv.pszText = (LPSTR)result[i]["content"].c_str();
		lv.iSubItem = 2;
		m_list.SetItem(&lv);

		m_list.SetItemData(i, (DWORD) atoi(result[i]["id"].c_str()));
	}

	SetWindowText("Search result : " + key);
    m_result = result;
	m_key = key;
}

void CSearchResult::FillItem()
{
	LVITEM				lv;
	CString				s; 
    
    m_result.clear();
	m_list.DeleteAllItems();

	if (m_key == "")
		return;

	if (m_db->SearchKey(m_result, m_key, LINE_FEED, false) == 0)
	{
		return;
	}

	lv.mask = LVIF_TEXT;

	for(int i = 0; i < (int) m_result.size(); i++)
	{
		s.Format("%d", i);
		lv.iItem = i;
		lv.iSubItem = 0;
		lv.pszText = (LPSTR)(LPCTSTR) s;
		m_list.InsertItem(&lv);

		lv.pszText = (LPSTR)m_result[i]["key"].c_str();
		lv.iSubItem = 1;
		m_list.SetItem(&lv);

		lv.pszText = (LPSTR)m_result[i]["content"].c_str();
		lv.iSubItem = 2;
		m_list.SetItem(&lv);

//        CString a = result[i]["id"].c_str();
//		m_list.SetItemData(i, (DWORD) atoi(result[i]["id"].c_str()));
	}

	SetWindowText("Search result : " + m_key);
}

void CSearchResult::Clear()
{
	m_list.DeleteAllItems();
	SetWindowText("Search result ");
}

void CSearchResult::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	CCardView *		wnd = (CCardView *)m_parent;

	NMLISTVIEW *pNMListView = (NMLISTVIEW *)pNMHDR;

	if (pNMListView->iItem < 0)
		return;

    int i = pNMListView->iItem;
    wnd->GotoById(m_result[i]["db"].c_str(), m_result[i]["id"].c_str(), 
        true, true);
//	int	itime = m_list.GetItemData(pNMListView->iItem);
//	wnd->GotoByitime(itime);
	*pResult = 0;
}

void CSearchResult::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	CResizableDialog::PostNcDestroy();
	delete this;
}
