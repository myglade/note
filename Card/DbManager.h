#pragma once

#include "Db.h"
#include "Env.h"

#define DB_STATUS_OPEN		1
#define DB_STATUS_CLOSE		0

struct DbInfo
{
	DbInfo() 
	{
		db = NULL;
		status = DB_STATUS_CLOSE;
	}
	~DbInfo()
	{
		if (db) delete db;
	}
	std::string		name;
	std::string		path;
	std::string		orgPath;
    std::string     orgDbPath;
	int				status;
	CDb *			db;
	CTime			lastWriteTime;
};

struct CacheInfo
{
	CString			path;
	ULONGLONG		size;
};

typedef std::map<std::string, DbInfo *> DbMap;
typedef std::map<__time64_t, CacheInfo> CacheMap;

#define DB_TAG_CHANGE       0

class CDbListener
{
public:
    virtual void DbNotify(int msg) {};
};

class CDbManager
{
public:
	CDbManager(void);
	~CDbManager(void);

	int Init(LPCTSTR orgFolder, LPCTSTR workingFolder = NULL);
	int CreateDb(LPCTSTR name, BOOL dictMode = FALSE);
	void GetDbList(CStringArray &sections);
	int DeleteDb(LPCTSTR name);
	int DeleteDbByFile(CString name);
	int OpenDb(DbInfo *info, BOOL dictMode = FALSE);
	int ChangeDb(int index);
	int ChangeDb(LPCTSTR name);
	void CloseDb(DbInfo *info, BOOL closeDb = FALSE);
	int CloseDb(LPCTSTR name, BOOL closeDb);
	BOOL IsDictMode(DbInfo *info);
	BOOL IsDictMode(LPCTSTR name);

	DbInfo *GetCurDb() { return m_curDb; }

	int CreateCategory(CString &name);
	int GetCategoryList(CNames &names);
    int GetCategoryList(LPCTSTR db, CNames &names);
	int DeleteCategory(CString &name);
	int RenameCategory(CString &old, CString &newName);
	int CreateTag(CString &name);
	int GetTagList(CNames &names);
	int DeleteTag(CString &name);
	int RenameTag(CString &old, CString &newName);

	void ChangeCategory(int category);
	void ChangeBookmarkMode(int mode);
	void ChangeTag(CUIntArray &tag);
    void ChangeTagSearchMode(int mode);
	void Prev();
	void Next();
	void First();
	void Last();
	void Goto(int index);
	int GotoByitime(unsigned int itime);
	int GotoByid(LPCTSTR db, LPCTSTR id, BOOL useCategory);
	void ToFirst();
	void ToPrev();
	void ToNext();
    int MoveTo(CUIntArray &src, int dst);

	CString GetCurId();
	CString GetCurDbName();
	int GetCurCategory();
	int GetCurBookmarkMode();
	CUIntArray &GetCurTag();
	void GetCurKey(CString &str);
	void GetCurContent(CString &str) ;
	RtfImageList &GetCurKeyImageList();
	RtfImageList &GetCurContentImageList();

    void SetBookmark(BOOL set);
	void SetSort(LPCTSTR sort);
	void SetSort(LPCTSTR name, LPCTSTR sort);
	CString GetSort();
	CString GetSort(LPCTSTR name);

	int AddItem(int category, int bookmark, CUIntArray &tag, 
		CString &key, CString &value);
	int UpdateItem(int mask, int category, int bookmark, 
		CUIntArray &tag, CString &key, CString &value);
	int DeleteItem();

	int GetCurItemCategory();
	int GetCurItemBookmark();
	CUIntArray &GetCurItemTag();
	int GetItemIndex();
	int GetItemCount();
	int LoadDataFromCurrentSetting(CppSQLite3Query &q, int start, int count);
	int LoadImage(unsigned int id, int owner, RtfImageList &imageList);

	CString &GetError() const;

	int GetSummary(StringMapArray &result, BOOL categoryName,
                    CString lineFeed, bool stressHead);
    int GetSummary(LPCTSTR db, int category,  
                    StringMapArray &result, BOOL categoryName,
                    CString lineFeed, bool stressHead);

	int SaveIndex(DbInfo *info);
//	int LoadIndex(DbInfo *info);
	int LoadIndex(LPCTSTR dbName, int category, int bookmark);
	void BackupEnv();
	void RestoreEnv();
	int SetDbEnv(DbInfo *info, LPCTSTR key, LPCTSTR value);
	int SetDbEnv(DbInfo *info, LPCTSTR key, int value);
	int GetDbEnv(DbInfo *info, LPCTSTR key, CString &value, LPCTSTR defaultValue="");
	int GetDbEnv(DbInfo *info, LPCTSTR key, int & value, int defaultValue=0);
	int SetDbEnv(LPCTSTR name, LPCTSTR key, LPCTSTR value);
	int SetDbEnv(LPCTSTR name, LPCTSTR key, int value);
	int GetDbEnv(LPCTSTR name, LPCTSTR key, CString &value, LPCTSTR defaultValue="");
	int GetDbEnv(LPCTSTR name, LPCTSTR key, int & value, int defaultValue=0);

	int SaveAsHtml(CWnd *wnd);
	int SaveAsJson();

    int SetNumbering();
    int ExportToHtml(BOOL exportContent = TRUE);

	static CDbManager *GetInstance() { return m_dbManager_instance; }

	int GetDbListAsJson(std::string &s);
	int Query(StringMapArray &result, int contentType, LPCTSTR db, int category, 
			int bookmark, CUIntArray &tagList, int tagSearchMode, LPCTSTR sort, int index, int count = 1);
    int Query(StringMapArray &result, LPCTSTR db, LPCTSTR id, BOOL useCategory, CString sort, int &index);

	int SearchKey(StringMapArray &result, CString &key,
            CString lineFeed, bool stressHead);
	int GetContentInfoAsJson(std::string &s);
	int GetSummaryAsJson(std::string &s, LPCTSTR db, int category, 
		int bookmark, CUIntArray &tagList, int tagSearchMode, LPCTSTR sort, 
        CString lineFeed, bool stressHead, int start = -1, int count = -1);
	int UpdateTag(LPCTSTR db, unsigned int id, 
				   int mask, int bookmark, CUIntArray &tag);

	void SyncW2O(DbInfo *info);
	BOOL SyncO2W(LPCTSTR file);
	int CopyFile(LPCTSTR oldFile, LPCTSTR newFile);
	void Compact();

	BOOL GetFileFromCache(LPCTSTR name, CString &s);
	BOOL SaveFileToCache(LPCTSTR name, CString &s);
	void ClearCache();
	void DeleteFromCache(LPCTSTR name);

    BOOL isDirty(BOOL reset = TRUE);
    void Refresh();

    void SetDbListener(CDbListener *);

	////////////////////
	void NormailizeDicText(CString &s, CString key);
	void ReplaceWithKey(CString &s, CString key);
	void RemoveTab(CString &src);

	static CDbManager *		m_dbManager_instance;

private:
	DbMap				m_dbMap;
	DbInfo *			m_curDb;
	CString 			m_oldDb;
	BOOL				m_useWorkingFolder;
	CString				m_orgDbFolder;
	CString				m_workingDbFolder;
	CString				m_dbFolder;
    BOOL                m_isDirty;
    CDbListener *       m_dbListener;
};
