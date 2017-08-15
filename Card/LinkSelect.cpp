// LinkSelect.cpp : implementation file
//

#include "stdafx.h"
#include "Card.h"
#include "LinkSelect.h"


// CLinkSelect dialog

IMPLEMENT_DYNAMIC(CLinkSelect, CDialog)

CLinkSelect::CLinkSelect(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CLinkSelect::IDD, pParent)
    , m_display(_T(""))
    , m_address(_T(""))
    , m_db(NULL)
    , m_curCategory(-1)
    , m_address_onenote(_T(""))
{

}

CLinkSelect::~CLinkSelect()
{
}

void CLinkSelect::DoDataExchange(CDataExchange* pDX)
{
    CResizableDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, m_display);
    DDX_Text(pDX, IDC_EDIT6, m_address);
    DDX_Control(pDX, IDC_COMBO1, m_dbList);
    DDX_Control(pDX, IDC_LIST1, m_list);
    DDX_Control(pDX, IDC_COMBO2, m_category);
    DDX_Text(pDX, IDC_EDIT7, m_address_onenote);
}


BEGIN_MESSAGE_MAP(CLinkSelect, CResizableDialog)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CLinkSelect::OnLvnItemchangedList1)
    ON_CBN_SELCHANGE(IDC_COMBO1, &CLinkSelect::OnCbnSelchangeCombo1)
    ON_CBN_SELCHANGE(IDC_COMBO2, &CLinkSelect::OnCbnSelchangeCategory)
END_MESSAGE_MAP()


// CLinkSelect message handlers

BOOL CLinkSelect::OnInitDialog()
{
    CResizableDialog::OnInitDialog();

	if (m_db == NULL)
		return FALSE;

    m_display = "";
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
    AddAnchor(IDC_EDIT1, TOP_LEFT, TOP_RIGHT);
    AddAnchor(IDC_EDIT6, TOP_LEFT, TOP_RIGHT);
    AddAnchor(IDC_EDIT7, TOP_LEFT, TOP_RIGHT);
    AddAnchor(IDOK, BOTTOM_RIGHT, BOTTOM_RIGHT);
    AddAnchor(IDCANCEL, BOTTOM_RIGHT, BOTTOM_RIGHT);

    UpdateData(0);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

int CLinkSelect::FindItemData(CComboBox &wnd, int value)
{
	for(int i = 0; i < wnd.GetCount(); i++)
	{
		if (value == wnd.GetItemData(i))
			return i;
	}

	return -1;
}

void CLinkSelect::FillCategory(LPCTSTR dbName)
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

void CLinkSelect::FillItems(LPCTSTR dbName)
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

void CLinkSelect::OnCbnSelchangeCombo1()
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

void CLinkSelect::OnCbnSelchangeCategory()
{
	int i = m_category.GetCurSel();
	
	if (i < 0)
		return;
	
    m_curCategory = m_category.GetItemData(i);
   	FillItems(m_curDb);

}

// card://sec=algorithm&id=55555
void CLinkSelect::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (pNMLV->iItem < 0)
		return;
    
    m_address.Format("card://sec=%s&id=%s", m_curDb, m_summary[pNMLV->iItem]["id"].c_str());
    m_display = m_summary[pNMLV->iItem]["key"].c_str();

    UpdateData(0);
    *pResult = 0;
}
