#pragma once


// CSearchBox dialog

class CSearchBox : public CDialog
{
	DECLARE_DYNAMIC(CSearchBox)

public:
	CSearchBox(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSearchBox();

// Dialog Data
	enum { IDD = IDD_SEARCH_BOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    CString m_string;
};
