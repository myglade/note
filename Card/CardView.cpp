// CardView.cpp : implementation of the CCardView class
//

#include "stdafx.h"
#include "Card.h"

#include "CardDoc.h"
#include "CardView.h"
#include "MainFrm.h"
#include "Setting.h"
#include "Tag.h"
#include "Env.h"
#include "Category.h"
#include "json/reader.h"
#include "json/writer.h"
#include "json/elements.h"
#include "RtfParser/RtfImage.h"
#include "LinkSelect.h"
#include "Goto.h"
#include "SearchResult.h"
#include "SearchBox.h"
#include "MoveTo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static void Save(CString name, CString &s)
{
    CFile       file;

    return;

    file.Open(name, CFile::modeCreate | CFile::modeReadWrite);
    file.Write(s, s.GetLength() + 1);
    file.Close();
}

std::string charToHex(unsigned char c) {
    short i = c;
         
    std::stringstream s;
         
    s << "%" << std::setw(2) << std::setfill('0') << std::hex << i;
         
    return s.str();
}
 
unsigned char hexToChar(const std::string &str) {
    short c = 0;
         
    if(!str.empty()) {
        std::istringstream in(str);
             
        in >> std::hex >> c;
             
        if(in.fail()) {
            throw std::runtime_error("stream decode failure");
        }
    }
         
    return static_cast<unsigned char>(c);
}

std::string urlEncode(const std::string toEncode) {
    std::ostringstream out;
         
    for(std::string::size_type i=0; i < toEncode.length(); ++i) {
        short t = toEncode.at(i);
             
        if(
            t == 45 ||          // hyphen
            (t >= 48 && t <= 57) ||       // 0-9
            (t >= 65 && t <= 90) ||       // A-Z
            t == 95 ||          // underscore
            (t >= 97 && t <= 122) ||  // a-z
            t == 126            // tilde
        ) {
            out << toEncode.at(i);
        } else {
            out << charToHex(toEncode.at(i));
        }
    }
         
    return out.str();
}
 
std::string urlDecode(const std::string toDecode) {
    std::ostringstream out;
         
    for(std::string::size_type i=0; i < toDecode.length(); ++i) {
        if(toDecode.at(i) == '%') {
            std::string str(toDecode.substr(i+1, 2));
            out << hexToChar(str);
            i += 2;
        } else {
            out << toDecode.at(i);
        }
    }
         
    return out.str();
}

// CCardView

IMPLEMENT_DYNCREATE(CCardView, CFormView)

BEGIN_MESSAGE_MAP(CCardView, CFormView)
    ON_MESSAGE(WM_DB_CHANGE, OnDbStatusChange)
    ON_MESSAGE(WM_FILE_NOTIFY, OnFileNotification)
	ON_MESSAGE(WM_RESIZER_UNHOOK, OnResizerUnhook)
	ON_MESSAGE(WM_RESIZER_SIZE, OnResizerSize)
	ON_MESSAGE(WM_RESIZER_PORTION_CHANGE, OnResizerPortionChange)
	ON_MESSAGE(WM_LINK_CLICK, OnLinkClick)
	ON_MESSAGE(WM_LINK_SET, OnLinkSet)
	ON_COMMAND(ID_REQUEST, &CCardView::OnOpenReq)
	ON_COMMAND(ID_FILE_SAVE_IOS, &CCardView::OnFileSaveIos)
	ON_COMMAND(ID_EDIT_ADDITEM, &CCardView::OnEditAdditem)
	ON_COMMAND(ID_EDIT_REMOVEITEM, &CCardView::OnEditRemoveitem)
	ON_COMMAND(ID_EDIT_MOVETOFIRST, &CCardView::OnEditMovetofirst)
	ON_COMMAND(ID_EDIT_MOVETOPREV, &CCardView::OnEditMovetoprev)
	ON_COMMAND(ID_EDIT_MOVETONEXT, &CCardView::OnEditMovetonext)
	ON_COMMAND(ID_VIEW_FIRST, &CCardView::OnViewFirst)
	ON_COMMAND(ID_VIEW_PREV, &CCardView::OnViewPrev)
	ON_COMMAND(ID_VIEW_NEXT, &CCardView::OnViewNext)
	ON_COMMAND(ID_VIEW_LAST, &CCardView::OnViewLast)
	ON_COMMAND(ID_WINDOW_KEYLIST, &CCardView::OnWindowKeyList)
	ON_COMMAND(ID_WINDOW_SEARCH, &CCardView::OnWindowSearch)
	ON_CBN_SELCHANGE(ID_SECTION_HOLDER, &CCardView::OnCbnSelchangeSection)
	ON_CBN_SELCHANGE(ID_CATE_HOLDER, &CCardView::OnCbnSelchangeCategory)
	ON_CBN_CLOSEUP(ID_TAG_HOLDER, &CCardView::OnCbnSelchangeTagCombo)
	ON_COMMAND(ID_OR_TAG_HOLDER, &CCardView::OnBnClickedSearchmode)
//	ON_BN_CLICKED(ID_BOOKMARK_HOLDER, &CCardView::OnBnClickedBookmarkmode)
//	ON_BN_CLICKED(ID_VIEW_READONLYMODE, &CCardView::OnBnClickedReadonlymode)
//	ON_BN_CLICKED(ID_VIEW_TESTMODE, &CCardView::OnBnClickedTestmode)
	ON_COMMAND(ID_EDIT_SETTING, &CCardView::OnEditSetting)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FIRST, &CCardView::OnUpdateViewPrev)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PREV, &CCardView::OnUpdateViewPrev)
	ON_UPDATE_COMMAND_UI(ID_VIEW_NEXT, &CCardView::OnUpdateViewNext)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LAST, &CCardView::OnUpdateViewNext)
	ON_CBN_SELCHANGE(IDC_CATEGORY, &CCardView::OnCbnSelchangeItemCategory)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ADDITEM, &CCardView::OnUpdateEditAdditem)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REMOVEITEM, &CCardView::OnUpdateEditCheckitem)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MOVETOFIRST, &CCardView::OnUpdateEditMovetofirst)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MOVETOPREV, &CCardView::OnUpdateEditMovetoprev)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MOVETONEXT, &CCardView::OnUpdateEditMovetonext)
