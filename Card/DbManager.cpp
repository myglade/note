#include "StdAfx.h"
#include "DbManager.h"
#include "CardView.h"

using namespace json;


CDbManager *CDbManager::m_dbManager_instance;

void GetDbImage(RtfImageList &src, RtfImageList &dst, DbRtfImages &res)
{
    RtfImageList::iterator  it1, it2;

    for(it1 = src.begin(); it1 != src.end(); it1++)
    {
//        if (dst.size() == 0)
//            continue;

        it2 = dst.find(it1->second.id);
        if (it2 == dst.end())
        {
            res.remove[it1->second.id] = it1->second;
            continue;
        }
        if (it1->second.width == it2->second.width &&
            it1->second.height == it2->second.height)
            res.nochange[it1->second.id] = it1->second;
        else
            res.update[it1->second.id] = it2->second;
    }

    for(it2 = dst.begin(); it2 != dst.end(); it2++)
    {
        if (src.size() > 0)
        {
            it1 = src.find(it2->first);
            if (it1 == src.end())
                res.add[it2->second.id] = it2->second;
        }
        else
            res.add[it2->second.id] = it2->second;
    }
}

CDbManager::CDbManager(void)
{
	m_curDb = NULL;
	CDbManager::m_dbManager_instance = this;
    m_isDirty = FALSE;
    m_dbListener = NULL;
}

CDbManager::~CDbManager(void)
{
	DbMap::iterator		iter;

	for(iter = m_dbMap.begin(); iter != m_dbMap.end(); iter++)
	{
		delete iter->second;
	}
}

int CDbManager::Init(LPCTSTR orgFolder, LPCTSTR workingFolder)
{
	CFileFind		finder;
	DbInfo *		info;
	BOOL			bWorking;

	if (orgFolder == NULL)
		return -1;

	m_orgDbFolder = orgFolder;
	if (m_orgDbFolder.Right(1) != '\\')
		m_orgDbFolder += '\\';

	SHCreateDirectoryEx(NULL, m_orgDbFolder, NULL);
	if (workingFolder != NULL && *workingFolder)
	{
		m_workingDbFolder = workingFolder;
		if (m_workingDbFolder.Right(1) != '\\')
			m_workingDbFolder += '\\';

		m_dbFolder = m_workingDbFolder;
		m_useWorkingFolder = TRUE;
		SHCreateDirectoryEx(NULL, m_dbFolder, NULL);
	}
	else
	{
		m_dbFolder = m_orgDbFolder;
		m_useWorkingFolder = FALSE;
	}

	if (m_useWorkingFolder == FALSE)
	{
		bWorking = finder.FindFile(m_dbFolder + _T("*.db"));
		while (bWorking)
		{
			bWorking = finder.FindNextFile();

			info = new DbInfo();
			info->name = finder.GetFileTitle();
			info->path = finder.GetFilePath();
			info->status = DB_STATUS_CLOSE;
            info->orgDbPath = (LPCTSTR)m_orgDbFolder;
			finder.GetLastWriteTime(info->lastWriteTime);
			if (OpenDb(info) == -1)
			{
				delete info;
				continue;
			}
			m_dbMap[info->name] = info;
		} 
	}
	else
	{
		bWorking = finder.FindFile(m_orgDbFolder + _T("*.db"));
		while (bWorking)
		{
			bWorking = finder.FindNextFile();

			info = new DbInfo();
			info->name = finder.GetFileTitle();
			info->path = m_workingDbFolder + finder.GetFileName();
			info->orgPath = finder.GetFilePath();
			info->status = DB_STATUS_CLOSE;
            info->orgDbPath = (LPCTSTR) m_orgDbFolder;
			finder.GetLastWriteTime(info->lastWriteTime);
			
			CopyFile(finder.GetFilePath(), info->path.c_str());

			if (OpenDb(info) == -1)
			{
				delete info;
				continue;
			}
			m_dbMap[info->name] = info;
		} 
	}

	CString		db = GetEnv(PROFILE_SECTION, "db", "");

	if (db == "")
		ChangeDb(0);
	else if (ChangeDb(db) == -1)
	{
		ChangeDb(0);
	}

	return 0;
}

void CDbManager::SyncW2O(DbInfo *info)
{
	if (info == NULL || m_useWorkingFolder == FALSE || info->orgPath == "")
		return;

	CFileFind		finder;

	BOOL bWorking = finder.FindFile(info->path.c_str());
	if (bWorking == FALSE)
	{
		DeleteFile(info->orgPath.c_str());
		return;
	}

	bWorking = finder.FindNextFile();
	finder.GetLastWriteTime(info->lastWriteTime);

	CopyFile(info->path.c_str(), info->orgPath.c_str());
}

BOOL CDbManager::SyncO2W(LPCTSTR file)
{
	if (file == NULL || m_useWorkingFolder == FALSE)
		return FALSE;

	CFileFind			finder;
	DbMap::iterator		iter;
	DbInfo	*			info;

	BOOL bWorking = finder.FindFile(file);
	if (bWorking == FALSE)
	{
		if (DeleteDbByFile(file) == 0)
			return TRUE;
		return FALSE;
	}
	bWorking = finder.FindNextFile();
	iter = m_dbMap.find((LPCTSTR) finder.GetFileTitle());
	if (iter == m_dbMap.end())
	{
		info = new DbInfo();
		info->name = finder.GetFileTitle();
		info->path = m_workingDbFolder + finder.GetFileName();
		info->orgPath = finder.GetFilePath();
		info->status = DB_STATUS_CLOSE;
		finder.GetLastWriteTime(info->lastWriteTime);
		CopyFile(finder.GetFilePath(), info->path.c_str());

		if (OpenDb(info) == -1)
		{
			delete info;
			return FALSE;
		}
		m_dbMap[info->name] = info;
	}
	else
	{
		CTime		time;

		info = iter->second;
        
		finder.GetLastWriteTime(time);
		if (time == info->lastWriteTime)
        {
            TRACE("Same file. Skip\n");
			return FALSE;
        }
		if (time < info->lastWriteTime)
		{
		//	MessageBox(NULL, "Original file is out of date than working file",
		//		"Warning", MB_OK);
            TRACE("Original file is out of date than working file\n");
			return FALSE;
		}
        
		CloseDb(info, TRUE);
		CopyFile(finder.GetFilePath(), info->path.c_str());

		if (OpenDb(info) == -1)
		{
			return FALSE;
		}

		if (m_curDb != info)
			return FALSE;

		m_curDb->db->UpdateCurData();
		TRACE("Load DB = %s\n", m_curDb->name);
	}

	return TRUE;
}

