// Setting.cpp : implementation file
//

#include "stdafx.h"
#include "Card.h"
#include "Setting.h"
#include "CreateSection.h"
#include "CardView.h"
#include "Env.h"

// CSetting dialog

IMPLEMENT_DYNAMIC(CSetting, CDialog)

CSetting::CSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CSetting::IDD, pParent)
	, m_sort(0)
	, m_port(0)
	, m_makingWorkingFolder(FALSE)
	, m_webServer(FALSE)
	, m_workingFolder(_T(""))
	, m_cacheSize(0)
    , m_hideTags(_T(""))
    , m_allowImmediatePush(FALSE)
    , m_pushInterval(0)
{
}

CSetting::~CSetting()
{
}

void CSetting::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_sections);
    DDX_Radio(pDX, IDC_RADIO1, m_sort);
    DDX_Text(pDX, IDC_EDIT1, m_port);
    DDV_MinMaxUInt(pDX, m_port, 80, 65000);
    DDX_Check(pDX, IDC_CHECK1, m_makingWorkingFolder);
    DDX_Check(pDX, IDC_CHECK3, m_webServer);
    DDX_Text(pDX, IDC_EDIT3, m_workingFolder);
    DDX_Control(pDX, IDC_EDIT3, m_workingFolderCtrl);
    DDX_Control(pDX, IDC_SET_WORKING_FOLDER, m_setWorkingFolderBtn);
    DDX_Control(pDX, IDC_EDIT1, m_portCtrl);
    DDX_Text(pDX, IDC_EDIT4, m_cacheSize);
    DDV_MinMaxUInt(pDX, m_cacheSize, 10, 1024);
    DDX_Control(pDX, IDC_KEY_SIZE_TAG, m_keySizeTagBtn);
    DDX_Control(pDX, IDC_CONTENT_SIZE_TAG, m_contentSizeTagBtn);
    DDX_Control(pDX, IDC_ADD_IMAGE_TO_FC, m_addImage2FcBtn);
    DDX_Control(pDX, IDC_EDIT5, m_dictMode);
    DDX_Text(pDX, IDC_EDIT8, m_hideTags);
    DDX_Check(pDX, IDC_CHECK4, m_allowImmediatePush);
    DDX_Text(pDX, IDC_EDIT9, m_pushInterval);
	DDV_MinMaxUInt(pDX, m_pushInterval, 0, UINT_MAX);
}


BEGIN_MESSAGE_MAP(CSetting, CDialog)
	ON_BN_CLICKED(IDC_ADD_SECTION, &CSetting::OnBnClickedAddSection)
	ON_BN_CLICKED(IDC_REMOVE_SECTION, &CSetting::OnBnClickedRemoveSection)
	ON_BN_CLICKED(IDC_SET_WORKING_FOLDER, &CSetting::OnBnClickedSetWorkingFolder)
	ON_BN_CLICKED(IDC_CHECK1, &CSetting::OnBnClickedMakeWorkingFolder)
	ON_BN_CLICKED(IDC_CHECK3, &CSetting::OnBnClickedLaunchWebserver)
	ON_BN_CLICKED(IDC_CLEAR_CACHE, &CSetting::OnBnClickedClearCache)
	ON_LBN_SELCHANGE(IDC_LIST1, &CSetting::OnLbnSelchangeDatabase)
	ON_BN_CLICKED(IDC_RADIO1, &CSetting::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CSetting::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_KEY_SIZE_TAG, &CSetting::OnBnClickedKeySizeTag)
	ON_BN_CLICKED(IDC_CONTENT_SIZE_TAG, &CSetting::OnBnClickedContentSizeTag)
	ON_BN_CLICKED(IDC_ADD_IMAGE_TO_FC, &CSetting::OnBnClickedAddImageToFc)
END_MESSAGE_MAP()


// CSetting message handlers


BOOL CSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	LoadSections();

	m_makingWorkingFolder = GetEnv(PROFILE_SECTION, USE_WORKING_FOLDER, 0);
	m_workingFolder = GetEnv(PROFILE_SECTION, WORKING_FOLDER, "c:\\temp\\");
	m_webServer = GetEnv(PROFILE_SECTION, LAUNCH_WEBSERVER, 0);
	m_port = GetEnv(PROFILE_SECTION, WEBSERVER_PORT, 9999);
	m_cacheSize = GetEnv(PROFILE_SECTION, CACHE_SIZE, 30);

