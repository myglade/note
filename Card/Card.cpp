// Card.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Card.h"
#include "MainFrm.h"
#include "Util.h"

#include "CardDoc.h"
#include "CardView.h"
#include <process.h>
#include "http/webserver.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const TCHAR* MyMainWndClassName = _T("hscard");

unsigned int __stdcall HttpServer(void *p);

// CCardApp

BEGIN_MESSAGE_MAP(CCardApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CCardApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CCardApp construction

CCardApp::CCardApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CCardApp object

CCardApp theApp;


// CCardApp initialization

BOOL CCardApp::InitInstance()
{
    CWnd *	wnd;
	if (wnd = FirstInstance())
	{
		if (__targv[1] == 0)
			return FALSE;

//		CUsifcDoc::SetSetting("request", __targv[1]);
        SetSetting("req", __targv[1]);
		wnd->SendMessage(WM_COMMAND, ID_REQUEST, 0);
		return FALSE;
	}

//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
#ifdef _DEBUG
	SetRegistryKey(_T("Flash Card Test"));
#else
	SetRegistryKey(_T("Flash Card"));
#endif

	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CCardDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CCardView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
    cmdInfo.m_nShellCommand = CCommandLineInfo::FileNew;

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand

	webserver::m_port = GetEnv(PROFILE_SECTION, WEBSERVER_PORT, 9999);
	SetHostAddr();

	int webServer = GetEnv(PROFILE_SECTION, LAUNCH_WEBSERVER, 0);

	if (webServer)
	{
		unsigned ret;
		m_handle = reinterpret_cast<HANDLE>(_beginthreadex(0, 0, HttpServer, 0, 0, &ret));
	}
	else
	{
		m_handle = NULL;
	}

    if (cmdInfo.m_strFileName != "") 
    {
        SetSetting("req", cmdInfo.m_strFileName);
	    m_pMainWnd->SendMessage(WM_COMMAND, ID_REQUEST, 0);
    }

	return TRUE;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	CString m_host;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	m_host = webserver::GetExternalAddr();
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_host);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CCardApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CCardApp message handlers


int CCardApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_handle != NULL)
	{
		webserver::m_stop = 1;
		WaitForSingleObject(m_handle, INFINITE);
		CloseHandle(m_handle);
	}

	GdiplusShutdown(m_gdiplusToken);	

	return CWinApp::ExitInstance();
}


CWnd *CCardApp::FirstInstance()
{
    CWnd *PrevCWnd, *ChildCWnd;

    // Determine if another window with our class name exists.
    PrevCWnd = CWnd::FindWindow(MyMainWndClassName, NULL);
    if (PrevCWnd == NULL)
        return NULL;

    PrevCWnd->ShowWindow(SW_NORMAL);	
    // If so, does it have any pop-ups?
    ChildCWnd = PrevCWnd->GetLastActivePopup();
    // Bring the main window to the top.
    PrevCWnd->BringWindowToTop();
    // If iconic, restore the main window.
    if (PrevCWnd->IsIconic())
        PrevCWnd->ShowWindow(SW_RESTORE);

    // If there are pop-ups, bring them along too!
    //if (PrevCWnd != ChildCWnd)
        ChildCWnd->BringWindowToTop();
    ChildCWnd->SetForegroundWindow();

    // Return FALSE. This isn't the first instance
    // and we are done activating the previous one.
    return PrevCWnd;
}