int CDbManager::CreateDb(LPCTSTR name, BOOL dictMode)
{
	DbMap::iterator		iter;
	DbInfo	*			info;

	if (name == NULL)
		return -1;

	iter = m_dbMap.find(name);
	if (iter != m_dbMap.end())
		return -1;
	
	info = new DbInfo();
	info->name = name;
	info->path = m_dbFolder + name + ".db";
	info->orgPath = m_orgDbFolder + name + ".db";
	info->status = DB_STATUS_CLOSE;

	if (OpenDb(info, dictMode) == -1)
	{
		delete info;
		return -1;
	}

	m_dbMap[info->name] = info;
	SyncW2O(info);

	return 0;
}

BOOL CDbManager::IsDictMode(DbInfo *info)
{
	if (info == NULL)
		return FALSE;

	return info->db->IsDictMode();
}

BOOL CDbManager::IsDictMode(LPCTSTR name)
{
	DbMap::iterator		iter;

	if (name == NULL)
		return FALSE;

	iter = m_dbMap.find(name);
	if (iter == m_dbMap.end())
		return FALSE;

	DbInfo *info = iter->second;

	return info->db->IsDictMode();
}

void CDbManager::GetDbList(CStringArray &dbList)
{
	DbMap::iterator		iter;

	dbList.RemoveAll();
	for(iter = m_dbMap.begin(); iter != m_dbMap.end(); iter++)
	{
		dbList.Add(iter->first.c_str());
	}
}

int CDbManager::DeleteDb(LPCTSTR name)
{
	DbMap::iterator		iter;
	DbInfo *			info;
	BOOL				changeDb = FALSE;

	if (name == NULL)
		return -1;

	iter = m_dbMap.find(name);
	if (iter == m_dbMap.end())
		return -1;

	info = iter->second;
	if (info == m_curDb)
		m_curDb = NULL;

	CString path = info->path.c_str();
	if (m_useWorkingFolder)
	{
		::DeleteFile(info->orgPath.c_str());
	}

	delete info;
	::DeleteFile(path);

	m_dbMap.erase(iter);
	if (m_curDb == NULL)
		ChangeDb(0);

	return 0;
}

int CDbManager::DeleteDbByFile(CString name)
{
	int					i;

	i = name.ReverseFind('\\');
	if (i == -1)
		return -1;

	name = name.Mid(i + 1, name.GetLength() - 3 - i - 1);
	return DeleteDb(name);
}

int CDbManager::ChangeDb(LPCTSTR name)
{
	DbMap::iterator		iter;

	if (name == NULL)
		return -1;

	if (m_curDb && name == m_curDb->name)
		return 0;

	iter = m_dbMap.find(name);
	if (iter == m_dbMap.end())
		return -1;

	DbInfo *info = iter->second;
	if (OpenDb(info) == -1)
	{
		return -1;
	}

	CloseDb(m_curDb, FALSE);
	m_curDb = info;
	SetEnv(PROFILE_SECTION, "db", info->name.c_str());

    int category = GetEnv(info->name.c_str(), "category", -1);
	int bookmark = GetEnv(info->name.c_str(), "bookmark", 0);
	int index = LoadIndex(info->name.c_str(), category, bookmark);
	info->db->SetStatus(index, category, bookmark);

    m_curDb->db->UpdateCurData();
    m_isDirty = TRUE;

	return 0;
}

int CDbManager::ChangeDb(int index)
{
	DbMap::iterator		iter;
	int					i = 0;

	if (index < 0 || index > (int) m_dbMap.size() - 1)
		return -1;

	for(iter = m_dbMap.begin(); iter != m_dbMap.end(); iter++, i++)
	{
		if (i == index)
			break;
	}
	if (iter == m_dbMap.end())
		return -1;

	DbInfo *info = iter->second;

    if (info == m_curDb)
        return 0;

	if (OpenDb(info) == -1)
	{
		return -1;
	}
    
	if (m_curDb)
		CloseDb(m_curDb, FALSE);
	m_curDb = info;
	SetEnv(PROFILE_SECTION, "db", info->name.c_str());
	m_curDb->db->UpdateCurData();
    m_isDirty = TRUE;

	return 0;
}

int CDbManager::OpenDb(DbInfo *info, BOOL dictMode)
{
	if (!info)
		return -1;
	
	if (info->status == DB_STATUS_OPEN)
		return 0;

	if (info->db == NULL)
	{
		info->db = new CDb();
		if (info->db == NULL)
			return -1;
	}

    int res = info->db->Init(info->orgDbPath.c_str(), info->path.c_str(), info->name.c_str(), dictMode);
	if (res == -1)
	{
		delete info->db;
		info->db = NULL;
		return -1;
	}
    else if (res == 1)  // upgrade
        ClearCache();

	int category = GetEnv(info->name.c_str(), "category", -1);
	int bookmark = GetEnv(info->name.c_str(), "bookmark", 0);
	int index = LoadIndex(info->name.c_str(), category, bookmark);

	info->db->SetStatus(index, category, bookmark);
//	info->db->SetSort("ASC", 0);
	info->status = DB_STATUS_OPEN;

	return 0;
}

