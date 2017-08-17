// KeyList.cpp : implementation file
//

#include "stdafx.h"
#include "CardView.h"
#include "KeyList.h"
#include "RtfParser/TextListener.h"
#include "Env.h"

// CKeyList dialog

IMPLEMENT_DYNAMIC(CKeyList, CDialog)

CKeyList::CKeyList(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CKeyList::IDD, pParent)
{
	m_db = NULL;
}

CKeyList::~CKeyList()
{
}

void CKeyList::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CKeyList, CResizableDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CKeyList::OnNMDblclkList1)
END_MESSAGE_MAP()


// CKeyList message handlers
void CKeyList::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	CCardView *		wnd = (CCardView *) m_parent;

	if (wnd)
	{
		wnd->m_keyListView = 0;
	}
	WINDOWPLACEMENT	wp;
	wp.length = sizeof wp;
	if ( GetWindowPlacement(&wp) )
	{
		if ( IsIconic() )
		  wp.showCmd = SW_SHOW ;

		if ((wp.flags & WPF_RESTORETOMAXIMIZED) != 0)
		  wp.showCmd = SW_SHOWMAXIMIZED ;

		WriteWindowPlacement("keylist", &wp);
	}
	DestroyWindow();
//	CDialog::OnOK();
}

void CKeyList::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	CCardView *		wnd = (CCardView *) m_parent;

	if (wnd)
	{
		wnd->m_keyListView = 0;
	}
	WINDOWPLACEMENT	wp;
	wp.length = sizeof wp;
	if ( GetWindowPlacement(&wp) )
	{
		if ( IsIconic() )
		  wp.showCmd = SW_SHOW ;

		if ((wp.flags & WPF_RESTORETOMAXIMIZED) != 0)
		  wp.showCmd = SW_SHOWMAXIMIZED ;

		WriteWindowPlacement("keylist", &wp);
	}

	DestroyWindow();
//	CDialog::OnCancel();
}

BOOL CKeyList::OnInitDialog()
{
	CResizableDialog::OnInitDialog();

	if (m_db == NULL)
		return FALSE;

	m_list.SetExtendedStyle
	   (m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT );

	m_list.InsertColumn(0, "#", LVCFMT_CENTER, 30);
	m_list.InsertColumn(1, "Cate", LVCFMT_CENTER, 100);
	m_list.InsertColumn(2, "B", LVCFMT_CENTER, 20);
	m_list.InsertColumn(3, "Tag", LVCFMT_CENTER, 100);
	m_list.InsertColumn(4, "Key", LVCFMT_LEFT, 900);

	FillItem(true);

    AddAnchor(IDC_LIST1, TOP_LEFT, BOTTOM_RIGHT);

	WINDOWPLACEMENT wp;
	if (ReadWindowPlacement("keylist", &wp))
	{
		 SetWindowPlacement(&wp);
	}
    else
    {
	    CRect	r, r2;
        this->ShowWindow(SW_SHOW); 
	    this->GetWindowRect(&r);
	    CCardView *		wnd = (CCardView *) m_parent;
	    wnd->GetWindowRect(&r2);
	    this->SetWindowPos(&wndNoTopMost, r2.left-r.Width() - 10, r2.top + 5, 0, 0, SWP_NOSIZE);
    }

    return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CKeyList::FillItem(bool force)
{
	LVITEM				lv;
	CString				s; 
	StringMapArray		summary;

    if (!force && m_db->isDirty() == FALSE)
        return;

	m_list.DeleteAllItems();

	lv.mask = LVIF_TEXT;
	m_db->GetSummary(summary, TRUE, LINE_FEED, false);

	for(int i = 0; i < (int) summary.size(); i++)
	{
		s.Format("%d", i+1);
		lv.iItem = i;
		lv.iSubItem = 0;
		lv.pszText = (LPSTR)(LPCTSTR) s;
		m_list.InsertItem(&lv);

		lv.iSubItem = 1;
		lv.pszText = (LPSTR)summary[i]["category"].c_str();
		m_list.SetItem(&lv);
		
		// bookmark
		lv.iSubItem = 2;
		lv.pszText = (LPSTR)summary[i]["bookmark"].c_str();
		m_list.SetItem(&lv);

		lv.iSubItem = 3;
		lv.pszText = (LPSTR)summary[i]["tag"].c_str();
		m_list.SetItem(&lv);

		lv.pszText = (LPSTR)summary[i]["key"].c_str();
		lv.iSubItem = 4;
		m_list.SetItem(&lv);

        m_list.SetItemData(i, (DWORD) atoi(summary[i]["id"].c_str()));
	}
    
    int index = m_db->GetItemIndex();
 //   int viewIndex = index + 10;
    int viewIndex = index + m_list.GetCountPerPage() / 2;

    if (viewIndex >= m_db->GetItemCount())
        viewIndex = m_db->GetItemCount() - 1;

    m_list.SetItemState(index, LVIS_SELECTED, LVIS_SELECTED);
    m_list.EnsureVisible(viewIndex, FALSE);
}

void CKeyList::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	CCardView *		wnd = (CCardView *)m_parent;

	NMLISTVIEW *pNMListView = (NMLISTVIEW *)pNMHDR;

	if (pNMListView->iItem < 0)
		return;

	wnd->OnGoto(pNMListView->iItem);
	*pResult = 0;
}

void CKeyList::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	CResizableDialog::PostNcDestroy();
	delete this;
}

BOOL CKeyList::PreTranslateMessage(MSG* pMsg)
{
    if(pMsg->message==WM_KEYDOWN)
    {
		switch(pMsg->wParam)
		{
		case VK_F2:
		case VK_F3:
		case VK_F4:
		case VK_F5:
		case VK_F6:
		case VK_F7:
		case VK_F8:
		case VK_F9:
		case VK_F10:
		case VK_F11:
            FillItem();
			break;
		}	
	}

    return CResizableDialog::PreTranslateMessage(pMsg);
}

void CKeyList::GetIdListofSelection(std::vector<std::pair<int, int>> &list)
{
    POSITION pos = m_list.GetFirstSelectedItemPosition();

    list.clear();

    if (pos == NULL)
        return;
    else
    {
       while (pos)
       {
          int nItem = m_list.GetNextSelectedItem(pos);
          list.push_back(pair<int, int>(nItem, m_list.GetItemData(nItem)));
       }
    }
}


