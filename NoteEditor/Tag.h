#pragma once
#include "ResizableLib/ResizableDialog.h"


// Tag dialog

class Tag : public CResizableDialog
{
	DECLARE_DYNAMIC(Tag)

public:
	Tag(CWnd* pParent = nullptr);   // standard constructor
	virtual ~Tag();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedTagAdd();
	afx_msg void OnBnClickedTagRemove();
	afx_msg void OnBnClickedTagRename();
	afx_msg void OnLbnDblclkList2();
	CListBox m_list;
	afx_msg void OnBnClickedTagUp();
	afx_msg void OnBnClickedTagDown();
	afx_msg void OnLbnSelchangeList();
	CButton m_remove_bt;
	CButton m_rename_bt;
	CButton m_move_up_bt;
	CButton m_move_down_bt;
	virtual void OnOK();
};
