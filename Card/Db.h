#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <set>
#include <utility>

#include "sqlite/CppSQLite3.h"
#include "RtfParser/RtfParser.h"

#include "json/reader.h"
#include "json/writer.h"
#include "json/elements.h"

#include "Util.h"

#define NON_CATEGORY	"(NONE)"

#define CONTENT_TYPE_KEY	0
#define CONTENT_TYPE_VALUE	1
#define CONTENT_TYPE_BOTH	2

#define CATEGORY_UPDATE		(1 << 0)
#define BOOKMARK_UPDATE		(1 << 1)
#define KEY_UPDATE			(1 << 2)
#define VALUE_UPDATE		(1 << 3)
#define TAG_UPDATE			(1 << 4)
#define LEVEL_UPDATE		(1 << 5)

using namespace std;

struct CName
{
	int			id;
	CString		name;
};


typedef CArray<CName, CName>				CNames;
typedef std::map<std::string, std::string>	StringMap;
typedef std::vector<StringMap>				StringMapArray;

namespace Db
{
	struct IdNameMap
	{
		int			id;
		CString		name;
	};
	typedef std::vector<IdNameMap>		IdNameMapList;

	struct Page
	{
		int			mask;

		int			pageId;
		int			seq;
		int			sectionId;
		int			level;
		CString		title;
		CString		content;
		CString		titleText;
		CString		contentText;
		CUIntArray	tags;
	};

}

class CDb
{
public:
	CDb(void);
	virtual ~CDb(void);

    virtual int Init(LPCTSTR db_path, LPCTSTR file, LPCTSTR dbName, BOOL dictMode = FALSE);
	LPCTSTR GetName() { return m_dbName; }

	virtual int CreateTable();

	virtual int CreateCategory(LPCTSTR name, int insertBefore = -1);
	virtual int DeleteCategory(LPCTSTR name);
	virtual int GetCategoryList(CNames &names);
	virtual int RenameCategory(LPCTSTR old, LPCTSTR newName);

	virtual int CreateTag(LPCTSTR name, bool set = false);
	virtual int DeleteTag(LPCTSTR name);
	virtual int GetTagList(CNames &names);
	virtual int RenameTag(LPCTSTR old, LPCTSTR newName);

	virtual void ChangeCategory(int category, int index);
	virtual void ChangeBookmarkMode(int mode, int index);
	virtual void ChangeTag(CUIntArray &tag);
	virtual void ChangeTagSearchMode(int mode);
	virtual void SetSort(LPCTSTR sort, int index);
	virtual CString& GetSort() { return m_sort; }
	virtual void SetBookmark(BOOL set);
	
	BOOL IsDictMode() { return m_dictMode; }

	virtual int CreateItem(int category, int bookmark, CUIntArray &tag,
					CString &key, CString &value, 
					RtfImageList &keyImageList,
					RtfImageList &contentImageList);
	virtual int UpdateItem(int mask, int category, int bookmark,
					CUIntArray &tag, CString &key, CString &value, 
					DbRtfImages &keyImageList,
					DbRtfImages &contentImageList);
	virtual int DeleteItem();
	virtual int LoadData(CppSQLite3Query &q, BOOL raw, int contentType, int category,
			int bookmark, CUIntArray &tagList, pair<int, int> user1, pair<int, int> user2,
            int tagSearchMode, LPCTSTR sort, int start, int count);
	virtual int LoadData(CppSQLite3Query &q, BOOL raw, int contentType,
			unsigned int id, unsigned int itime, LPCTSTR sort);
	virtual int SearchKey(CppSQLite3Query &q, CString key);
	virtual int LoadImage(unsigned int id, int owner, RtfImageList &imageList, bool loadActualData = false);
	virtual int LoadDataFromCurrentSetting(CppSQLite3Query &q, int start, int count);

	virtual void UpdateCurData();
	virtual void SetCategory(int cate);

	CString & GetCurSection() { return m_dbName; }
	int GetCurCategory() { return m_curCategory; }
	int GetCurBookmarkMode() { return m_curBookmarkMode; }
    int GetCurTagSearchMode() { return m_curTagSearchMode; }
	CUIntArray &GetCurTag() { return m_curTag;}

