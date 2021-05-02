/* ==========================================================================
	File :			RuleRichEditCtrl.cpp

	Class :			CRulerRichEditCtrl

	Author :		Johan Rosengren, Abstrakt Mekanik AB
					Iain Clarke

	Date :			2004-04-17

	Purpose :		"CRulerRichEditCtrl" is a "CWnd" derived class containing an 
					embedded RTF-control, a ruler-control with dragable tab-
					positions and a formatting toolbar. The class can be used 
					to - for example - add a complete mini-editor to a modal 
					or modeless dialog box. 

	Description :	The class mainly handles mouse messages. The mouse
					messages are sent from the ruler control, and are 
					button down, where the a check is made for the cursor 
					located on one of the tab-markers, mouse move, where an 
					XORed line is drawn across the RTF-control and button up, 
					where a new tab position is set. The class also handles 
					the toolbar buttons, setting styles as 
					appropriate for the selected text.

	Usage :			Add a "CRulerRichEditCtrl"-member to the parent class. 
					Call Create to create the control. "GetRichEditCtrl" can 
					be used to access the embedded RTF-control. Remember to 
					call "AfxInitRichEdit(2)"!

					The contents can be saved to disk by calling "Save", and 
					loaded from disk by calling "Load". The two functions 
					will automatically display a file dialog if the file 
					name parameter of the calls are left empty.

					"GetRTF" and "SetRTF" can be used to get and set the 
					contents of the embedded RTF-control as RTF 
					respectively.

					The ruler measures can be displayed as inches or 
					centimeters, by calling "SetMode". "GetMode" will get the 
					current mode.

   ========================================================================*/
 
#include "stdafx.h"
#include "StdGrfx.h"
#include "RulerRichEditCtrl.h"
#include "TextFile/TextFile.h"
#include "LineSpacing.h"
#include "ids.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #define USE_TAB

/////////////////////////////////////////////////////////////////////////////
// Registered messages for ruler/CRulerRichEditCtrl communication

UINT urm_RULERACTION = ::RegisterWindowMessage( _T( "_RULERRICHEDITCTRL_RULER_TRACK_" ) );
UINT urm_GETSCROLLPOS = ::RegisterWindowMessage( _T( "_RULERRICHEDITCTRL_GET_SCROLL_POS_" ) );
UINT urm_SETCURRENTFONTNAME = ::RegisterWindowMessage( _T( "_RULERRICHEDITCTRL_SET_CURRENT_FONT_NAME" ) );
UINT urm_SETCURRENTFONTSIZE = ::RegisterWindowMessage( _T( "_RULERRICHEDITCTRL_SET_CURRENT_FONT_SIZE" ) );
UINT urm_SETCURRENTFONTCOLOR = ::RegisterWindowMessage( _T( "_RULERRICHEDITCTRL_SET_CURRENT_FONT_COLOR" ) );

#define ID_RTF_COPY			500
#define ID_RTF_CUT			501
#define ID_RTF_PASTE		502
#define ID_RTF_SELECT_ALL	503
#define ID_RTF_UNDO			504
#define ID_RTF_LINK			505

/////////////////////////////////////////////////////////////////////////////
// Stream callback functions
// Callbacks to the Save and Load functions.
struct MyMemoryFile
{
	BYTE *		m_buf;
	int			m_bufLen;
	int			m_pos;
	BOOL		m_borrow;

	MyMemoryFile()
	{
		m_bufLen = 100 * 1024;
		m_buf = new BYTE[m_bufLen]; 
		m_borrow = FALSE;
		m_pos = 0;
	}
	MyMemoryFile(BYTE *p, int len)
	{
		m_buf = p; m_bufLen = len; m_borrow = TRUE;
		m_pos = 0;
	}
	~MyMemoryFile()
	{
		if (m_borrow == FALSE && m_buf)
			delete m_buf;
	}
	void SetEnd()
	{
		m_buf[m_pos] = 0;
	}
	int Write(BYTE *p, int len)
	{
		if (!p || len <= 0)
			return 0;

		if (m_pos + len > m_bufLen)
		{
			int		newLen = m_bufLen;

			while(newLen < m_pos + len)
			{
				if (newLen >= 1024 * 1000)
					newLen = newLen * 3;
				else if (newLen >= 1024)
					newLen = newLen * 10;
			}

			BYTE *newBuf = new BYTE[newLen];
			memcpy(newBuf, m_buf, m_pos);
			delete m_buf;
			m_buf = newBuf;
			m_bufLen = newLen;
		}

		memcpy(m_buf + m_pos, p, len);
		m_pos += len;
		return len;
	}

	int Read(BYTE *p, int len)
	{
		if (m_buf == NULL || m_bufLen <= 0 || !p || len <= 0 ||
			m_pos >= m_bufLen)
			return 0;

		if (m_pos + len > m_bufLen)
		{
			len = m_bufLen - m_pos;
		}
		memcpy(p, m_buf + m_pos, len);
		m_pos += len;

		return len;
	}

};

static DWORD CALLBACK StreamOut( DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb )
{
	MyMemoryFile *file = (MyMemoryFile *) dwCookie;
	if (file == NULL)
	{
		*pcb = 0;
		return 0;
	}

	*pcb = file->Write(pbBuff, cb);
	return 0;

}

static DWORD CALLBACK StreamIn( DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb )
{
	MyMemoryFile *file = (MyMemoryFile *) dwCookie;
	if (file == NULL)
	{
		*pcb = 0;
		return 0;
	}

	*pcb = file->Read(pbBuff, cb);

	return 0;

}

#if 0

static DWORD CALLBACK StreamOut( DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb )
{

	// Setting up temp buffer
	char*	buff;
	buff = new char[ cb + 1 ];
	buff[ cb ] = ( char ) 0;
	strncpy( buff, ( LPCSTR ) pbBuff, cb );
	int max = strlen( buff );

	CString* str = ( CString* ) dwCookie;

#ifdef _UNICODE

	// We want to convert the buff to wide chars
	int length = ::MultiByteToWideChar( CP_UTF8, 0, buff, max, NULL, 0 );
	if( length )
	{
		TCHAR* wBuff = new TCHAR[ length ];
		::MultiByteToWideChar( CP_UTF8, 0, buff, max, wBuff, length );

		*str += wBuff;
		delete[] wBuff;
	}

#else

	*str += buff;

#endif

	delete[] buff;
	*pcb = max;
	
	return 0;

}

static DWORD CALLBACK StreamIn( DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb )
{

	CString* str = ( ( CString* ) dwCookie );

#ifdef _UNICODE

	// Unicode is only supported for SF_TEXT, so we need
	// to convert
	LPCTSTR ptr = str->GetBuffer( (*str).GetLength() );
	int length = ::WideCharToMultiByte( CP_UTF8, 0, ptr, -1, NULL, 0, NULL, NULL );
	int max = min( cb, length );
	if( length )
	{
		char* buff = new char[ length ];
		::WideCharToMultiByte( CP_UTF8, 0, ptr, -1, buff, length + 1, NULL, NULL );
		strncpy( (LPSTR) pbBuff, buff, max );
		delete[] buff;
	}
	str->ReleaseBuffer();

#else

	int max = min( cb, (*str).GetLength() );
	strncpy( ( LPSTR ) pbBuff, (*str) , max  );

#endif

	(*str) = (*str).Right( (*str).GetLength() - max );

	*pcb = max;

	return 0;

}

#endif

/////////////////////////////////////////////////////////////////////////////
// CRulerRichEditCtrl

CRulerRichEditCtrl::CRulerRichEditCtrl() : m_pen( PS_DOT, 0, RGB( 0, 0, 0 ) )
/* ============================================================
	Function :		CRulerRichEditCtrl::CRulerRichEditCtrl
	Description :	constructor
	Access :		Public
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	m_rulerPosition = 0;
	m_margin = 0;
	m_movingtab = -1;
	m_offset = 0;
	m_readOnly = FALSE;
	m_richEditModule = LoadLibrary (_T("Riched20.dll"));
	ShowToolbar();
	ShowRuler();

}

CRulerRichEditCtrl::~CRulerRichEditCtrl()
/* ============================================================
	Function :		CRulerRichEditCtrl::~CRulerRichEditCtrl
	Description :	destructor
	Access :		Public
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	if( m_richEditModule )
		FreeLibrary ( m_richEditModule );

	m_pen.DeleteObject();

}


BOOL CRulerRichEditCtrl::Create( DWORD dwStyle, const RECT &rect, CWnd* pParentWnd, UINT nID, 
								CString font, int size, BOOL multiLine, BOOL autohscroll )
/* ============================================================
	Function :		CRulerRichEditCtrl::Create
	Description :	Creates the control and sub controls.
	Access :		Public
					
	Return :		BOOL				-	"TRUE" if created OK.
	Parameters :	DWORD dwStyle		-	Style of the control, 
											normally "WS_CHILD" 
											and "WS_VISIBLE".
					const RECT &rect	-	Placement rectangle.
					CWnd* pParentWnd	-	Parent window.
					UINT nID			-	Control ID
					BOOL autohscroll	-	"TRUE" if the RTF-control
											should have the 
											"ES_AUTOHSCROLL" style
											set.

	Usage :			Call to create the control.

   ============================================================*/
{

	BOOL result = CWnd::Create( NULL, _T( "" ), dwStyle, rect, pParentWnd, nID );
	if ( result )
	{

		result = FALSE;
		// Save screen resolution for
		// later on.
		CClientDC dc( this );
		m_physicalInch = dc.GetDeviceCaps( LOGPIXELSX );

		// Create sub-controls
		if( CreateRTFControl( font, size, multiLine, autohscroll ) )
		{
			CreateMargins();
			if( CreateToolbar() )
			{
				if( CreateRuler() )
				{

					UpdateToolbarButtons();
					result = TRUE;
				}
			}
		}		
	}

	return result;

}

