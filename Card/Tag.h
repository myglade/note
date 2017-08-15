#pragma once
#include "afxwin.h"
#include "DbManager.h"
#include "ResizableLib/ResizableDialog.h"


// CTag dialog

class CTag : public CResizableDialog
{
	DECLARE_DYNAMIC(CTag)

public:
	CTag(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTag();

// Dialog Data
	enum { IDD = IDD_TAG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CDbManager *		m_db;
    CWnd *              m_cardView;

	CListBox m_list;

	void Load();
    void UpdateParentWindow();

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedTagAdd();
	afx_msg void OnBnClickedTagRemove();
	afx_msg void OnBnClickedTagRename();
	afx_msg void OnLbnDblclkList2();
};
