#pragma once


// CInput dialog

class CInput : public CDialog
{
	DECLARE_DYNAMIC(CInput)

public:
	CInput(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInput();

// Dialog Data
	enum { IDD = IDD_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_name;
	CString m_title;

	void Set(CString title, CString name);
	virtual BOOL OnInitDialog();
};