BOOL CRulerRichEditCtrl::CreateToolbar()
/* ============================================================
	Function :		CRulerRichEditCtrl::CreateToolbar
	Description :	Creates the toolbar control
	Access :		Private

	Return :		BOOL	-	"TRUE" if the toolbar was created ok.
	Parameters :	none

	Usage :			Called during control creation

   ============================================================*/
{

	CRect rect;
	GetClientRect( rect );

	CRect toolbarRect( 0, 0, rect.right, TOOLBAR_HEIGHT );
	return m_toolbar.Create( this, toolbarRect );

}

BOOL CRulerRichEditCtrl::CreateRuler()
/* ============================================================
	Function :		CRulerRichEditCtrl::CreateRuler
	Description :	Creates the ruler control
	Access :		Private

	Return :		BOOL	-	"TRUE" if created ok.
	Parameters :	none

	Usage :			Called during control creation

   ============================================================*/
{

	CRect rect;
	GetClientRect( rect );

	CRect rulerRect( 0, TOOLBAR_HEIGHT, rect.right, TOOLBAR_HEIGHT + RULER_HEIGHT );
	return m_ruler.Create( rulerRect, this, RULER_CONTROL );

}

BOOL CRulerRichEditCtrl::CreateRTFControl(CString &font, int size, BOOL multiLine, BOOL autohscroll )
/* ============================================================
	Function :		CRulerRichEditCtrl::CreateRTFControl
	Description :	Creates the embedded RTF-control.
	Access :		Private
					
	Return :		BOOL				-	"TRUE" if created ok.
	Parameters :	BOOL autohscroll	-	"TRUE" if the RTF-control
											should have the
											"ES_AUTOHSCROLL" style
											set.

	Usage :			Called during control creation

   ============================================================*/
{

	BOOL result = FALSE;

	CRect rect;
	GetClientRect( rect );

	int top = TOOLBAR_HEIGHT + RULER_HEIGHT;
	CRect rtfRect( 0, top, rect.right, rect.bottom );
	DWORD style = WS_TABSTOP | ES_NOHIDESEL | WS_CHILD | WS_VISIBLE | 
		WS_BORDER | WS_HSCROLL | WS_VSCROLL | ES_WANTRETURN;
	
	if (multiLine)
		style |= ES_MULTILINE;

	if( autohscroll )
		style |= ES_AUTOHSCROLL;

	if( m_rtf.Create( style, rtfRect, this ) )
	{
#ifdef USE_TAB
		// Setting up default tab stops
		ParaFormat para( PFM_TABSTOPS );
		para.cTabCount = MAX_TAB_STOPS;
		for( int t = 0; t < MAX_TAB_STOPS ; t++ )
			para.rgxTabs[ t ] = 640 * ( t + 1 );

		m_rtf.SetParaFormat( para );
#endif
		// Setting default character format
		CharFormat	cf;
		cf.dwMask = CFM_SIZE | CFM_FACE | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE;
		cf.yHeight = size * 20;
		cf.dwEffects = 0;
		lstrcpy( cf.szFaceName, font );
		m_rtf.SendMessage(EM_SETCHARFORMAT, 0, (LPARAM)&cf);

#ifdef USE_TAB
		// Set the internal tabs array
		SetTabStops( ( LPLONG ) ( para.rgxTabs ), MAX_TAB_STOPS );
#endif
 	    m_rtf.SendMessage( EM_AUTOURLDETECT, TRUE, 0);  

		m_rtf.SetEventMask( m_rtf.GetEventMask() | ENM_SELCHANGE | ENM_SCROLL | ENM_CHANGE | ENM_LINK |ENM_MOUSEEVENTS );
		SetReadOnly( GetReadOnly() ); 

	//	m_rtf.SetEventMask(ENM_MOUSEEVENTS);
		m_rtf.SetSel(0,0);

		m_rtf.SetTargetDevice(NULL, 0);
		result = TRUE;
	}

	return result;

}

void CRulerRichEditCtrl::CreateMargins()
/* ============================================================
	Function :		CRulerRichEditCtrl::CreateMargins
	Description :	Sets the margins for the subcontrols and 
					the RTF-control edit rect.
	Access :		Private

	Return :		void
	Parameters :	none

	Usage :			Called during control creation.

   ============================================================*/
{

	// Set up edit rect margins
	int scmargin = 4;
	CRect rc;
	m_rtf.GetClientRect( rc );

	rc.top = scmargin;
	rc.left = scmargin * 2;
	rc.right -= scmargin * 2;

	m_rtf.SetRect( rc );

	// Get the diff between the window- and client 
	// rect of the RTF-control. This gives the actual 
	// size of the RTF-control border.
	CRect	r1;
	CRect	r2;

	m_rtf.GetWindowRect( r1 );
	m_rtf.GetClientRect( r2 );
	m_rtf.ClientToScreen( r2 );

	// Create the margin for the toolbar 
	// controls and the ruler.
	m_margin = scmargin * 2 + r2.left - r1.left;
	m_ruler.SetMargin( m_margin );

}

BEGIN_MESSAGE_MAP(CRulerRichEditCtrl, CWnd)
	//{{AFX_MSG_MAP(CRulerRichEditCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_MESSAGE( WM_SETTEXT, OnSetText )
	ON_MESSAGE( WM_GETTEXT, OnGetText  )
	ON_MESSAGE( WM_GETTEXTLENGTH, OnGetTextLength )
	ON_BN_CLICKED(BUTTON_FONT, OnButtonFont)
	ON_BN_CLICKED(BUTTON_COLOR, OnButtonColor)
	ON_BN_CLICKED(BUTTON_BOLD, OnButtonBold)
	ON_BN_CLICKED(BUTTON_ITALIC, OnButtonItalic)
	ON_BN_CLICKED(BUTTON_UNDERLINE, OnButtonUnderline)
	ON_BN_CLICKED(BUTTON_LEFTALIGN, OnButtonLeftAlign)
	ON_BN_CLICKED(BUTTON_CENTERALIGN, OnButtonCenterAlign)
	ON_BN_CLICKED(BUTTON_RIGHTALIGN, OnButtonRightAlign)
	ON_BN_CLICKED(BUTTON_INDENT, OnButtonIndent)
	ON_BN_CLICKED(BUTTON_OUTDENT, OnButtonOutdent)
	ON_BN_CLICKED(BUTTON_BULLET, OnButtonBullet)
	ON_BN_CLICKED(BUTTON_LINE, OnButtonLine)
	ON_WM_SETFOCUS()
	ON_REGISTERED_MESSAGE(urm_RULERACTION, OnTrackRuler)
	ON_REGISTERED_MESSAGE(urm_GETSCROLLPOS, OnGetScrollPos)
	ON_REGISTERED_MESSAGE(urm_SETCURRENTFONTNAME, OnSetCurrentFontName)
	ON_REGISTERED_MESSAGE(urm_SETCURRENTFONTSIZE, OnSetCurrentFontSize)
	ON_REGISTERED_MESSAGE(urm_SETCURRENTFONTCOLOR, OnSetCurrentFontColor)
	ON_MESSAGE(ID_RTF_COPY, OnCopy) 
	ON_MESSAGE(ID_RTF_CUT, OnCut)
	ON_MESSAGE(ID_RTF_PASTE, OnPaste)
	ON_MESSAGE(ID_RTF_SELECT_ALL, OnSelectAll)
	ON_MESSAGE(ID_RTF_UNDO, OnUndo)
	ON_MESSAGE(ID_RTF_LINK, OnLink)
	//}}AFX_MSG_MAP
    ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRulerRichEditCtrl message handlers

void CRulerRichEditCtrl::OnPaint() 
/* ============================================================
	Function :		CRulerRichEditCtrl::OnPaint
	Description :	Paints the ruler.
	Access :		Protected
					
	Return :		void
	Parameters :	none

	Usage :			Called from MFC. 

   ============================================================*/
{

	CPaintDC mainDC(this);
	UpdateTabStops();

}

