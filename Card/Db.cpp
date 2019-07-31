#include "StdAfx.h"
#include "Db.h"
#include <stdio.h>
#include <stdlib.h>
#include "RtfParser/HtmlListener.h"
#include "RtfParser/HtmlListener2.h"
#include "RtfParser/TextListener.h"
#include "RtfParser/NumberingListener.h"
#include "Util.h"
#include "Env.h"
#include "miniz.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CString g_html_template;

// declare 	lock before any variable declaration for real synchro
//
#define SORT	"ASC"
#define DEFAULT_RANGE {-1,-1}

using namespace json;

int compare( const void *arg1, const void *arg2 )
{
   /* Compare all of both strings: */
   return *((int *) arg1) - *((int *) arg2);
}

CDb::CDb(void)
{
	m_index = 0;
	m_cur_itemCount = 0;
}

CDb::~CDb(void)
{
	Lock		lock(m_mutex);

	ResetImageList(m_cur_keyImageList);
	ResetImageList(m_cur_contentImageList);
	m_db.close();
}

int CDb::Init(LPCTSTR db_path, LPCTSTR file, LPCTSTR dbName, BOOL dictMode)
{
	Lock		lock(m_mutex);
    int         res = 0;

	if (file == NULL)
		return -1;

    m_db_path = db_path;
    m_media_path.Format("%s_media", dbName);
    Util::CreateDirectory(m_media_path);

	m_dictMode = dictMode;
	m_error = "";
	try
	{
		m_db.open(file);
		m_dbName = dbName;
		
//		m_db.execDML("PRAGMA auto_vacuum = 1;");

		if (m_db.tableExists("env") == false)
		{
			m_db.execDML("CREATE TABLE env(key varchar(255) PRIMARY KEY, value varchar(255));");
		}

		m_index = 0;
		m_curCategory = -1;
		m_curBookmarkMode = 0;
		m_sort = "ASC";
        m_curTagSearchMode = 0; // AND mode

		if (m_db.tableExists("data") == false)
		{
			if (CreateTable() == -1)
				return -1;

            m_version = 4;
			SetEnv(KEY_SIZE_TAG, 0);
			SetEnv(CONTENT_SIZE_TAG, 0);
			SetEnv(ADD_IMAGE_TO_FC_TAG, 1);
			SetEnv("DictMode", m_dictMode);
			SetEnv("sort", "ASC");
            SetEnv("version", m_version);
		}
		else
		{
			GetEnv("DictMode", m_dictMode);
			GetEnv("sort", m_sort, "ASC");
			GetEnv("version", m_version, 1);

            if (m_version == 1)
            {
                if (Upgrade() == -1)
                {
                    MessageBox(NULL, "Fail to upgrade", "Error", MB_OK);
                }
                res = 1;
            }
			else if (m_version == 2)
			{
                auto r = 0;

                if (m_dictMode)
                    r = Upgrade2To3inDictMode();
                else
                    r = Upgrade2To3();

				if (r == -1)
				{
					MessageBox(NULL, "Fail to upgrade", "Error", MB_OK);
					return -1;
				}
				return 1;
			}
            else if (m_version == 3) {
                reset_itime();
                m_version = 4;
                SetEnv("version", m_version);
            }
            else if (m_version == 4) {
                AddColumns();
                m_version = 5;
                SetEnv("version", m_version);
            }
        }

    //    
	}
	catch(CppSQLite3Exception &ex)
	{
		m_error = ex.errorMessage();
		return -1;
	}
	return res;
}

int CDb::GetVersion(LPCTSTR file)
{
	if (file == NULL)
		return -1;

	CppSQLite3DB	db;
	CppSQLite3Query q;
	int				version = -1;

	try
	{
		db.open(file);
		q = db.execQuery("SELECT value FROM env WHERE key='version';");
		if (q.eof())
			return -1;

		version = q.getIntField(0);
	}
	catch(CppSQLite3Exception &)
	{
		return -1;
	}

	return version;
}

void CDb::Compact()
{
	Lock		lock(m_mutex);

	ExecuteSql("VACUUM");
}

void CDb::SetStatus(int index, int category, int bookmark)
{
	m_index = index;
	m_curCategory = category;
	m_curBookmarkMode = bookmark;
//	LoadData();
}

int CDb::SetEnv(LPCTSTR key, LPCTSTR value)
{
	Lock		lock(m_mutex);

	if (key == NULL || value == NULL)
		return -1;

	if (ExecuteSql("UPDATE env SET value='%s' WHERE key='%s';", 
		value, key) <= 0)
	{
		return ExecuteSql("INSERT INTO env VALUES ('%s', '%s');", 
		key, value);
	}

	return 0;
}

int CDb::SetEnv(LPCTSTR key, int value)
{
	CString		v;

	v.Format("%d", value);
	return SetEnv(key, v);
}

int CDb::GetEnv(LPCTSTR key, CString &value, LPCTSTR defaultValue)
{
	Lock			lock(m_mutex);
	CppSQLite3Query q;

	if (key == NULL)
		return -1;

	if (ExecuteSql(q, "SELECT value FROM env WHERE key='%s';", 
		key) == -1)
	{
		value = defaultValue;
		return SetEnv(key, value);
	}
	value = q.getStringField(0);
	return 0;
}

int CDb::GetEnv(LPCTSTR key, int & value, int defaultValue)
{
	Lock			lock(m_mutex);
	CppSQLite3Query q;

	if (key == NULL)
		return -1;

	if (ExecuteSql(q, "SELECT value FROM env WHERE key='%s';", 
		key) == -1)
	{
		value = defaultValue;
		return SetEnv(key, value);
	}

	value = q.getIntField(0);
	return 0;
}

int CDb::ExecuteSql(char *format, ...)
{
    va_list         va;
	char 			query[8192];

	va_start(va, format);
    vsprintf(query, format, va);
	va_end(va);

//	TRACE("%s\n", query);
	m_error = "";
	try
	{
		return m_db.execDML(query);
	}
	catch(CppSQLite3Exception &ex)
	{
		m_error = ex.errorMessage();
		return -1;
	}
}

int CDb::ExecuteSql(CppSQLite3Query &q, char *format, ...)
{
    va_list         va;
	char 			query[8192];

    va_start(va, format);
    vsprintf(query, format, va);
	va_end(va);

//	TRACE("%s\n", query);
	m_error = "";
	try
	{
		q = m_db.execQuery(query);
		if (q.eof())
			return -1;

		return 0;
	}
	catch(CppSQLite3Exception &ex)
	{
		m_error = ex.errorMessage();
		return -1;
	}
}

int CDb::ExecuteSql2(CppSQLite3Query &q, const char *query)
{
	if (!query)
		return -1;

//	TRACE("%s\n", query);
	m_error = "";
	try
	{
		q = m_db.execQuery(query);
		if (q.eof())
			return -1;

		return 0;
	}
	catch(CppSQLite3Exception &ex)
	{
		m_error = ex.errorMessage();
		return -1;
	}
}

int CDb::CreateTable()
{
	Lock		lock(m_mutex);

	// itime : insert time
	// mtime : modified time
	if (ExecuteSql(
		"CREATE TABLE data(id INTEGER PRIMARY KEY AUTOINCREMENT, itime INTEGER, mtime INTEGER, category INTEGER,"
		"bookmark INTEGER, tag VARCHAR(255), keyImage INTEGER, contentImage INTEGER, "
		"dictKey TEXT, key BLOB, content BLOB, keyCompressed, contentCompressed INTEGER, "
        "user1 INTEGER DEFAULT 0, user2 INTEGER DEFAULT 0);") == -1)
		return -1;

	if (ExecuteSql(
		"CREATE TABLE image(uid INTEGER PRIMARY KEY AUTOINCREMENT, did INTEGER, owner INTEGER, id INTEGER, "
		"width INTEGER, height INTEGER, owidth INTEGER, oheight INTEGER, type CHAR[4], data BLOB, "
		"FOREIGN KEY(did) REFERENCES data(id) ON UPDATE CASCADE ON DELETE CASCADE);") == -1)
		return -1;

	if (ExecuteSql("CREATE INDEX IDX_TAG on data(tag);") == -1)
	{
		return -1;
	}
    if (ExecuteSql("CREATE INDEX IDX_USER1 on data(user1);") == -1)
    {
        return -1;
    }
    if (ExecuteSql("CREATE INDEX IDX_USER2 on data(user2);") == -1)
    {
        return -1;
    }

    if (ExecuteSql("CREATE INDEX IDX_ITIME on data(itime);") == -1)
	{
		return -1;
	}
	if (ExecuteSql("CREATE INDEX IDX_CATEGORY on data(category);") == -1)
	{
		return -1;
	}
	if (ExecuteSql("CREATE INDEX IDX_DID on image(did);") == -1)
	{
		return -1;
	}
	if (ExecuteSql("CREATE TABLE category(id INTEGER PRIMARY KEY, name VARCHAR(255) UNIQUE COLLATE NOCASE);") == -1)
	{
		return -1;
	}

	if (ExecuteSql("CREATE TABLE tag(id INTEGER PRIMARY KEY, name VARCHAR(255) UNIQUE COLLATE NOCASE);") == -1)
	{
		return -1;
	}

	if (ExecuteSql("INSERT INTO category VALUES(0, '%s');", NON_CATEGORY) == -1)
	{
		return -1;
	}

	return 0;
}

int CDb::CreateCategory(LPCTSTR name, int insertBefore)
{
	Lock		lock(m_mutex);

	if (name == NULL || *name == 0 || CString(name) == NON_CATEGORY)
		return -1;

	return ExecuteSql("INSERT INTO category VALUES(NULL, '%s');", name);
}

int CDb::DeleteCategory(LPCTSTR name)
{
	Lock		lock(m_mutex);

	if (name == NULL || *name == 0 || CString(name) == NON_CATEGORY)
		return -1;

	return ExecuteSql("DELETE FROM category WHERE name='%s';", name);
}

int CDb::GetCategoryList(CNames &names)
{
	Lock			lock(m_mutex);
	CppSQLite3Query q;

	if (ExecuteSql(q, "SELECT * FROM category ORDER BY name ASC;") == -1)
		return -1;

	names.RemoveAll();
	while(!q.eof())
	{
		CName	name;

		name.id = q.getIntField(0);
		name.name = q.getStringField(1);
		names.Add(name);
		q.nextRow();
	}
	return 0;
}

int CDb::RenameCategory(LPCTSTR old, LPCTSTR newName)
{
	Lock		lock(m_mutex);

	if (old == NULL || newName == NULL || *newName == 0)
		return -1;

	return ExecuteSql("UPDATE category SET name='%s' WHERE name='%s';", newName, old);
}