//	ON_UPDATE_COMMAND_UI(ID_EDIT_FILTER, &CCardView::OnUpdateEditCheckitem)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ADDBOOKMARK, &CCardView::OnUpdateEditCheckitem)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CATEGORY, &CCardView::OnUpdateEditAdditem)
	ON_UPDATE_COMMAND_UI(ID_EDIT_TAG, &CCardView::OnUpdateEditAdditem)
	ON_COMMAND(ID_EDIT_COPY, &CCardView::OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CCardView::OnEditPaste)
	ON_COMMAND(ID_EDIT_CUT, &CCardView::OnEditCut)
	ON_COMMAND(ID_EDIT_UNDO, &CCardView::OnEditUndo)
	ON_COMMAND(ID_EDIT_SELECT_ALL, &CCardView::OnEditSelectAll)
	ON_COMMAND(ID_EDIT_SAVE, &CCardView::OnEditSave)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SAVE, &CCardView::OnUpdateEditSave)
	ON_COMMAND(ID_VIEW_GOTO, &CCardView::OnViewGoto)
	ON_COMMAND(ID_VIEW_SEARCHINKEY, &CCardView::OnViewSearchinkey)
	ON_COMMAND(ID_VIEW_SEARCHINCONTENT, &CCardView::OnViewSearchincontent)
	ON_COMMAND(ID_VIEW_BOOKMARKMODE, &CCardView::OnViewBookmarkmode)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BOOKMARKMODE, &CCardView::OnUpdateViewBookmarkmode)
    ON_COMMAND(ID_VIEW_URL, &CCardView::OnViewUrl)
    ON_UPDATE_COMMAND_UI(ID_VIEW_URL, &CCardView::OnUpdateViewUrl)
    ON_COMMAND(ID_EDIT_CATEGORY, &CCardView::OnEditCategory)
	ON_COMMAND(ID_EDIT_TAG, &CCardView::OnEditTag)
	ON_CBN_CLOSEUP(IDC_TAG_COMBO, &CCardView::OnCbnCloseupItemTagCombo)
	ON_COMMAND(ID_VIEW_READONLYMODE, &CCardView::OnViewReadonlymode)
	ON_UPDATE_COMMAND_UI(ID_VIEW_READONLYMODE, &CCardView::OnUpdateViewReadonlymode)
	ON_COMMAND(ID_VIEW_TESTMODE, &CCardView::OnViewTestmode)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TESTMODE, &CCardView::OnUpdateViewTestmode)
	ON_COMMAND(ID_EDIT_COMPACTDB, &CCardView::OnEditCompactdb)
    ON_WM_PARENTNOTIFY()
	ON_COMMAND(ID_VIEW_PREVVIEW, &CCardView::OnViewPrevview)
	ON_COMMAND(ID_VIEW_NEXTVIEW, &CCardView::OnViewNextview)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PREVVIEW, &CCardView::OnUpdateViewPrevview)
	ON_UPDATE_COMMAND_UI(ID_VIEW_NEXTVIEW, &CCardView::OnUpdateViewNextview)
    ON_WM_TIMER()
    ON_COMMAND(IDD_DB_CHANGE, &CCardView::OnDbChange)
    ON_BN_CLICKED(IDC_BOOKMARK, &CCardView::OnBnClickedBookmark)
    ON_COMMAND(ID_EDIT_RELOAD, &CCardView::OnEditReload)
    ON_COMMAND(ID_EDIT_SETBOOKMARK, &CCardView::OnEditSetbookmark)
    ON_COMMAND(ID_EDIT_RESETBOOKMARK, &CCardView::OnEditResetbookmark)
    ON_COMMAND(ID_EDIT_MOVETO, &CCardView::OnEditMoveto)
    ON_UPDATE_COMMAND_UI(ID_EDIT_MOVETO, &CCardView::OnUpdateEditMoveto)
    ON_COMMAND(ID_EDIT_SETNUMBERING, &CCardView::OnEditSetnumbering)
    ON_COMMAND(ID_EDIT_EXPORTTOHTML, &CCardView::OnEditExporttohtml)
    ON_COMMAND(IDC_PUSH_FOR_SYNC, &CCardView::OnPushForSync)
    ON_UPDATE_COMMAND_UI(IDC_PUSH_FOR_SYNC, &CCardView::OnUpdatePushForSync)
	ON_COMMAND(ID_VIEW_COPY_URL, &CCardView::OnViewCopyUrl)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COPY_URL, &CCardView::OnUpdateViewUrl)
END_MESSAGE_MAP()

// CCardView construction/destruction

CCardView::CCardView()
	: CFormView(CCardView::IDD)
	, m_readOnlyMode(FALSE)
	, m_testMode(FALSE)
	, m_itemCategoryChange(FALSE)
	, m_showContent(TRUE)
	, m_info(_T(""))
	, m_itemBookmark(FALSE)
	, m_itemBookmarkChange(FALSE)
	, m_bookmarkMode(FALSE)
	, m_keyListView(NULL)
	, m_itemTagChange(FALSE)
	, m_dictMode(FALSE)
	, m_setSplitterPos(FALSE)
	, m_add(0)
{
	m_dbFolder = "db";
    m_currentTimer = 1000;
    m_searchView = NULL;
    m_pushForSyncTimer = -1;
}

CCardView::~CCardView()
{
	if (m_keyListView)
	{
//		m_keyListView->CloseWindow();
//		delete m_keyListView;
	}

//    if (m_pushForSyncTimer != -1)
//        KillTimer(m_pushForSyncTimer);
}

void CCardView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CATEGORY, m_itemCategory);
    DDX_Control(pDX, IDC_STATIC_PLACEMENT2, m_keyPlacement);
    DDX_Control(pDX, IDC_STATIC_PLACEMENT3, m_valuePlacement);
    DDX_Text(pDX, IDC_EDIT1, m_info);
    DDX_Check(pDX, IDC_BOOKMARK, m_itemBookmark);
    DDX_Control(pDX, IDC_TAG_COMBO, m_itemTagCombo);
    DDX_Control(pDX, IDC_PUSH_FOR_SYNC, m_syncButton);
}

BOOL CCardView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CCardView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	ImageStringConvertTest();
	m_useWorkingFolder = GetEnv(PROFILE_SECTION, USE_WORKING_FOLDER, 0);

	if (OpenDB() == -1)
		return;

	if (m_db.GetCurDb() != NULL)
		m_dictMode = m_db.IsDictMode(m_db.GetCurDb());

	CreateEditors();
	CMainFrame* frame=(CMainFrame*) AfxGetMainWnd();
	m_section = &frame->m_section;
	m_category = &frame->m_category;
	m_bookmarkBtn = &frame->m_bookmarkBtn;