int CDbManager::CloseDb(LPCTSTR name, BOOL closeDb)
{
	DbMap::iterator		iter;

	if (name == NULL)
		return -1;

	iter = m_dbMap.find(name);
	if (iter == m_dbMap.end())
		return -1;

	DbInfo *info = iter->second;
	CloseDb(info, closeDb);

	return 0;
}

void CDbManager::CloseDb(DbInfo *info, BOOL closeDb)
{
	if (!info || info->status == DB_STATUS_CLOSE)
		return;

	if (closeDb)
	{
		info->status = DB_STATUS_CLOSE;
		delete info->db;
		info->db = NULL;

		return;
	}

	return;
}

int CDbManager::LoadIndex(LPCTSTR dbName, int category, int bookmark)
{
	CString		key;

	ASSERT(dbName);
	key.Format("%d-%d-index", 
		category,
		bookmark);
	
	return GetEnv(dbName, key, 0);
}

/*
int CDbManager::LoadIndex(DbInfo *info)
{
	CString		key;

	ASSERT(info);
	key.Format("%d-%d-index", 
		info->db->GetCurCategory(),
		info->db->GetCurBookmarkMode());
	
	return GetEnv(info->db->GetName(), key, 0);
}
*/
int CDbManager::SaveIndex(DbInfo *info)
{
	CString		key;

	ASSERT(info);
	key.Format("%d-%d-index", 
		info->db->GetCurCategory(),
		info->db->GetCurBookmarkMode());

	return SetEnv(info->db->GetName(), key, info->db->GetItemIndex());
}

int CDbManager::GetContentInfoAsJson(std::string &s)
{
	CNames				names;
	Object				item;
	Array				list;
	Object				obj;
	DbMap::iterator		iter;

	obj.Clear();

	for(iter = m_dbMap.begin(); iter != m_dbMap.end(); iter++)
	{
		DbInfo *info = iter->second;
		
		if (OpenDb(info) == -1)
			continue;

		names.RemoveAll();	
		info->db->GetCategoryList(names);
		list.Clear();
		item.Clear();
		for(int j = 0; j < names.GetCount(); j++)
		{
			item["name"] = String((LPCTSTR) names[j].name);
			item["id"] = Number(names[j].id);
			list.Insert(item);
		}
		obj[info->name]["category"] = list;

		list.Clear();
		item.Clear();
		names.RemoveAll();
		info->db->GetTagList(names);
		for(int j = 0; j < names.GetCount(); j++)
		{
			item["name"] = String((LPCTSTR) names[j].name);
			item["id"] = Number(names[j].id);
			list.Insert(item);
		}
		obj[info->name]["tag"] = list;

	}
	std::stringstream stream;
	Writer::Write(obj, stream);
	s = stream.str();

	return 0;
}

int CDbManager::GetDbListAsJson(std::string &s)
{
	Object				obj;
	Array				list;
	DbMap::iterator		iter;

	s = "{}";

	for(iter = m_dbMap.begin(); iter != m_dbMap.end(); iter++)
	{
		list.Insert(String(iter->first.c_str()));
	}
	obj["section"] = list;

	std::stringstream stream;
	Writer::Write(obj, stream);
	s = stream.str();

	return 0;
}

int CDbManager::Query(StringMapArray &result, int contentType, LPCTSTR db, int category, 
		int bookmark, CUIntArray &tagList, int tagSearchMode, LPCTSTR sort, int index, int count)
{
	DbMap::iterator		iter;

	if (db == NULL)
		return -1;
	iter = m_dbMap.find(db);
	if (iter == m_dbMap.end())
		return -1;

	if (OpenDb(iter->second) == -1)
		return - 1;

	return iter->second->db->Query(result, contentType, category, bookmark, tagList, tagSearchMode,
		sort, index, count);
}

int CDbManager::Query(StringMapArray &result, LPCTSTR db, LPCTSTR id, 
                      BOOL useCategory, CString sort, int &index)
{
	DbMap::iterator		iter;

	if (db == NULL)
		return -1;
	iter = m_dbMap.find(db);
	if (iter == m_dbMap.end())
		return -1;

	if (OpenDb(iter->second) == -1)
		return - 1;

	return iter->second->db->Query(result, id, useCategory, sort, index);
}

int CDbManager::GetSummaryAsJson(std::string &s, LPCTSTR db, int category, 
		int bookmark, CUIntArray &tagList, int tagSearchMode, LPCTSTR sort, 
        CString lineFeed, bool stressHead, int start, int count)
{
	DbMap::iterator		iter;

	if (db == NULL)
		return -1;
	iter = m_dbMap.find(db);
	if (iter == m_dbMap.end())
		return -1;

	if (OpenDb(iter->second) == -1)
		return - 1;

	return iter->second->db->GetSummaryAsJson(s, category, 
		bookmark, tagList, tagSearchMode, sort, lineFeed, stressHead, start, count);
}

int CDbManager::UpdateTag(LPCTSTR db, unsigned int id, 
				   int mask, int bookmark, CUIntArray &tag)
{
	DbMap::iterator		iter;

	if (db == NULL)
		return -1;
	iter = m_dbMap.find(db);
	if (iter == m_dbMap.end())
		return -1;
	
	if (OpenDb(iter->second) == -1)
		return - 1;

	int res = iter->second->db->UpdateTag(id, mask, bookmark, tag);
	if (res == 0)
    {
		SyncW2O(iter->second);
        if (GetCurDbName() == db)
        {
            if (m_dbListener)
            {
                m_dbListener->DbNotify(DB_TAG_CHANGE);
            }
        }
    }
    m_isDirty = TRUE;

    return res;
}