int CDb::CreateTag(LPCTSTR name, bool set)
{
	Lock		lock(m_mutex);

	if (name == NULL || *name == 0)
		return -1;

    // find first available id
    CppSQLite3Query q;

	if (ExecuteSql(q, "SELECT id FROM tag ORDER BY id ASC;") == -1)
		return -1;

    int next = 1;
	while(!q.eof())
	{
        int id = q.getIntField(0);
        if (id != next)
            break;

        next++;
		q.nextRow();
	}

    if (ExecuteSql("INSERT INTO tag VALUES(%d, '%s');", next, name) == -1)
        return -1;

    if (set == false)
        return 0;

    if (ExecuteSql(q, "SELECT id,tag FROM data") == -1)
        return -1;

    CString query;

    while (!q.eof())
    {
        int     id = q.getIntField("id");
        CString tag = q.getStringField("tag");

        CUIntArray tagList;
        bool found = false;

        DecodeTagString(tag, tagList);

        for (int i = 0; i < tagList.GetCount(); i++) {
            if (tagList[i] == next) {
                found = true;
                break;
            }
        }

        if (found == false)
            tagList.Add(next);

        CString newTagStr;
        EncodeTagString(tagList, newTagStr);
        ExecuteSql("UPDATE data SET tag='%s' WHERE id=%u;", newTagStr, id);

        q.nextRow();
    }
}

int CDb::DeleteTag(LPCTSTR name)
{
	Lock		lock(m_mutex);

	if (name == NULL || *name == 0)
		return -1;

    CppSQLite3Query q;
	if (ExecuteSql(q, "SELECT id FROM tag WHERE name='%s';", name) == -1)
		return -1;

    int id = q.getIntField(0);
 	ExecuteSql("DELETE FROM tag WHERE name='%s';", name);

    return DeleteTagFromData(id);
}

int CDb::DeleteTagFromData(int tagId)
{
    CppSQLite3Query q;
	if (ExecuteSql(q, "SELECT id, tag FROM data;") == -1)
		return -1;

    int     count = 0;
	while(!q.eof())
	{
        int			curPos = 1;
        unsigned int uid = q.getIntField(0);
        CString     tagStr = q.getStringField(1);

        CString     resToken= tagStr.Tokenize(_T(" "), curPos);
        CString     s;
        CUIntArray  tag;
        BOOL        update = false;

	    while (resToken != _T(""))
	    {
		    s = resToken.Mid(1, resToken.GetLength() - 2);
		    int     id = atoi(s);

            if (id != tagId)
		        tag.Add(id);
            else
            {
                update = true;
            }
		    resToken = tagStr.Tokenize(_T(" "), curPos);
	    };   
        
        if (update) 
        {
            CString     newTagStr;

            EncodeTagString(tag, newTagStr);
            ExecuteSql("UPDATE data SET tag='%s' WHERE id=%u;", 
			    newTagStr, uid);
        }
        count++;
		q.nextRow();
	}
    TRACE("Total %d tags are updated\n", count);

    return 0;
}

int CDb::GetTagList(CNames &names)
{
	Lock			lock(m_mutex);
	CppSQLite3Query q;

	if (ExecuteSql(q, "SELECT * FROM tag ORDER BY name ASC;") == -1)
		return -1;

	names.RemoveAll();
	while(!q.eof())
	{
		CName	name;

		name.id = q.getIntField(0);
		name.name = q.getStringField(1);
		names.Add(name);
		q.nextRow();
	}
	return 0;
}

int CDb::RenameTag(LPCTSTR old, LPCTSTR newName)
{
	Lock		lock(m_mutex);

	if (old == NULL || newName == NULL ||  *newName == 0)
		return -1;

	return ExecuteSql("UPDATE tag SET name='%s' WHERE name='%s';", newName, old);
}

void CDb::ChangeCategory(int category, int index)
{
    char    query[1024];

	_sntprintf(query, 1024, "SELECT count(*) FROM category WHERE id=%d;", category);
	if (m_db.execScalar(query) == 0)
        m_curCategory = -1;
    else
	    m_curCategory = category;

	m_index = index;
	UpdateCurData();
}

void CDb::ChangeBookmarkMode(int mode, int index)
{
	if (mode != 0)
		m_curBookmarkMode = 1;
	else
		m_curBookmarkMode = 0;

	m_index = index;
	UpdateCurData();
}

void CDb::ChangeTag(CUIntArray &tag)
{
	m_curTag.RemoveAll();
	for(int i = 0; i < tag.GetCount(); i++)
		m_curTag.Add(tag[i]);
	UpdateCurData();
}

void CDb::ChangeTagSearchMode(int mode)
{
	if (mode != 0)
		m_curTagSearchMode = 1;
	else
		m_curTagSearchMode = 0;
    UpdateCurData();
}

void CDb::SetSort(LPCTSTR sort, int index)
{
    if (CString(sort) != _T("ASC")) {
        MessageBox(NULL, "Only ASC is supported", "Warning", MB_OK);
        return;
    }

    m_sort = "ASC";

    if (m_sort == sort)
        return;

	m_sort = sort;

	if (m_index >= 0)
	{
		m_index = index;
		UpdateCurData();
	}
	SetEnv("sort", sort);
}

void CDb::SetBookmark(BOOL set)
{
	CString		tag;

    CreateTagField(m_curTag, tag, m_curTagSearchMode);

    if (m_curCategory == -1)
    {
	    ExecuteSql("UPDATE data SET bookmark=%d WHERE 1 %s", 
            set, tag);
    }
    else
    {
	    ExecuteSql("UPDATE data SET bookmark=%d WHERE category=%d %s", 
            set, m_curCategory, tag);
    }
    UpdateCurData();
}

int CDb::CreateItem(int category, int bookmark, CUIntArray &tag,
				 CString &key, CString &content, 
				 RtfImageList &keyImages, RtfImageList& contentImages)
{
	Lock		lock(m_mutex);
    char		query[1024];

	if (key == "" || category == -1)
		return -1;

	CString			tagStr;
	int				t = (int) time(NULL);

	if (bookmark != 0)
		bookmark = 1;

	EncodeTagString(tag, tagStr);

    _sntprintf(query, 1024, "SELECT MAX(itime) FROM data WHERE category=%u;", category);
    int itime = m_db.execScalar(query, INT_MAX);

    if (itime == INT_MAX)
        itime = 0;
    else
        itime++;

	// 1      2       3        4         5     6            7           8       9     10
	// itime, mtime, category, bookmark, tag, keyImage, contentImage, dictKey, key, content
	CppSQLite3Statement stmt = m_db.compileStatement("insert into data values(NULL,?,?,?,?,?,?,?,?,?,?,1,1);");

	stmt.bind(1, itime);
	stmt.bind(2, t);
	stmt.bind(3, category);
	stmt.bind(4, bookmark);
	stmt.bind(5, tagStr);
	if (m_dictMode)
		stmt.bind(6, 0);
	else
		stmt.bind(6, (int) keyImages.size()); 

	stmt.bind(7, (int) contentImages.size()); 

	if (m_dictMode)
	{
		stmt.bind(8, key);
		stmt.bindNull(9);
	}
	else
	{
		stmt.bindNull(8);

        std::shared_ptr<BYTE>   dst;
        unsigned long           dstLen;

        if (Compress(key, key.GetLength() + 1, dst, dstLen) != 0)
        {
            m_error = "Fail to compress";
            return -1;
        }
        stmt.bind(9, dst.get(), dstLen);
    }

    std::shared_ptr<BYTE>   dst;
    unsigned long           dstLen;

    if (Compress(content, content.GetLength() + 1, dst, dstLen) != 0)
    {
        m_error = "Fail to compress";
        return -1;
    }

    stmt.bind(10, dst.get(), dstLen);

//	TRACE("%s\n", query);
	m_error = "";
	try
	{
		stmt.execDML();
	}
	catch(CppSQLite3Exception &ex)
	{
		m_error = ex.errorMessage();
		return -1;
	}

	int rowid = (int) m_db.lastRowId();
    RtfImageList::iterator  it;

    if (keyImages.size() + contentImages.size() > 0)
    {
        CreateMediaPath(rowid);
    }

	{
		CppSQLite3Statement stmt = m_db.compileStatement("insert into image values(NULL,?,?,?,?,?,?,?,?,?);");
        for(it = keyImages.begin(); it != keyImages.end(); it++)
		{
			stmt.bind(1, rowid);
			stmt.bind(2, 0);
			stmt.bind(3, (int) it->second.id);
			stmt.bind(4, it->second.width);
			stmt.bind(5, it->second.height);
			stmt.bind(6, it->second.owidth);
			stmt.bind(7, it->second.oheight);
			stmt.bind(8, it->second.type);
            stmt.bind(9, NULL, 0);
			//stmt.bind(9, it->second.data.get(), it->second.len);
			stmt.execDML();
			stmt.reset();

            if (ExportMedia(rowid, 0, it->second) == -1)
            {
                m_error = "Fail to export image";
                return -1;
            }
		}
	}
	{
		CppSQLite3Statement stmt = m_db.compileStatement("insert into image values(NULL,?,?,?,?,?,?,?,?,?);");
        for(it = contentImages.begin(); it != contentImages.end(); it++)
		{
			stmt.bind(1, rowid);
			stmt.bind(2, 1);
			stmt.bind(3, (int)it->second.id);
			stmt.bind(4, it->second.width);
			stmt.bind(5, it->second.height);
			stmt.bind(6, it->second.owidth);
			stmt.bind(7, it->second.oheight);
			stmt.bind(8, it->second.type);
            stmt.bind(9, NULL, 0);
			//stmt.bind(9, it->second.data.get(), it->second.len);
			stmt.execDML();
			stmt.reset();
            if (ExportMedia(rowid, 1, it->second) == -1)
            {
                m_error = "Fail to export image";
                return -1;
            }
        }
	}
	UpdateCurData();
	ResetImageList(keyImages);
	ResetImageList(contentImages);

	return 0;
}

