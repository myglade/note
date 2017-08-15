#pragma once
#include "afxcmn.h"
#include "ResizableLib/ResizableDialog.h"
#include "DbManager.h"

// CKeyList dialog
typedef CMap<int, int, int, int>	IdMap;

class CKeyList : public CResizableDialog
{
	DECLARE_DYNAMIC(CKeyList)

public:
	CKeyList(CWnd* pParent = NULL);   // standard constructor
	virtual ~CKeyList();

// Dialog Data
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual void OnCancel();
public:
	CListCtrl m_list;
	CDbManager *m_db;

	virtual BOOL OnInitDialog();
	void FillItem(bool force = false);
    void GetIdListofSelection(CUIntArray &list);

	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	virtual void PostNcDestroy();

public:
	CWnd *		m_parent;
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};
