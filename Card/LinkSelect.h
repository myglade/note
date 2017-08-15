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

// CLinkSelect dialog

class CLinkSelect : public CResizableDialog
{
	DECLARE_DYNAMIC(CLinkSelect)

public:
	CLinkSelect(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLinkSelect();

    void FillItems(LPCTSTR dbName);
    void FillCategory(LPCTSTR dbName);
    int FindItemData(CComboBox &wnd, int value);

// Dialog Data
	enum { IDD = IDD_LINK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    CString         m_display;
    CString         m_address;
    CComboBox       m_dbList;
    CListCtrl       m_list;
  	CDbManager *    m_db;
 //   CDbMap          m_dbMap;
    CString         m_curDb;
    CComboBox       m_category;
    int             m_curCategory;
    StringMapArray  m_summary;

    afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
    virtual BOOL OnInitDialog();
    afx_msg void OnCbnSelchangeCombo1();
    afx_msg void OnCbnSelchangeCategory();
    CString m_address_onenote;
};