int CDb::UpdateItem(int mask, int category, int bookmark, 
				CUIntArray &tag, CString &key, CString &content, 
				DbRtfImages &keyImages, DbRtfImages &contentImages)
{
	CString		query, s;
	int			i = 1;
	Lock		lock(m_mutex);
    int         itime = 0;
    bool        need_reindex = false;

	if (mask == 0)
		return -1;

	s.Format("UPDATE data SET mtime=%u", (int) time(NULL));

    if (mask & CATEGORY_UPDATE) {
        s += ", category=?";

        if (m_cur_itemCategory != category) {
            char		query2[1024];

            _sntprintf(query2, 1024, "SELECT MAX(itime) FROM data WHERE category=%u;", category);
            itime = m_db.execScalar(query2, INT_MAX);

            if (itime == INT_MAX)
                itime = 0;
            else
                itime++;

            s += ", itime=?";
            need_reindex = true;
        }
    }

	if (mask & BOOKMARK_UPDATE)
		s += ", bookmark=?";

	if (mask & TAG_UPDATE)
		s += ", tag=?";

	if (mask & KEY_UPDATE)
	{
		if (m_dictMode)
			s += ", dictKey=?";
		else
			s += ", keyImage=?, key=?, keyCompressed=1";
	}

    if (mask & VALUE_UPDATE)
    {
        s += ", contentImage=?, content=?, contentCompressed=1";
    }

	query.Format("%s WHERE id=%u;", s, m_cur_id);

	CppSQLite3Statement stmt = m_db.compileStatement(query);

	if (mask & CATEGORY_UPDATE)
	{
		stmt.bind(i++, category);
        if (m_cur_itemCategory != category) {
            stmt.bind(i++, itime);
        }
    }
	if (mask & BOOKMARK_UPDATE)
	{
		if (bookmark != 0)
			bookmark = 1;

		stmt.bind(i++, bookmark);
	}
	if (mask & TAG_UPDATE)
	{
		CString		tagStr;

		EncodeTagString(tag, tagStr);
		stmt.bind(i++, tagStr);
	}

	if (mask & KEY_UPDATE)
	{
		if (m_dictMode)
			stmt.bind(i++, key);
		else
		{
			stmt.bind(i++, (int) keyImages.size());
			
            std::shared_ptr<BYTE>   dst;
            unsigned long           dstLen;
            
            if (Compress(key, key.GetLength() + 1, dst, dstLen) != 0)
            {
                m_error = "Fail to compress";
                return -1;
            }

            stmt.bind(i++, dst.get(), dstLen);
		}
	}
	if (mask & VALUE_UPDATE)
	{
		stmt.bind(i++, (int) contentImages.size());

        std::shared_ptr<BYTE>   dst;
        unsigned long           dstLen;

        if (Compress(content, content.GetLength() + 1, dst, dstLen) != 0)
        {
            m_error = "Fail to compress";
            return -1;
        }

        stmt.bind(i++, dst.get(), dstLen);
	}

	m_error = "";
	try
	{
		stmt.execDML();
	}
	catch(CppSQLite3Exception &ex)
	{
		m_error = ex.errorMessage();
		return -1;
	}

    if (keyImages.size() + contentImages.size() > 0)
    {
        CreateMediaPath(m_cur_id);
    }

    RtfImageList::iterator  it;
	if (mask & KEY_UPDATE)
	{
        // remove
        for(it = keyImages.remove.begin(); it != keyImages.remove.end(); it++)
		{
		    if (ExecuteSql("DELETE FROM image WHERE did=%u AND owner=0 AND id=%u;", 
                            m_cur_id, it->second.id) == -1)
			    return -1;

            DeleteMedia(m_cur_id, 0, it->second);
        }

        // update
        for(it = keyImages.update.begin(); it != keyImages.update.end(); it++)
		{
		    if (ExecuteSql("UPDATE image SET width=%d, height=%d WHERE did=%u AND owner=0 AND id=%u;", 
                            it->second.width, it->second.height, 
                            m_cur_id, it->second.id) == -1)
			    return -1;
        }

        // add
		CppSQLite3Statement stmt = m_db.compileStatement("insert into image values(NULL,?,?,?,?,?,?,?,?,?);");
        for(it = keyImages.add.begin(); it != keyImages.add.end(); it++)
		{
			stmt.bind(1, (int) m_cur_id);
			stmt.bind(2, 0);
			stmt.bind(3, (int) it->second.id);
			stmt.bind(4, it->second.width);
			stmt.bind(5, it->second.height);
			stmt.bind(6, it->second.owidth);
			stmt.bind(7, it->second.oheight);
			stmt.bind(8, it->second.type);
            stmt.bind(9, NULL, 0);
//			stmt.bind(9, it->second.data.get(), it->second.len);
			stmt.execDML();
			stmt.reset();

            if (ExportMedia(m_cur_id, 0, it->second) == -1)
            {
                m_error = "Fail to export image";
                return -1;
            }
        }
	}

	if (mask & VALUE_UPDATE)
	{
        // remove
        for(it = contentImages.remove.begin(); it != contentImages.remove.end(); it++)
		{
		    if (ExecuteSql("DELETE FROM image WHERE did=%u AND owner=1 AND id=%u;", 
                            m_cur_id, it->second.id) == -1)
			    return -1;
            DeleteMedia(m_cur_id, 1, it->second);
        }

        // update
        for(it = contentImages.update.begin(); it != contentImages.update.end(); it++)
		{
		    if (ExecuteSql("UPDATE image SET width=%d, height=%d WHERE did=%u AND owner=1 AND id=%u;", 
                            it->second.width, it->second.height, 
                            m_cur_id, it->second.id) == -1)
			    return -1;
        }

		CppSQLite3Statement stmt = m_db.compileStatement("insert into image values(NULL,?,?,?,?,?,?,?,?,?);");
        for(it = contentImages.add.begin(); it != contentImages.add.end(); it++)
		{
			stmt.bind(1,(int) m_cur_id);
			stmt.bind(2, 1);
			stmt.bind(3, (int) it->second.id);
			stmt.bind(4, it->second.width);
			stmt.bind(5, it->second.height);
			stmt.bind(6, it->second.owidth);
			stmt.bind(7, it->second.oheight);
			stmt.bind(8, it->second.type);
            stmt.bind(9, NULL, 0);
			//stmt.bind(9, it->second.data.get(), it->second.len);
			stmt.execDML();
			stmt.reset();

            if (ExportMedia(m_cur_id, 1, it->second) == -1)
            {
                m_error = "Fail to export image";
                return -1;
            }
        }
	}

    if (need_reindex)
        Reindex();

	UpdateCurData();
	ResetImageList(keyImages.add);
//	ResetImageList(keyImages.nochange);
//	ResetImageList(keyImages.remove);
//	ResetImageList(keyImages.update);

//	ResetImageList(contentImages.remove);
	ResetImageList(contentImages.add);
//	ResetImageList(contentImages.nochange);
//	ResetImageList(contentImages.update);

	return 0;
}

int CDb::DeleteItem()
{
	Lock		lock(m_mutex);

	if (ExecuteSql("DELETE FROM data WHERE id=%u;", m_cur_id) == -1)
		return -1;
	if (ExecuteSql("DELETE FROM image WHERE did=%u;", m_cur_id) == -1)
		return -1;

    DeleteMediaPath(m_cur_id);

	UpdateCurData();
	return 0;
}

int CDb::LoadData(CppSQLite3Query &q, BOOL raw, int contentType, int category, 
	int bookmark, CUIntArray &tagList, pair<int,int> user1, pair<int,int> user2,
    int tagSearchMode, LPCTSTR sort, int start, int count)
{
	char		query[1024];
	CString		tag;
	CString		limit;
	CString		content;
	int			total;
	Lock		lock(m_mutex);
    CString     user;

	q.finalize();
	CreateTagField(tagList, tag, tagSearchMode);

	if (contentType == CONTENT_TYPE_KEY)
	{
		if (m_dictMode)
			content = "dictKey as key";
		else
			content = "key";
	}
	else if (contentType == CONTENT_TYPE_VALUE)
		content = "content";
	else if (contentType == CONTENT_TYPE_BOTH)
	{
		if (m_dictMode)
			content = "dictKey as key,content";
		else
			content = "key,content";
	}
	else
		return -1;

    if (user1.first >= 0 && user1.second >= 0 && user1.first <= user1.second) {
        user.Format(" AND (%d <= user1 AND user1 <= %d) ", user1.first, user1.second);
    }

    if (user2.first >= 0 && user2.second >= 0 && user2.first <= user2.second) {
        user.Format("%s AND (%d <= user1 AND user1 <= %d) ", user, user2.first, user2.second);
    }

    /* get total */
	if (bookmark == 1)
	{
		if (category == -1)
			_sntprintf(query, 1024, 
			"SELECT count(*) FROM data WHERE bookmark=1 %s %s;", user, tag);
		else
			_sntprintf(query, 1024, 
				"SELECT count(*) FROM data WHERE category=%d AND bookmark=1 %s %s ;", 
				category, user, tag);
	}
	else
	{
		if (category == -1)
			_sntprintf(query, 1024, 
				"SELECT count(*) FROM data WHERE 1 %s %s;", user, tag);
		else
			_sntprintf(query, 1024, 
				"SELECT count(*) FROM data WHERE category=%d %s %s;", 
				category, user, tag);
	}
	total = m_db.execScalar(query);
	if (total == 0)
		return 0;

	if (start > total - 1)
		start = total - 1;

	if (start >= 0)
	{
		if (count > 0)
			limit.Format("LIMIT %d, %d", start, count);
		else
			limit.Format("LIMIT %d, 1", start);
	}

	if (raw)
	{
		if (bookmark == 1)
		{
			if (category == -1)
				_sntprintf(query, 1024, 
					"SELECT a.id,itime,mtime,category,bookmark,tag,"
				    "keyImage,contentImage,%s,keyCompressed,contentCompressed,user1,user2 FROM data a "
				    "INNER JOIN category b ON a.category = b.id "
					"WHERE bookmark=1 %s %s ORDER BY b.name, itime %s %s;", 
					content, user, tag, sort, limit);
			else
				_sntprintf(query, 1024, 
					"SELECT a.id,itime,mtime,category,bookmark,tag,"
				    "keyImage,contentImage,%s,keyCompressed,contentCompressed,user1,user2 FROM data a "
				    "INNER JOIN category b ON a.category = b.id "
					"WHERE category=%d AND bookmark=1 %s %s ORDER BY b.name, itime %s %s;", 
					content,
					category,
                    user,
					tag, 
					sort,
					limit);
		}
		else
		{
			if (category == -1)
				_sntprintf(query, 1024, 
					"SELECT a.id,itime,mtime,category,bookmark,tag,"
				    "keyImage,contentImage,%s,keyCompressed,contentCompressed,user1,user2 FROM data a "
				    "INNER JOIN category b ON a.category = b.id "
					"WHERE 1 %s %s ORDER BY b.name, itime %s %s;", 
					content, user, tag, sort, limit);
			else
				_sntprintf(query, 1024, 
					"SELECT a.id,itime,mtime,category,bookmark,tag,"
				    "keyImage,contentImage,%s,keyCompressed,contentCompressed,user1,user2 FROM data a "
				    "INNER JOIN category b ON a.category = b.id "
					"WHERE category=%d %s %s ORDER BY b.name, itime %s %s;", 
					content, 
					category,
                    user,
					tag, 
					sort,
					limit);
		}
		if (ExecuteSql2(q, query) == -1)
			return -1;
		return total;
	}

	if (bookmark == 1)
	{
		if (category == -1)
			_sntprintf(query, 1024, 
				"SELECT a.id,itime,mtime,b.name as category,bookmark,tag,"
				"keyImage,contentImage,%s,keyCompressed,contentCompressed,user1,user2 FROM data a "
				"INNER JOIN category b ON a.category = b.id "
				"WHERE bookmark=1 %s %s ORDER BY b.name, itime %s %s;", 
				content, user, tag, sort, limit);
		else
			_sntprintf(query, 1024, 
				"SELECT a.id,itime,mtime,b.name as category,bookmark,tag,"
				"keyImage,contentImage,%s,keyCompressed,contentCompressed,user1,user2 FROM data a "
				"INNER JOIN category b ON a.category = b.id "
				"WHERE category=%d AND bookmark=1 %s %s ORDER BY b.name, itime %s %s;", 
				content,
				category,
                user,
				tag, 
				sort,
				limit);
	}
	else
	{
		if (category == -1)
			_sntprintf(query, 1024, 
				"SELECT a.id,itime,mtime,b.name as category,bookmark,tag,"
				"keyImage,contentImage,%s,keyCompressed,contentCompressed,user1,user2 FROM data a "
				"INNER JOIN category b ON a.category = b.id "
				"WHERE 1 %s %s ORDER BY b.name, itime %s %s;", 
				content, user, tag, sort, limit);
		else
			_sntprintf(query, 1024, 
				"SELECT a.id,itime,mtime,b.name as category,bookmark,tag,"
				"keyImage,contentImage,%s,keyCompressed,contentCompressed,user1,user2 FROM data a "
				"INNER JOIN category b ON a.category = b.id "
				"WHERE category=%d %s %s ORDER BY b.name, itime %s %s;", 
				content, 
				category,
                user,
				tag, 
				sort,
				limit);
	}

	if (ExecuteSql2(q, query) == -1)
		return -1;

	return total;
}

