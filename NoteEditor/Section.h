#pragma once
#include "ResizableLib/ResizableDialog.h"


// Section dialog

class Section : public CResizableDialog
{
	DECLARE_DYNAMIC(Section)

public:
	Section(CWnd* pParent = nullptr);   // standard constructor
	virtual ~Section();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SECTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSectionAdd();
	afx_msg void OnBnClickedSectionRemove();
	afx_msg void OnBnClickedSectionRename();
	afx_msg void OnLbnDblclkList2();
	CListBox m_list;
	afx_msg void OnBnClickedSectionUp();
	afx_msg void OnBnClickedSectionDown();
	afx_msg void OnLbnSelchangeList();
	CButton m_move_up_bt;
	CButton m_move_down_bt;
	CButton m_rename_bt;
	CButton m_remove_bt;
};
