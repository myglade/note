// Goto.cpp : implementation file
//

#include "stdafx.h"
#include "Card.h"
#include "Goto.h"


// CGoto dialog

IMPLEMENT_DYNAMIC(CGoto, CDialog)

CGoto::CGoto(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CGoto::IDD, pParent)
    , m_db(NULL)
    , m_curCategory(-1)
{

}

CGoto::~CGoto()
{
}

void CGoto::DoDataExchange(CDataExchange* pDX)
{
    CResizableDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO1, m_dbList);
    DDX_Control(pDX, IDC_LIST1, m_list);
    DDX_Control(pDX, IDC_COMBO2, m_category);
}


BEGIN_MESSAGE_MAP(CGoto, CResizableDialog)
    ON_CBN_SELCHANGE(IDC_COMBO1, &CGoto::OnCbnSelchangeCombo1)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CGoto::OnLvnItemchangedList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CGoto::OnNMDblclkList1)
    ON_CBN_SELCHANGE(IDC_COMBO2, &CGoto::OnCbnSelchangeCategory)
END_MESSAGE_MAP()


// CGoto message handlers
BOOL CGoto::OnInitDialog()
{
    CResizableDialog::OnInitDialog();

	if (m_db == NULL)
		return FALSE;

	m_list.SetExtendedStyle
	   (m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT );

	m_list.InsertColumn(0, "#", LVCFMT_CENTER, 30);
	m_list.InsertColumn(1, "Cate", LVCFMT_CENTER, 100);
	m_list.InsertColumn(2, "Key", LVCFMT_LEFT, 900);

  	CStringArray	sections;

	m_dbList.ResetContent();
	m_db->GetDbList(sections);
	for(int i = 0; i < (int) sections.GetSize(); i++)
	{
		m_dbList.AddString(sections[i]);
	}

    m_curDb = m_db->GetCurDbName();
	m_dbList.SetCurSel(m_dbList.FindStringExact(0, m_curDb));
    m_curCategory = m_db->GetCurCategory();

    FillCategory(m_curDb);
    FillItems(m_curDb);

    AddAnchor(IDC_LIST1, TOP_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDOK, BOTTOM_RIGHT, BOTTOM_RIGHT);
    AddAnchor(IDCANCEL, BOTTOM_RIGHT, BOTTOM_RIGHT);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

int CGoto::FindItemData(CComboBox &wnd, int value)
{
	for(int i = 0; i < wnd.GetCount(); i++)
	{
		if (value == wnd.GetItemData(i))
			return i;
	}

	return -1;
}

void CGoto::FillCategory(LPCTSTR dbName)
{
	CNames	category;
	int		n;

    m_category.ResetContent();
    m_db->GetCategoryList(dbName, category);
	n = m_category.AddString("ALL");
	m_category.SetItemData(n, -1);

	for(int i = 0; i < (int) category.GetSize(); i++)
	{
		n = m_category.AddString(category[i].name);
		m_category.SetItemData(n, category[i].id);
	}

	if (m_curCategory == -1)
		m_category.SetCurSel(0);
	else
		m_category.SetCurSel(FindItemData(m_category, m_curCategory));

}

void CGoto::FillItems(LPCTSTR dbName)
{
    m_summary.clear();
    m_db->GetSummary(dbName, m_curCategory, m_summary, true, LINE_FEED, false);

	LVITEM				lv;
	CString				s; 

	m_list.DeleteAllItems();

	lv.mask = LVIF_TEXT;
	for(int i = 0; i < (int) m_summary.size(); i++)
	{
		s.Format("%d", i+1);
		lv.iItem = i;
		lv.iSubItem = 0;
		lv.pszText = (LPSTR)(LPCTSTR) s;
		m_list.InsertItem(&lv);

		lv.iSubItem = 1;
		lv.pszText = (LPSTR)m_summary[i]["category"].c_str();
		m_list.SetItem(&lv);
		
		lv.pszText = (LPSTR)m_summary[i]["key"].c_str();
		lv.iSubItem = 2;
		m_list.SetItem(&lv);

	}

}

void CGoto::OnCbnSelchangeCombo1()
{
    int     i = m_dbList.GetCurSel();

    if (i < 0)
        return;

    m_dbList.GetLBText(i, m_curDb);  
    if (m_curDb == m_db->GetCurDbName())
    {
        m_curCategory = m_db->GetCurCategory();
    }
    else
        m_curCategory = -1;
    FillCategory(m_curDb);
    FillItems(m_curDb);
}

void CGoto::OnCbnSelchangeCategory()
{
	int i = m_category.GetCurSel();
	
	if (i < 0)
		return;
	
    m_curCategory = m_category.GetItemData(i);
   	FillItems(m_curDb);
}

void CGoto::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (pNMLV->iItem < 0)
		return;
    
	m_curId = m_summary[pNMLV->iItem]["id"].c_str();

	*pResult = 0;
}

void CGoto::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	if (pNMItemActivate->iItem < 0)
		return;
    
	m_curId = m_summary[pNMItemActivate->iItem]["id"].c_str();

	this->EndDialog(IDOK);
	*pResult = 0;
}