	unsigned int GetCurId() { return m_cur_id; }
	unsigned int GetCurMTime() { return m_cur_mtime; }
	unsigned int GetCurITime() { return m_cur_itime; }
	CString &GetCurKey() { return m_cur_key; }
	CString &GetCurContent() { return m_cur_content;}
	int GetCurItemCategory() { return m_cur_itemCategory;}
	int GetCurItemBookmark() { return m_cur_itemBookmark;}
	CUIntArray &GetCurItemTag() { return m_cur_itemTag; }
	RtfImageList &GetCurKeyImageList() { return m_cur_keyImageList; }
	RtfImageList &GetCurContentImageList() { return m_cur_contentImageList; }

	static int GetVersion(LPCTSTR file);
	int GetItemIndex() { return m_index; }

	int GetItemCount() { return m_cur_itemCount; }
	void Normalize(CString &src);

	virtual void Prev();
	virtual void Next();
	virtual void First();
	virtual void Last();
	virtual void Goto(int index);
	virtual int GotoByitime(unsigned int itime);
	virtual int GotoByid(LPCTSTR id, BOOL useCategory);
	virtual int ToFirst();
	virtual int ToPrev();
	virtual int ToNext();
	virtual int MoveTo(std::vector<std::pair<int, int>> &src, int dst);
	
//	virtual int SaveAsHtml(CWnd *wnd);
//	virtual int SaveAsJson();

	CString &GetError() { return m_error; }
	
	virtual void SetStatus(int index, int category, int bookmark);

	virtual int ExecuteSql(char *format, ...);
	virtual int ExecuteSql(CppSQLite3Query &q, char *format, ...);
	virtual int ExecuteSql2(CppSQLite3Query &q, const char *query);
	virtual void CreateTagField(CUIntArray &list, CString &tag, int mode);
	virtual void EncodeTagString(CUIntArray &tag, CString &tagStr);
	virtual void DecodeTagString(LPCTSTR tagStr, CUIntArray &tag);
	virtual void DecodeTagString(LPCTSTR tagStr, CNames &names);

	virtual int Query(StringMapArray &result, int contentType, int category,
			int bookmark, CUIntArray &tagList, pair<int, int> user1, pair<int, int> user2,
            int tagSearchMode, LPCTSTR sort, int index, int count = 1);
	virtual int Query(StringMapArray &result, LPCTSTR id, BOOL useCategory, CString sort, int &index);

	virtual int GetCategoryName(int id, CString &name);
	virtual int GetTagName(int id, CString &name);
	virtual int GetSummary(StringMapArray &result, BOOL categoryName,
                    CString lineFeed, bool stressHead);
	virtual int GetSummary(int category, int bookmark, CUIntArray &tags, int tagSearchMode,
                    StringMapArray &result, BOOL categoryName,
                    CString lineFeed, bool stressHead);
	virtual int GetSummaryAsJson(std::string &s, int category,
			int bookmark, CUIntArray &tagList, pair<int, int> user1, pair<int, int> user2, int tagSearchMode, LPCTSTR sort,
            CString lineFeed, bool stressHead, int maxLen, int start = -1, int count = -1);
	virtual int GetCategoryListAsJson(std::string &s);
	virtual int SearchKey(StringMapArray &result, CString &key,
            CString lineFeed, bool stressHead);
	virtual int GetTagListAsJson(std::string &s);
	virtual int UpdateTag(unsigned int id, int mask, int bookmark, CUIntArray &tag);
    virtual int UpdateUserFields( unsigned int id, int user1, int user2);

    int ExportToHtml(BOOL exportContent = TRUE);
    int SetNumbering();

	virtual int SetEnv(LPCTSTR key, LPCTSTR value);
	virtual int SetEnv(LPCTSTR key, int value);
	virtual int GetEnv(LPCTSTR key, CString &value, LPCTSTR defaultValue = "");
	virtual int GetEnv(LPCTSTR key, int & value, int defaultValue = 0);
	virtual void Compact();
	virtual int Upgrade();
	virtual int Upgrade2To3();
    virtual int Upgrade2To3inDictMode();
    virtual int DeleteTagFromData(int tagId);