//    m_url = GetEnv(PROFILE_SECTION, URL, "http://localhost:9999/db/query2");
    m_hideTags = GetEnv(PROFILE_SECTION, HIDE_TAGS, "");
    m_pushInterval = GetEnv(PROFILE_SECTION, PUSH_INTERVAL, 300);
    m_allowImmediatePush = GetEnv(PROFILE_SECTION, ALLOW_IMMEDIATE_PUSH, 0);

	m_workingFolderCtrl.EnableWindow(m_makingWorkingFolder);
	m_setWorkingFolderBtn.EnableWindow(m_makingWorkingFolder);
	m_portCtrl.EnableWindow(m_webServer);

	if (m_port < 80)
		m_port = 9999;

	UpdateData(0);
	SetFcOption();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSetting::SetFcOption()
{
	int		i = m_sections.GetCurSel();

	if (i < 0)
	{
		m_keySizeTagBtn.SetCheck(BST_UNCHECKED);
		m_contentSizeTagBtn.SetCheck(BST_UNCHECKED);
		m_addImage2FcBtn.SetCheck(BST_UNCHECKED);

		m_keySizeTagBtn.EnableWindow(false);
		m_contentSizeTagBtn.EnableWindow(false);
		m_addImage2FcBtn.EnableWindow(false);
		m_dictMode.SetWindowText("");
		this->GetDlgItem(IDC_RADIO1)->EnableWindow(false);
		this->GetDlgItem(IDC_RADIO2)->EnableWindow(false);
		return;
	}

	CString name;
	int		v = 0;

	m_sections.GetText(i, name);
	m_contentSizeTagBtn.EnableWindow(true);
	m_addImage2FcBtn.EnableWindow(true);
	this->GetDlgItem(IDC_RADIO1)->EnableWindow(true);
	this->GetDlgItem(IDC_RADIO2)->EnableWindow(true);

	m_db->GetDbEnv(name, CONTENT_SIZE_TAG, v, 0);
	m_contentSizeTagBtn.SetCheck(v);
	m_db->GetDbEnv(name, ADD_IMAGE_TO_FC_TAG, v, 1);
	m_addImage2FcBtn.SetCheck(v);

	if (m_db->GetSort(name) == "DESC")
		m_sort = 1;
	else
		m_sort = 0;

	if (m_db->IsDictMode(name))
	{
		m_dictMode.SetWindowText("On");
		m_keySizeTagBtn.EnableWindow(false);
	}
	else
	{
		m_dictMode.SetWindowText("Off");
		m_keySizeTagBtn.EnableWindow(true);
	}

	m_db->GetDbEnv(name, KEY_SIZE_TAG, v, 0);
	m_keySizeTagBtn.SetCheck(v);

	UpdateData(0);
}

void CSetting::OnBnClickedAddSection()
{
	CCreateSection	input;

	int res = input.DoModal();
	if (res == IDCANCEL)
		return;
	
	if (input.m_name == "" || input.m_name[0] == '_' ||
		input.m_name[0] == '-')
	{
		MessageBox("Invalid name. ", "Error");
		return;
	}

	if (m_sections.FindStringExact(0, input.m_name) != -1)
	{
		MessageBox("Name already exists", "Error");
		return;
	}

	if (m_db->CreateDb(input.m_name, input.m_dictMode) == -1)
	{
		MessageBox("Faile to create db", "Error");
		return;
	}
	if (input.m_dictMode)
	{
		m_db->SetDbEnv(input.m_name, KEY_SIZE_TAG, 0);
	}

	LoadSections();
	m_sections.SetCurSel(m_sections.FindStringExact(0, input.m_name));
	SetFcOption();
}

void CSetting::OnBnClickedRemoveSection()
{
	int		sel = m_sections.GetCurSel();

	if (sel < 0)
		return;

	int		res = MessageBox("Do you really want to remove?", "Warning",
				MB_YESNO);

	if (res == IDNO)
		return;

	CString s;

	m_sections.GetText(sel, s);
	m_db->DeleteDb(s);

	LoadSections();
	SetFcOption();
}

void CSetting::LoadSections()
{
	m_sections.ResetContent();
	CStringArray	sections;
	m_db->GetDbList(sections);
	for(int i = 0; i < (int) sections.GetSize(); i++)
	{
		m_sections.AddString(sections[i]);
	}
}


