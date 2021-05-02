// CardView.h : interface of the CCardView class
//


#pragma once

#include "Resource.h"
#include "CardDoc.h"
#include "DbManager.h"
#include "afxwin.h"
#include "KeyList.h"
#include "SearchResult.h"
#include "CheckComboBox.h"
#include "WndResizer.h"
#include "DirectoryChanges.h"
#include "DbDirChangeHandler.h"
#include <list>
#include <map>
#include "CEXPLORER2.h"

#define MAX_HISTORY     100

#define WM_DB_CHANGE 		(WM_USER+3001)

#define PUSHFORSYNC_TIMER   1600

using namespace std;
struct ViewInfo
{
	CString			db;
	CString			id;
};

typedef map<string, int> NotifySet;
typedef map<int, string> NotifyMap;

typedef list<ViewInfo>	ViewList;

class CCardView : public CFormView, CDbListener
{
friend class DirChangeHandler;
protected: // create from serialization only
	CCardView();
	DECLARE_DYNCREATE(CCardView)

public:
	enum{ IDD = IDD_CARD_FORM };

// Attributes
public:
	CCardDoc* GetDocument() const;

	CKeyList *			m_keyListView;
	CSearchResult *		m_searchView;

private:
	CWndResizer			m_resizer;

	BOOL				m_readOnlyMode;
	BOOL				m_testMode;
	CString				m_info;
	BOOL				m_itemBookmark;
	CCheckComboBox		m_itemTagCombo;

	CEXPLORER2			m_content;
	CEXPLORER2			m_key;
	CComboBox			m_itemCategory;
	CComboBox *			m_category;
	CComboBox *			m_section;
	CButton *			m_bookmarkBtn;
//	CButton *			m_readOnlyBtn;
	CButton *			m_orBtn;
	CCheckComboBox *	m_tagCombo;
	CUIntArray			m_tagComboIDList;

	CDbManager			m_db;
	CString				m_dbFolder;
	int					m_add;
	BOOL				m_itemCategoryChange;
	BOOL				m_showContent;
	BOOL				m_itemBookmarkChange;
	BOOL				m_bookmarkMode;
	BOOL				m_itemTagChange;
	BOOL				m_dictMode;
	BOOL				m_setSplitterPos;
	
	CDirectoryChangeWatcher m_dirWatcher;
	DbDirChangeHandler	m_dirHandler;
	BOOL				m_useWorkingFolder;
    int                 m_timer;

    NotifySet           m_notifySet;
    NotifyMap           m_notifyMap;
    int                 m_currentTimer;

	// for navigation
	ViewList			m_prevView;
	ViewList			m_nextView;

    int                 m_pushForSyncTimer;
// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CCardView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	int SetItem();
	void DisplayText(BOOL blank = FALSE);
	void ShowContent(int bShow);
	void OnGoto(int index);
    void GotoByitime(int itime);
	void UpdateKeyListView();

// helper
	int OpenDB();
	void UpdateSection();
	void UpdateCategory();
	void UpdateTag();
	void CheckBookmarkChange();
	void SetTagList(CCheckComboBox &combo, CUIntArray &list);
	void GetTagList(CCheckComboBox &wnd, CUIntArray &res);
	BOOL CompareTagList(CCheckComboBox &wnd, CUIntArray &tags);
	BOOL CompareTagList(CUIntArray &tag1, CUIntArray &tag2);
	int FindItemData(CComboBox &wnd, int value);
	void ConvertRtf2Text(const char *rtf, CString &res);
	void ConvertRtf2Text(CString &rtf, CString &res);
	void GetInfo(CString &s);
	void CreateEditors();
	int SaveAsFc();
	void GotoById(LPCTSTR db, LPCTSTR id, BOOL setHistory, BOOL delayUpdate);
	void SetHistory();
    void toClipboard(CString &s);
    void TriggerPush();