BOOL CRulerRichEditCtrl::OnEraseBkgnd( CDC* /*pDC*/ ) 
/* ============================================================
	Function :		CRulerRichEditCtrl::OnEraseBkgnd
	Description :	Returns "TRUE" to avoid flicker.
	Access :		Protected
					
	Return :		BOOL		-	Always "TRUE",
	Parameters :	CDC* pDC	-	Not used
					
	Usage :			Called from MFC. 

   ============================================================*/
{
	
	return TRUE;

}

BOOL CRulerRichEditCtrl::OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult ) 
/* ============================================================
	Function :		CRulerRichEditCtrl::OnNotify
	Description :	Called as the RTF-control is updated or 
					the selection changes.
	Access :		Protected
					
	Return :		BOOL				-	From base class
	Parameters :	WPARAM wParam		-	Control ID
					LPARAM lParam		-	Not interested
					LRESULT* pResult	-	Not interested
					
	Usage :			Called from MFC. We must check the control 
					every time the selection changes or the 
					contents are changed, as the cursor might 
					have entered a new paragraph, with new tab 
					and/or font settings.

   ============================================================*/
{
    LPNMHDR a = (LPNMHDR)lParam;

    if (((LPNMHDR)lParam)->code == EN_LINK)
    {
        m_isLink = TRUE;
        ENLINK * enLinkInfo = (ENLINK *)lParam;
        
        if (enLinkInfo->msg == WM_LBUTTONUP)
        {
            CString s;

            GetText(s, enLinkInfo->chrg.cpMin, enLinkInfo->chrg.cpMax);

            this->GetParent()->SendMessage(WM_LINK_CLICK, wParam, (LPARAM) &s);
            return CWnd::OnNotify( wParam, lParam, pResult );
        }
    }

	if( wParam == RTF_CONTROL )
	{
        m_isLink = false;

		// Update the toolbar
		UpdateToolbarButtons();

		// Update ruler
		CRect rect;
		GetClientRect( rect );
		rect.top = TOOLBAR_HEIGHT;
		rect.bottom = rect.top + RULER_HEIGHT;

	//	RedrawWindow( rect );

		MSGFILTER * lpMsgFilter = (MSGFILTER *)lParam; 

		if ((lpMsgFilter->nmhdr.code == EN_MSGFILTER) && (lpMsgFilter->msg == WM_RBUTTONDOWN))                                       
		{
			//if we get through here, we have trapped the right click event of the richeditctrl! 
			CPoint point;	                                        
  			::GetCursorPos(&point); //where is the mouse?

            CRect r;
            CPoint point2;
            m_rtf.GetWindowRect(r);
            point2.x = point.x - r.left;
            point2.y = point.y - r.top;

            int     n = m_rtf.CharFromPos(point2);
            
            long start, end;
            m_rtf.GetSel(start, end);
            if (n < start || n > end)
                m_rtf.SetSel(n, n);
            
            CMenu menu; //lets display out context menu :) 
 			UINT dwSelectionMade;                                       
			VERIFY(menu.CreatePopupMenu());
			if (m_rtf.CanUndo())
				menu.AppendMenu(MF_STRING, ID_RTF_UNDO, "Undo");
			else
				menu.AppendMenu(MF_STRING | MF_GRAYED, ID_RTF_UNDO, "Undo");

			menu.AppendMenu(MF_SEPARATOR, 504, " ");
			if (m_rtf.GetSelectionType() != SEL_EMPTY)
			{
				menu.AppendMenu(MF_STRING, ID_RTF_CUT, "Cut");
				menu.AppendMenu(MF_STRING, ID_RTF_COPY, "Copy");
			}
			else
			{
				menu.AppendMenu(MF_STRING | MF_GRAYED, ID_RTF_CUT, "Cut");
				menu.AppendMenu(MF_STRING | MF_GRAYED, ID_RTF_COPY, "Copy");
			}

			if (m_rtf.CanPaste())
				menu.AppendMenu(MF_STRING, ID_RTF_PASTE, "Paste");
			else
				menu.AppendMenu(MF_STRING | MF_GRAYED, ID_RTF_PASTE, "Paste");

			menu.AppendMenu(MF_SEPARATOR, 504, " ");
			menu.AppendMenu(MF_STRING, ID_RTF_SELECT_ALL, "Select All");

			menu.AppendMenu(MF_SEPARATOR, 504, " ");

            CharFormat	cf;
		    cf.dwMask = CFM_LINK;
		    m_rtf.SendMessage( EM_GETCHARFORMAT, SCF_SELECTION, ( LPARAM ) &cf );
            if (cf.dwEffects & CFE_LINK)  
                menu.AppendMenu(MF_STRING | MF_GRAYED, ID_RTF_LINK, "Link");
            else
			    menu.AppendMenu(MF_STRING, ID_RTF_LINK, "Link");

            dwSelectionMade = menu.TrackPopupMenu( (TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_NONOTIFY|TPM_RETURNCMD),
  									point.x, point.y, this
        								);                                
	  
 			menu.DestroyMenu(); 
			PostMessage(dwSelectionMade,0,0);
 		}
	}
	
	return CWnd::OnNotify( wParam, lParam, pResult );

}

void CRulerRichEditCtrl::OnSize( UINT nType, int cx, int cy ) 
/* ============================================================
	Function :		CRulerRichEditCtrl::OnSize
	Description :	We resize the embedded RTF-control.
	Access :		Protected
					
	Return :		void
	Parameters :	UINT nType	-	Not interested
					int cx		-	New width
					int cy		-	New height
					
	Usage :			Called from MFC. 

   ============================================================*/
{

	CWnd::OnSize( nType, cx, cy );
	
	if( m_rtf.m_hWnd )
	{

		LayoutControls( cx, cy );

	}
	
}

void CRulerRichEditCtrl::OnSetFocus( CWnd* pOldWnd ) 
/* ============================================================
	Function :		CRulerRichEditCtrl::OnSetFocus
	Description :	We handle over the focus to the embedded 
					RTF-control.
	Access :		Protected
					
	Return :		void
	Parameters :	CWnd* pOldWnd	-	Not used
					
	Usage :			Called from MFC. 

   ============================================================*/
{

	CWnd::OnSetFocus( pOldWnd );
	m_rtf.SetFocus();
	
}

LRESULT CRulerRichEditCtrl::OnGetScrollPos(WPARAM, LPARAM)
/* ============================================================
	Function :		CRulerRichEditCtrl::OnGetScrollPos
	Description :	The function handles the registered message 
					"urm_GETSCROLLPOS", that is sent from the 
					ruler to get the current scroll position 
					of the embedded RTF-control.
	Access :		Protected

	Return :		LRESULT		-	Current scroll pos
	Parameters :	WPARAM mode	-	Not used
					LPARAM pt	-	Not used
					
	Usage :			Called from MFC

   ============================================================*/
{

	return m_rtf.GetScrollPos( SB_HORZ );

}