int CDbManager::SetDbEnv(DbInfo *info, LPCTSTR key, LPCTSTR value)
{
	if (info == NULL)
		return -1;

	int  res = info->db->SetEnv(key, value);
	if (res < 0)
	{
		return -1;
	}

	SyncW2O(info);
	return res;
}

int CDbManager::SetDbEnv(DbInfo *info, LPCTSTR key, int value)
{
	if (info == NULL)
		return -1;

	int  res = info->db->SetEnv(key, value);
	if (res < 0)
	{
		return -1;
	}

	SyncW2O(info);
	return res;
}

int CDbManager::GetDbEnv(DbInfo *info, LPCTSTR key, CString &value, LPCTSTR defaultValue)
{
	if (info == NULL)
		return -1;

	int  res = info->db->GetEnv(key, value, defaultValue);
	if (res < 0)
	{
		return -1;
	}

	return res;
}

int CDbManager::GetDbEnv(DbInfo *info, LPCTSTR key, int & value, int defaultValue)
{
	if (info == NULL)
		return -1;

	int  res = info->db->GetEnv(key, value, defaultValue);
	if (res < 0)
	{
		return -1;
	}

	return res;
}

int CDbManager::SetDbEnv(LPCTSTR name, LPCTSTR key, LPCTSTR value)
{
	DbMap::iterator		iter;

	if (name == NULL)
		return -1;

	iter = m_dbMap.find(name);
	if (iter == m_dbMap.end())
		return -1;

	DbInfo *info = iter->second;

	int  res = info->db->SetEnv(key, value);
	if (res < 0)
	{
		return -1;
	}

	SyncW2O(info);
	return res;
}

int CDbManager::SetDbEnv(LPCTSTR name, LPCTSTR key, int value)
{
	DbMap::iterator		iter;

	if (name == NULL)
		return -1;

	iter = m_dbMap.find(name);
	if (iter == m_dbMap.end())
		return -1;

	DbInfo *info = iter->second;

	int  res = info->db->SetEnv(key, value);
	if (res < 0)
	{
		return -1;
	}

	SyncW2O(info);
	return res;
}

int CDbManager::GetDbEnv(LPCTSTR name, LPCTSTR key, CString &value, LPCTSTR defaultValue)
{
	DbMap::iterator		iter;

	if (name == NULL)
		return -1;

	iter = m_dbMap.find(name);
	if (iter == m_dbMap.end())
		return -1;

	DbInfo *info = iter->second;

	int  res = info->db->GetEnv(key, value, defaultValue);
	if (res < 0)
	{
		return -1;
	}

	return res;
}

int CDbManager::GetDbEnv(LPCTSTR name, LPCTSTR key, int & value, int defaultValue)
{
	DbMap::iterator		iter;

	if (name == NULL)
		return -1;

	iter = m_dbMap.find(name);
	if (iter == m_dbMap.end())
		return -1;

	DbInfo *info = iter->second;

	int  res = info->db->GetEnv(key, value, defaultValue);
	if (res < 0)
	{
		return -1;
	}

	return res;
}

void CDbManager::SetSort(LPCTSTR sort)
{
	if (m_curDb == NULL)
		return;

	if (GetSort() == sort)
		return;

	m_curDb->db->SetSort(sort, 0);
    m_isDirty = TRUE;
}

void CDbManager::SetSort(LPCTSTR name, LPCTSTR sort)
{
	DbMap::iterator		iter;

	if (name == NULL)
		return;

	iter = m_dbMap.find(name);
	if (iter == m_dbMap.end())
		return;

	DbInfo *info = iter->second;
	if (info->db->GetSort() == sort)
		return;

	info->db->SetSort(sort, 0);
    m_isDirty = TRUE;
    SyncW2O(m_curDb);
}

CString CDbManager::GetSort()
{
	if (m_curDb == NULL)
		return NULL;

	return m_curDb->db->GetSort();
}

CString CDbManager::GetSort(LPCTSTR name)
{
	DbMap::iterator		iter;

	if (name == NULL)
		return "";

	iter = m_dbMap.find(name);
	if (iter == m_dbMap.end())
		return "";

	DbInfo *info = iter->second;

	return info->db->GetSort();
}

void CDbManager::SetBookmark(BOOL set)
{
	if (m_curDb == NULL)
		return;

	m_curDb->db->SetBookmark(set);
    m_isDirty = TRUE;
    SyncW2O(m_curDb);
}

void CDbManager::BackupEnv()
{
	if (m_curDb)
		m_oldDb = m_curDb->name.c_str();
	else
		m_oldDb = "";
}

void CDbManager::RestoreEnv()
{
	if (m_oldDb == "" && m_dbMap.size() > 0)
	{
		ChangeDb(0);
		return;
	}

	DbMap::iterator		iter;

	iter = m_dbMap.find((LPCTSTR) m_oldDb);
	if (iter == m_dbMap.end())
	{
		m_curDb = NULL;
		if (m_dbMap.size() > 0)
			ChangeDb(0);

		return;
	}

	DbInfo *info = iter->second;
	if (OpenDb(info) == -1)
	{
		m_curDb = NULL;
		return;
	}
}

int CDbManager::CopyFile(LPCTSTR oldFile, LPCTSTR newFile)
{
	if (!oldFile || !newFile)
		return -1;

	int		delay = 1000;
	for(int i = 0; i < 10; i++)
	{
		if (::CopyFile(oldFile, newFile, FALSE))
			return 0;
	
		Sleep(delay);
		delay *= 2;
	}

	return -1;
}

