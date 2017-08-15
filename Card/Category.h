#pragma once
#include "afxwin.h"
#include "DbManager.h"
#include "ResizableLib/ResizableDialog.h"


// CCategory dialog

class CCategory : public CResizableDialog
{
	DECLARE_DYNAMIC(CCategory)

public:
	CCategory(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCategory();

// Dialog Data
	enum { IDD = IDD_CATEGORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CDbManager *		m_db;

	CListBox m_list;

	void Load();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCateAdd();
	afx_msg void OnBnClickedCateRemove();
	afx_msg void OnBnClickedCateRename();
	afx_msg void OnLbnDblclkList2();
};