LRESULT CRulerRichEditCtrl::OnTrackRuler(WPARAM mode, LPARAM pt)
/* ============================================================
	Function :		CRulerRichEditCtrl::OnTrackRuler
	Description :	The function handles the registered message 
					"urm_RULERACTION", that is sent from the 
					mouse handling mappings in the ruler control.
					The function handles dragging of tabulator 
					points in the ruler.
	Access :		Protected
					
	Return :		LRESULT		-	Not used
	Parameters :	WPARAM mode	-	The type of mouse operation, 
									"DOWN", "MOVE" or "UP"
					LPARAM pt	-	Cursor point for the cursor.
					
	Usage :			Called from MFC.

   ============================================================*/
{

	CPoint* point = ( CPoint* ) pt;
	int toolbarHeight = 0;
	if( m_showToolbar )
		toolbarHeight = TOOLBAR_HEIGHT;

	switch( mode )
	{
		case DOWN:
			// The left mouse button is clicked
			{
				// Check if we clicked on a tab-marker.
				int pos = m_rtf.GetScrollPos( SB_HORZ );
				m_movingtab = -1;
				CRect hitRect;
				int y = RULER_HEIGHT - 9;
				for( int t = 0 ; t < MAX_TAB_STOPS ; t++ )
				{
					int x = m_tabs[ t ] + m_margin - pos;
					hitRect.SetRect( x - 2, y - 1, x + 3, y + 3 );
					if( hitRect.PtInRect( *point ) )
					{
						// Yes, we did.
						m_movingtab = t;

						// Calc offset, as the hit area is wider than
						// the 1 pixel tab line
						m_offset = point->x - ( m_tabs[ t ] + m_margin - pos );
					}
				}

				if( m_movingtab != -1 )
				{

					// We did click in a tab marker.
					// Start dragging

					// Find initial ruler position
					m_rulerPosition = point->x - m_offset;
					CRect rect;
					GetClientRect( rect );

					// Draw a new ruler line
					CClientDC dc( this );
					dc.SelectObject( &m_pen );
					dc.SetROP2( R2_XORPEN );

					dc.MoveTo( m_rulerPosition, toolbarHeight + 3 );
					dc.LineTo( m_rulerPosition, rect.Height() );

					dc.SelectStockObject( BLACK_PEN );

				}
			}
			break;

		case MOVE:
			// The mouse is moved
			{
				if( m_movingtab != -1 )
				{
					CRect rect;
					GetClientRect( rect );
					CClientDC dc( this );

					// Erase previous line
					dc.SelectObject( &m_pen );
					dc.SetROP2( R2_XORPEN );

					dc.MoveTo( m_rulerPosition, toolbarHeight + 3 );
					dc.LineTo( m_rulerPosition, rect.Height() );

					// Set up new line
					// Calc min and max. We can not place this marker 
					// before the previous or after the next. Neither 
					// can we move the marker outside the ruler.
					int pos = m_rtf.GetScrollPos( SB_HORZ );
					int min = m_margin + m_offset;
					if( m_movingtab > 0 )
						min = ( m_tabs[ m_movingtab - 1 ] + m_margin - pos ) + 3 + m_offset;

					int max = rect.Width() - 5 + m_offset;
					if( m_movingtab < m_tabs.GetUpperBound() )
						max = ( m_tabs[ m_movingtab + 1 ] + m_margin - pos ) - 3 + m_offset;
					max = min( max, rect.Width() - 5 + m_offset );

					// Set new positions
					m_rulerPosition = max( min, point->x - m_offset );
					m_rulerPosition = min( m_rulerPosition, max );

					// Draw the new line
					dc.MoveTo( m_rulerPosition, toolbarHeight + 3 );
					dc.LineTo( m_rulerPosition, rect.Height() );

					dc.SelectStockObject( BLACK_PEN );

				}
			}
			break;

		case UP:
			// The mouse button is released
			{
				if( m_movingtab != -1 )
				{

					// Set new value for tab position
					int pos = m_rtf.GetScrollPos( SB_HORZ );
					m_tabs[ m_movingtab ] = m_rulerPosition - m_margin + pos - m_offset;

					// Get the current tabstops, as we
					// must set all tabs in one operation
					ParaFormat para( PFM_TABSTOPS );
					para.cTabCount = MAX_TAB_STOPS;
					m_rtf.GetParaFormat( para );

					// Convert current position to twips
					double twip = ( double )m_physicalInch / 1440;
					int tabpos = m_tabs[ m_movingtab ];
					tabpos = ( int ) ( ( double ) tabpos / twip +.5 );
					para.rgxTabs[ m_movingtab ] = tabpos; 

					// Set tabs to control
					m_rtf.SetParaFormat( para );

					// Erase the ruler
					m_ruler.RedrawWindow();
					m_rtf.RedrawWindow();

					m_movingtab = -1;
					m_rtf.SetFocus();

				}
			}
			break;
	}

	return 0;

}

LRESULT CRulerRichEditCtrl::OnSetText( WPARAM wParam, LPARAM lParam )
/* ============================================================
	Function :		CRulerRichEditCtrl::OnSetText
	Description :	The function handles the "WM_SETTEXT" 
					message. The handler sets the text in the 
					RTF-control
	Access :		Protected
										
	Return :		LRESULT			-	From the control
	Parameters :	WPARAM wParam	-	Passed on
					LPARAM lParam	-	Passed on
					
	Usage :			Called from MFC.

   ============================================================*/
{
	
	return m_rtf.SendMessage( WM_SETTEXT, wParam, lParam );

}

LRESULT CRulerRichEditCtrl::OnGetText( WPARAM wParam, LPARAM lParam )
/* ============================================================
	Function :		CRulerRichEditCtrl::OnGetText
	Description :	The function handles the "WM_GETTEXT" 
					message. The handler gets the text from the 
					RTF-control
	Access :		Protected
										
	Return :		LRESULT			-	From the control
	Parameters :	WPARAM wParam	-	Passed on
					LPARAM lParam	-	Passed on
					
	Usage :			Called from MFC.

   ============================================================*/
{

	return m_rtf.SendMessage( WM_GETTEXT, wParam, lParam );

}

LRESULT CRulerRichEditCtrl::OnGetTextLength( WPARAM wParam, LPARAM lParam )
/* ============================================================
	Function :		CRulerRichEditCtrl::OnGetTextLength
	Description :	The function handles the "WM_GETTEXTLENGTH" 
					message. The handler gets the length of 
					the text in the RTF-control
	Access :		Protected
										
	Return :		LRESULT			-	From the control
	Parameters :	WPARAM wParam	-	Passed on
					LPARAM lParam	-	Passed on
					
	Usage :			Called from MFC.

   ============================================================*/
{

	return m_rtf.GetTextLength();

}

/////////////////////////////////////////////////////////////////////////////
// CRulerRichEditCtrl public implementation

void CRulerRichEditCtrl::GetRTF(CString &str)
/* ============================================================
	Function :		CRulerRichEditCtrl::GetRTF
	Description :	Returns the contents of the control as RTF.
	Access :		Public
					
	Return :		CString	-	The RTF-contents of the control.
	Parameters :	none

	Usage :			Call this function to get a char buffer 
					with the contents of the embedded RTF-
					control.

   ============================================================*/
{
#if 0
	CString* str = new CString;
	EDITSTREAM	es;
	es.dwCookie = ( DWORD ) str;
	es.pfnCallback = StreamOut;
	m_rtf.StreamOut( SF_RTF, es );

	CString output( *str );
	delete str;
#endif
	MyMemoryFile *	file = new MyMemoryFile();

	EDITSTREAM	es;
	es.dwCookie = ( DWORD ) file;
	es.pfnCallback = StreamOut;
	m_rtf.StreamOut( SF_RTF, es );
	file->m_buf[file->m_pos] = 0;
	str = file->m_buf;
	delete file;
}

void CRulerRichEditCtrl::SetRTF( const CString& rtf )
/* ============================================================
	Function :		CRulerRichEditCtrl::SetRTF
	Description :	Set the contents of the embedded RTF-
					control from rtf.
	Access :		Public
					
	Return :		void
	Parameters :	const CString& rtf	-	The rtf-contents to 
											set.
					
	Usage :			Call this function to set the RTF-contents 
					of the control.

   ============================================================*/
{
#if 0
	CString* str = new CString( rtf );

	EDITSTREAM	es;
	es.dwCookie = ( DWORD ) str;
	es.pfnCallback = StreamIn;
	m_rtf.StreamIn( SF_RTF, es );

	delete str;
#endif
	MyMemoryFile *	file = new MyMemoryFile((BYTE *)(LPCTSTR) rtf, rtf.GetLength());

	EDITSTREAM	es;
	es.dwCookie = ( DWORD ) file;
	es.pfnCallback = StreamIn;
	m_rtf.StreamIn( SF_RTF, es );
	delete file;
}

BOOL CRulerRichEditCtrl::Save( CString& filename )
/* ============================================================
	Function :		CRulerRichEditCtrl::Save
	Description :	Saves the contents to the file filename. 
					If filename is empty, a file dialog will 
					be displayed and the selected name will be 
					returned in the "CString".
	Access :		Public
					
	Return :		BOOL				-	"TRUE" if the file 
											was saved.
	Parameters :	CString& filename	-	The file name to save 
											to. Can be empty.
					
	Usage :			Call to save the contents of the embedded 
					RTF-control do a file.

   ============================================================*/
{

	BOOL result = TRUE;

	CString* str = new CString;

	EDITSTREAM	es;
	es.dwCookie = ( DWORD ) str;
	es.pfnCallback = StreamOut;
	m_rtf.StreamOut( SF_RTF, es );

	CTextFile f( _T( "rtf" ) );
	result = f.WriteTextFile( filename, *str );

	delete str;
	return result;

}

BOOL CRulerRichEditCtrl::Load( CString& filename )
/* ============================================================
	Function :		CRulerRichEditCtrl::Load
	Description :	Loads the embedded RTF-control with the 
					contents from the file filename. 
					If filename is empty, a file dialog will 
					be displayed and the selected name will be 
					returned in the "CString".
	Access :		Public
					
	Return :		BOOL				-	"TRUE" if the file 
											was loaded.
	Parameters :	CString& filename	-	File name to load 
											from. Can be empty.
					
	Usage :			Call to load an RTF-file to the control.

   ============================================================*/
{

	BOOL result = TRUE;

	CString* str = new CString;
	CTextFile f( _T( "rtf" ) );
	result = f.ReadTextFile( filename, *str );
	if( result )
	{
		EDITSTREAM	es;
		es.dwCookie = ( DWORD ) str;
		es.pfnCallback = StreamIn;
		m_rtf.StreamIn( SF_RTF, es );
	}

	delete str;
	return result;

}

