// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Card.h"

#include "MainFrm.h"
#include "DbManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern const TCHAR* MyMainWndClassName;

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZING()
	ON_WM_CLOSE()
//	ON_WM_XBUTTONDOWN()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	int		nIndex;

	m_font.CreatePointFont(80, "Arial", NULL);  

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	/* 
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
*/
	CRect rect;

	
	/* search input */
	/*
	nIndex = m_wndToolBar.GetToolBarCtrl().CommandToIndex(ID_SEARCH_HOLDER);
	m_wndToolBar.SetButtonInfo(nIndex, ID_SEARCH_HOLDER, TBBS_SEPARATOR, 80);
	m_wndToolBar.GetToolBarCtrl().GetItemRect(nIndex, &rect);
	rect.top = 1;
	if(!m_search.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		rect, &m_wndToolBar, ID_SEARCH_HOLDER))
	{
		TRACE(_T("Failed to create combo-box\n"));
		return FALSE;
	}
    m_search.SendMessage(WM_SETFONT, (WPARAM)HFONT(m_font), TRUE); 
	*/
#if 0
	/* readonly mode */
	nIndex = m_wndToolBar.GetToolBarCtrl().CommandToIndex(ID_VIEW_READONLYMODE);
	m_wndToolBar.SetButtonInfo(nIndex, ID_VIEW_READONLYMODE, TBBS_SEPARATOR, 30);
	m_wndToolBar.GetToolBarCtrl().GetItemRect(nIndex, &rect);
	rect.left += 5;
	rect.right += 5;
	rect.top = 1; rect.bottom = rect.top + 22;
    if (!m_readOnlyBtn.Create("R",
        BS_AUTOCHECKBOX|WS_CHILD|WS_VISIBLE, rect, &m_wndToolBar,
        ID_VIEW_READONLYMODE)) 
    { 
        TRACE0("Failed to create CENTER check-box\n");       
        return FALSE;    
    }    
    m_readOnlyBtn.SendMessage(WM_SETFONT, (WPARAM)HFONT(m_font),TRUE);  

	/* test mode */
	nIndex = m_wndToolBar.GetToolBarCtrl().CommandToIndex(ID_VIEW_TESTMODE);
	m_wndToolBar.SetButtonInfo(nIndex, ID_VIEW_TESTMODE, TBBS_SEPARATOR, 30);
	m_wndToolBar.GetToolBarCtrl().GetItemRect(nIndex, &rect);
	rect.left += 5;
	rect.right += 5;
	rect.top = 1; rect.bottom = rect.top + 22;
    if (!m_testBtn.Create("T",
        BS_AUTOCHECKBOX|WS_CHILD|WS_VISIBLE, rect, &m_wndToolBar,
        ID_VIEW_TESTMODE)) 
    { 
        TRACE0("Failed to create CENTER check-box\n");       
        return FALSE;    
    }    
    m_testBtn.SendMessage(WM_SETFONT, (WPARAM)HFONT(m_font),TRUE);  