void CDbManager::Compact()
{
	DbMap::iterator		iter;
	DbInfo *			info;

	for(iter = m_dbMap.begin(); iter != m_dbMap.end(); iter++)
	{
		info = iter->second;
		if (OpenDb(info) == -1)
			continue;

		info->db->Compact();
		SyncW2O(info);
	}
}
//////////////////////////////////////////////////////
int CDbManager::CreateCategory(CString &name)
{ 
	if (m_curDb == NULL)
		return -1;
	if (m_curDb->db->CreateCategory(name) == -1)
		return -1;

	SyncW2O(m_curDb);
	return 0;
}

int CDbManager::GetCategoryList(CNames &names)
{ 
	if (m_curDb == NULL)
		return -1;
	return m_curDb->db->GetCategoryList(names); 
}

int CDbManager::GetCategoryList(LPCTSTR db, CNames &names)
{ 
    DbMap::iterator		iter;

	if (db == NULL)
		return -1;
	iter = m_dbMap.find(db);
	if (iter == m_dbMap.end())
		return -1;

	if (OpenDb(iter->second) == -1)
		return -1;

	return iter->second->db->GetCategoryList(names); 
}

int CDbManager::DeleteCategory(CString &name)
{ 
	if (m_curDb == NULL)
		return -1;
	if (m_curDb->db->DeleteCategory(name) == -1)
		return -1;

	SyncW2O(m_curDb);
	return 0;
}

int CDbManager::RenameCategory(CString &old, CString &newName)
{ 
	if (m_curDb == NULL)
		return -1;
	if (m_curDb->db->RenameCategory(old, newName) == -1)
		return -1; 

	SyncW2O(m_curDb);
	return 0;
}

int CDbManager::CreateTag(CString &name)
{ 
	if (m_curDb == NULL)
		return -1;
	if (m_curDb->db->CreateTag(name) == -1)
		return -1;

	SyncW2O(m_curDb);
	return 0;
}

int CDbManager::GetTagList(CNames &names)
{ 
	if (m_curDb == NULL)
		return -1;
	return m_curDb->db->GetTagList(names); 
}

int CDbManager::DeleteTag(CString &name)
{ 
	if (m_curDb == NULL)
		return -1;
	if (m_curDb->db->DeleteTag(name) == -1)
		return -1;

	SyncW2O(m_curDb);
	return 0;
}

int CDbManager::RenameTag(CString &old, CString &newName)
{ 
	if (m_curDb == NULL)
		return -1;
	if (m_curDb->db->RenameTag(old, newName) == -1)
		return -1;

	SyncW2O(m_curDb);
	return 0;
}
void CDbManager::ChangeCategory(int category)
{
	ASSERT(m_curDb);
	SetEnv(m_curDb->db->GetName(), "category", category);

	int index = LoadIndex(m_curDb->db->GetName(), category, m_curDb->db->GetCurBookmarkMode());
	m_curDb->db->ChangeCategory(category, index); 
    m_isDirty = TRUE;
}

void CDbManager::ChangeBookmarkMode(int mode)
{
	ASSERT(m_curDb);
	SetEnv(m_curDb->db->GetName(), "bookmark", mode);
	int index = LoadIndex(m_curDb->db->GetName(), 
						m_curDb->db->GetCurCategory(), mode);
	m_curDb->db->ChangeBookmarkMode(mode, index); 
    m_isDirty = TRUE;
}

void CDbManager::ChangeTag(CUIntArray &tag)
{
	ASSERT(m_curDb);
	m_curDb->db->ChangeTag(tag); 
    m_isDirty = TRUE;
}

void CDbManager::ChangeTagSearchMode(int mode)
{
	ASSERT(m_curDb);
	m_curDb->db->ChangeTagSearchMode(mode); 
    m_isDirty = TRUE;
}


//////////////////////////////////////////////////////

CString CDbManager::GetCurDbName() 
{ 
	if (m_curDb == NULL)
		return "";
	return m_curDb->db->GetName(); 
}
int CDbManager::GetCurCategory() 
{	
	if (m_curDb == NULL)
		return -1;
	return m_curDb->db->GetCurCategory(); 
}
int CDbManager::GetCurBookmarkMode() 
{ 
	if (m_curDb == NULL)
		return -1;
	return m_curDb->db->GetCurBookmarkMode(); 
}

CString CDbManager::GetCurId() 
{ 
	if (m_curDb == NULL)
		return "";

	CString		s;

	s.Format("%u", m_curDb->db->GetCurId());
	return s; 
}

CUIntArray &CDbManager::GetCurTag() 
{ 
	ASSERT(m_curDb);
	return m_curDb->db->GetCurTag(); 
}

BOOL CDbManager::GetFileFromCache(LPCTSTR name, CString &s)
{
	CFile		file;
	char		tempPath[512];
	CString		path;
	char *		buf;

	GetTempPath(512, tempPath);
	path.Format("%sfc\\%s", tempPath, name);
	if (file.Open(path, CFile::modeRead) == FALSE)
		return FALSE;

	buf = new char[(int) file.GetLength() + 1];
	if (buf == NULL)
		return FALSE;

	file.Read(buf, (int)file.GetLength());
	buf[(int)file.GetLength()] = 0;
	s = buf;
	file.Close();

	delete buf;
	TRACE("CACHED : %s\n", name);

	return TRUE;
}

