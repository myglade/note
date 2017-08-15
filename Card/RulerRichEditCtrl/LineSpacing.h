#pragma once

#include "resource"

// CLineSpacing dialog

class CLineSpacing : public CDialog
{
	DECLARE_DYNAMIC(CLineSpacing)

public:
	CLineSpacing(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLineSpacing();

// Dialog Data
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    CComboBox m_space;
    int m_value;
    afx_msg void OnCbnSelchangeCombo1();
};