    int Compress(const char *src, int srcLen, std::shared_ptr<BYTE> &dst, unsigned long &dstLen);
    int Decompress(const unsigned char *src, int srcLen, CString &dst);
    void CreateMediaPath(int did);
    void DeleteMediaPath(int did);
    void DeleteMedia(int did, int owner, RtfImage &info);

    int ExportMedia(int did, int owner, RtfImage &info);
    int LoadMedia(int did, int owner, RtfImage &info);
    int UpgradeImageDb();

    void reset_itime();
    void AddColumns();
    void Reindex();

protected:
	CppSQLite3DB		m_db;

	unsigned int		m_cur_id;
	unsigned int		m_cur_itime;
	unsigned int		m_cur_mtime;
	CString				m_cur_key;
	CString				m_cur_content;
	int					m_cur_itemCategory;
	int					m_cur_itemBookmark;
	CUIntArray			m_cur_itemTag;
	RtfImageList	    m_cur_keyImageList;
	RtfImageList	    m_cur_contentImageList;
	int					m_cur_itemCount;

	CString				m_dbName;
	int					m_curCategory;
	int					m_curBookmarkMode;
	CUIntArray			m_curTag;
    int                 m_curTagSearchMode;
	CString				m_sort;
	int					m_index;
	BOOL				m_dictMode;

	CString				m_error;
	Mutex				m_mutex;	
    int                 m_version;

	CString				m_db_path;
	CString				m_media_path;
};



/*
http://www.codeproject.com/KB/database/CppSQLite.aspx

open() and close() methods are self explanatory. SQLite does provide a mode argument 
to sqlite_open() but this is documented as having no effect, so is not provided for 
in CppSQLite.

execDML() is used to execute Data Manipulation Language (DML) commands such as create
/drop/insert/update/delete statements. It returns the number of rows affected. 
Multiple SQL statements separated by semi-colons can be submitted and executed all 
at once. Note: there is a potential problem with the way that CppSQLite returns 
the number of rows affected. If there are any other un-finalized() operations in progress 
the number of rows affected will be cumulative and include those from previous statements. 
So if this feature is important to you, you have to make sure that any CppSQLiteQuery and
CppSQLiteStatement objects that have not destructed yet have finalize() called on them 
before you execDML().

execQuery() is used to execute queries. The CppSQLiteQuery object is returned by value, 
as this frees the programmer from having to delete it.

execScalar() is an idea I got from ADO.NET. It is a shortcut for when you need to run 
a simple aggregate function, for example, "select count(*) from emp" or "select max(empno) 
from emp". It returns the value of the first field in the first row of the query result. 
Other columns and rows are ignored.

getTable() allows for the SQLite feature which can fetch a whole table in a single 
operation, rather than having to fetch one row at a time as with a query. Actually, 
subsets of table rows can be fetched by specifying a query with a where clause, but the 
whole result set is returned at once. Again, the CppSQLiteTable object is returned by 
value for convenience.

compileStatement() allows for the experimental SQLite pre-compiled SQL feature. 
See CppSQLiteStatement below.

SQLite is typeless, which means all fields are stored as strings. The one exception to 
this is the INTEGER PRIMARY KEY type, which allows an auto increment field, much like 
the SQL Server's identity columns. The lastRowId() function is used to determine the value
of the primary key from the last row inserted.

interrupt() is useful when multithreading, and allows one thread to interrupt an operation 
in progress on another thread.

setBusyTimeout() can also be useful when multithreading, and allows the programmer to 
dictate how long SQLite waits before returning SQLITE_BUSY if another thread has a lock on 
the database. The default value is 60 seconds, set when the database is opened.

The copy constructor and operator=() are made private, as it does not make sense to copy 
a CppSQLiteDB object.

Finally, the static method SQLiteVersion() returns the version number of the underlying 
SQLite DLL.

*/