BOOL CDbManager::SaveFileToCache(LPCTSTR name, CString &s)
{
	CFile			file;
	char			tempPath[512];
	CString			path;
	CFileFind		finder;
	CString			fileName;
	ULONGLONG		totalSize = 0;
	CacheMap		cache;
	CTime			accessTime;
	CacheInfo		info;
	ULONGLONG		cacheSize;

	GetTempPath(512, tempPath);
	path.Format("%sfc\\", tempPath);
	SHCreateDirectoryEx(NULL, path, NULL);
	fileName = path + name;

	BOOL bWorking = finder.FindFile(path + "*.rtf");

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		info.path = finder.GetFilePath();
		info.size = finder.GetLength();
		totalSize += info.size;

		finder.GetLastAccessTime(accessTime);
		cache[accessTime.GetTime()] = info;
	}

	cacheSize = GetEnv(PROFILE_SECTION, CACHE_SIZE, 30) * 1024 * 1024;
	if (totalSize > cacheSize)
	{
		CacheMap::iterator		it;

		for(it = cache.begin(); it != cache.end(); it++)
		{
			DeleteFile(it->second.path);
			totalSize -= it->second.size;
			if (totalSize < cacheSize)
				break;
		}
	}

	if (file.Open(fileName, CFile::modeCreate | CFile::modeWrite) == FALSE)
		return FALSE;

	file.Write((LPCTSTR) s, s.GetLength());
	file.Close();

	return TRUE;
}

void CDbManager::DeleteFromCache(LPCTSTR name)
{
	CFileFind		finder;
	char			tempPath[512];
	CString			path;

	GetTempPath(512, tempPath);
	path.Format("%sfc\\%s", tempPath, name);
	BOOL bWorking = finder.FindFile(path);
	if (bWorking == FALSE)
		return;

	finder.FindNextFile();
	DeleteFile(finder.GetFilePath());
}

void CDbManager::ClearCache()
{
	CFileFind		finder;
	char			tempPath[512];
	CString			path;

	GetTempPath(512, tempPath);
	path.Format("%sfc\\*.rtf", tempPath);
	BOOL bWorking = finder.FindFile(path);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		DeleteFile(finder.GetFilePath());
	}
}

void CDbManager::GetCurKey(CString &str)
{
	RtfImageList &	list = m_curDb->db->GetCurKeyImageList();

	if (list.size() == 0)
	{
		str = m_curDb->db->GetCurKey();
		return;
	}
	
	CString		name;

	name.Format("%s_0_%u_%u.rtf", m_curDb->db->GetName(),
		m_curDb->db->GetCurId(), m_curDb->db->GetCurMTime());
	if (GetFileFromCache(name, str) == TRUE)
		return;

	if (CRtfParser::AttachImage(m_curDb->db->GetCurKey(), list, str) == -1)
	{
		str = "";
		return;
	}
	SaveFileToCache(name, str);

	return;
}

void CDbManager::GetCurContent(CString &str) 
{
	RtfImageList &	list = m_curDb->db->GetCurContentImageList();

	if (list.size() == 0)
	{
		str = m_curDb->db->GetCurContent();
		return;
	}

	CString		name;

	name.Format("%s_1_%u_%u.rtf", m_curDb->db->GetName(),
		m_curDb->db->GetCurId(), m_curDb->db->GetCurMTime());
	if (GetFileFromCache(name, str) == TRUE)
		return;

	if (CRtfParser::AttachImage(m_curDb->db->GetCurContent(), list, str) == -1)
	{
		str = "";
		return;
	}
	SaveFileToCache(name, str);

	return;
}

RtfImageList &CDbManager::GetCurKeyImageList() 
{ 
	ASSERT(m_curDb);
	return m_curDb->db->GetCurKeyImageList(); 
}
RtfImageList &CDbManager::GetCurContentImageList() 
{ 
	ASSERT(m_curDb);
	return m_curDb->db->GetCurContentImageList(); 
}

int CDbManager::AddItem(int category, int bookmark, CUIntArray &tag, 
	CString &key, CString &content)
{
	RtfImageList	    keyImage;
	RtfImageList	    contentImage;
	CString				finalContent;
	CString				finalKey;
    RtfImageList        orgImage;

	ASSERT(m_curDb);
	if (m_curDb->db->IsDictMode())
	{
		RemoveTab(content);
		finalKey = key;
	}
	else
	{
		RemoveTab(key);
		RemoveTab(content);
		if (CRtfParser::DetachImage(key, orgImage, keyImage, finalKey) == -1)
			return -1;
	}

	if (CRtfParser::DetachImage(content, orgImage, contentImage, finalContent) == -1)
		return -1;

	if (m_curDb->db->CreateItem(category, bookmark, tag,
		finalKey, finalContent, keyImage, contentImage) == -1)
		return -1;

	SyncW2O(m_curDb);
	SaveIndex(m_curDb);
    m_isDirty = TRUE;
	return 0;
}

int CDbManager::UpdateItem(int mask, int category, int bookmark, 
	CUIntArray &tag, CString &key, CString &content)
{
	DbRtfImages	        keyImage;
	DbRtfImages	        contentImage;
	CString				finalContent;
	CString				finalKey;
    RtfImageList        image;

	if (mask & KEY_UPDATE)
	{
		if (m_curDb->db->IsDictMode() == FALSE)
		{
			RemoveTab(key);
            image.clear();
			if (CRtfParser::DetachImage(key, m_curDb->db->GetCurKeyImageList(), image, finalKey) == -1)
				return -1;

            GetDbImage(m_curDb->db->GetCurKeyImageList(), image, keyImage);
		}
		else
			finalKey = key;

	}

	if (mask & VALUE_UPDATE)
	{
		RemoveTab(content);

        image.clear();
		if (CRtfParser::DetachImage(content, m_curDb->db->GetCurContentImageList(), image, finalContent) == -1)
			return -1;

        GetDbImage(m_curDb->db->GetCurContentImageList(), image, contentImage);
    }

	CString		name;

	if (m_curDb->db->GetCurKeyImageList().size() > 0)
	{
		name.Format("%s_0_%u_%u.rtf", m_curDb->db->GetName(),
			m_curDb->db->GetCurId(), m_curDb->db->GetCurMTime());
		DeleteFromCache(name);
	}
	if (m_curDb->db->GetCurContentImageList().size() > 0)
	{
		name.Format("%s_1_%u_%u.rtf", m_curDb->db->GetName(),
			m_curDb->db->GetCurId(), m_curDb->db->GetCurMTime());
		DeleteFromCache(name);
	}

	if (m_curDb->db->UpdateItem(mask, category, bookmark, 
		tag, finalKey, finalContent, keyImage, contentImage) == -1)
		return -1;

	SyncW2O(m_curDb);
	SaveIndex(m_curDb);
    m_isDirty = TRUE;
	return 0;
}