#endif


	/* section combobox */
	nIndex = m_wndToolBar.GetToolBarCtrl().CommandToIndex(ID_SECTION_HOLDER);
	m_wndToolBar.SetButtonInfo(nIndex, ID_SECTION_HOLDER, TBBS_SEPARATOR, 80);
	m_wndToolBar.GetToolBarCtrl().GetItemRect(nIndex, &rect);
	rect.top = 1;
	rect.bottom = rect.top + 250 /*drop height*/;
	if(!m_section.Create(CBS_DROPDOWNLIST | CBS_SORT | WS_VISIBLE |
		WS_TABSTOP | WS_VSCROLL, rect, &m_wndToolBar, ID_SECTION_HOLDER))
	{
		TRACE(_T("Failed to create combo-box\n"));
		return FALSE;
	}
    m_section.SendMessage(WM_SETFONT, (WPARAM)HFONT(m_font), TRUE); 

	/* category combobox */
	nIndex = m_wndToolBar.GetToolBarCtrl().CommandToIndex(ID_CATE_HOLDER);
	m_wndToolBar.SetButtonInfo(nIndex, ID_CATE_HOLDER, TBBS_SEPARATOR, 150);
	m_wndToolBar.GetToolBarCtrl().GetItemRect(nIndex, &rect);
	rect.top = 1;
	rect.bottom = rect.top + 400 /*drop height*/;
	if(!m_category.Create(CBS_DROPDOWNLIST | WS_VISIBLE |
		WS_TABSTOP | WS_VSCROLL, rect, &m_wndToolBar, ID_CATE_HOLDER))
	{
		TRACE(_T("Failed to create combo-box\n"));
		return FALSE;
	}
    m_category.SendMessage(WM_SETFONT, (WPARAM)HFONT(m_font), TRUE); 

	/* tag combobox */
	nIndex = m_wndToolBar.GetToolBarCtrl().CommandToIndex(ID_TAG_HOLDER);
	m_wndToolBar.SetButtonInfo(nIndex, ID_TAG_HOLDER, TBBS_SEPARATOR, 150);
	m_wndToolBar.GetToolBarCtrl().GetItemRect(nIndex, &rect);
	rect.top = 1;
	rect.bottom = rect.top + 400 /*drop height*/;
	if(!m_tag.Create(CBS_DROPDOWNLIST | WS_VISIBLE |
		WS_TABSTOP | WS_VSCROLL, rect, &m_wndToolBar, ID_TAG_HOLDER))
	{
		TRACE(_T("Failed to create combo-box\n"));
		return FALSE;
	}
    m_tag.SendMessage(WM_SETFONT, (WPARAM)HFONT(m_font), TRUE); 

	/* tag or mode */
	nIndex = m_wndToolBar.GetToolBarCtrl().CommandToIndex(ID_OR_TAG_HOLDER);
	m_wndToolBar.SetButtonInfo(nIndex, ID_OR_TAG_HOLDER, TBBS_SEPARATOR, 50);
	m_wndToolBar.GetToolBarCtrl().GetItemRect(nIndex, &rect);
	rect.left += 5;
	rect.right += 5;
	rect.top = 1; rect.bottom = rect.top + 22;
    if (!m_orBtn.Create("OR",
        BS_AUTOCHECKBOX|WS_CHILD|WS_VISIBLE, rect, &m_wndToolBar,
        ID_OR_TAG_HOLDER)) 
    { 
        TRACE0("Failed to create CENTER check-box\n");       
        return FALSE;    
    }    
    m_orBtn.SendMessage(WM_SETFONT, (WPARAM)HFONT(m_font),TRUE);  


	/* bookmark mode */
	nIndex = m_wndToolBar.GetToolBarCtrl().CommandToIndex(ID_VIEW_BOOKMARKMODE);
	m_wndToolBar.SetButtonInfo(nIndex, ID_VIEW_BOOKMARKMODE, TBBS_SEPARATOR, 80);
	m_wndToolBar.GetToolBarCtrl().GetItemRect(nIndex, &rect);
	rect.left += 5;
	rect.right += 5;
	rect.top = 1; rect.bottom = rect.top + 22;
    if (!m_bookmarkBtn.Create("Bookmark",
        BS_AUTOCHECKBOX|WS_CHILD|WS_VISIBLE, rect, &m_wndToolBar,
        ID_VIEW_BOOKMARKMODE)) 
    { 
        TRACE0("Failed to create CENTER check-box\n");       
        return FALSE;    
    }    
    m_bookmarkBtn.SendMessage(WM_SETFONT, (WPARAM)HFONT(m_font),TRUE);  



	// TODO: Delete these three lines if you don't want the toolbar to be dockable
//	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
//	EnableDocking(CBRS_ALIGN_ANY);
//	DockControlBar(&m_wndToolBar);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	WNDCLASS wndcls;

    // Start with NULL defaults.
    memset(&wndcls, 0, sizeof(WNDCLASS));   
    // Get class information for default window class.
    ::GetClassInfo(AfxGetInstanceHandle(), cs.lpszClass, &wndcls);
    wndcls.lpszClassName = MyMainWndClassName;
	wndcls.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME); 
    // Register new class and return the result code.
	::RegisterClass(&wndcls);
	cs.lpszClass = MyMainWndClassName;

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers




void CMainFrame::OnSizing(UINT fwSide, LPRECT pRect)
{
	CFrameWnd::OnSizing(fwSide, pRect);
	
	switch(fwSide)
	{
	case WMSZ_LEFT:
		if (pRect->right - pRect->left < MIN_WIDTH)
			pRect->left = pRect->right - MIN_WIDTH;
		break;
	case WMSZ_RIGHT:
		if (pRect->right - pRect->left < MIN_WIDTH)
			pRect->right = pRect->left + MIN_WIDTH;
		break;
	case WMSZ_TOP:
		if (pRect->bottom - pRect->top < MIN_HEIGHT)
			pRect->top = pRect->bottom - MIN_HEIGHT;
		break;
	case WMSZ_BOTTOM:
		if (pRect->bottom - pRect->top < MIN_HEIGHT)
			pRect->bottom = pRect->top + MIN_HEIGHT;
		break;
	case WMSZ_TOPLEFT:
		if (pRect->bottom - pRect->top < MIN_HEIGHT)
			pRect->top = pRect->bottom - MIN_HEIGHT;
		if (pRect->right - pRect->left < MIN_WIDTH)
			pRect->left = pRect->right - MIN_WIDTH;
		break;
	case WMSZ_TOPRIGHT:
		if (pRect->bottom - pRect->top < MIN_HEIGHT)
			pRect->top = pRect->bottom - MIN_HEIGHT;
		if (pRect->right - pRect->left < MIN_WIDTH)
			pRect->right = pRect->left + MIN_WIDTH;
		break;
	case WMSZ_BOTTOMLEFT:
		if (pRect->right - pRect->left < MIN_WIDTH)
			pRect->left = pRect->right - MIN_WIDTH;
		if (pRect->bottom - pRect->top < MIN_HEIGHT)
			pRect->bottom = pRect->top + MIN_HEIGHT;
		break;
	case WMSZ_BOTTOMRIGHT:
		if (pRect->right - pRect->left < MIN_WIDTH)
			pRect->right = pRect->left + MIN_WIDTH;
		if (pRect->bottom - pRect->top < MIN_HEIGHT)
			pRect->bottom = pRect->top + MIN_HEIGHT;
		break;

	}
}