int CDb::LoadData(CppSQLite3Query &q, BOOL raw, int contentType, 
				  unsigned int id, unsigned int itime, LPCTSTR sort)
{
	char		query[1024];
	CString		content;
	int			index;
	Lock		lock(m_mutex);

	q.finalize();

	if (contentType == CONTENT_TYPE_KEY)
	{
		if (m_dictMode)
			content = "dictKey as key";
		else
			content = "key";
	}
	else if (contentType == CONTENT_TYPE_VALUE)
		content = "content";
	else if (contentType == CONTENT_TYPE_BOTH)
	{
		if (m_dictMode)
			content = "dictKey as key,content";
		else
			content = "key,content";
	}
	else
		return -1;

	if (sort == "ASC")
		_sntprintf(query, 1024, "SELECT count(*) FROM data WHERE itime < %u;", itime);
	else
		_sntprintf(query, 1024, "SELECT count(*) FROM data WHERE itime > %u;", itime);

	index = m_db.execScalar(query);
	if (index < 0)
		index = 0;

	if (raw)
	{
		_sntprintf(query, 1024, 
			"SELECT id,itime,mtime,category,bookmark,tag,"
			"keyImage,contentImage,%s,keyCompressed,contentCompressed FROM data "
			"WHERE id=%u;", content, id);
	}
	else
	{
		_sntprintf(query, 1024, 
			"SELECT a.id,itime,mtime,b.name as category,bookmark,tag,"
			"keyImage,contentImage,%s,keyCompressed,contentCompressed FROM data a "
			"INNER JOIN category b ON a.category = b.id "
			"WHERE a.id=%u;", content, id);
	}
	if (ExecuteSql2(q, query) == -1)
		return -1;

	return index;
}

int CDb::SearchKey(CppSQLite3Query &q, CString key)
{
	Lock		lock(m_mutex);
	CppSQLite3Statement stmt;

	q.finalize();

	if (m_dictMode == FALSE)
		return -1;

	CppSQLite3Buffer buffer;

    key.Format("%%%s%%", key);
	buffer.format(
		"SELECT id,itime,dictKey as key,content,keyCompressed,contentCompressed FROM data WHERE dictKey LIKE %Q;", key);

	return ExecuteSql2(q, buffer);
}

int CDb::LoadDataFromCurrentSetting(CppSQLite3Query &q, int start, int count)
{
	return LoadData(q, TRUE, CONTENT_TYPE_BOTH,
		m_curCategory, m_curBookmarkMode, m_curTag, DEFAULT_RANGE, DEFAULT_RANGE, 
        m_curTagSearchMode, m_sort, start, count);
}


int CDb::LoadImage(unsigned int id, int owner, RtfImageList &imageList, bool loadActualData)
{
	Lock				lock(m_mutex);
	CppSQLite3Query		q;
	RtfImage    		info;

	ResetImageList(imageList);

	if (ExecuteSql(q, "SELECT * FROM image WHERE did=%d AND owner=%d ORDER BY id ASC;", 
					id, owner) == -1)
		return -1;

	while(!q.eof())
	{
        info.data.reset();
        info.len = 0;

		info.id = q.getIntField("id");
		info.width = q.getIntField("width");
		info.height = q.getIntField("height");
		info.owidth = q.getIntField("owidth");
		info.oheight = q.getIntField("oheight");
		info.type = q.getStringField("type");

        info.http_path.Format("%s/%u/%u_%u.%s", m_media_path, id, owner, info.id, info.type);

        if (loadActualData && LoadMedia(id, owner, info) == -1)
            return -1;

        imageList[info.id] = info;
        /*
		const unsigned char *p = q.getBlobField("data", info.len);
		if (info.len > 0)
		{
			info.data = std::shared_ptr<BYTE>(new BYTE[info.len]);
			memcpy(info.data.get(), p, info.len);
            imageList[info.id] = info;
		}
        */

		q.nextRow();
	}
	
	return 0;
}

void CDb::UpdateCurData()
{
	Lock		lock(m_mutex);
	CppSQLite3Query		q;

	ResetImageList(m_cur_keyImageList);
	ResetImageList(m_cur_contentImageList);

	m_cur_itemCount = LoadDataFromCurrentSetting(q, m_index, 1);

	if (m_cur_itemCount <= 0)
	{
		m_index = 0;
		m_cur_key = "";
		m_cur_content = "";
		m_cur_itemCategory = 0;
		m_cur_itime = 0;
		m_cur_itemBookmark = 0;
		m_cur_itemTag.RemoveAll();
		m_cur_id = 0;

		return;
	}
	if (m_index > m_cur_itemCount - 1)
		m_index = m_cur_itemCount - 1;
	
	m_cur_id = q.getIntField("id");
	m_cur_itime = q.getIntField("itime");
	m_cur_mtime = q.getIntField("mtime");
	SetCategory(q.getIntField("category"));
	m_cur_itemBookmark = q.getIntField("bookmark");
	DecodeTagString(CString(q.getStringField("tag")), m_cur_itemTag);
	if (q.getIntField("keyImage") > 0)
		LoadImage(m_cur_id, 0, m_cur_keyImageList, true);

	if (q.getIntField("contentImage") > 0)
		LoadImage(m_cur_id, 1, m_cur_contentImageList, true);

    if (q.getIntField("keyCompressed") == 0 || m_dictMode)
    {
        m_cur_key = q.getStringField("key");
    }
    else
    {
        int     len = 0;
        auto    p = q.getBlobField("key", len);

        Decompress(p, len, m_cur_key);
    }

    if (q.getIntField("contentCompressed") == 0)
    {
        m_cur_content = q.getStringField("content");
    }
    else
    {
        int     len = 0;
        auto    p = q.getBlobField("content", len);

        Decompress(p, len, m_cur_content);
    }
}


void CDb::SetCategory(int cate)
{
	Lock		lock(m_mutex);
	char		query[255];

	_sntprintf(query, 255, 
		"SELECT count(*) FROM category WHERE id=%d", cate);

	if (m_db.execScalar(query) == 0)
	{
		ExecuteSql("UPDATE data SET category=0 WHERE id=%u;", 
			m_cur_id);
		m_cur_itemCategory = 0;
	}
	else
		m_cur_itemCategory = cate;

	return;
}

int CDb::GotoByitime(unsigned int itime)
{
	Lock		lock(m_mutex);
	int			index;
	char		query[1024];

	if (m_sort == "ASC")
		_sntprintf(query, 1024, "SELECT count(*) FROM data WHERE category=%d AND itime < %u;", m_curCategory, itime);
	else
		_sntprintf(query, 1024, "SELECT count(*) FROM data WHERE category=%d AND itime > %u;", m_curCategory, itime);

	index = m_db.execScalar(query);
	if (index < 0)
		return -1;

	m_index = index;
	UpdateCurData();

	return 0;
}

int CDb::GotoByid(LPCTSTR id, BOOL useCategory)
{
	Lock		        lock(m_mutex);
	int			        index;
    int                 cate;
	char		        query[1024];
    unsigned int        itime;
    CppSQLite3Query		q;

	if (ExecuteSql(q, "SELECT itime, category FROM data WHERE id=%s;", id) == -1)
		return -1;

    itime = q.getIntField("itime");
    cate = q.getIntField("category");

    if (useCategory)
    {
        if (m_sort == "ASC")
		    _sntprintf(query, 1024, "SELECT count(*) FROM data WHERE itime < %u AND category=%d;", 
                    itime, cate);
	    else
		    _sntprintf(query, 1024, "SELECT count(*) FROM data WHERE itime > %u AND category=%d;", 
                    itime, cate);
    }
    else
    {
        if (m_sort == "ASC")
		    _sntprintf(query, 1024, "SELECT count(*) FROM data WHERE itime < %u;", itime);
	    else
		    _sntprintf(query, 1024, "SELECT count(*) FROM data WHERE itime > %u;", itime);
    }

	index = m_db.execScalar(query);
	if (index < 0)
		return -1;

    m_curTag.RemoveAll();
    m_curBookmarkMode = 0;

    if (useCategory)
    {
        m_curCategory = cate;
    }
    else
    {
        m_curCategory = -1;
    }

	m_index = index;
	UpdateCurData();

	return 0;
}

void CDb::Goto(int index)
{
	if (index < 0 || index > m_cur_itemCount - 1)
		return;

	m_index = index;
	UpdateCurData();
}

void CDb::Prev()
{
	if (m_index <= 0)
		return;

	m_index--;
	UpdateCurData();
}

void CDb::Next()
{
	if (m_index >= m_cur_itemCount - 1)
		return;

	m_index++;
	UpdateCurData();
}

void CDb::First()
{
	m_index = 0;
	UpdateCurData();
}