int CDbManager::DeleteItem()
{
	ASSERT(m_curDb);
	if (m_curDb->db->DeleteItem() == -1)
		return -1;

	SyncW2O(m_curDb);
	SaveIndex(m_curDb);
    m_isDirty = TRUE;
	return 0;
}

int CDbManager::GetCurItemCategory()
{
	ASSERT(m_curDb);
	return m_curDb->db->GetCurItemCategory(); 
}

int CDbManager::GetCurItemBookmark()
{
	ASSERT(m_curDb);
	return m_curDb->db->GetCurItemBookmark(); 
}

CUIntArray &CDbManager::GetCurItemTag()
{
	ASSERT(m_curDb);
	return m_curDb->db->GetCurItemTag(); 
}

int CDbManager::GetItemIndex()
{
	if (m_curDb == NULL)
		return -1;
	return m_curDb->db->GetItemIndex(); 
}

int CDbManager::GetItemCount()
{
	if (m_curDb == NULL)
		return 0;
	return m_curDb->db->GetItemCount(); 
}

void CDbManager::Prev()
{
	ASSERT(m_curDb);
	m_curDb->db->Prev(); 
	SaveIndex(m_curDb);
}

void CDbManager::Next()
{
	ASSERT(m_curDb);
	m_curDb->db->Next(); 
	SaveIndex(m_curDb);
}

void CDbManager::First()
{
	ASSERT(m_curDb);
	m_curDb->db->First(); 
	SaveIndex(m_curDb);
}

void CDbManager::Last()
{
	ASSERT(m_curDb);
	m_curDb->db->Last(); 
	SaveIndex(m_curDb);
}

void CDbManager::Goto(int index)
{
	ASSERT(m_curDb);
	m_curDb->db->Goto(index); 
	SaveIndex(m_curDb);
}

int CDbManager::GotoByitime(unsigned int itime)
{
	ASSERT(m_curDb);
	if (m_curDb->db->GotoByitime(itime) < 0)
		return -1;

	SaveIndex(m_curDb);
	return 0;
}

int CDbManager::GotoByid(LPCTSTR db, LPCTSTR id, BOOL useCategory)
{
	if (ChangeDb(db) == -1)
		return -1;

	if (m_curDb->db->GotoByid(id, useCategory) < 0)
		return -1;

    if (useCategory) {
        int category = GetCurCategory();
        SetEnv(m_curDb->db->GetName(), "category", category);
    }
	SaveIndex(m_curDb);
	return 0;
}

void CDbManager::ToFirst()
{
	ASSERT(m_curDb);
	m_curDb->db->ToFirst(); 
	SaveIndex(m_curDb);
    m_isDirty = true;
}

void CDbManager::ToPrev()
{
	ASSERT(m_curDb);
	m_curDb->db->ToPrev(); 
	SaveIndex(m_curDb);
    m_isDirty = true;
}

void CDbManager::ToNext()
{
	ASSERT(m_curDb);
	m_curDb->db->ToNext(); 
	SaveIndex(m_curDb);
    m_isDirty = true;
}

int CDbManager::MoveTo(std::vector<std::pair<int, int>> &src, int dst)
{
	ASSERT(m_curDb);
	if (m_curDb->db->MoveTo(src, dst) == -1)
        return -1;

	SaveIndex(m_curDb);
    m_isDirty = true;
    return 0;
}

CString &CDbManager::GetError() const
{
	ASSERT(m_curDb);
	return m_curDb->db->GetError(); 
}

int CDbManager::LoadDataFromCurrentSetting(CppSQLite3Query &q, int start, int count) 
{
	ASSERT(m_curDb);
	return m_curDb->db->LoadDataFromCurrentSetting(q, start, count); 
}

int CDbManager::LoadImage(unsigned int id, int owner, RtfImageList &imageList)
{
	ASSERT(m_curDb);
	return m_curDb->db->LoadImage(id, owner, imageList); 
}

int CDbManager::GetSummary(StringMapArray &result, BOOL categoryName,
    CString lineFeed, bool stressHead)
{
	ASSERT(m_curDb);
	return m_curDb->db->GetSummary(result, categoryName, lineFeed, stressHead); 
}

int CDbManager::GetSummary(LPCTSTR db, int category, StringMapArray &result, 
    BOOL categoryName, CString lineFeed, bool stressHead)
{	
    DbMap::iterator		iter;

	if (db == NULL)
		return -1;
	iter = m_dbMap.find(db);
	if (iter == m_dbMap.end())
		return -1;

	if (OpenDb(iter->second) == -1)
		return -1;

    CUIntArray          tagList;
	return iter->second->db->GetSummary(category, 
        0, tagList, 0, result, categoryName, lineFeed, stressHead); 
}

int CDbManager::SetNumbering()
{
    ASSERT(m_curDb);
    m_isDirty = true;

    return m_curDb->db->SetNumbering();
}

