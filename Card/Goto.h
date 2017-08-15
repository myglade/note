#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "ResizableLib/ResizableDialog.h"
#include "DbManager.h"
#include <string>
#include <map>
#include <vector>

using namespace std;

//typedef map<string, StringMapArray > CDbMap;

// CGoto dialog

class CGoto : public CResizableDialog
{
	DECLARE_DYNAMIC(CGoto)

public:
	CGoto(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGoto();

    void FillItems(LPCTSTR dbName);
    void FillCategory(LPCTSTR dbName);
    int FindItemData(CComboBox &wnd, int value);

    // Dialog Data
	enum { IDD = IDD_GOTO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
    CComboBox       m_dbList;
    CListCtrl       m_list;
  	CDbManager *    m_db;
//    CDbMap          m_dbMap;
    CString         m_curDb;
	CString			m_curId;
    int             m_curCategory;
    StringMapArray  m_summary;
    CComboBox       m_category;

    virtual BOOL OnInitDialog();
    afx_msg void OnCbnSelchangeCombo1();

	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnCbnSelchangeCategory();
};