void CRulerRichEditCtrl::SetMode( int mode )
/* ============================================================
	Function :		CRulerRichEditCtrl::SetMode
	Description :	Sets the internal mode, that is, if the 
					ruler should display inches or centimeters.
	Access :		Public
					
	Return :		void
	Parameters :	int mode	-	Mode to use, "MODE_INCH" or 
									"MODE_METRIC" (default)
					
	Usage :			Call to change the mode.

   ============================================================*/
{

	m_ruler.SetMode( mode );

}

int CRulerRichEditCtrl::GetMode() const
/* ============================================================
	Function :		CRulerRichEditCtrl::GetMode
	Description :	Gets the mode, that is, either "MODE_INCH" or 
					"MODE_METRIC", that is used to draw the ruler.
	Access :		Public
					
	Return :		int		-	The mode, either "MODE_INCH" or 
								"MODE_METRIC"
	Parameters :	none

	Usage :			Call to get the current mode.

   ============================================================*/
{

	return m_ruler.GetMode();

}

CRichEditCtrl& CRulerRichEditCtrl::GetRichEditCtrl()
/* ============================================================
	Function :		CRulerRichEditCtrl::GetRichEditCtrl
	Description :	Returns an alias to the embedded RTF-control.
	Access :		Public
					
	Return :		CRichEditCtrl&	-	An alias to the rtf-control
	Parameters :	none

	Usage :			Call to access the RTF-control directly.

   ============================================================*/
{

	return m_rtf;

}

/////////////////////////////////////////////////////////////////////////////
// CRulerRichEditCtrl toolbar button handlers

void CRulerRichEditCtrl::OnButtonFont() 
/* ============================================================
	Function :		CRulerRichEditCtrl::OnButtonFont
	Description :	Button handler for the Font button
	Access :		Protected
					
	Return :		void
	Parameters :	none

	Usage :			Called from MFC

   ============================================================*/
{

	DoFont();


}

void CRulerRichEditCtrl::OnButtonColor() 
/* ============================================================
	Function :		CRulerRichEditCtrl::OnButtonColor
	Description :	Button handler for the Color button
	Access :		Protected
					
	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	DoColor();

}

void CRulerRichEditCtrl::OnButtonBold() 
/* ============================================================
	Function :		CRulerRichEditCtrl::OnButtonBold
	Description :	Button handler for the Bold button
	Access :		Protected
					
	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	DoBold();

}

void CRulerRichEditCtrl::OnButtonItalic() 
/* ============================================================
	Function :		CRulerRichEditCtrl::OnButtonItalic
	Description :	Button handler for the Italic button
	Access :		Protected
					
	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	DoItalic();

}

void CRulerRichEditCtrl::OnButtonUnderline() 
/* ============================================================
	Function :		CRulerRichEditCtrl::OnButtonUnderline
	Description :	Button handler for the Underline button
	Access :		Protected
					
	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	DoUnderline();

}

void CRulerRichEditCtrl::OnButtonLeftAlign() 
/* ============================================================
	Function :		CRulerRichEditCtrl::OnButtonLeftAlign
	Description :	Button handler for the Left aligned button
	Access :		Protected
					
	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	DoLeftAlign();

}

void CRulerRichEditCtrl::OnButtonCenterAlign() 
/* ============================================================
	Function :		CRulerRichEditCtrl::OnButtonCenterAlign
	Description :	Button handler for the Center button
	Access :		Protected
					
	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	DoCenterAlign();

}

void CRulerRichEditCtrl::OnButtonRightAlign() 
/* ============================================================
	Function :		CRulerRichEditCtrl::OnButtonRightAlign
	Description :	Button handler for the Right-aligned button
	Access :		Protected
					
	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	DoRightAlign();

}

void CRulerRichEditCtrl::OnButtonIndent() 
/* ============================================================
	Function :		CRulerRichEditCtrl::OnButtonIndent
	Description :	Button handler for the Indent button
	Access :		Protected
					
	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	DoIndent();

}

void CRulerRichEditCtrl::OnButtonOutdent() 
/* ============================================================
	Function :		CRulerRichEditCtrl::OnButtonOutdent
	Description :	Button handler for the outdent button
	Access :		Protected
					
	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	DoOutdent();

}

void CRulerRichEditCtrl::OnButtonBullet() 
/* ============================================================
	Function :		CRulerRichEditCtrl::OnButtonBullet
	Description :	Button handler for the Bullet button
	Access :		Protected
					
	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	DoBullet();

}

void CRulerRichEditCtrl::GetText(CString &s, int start, int end)
{
	//Set the size of the character buffers, + 1 for null character
	int nLength = int(end - start + 1);

	//create an ANSI buffer and a Unicode (Wide Character) buffer
	LPTSTR lpsz = s.GetBufferSetLength(nLength * 2);
	lpsz[0] = NULL;

	LPWSTR lpszWChar = new WCHAR[nLength];

	TEXTRANGEW range;
	//Set the trRE50W LPWSTR character buffer = Unicode buffer
	range.chrg.cpMin = start;
	range.chrg.cpMax = end;
	range.lpstrText = lpszWChar;

	//Get the Unicode text
	m_rtf.SendMessage(EM_GETTEXTRANGE, 0,  (LPARAM) &range);  

	// Convert the Unicode RTF text to ANSI.
	WideCharToMultiByte(CP_ACP, 0, lpszWChar, -1, lpsz, nLength, NULL, NULL);

	//Release buffer memory
	delete lpszWChar;

	return;
}


/////////////////////////////////////////////////////////////////////////////
// CRulerRichEditCtrl private helpers

void CRulerRichEditCtrl::SetTabStops( LPLONG tabs, int size )
/* ============================================================
	Function :		CRulerRichEditCtrl::SetTabStops
	Description :	Set the tab stops in the internal tab stop 
					list from the RTF-control, converting the 
					twip values to physical pixels.
	Access :		Private
					
	Return :		void
	Parameters :	LPLONG tabs	-	A pointer to an array of 
									"LONG" twip values.
					int size	-	The size of "tabs"
					
	Usage :			Call to set the tab list.

   ============================================================*/
{
#ifdef USE_TAB
	m_tabs.RemoveAll();

	double twip = ( double )m_physicalInch / 1440;
	for( int t = 0 ; t < size ; t++ )
	{
		// Convert from twips to pixels
		int tabpos = *( tabs + t );
		tabpos = ( int ) ( ( double ) tabpos * twip +.5 );
		m_tabs.Add( tabpos );

	}

	m_ruler.SetTabStops( m_tabs );
#endif
}

void CRulerRichEditCtrl::UpdateTabStops()
/* ============================================================
	Function :		CRulerRichEditCtrl::UpdateTabStops
	Description :	Sets the tabs in the internal tab stop 
					list, converting the twip physical (pixel) 
					position to twip values.
	Access :		Private
					
	Return :		void
	Parameters :	none

	Usage :			Call to refresh the tab list from the RTF-
					control. Called from the "OnPaint" handler.

   ============================================================*/
{
#ifdef USE_TAB
	ParaFormat para( PFM_TABSTOPS );
	m_rtf.GetParaFormat( para );
	SetTabStops( (LPLONG)(para.rgxTabs), MAX_TAB_STOPS );
#endif
}

void CRulerRichEditCtrl::UpdateToolbarButtons()
/* ============================================================
	Function :		CRulerRichEditCtrl::UpdateToolbarButtons
	Description :	Updates the toolbar button, by getting 
					formatting information from the currently 
					selected text in the embedded RTF-control.
	Access :		Private
					
	Return :		void
	Parameters :	none

	Usage :			Call as the selection changes in the 
					RTF-control

   ============================================================*/
{
	if( m_showToolbar && m_toolbar.m_hWnd )
	{
		CharFormat	cf;
		cf.dwMask = CFM_LINK | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE;
		m_rtf.SendMessage( EM_GETCHARFORMAT, SCF_SELECTION, ( LPARAM ) &cf );

		ParaFormat para( PFM_ALIGNMENT | PFM_NUMBERING );
		m_rtf.GetParaFormat( para );

		// Style
		m_toolbar.SetState( BUTTON_BOLD, TBSTATE_ENABLED | ( ( cf.dwEffects & CFE_BOLD )  ? TBSTATE_CHECKED : 0 ) );
		m_toolbar.SetState( BUTTON_ITALIC, TBSTATE_ENABLED | ( ( cf.dwEffects & CFE_ITALIC )  ? TBSTATE_CHECKED : 0 ) );
		m_toolbar.SetState( BUTTON_UNDERLINE, TBSTATE_ENABLED | ( ( cf.dwEffects & CFM_UNDERLINE )  ? TBSTATE_CHECKED : 0 ) );

		m_toolbar.SetState( BUTTON_LEFTALIGN, TBSTATE_ENABLED | ( para.wAlignment == PFA_LEFT ? TBSTATE_CHECKED : 0 ) );
		m_toolbar.SetState( BUTTON_CENTERALIGN, TBSTATE_ENABLED | ( para.wAlignment == PFA_CENTER ? TBSTATE_CHECKED : 0 ) );
		m_toolbar.SetState( BUTTON_RIGHTALIGN, TBSTATE_ENABLED | ( para.wAlignment == PFA_RIGHT ? TBSTATE_CHECKED : 0 ) );

		m_toolbar.SetState( BUTTON_BULLET, TBSTATE_ENABLED | ( para.wNumbering ? TBSTATE_CHECKED : 0 ) );

		if( cf.dwMask & CFM_FACE )
			m_toolbar.SetFontName( CString( cf.szFaceName ) );

		if( cf.dwMask & CFM_SIZE )
			m_toolbar.SetFontSize( cf.yHeight / 20 );

		if( cf.dwMask & CFM_COLOR )
			m_toolbar.SetFontColor( cf.crTextColor );

	}

}

