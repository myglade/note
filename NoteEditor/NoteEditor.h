
// NoteEditor.h : main header file for the NoteEditor application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CNoteEditorApp:
// See NoteEditor.cpp for the implementation of this class
//

class CNoteEditorApp : public CWinApp
{
public:
	CNoteEditorApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CNoteEditorApp theApp;
