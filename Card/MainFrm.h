// MainFrm.h : interface of the CMainFrame class
//


#pragma once
#include "CheckComboBox.h"

#define MIN_WIDTH			570
#define MIN_HEIGHT			460

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CComboBox		m_section;
	CComboBox		m_category;
	CEdit			m_search;
	CFont			m_font;
	CButton			m_bookmarkBtn;
//	CButton			m_readOnlyBtn;
//	CButton			m_testBtn;
	CCheckComboBox	m_tag;
	CButton			m_orBtn;
// Operations
public:
	BOOL ReadWindowPlacement(WINDOWPLACEMENT *pwp);
	void WriteWindowPlacement(WINDOWPLACEMENT *pwp);

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
//	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	virtual void ActivateFrame(int nCmdShow = -1);
	afx_msg void OnClose();
};