void CRulerRichEditCtrl::SetEffect( int mask, int effect )
/* ============================================================
	Function :		CRulerRichEditCtrl::SetEffect
	Description :	Sets the effect (bold, italic and/or 
					underline) for the currently selected text 
					in the embedded RTF-control.
	Access :		Private
					
	Return :		void
	Parameters :	int mask	-	What effects are valid. See 
									the documentation for 
									"CHARFORMAT".
					int effect	-	What effects to set. See the 
									documentation for "CHARFORMAT".
					
	Usage :			Called internally from button handlers

   ============================================================*/
{

	CharFormat cf;
	cf.dwMask = mask;
	cf.dwEffects = effect;

	m_rtf.SendMessage( EM_SETCHARFORMAT, SCF_SELECTION, ( LPARAM ) &cf );
	m_rtf.SetFocus();

}

void CRulerRichEditCtrl::SetAlignment( int alignment )
/* ============================================================
	Function :		CRulerRichEditCtrl::SetAlignment
	Description :	Sets the alignment for the currently 
					selected text in the embedded RTF-control.
	Access :		Private
					
	Return :		void
	Parameters :	int alignment	-	Alignment to set. See
										documentation for
										"PARAFORMAT"
					
	Usage :			Called internally from button handlers

   ============================================================*/
{

	ParaFormat	para( PFM_ALIGNMENT );
	para.wAlignment = ( WORD ) alignment;

	m_rtf.SetParaFormat( para );
	UpdateToolbarButtons();
	m_rtf.SetFocus();

}

// Virtual interface
void CRulerRichEditCtrl::DoFont()
/* ============================================================
	Function :		CRulerRichEditCtrl::DoFont
	Description :	Externally accessible member to set the
					font of the control
	Access :		Public
					
	Return :		void
	Parameters :	none
					
	Usage :			Call to set the font of the selected text.

   ============================================================*/
{

	// Get the current font
	LOGFONT	lf;
	ZeroMemory( &lf, sizeof( LOGFONT ) );
	CharFormat	cf;
	m_rtf.SendMessage( EM_GETCHARFORMAT, SCF_SELECTION, ( LPARAM ) &cf );
	int height;

	// Creating a LOGFONT from the current font settings

	// Font
	if( cf.dwMask & CFM_FACE )
		lstrcpy( lf.lfFaceName, cf.szFaceName );

	if( cf.dwMask & CFM_SIZE )
	{
		double twip = ( double )m_physicalInch / 1440;
		height = cf.yHeight;
		height = -( int ) ( ( double ) height * twip +.5 );
		lf.lfHeight = height;

	}

	// Effects
	if( cf.dwMask & CFM_BOLD )
	{
		if( cf.dwEffects & CFE_BOLD )
			lf.lfWeight = FW_BOLD;
		else
			lf.lfWeight = FW_NORMAL;
	}

	if( cf.dwMask & CFM_ITALIC )
		if( cf.dwEffects & CFE_ITALIC )
			lf.lfItalic = TRUE;

	if( cf.dwMask & CFM_UNDERLINE )
		if( cf.dwEffects & CFE_UNDERLINE )
			lf.lfUnderline = TRUE;

	// Show font dialog
	CFontDialog	dlg( &lf );
	if( dlg.DoModal() == IDOK )
	{
		// Apply new font
		cf.yHeight = dlg.GetSize() * 2;
		lstrcpy( cf.szFaceName, dlg.GetFaceName() );

		cf.dwMask = CFM_FACE | CFM_SIZE;
		cf.dwEffects = 0;

		if( dlg.IsBold() )
		{

			cf.dwMask |= CFM_BOLD;
			cf.dwEffects |= CFE_BOLD;

		}

		if( dlg.IsItalic() )
		{

			cf.dwMask |= CFM_ITALIC;
			cf.dwEffects |= CFE_ITALIC;

		}

		if( dlg.IsUnderline() )
		{

			cf.dwMask |= CFM_UNDERLINE;
			cf.dwEffects |= CFE_UNDERLINE;

		}

		m_rtf.SendMessage( EM_SETCHARFORMAT, SCF_SELECTION, ( LPARAM ) &cf );

	}

	m_rtf.SetFocus();

}

void CRulerRichEditCtrl::SetCurrentFontName( const CString& font )
/* ============================================================
	Function :		CRulerRichEditCtrl::SetCurrentFontName
	Description :	Changes the font of the selected text in 
					the editor to "font".
	Access :		Public
					
	Return :		void
	Parameters :	const CString& font	-	Font name of font 
											to change to.

	Usage :			Call to set the font of the selected text 
					in the editor.

   ============================================================*/
{
	CharFormat	cf;
	cf.dwMask = CFM_FACE;

	lstrcpy( cf.szFaceName, font );

	m_rtf.SendMessage( EM_SETCHARFORMAT, SCF_SELECTION, ( LPARAM ) &cf );
}

void CRulerRichEditCtrl::SetFontName( const CString& font, int size )
/* ============================================================
	Function :		CRulerRichEditCtrl::SetCurrentFontName
	Description :	Changes the font of the selected text in 
					the editor to "font".
	Access :		Public
					
	Return :		void
	Parameters :	const CString& font	-	Font name of font 
											to change to.

	Usage :			Call to set the font of the selected text 
					in the editor.

   ============================================================*/
{
	CharFormat	cf;
	cf.dwMask = CFM_FACE;

	if (size > 0)
	{
		cf.dwMask |= CFM_SIZE;
		cf.yHeight = size * 20;
	}
	lstrcpy( cf.szFaceName, font );

	m_rtf.SendMessage( EM_SETCHARFORMAT, 0, ( LPARAM ) &cf );
}

void CRulerRichEditCtrl::SetCurrentFontSize( int size )
/* ============================================================
	Function :		CRulerRichEditCtrl::SetCurrentFontSize
	Description :	Changes the size of the selected text in 
					the editor to "size" (measured in 
					typographical points).
	Access :		Public
					
	Return :		void
	Parameters :	int size	-	New size in typographical 
									points

	Usage :			Call to change the size of the selected 
					text.

   ============================================================*/
{
	CharFormat	cf;
	cf.dwMask = CFM_SIZE;
	cf.yHeight = size * 20;

	m_rtf.SendMessage( EM_SETCHARFORMAT, SCF_SELECTION, ( LPARAM ) &cf );
}

void CRulerRichEditCtrl::SetCurrentFontColor( COLORREF color )
/* ============================================================
	Function :		CRulerRichEditCtrl::SetCurrentFontSize
	Description :	Changes the color of the selected text in 
					the editor to "color".
	Access :		Public
					
	Return :		void
	Parameters :	COLORREF color	-	New color

	Usage :			Call to change the color of the selected 
					text.

   ============================================================*/
{

	CharFormat	cf;
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = color;

	m_rtf.SendMessage( EM_SETCHARFORMAT, SCF_SELECTION, ( LPARAM ) &cf );

}

LRESULT CRulerRichEditCtrl::OnSetCurrentFontName( WPARAM font, LPARAM )
/* ============================================================
	Function :		CRulerRichEditCtrl::OnSetCurrentFontName
	Description :	Handler for the registered message 
					"urm_SETCURRENTFONTNAME", called when the 
					font name is changed from the toolbar.
	Access :		Protected

	Return :		LRESULT		-	Not used
	Parameters :	WPARAM font	-	Pointer to the new font name
					LPARAM		-	Not used

	Usage :			Called from MFC

   ============================================================*/
{

	CString fnt( ( LPCTSTR ) font );
	SetCurrentFontName( fnt );

	return 0;
	
}

LRESULT CRulerRichEditCtrl::OnSetCurrentFontSize( WPARAM, LPARAM size )
/* ============================================================
	Function :		CRulerRichEditCtrl::OnSetCurrentFontSize
	Description :	Handler for the registered message 
					"urm_SETCURRENTFONTSIZE", called when the 
					font size is changed from the toolbar.
	Access :		Protected

	Return :		LRESULT		-	Not used
	Parameters :	WPARAM		-	Not used
					LPARAM size	-	New font size in typographical 
									points of the selected text

	Usage :			Called from MFC

   ============================================================*/
{

	SetCurrentFontSize( size );
	return 0;
	
}

