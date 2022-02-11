
// MainFrm.h : interface of the CMainFrame class
//

#pragma once

#include "MyDialogBar.h"

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	MyDialogBar& GetDialogBar() { return m_wndDlgBar;  }

// Operations
public:
	BOOL ReadWindowPlacement(WINDOWPLACEMENT* pwp);
	void WriteWindowPlacement(WINDOWPLACEMENT* pwp);

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
	CToolBar          m_wndToolBar;
	CReBar            m_wndReBar;
	MyDialogBar       m_wndDlgBar;
	CFont			  m_font;
	CComboBox		  m_section;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	virtual void ActivateFrame(int nCmdShow = -1);
	afx_msg void OnClose();
};


