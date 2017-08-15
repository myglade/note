#pragma once
#include "afxcmn.h"
#include "ResizableLib/ResizableDialog.h"
#include "DbManager.h"
#include "Resource.h"

// CSearchResult dialog

class CSearchResult : public CResizableDialog
{
	DECLARE_DYNAMIC(CSearchResult)

public:
	CSearchResult(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSearchResult();

// Dialog Data
	enum { IDD = IDD_SEARCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual void OnCancel();
public:
	CListCtrl m_list;
	CDbManager *m_db;

	virtual BOOL OnInitDialog();
	void FillItem(CString & key, StringMapArray &result);
	void FillItem();
	void Clear();

	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	virtual void PostNcDestroy();

public:
	CWnd *		m_parent;
	CString		m_key;
    StringMapArray  m_result;
};