void CMainFrame::ActivateFrame(int nCmdShow)
{
	// nCmdShow is the normal show mode this frame should be in
	// translate default nCmdShow (-1)
	if (nCmdShow == -1)
	{
		if (!IsWindowVisible())
			nCmdShow = SW_SHOWNORMAL;
		else if (IsIconic())
			nCmdShow = SW_RESTORE;
	}

	// bring to top before showing
	BringToTop(nCmdShow);

	if (nCmdShow != -1)
	{
		// show the window as specified
	  WINDOWPLACEMENT wp;

	  //  Read and set the previously saved window position and state 
	  if ( !ReadWindowPlacement(&wp) )
	  {
		  ShowWindow(nCmdShow);
	  }
	  else
	  {
		if ( nCmdShow != SW_SHOWNORMAL )  
		  wp.showCmd = nCmdShow;
		  SetWindowPlacement(&wp);
	  }
	  
	  // and finally, bring to top after showing
	  BringToTop(nCmdShow);
	}

    return ;
}

void CMainFrame::OnClose()
{
//	Save window state and position

	WINDOWPLACEMENT	wp;

	// before it is destroyed, save the position of the window
	wp.length = sizeof wp;

	if ( GetWindowPlacement(&wp) )
	{

		if ( IsIconic() )
		  // never restore to Iconic state
		  wp.showCmd = SW_SHOW ;

		if ((wp.flags & WPF_RESTORETOMAXIMIZED) != 0)
		  // if maximized and maybe iconic restore maximized state
		  wp.showCmd = SW_SHOWMAXIMIZED ;

		// and write it to the .INI file
		WriteWindowPlacement(&wp);
	}
	
	CFrameWnd::OnClose();
}

static char szSection[]   = "Settings";
static char szWindowPos[] = "WindowPos";
static char szFormat[] = "%u,%u,%d,%d,%d,%d,%d,%d,%d,%d";


BOOL CMainFrame::ReadWindowPlacement(WINDOWPLACEMENT *pwp)
{
	CString         strBuffer;
	int             nRead ;

	strBuffer = AfxGetApp()->GetProfileString(szSection, szWindowPos);
	if ( strBuffer.IsEmpty() )  
		return FALSE;

	nRead = sscanf(strBuffer, szFormat,
					&pwp->flags, &pwp->showCmd,
					&pwp->ptMinPosition.x, &pwp->ptMinPosition.y,
					&pwp->ptMaxPosition.x, &pwp->ptMaxPosition.y,
					&pwp->rcNormalPosition.left,  &pwp->rcNormalPosition.top,
					&pwp->rcNormalPosition.right, &pwp->rcNormalPosition.bottom);
	if ( nRead != 10 )  
		return FALSE;
	pwp->length = sizeof(WINDOWPLACEMENT);

	return TRUE;
}

void CMainFrame::WriteWindowPlacement(WINDOWPLACEMENT *pwp)
{
	char        szBuffer[sizeof("-32767")*8 + sizeof("65535")*2];
	int         max = 1;
	CString     s;

	sprintf(szBuffer, szFormat,
					pwp->flags, pwp->showCmd,
					pwp->ptMinPosition.x, pwp->ptMinPosition.y,
					pwp->ptMaxPosition.x, pwp->ptMaxPosition.y,
					pwp->rcNormalPosition.left, pwp->rcNormalPosition.top,
					pwp->rcNormalPosition.right, pwp->rcNormalPosition.bottom);
	AfxGetApp()->WriteProfileString(szSection, szWindowPos, szBuffer);
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_LBUTTONDOWN)
    {
        auto db = CDbManager::GetInstance();

        db->SyncO2W();
    }

    return CFrameWnd::PreTranslateMessage(pMsg);
}