//	m_readOnlyBtn = &frame->m_readOnlyBtn;
//	m_testBtn = &frame->m_testBtn;
	m_tagCombo = &frame->m_tag;
    m_orBtn = &frame->m_orBtn;

	m_content.SetReadOnly(m_readOnlyMode);
	m_key.SetReadOnly(m_readOnlyMode);

	if (m_db.GetCurDb() != NULL)
	{
		UpdateSection();
		UpdateCategory();
		UpdateTag();
		m_bookmarkMode = m_db.GetCurBookmarkMode();
	}

	m_bookmarkBtn->SetCheck(m_bookmarkMode);
    TriggerPush();

//	GetInfo(m_info);
//	UpdateData(0);

	DisplayText();
	m_key.SetFocus();
}

LRESULT  CCardView::OnResizerUnhook(WPARAM wParam, LPARAM lParam)
{
//	UINT	pos;

//	m_resizer.GetSplitterPosition(_T("OutterSplit"), pos);
//	AfxGetApp()->WriteProfileInt(PROFILE_SECTION, KEY_WND_HEIGHT, pos);
	return 0;
}

LRESULT  CCardView::OnResizerPortionChange(WPARAM wParam, LPARAM lParam)
{
	UINT	pos;

	m_resizer.GetSplitterPosition(_T("OutterSplit"), pos);
	AfxGetApp()->WriteProfileInt(PROFILE_SECTION, KEY_WND_HEIGHT, pos);
	return 0;
}

LRESULT  CCardView::OnResizerSize(WPARAM wParam, LPARAM lParam)
{
	if (m_setSplitterPos)
	{
		UINT	pos;
	
		pos = AfxGetApp()->GetProfileInt(PROFILE_SECTION, KEY_WND_HEIGHT, 120);
		m_resizer.SetSplitterPosition(_T("OutterSplit"), pos);
		m_setSplitterPos = FALSE;
	}
	return 0;
}

LRESULT  CCardView::OnLinkClick(WPARAM wParam, LPARAM lParam)
{
    CString s = *((CString *) lParam);

//    CString t = s.Left(7).MakeLower();

    int i = s.Find(':');

    if (i == -1) 
    {
        TRACE("Invalid Protocol");
        return 0;
    }

    CString t = s.Left(i).MakeLower();
    TRACE("%s\n", s);

    if (t != "http" && t != "https" && t != "card" && t != "onenote")
        return 0;

    if (t == "card")
    {
        t = s.Mid(7);

        int curPos = 0;
        CString id, db, key, value;
        CString resToken= t.Tokenize(_T("&"),curPos);
        while (resToken != _T(""))
        {
            int     pos = 0;
            key = resToken.Tokenize(_T("="), pos);
            value = resToken.Tokenize(_T("="), pos);

            if (key == "sec")
                db = value;
            else if (key == "id")
                id = value;

            resToken = t.Tokenize(_T("% #"), curPos);
        }; 

		GotoById(db, id, TRUE, TRUE);
    }
    else
    {
        ShellExecute(NULL, "Open", s, NULL, NULL, SW_SHOW);
    }

    return 0;
}

LRESULT  CCardView::OnLinkSet(WPARAM wParam, LPARAM lParam)
{
    CLinkSelect             dlg;
    CRulerRichEditCtrl *    ctl = (CRulerRichEditCtrl *) lParam;
    CString *               s = (CString *) wParam;

    if (ctl == NULL || s == NULL)
        return 0;

    dlg.m_display = *s;
    dlg.m_db = &m_db;
    if (dlg.DoModal() != IDOK)
        return 0;

    TRACE("%s", dlg.m_address_onenote);
    if (dlg.m_display != "" || dlg.m_address != "") 
    {
        ctl->SetLink(dlg.m_display, dlg.m_address);
        return 0;
    }
    else if (dlg.m_address_onenote != "")
    {
        int curPos = 0;
        CString http, onenote;

        CString s = dlg.m_address_onenote.Tokenize(_T("\r\n"), curPos);
        while (s != _T(""))
        {
            int i = s.Find(':');
            if (i != -1) 
            {
                CString t = s.Left(i);

                if (t == "https" || t == "http")
                    http = s;
                else if (t == "onenote")
                    onenote = s;
            }
            s = dlg.m_address_onenote.Tokenize(_T("\r\n"), curPos);
        };

        CString title;

        if (http != "") 
        {
            if (dlg.m_display == "") 
            {
                int i = http.Find(".one");
                i = http.Find("%2F", i);
                if (i != -1) 
                {
                    int start = i + strlen("%2F");
                    int end = http.Find("%7C", start);

                    title = http.Mid(start, end - start);
                }
               
                title = CString("Onenote Online: ") + title;
                title = urlDecode((LPCTSTR) title).c_str();
            }
            else 
            {
                 title.Format("Onenote Online: %s", dlg.m_display);
            }

            //ctl->SetLink(title, http, true);
        }
        if (onenote != "") 
        {
            if (dlg.m_display == "") 
            {
                int i = onenote.Find(".one");
                i = onenote.Find("#", i);
                if (i != -1) 
                {
                    int start = i + 1;
                    int end = onenote.Find("&", start);

                    title = onenote.Mid(start, end - start);
                }
               
                title = CString("Onenote: ") + title;
                title = urlDecode((LPCTSTR) title).c_str();
            }
            else 
            {
                 title.Format("Onenote: %s", dlg.m_display);
            }

            ctl->SetLink(title, onenote, true);
        }    
    }

    return 0;
}

void  CCardView::OnOpenReq()
{
    CString str;
    map<string, string>     m;
    char tmp[4096] = {0};

    GetSetting("req", str);
    if (str == "" || str.Left(5) != "card:")
        return;

    UrlDecode((LPCTSTR) str.Mid(5), tmp);
    char *p = tmp;

	p = strtok(p, "=");
    while(p) 
    {
        string key = p;
        p = strtok(0, "&");
        string value = p;

        m[key] = value;
        p = strtok(0, "=");
    }
	
    if (m.find("cmd") == m.end()) 
    {
        if (m.find("sec") == m.end())
            return;
        
        CString sec = m["sec"].c_str();
        CString id = m["id"].c_str();
        GotoById(sec, id, TRUE, FALSE);

    }
    return;
}

// CCardView diagnostics

#ifdef _DEBUG
void CCardView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCardView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CCardDoc* CCardView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCardDoc)));
	return (CCardDoc*)m_pDocument;
}
#endif //_DEBUG

