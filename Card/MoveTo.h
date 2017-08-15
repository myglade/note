#pragma once
#include "afxwin.h"


// CMoveTo dialog

class CMoveTo : public CDialog
{
	DECLARE_DYNAMIC(CMoveTo)

public:
	CMoveTo(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMoveTo();

// Dialog Data
	enum { IDD = IDD_MOVETO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
    UINT    m_number;
    UINT    m_max;
    CEdit m_numberWnd;
};

