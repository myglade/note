#pragma once
#include "afxwin.h"
#include "DbManager.h"

// CSetting dialog

class CSetting : public CDialog
{
	DECLARE_DYNAMIC(CSetting)

public:
	CSetting(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetting();

// Dialog Data
	enum { IDD = IDD_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_sections;
	CDbManager *	m_db;

	void LoadSections();

	afx_msg void OnBnClickedAddSection();
	afx_msg void OnBnClickedRemoveSection();
	afx_msg void OnBnClickedSetWorkingFolder();
	afx_msg void OnBnClickedMakeWorkingFolder();
	afx_msg void OnBnClickedLaunchWebserver();
	afx_msg void OnBnClickedClearCache();
	afx_msg void OnLbnSelchangeDatabase();
	afx_msg void OnBnClickedKeySizeTag();
	afx_msg void OnBnClickedContentSizeTag();
	afx_msg void OnBnClickedAddImageToFc();
	afx_msg void OnBnClickedRadio1();

	virtual BOOL OnInitDialog();
	int m_sort;
protected:
	virtual void OnOK();
	void SetFcOption();

public:
	UINT m_port;
	BOOL m_makingWorkingFolder;
	BOOL m_webServer;
	CString m_workingFolder;
	CEdit m_workingFolderCtrl;
	CButton m_setWorkingFolderBtn;
	CEdit m_portCtrl;
	UINT m_cacheSize;
	CEdit m_dictMode;
	CButton m_keySizeTagBtn;
	CButton m_contentSizeTagBtn;
	CButton m_addImage2FcBtn;
 //   CString m_url;
    CString m_hideTags;
    BOOL m_allowImmediatePush;
    UINT m_pushInterval;
};