void CDb::Last()
{
	m_index = m_cur_itemCount - 1;
	UpdateCurData();
}

void CDb::Reindex()
{
    CppSQLite3Query		q;
    int                 index = 0;

    int n = LoadDataFromCurrentSetting(q, 0, m_cur_itemCount);
    if (n == 0)
        return;

    while (!q.eof()) {
        auto id = q.getIntField("id");
        auto itime = q.getIntField("itime");

        if (itime != index) {
            if (ExecuteSql("UPDATE data SET itime=%u WHERE id=%u",
                index, id) == -1)
                return;

        }

        index++;
        q.nextRow();
    }
}


int CDb::MoveTo(std::vector<std::pair<int, int>> &src, int dst)
{
	Lock				lock(m_mutex);
	unsigned int		start, end;
    int                 inc = 100;

    if (m_cur_itemCount <= 0 || src.size() == 0)
		return -1;

    map<unsigned int, int> m;
    CppSQLite3Query		q;
    int first, last;

    first = src.front().first;
    last = src.back().first;

	if (m_sort == "ASC")
	{   
        start = min(first, dst);
        end = max(last, dst);
 
        for (int i = 0; i < src.size(); i++)
        {
            m[src[i].second] = -1;
        }

        LoadDataFromCurrentSetting(q, start, end - start + 1);

        int new_itime = start;

        while (!q.eof()) {
            auto id = q.getIntField("id");
            auto itime = q.getIntField("itime");

            if (m.count(id) > 0) {
                q.nextRow();
                continue;
            }

            if (dst == itime) {
                for (auto &s : src) {
                    m[s.second] = new_itime;

                    if (ExecuteSql("UPDATE data SET itime=%u WHERE id=%u",
                        new_itime++, s.second) == -1)
                        return -1;
                }
            }
            
            if (ExecuteSql("UPDATE data SET itime=%u WHERE id=%u",
                new_itime++, id) == -1)
                return -1;

            q.nextRow();
        }

        if (m.begin()->second == -1) {
            for (auto &s : src) {
                m[s.second] = new_itime;

                if (ExecuteSql("UPDATE data SET itime=%u WHERE id=%u",
                    new_itime++, s.second) == -1)
                    return -1;
            }
        }
	}
	else
	{
        MessageBox(NULL, "Not Support in desc sort", "Error", MB_OK);
        return 0;
    }

    m_index = dst;
    if (m_index > m_cur_itemCount - 1)
        m_index = m_cur_itemCount - 1;

	UpdateCurData();
	return 0;
}

int CDb::ToFirst()
{
	Lock				lock(m_mutex);
	unsigned int		t;

	if (m_cur_itemCount <= 0)
		return -1;

	if (m_sort == "ASC")
	{
        vector<pair<int, int>> item = { {m_cur_itime, m_cur_id } };
        MoveTo(item, 0);
	}
	else
	{
		t = (unsigned int) time(0);
	}

	UpdateCurData();
	return 0;
}

int CDb::ToPrev()
{
	Lock				lock(m_mutex);
	unsigned int		new_itime, new_id;
	CppSQLite3Query		q;

	if (m_cur_itemCount <= 0 || m_index <= 0)
		return -1;

	LoadDataFromCurrentSetting(q, --m_index, 1);
	new_id = q.getIntField("id");
	new_itime = q.getIntField("itime");

	if (ExecuteSql("UPDATE data SET itime=%u WHERE id=%u", 
		m_cur_itime, new_id) == -1)
		return -1;

	if (ExecuteSql("UPDATE data SET itime=%u WHERE id=%u", 
		new_itime, m_cur_id) == -1)
		return -1;

	UpdateCurData();
	return 0;
}

int CDb::ToNext()
{
	Lock				lock(m_mutex);
	unsigned int		new_itime, new_id;
	CppSQLite3Query		q;

	if (m_cur_itemCount <= 0 || m_index >= m_cur_itemCount - 1)
		return -1;

	LoadDataFromCurrentSetting(q, ++m_index, 1);
	new_id = q.getIntField("id");
	new_itime = q.getIntField("itime");

	if (ExecuteSql("UPDATE data SET itime=%u WHERE id=%u", 
		m_cur_itime, new_id) == -1)
		return -1;

	if (ExecuteSql("UPDATE data SET itime=%u WHERE id=%u", 
		new_itime, m_cur_id) == -1)
		return -1;

	UpdateCurData();
	return 0;
}

void CDb::Normalize(CString &src)
{
	if (src == _T(""))
		return;

	src.Replace("'", "''");

	return;
}

void CDb::CreateTagField(CUIntArray &list, CString &tag, int mode)
{
	if (list.GetCount() == 0)
	{
		tag = "";
		return;
	}
	CUIntArray		temp;
	
	temp.Copy(list);
	qsort(temp.GetData(), temp.GetCount(), sizeof(int), compare);

	CString s;

    if (mode == 0)  // AND
    {
	    tag = "AND tag LIKE '";
	    for(int i = 0; i < temp.GetCount(); ++i)
	    {
		    s.Format("%%$%d$", temp[i]);
		    tag += s;
	    }
	    tag += "%'";
    }
    else
    {
	    tag = "AND (";

        for(int i = 0; i < temp.GetCount(); ++i)
	    {
            if (i == 0)
		        s.Format("tag LIKE '%%$%d$%%' ", temp[i]);
            else
		        s.Format("OR tag LIKE '%%$%d$%%' ", temp[i]);
		    tag += s;
	    }
	    tag += ")";
    }

	return;
}

void CDb::EncodeTagString(CUIntArray &tag, CString &tagStr)
{
	if (tag.GetCount() == 0)
	{
		tagStr = " ";
		return;
	}
	CUIntArray		temp;
	
	temp.Copy(tag);
	qsort(temp.GetData(), temp.GetCount(), sizeof(int), compare);

	CString s;
	tagStr = "";
	for(int i = 0; i < temp.GetCount(); ++i)
	{
		s.Format(" $%d$", temp[i]);
		tagStr += s;
	}
	tagStr += " ";
	return;
}

void CDb::DecodeTagString(LPCTSTR src, CUIntArray &tag)
{
	Lock		lock(m_mutex);
	int			id;
	BOOL		update = FALSE;
	char		query[255];
	CString		tagStr;
	CString		resToken, s;
	int			curPos = 1;

	tag.RemoveAll();
	if (src == NULL)
		return;

	tagStr = src;

	resToken= tagStr.Tokenize(_T(" "),curPos);
	while (resToken != _T(""))
	{
		s = resToken.Mid(1, resToken.GetLength() - 2);
		id = atoi(s);
		_sntprintf(query, 255, 
			"SELECT count(*) FROM tag WHERE id=%d", id);
		if (m_db.execScalar(query) > 0)
		{
			tag.Add(id);
		}
		else
		{
			update = TRUE;
		}
		resToken = tagStr.Tokenize(_T(" "), curPos);
	};   

	if (update)
	{
		tagStr = "";
		TRACE("tag should be updated\n");
		EncodeTagString(tag, tagStr);

		ExecuteSql("UPDATE data SET tag='%s' WHERE id=%u;", 
			tagStr, m_cur_id);
	}
}

////////////////////////////////////////////////////////////////////////////////////////
int CDb::GetCategoryName(int id, CString &name)
{
	Lock				lock(m_mutex);
	CppSQLite3Query q;

	name = "";
	if (ExecuteSql(q, "SELECT name FROM category WHERE id=%d;", id) == -1)
		return -1;

	name = q.getStringField(0);
	return 0;
}

int CDb::GetTagName(int id, CString &name)
{
	Lock				lock(m_mutex);
	CppSQLite3Query q;

	name = "";
	if (ExecuteSql(q, "SELECT name FROM tag WHERE id=%d;", id) == -1)
		return -1;

	name = q.getStringField(0);
	return 0;

}

void CDb::DecodeTagString(LPCTSTR src, CNames &names)
{
	Lock				lock(m_mutex);
	int					id;
	CName				name;
	CppSQLite3Query		q;
	CString				tagStr;

	names.RemoveAll();
	if (src == NULL)
		return;

	CString		resToken, s;
	int			curPos = 1;

	tagStr = src;
	resToken= tagStr.Tokenize(_T(" "),curPos);
	while (resToken != _T(""))
	{
		s = resToken.Mid(1, resToken.GetLength() - 2);
		id = atoi(s);
		if (ExecuteSql(q, "SELECT name FROM tag WHERE id=%d", id) == 0)
		{
			name.id = id;
			name.name = q.getStringField(0);
			names.Add(name);
		}
		resToken = tagStr.Tokenize(_T(" "), curPos);
	};   
}

int CDb::Query(StringMapArray &result, int contentType, int category, 
	int bookmark, CUIntArray &tagList, pair<int, int> user1, pair<int, int> user2, int tagSearchMode,
    LPCTSTR sort, int start, int count)
{
	Lock				lock(m_mutex);
	CppSQLite3Query		q;
	int					total;
	CNames				names;
	CRtfParser			parser;
	CHtmlListener		listener;
	RtfImageList	    imageList;
	CString				tag;
    CString             str;


	result.clear();
	total = LoadData(q, FALSE, contentType, category, bookmark, tagList, user1, user2, 
            tagSearchMode, sort, start, count);
	if (total <= 0)
		return 0;

	while(!q.eof())
	{
		StringMap	sm;

		for(int i = 0; i < q.numFields(); i++)
		{
			if (strcmp(q.fieldName(i), "tag") == 0)
			{
				DecodeTagString(q.getStringField(i), names);
				tag = "";

				CString		s;
				for(int j = 0; j < names.GetCount(); j++)
				{
					if (j == 0)
					{
						tag.Format("%d", names[j].id);
					}
					else
					{
						s.Format(",%d", names[j].id);
						tag += s;
					}
				}
				sm["tag"] = tag;
			}
			else if (strcmp(q.fieldName(i), "key") == 0)
			{
				if (m_dictMode)
				{
					sm[q.fieldName(i)] = String(q.getStringField(i));
					continue;
				}

				LoadImage(q.getIntField("id"), 0, imageList);

                if (q.getIntField("keyCompressed") == 0)
                {
                    str = q.getStringField(i);
                }
                else
                {
                    int     len = 0;
                    auto    p = q.getBlobField("key", len);

                    Decompress(p, len, str);
                }

                if (parser.OpenFromString(str, &imageList) == -1)
				{
					return -1;
				}
				if (parser.Parse(&listener, "0") == -1)
				{
					return -1;
				}
				sm[q.fieldName(i)] = listener.m_text;
				ResetImageList(imageList);
			}
			else if (strcmp(q.fieldName(i), "content") == 0)
			{
				LoadImage(q.getIntField("id"), 1, imageList);

                if (q.getIntField("contentCompressed") == 0)
                {
                    str = q.getStringField(i);
                }
                else
                {
                    int     len = 0;
                    auto    p = q.getBlobField("content", len);

                    Decompress(p, len, str);
                }

                if (parser.OpenFromString(str, &imageList) == -1)
				{
					return -1;
				}
				if (parser.Parse(&listener, "1") == -1)
				{
					return -1;
				}
				sm[q.fieldName(i)] = listener.m_text;
				ResetImageList(imageList);
			}
            else if (strcmp(q.fieldName(i), "keyCompressed") == 0 || 
                     strcmp(q.fieldName(i), "contentCompressed") == 0)
            {
                continue;
            }
			else
			{
				sm[q.fieldName(i)] = q.getStringField(i);
			}
		}
        sm["css"] = (LPCTSTR)listener.GetCss();
        listener.ClearCss();
		
		result.push_back(sm);
		q.nextRow();
	}
	return total;
}

