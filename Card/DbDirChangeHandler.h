#pragma once

#include "DirectoryChanges.h"

#define WM_FILE_NOTIFY		(WM_USER+151)

class CCardView;

class DbDirChangeHandler : public CDirectoryChangeHandler
{
public:
	DbDirChangeHandler() : m_view(NULL) { m_lastTime = time(0); }

	void SetView(CCardView *view);

	void On_FileAdded(const CString & strFileName);
	void On_FileRemoved(const CString & strFileName);
	void On_FileModified(const CString & strFileName);
	void On_FileNameChanged(const CString & strOldFileName, const CString & strNewFileName);

	CString			m_lastFile;
	time_t			m_lastTime;
	CCardView *		m_view;
};
