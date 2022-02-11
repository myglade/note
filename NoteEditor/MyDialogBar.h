#pragma once
#include "afxdialogex.h"

// MyDialogBar dialog

class MyDialogBar : public CDialogBar
{
	DECLARE_DYNAMIC(MyDialogBar)

public:
	MyDialogBar();   // standard constructor
	virtual ~MyDialogBar();
	void Init();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDR_MAINFRAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	CComboBox m_prop;
	CComboBox m_tag;
	CStatic m_keys;
	//CButton m_cur_status;
	afx_msg void OnStnDblclickStatic1();
	CEdit m_keys_editor;
	afx_msg void OnEnKillfocusEdit1();
};