int CCardView::SetItem()
{
	CString			key;
	CString			desc;
	int				res = 0;
	int				cate;
	CUIntArray		tagList;

	CheckBookmarkChange();
	cate = m_itemCategory.GetItemData(m_itemCategory.GetCurSel());

	GetTagList(m_itemTagCombo, tagList);

	if (m_add == 1)
	{
		if ((m_itemTagChange || m_itemBookmarkChange || m_itemCategoryChange 
			|| m_key.GetModify() 
			|| m_content.GetModify()))
		{
			m_key.GetText(key);

			if (m_dictMode)
            {
                if (key == "")
			    {
				    MessageBox("Key is Empty");
				    return 0;
			    }
                m_content.SetAllFont("Arial");
                m_content.GetText(desc);
                m_db.NormailizeDicText(desc, key);
            }
            else
            {
			    m_content.GetText(desc);
            }

			m_db.AddItem(cate, m_itemBookmark, tagList, key, desc);

			res = 1;
			m_add = 0;
			m_key.SetModify(0);
			m_content.SetModify(0);
			m_itemBookmarkChange = FALSE;
			m_itemCategoryChange = FALSE;
			m_itemTagChange = FALSE;
            if (m_db.GetSort() == "ASC")
			    m_db.Last();
            else
                m_db.First();

			UpdateKeyListView();
		} 
	}
	else
	{
		if (m_itemCategoryChange || m_itemBookmarkChange || m_itemTagChange ||
			m_key.GetModify() || 
			m_content.GetModify())
		{
			int mask = 0;

			if (m_key.GetModify())
			{
				m_key.GetText(key);
				if (m_dictMode && key == "")
				{
					MessageBox("Key is Empty");
					return 0;
				}
			}
			else if (m_dictMode)
				m_key.GetText(key);

			if (m_content.GetModify())
			{
			    if (m_dictMode)
                {
                    m_content.SetAllFont("Arial");
                    m_content.GetText(desc);
                    m_db.NormailizeDicText(desc, key);
                }
                else
                    m_content.GetText(desc);
			}
			else
			{
				desc = "";
			}

			if (m_itemCategoryChange)
				mask |= CATEGORY_UPDATE;

			if (m_itemBookmarkChange)
				mask |= BOOKMARK_UPDATE;

			if (m_itemTagChange)
				mask |= TAG_UPDATE;

			if (m_key.GetModify())
				mask |= KEY_UPDATE;

			if (m_content.GetModify())
				mask |= VALUE_UPDATE;

			m_db.UpdateItem(mask, cate, m_itemBookmark, tagList, key, desc);
			res = 2;
			m_add = 0;
			m_key.SetModify(0);
			m_content.SetModify(0);
			m_itemCategoryChange = FALSE;
			m_itemBookmarkChange = FALSE;
			m_itemTagChange = FALSE;

			UpdateKeyListView();
		}
	}

	return res;
}

void CCardView::DisplayText(BOOL blank)
{
	CString			s;

	if (blank || m_db.GetItemCount() == 0)
	{
		m_key.SelectAll();
		m_key.Clear();
		m_content.SelectAll();
		m_content.Clear();
		m_itemTagCombo.SelectAll(false);
		m_itemBookmark = 0;
		m_key.ReplaceSel("");
		m_content.ReplaceSel("");
	    if (m_db.GetCurCategory() == -1)
		    m_itemCategory.SetCurSel(0);
	    else
		    m_itemCategory.SetCurSel(FindItemData(*m_category, m_db.GetCurCategory()) - 1);

	}
	else
	{
#if _DEBUG
		m_db.GetCurKey(s);
        Save("curKey.rtf", s);
		m_key.SetText(s);
		m_db.GetCurContent(s);
        Save("curContent.rtf", s);
#else
		m_db.GetCurKey(s);
		m_key.SetText(s);
		m_db.GetCurContent(s);
#endif

		m_content.SetText(s);

		if (m_db.GetCurItemCategory() == -1)
			m_itemCategory.SetCurSel(0);
		else 
		{
			m_itemCategory.SetCurSel(FindItemData(m_itemCategory, m_db.GetCurItemCategory()));
		}
		m_itemBookmark = m_db.GetCurItemBookmark();
		SetTagList(m_itemTagCombo, m_db.GetCurItemTag());
	}

	if (m_add == 1 || m_db.GetItemCount() > 0)
	{
		m_key.SetReadOnly(FALSE);
		m_content.SetReadOnly(FALSE);
	}
	else //if (m_add == 0 && (m_db.GetItemCount() == 0))
	{
		m_key.SetReadOnly(TRUE);
		m_content.SetReadOnly(TRUE);
	}

	m_key.SetModify(0);
	m_content.SetModify(0);
	m_itemCategoryChange = FALSE; 
	m_itemBookmarkChange = FALSE;
	m_itemTagChange = FALSE;

	GetInfo(m_info);
	if (m_add == 1)
		m_info = "(ADD) " + m_info;

	if (m_bookmarkMode)
		m_info = "(BOOKMARK) " + m_info;

	UpdateData(0);

	ShowContent(m_showContent);
}

void CCardView::ShowContent(int bShow)
{
	if (bShow)
	{
		m_content.ShowWindow(SW_SHOWNORMAL);
	}
	else
	{
		m_content.ShowWindow(SW_HIDE);
	}
}

// CCardView message handlers

void CCardView::OnFileSaveIos()
{
	SaveAsFc();
}

void CCardView::OnEditAdditem()
{
//	if (m_add == 1)
		SetItem();

	m_add = 1;
	m_bookmarkMode = FALSE;
	m_bookmarkBtn->SetCheck(m_bookmarkMode);
	m_db.ChangeBookmarkMode(m_bookmarkMode);
	UpdateKeyListView();

    // reset tag
    for(int i = 0; i < (int) m_itemTagCombo.GetCount(); i++)
	{
		m_itemTagCombo.SetCheck(i, FALSE);
	}

	DisplayText(1);
	m_key.SetFocus();
}

void CCardView::OnEditRemoveitem()
{
	int		res = MessageBox("Do you really want to remove?", "Warning",
				MB_YESNO);

	if (res == IDNO)
		return;

	m_db.DeleteItem();
	DisplayText();
	UpdateKeyListView();
}

void CCardView::OnEditSave()
{
	if (m_itemCategoryChange || m_itemBookmarkChange || m_itemTagChange ||
		m_key.GetModify() || m_content.GetModify())
	{
        BOOL  updateDisplay = false;

        if (m_itemCategoryChange || m_itemBookmarkChange || m_itemTagChange || m_add == 1)
            updateDisplay = true;

		SetItem();
        if (m_db.IsDictMode(m_db.GetCurDb()) || updateDisplay)
		    DisplayText();
	}
}