    virtual void DbNotify(int msg);

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnFileSaveIos();
	afx_msg void OnEditAdditem();
	afx_msg void OnEditRemoveitem();
	afx_msg void OnEditMovetofirst();
	afx_msg void OnViewFirst();
	afx_msg void OnViewPrev();
	afx_msg void OnViewNext();
	afx_msg void OnViewLast();
	afx_msg void OnWindowKeyList();
	afx_msg void OnWindowSearch();
	afx_msg void OnCbnSelchangeCategory();
	afx_msg void OnCbnSelchangeSection();
	afx_msg void OnBnClickedBookmarkmode();
	afx_msg void OnBnClickedSearchmode();
	afx_msg void OnBnClickedReadonlymode();
	afx_msg void OnBnClickedTestmode();
	afx_msg void OnEditSetting();
	afx_msg void OnUpdateViewPrev(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewNext(CCmdUI *pCmdUI);
	afx_msg void OnCbnSelchangeItemCategory();
	afx_msg void OnUpdateEditAdditem(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditCheckitem(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditMovetofirst(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditMovetoprev(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditMovetonext(CCmdUI *pCmdUI);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditCut();
	afx_msg void OnEditUndo();
	afx_msg void OnEditSelectAll();
	afx_msg void OnEditSave();
	afx_msg void OnUpdateEditSave(CCmdUI *pCmdUI);
	afx_msg void OnViewGoto();
	afx_msg void OnViewSearchinkey();
	afx_msg void OnViewSearchincontent();
	afx_msg void OnViewBookmarkmode();
	afx_msg void OnUpdateViewBookmarkmode(CCmdUI *pCmdUI);
    afx_msg void OnViewUrl();
    afx_msg void OnUpdateViewUrl(CCmdUI *pCmdUI);
    afx_msg void OnCbnSelchangeTagCombo();
	afx_msg void OnEditCategory();
	afx_msg void OnEditTag();
	afx_msg void OnCbnCloseupItemTagCombo();
	afx_msg void OnOpenReq();

	LRESULT  OnResizerUnhook(WPARAM wParam, LPARAM lParam);
	LRESULT  OnResizerSize(WPARAM wParam, LPARAM lParam);
	LRESULT  OnResizerPortionChange(WPARAM wParam, LPARAM lParam);
	LRESULT  OnFileNotification(WPARAM wParam, LPARAM lParam);
    LRESULT  OnDbStatusChange(WPARAM wParam, LPARAM lParam);

protected:
public:
	afx_msg void OnViewReadonlymode();
	afx_msg void OnUpdateViewReadonlymode(CCmdUI *pCmdUI);
	afx_msg void OnViewTestmode();
	afx_msg void OnUpdateViewTestmode(CCmdUI *pCmdUI);
	afx_msg void OnEditCompactdb();
	afx_msg void OnEditMovetoprev();
	afx_msg void OnEditMovetonext();
protected:
public:
	afx_msg void OnViewPrevview();
	afx_msg void OnViewNextview();
	afx_msg void OnUpdateViewPrevview(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewNextview(CCmdUI *pCmdUI);
//	afx_msg void OnXButtonDown(UINT nFlags, UINT nButton, CPoint point);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDbChange();

    afx_msg void OnBnClickedBookmark();
    afx_msg void OnEditReload();
    afx_msg void OnEditSetbookmark();
    afx_msg void OnEditResetbookmark();
    afx_msg void OnEditMoveto();
    afx_msg void OnUpdateEditMoveto(CCmdUI *pCmdUI);
    afx_msg void OnEditSetnumbering();
    afx_msg void OnEditExporttohtml();
    afx_msg void OnPushForSync();
    afx_msg void OnUpdatePushForSync(CCmdUI *pCmdUI);
    CButton m_syncButton;
};

#ifndef _DEBUG  // debug version in CardView.cpp
inline CCardDoc* CCardView::GetDocument() const
   { return reinterpret_cast<CCardDoc*>(m_pDocument); }
#endif