LRESULT CRulerRichEditCtrl::OnSetCurrentFontColor( WPARAM, LPARAM color )
/* ============================================================
	Function :		CRulerRichEditCtrl::OnSetCurrentFontColor
	Description :	Handler for the registered message 
					"urm_SETCURRENTFONTCOLOR", called when the 
					font color is changed from the toolbar.
	Access :		Protected

	Return :		LRESULT		-	Not used
	Parameters :	WPARAM		-	Not used
					LPARAM		-	New color of the selected 
									text

	Usage :			Called from MFC

   ============================================================*/
{

	SetCurrentFontColor( ( COLORREF ) color );
	return 0;
	
}

void CRulerRichEditCtrl::DoColor()
/* ============================================================
	Function :		CRulerRichEditCtrl::DoColor
	Description :	Externally accessible member to set the
					color of the selected text
	Access :		Public
					
	Return :		void
	Parameters :	none
					
	Usage :			Call to set the color of the selected text.

   ============================================================*/
{

	// Get the current color
	COLORREF	clr( RGB( 0, 0, 0 ) );
	CharFormat	cf;
	m_rtf.SendMessage( EM_GETCHARFORMAT, SCF_SELECTION, ( LPARAM ) &cf );
	if( cf.dwMask & CFM_COLOR )
		clr = cf.crTextColor;

	// Display color selection dialog
	CColorDialog dlg( clr );
	if( dlg.DoModal() == IDOK )
	{

		// Apply new color
		cf.dwMask = CFM_COLOR;
		cf.dwEffects = 0;
		cf.crTextColor = dlg.GetColor();

		m_rtf.SendMessage( EM_SETCHARFORMAT, SCF_SELECTION, ( LPARAM ) &cf );

	}

	m_rtf.SetFocus();

}

void CRulerRichEditCtrl::DoBold()
/* ============================================================
	Function :		CRulerRichEditCtrl::DoBold
	Description :	Externally accessible member to set/unset
					the selected text to/from bold
	Access :		Public
					
	Return :		void
	Parameters :	none
					
	Usage :			Call to toggle the selected text to/from 
					bold.

   ============================================================*/
{

	m_toolbar.CheckButton( BUTTON_BOLD, !m_toolbar.IsButtonChecked( BUTTON_BOLD ) );

	int effect = 0;
	if( m_toolbar.IsButtonChecked( BUTTON_BOLD ) )
		effect = CFE_BOLD;

	SetEffect( CFM_BOLD, effect );

}

void CRulerRichEditCtrl::DoItalic()
/* ============================================================
	Function :		CRulerRichEditCtrl::DoItalic
	Description :	Externally accessible member to set/unset
					the selected text to/from italic
	Access :		Public
					
	Return :		void
	Parameters :	none
					
	Usage :			Call to toggle the selected text to/from 
					italic.

   ============================================================*/
{

	m_toolbar.CheckButton( BUTTON_ITALIC, !m_toolbar.IsButtonChecked( BUTTON_ITALIC ) );

	int effect = 0;
	if( m_toolbar.IsButtonChecked( BUTTON_ITALIC ) )
		effect = CFE_ITALIC;

	SetEffect( CFM_ITALIC, effect );

}

void CRulerRichEditCtrl::DoUnderline()
/* ============================================================
	Function :		CRulerRichEditCtrl::DoUnderline
	Description :	Externally accessible member to set/unset
					the selected text to/from underline
	Access :		Public
					
	Return :		void
	Parameters :	none
					
	Usage :			Call to toggle the selected text to/from 
					underlined.

   ============================================================*/
{

	m_toolbar.CheckButton( BUTTON_UNDERLINE, !m_toolbar.IsButtonChecked( BUTTON_UNDERLINE ) );

	int effect = 0;
	if( m_toolbar.IsButtonChecked( BUTTON_UNDERLINE ) )
		effect = CFE_UNDERLINE;

	SetEffect( CFM_UNDERLINE, effect );

}

void CRulerRichEditCtrl::DoLeftAlign()
/* ============================================================
	Function :		CRulerRichEditCtrl::DoLeftAlign
	Description :	Externally accessible member to set the
					selected text to left aligned.
	Access :		Public
					
	Return :		void
	Parameters :	none
					
	Usage :			Call to left-align the selected text

   ============================================================*/
{

	if( !m_toolbar.IsButtonChecked( BUTTON_LEFTALIGN ) )
		SetAlignment( PFA_LEFT );

}

void CRulerRichEditCtrl::DoCenterAlign()
/* ============================================================
	Function :		CRulerRichEditCtrl::DoCenterAlign
	Description :	Externally accessible member to set the
					selected text to center aligned
	Access :		Public
					
	Return :		void
	Parameters :	none
					
	Usage :			Call to center-align the selected text

   ============================================================*/
{

	if( !m_toolbar.IsButtonChecked( BUTTON_CENTERALIGN ) )
		SetAlignment( PFA_CENTER );

}

void CRulerRichEditCtrl::DoRightAlign()
/* ============================================================
	Function :		CRulerRichEditCtrl::DoRightAlign
	Description :	Externally accessible member to set the
					selected text to right aligned
	Access :		Public
					
	Return :		void
	Parameters :	none
					
	Usage :			Call to right-align the selected text

   ============================================================*/
{

	if( !m_toolbar.IsButtonChecked( BUTTON_RIGHTALIGN ) )
		SetAlignment( PFA_RIGHT );

}

void CRulerRichEditCtrl::DoIndent()
/* ============================================================
	Function :		CRulerRichEditCtrl::DoIndent
	Description :	Externally accessible member to indent the
					selected text to the next tab position
	Access :		Public
					
	Return :		void
	Parameters :	none
					
	Usage :			Call to indent the selected text

   ============================================================*/
{

	// Get current indent
	ParaFormat	para( PFM_STARTINDENT | PFM_TABSTOPS );
	m_rtf.GetParaFormat( para );
	int newindent = para.dxStartIndent;

#ifdef USE_TAB
	// Find next larger tab
	for( int t = MAX_TAB_STOPS - 1 ; t >= 0 ; t-- )
	{
		if( para.rgxTabs[ t ] > para.dxStartIndent )
			newindent = para.rgxTabs[ t ];

	}
#else
	newindent = para.dxStartIndent + 640;
#endif

	if( newindent != para.dxStartIndent )
	{

		// Set indent to this value
		para.dwMask = PFM_STARTINDENT | PFM_OFFSET;
		para.dxStartIndent = newindent;
		para.dxOffset = newindent;

		m_rtf.SetParaFormat( para );

	}

	m_rtf.SetFocus();

}

void CRulerRichEditCtrl::DoOutdent()
/* ============================================================
	Function :		CRulerRichEditCtrl::DoOutdent
	Description :	Externally accessible member to outdent the
					selected text to the previous tab position
	Access :		Public
					
	Return :		void
	Parameters :	none
					
	Usage :			Call to outdent the selected text

   ============================================================*/
{

	// Get the current indent, if any
	ParaFormat	para( PFM_STARTINDENT | PFM_TABSTOPS );
	m_rtf.GetParaFormat( para );
	int newindent = 0;

#ifdef USE_TAB
	// Find closest smaller tab
	for( int t = 0 ; t < MAX_TAB_STOPS ; t++ )
	{
		if( para.rgxTabs[ t ] < para.dxStartIndent )
			newindent = para.rgxTabs[ t ];
	}
#else
	newindent = para.dxStartIndent - 640;
	if (newindent < 0)
		newindent = 0;
#endif

	// Set indent to this value or 0 if none
	para.dwMask = PFM_STARTINDENT | PFM_OFFSET;
	para.dxStartIndent = newindent;
	para.dxOffset = newindent;

	m_rtf.SetParaFormat( para );
	m_rtf.SetFocus();


}

void CRulerRichEditCtrl::DoBullet()
/* ============================================================
	Function :		CRulerRichEditCtrl::DoBullet
	Description :	Externally accessible member to set the
					selected text to bulleted
	Access :		Public
					
	Return :		void
	Parameters :	none
					
	Usage :			Call to set the selected text to bulleted.

   ============================================================*/
{

	m_toolbar.CheckButton( BUTTON_BULLET, !m_toolbar.IsButtonChecked( BUTTON_BULLET ) );

	ParaFormat	para( PFM_NUMBERING );
	if( m_toolbar.IsButtonChecked( BUTTON_BULLET ) )
		para.wNumbering = PFN_BULLET;
	else
		para.wNumbering = 0;

	m_rtf.SetParaFormat( para );
	m_rtf.SetFocus();

}

