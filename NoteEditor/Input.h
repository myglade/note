#pragma once
#include "afxdialogex.h"


// Input dialog

class Input : public CDialogEx
{
	DECLARE_DYNAMIC(Input)

public:
	Input(CWnd* pParent = nullptr);   // standard constructor
	virtual ~Input();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INPUT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void Set(const CString& title, const CString& name);

	CString m_title;
	CString m_name;
};
