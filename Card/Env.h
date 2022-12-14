#pragma once


#define PROFILE_SECTION			"Settings"
#define KEY_WND_HEIGHT			"key_wnd_height"

#define DICT_KEY_HEIGHT			25
#define KEY_SIZE_TAG			"key-size-tag"
#define CONTENT_SIZE_TAG		"content-size-tag"
#define ADD_IMAGE_TO_FC_TAG		"add-image-fc-tag"
#define WORKING_FOLDER			"working_folder"
#define USE_WORKING_FOLDER		"use_working_folder"
#define LAUNCH_WEBSERVER		"launch_webserver"
#define WEBSERVER_PORT			"webserver_port"
#define CACHE_SIZE				"cache_size"
#define URL                     "url"
#define ALLOW_IMMEDIATE_PUSH    "allow_immediate_push"
#define PUSH_INTERVAL           "push_interval"
#define HIDE_TAGS               "hide_tags"

#define WINDOW_SETTING_SECTION	"Settings"

#define LINE_FEED               ""

#ifdef _DEBUG
#define REGKEY "Software\\Flash Card Test\\Card\\"
#else
#define REGKEY "Software\\Flash Card\\Card\\"
#endif

inline int SetEnv(LPCTSTR section, LPCTSTR key, LPCTSTR value)
{
	return AfxGetApp()->WriteProfileString(section, key, value);
}

inline int SetEnv(LPCTSTR section, LPCTSTR key, int value)
{
	return AfxGetApp()->WriteProfileInt(section, key, value);
}

inline int GetEnv(LPCTSTR section, LPCTSTR key, int defaultValue)
{
	return AfxGetApp()->GetProfileInt(section, key, defaultValue);
}

inline CString GetEnv(LPCTSTR section, LPCTSTR key, LPCTSTR defaultValue)
{
	return AfxGetApp()->GetProfileString(section, key, defaultValue);
}

inline BOOL ReadWindowPlacement(LPCTSTR window, WINDOWPLACEMENT *pwp)
{
	CString         strBuffer;
	int             nRead ;

	strBuffer = AfxGetApp()->GetProfileString(WINDOW_SETTING_SECTION, window);
	if ( strBuffer.IsEmpty() )  
		return FALSE;

	nRead = sscanf(strBuffer, "%u,%u,%d,%d,%d,%d,%d,%d,%d,%d",
					&pwp->flags, &pwp->showCmd,
					&pwp->ptMinPosition.x, &pwp->ptMinPosition.y,
					&pwp->ptMaxPosition.x, &pwp->ptMaxPosition.y,
					&pwp->rcNormalPosition.left,  &pwp->rcNormalPosition.top,
					&pwp->rcNormalPosition.right, &pwp->rcNormalPosition.bottom);
	if ( nRead != 10 )  
		return FALSE;
	pwp->length = sizeof(WINDOWPLACEMENT);

	return TRUE;
}

inline void WriteWindowPlacement(LPCTSTR window, WINDOWPLACEMENT *pwp)
{
	char        szBuffer[sizeof("-32767")*8 + sizeof("65535")*2];
	int         max = 1;
	CString     s;

	sprintf(szBuffer, "%u,%u,%d,%d,%d,%d,%d,%d,%d,%d",
					pwp->flags, pwp->showCmd,
					pwp->ptMinPosition.x, pwp->ptMinPosition.y,
					pwp->ptMaxPosition.x, pwp->ptMaxPosition.y,
					pwp->rcNormalPosition.left, pwp->rcNormalPosition.top,
					pwp->rcNormalPosition.right, pwp->rcNormalPosition.bottom);
	AfxGetApp()->WriteProfileString(WINDOW_SETTING_SECTION, window, szBuffer);
}

    
inline int SetSetting(char *key, const char *str)
{
	HKEY		hKey;
	DWORD		disp;
   
	if (!key || !str || RegOpenKeyEx(HKEY_CURRENT_USER, REGKEY, 0, KEY_ALL_ACCESS, &hKey) != 0)
	{
		if (RegCreateKeyEx(HKEY_CURRENT_USER, REGKEY, 0, NULL, REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS, NULL, &hKey, &disp) != 0)
			return -1;
	}
	
	RegSetValueEx(hKey, key, 0, REG_SZ, (BYTE *)str, strlen(str));

	RegFlushKey(hKey);
	RegCloseKey(hKey);
	return -1;
}

inline int GetSetting(char *key, CString &value)
{
	HKEY		hKey;
	char		temp[4096];
	DWORD		type;
	DWORD		len;

	if (!key || RegOpenKeyEx(HKEY_CURRENT_USER, REGKEY, 0, KEY_ALL_ACCESS, &hKey) != 0)
	{
		return -1;
	}

	type = REG_SZ; len = 4096;
	if (RegQueryValueEx(hKey, key, NULL, &type, (BYTE *) temp, &len) == ERROR_SUCCESS)
	{
		temp[len] = 0;
		value = temp;
		RegCloseKey(hKey);
		return 0;
	}

	RegCloseKey(hKey);
	return -1;
}