int CDbManager::ExportToHtml(BOOL exportContent)
{
    ASSERT(m_curDb);

    return m_curDb->db->ExportToHtml(exportContent);
}

int CDbManager::SearchKey(StringMapArray &result, CString &key,
    CString lineFeed, bool stressHead)
{
	if (m_curDb == NULL)
		return 0;
	return m_curDb->db->SearchKey(result, key, lineFeed, stressHead); 
}

BOOL CDbManager::isDirty(BOOL reset)
{
	BOOL    dirty = m_isDirty;

    if (reset)
        m_isDirty = FALSE;

    return dirty;
}

void CDbManager::SetDbListener(CDbListener * listener)
{
    if (listener == NULL)
        return;

    m_dbListener = listener;
}

void CDbManager::Refresh()
{
	if (m_curDb == NULL)
		return;
	return m_curDb->db->UpdateCurData(); 
}

/*
void CDbManager::NormailizeDicText(CString &s, CString key)
{
	int			n;
	CString		f;

    if (!m_curDb->db->IsDictMode())
        return;

    ReplaceWithKey(s, key);
	n = s.Find("\\fswiss\\fcharset2 Bookshelf Symbol 5");
	if (n != -1)
	{
		f = s.Mid(n - 3, 3); 
		s.Replace("\\fswiss\\fcharset2 Bookshelf Symbol 5", "\\fswiss Arial");
		n = s.Replace(f + " R\\", f + ",\\");
	}

	n = s.Find("\\fswiss\\fcharset2 Wingdings");
	if (n != -1)
	{
		f = s.Mid(n - 3, 3); 
		s.Replace("\\fswiss\\fcharset2 Wingdings", "\\fswiss Arial");
		n = s.Replace(f + "\\fs22\\'f3", f + "\\fs22<>");
		n = s.Replace(f + "\\fs22\\'f0", f + "\\fs22>>");
	}
	n = s.Replace("\\'a2\\'b9", f + " ->");
}
*/

void CDbManager::NormailizeDicText(CString &s, CString key)
{
	int			n;
	CString		f;

    if (!m_curDb->db->IsDictMode())
        return;

    ReplaceWithKey(s, key);
    return;

	n = s.Find("\\fswiss\\fcharset2 Bookshelf Symbol 5");
	if (n != -1)
	{
		f = s.Mid(n - 3, 3); 
		s.Replace("\\fswiss\\fcharset2 Bookshelf Symbol 5", "\\fswiss Arial");
		n = s.Replace(f + " R\\", f + ",\\");
	}

	n = s.Find("\\fswiss\\fcharset2 Wingdings");
	if (n != -1)
	{
		f = s.Mid(n - 3, 3); 
		s.Replace("\\fswiss\\fcharset2 Wingdings", "\\fswiss Arial");
		n = s.Replace(f + "\\fs22\\'f3", f + "\\fs22<>");
		n = s.Replace(f + "\\fs22\\'f0", f + "\\fs22>>");
	}
	n = s.Replace("\\'a2\\'b9", f + " ->");
}

void CDbManager::ReplaceWithKey(CString &s, CString key)
{
	int			n;
	CString		eng;

    /*
	n = s.Find("Times New Roman");
	if (n != -1)
	{
//		n = s.Replace("Times New Roman", "\\'b1\\'bc\\'b8\\'b2");
		n = s.Replace("Times New Roman", "Arial");
	}
    */
    s.Replace(" R\\", ",\\");
    s.Replace("\\fs22\\'f3", "\\fs22<>");
    s.Replace("\\fs22\\'f0", "\\fs22>>");
    n = s.Replace("\\'a2\\'b9", " ->");

    CString key1 = "\\b " + key + "\\b0";
    n = s.Replace("\\'a1\\'ad\\lang1033\\f0 R", "\\f0" + key);

	n = s.Find("\\'a1\\'ad");
	if (n != -1)
	{
		n = s.Find("\\f", n);
		eng = s.Mid(n, 3);
	}

    key = "\\b\\i " + key + "\\i0\\b0";

//	n = s.Replace("~", key);
	n = s.Replace("\\'a1\\'ad", eng + key);

//	s.Replace("\\tab", " ");
}

void CDbManager::RemoveTab(CString &src)
{
	const char *s = src;
	const char *last = s + src.GetLength();
	const char *p;
	bool		tag;
	int			space;

	p = strstr(s, "\\tab");
	if (!p)
	{
		return;
	}
	char *temp = new char[src.GetLength() * 2];
	char *		dst = temp;
	
	while(p)
	{
		memcpy(dst, s, (int) (p - s));
		dst += (int) (p - s);

		char *t1;
		t1 = dst - 1;

		while(t1 != temp)
		{
			if (*t1 == ' ' || *t1 == '\n')
			{
				tag = false;
				break;
			}
			if (*t1 == '\\' && *(t1 - 1) != '\\')
			{
				tag = true;
				break;
			}
			--t1;
		}
		if (t1 == temp)
		{
			ASSERT("Error");
			delete temp;
			return;
		}
		if (tag)
		{
			if (*(p + 4) == '\\' && *(p + 5) != '\\')
				space = TAB_SPACE + 1;
			else
				space = TAB_SPACE;
		}
		else
		{
			if (*(p + 4) == ' ' || (*(p + 4) == '\\' && *(p + 5) == '\\'))
				space = TAB_SPACE - 1;
			else
				space = TAB_SPACE;
		}
		for(int i = 0; i < space; i++)
		{
			*dst++ = ' ';
		}

		s = p + 4;
		p = strstr(s, "\\tab");
	}

	if (s < last)
	{
		while(*s)
			*dst++ = *s++;
	}
	*dst = 0;
	src = temp;
	delete temp;
}