int CDb::Query(StringMapArray &result, LPCTSTR id, BOOL useCategory, CString sort, int &index)
{
	Lock				lock(m_mutex);
	CppSQLite3Query		q;
	int					total;
	CNames				names;
	CRtfParser			parser;
	CHtmlListener		listener;
	RtfImageList	    imageList;
	CString				tag;
    int                 category;
    CUIntArray          tagList;
    unsigned int        itime;
	char		        query[1024];
    CString             str;

	result.clear();

	if (ExecuteSql(q, "SELECT itime, category FROM data WHERE id=%s;", id) == -1)
		return -1;

    itime = q.getIntField("itime");
    category = q.getIntField("category");

    if (useCategory)
    {
        if (sort == "ASC")
		    _sntprintf(query, 1024, "SELECT count(*) FROM data WHERE itime < %u AND category=%d;", 
                    itime, category);
	    else
		    _sntprintf(query, 1024, "SELECT count(*) FROM data WHERE itime > %u AND category=%d;", 
                    itime, category);
    }
    else
    {
        if (sort == "ASC")
		    _sntprintf(query, 1024, "SELECT count(*) FROM data WHERE itime < %u;", itime);
	    else
		    _sntprintf(query, 1024, "SELECT count(*) FROM data WHERE itime > %u;", itime);

        category = -1;
    }

	index = m_db.execScalar(query);
	if (index < 0)
		return -1;

	total = LoadData(q, TRUE, CONTENT_TYPE_KEY, category, 0, tagList, 
                DEFAULT_RANGE, DEFAULT_RANGE, 0, sort, index, 1);
	if (total <= 0)
		return 0;

	while(!q.eof())
	{
		StringMap	sm;

		for(int i = 0; i < q.numFields(); i++)
		{
			if (strcmp(q.fieldName(i), "tag") == 0)
			{
				DecodeTagString(q.getStringField(i), names);
				tag = "";

				CString		s;
				for(int j = 0; j < names.GetCount(); j++)
				{
					if (j == 0)
					{
						tag.Format("%d", names[j].id);
					}
					else
					{
						s.Format(",%d", names[j].id);
						tag += s;
					}
				}
				sm["tag"] = tag;
			}
			else if (strcmp(q.fieldName(i), "key") == 0)
			{
				if (m_dictMode)
				{
					sm[q.fieldName(i)] = String(q.getStringField(i));
					continue;
				}

				LoadImage(q.getIntField("id"), 0, imageList);

                if (q.getIntField("keyCompressed") == 0)
                {
                    str = q.getStringField(i);
                }
                else
                {
                    int     len = 0;
                    auto    p = q.getBlobField("key", len);

                    Decompress(p, len, str);
                }
                
                if (parser.OpenFromString(str, &imageList) == -1)
				{
					return -1;
				}
				if (parser.Parse(&listener, "0") == -1)
				{
					return -1;
				}
				sm[q.fieldName(i)] = listener.m_text;
				ResetImageList(imageList);
			}
			else if (strcmp(q.fieldName(i), "content") == 0)
			{
				LoadImage(q.getIntField("id"), 1, imageList);
                
                if (q.getIntField("contentCompressed") == 0)
                {
                    str = q.getStringField(i);
                }
                else
                {
                    int     len = 0;
                    auto    p = q.getBlobField("content", len);

                    Decompress(p, len, str);
                }
                
                if (parser.OpenFromString(str, &imageList) == -1)
				{
					return -1;
				}
				if (parser.Parse(&listener, "1") == -1)
				{
					return -1;
				}
				sm[q.fieldName(i)] = listener.m_text;
				ResetImageList(imageList);
			}
            else if (strcmp(q.fieldName(i), "keyCompressed") == 0 ||
                strcmp(q.fieldName(i), "contentCompressed") == 0)
            {
                continue;
            }
            else
			{
				sm[q.fieldName(i)] = q.getStringField(i);
			}
		}
        sm["css"] = (LPCTSTR)listener.GetCss();
        listener.ClearCss();
		
		result.push_back(sm);
		q.nextRow();
	}
	return total;
}

int CDb::GetSummary(StringMapArray &result, BOOL categoryName,
                    CString lineFeed, bool stressHead)
{
	Lock				lock(m_mutex);
	CNames				names;
	CRtfParser			parser;
	CTextListener		listener(100, lineFeed, stressHead);
	CString				tag;
	CString				s;
	CppSQLite3Query		q;
	int					total = 0;
    CString             str;

	result.clear();
	// error or total 0
	total = LoadData(q, TRUE, CONTENT_TYPE_KEY,
		             m_curCategory, m_curBookmarkMode, m_curTag, 
                     DEFAULT_RANGE, DEFAULT_RANGE, m_curTagSearchMode, m_sort, -1, 0);
	if (total <= 0) 
		return 0;

	while(!q.eof())
	{
		StringMap	sm;

		for(int i = 0; i < q.numFields(); i++)
		{
			if (strcmp(q.fieldName(i), "tag") == 0)
			{
				DecodeTagString(q.getStringField(i), names);
				tag = "";
				for(int j = 0; j < names.GetCount(); j++)
				{
					if (j == 0)
						tag = names[j].name;
					else
					{
						tag += ",";
						tag +=  names[j].name;
					}
				}
				sm["tag"] = tag;
			}
			else if (strcmp(q.fieldName(i), "key") == 0)
			{
				if (m_dictMode)
				{
					sm[q.fieldName(i)] = q.getStringField(i);
					continue;
				}

                if (q.getIntField("keyCompressed") == 0)
                {
                    str = q.getStringField(i);
                }
                else
                {
                    int     len = 0;
                    auto    p = q.getBlobField("key", len);

                    Decompress(p, len, str);
                }

                if (parser.OpenFromString(str, NULL) == -1)
				{
					return -1;
				}
				if (parser.Parse(&listener, 0) == -1)
				{
					return -1;
				}
				sm[q.fieldName(i)] = listener.m_text;
			}
			else if (strcmp(q.fieldName(i), "category") == 0)
			{
				if (categoryName)
				{
					GetCategoryName(q.getIntField(i), s);
					sm["category"] = s;
				}
				else
				{
					sm["category"] = q.getStringField(i);
				}
			}
			else
			{
				sm[q.fieldName(i)] = q.getStringField(i);
			}
		}

		result.push_back(sm);
		q.nextRow();
	}

	return total;
}

int CDb::GetSummary(int category, int bookmark, CUIntArray &tagList, int tagSearchMode, 
                    StringMapArray &result, BOOL categoryName,
                    CString lineFeed, bool stressHead)
{
	Lock				lock(m_mutex);
	CNames				names;
	CRtfParser			parser;
	CTextListener		listener(100, lineFeed, stressHead);
	CString				tag;
	CString				s, str;
	CppSQLite3Query		q;
	int					total = 0;
//    CUIntArray          tagList;

	result.clear();
	// error or total 0
//	total = LoadData(q, TRUE, CONTENT_TYPE_KEY,
//		             -1, 0, tagList, m_sort, -1, 0);
	total = LoadData(q, TRUE, CONTENT_TYPE_KEY,
		            category, bookmark, tagList, 
                    DEFAULT_RANGE, DEFAULT_RANGE, tagSearchMode, m_sort, -1, 0);
	if (total <= 0) 
		return 0;

	while(!q.eof())
	{
		StringMap	sm;

		for(int i = 0; i < q.numFields(); i++)
		{
			if (strcmp(q.fieldName(i), "tag") == 0)
			{
				DecodeTagString(q.getStringField(i), names);
				tag = "";
				for(int j = 0; j < names.GetCount(); j++)
				{
					if (j == 0)
						tag = names[j].name;
					else
					{
						tag += ",";
						tag +=  names[j].name;
					}
				}
				sm["tag"] = tag;
			}
			else if (strcmp(q.fieldName(i), "key") == 0)
			{
				if (m_dictMode)
				{
					sm[q.fieldName(i)] = q.getStringField(i);
					continue;
				}

                if (q.getIntField("keyCompressed") == 0)
                {
                    str = q.getStringField(i);
                }
                else
                {
                    int     len = 0;
                    auto    p = q.getBlobField("key", len);

                    Decompress(p, len, str);
                }
                
                if (parser.OpenFromString(str, NULL) == -1)
				{
					return -1;
				}
				if (parser.Parse(&listener, 0) == -1)
				{
					return -1;
				}
				sm[q.fieldName(i)] = listener.m_text;
			}
			else if (strcmp(q.fieldName(i), "category") == 0)
			{
				if (categoryName)
				{
					GetCategoryName(q.getIntField(i), s);
					sm["category"] = s;
				}
				else
				{
					sm["category"] = q.getStringField(i);
				}
			}
			else
			{
				sm[q.fieldName(i)] = q.getStringField(i);
			}
		}

		result.push_back(sm);
		q.nextRow();
	}

	return total;
}

int CDb::SearchKey(StringMapArray &result, CString &key, CString lineFeed, bool stressHead)
{
	Lock				lock(m_mutex);
	CRtfParser			parser;
	CTextListener		listener(100, lineFeed, stressHead);
	CString				tag;
	CppSQLite3Query		q;
    CString             str;

	result.clear();

	if (SearchKey(q, key) < 0) 
		return 0;

	while(!q.eof())
	{
		StringMap	sm;

        sm["db"] = this->GetName();
		for(int i = 0; i < q.numFields(); i++)
		{
			if (strcmp(q.fieldName(i), "content") == 0)
			{
                if (q.getIntField("contentCompressed") == 0)
                {
                    str = q.getStringField(i);
                }
                else
                {
                    int     len = 0;
                    auto    p = q.getBlobField("content", len);

                    Decompress(p, len, str);
                }

                if (parser.OpenFromString(str, NULL) == -1)
				{
					return -1;
				}
				if (parser.Parse(&listener, 0) == -1)
				{
					return -1;
				}
				sm[q.fieldName(i)] = listener.m_text;
			}
			else
			{
				sm[q.fieldName(i)] = q.getStringField(i);
			}
		}

		result.push_back(sm);
		q.nextRow();
	}

	return result.size();
}