void CSetting::OnOK()
{
	UpdateData();

	if (m_makingWorkingFolder && m_workingFolder == "")
	{
		MessageBox("You have to set working folder", "Error");
		return;
	}

	SetEnv(PROFILE_SECTION, USE_WORKING_FOLDER, m_makingWorkingFolder);
	if (m_makingWorkingFolder)
	{
		if (m_workingFolder.Right(1) != '\\')
			m_workingFolder += '\\';

		SetEnv(PROFILE_SECTION, WORKING_FOLDER, m_workingFolder);
	}
	int old = GetEnv(PROFILE_SECTION, LAUNCH_WEBSERVER, 0);
	
	if (old != m_webServer)
	{
		MessageBox("Webserver setting has changed.  You have to restart this program", 
			"Warning");
	}

	SetEnv(PROFILE_SECTION, LAUNCH_WEBSERVER, m_webServer);
	if (m_webServer)
	{
		old = GetEnv(PROFILE_SECTION, WEBSERVER_PORT, 9999);
		if (old != m_port)
		{
			MessageBox("Webserver setting has changed.  You have to restart this program", 
				"Warning");
		}
		SetEnv(PROFILE_SECTION, WEBSERVER_PORT, m_port);
	}
	SetEnv(PROFILE_SECTION, CACHE_SIZE, m_cacheSize);

//    SetEnv(PROFILE_SECTION, URL, m_url);
    SetEnv(PROFILE_SECTION, HIDE_TAGS, m_hideTags);

    SetEnv(PROFILE_SECTION, PUSH_INTERVAL, m_pushInterval);
    SetEnv(PROFILE_SECTION, ALLOW_IMMEDIATE_PUSH, m_allowImmediatePush);

	CDialog::OnOK();
}

void CSetting::OnBnClickedSetWorkingFolder()
{
	BROWSEINFO bi;
	memset(&bi, 0, sizeof(bi));

	bi.ulFlags   = BIF_USENEWUI;
	bi.hwndOwner = this->m_hWnd;
	bi.lpszTitle = "Working Folder";

	// must call this if using BIF_USENEWUI

	::OleInitialize(NULL);

	// Show the dialog and get the itemIDList for the 
	// selected folder.

	LPITEMIDLIST pIDL = ::SHBrowseForFolder(&bi);
	if(pIDL != NULL)
	{
		// Create a buffer to store the path, then 
		// get the path.

		char buffer[_MAX_PATH] = {'\0'};
		if(::SHGetPathFromIDList(pIDL, buffer) != 0)
		{
			// Set the string value.
			m_workingFolder = buffer;
			UpdateData(0);
		}
	    // free the item id list
		CoTaskMemFree(pIDL);
	}

	::OleUninitialize();
}

void CSetting::OnBnClickedMakeWorkingFolder()
{
	UpdateData();

	m_workingFolderCtrl.EnableWindow(m_makingWorkingFolder);
	m_setWorkingFolderBtn.EnableWindow(m_makingWorkingFolder);
}

void CSetting::OnBnClickedLaunchWebserver()
{
	UpdateData();

	m_portCtrl.EnableWindow(m_webServer);
}

void CSetting::OnBnClickedClearCache()
{
	m_db->ClearCache();
}

void CSetting::OnLbnSelchangeDatabase()
{
	SetFcOption();
}

void CSetting::OnBnClickedKeySizeTag()
{
	CString name;
	int		i = m_sections.GetCurSel();
	int		v;

	if (i < 0)
		return;

	m_sections.GetText(i, name);
	v = m_keySizeTagBtn.GetCheck();
	m_db->SetDbEnv(name, KEY_SIZE_TAG, v);
}

void CSetting::OnBnClickedContentSizeTag()
{
	CString name;
	int		i = m_sections.GetCurSel();
	int		v;

	if (i < 0)
		return;

	m_sections.GetText(i, name);
	v = m_contentSizeTagBtn.GetCheck();
	m_db->SetDbEnv(name, CONTENT_SIZE_TAG, v);
}

void CSetting::OnBnClickedAddImageToFc()
{
	CString name;
	int		i = m_sections.GetCurSel();
	int		v;

	if (i < 0)
		return;

	m_sections.GetText(i, name);
	v = m_addImage2FcBtn.GetCheck();
	m_db->SetDbEnv(name, ADD_IMAGE_TO_FC_TAG, v);
}

void CSetting::OnBnClickedRadio1()
{
	CString name;
	int		i = m_sections.GetCurSel();

	m_sections.GetText(i, name);
	UpdateData(1);
	if (m_sort == 0)
		m_db->SetSort(name, "ASC");
	else
		m_db->SetSort(name, "DESC");
}

