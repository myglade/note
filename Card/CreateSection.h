#pragma once


// CCreateSection dialog

class CCreateSection : public CDialog
{
	DECLARE_DYNAMIC(CCreateSection)

public:
	CCreateSection(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCreateSection();

// Dialog Data
	enum { IDD = IDD_CREATE_SECTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_dictMode;
	CString m_name;
	virtual BOOL OnInitDialog();
};
