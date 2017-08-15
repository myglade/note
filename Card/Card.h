// Card.h : main header file for the Card application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CCardApp:
// See Card.cpp for the implementation of this class
//

class CCardApp : public CWinApp
{
public:
	CCardApp();

    CWnd *FirstInstance();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();

	HANDLE m_handle;
	ULONG_PTR m_gdiplusToken; 

};

extern CCardApp theApp;