void CCardView::OnEditMovetofirst()
{
	if (m_db.GetItemCount() == 0)
		return;

	if (m_testMode)
		m_showContent = false;
	else
		m_showContent = true;

	SetItem();
	m_add = 0;

	m_db.ToFirst();
	DisplayText();
	UpdateKeyListView();
}

void CCardView::OnEditMovetoprev()
{
	if (m_db.GetItemCount() == 0)
		return;

	if (m_testMode)
		m_showContent = false;
	else
		m_showContent = true;

	SetItem();
	m_add = 0;

	m_db.ToPrev();
	DisplayText();
	UpdateKeyListView();
}

void CCardView::OnEditMovetonext()
{
	if (m_db.GetItemCount() == 0)
		return;

	if (m_testMode)
		m_showContent = false;
	else
		m_showContent = true;

	SetItem();
	m_add = 0;

	m_db.ToNext();
	DisplayText();
	UpdateKeyListView();
}


void CCardView::OnEditMoveto()
{
    CMoveTo     dlg;

	if (m_db.GetItemCount() == 0 || !m_keyListView)
		return;

    dlg.m_max = m_db.GetItemCount() + 1;
    if (dlg.DoModal() == IDCANCEL)
        return;
    
    if (dlg.m_number < 1 || dlg.m_number > dlg.m_max)
    {
        MessageBox("Invalid Number");
        return;
    }
    std::vector<std::pair<int, int>> src;

    m_keyListView->GetIdListofSelection(src);
    if (src.size() == 0)
    {
        MessageBox("Nothing is selected");
        return;
    }

    if (m_testMode)
		m_showContent = false;
	else
		m_showContent = true;

	SetItem();
	m_add = 0;

	if (m_db.MoveTo(src, dlg.m_number - 1) == -1)
    {
        MessageBox("Fail to move.   inc size failure");
    }

	DisplayText();
	UpdateKeyListView();
}



void CCardView::OnViewFirst()
{
	if (m_db.GetItemCount() == 0)
		return;

	SetItem();
	m_add = 0;

	SetHistory();
	m_db.First();

	if (m_testMode)
		m_showContent = false;
	else
		m_showContent = true;

	DisplayText();
}

void CCardView::OnViewPrev()
{
	int			res;

	if (m_db.GetItemCount() == 0)
		return;

	if (m_testMode)
	{
		m_showContent = !m_showContent;
		if (m_showContent == false)
		{
			ShowContent(m_showContent);
			return;
		}
	}
	else
		m_showContent = true;

	res = SetItem();
	if (m_add == 0 || res != 0)
	{
		SetHistory();
		m_db.Prev();
	}

	m_add = 0;
	DisplayText();
}

void CCardView::OnViewNext()
{
	if (m_db.GetItemCount() == 0)
		return;

	if (m_testMode)
	{
		m_showContent = !m_showContent;
		if (m_showContent == (BOOL) true)
		{
			ShowContent(m_showContent);
			return;
		}
	}
	else
		m_showContent = true;

	SetItem();
	m_add = 0;

	SetHistory();
	m_db.Next();
	DisplayText();
}

void CCardView::OnViewLast()
{
	if (m_db.GetItemCount() == 0)
		return;

	if (m_testMode)
		m_showContent = false;
	else
		m_showContent = true;

	SetItem();
	m_add = 0;

	SetHistory();
	m_db.Last();
	DisplayText();
}

void CCardView::OnWindowKeyList()
{
	if (!m_keyListView)
	{
		m_keyListView = new CKeyList();
		m_keyListView->m_db = &m_db;
		m_keyListView->m_parent = this;
	}

    if (!::IsWindow(m_keyListView->GetSafeHwnd()))
	{
        m_keyListView->Create(IDD_DIALOG2, this);
	}
	else
		m_keyListView->FillItem();

}

void CCardView::OnWindowSearch()
{
	CString			s;
	StringMapArray	result;
    CSearchBox      search;

    if (search.DoModal() != IDOK)
        return;
   
    s = search.m_string;
	s = s.Trim(" \t\n\r");
	if (s == "")
		return;

	if (m_db.SearchKey(result, s, LINE_FEED, false) == 0)
	{
		MessageBox(s + " doesn't exist", MB_OK);
		if (m_searchView)
			m_searchView->Clear();

		return;
	}

	if (!m_searchView)
	{
		m_searchView = new CSearchResult();
		m_searchView->m_db = &m_db;
		m_searchView->m_parent = this;
	}

    if (!::IsWindow(m_searchView->GetSafeHwnd()))
	{
        m_searchView->Create(IDD_SEARCH, this);
	}

	m_searchView->FillItem(s, result);
    m_searchView->ShowWindow(SW_SHOW); 
}

void CCardView::OnCbnSelchangeSection()
{
	CString		s;
	BOOL		oldMode = m_dictMode;

	int i = m_section->GetCurSel();
	
	if (i < 0)
		return;

	SetItem();
	m_add = 0;

	m_section->GetLBText(i, s);

	SetHistory();
	m_db.ChangeDb(s);
//	m_db.ChangeCategory(-1);
	
	UpdateCategory();
	UpdateKeyListView();
	UpdateTag();

	m_dictMode = m_db.IsDictMode(m_db.GetCurDb());
	if (m_dictMode != oldMode)
	{
		m_key.DestroyWindow();
		m_content.DestroyWindow();
		CreateEditors();
	}
	DisplayText();
	GetDocument()->SetTitle(m_db.GetCurDbName());
}

void CCardView::OnCbnSelchangeCategory()
{
	int i = m_category->GetCurSel();
	
	if (i < 0)
		return;
	
	SetItem();
	m_add = 0;

	if (m_category->GetItemData(i) == m_db.GetCurCategory())
		return;

	SetHistory();
	m_db.ChangeCategory(m_category->GetItemData(i));
	
	UpdateKeyListView();
	DisplayText();
}

void CCardView::OnCbnSelchangeTagCombo()
{
	CUIntArray		tag;

	if (m_db.GetCurDb() == NULL)
		return;

	GetTagList(*m_tagCombo, tag);

	if (CompareTagList(tag, m_db.GetCurTag()))
		return;

	SetItem();
	m_add = 0;

	SetHistory();
	m_db.ChangeTag(tag);
	
	UpdateKeyListView();
	DisplayText();
}

void CCardView::OnBnClickedBookmarkmode()
{
	if (m_bookmarkBtn->GetCheck() == BST_CHECKED)
	{
		m_bookmarkMode = TRUE;
	}
	else
	{
		m_bookmarkMode = FALSE;
	}

	if (m_db.GetCurDb() == NULL)
		return;

	SetHistory();
	m_db.ChangeBookmarkMode(m_bookmarkMode);
	UpdateKeyListView();
	DisplayText();
}