int CDb::GetSummaryAsJson(std::string &s, int category, 
	int bookmark, CUIntArray &tagList, pair<int, int> user1, pair<int, int> user2, 
    int tagSearchMode, LPCTSTR sort,
    CString lineFeed, bool stressHead, int maxLen, int start, int count)
{
	Lock				lock(m_mutex);
	CString				tag;
	CppSQLite3Query		q;
	int					total;
	Object				item;
	Array				list;
	Object				obj;
	CString				limit;
	std::stringstream	stream;
	CNames				names;
	CRtfParser			parser;
	CTextListener		listener(maxLen, lineFeed, stressHead);
    CString             str;

	s = "{}";
	total = LoadData(q, FALSE, CONTENT_TYPE_KEY, category, bookmark, tagList, user1, user2, tagSearchMode,
		sort, start, count);
	if (total <= 0)
		total = 0;

	obj["total"] = Number(total);
	if (total == 0)
	{
		Writer::Write(obj, stream);
		s = stream.str();
		return 0;
	}

	list.Clear();
	while(!q.eof())
	{
		item.Clear();

		for(int i = 0; i < q.numFields(); i++)
		{
			if (strcmp(q.fieldName(i), "tag") == 0)
			{
				DecodeTagString(CString(q.getStringField(i)), names);
				tag = "";
				for(int j = 0; j < names.GetCount(); j++)
				{
					if (j == 0)
						tag = names[j].name;
					else
					{
						tag += ",";
						tag +=  names[j].name;
					}
				}
				item["tag"] = String((LPCTSTR) tag);
			}
			else if (strcmp(q.fieldName(i), "key") == 0)
			{
				if (m_dictMode)
				{
					item[q.fieldName(i)] = String(q.getStringField(i));
					continue;
				}

                if (q.getIntField("keyCompressed") == 0)
                {
                    str = q.getStringField(i);
                }
                else
                {
                    int     len = 0;
                    auto    p = q.getBlobField("key", len);

                    Decompress(p, len, str);
                }

                if (parser.OpenFromString(str, NULL) == -1)
				{
					return -1;
				}
				if (parser.Parse(&listener, 0) == -1)
				{
					return -1;
				}
				item[q.fieldName(i)] = String((LPCTSTR) listener.m_text);
			}
			else
			{
				item[q.fieldName(i)] = String(q.getStringField(i));
			}
		}

		list.Insert(item);
		q.nextRow();
	}

	obj["summary"] = list;
	Writer::Write(obj, stream);
	s = stream.str();

	return total;
}

int CDb::GetCategoryListAsJson(std::string &s)
{
	Lock				lock(m_mutex);
	Object				obj;
	CppSQLite3Query		q;
	Array				list;
	Object				item;

	s = "{}";

	if (ExecuteSql(q, "SELECT * FROM category ORDER BY name ASC;") == -1)
		return -1;

	while(!q.eof())
	{
		item["id"] = Number(q.getIntField(0));
		item["name"] = String(q.getStringField(1));
		list.Insert(item);

		q.nextRow();
	}
	obj["category"] = list;

	std::stringstream stream;
	Writer::Write(obj, stream);
	s = stream.str();

	return 0;
}


int CDb::GetTagListAsJson(std::string &s)
{
	Lock				lock(m_mutex);
	Object				obj;
	CppSQLite3Query		q;
	Array				list;
	Object				item;

	s = "{}";

	if (ExecuteSql(q, "SELECT * FROM tag ORDER BY name ASC;") == -1)
		return -1;

	while(!q.eof())
	{
		item["id"] = Number(q.getIntField(0));
		item["name"] = String(q.getStringField(1));
		list.Insert(item);

		q.nextRow();
	}
	obj["tag"] = list;

	std::stringstream stream;
	Writer::Write(obj, stream);
	s = stream.str();

	return 0;
}

int CDb::UpdateTag(unsigned int id, 
				   int mask, int bookmark, CUIntArray &tag)
{
	Lock	lock(m_mutex);
	char	query[2048];
	int		total = 2048;
	int		update = 0;

	int		len = _sntprintf(query, total, "UPDATE data SET mtime=%u", 
				(unsigned int) time(0));

	if (mask & BOOKMARK_UPDATE)
	{
		if (bookmark != 0)
			bookmark = 1;

		len += _sntprintf(query + len, total - len, ", bookmark=%d", bookmark);
		update = 1;
	}
	if (mask & TAG_UPDATE)
	{
		CString		tagStr;

		EncodeTagString(tag, tagStr);
		len += _sntprintf(query + len, total - len, ", tag='%s'", tagStr);
		update = 1;
	}
	if (update == 0)
		return 0;

	len += _sntprintf(query + len, total - len, " WHERE id=%u;", id);
	m_error = "";
	try
	{
		m_db.execDML(query);
	}
	catch(CppSQLite3Exception &ex)
	{
		m_error = ex.errorMessage();
		return -1;
	}

	return 0;
}

int CDb::UpdateUserFields(unsigned int id, int user1, int user2)
{
    Lock	lock(m_mutex);
    char	query[2048];
    int		total = 2048;

    if (user1 >= 0 && user2 >= 0) {
        _sntprintf(query, total, "UPDATE data SET mtime=%u, user1=%1, user2=%1 WHERE id=%u",
            (unsigned int)time(0), user1, user2, id);
    }
    else if (user1 >= 0) {
        _sntprintf(query, total, "UPDATE data SET mtime=%u, user1=%1 WHERE id=%u",
            (unsigned int)time(0), user1, id);
    }
    else if (user2 >= 0) {
        _sntprintf(query, total, "UPDATE data SET mtime=%u, user2=%1 WHERE id=%u",
            (unsigned int)time(0), user2, id);
    }
    else
        return 0;
    
    m_error = "";
    try
    {
        m_db.execDML(query);
    }
    catch (CppSQLite3Exception &ex)
    {
        m_error = ex.errorMessage();
        return -1;
    }

    return 0;
}



int CDb::Upgrade()
{
	CppSQLite3Query		q;

    if (ExecuteSql(
		"CREATE TABLE image2(did INTEGER, owner INTEGER, id INTEGER, "
		"width INTEGER, height INTEGER, owidth INTEGER, oheight INTEGER, type CHAR[4], data BLOB, "
		"FOREIGN KEY(did) REFERENCES data(id) ON UPDATE CASCADE ON DELETE CASCADE);") == -1)
		return -1;

	if (ExecuteSql(q, "SELECT * FROM image") == -1)
		return -1;

	CppSQLite3Statement stmt = m_db.compileStatement("insert into image2 values(?,?,?,?,?,?,?,?,?);");
	while(!q.eof())
	{
		stmt.bind(1, q.getIntField("did"));
		stmt.bind(2, q.getIntField("owner"));
		stmt.bind(3, q.getIntField("id"));
		stmt.bind(4, q.getIntField("width"));
		stmt.bind(5, q.getIntField("height"));
		stmt.bind(6, q.getIntField("width"));
		stmt.bind(7, q.getIntField("height"));
		stmt.bind(8, q.getStringField("type"));

        int len;
        const unsigned char *p = q.getBlobField("data", len);
		stmt.bind(9, p, len);
		stmt.execDML();
		stmt.reset();

		q.nextRow();
	}
    if (ExecuteSql("DROP TABLE image;") == -1)
		return -1;

    if (ExecuteSql("ALTER TABLE image2 RENAME TO image;") == -1)
		return -1;

	if (ExecuteSql("CREATE INDEX IDX_DID on image(did);") == -1)
		return -1;

	if (ExecuteSql(q, "SELECT id, key, content FROM data") == -1)
		return -1;

    CString query;
	while(!q.eof())
	{
        CString key = q.getStringField("key");
        CString content = q.getStringField("content");
        CString key2, content2;

        CRtfParser::ConvertImage(key, key2);
        CRtfParser::ConvertImage(content, content2);

        int     id = q.getIntField("id");

  	    query.Format("UPDATE data SET key=?, content=? WHERE id=%u;", id);
 	    CppSQLite3Statement stmt = m_db.compileStatement(query);
     
        stmt.bind(1, key2);
        stmt.bind(2, content2);
	    try
	    {
		    stmt.execDML();
	    }
	    catch(CppSQLite3Exception &ex)
	    {
		    m_error = ex.errorMessage();
		    return -1;
	    }

		q.nextRow();
	}


    m_version = 2;
    SetEnv("version", m_version);
    return 0;
}

int CDb::Upgrade2To3()
{
    CppSQLite3Query		q;
    UpgradeImageDb();

    if (ExecuteSql(q, "SELECT id,key,content FROM data") == -1)
        return -1;

    CString query;

    while (!q.eof())
    {
        int     id = q.getIntField("id");

        query.Format("UPDATE data SET key=?,content=?,keyCompressed=1,contentCompressed=1 WHERE id=%u;", id);
        CppSQLite3Statement stmt = m_db.compileStatement(query);

        std::shared_ptr<BYTE>   dst;
        unsigned long           dstLen;

        CString                 key = q.getStringField("key");
        if (Compress(key, key.GetLength() + 1, dst, dstLen) != 0)
        {
            m_error = "Fail to compress";
            return -1;
        }
        stmt.bind(1, dst.get(), dstLen);

        CString                 content = q.getStringField("content");
        if (Compress(content, content.GetLength() + 1, dst, dstLen) != 0)
        {
            m_error = "Fail to compress";
            return -1;
        }
        stmt.bind(2, dst.get(), dstLen);

        try
        {
            stmt.execDML();
        }
        catch (CppSQLite3Exception &ex)
        {
            m_error = ex.errorMessage();
            return -1;
        }

        q.nextRow();
    }

    m_version = 3;
	SetEnv("version", m_version);

	return 0;
}

int CDb::Upgrade2To3inDictMode()
{
    CppSQLite3Query		q;

    if (ExecuteSql(q, "SELECT id,key,content FROM data") == -1)
        return -1;

    CString query;

    while (!q.eof())
    {
        int     id = q.getIntField("id");

        query.Format("UPDATE data SET content=?,keyCompressed=0,contentCompressed=1 WHERE id=%u;", id);
        CppSQLite3Statement stmt = m_db.compileStatement(query);

        std::shared_ptr<BYTE>   dst;
        unsigned long           dstLen;
 
        CString                 content = q.getStringField("content");
        if (Compress(content, content.GetLength() + 1, dst, dstLen) != 0)
        {
            m_error = "Fail to compress";
            return -1;
        }
        stmt.bind(1, dst.get(), dstLen);

        try
        {
            stmt.execDML();
        }
        catch (CppSQLite3Exception &ex)
        {
            m_error = ex.errorMessage();
            return -1;
        }

        q.nextRow();
    }

    m_version = 3;
    SetEnv("version", m_version);

    return 0;
}