void CRulerRichEditCtrl::OnButtonLine()
/* ============================================================
bLineSpacingRule
Type of line spacing. To use this member, set the PFM_SPACEAFTER flag in the dwMask member. This member can be one of the following values. 
0
Single spacing. The dyLineSpacing member is ignored.
1
One-and-a-half spacing. The dyLineSpacing member is ignored.
2
Double spacing. The dyLineSpacing member is ignored.
3
The dyLineSpacing member specifies the spacingfrom one line to the next, in twips. However, if dyLineSpacing specifies a value that is less than single spacing, the control displays single-spaced text.
4
The dyLineSpacing member specifies the spacing from one line to the next, in twips. The control uses the exact spacing specified, even if dyLineSpacing specifies a value that is less than single spacing.
5
The value of dyLineSpacing / 20 is the spacing, in lines, from one line to the next. Thus, setting dyLineSpacing to 20 produces single-spaced text, 40 is double spaced, 60 is triple spaced, and so on.

   ============================================================*/
{
    CLineSpacing     dlg;

    if (dlg.DoModal() != IDOK)
        return;

	PARAFORMAT2 	para;
	memset(&para, 0, sizeof(para));
	para.cbSize = sizeof(ParaFormat);
	para.dwMask = PFM_LINESPACING;

    para.dyLineSpacing = dlg.m_value;
    para.bLineSpacingRule = 4;

	m_rtf.SetParaFormat( para );
	UpdateToolbarButtons();
	m_rtf.SetFocus();
}

void CRulerRichEditCtrl::ShowToolbar( BOOL show )
/* ============================================================
	Function :		CRulerRichEditCtrl::ShowToolbar
	Description :	Shows or hides the toolbar
	Access :		Public
					
	Return :		void
	Parameters :	BOOL show	-	"TRUE" to show
					
	Usage :			Call to show or hide the toolbar subcontrol

   ============================================================*/
{

	m_showToolbar = show;

	if( m_hWnd )
	{
		if( show )
			m_toolbar.ShowWindow( SW_SHOW );
		else
			m_toolbar.ShowWindow( SW_HIDE );

		CRect rect;
		GetClientRect( rect );
		LayoutControls( rect.Width(), rect.Height() );
	}

}

void CRulerRichEditCtrl::ShowRuler( BOOL show )
/* ============================================================
	Function :		CRulerRichEditCtrl::ShowRuler
	Description :	Shows or hides the ruler
	Access :		Public
					
	Return :		void
	Parameters :	BOOL show	-	"TRUE" to show
					
	Usage :			Call to show or hide the ruler subcontrol

   ============================================================*/
{

	m_showRuler = show;

	if( m_hWnd )
	{
		if( show )
			m_ruler.ShowWindow( SW_SHOW );
		else
			m_ruler.ShowWindow( SW_HIDE );

		CRect rect;
		GetClientRect( rect );
		LayoutControls( rect.Width(), rect.Height() );
	}
}

void CRulerRichEditCtrl::LayoutControls( int width, int height )
/* ============================================================
	Function :		CRulerRichEditCtrl::LayoutControls
	Description :	Lays out the sub-controls depending on 
					visibility.
	Access :		Private
					
	Return :		void
	Parameters :	int width	-	Width of control
					int height	-	Height of control
					
	Usage :			Called internally to lay out the controls

   ============================================================*/
{

	int toolbarHeight = 0;
	if( m_showToolbar )
		toolbarHeight = TOOLBAR_HEIGHT;
	int rulerHeight = 0;
	if( m_showRuler )
		rulerHeight = RULER_HEIGHT;

	m_toolbar.MoveWindow( 0, 0, width, toolbarHeight );
	m_ruler.MoveWindow( 0, toolbarHeight, width, rulerHeight );

	int top = toolbarHeight + rulerHeight;
	CRect rect( 0, top, width, height );
	m_rtf.MoveWindow( rect );

}

BOOL CRulerRichEditCtrl::IsToolbarVisible() const
/* ============================================================
	Function :		CRulerRichEditCtrl::IsToolbarVisible
	Description :	Returns if the toolbar is visible or not
	Access :		Public
					
	Return :		BOOL	-	"TRUE" if visible
	Parameters :	none
					
	Usage :			Call to get the visibility of the toolbar

   ============================================================*/
{

	return m_showToolbar;

}

BOOL CRulerRichEditCtrl::IsRulerVisible() const
/* ============================================================
	Function :		CRulerRichEditCtrl::IsRulerVisible
	Description :	Returns if the ruler is visible or not
	Access :		Public
					
	Return :		BOOL	-	"TRUE" if visible
	Parameters :	none
					
	Usage :			Call to get the visibility of the ruler

   ============================================================*/
{

	return m_showRuler;

}

void CRulerRichEditCtrl::SetReadOnly( BOOL readOnly )
/* ============================================================
	Function :		CRulerRichEditCtrl::SetReadOnly
	Description :	Sets the control to read only or not.
	Access :		Public
					
	Return :		void
	Parameters :	BOOL readOnly	-	New read only state
					
	Usage :			Call to set the read only state of the 
					control

   ============================================================*/
{

	if( m_rtf.m_hWnd )
		m_rtf.SetReadOnly( readOnly );

	m_readOnly = readOnly;

}

BOOL CRulerRichEditCtrl::GetReadOnly() const
/* ============================================================
	Function :		CRulerRichEditCtrl::GetReadOnly
	Description :	Returns if the control is read only or not
	Access :		Public
					
	Return :		BOOL	-	"TRUE" if read only
	Parameters :	none
					
	Usage :			Call to get the read only-state of the 
					control

   ============================================================*/
{

	return m_readOnly;

}

LRESULT CRulerRichEditCtrl::OnUndo(WPARAM wParam, LPARAM lParam)
{
	m_rtf.Undo();
	return 0;
}

LRESULT CRulerRichEditCtrl::OnCopy(WPARAM wParam, LPARAM lParam)
{
	m_rtf.Copy();
	return 0;
}

LRESULT CRulerRichEditCtrl::OnCut(WPARAM wParam, LPARAM lParam)
{
	m_rtf.Cut();
	return 0;
}

LRESULT CRulerRichEditCtrl::OnPaste(WPARAM wParam, LPARAM lParam)
{
	m_rtf.Paste();
	return 0;
}

LRESULT CRulerRichEditCtrl::OnSelectAll(WPARAM wParam, LPARAM lParam)
{
	m_rtf.SetSel(0, -1);
	return 0;
}


LRESULT CRulerRichEditCtrl::OnLink(WPARAM wParam, LPARAM lParam) 
{
	CString     s, str;
	CHARRANGE   cr;

	cr.cpMin = cr.cpMax = 0;
	m_rtf.SendMessage(EM_EXGETSEL, 0, (LPARAM)&cr);

	GetText(s, cr.cpMin, cr.cpMax);
    this->GetParent()->SendMessage(WM_LINK_SET, (WPARAM) &s, (LPARAM) this);

    return 0;
}

void CRulerRichEditCtrl::SetLink(CString &text, CString &link, bool addLF)
{
/*
	CString     s, str;
	CHARRANGE   cr;

	cr.cpMin = cr.cpMax = 0;
	m_rtf.SendMessage(EM_EXGETSEL, 0, (LPARAM)&cr);

	GetText(s, cr.cpMin, cr.cpMax);
*/
	CString     str;
	SETTEXTEX	t;
    t.flags = ST_KEEPUNDO | ST_SELECTION;
	t.codepage = CP_ACP;
    
    CString lf;
    if (addLF) 
        lf = "\\par";

	str.Format("{\\rtf{\\field{\\*\\fldinst{HYPERLINK \"%s\"}}{\\fldrslt{\\ul %s}}}%s}", link, text, lf);
	TRACE("%s", str);
    m_rtf.SendMessage( EM_SETTEXTEX, (WPARAM) &t, (LPARAM) (LPCTSTR)str);
	m_rtf.SetFocus();

#if 0
//	if( m_toolbar.IsButtonChecked( BUTTON_LINK ) )
	{
		t.flags = ST_KEEPUNDO | ST_SELECTION;
		t.codepage = CP_ACP;
		
		CString str, url;

//		url = "www.apple.com";
//		str.Format("{\\rtf{\\field{\\*\\fldinst{HYPERLINK \"%s\"}}{\\fldrslt{\\ul %s}}}}", url, s);
//		m_rtf.SendMessage( EM_SETTEXTEX, (WPARAM) &t, (LPARAM) (LPCTSTR)str);
	}
//	else
	{
		TRACE("%s\n", s);
		m_rtf.Clear();
		m_rtf.ReplaceSel("abc");
/*
		t.flags = ST_KEEPUNDO | ST_SELECTION;
		t.codepage = CP_ACP;
		
		CString str;
		str.Format("{\\rtf1%sabcdddddddd}", s);
		m_rtf.SendMessage( EM_SETTEXTEX, (WPARAM) &t, (LPARAM) (LPCTSTR)str);
		*/
	}

#endif
    return;
}


void CRulerRichEditCtrl::OnRButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    CRect r;
    
    m_rtf.GetWindowRect(r);
    point.x = point.x - r.left;
    point.y -= r.top;

     int     n = m_rtf.CharFromPos(point);
//   m_rtf.SetSel(n, n);
    CWnd::OnRButtonUp(nFlags, point);
}