/*
void CCardView::OnBnClickedReadonlymode()
{
	if (m_readOnlyBtn->GetCheck() == BST_CHECKED)
	{
		m_readOnlyMode = TRUE;
	}
	else
	{
		m_readOnlyMode = FALSE;
	}

	m_content.SetReadOnly(m_readOnlyMode);
	m_key.SetReadOnly(m_readOnlyMode);
	m_key.ShowToolbar(!m_readOnlyMode);
	m_content.ShowToolbar(!m_readOnlyMode);
}
*/

void CCardView::OnViewReadonlymode()
{
	m_readOnlyMode = !m_readOnlyMode;

	m_content.SetReadOnly(m_readOnlyMode);
	m_key.SetReadOnly(m_readOnlyMode);
	m_key.ShowToolbar(!m_readOnlyMode);
	m_content.ShowToolbar(!m_readOnlyMode);
}


void CCardView::OnUpdateViewReadonlymode(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_readOnlyMode);
}

void CCardView::OnBnClickedSearchmode()
{
    int     mode = 0;

	if (m_orBtn->GetCheck() == BST_CHECKED)
	{
		mode = 1;
	}
	else
	{
		mode = 0;
	}

  	if (m_db.GetCurDb() == NULL)
		return;

	SetItem();
	m_add = 0;

	SetHistory();
	m_db.ChangeTagSearchMode(mode);
	
	UpdateKeyListView();
	DisplayText();
}

/*
void CCardView::OnBnClickedTestmode()
{
	if (m_testBtn->GetCheck() == BST_CHECKED)
	{
		m_testMode = TRUE;
	}
	else
	{
		m_testMode = FALSE;
	}

	if (m_testMode)
	{
		m_readOnlyMode = TRUE;
		ShowContent(FALSE);
		m_content.SetReadOnly(m_readOnlyMode);
		m_key.SetReadOnly(m_readOnlyMode);
		m_key.ShowToolbar(FALSE);
		m_content.ShowToolbar(FALSE);
		m_readOnlyBtn->SetCheck(m_readOnlyMode);
	}
	else
	{
		ShowContent(TRUE);
		m_content.SetReadOnly(m_readOnlyMode);
		m_key.SetReadOnly(m_readOnlyMode);
		m_key.ShowToolbar(!m_readOnlyMode);
		m_content.ShowToolbar(!m_readOnlyMode);
	}
}
*/

void CCardView::OnViewTestmode()
{
	m_testMode = !m_testMode;
	if (m_testMode)
	{
		m_readOnlyMode = TRUE;
		ShowContent(FALSE);
		m_content.SetReadOnly(m_readOnlyMode);
		m_key.SetReadOnly(m_readOnlyMode);
		m_key.ShowToolbar(FALSE);
		m_content.ShowToolbar(FALSE);
//		m_readOnlyBtn->SetCheck(m_readOnlyMode);
	}
	else
	{
		ShowContent(TRUE);
		m_content.SetReadOnly(m_readOnlyMode);
		m_key.SetReadOnly(m_readOnlyMode);
		m_key.ShowToolbar(!m_readOnlyMode);
		m_content.ShowToolbar(!m_readOnlyMode);
	}
}

void CCardView::OnUpdateViewTestmode(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_testMode);
}


void CCardView::OnCbnSelchangeItemCategory()
{
	int i = m_itemCategory.GetCurSel();

	if (i == -1)
		return;

	if (m_itemCategory.GetItemData(i) != m_db.GetCurItemCategory())
		m_itemCategoryChange = TRUE;
	else
		m_itemCategoryChange = FALSE;
}

void CCardView::OnEditSetting()
{
	CSetting	setting;
	BOOL		oldMode = m_dictMode;

	m_db.BackupEnv();
	setting.m_db = &m_db;

	setting.DoModal();

	m_db.RestoreEnv();
	if (m_db.GetCurDb() == NULL)
	{
		m_dictMode = FALSE;
	}
	else
		m_dictMode = m_db.IsDictMode(m_db.GetCurDb());

	if (m_dictMode != oldMode)
	{
		m_key.DestroyWindow();
		m_content.DestroyWindow();
		CreateEditors();
	}

	UpdateSection();
	UpdateCategory();
	UpdateTag();
	DisplayText();
}

void CCardView::OnEditCategory()
{
	CCategory	dlg;

	m_db.BackupEnv();
	dlg.m_db = &m_db;
	dlg.DoModal();
	m_db.RestoreEnv();
	UpdateSection();
	UpdateCategory();
	UpdateTag();
	DisplayText();
}

void CCardView::OnEditTag()
{
	CTag	dlg;

	m_db.BackupEnv();
	dlg.m_db = &m_db;
    dlg.m_cardView = this;
	dlg.DoModal();
	m_db.RestoreEnv();
	UpdateSection();
	UpdateCategory();
	UpdateTag();
	DisplayText();
}


void CCardView::OnUpdateViewPrev(CCmdUI *pCmdUI)
{
	if (m_db.GetItemIndex() <= 0 || m_db.GetItemCount() == 0)
		pCmdUI->Enable(0);
	else
		pCmdUI->Enable(1);
}

void CCardView::OnUpdateViewNext(CCmdUI *pCmdUI)
{
	if (m_db.GetItemIndex() >= m_db.GetItemCount() - 1 || m_db.GetItemCount() == 0)
		pCmdUI->Enable(0);
	else
		pCmdUI->Enable(1);
}

void CCardView::OnUpdateEditSave(CCmdUI *pCmdUI)
{
	if (m_key.GetModify() ||
		m_content.GetModify() ||
		m_itemCategoryChange || 
		m_itemBookmarkChange ||
		m_itemTagChange)
		pCmdUI->Enable(1);
	else
		pCmdUI->Enable(0);
}


void CCardView::OnUpdateEditAdditem(CCmdUI *pCmdUI)
{
	if (m_db.GetCurDb() == NULL)
		pCmdUI->Enable(0);
	else
		pCmdUI->Enable(1);
}

void CCardView::OnUpdateEditCheckitem(CCmdUI *pCmdUI)
{
	if (m_db.GetItemCount() == 0)
		pCmdUI->Enable(0);
	else
		pCmdUI->Enable(1);
}

void CCardView::OnUpdateEditMovetofirst(CCmdUI *pCmdUI)
{
	if (m_db.GetItemIndex() <= 0 || m_db.GetItemCount() == 0)
		pCmdUI->Enable(0);
	else
		pCmdUI->Enable(1);
}