int CDb::UpgradeImageDb()
{
    CppSQLite3Query		q;

    if (ExecuteSql(q, "SELECT * FROM image") == -1)
        return -1;

    CString             query;
    RtfImage    		info;

    while (!q.eof())
    {
        int     uid = q.getIntField("uid");
        int     owner = q.getIntField("owner");
        int     did = q.getIntField("did");

        info.id = q.getIntField("id");
        info.width = q.getIntField("width");
        info.height = q.getIntField("height");
        info.owidth = q.getIntField("owidth");
        info.oheight = q.getIntField("oheight");
        info.type = q.getStringField("type");
        const unsigned char *p = q.getBlobField("data", info.len);
        if (info.len > 0)
        {
            info.data = std::shared_ptr<BYTE>(new BYTE[info.len]);
            memcpy(info.data.get(), p, info.len);
        }

        CreateMediaPath(did);
        ExportMedia(did, owner, info);

        query.Format("UPDATE image SET data='' WHERE uid=%u;", uid);
        try
        {
            m_db.execDML(query);
        }
        catch (CppSQLite3Exception &ex)
        {
            m_error = ex.errorMessage();
            return -1;
        }

        q.nextRow();
    }

    m_version = 3;
    SetEnv("version", m_version);

    return 0;
}

int CDb::Compress(const char *src, int srcLen, std::shared_ptr<BYTE>& dst, unsigned long &dstLen)
{
    int         cmp_status;

    dstLen = compressBound(srcLen);
    dst.reset(new BYTE[dstLen]);
    cmp_status = ::compress(dst.get(), &dstLen, (const unsigned char *)src, srcLen);

    if (cmp_status != Z_OK)
    {
        return -1;
    }

    return 0;
}


int CDb::Decompress(const unsigned char *src, int srcLen, CString &dst)
{
    int         cmp_status;

    std::shared_ptr<BYTE> p;
    unsigned long   pLen = srcLen * 20;
    p.reset(new BYTE[pLen]);

    dst = "";
    cmp_status = ::uncompress(p.get(), &pLen, (const unsigned char *)src, srcLen);

    if (cmp_status != Z_OK)
    {
        return -1;
    }

    dst = p.get();

    return 0;
}

void CDb::CreateMediaPath(int did)
{
    CString     s;

    s.Format("%s%s\\%u", m_db_path, m_media_path, did);
    Util::CreateDirectory(s);
}

void CDb::DeleteMediaPath(int did)
{
    CString     s;

    s.Format("%s%s\\%u", m_db_path, m_media_path, did);
    Util::DeleteDirectory(s);
}

int CDb::ExportMedia(int did, int owner, RtfImage &info)
{
    CString     s;

    s.Format("%s%s\\%u\\%u_%u.%s", m_db_path, m_media_path, did, owner, info.id, info.type);

    CFile			file;

    if (file.Open(s, CFile::modeCreate | CFile::modeWrite) == FALSE)
        return -1;

    file.Write(info.data.get(), info.len);
    file.Close();

    return 0;
}

void CDb::DeleteMedia(int did, int owner, RtfImage &info)
{
    CString     s;

    s.Format("%s%s\\%u\\%u_%u.%s", m_db_path, m_media_path, did, owner, info.id, info.type);
    CFile::Remove(s);
}

int CDb::LoadMedia(int did, int owner, RtfImage &info)
{
    CString     s;

    s.Format("%s%s\\%u\\%u_%u.%s", m_db_path, m_media_path, did, owner, info.id, info.type);

    CFile			file;

    if (file.Open(s, CFile::modeRead) == FALSE)
        return -1;

    info.len = file.GetLength();
    info.data = std::shared_ptr<BYTE>(new BYTE[info.len]);

    file.Read(info.data.get(), info.len);
    file.Close();

    return 0;

}

int CDb::SetNumbering()
{
    Lock				lock(m_mutex);
    CRtfParser			parser;
    CNumberingListener	listener;
    CString				s;
    CppSQLite3Query		q;
    int					total = 0;
    int                 count = 1;
    CString             query;

    if (m_dictMode)
        return 0;

    // error or total 0
    total = LoadData(q, TRUE, CONTENT_TYPE_KEY,
        m_curCategory, m_curBookmarkMode, m_curTag, DEFAULT_RANGE, DEFAULT_RANGE, 
        m_curTagSearchMode, m_sort, -1, 0);
    if (total <= 0)
        return 0;

    int d = (int) log10(double(total)) + 1;
    CString f;

 //   f.Format("%%%dd", d);
    f = "%d";

    while (!q.eof())
    {
        unsigned int id = q.getIntField("id");

        CString str;
        if (q.getIntField("keyCompressed") == 0)
        {
            str = q.getStringField("key");
        }
        else
        {
            int     len = 0;
            auto    p = q.getBlobField("key", len);

            Decompress(p, len, str);
        }

        string txt = str;
        if (parser.OpenFromString(txt.c_str(), NULL) == -1)
        {
            return -1;
        }
        if (parser.Parse(&listener, 0) == -1)
        {
            return -1;
        }

        int start, end;
        auto k = listener.m_offset;
        auto p = txt.c_str() + k;

        if (txt[k - 1] != ' ')
            k++;

        start = k;
        while (txt[k] && (txt[k] == ' '))
        {
            k++;
        }

        end = k - 1;

        if (isdigit(txt[k]))
        {
            // 110.  xxxx
            while (isdigit(txt[k]))
            {
                k++;
            }
            if (txt[k] == '.')
            {
                if (txt[k + 1] == ' ')
                    end = k + 1;
                else
                    end = k;
            }
        }

        s.Format(f + ". ", count);
        txt.replace(start, end - start + 1, (LPCTSTR) s);

        query.Format("UPDATE data SET keyCompressed=1,key=?,mtime=%u WHERE id=%u", (unsigned int)time(0), id);

        CppSQLite3Statement stmt = m_db.compileStatement(query);
 
        std::shared_ptr<BYTE>   dst;
        unsigned long           dstLen;
            
        if (Compress(txt.c_str(), txt.size() + 1, dst, dstLen) != 0)
        {
            m_error = "Fail to compress";
            return -1;
        }

        stmt.bind(1, dst.get(), dstLen);
        m_error = "";
        try
        {
            stmt.execDML();
        }
        catch (CppSQLite3Exception &ex)
        {
            m_error = ex.errorMessage();
            return -1;
        }

        count++;
        q.nextRow();
    }
    UpdateCurData();
    return total;
}

int CDb::ExportToHtml(BOOL exportContent)
{
    Lock				lock(m_mutex);
    CRtfParser			parser;
    CHtmlListener2   	listener;
    CString				s;
    CppSQLite3Query		q;
    int					total = 0;
    int                 count = 1;
    RtfImageList	    imageList;
    char			    dir[512];
    CString			    fileName;
    CString			    path, media_path;

    return 0;
    if (m_dictMode)
        return 0;

    GetCurrentDirectory(512, dir);

    path = dir;
    media_path.Format("%s\\export_media", dir);
    SHCreateDirectoryEx(NULL, media_path, NULL);

    fileName = path + "\\db_export.html";

    // error or total 0
    total = LoadData(q, TRUE, CONTENT_TYPE_BOTH,
        m_curCategory, m_curBookmarkMode, m_curTag, DEFAULT_RANGE, 
        DEFAULT_RANGE, m_curTagSearchMode, m_sort, -1, 0);
    if (total <= 0)
        return 0;

    while (!q.eof())
    {
        LoadImage(q.getIntField("id"), 0, imageList);

        if (parser.OpenFromString(q.getStringField("key"), &imageList) == -1)
        {
            return -1;
        }
        if (parser.Parse(&listener, to_string(count++).c_str()) == -1)
        {
            return -1;
        }
        ResetImageList(imageList);
        s += listener.m_text;

        if (exportContent)
        {
            LoadImage(q.getIntField("id"), 1, imageList);
            if (parser.OpenFromString(q.getStringField("content"), &imageList) == -1)
            {
                return -1;
            }
            if (parser.Parse(&listener, to_string(count++).c_str()) == -1)
            {
                return -1;
            }
            ResetImageList(imageList);
            s += listener.m_text;
        }

        s += "<hr>\n";

        q.nextRow();
    }
    s = listener.GetCss() + s;

    CString r = "<!DOCTYPE HTML PUBLIC \" -//W3C//DTD HTML 4.01 Strict//EN\">\n<html>\n";
    r += g_html_template;
    r += "<body>\n<pre class=\"content\">\n" + s;
    r += "\n</pre>\n</body>\n</html>\n";

    CFile			file;

    if (file.Open(fileName, CFile::modeCreate | CFile::modeWrite) == FALSE)
        return -1;

    file.Write((LPCTSTR)r, r.GetLength());
    file.Close();

    return total;
}

void CDb::reset_itime()
{
    CNames names;

    {
        CppSQLite3Query q;
        if (ExecuteSql(q, "SELECT * FROM category ORDER BY name ASC;") == -1)
            return;

        while (!q.eof())
        {
            CName	name;

            name.id = q.getIntField(0);
            name.name = q.getStringField(1);
            names.Add(name);

            q.nextRow();
        }
    }

    int d = 0;

    for(int i = 0; i < names.GetCount(); i++) 
    {
        char		query[1024];
        CppSQLite3Query q;

        _sntprintf(query, 1024, "SELECT id,itime FROM data WHERE category=%d ORDER BY itime ASC;",
            names[i].id);
        TRACE("%s\n", query);

        ExecuteSql(q, "SELECT id,itime FROM data WHERE category=%d ORDER BY itime ASC;",
                   names[i].id);

        unsigned int itime, id;
        d = 0;

        while (!q.eof())
        {
            id = q.getIntField(0);
            itime = q.getIntField(1);

            if (ExecuteSql("UPDATE data SET itime=%u WHERE id=%u",
                            d, id) == -1)
                return;

            d += 1;
            q.nextRow();
        }
    }
}

void CDb::AddColumns()
{
    if (ExecuteSql("ALTER TABLE data ADD COLUMN user1 INTEGER DEFAULT 0; " 
                   "ALTER TABLE data ADD COLUMN user2 INTEGER DEFAULT 0; ") == -1)
    {
        return;
    }
}