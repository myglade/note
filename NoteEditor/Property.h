#pragma once

#include "ResizableLib/ResizableDialog.h"

// Category dialog

class Property : public CResizableDialog
{
	DECLARE_DYNAMIC(Property)

public:
	Property(CWnd* pParent = nullptr);   // standard constructor
	virtual ~Property();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPERTY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedPropAdd();
	afx_msg void OnBnClickedPropRemove();
	afx_msg void OnBnClickedPropRename();
	afx_msg void OnLbnDblclkList2();
	CListBox m_list;
	afx_msg void OnBnClickedPropUp();
	afx_msg void OnBnClickedPropDown();
	afx_msg void OnLbnSelchangeList();
	CButton m_remove_bt;
	CButton m_rename_bt;
	CButton m_move_up_bt;
	CButton m_move_down_bt;
	virtual void OnOK();
};