void CCardView::OnUpdateEditMovetoprev(CCmdUI *pCmdUI)
{
	if (m_db.GetItemIndex() <= 0 || m_db.GetItemCount() == 0)
		pCmdUI->Enable(0);
	else
		pCmdUI->Enable(1);
}

void CCardView::OnUpdateEditMovetonext(CCmdUI *pCmdUI)
{
	if (m_db.GetItemCount() == 0 || 
		m_db.GetItemIndex() >= m_db.GetItemCount() - 1)
		pCmdUI->Enable(0);
	else
		pCmdUI->Enable(1);
}

void CCardView::OnUpdateEditMoveto(CCmdUI *pCmdUI)
{
	if (!m_keyListView || m_db.GetItemCount() == 0)
		pCmdUI->Enable(0);
	else
		pCmdUI->Enable(1);

}


BOOL CCardView::PreTranslateMessage(MSG* pMsg)
{
    if(pMsg->message==WM_KEYDOWN)
    {
		switch(pMsg->wParam)
		{
		case VK_F2:
			break;
		case VK_F3:
			break;
		case VK_F4:
			break;
		case VK_F5:
			break;
		case VK_F6:
			break;
		case VK_F7:
			break;
		case VK_F8:
			break;
		case VK_F9:
			break;
		case VK_F10:
			break;
		case VK_F11:
			break;
		case 32:
			break;
        case VK_ESCAPE:
            OnEditReload();
            break;
		}	
	}
	return CFormView::PreTranslateMessage(pMsg);
}

void CCardView::OnEditCopy()
{
	if (GetFocus() == m_key.GetWnd())
		m_key.Copy();
	else if (GetFocus() == m_content.GetWnd())
		m_content.Copy();
}

void CCardView::OnEditPaste()
{
	if (GetFocus() == m_key.GetWnd())
	{
		m_key.Paste();
		return;
	}
	else if (GetFocus() == m_content.GetWnd())
	{
		m_content.Paste();
		return;
	}
}

void CCardView::OnEditCut()
{
	if (GetFocus() == m_key.GetWnd())
		m_key.Cut();
	else if (GetFocus() == m_content.GetWnd())
		m_content.Cut();
}

void CCardView::OnEditUndo()
{
	if (GetFocus() == m_key.GetWnd())
		m_key.Undo();
	else if (GetFocus() == m_content.GetWnd())
		m_content.Undo();
}

void CCardView::OnEditSelectAll()
{
	if (GetFocus() == m_key.GetWnd())
		m_key.SelectAll();
	else if (GetFocus() == m_content.GetWnd())
		m_content.SelectAll();
}

void CCardView::GotoByitime(int itime)
{
	if (m_db.GetItemCount() == 0)
		return;

	if (m_testMode)
		m_showContent = false;
	else
		m_showContent = true;

	SetItem();
	m_add = 0;

	m_db.GotoByitime(itime);
	DisplayText();
	
}

void CCardView::OnViewGoto()
{
	CGoto	dlg;

	dlg.m_db = &m_db;
    if (dlg.DoModal() != IDOK)
        return;

	if (dlg.m_curDb == "" || dlg.m_curId == "")
		return;

	GotoById(dlg.m_curDb, dlg.m_curId, TRUE, FALSE);
}

void CCardView::OnGoto(int index)
{
	if (m_db.GetItemCount() == 0)
		return;

	if (m_testMode)
		m_showContent = false;
	else
		m_showContent = true;

	SetItem();
	m_add = 0;

	SetHistory();
	m_db.Goto(index);
	DisplayText();
}

void CCardView::GotoById(LPCTSTR db, LPCTSTR id, BOOL setHistory, BOOL delayUpdate)
{
    if (m_db.GetItemCount() == 0 || db == NULL)
	    return;

    if (m_testMode)
	    m_showContent = false;
    else
	    m_showContent = true;

    SetItem();
    m_add = 0;

	if (setHistory)
		SetHistory();

	BOOL		oldMode = m_dictMode;
    bool        bUpdateSection = false;
    if (db != m_db.GetCurDbName())
    {
        bUpdateSection = true;
    }

    m_db.GotoByid(db, id, TRUE);

    if (bUpdateSection)
    {
	    m_dictMode = m_db.IsDictMode(m_db.GetCurDb());
	    if (m_dictMode != oldMode)
	    {
            if (delayUpdate)
            {
                SetTimer(1, 100, 0);
                return;
            }
	        m_key.DestroyWindow();
	        m_content.DestroyWindow();
	        CreateEditors();
	    }
  	    m_section->SetCurSel(m_section->FindStringExact(0, m_db.GetCurDbName()));
   }

    UpdateCategory();
    UpdateTag();
    UpdateKeyListView();

    DisplayText();
    m_bookmarkMode = m_db.GetCurBookmarkMode();
    m_bookmarkBtn->SetCheck(m_bookmarkMode);
}

void CCardView::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == 1)
    {
	    m_key.DestroyWindow();
	    m_content.DestroyWindow();
	    CreateEditors();

 	    m_section->SetCurSel(m_section->FindStringExact(0, m_db.GetCurDbName()));
        UpdateCategory();
        UpdateTag();
        UpdateKeyListView();

        DisplayText();
        m_bookmarkMode = m_db.GetCurBookmarkMode();
        m_bookmarkBtn->SetCheck(m_bookmarkMode);
        KillTimer(1);
    }
    else if (nIDEvent == PUSHFORSYNC_TIMER) {
        TriggerPush();
    }
    else if (nIDEvent >= 1000 && nIDEvent <= 5000)
    {
/*
        NotifyMap::iterator     it;

        it = m_notifyMap.find(nIDEvent);
        if (it == m_notifyMap.end())
        {
            TRACE("Invalid timer\n");
            KillTimer(nIDEvent);
            return;
        }
        
        CString file = it->second.c_str();
        m_notifyMap.erase(nIDEvent);
        m_notifySet.erase((LPCTSTR) file);
        TRACE("Process file by notification %s\n", file);
        if (m_db.SyncO2W(file) == FALSE)
	    {
            KillTimer(nIDEvent);
            return;
	    }
	    if (m_db.GetCurDb() != NULL)
	    {
		    UpdateSection();
		    UpdateCategory();
		    UpdateTag();
		    UpdateKeyListView();
	    }
	    DisplayText();
	    TRACE("Notify Handled = %s\n", file);
*/
        KillTimer(nIDEvent);
        return;
    }

    CFormView::OnTimer(nIDEvent);
}


