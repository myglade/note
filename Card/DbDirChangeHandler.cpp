#include "stdafx.h"
#include "DbDirChangeHandler.h"
#include "CardView.h"

void DbDirChangeHandler::On_FileAdded(const CString & strFileName)
{
	if (m_view == NULL) 
		return;

/*
	time_t		t = time(0);
	if (m_lastFile == strFileName && (t - m_lastTime < 10))
		return;

	m_lastTime = t;
*/
    m_lastFile = strFileName;
	CString *name = new CString(strFileName);
	TRACE("ADD %s\n", strFileName);
	m_view->PostMessage(WM_FILE_NOTIFY, 0, (LPARAM) name);
}
void DbDirChangeHandler::On_FileRemoved(const CString & strFileName)
{
	if (m_view == NULL) 
		return;
/*
	time_t		t = time(0);
	if (m_lastFile == strFileName && (t - m_lastTime < 10))
		return;

	m_lastTime = t;
*/
	m_lastFile = strFileName;
	CString *name = new CString(strFileName);
	TRACE("removed %s\n", strFileName);
	m_view->PostMessage(WM_FILE_NOTIFY, 0, (LPARAM) name);
}

void DbDirChangeHandler::On_FileModified(const CString & strFileName)
{
	if (m_view == NULL) 
		return;

    /*
	time_t		t = time(0);
	if (m_lastFile == strFileName && (t - m_lastTime < 10))
		return;

	m_lastTime = t;
    */
	m_lastFile = strFileName;
	CString *name = new CString(strFileName);
	TRACE("modified %s\n", strFileName);
	m_view->PostMessage(WM_FILE_NOTIFY, 0, (LPARAM) name);
}

void DbDirChangeHandler::On_FileNameChanged(const CString & strOldFileName, const CString & strNewFileName)
{
	if (m_view == NULL) 
		return;

	CString *oldName = new CString(strOldFileName);
	TRACE(_T("The file %s was RENAMED to %s\n"), strOldFileName, strNewFileName);
	m_view->PostMessage(WM_FILE_NOTIFY, 0, (LPARAM) oldName);

	CString *name = new CString(strNewFileName);
	m_view->PostMessage(WM_FILE_NOTIFY, 0, (LPARAM) name);
}

void DbDirChangeHandler::SetView(CCardView *view)
{
	m_view = view;
}