void CCardView::OnViewSearchinkey()
{
	// TODO: Add your command handler code here
}

void CCardView::OnViewSearchincontent()
{
	// TODO: Add your command handler code here
}

void CCardView::OnViewBookmarkmode()
{
	if (m_db.GetCurDb() == NULL)
		return;

	SetHistory();
	m_bookmarkMode = !m_bookmarkMode;
	m_db.ChangeBookmarkMode(m_bookmarkMode);
	UpdateKeyListView();
	DisplayText();
}

void CCardView::OnUpdateViewBookmarkmode(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bookmarkMode);
}

void CCardView::toClipboard(CString &s) {
    ::OpenClipboard(0);
    ::EmptyClipboard();
    HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.GetLength() + 1);

    if (!hg) {
        ::CloseClipboard();
        return;
    }

    memcpy(GlobalLock(hg), (LPCTSTR) s, s.GetLength() + 1);
    GlobalUnlock(hg);
    ::SetClipboardData(CF_TEXT, hg);
    ::CloseClipboard();
    GlobalFree(hg);
}

void CCardView::OnViewUrl()
{
    if (m_db.GetCurDb() == NULL)
        return;

    auto id = m_db.GetCurId();
    CString	section;

    auto i = m_section->GetCurSel();
    if (i >= 0)
        m_section->GetLBText(i, section);

    CString s;
    CString url = GetEnv(PROFILE_SECTION, URL, "http://localhost:9999/db/query2");

    s.Format("%s?type=2&sec=%s&id=%s\ncard:sec=%s&id=%s", url, section, id, section, id);
    toClipboard(s);
    TRACE("%s\n", s);

    s.Format("%s?type=2&sec=%s&id=%s", url, section, id);
    ShellExecute(NULL, _T("open"), s, NULL, NULL, SW_SHOWNORMAL);
}

void CCardView::OnUpdateViewUrl(CCmdUI *pCmdUI)
{
    if (m_db.GetCurDb() == NULL)
        pCmdUI->Enable(0);
    else
        pCmdUI->Enable(1);
}


void CCardView::OnViewCopyUrl()
{
	if (m_db.GetCurDb() == NULL)
		return;

	auto id = m_db.GetCurId();
	CString	section;

	auto i = m_section->GetCurSel();
	if (i >= 0)
		m_section->GetLBText(i, section);

	CString summary = m_db.GetCurKeyAsText();
	int position = 0;
	CString title = summary.Tokenize("\n", position);
	
	int start = summary.Find(_T("https://leetcode.com/problems/"));
	CString leet = _T("");
	if (start >= 0) {
		int end = start;
		while (end < summary.GetLength()) {
			if (summary[end] == ' ' || summary[end] == '\\r' || summary[end] == '\\n')
				leet = summary.Mid(start, end - start);

			end++;
		}
	}
	CString s;
	CString url = GetEnv(PROFILE_SECTION, URL, "http://localhost:9999/db/query2");

	s.Format("%s\t%s\t%s?type=2&sec=%s&id=%s\tcard:sec=%s&id=%s", title, leet, url, section, id, section, id);
	toClipboard(s);
}


void CCardView::UpdateKeyListView()
{
	if (m_keyListView == NULL)
		return;

	m_keyListView->FillItem();

	if (m_searchView)
		m_searchView->FillItem();
}


void CCardView::OnCbnCloseupItemTagCombo()
{
	if (m_db.GetCurDb() == NULL)
		return;

	m_itemTagChange = !CompareTagList(m_itemTagCombo, m_db.GetCurItemTag());
}

void CCardView::OnEditCompactdb()
{
	m_db.Compact();
}

void CCardView::OnViewPrevview()
{
	if (m_prevView.empty())
		return;

	ViewInfo vi;

	vi.db = m_db.GetCurDbName();
	vi.id = m_db.GetCurId();

	m_nextView.push_front(vi);
    if (m_nextView.size() > MAX_HISTORY)
        m_nextView.pop_back();

	vi = m_prevView.front();
	m_prevView.pop_front();
	GotoById(vi.db, vi.id, FALSE, FALSE);
}

void CCardView::OnViewNextview()
{
	if (m_nextView.empty())
		return;

	ViewInfo vi;
	
	vi.db = m_db.GetCurDbName();
	vi.id = m_db.GetCurId();

	m_prevView.push_front(vi);
    if (m_prevView.size() > MAX_HISTORY)
        m_prevView.pop_back();
	
	vi = m_nextView.front();
	m_nextView.pop_front();
	GotoById(vi.db, vi.id, FALSE, FALSE);
}

void CCardView::OnUpdateViewPrevview(CCmdUI *pCmdUI)
{
	if (m_prevView.empty())
		pCmdUI->Enable(0);
	else
		pCmdUI->Enable(1);
}

void CCardView::OnUpdateViewNextview(CCmdUI *pCmdUI)
{
	if (m_nextView.empty())
		pCmdUI->Enable(0);
	else
		pCmdUI->Enable(1);
}

void CCardView::OnDbChange()
{
    m_db.Refresh();
    DisplayText();
    UpdateKeyListView();
}

void CCardView::OnBnClickedBookmark()
{
    UpdateData();
    if (m_itemBookmark != m_db.GetCurItemBookmark())
    {
        m_itemBookmarkChange = TRUE;
    }
    else
        m_itemBookmarkChange = FALSE;
}

void CCardView::OnEditReload()
{
    m_add = 0;
    DisplayText();
}

void CCardView::OnEditSetbookmark()
{
	int		res = MessageBox("Do you want to set bookmark?", "Warning",
				MB_YESNO);

    if (res != IDYES)
        return;

    m_db.SetBookmark(1);
    m_add = 0;
    DisplayText();
}

void CCardView::OnEditResetbookmark()
{
	int		res = MessageBox("Do you want to reset bookmark?", "Warning",
				MB_YESNO);

    if (res != IDYES)
        return;

    m_db.SetBookmark(0);
    m_add = 0;
    DisplayText();
}


void CCardView::OnEditSetnumbering()
{
    int		res = MessageBox("Do you want to do numbering?", "Warning",
        MB_YESNO);

    if (res != IDYES)
        return;

    m_db.SetNumbering();
    m_add = 0;
    DisplayText();
    UpdateKeyListView();
}


void CCardView::OnEditExporttohtml()
{
    m_db.ExportToHtml();
}


void CCardView::OnPushForSync()
{
    TriggerPush();
}


void CCardView::OnUpdatePushForSync(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_db.HasPushList());
}

