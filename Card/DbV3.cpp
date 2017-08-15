#include "StdAfx.h"
#include "DbV3.h"
#include <stdio.h>
#include <stdlib.h>
#include "RtfParser/HtmlListener.h"
#include "RtfParser/TextListener.h"
#include "Util.h"
#include "Env.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#if  0
// declare 	lock before any variable declaration for real synchro
//
#define SORT	"ASC"
#define CUR_DB_VERSION	3

using namespace json;

CDbV3::CDbV3(void)
{
}

CDbV3::~CDbV3(void)
{
}

int CDbV3::Init(LPCTSTR file, LPCTSTR dbName, BOOL dictMode)
{
	Lock		lock(m_mutex);
	int         res = 0;

	if (file == "")
		return -1;

	CString path = file;

	m_db_path = path.Left(path.ReverseFind('\\') + 1);

	file = m_db_path + dbName;
	m_media_path.Format("%s%s.media\\", (LPCTSTR)m_db_path, (LPCTSTR)dbName);

	Util::CreateDirectory(m_media_path);

	m_dictMode = dictMode;
	m_error = "";
	try
	{
		m_db.open(file);
		m_dbName = dbName;

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

			m_version = 3;
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

			if (m_version != CUR_DB_VERSION)
				return -1;

		}
	}
	catch (CppSQLite3Exception &ex)
	{
		m_error = ex.errorMessage();
		return -1;
	}
	return res;
}

int CDbV3::CreateTable()
{
	Lock		lock(m_mutex);

	// itime : insert time
	// mtime : modified time
	if (ExecuteSql(
		"CREATE TABLE page(pageId INTEGER PRIMARY KEY AUTOINCREMENT, mtime INTEGER, "
		"				   seq NUMERIC, sectionId INTEGER, level CHAR, "
		"				   title BLOB, content BLOB, "
		"				   FOREIGN KEY(sectionId) REFERENCES section(sectionId) ON UPDATE CASCADE ON DELETE CASCADE);") == -1)
		return -1;

	if (ExecuteSql("CREATE VIRTUAL TABLE pageText USING fts4(title, content);") == -1)
	{
		return -1;
	}
	if (ExecuteSql("CREATE TABLE tag(tagId INTEGER PRIMARY KEY, "
				   "				 seq INTEGER UNIQUE, "
				   "				 name VARCHAR(255) UNIQUE COLLATE NOCASE); ") == -1)
	{
		return -1;
	}

	if (ExecuteSql("CREATE TABLE section(sectionId INTEGER PRIMARY KEY, "
				   "					 seq INTEGER UNIQUE, "
				   "					 name VARCHAR(255) UNIQUE COLLATE NOCASE);") == -1)
	{
		return -1;
	}

	if (ExecuteSql("CREATE TABLE tagList(tagId INTEGER, pageId INTEGER, PRIMARY KEY (tagId, pageId)"
					"FOREIGN KEY(tagId) REFERENCES tag(tagId) ON UPDATE CASCADE ON DELETE CASCADE, "
					"FOREIGN KEY(pageId) REFERENCES page(pageId) ON UPDATE CASCADE ON DELETE CASCADE);") == -1)
	{
		return -1;
	}

	if (ExecuteSql("CREATE INDEX IDX_PAGE_SEQ on page(seq);") == -1)
	{
		return -1;
	}

	if (ExecuteSql("CREATE INDEX IDX_PAGE_SECTION on page(sectionId);") == -1)
	{
		return -1;
	}

	if (ExecuteSql("CREATE INDEX IDX_TAG_SEQ on tag(seq);") == -1)
	{
		return -1;
	}

	if (ExecuteSql(" CREATE INDEX IDX_SECTION_SEQ on section(seq);") == -1)
	{
		return -1;
	}

	return 0;
}

/*******************************************************************
	Section
*******************************************************************/
int CDbV3::CreateSection(db::Request &req, db::Result &res)
{
	Lock		lock(m_mutex);

	if (!req.verifyCmd(db::CREATE_SECTION))
	{
		res = db::Error(-1, "invalid command");
		return -1;
	}

	const char* name = req.get("name").c_str();
	int insertBefore = req.getInt("seq", -1);

	char    query[1024];

	_sntprintf(query, 1024, "SELECT count(*) FROM section WHERE name='%s'; ", name);
	// already exist
	if (m_db.execScalar(query) > 0)
	{
		res = db::Error(-1, "Duplication");
		return -1;
	}

	auto table = m_db.getTable("SELECT sectionId FROM section ORDER BY sectionId ASC;");
	int nextId = 1;
	for (auto i = 0; i < table.numRows(); i++)
	{
		int id = table.getIntField(0);
		if (id != nextId)
			break;

		nextId++;
		table.setRow(i);
	}

	if (insertBefore >= 0 && insertBefore < table.numRows())
	{
		if (ExecuteSql("UPDATE section SET seq = seq + 1 WHERE seq >= %d; ", insertBefore) == -1)
		{
			res = db::Error(-1, "Update Failure");
			return -1;
		}
	}
	else
	{
		insertBefore = table.numRows();
	}

	if (ExecuteSql("INSERT INTO section VALUES(%d,%d,'%s');", nextId, insertBefore, name) == -1)
	{
		res = db::Error(-1, "Insert Failure");
		return -1;
	}

	res = db::Success();
	return 0;
}

int CDbV3::DeleteSection(db::Request &req, db::Result &res)
{
	Lock		lock(m_mutex);

	CppSQLite3Query q;
	int		sectionId;

	if (!req.verifyCmd(db::DELETE_SECTION))
	{
		res = db::Error(-1, "invalid command");
		return -1;
	}

	if (!req.get("name").empty())
	{
		if (ExecuteSql(q, "SELECT sectionId FROM section WHERE name='%s';",
			req.get("name").c_str()) == -1)
		{
			res = db::Error(-1, "sql failure");
			return -1;
		}
		if (q.eof())
		{
			res = db::Error(-1, "section doesn't exist");
			return -1;
		}

		sectionId = q.getIntField(0);
	}
	else
	{
		sectionId = req.getInt("sectionId");
	}

	if (ExecuteSql(q, "SELECT id FROM page WHERE sectionId=%d;", sectionId) == -1)
	{
		res = db::Error(-1, "sql failure");
		return -1;
	}

	while (!q.eof())
	{
		DeletePageMedia(q.getIntField(0));
		q.nextRow();
	}
	if (ExecuteSql("DELETE FROM section WHERE sectionId=%d;", sectionId))
	{
		res = db::Error(-1, "delete Failure");
		return -1;
	}

	res = db::Success();
	return 0;

}

int CDbV3::GetSectionList(db::Request &req, db::Result &res)
{
	Lock			lock(m_mutex);
	CppSQLite3Query q;

	if (!req.verifyCmd(db::GET_SECTION_LIST))
	{
		res = db::Error(-1, "invalid command");
		return -1;
	}

	if (ExecuteSql(q, "SELECT * FROM section ORDER BY seq ASC;") == -1)
	{
		res = db::SqlError();
		return -1;
	}

	res.clear();
	while (!q.eof())
	{
		db::Dict	d;

		d.set("id", q.getIntField(0));
		d.set("name", q.getStringField(1));
		res.push_back(d);
		q.nextRow();
	}
	
	return 0;
}

int CDbV3::RenameSection(db::Request &req, db::Result &res)
{
	Lock		lock(m_mutex);

	if (!req.verifyCmd(db::GET_SECTION_LIST))
	{
		res = db::Error(-1, "invalid command");
		return -1;
	}

	if (ExecuteSql("UPDATE section SET name='%s' WHERE name='%s';", 
		req.get("newName").c_str(), req.get("oldName").c_str()))
	{
		res = db::SqlError();
		return -1;
	}

	res = db::Success();
	return 0;
}

int CDbV3::ReorderSection(db::Request &req, db::Result &res)
{
	Lock		lock(m_mutex);
	int			seq = req.getInt("seq");
	db::Dict &m = (db::Dict &) req.getDict("src");
	std::map<int, int> src;

	if (!req.verifyCmd(db::GET_SECTION_LIST))
	{
		res = db::Error(-1, "invalid command");
		return -1;
	}

	for (auto& i : m.dict())
	{
		// seq : sectionId
		src[std::stoi(i.first)] = std::stoi(i.second);
	}

	int			total = m_db.execScalar("select count(*) from section;");

	if (total == 0 || total == src.size())
	{
		res = db::SqlError();
		return 0;
	}

	if (seq < 0)
		seq = total;

	// id1 is key of src
	int start = src.begin()->first;
	int end = src.end()->first;

	CppSQLite3Query q;

	if (ExecuteSql(q, "SELECT sectionId, seq FROM section WHERE seq >= %d AND seq <= %d ORDER BY seq ASC;",
		start, end) == -1)
		return -1;

	int		notSelected = start;
	int		selected = seq;
	int		moved = 0;

	while (!q.eof())
	{
		int seq = q.getIntField(1);
		int sectionId = q.getIntField(0);

		if (src.find(seq) != src.end())
		{
			selected++;
			moved++;
		}
		else if (moved < total)
		{
			seq = notSelected;
			notSelected++;
		}
		else
		{
			seq += notSelected + total;
			notSelected++;
		}
		q.nextRow();

		ExecuteSql("UPDATE section SET seq=%d WHERE sectionId=%d;",
			seq, sectionId);
	}

	res = db::Success();
	return 0;
}

/*******************************************************************
	Tag
*******************************************************************/

int CDbV3::CreateTag(db::Request &req, db::Result &res)
{
	Lock		lock(m_mutex);

	if (!req.verifyCmd(db::CREATE_TAG))
	{
		res = db::Error(-1, "invalid command");
		return -1;
	}

	const char* name = req.get("name").c_str();
	int insertBefore = req.getInt("seq", -1);

	char    query[1024];

	_sntprintf(query, 1024, "SELECT count(*) FROM tag WHERE name='%s'; ", name);
	// already exist
	if (m_db.execScalar(query) > 0)
	{
		res = db::Error(-1, "Duplication");
		return -1;
	}

	auto table = m_db.getTable("SELECT tagId FROM tag ORDER BY tagId ASC;");
	int nextId = 1;
	for (auto i = 0; i < table.numRows(); i++)
	{
		int id = table.getIntField(0);
		if (id != nextId)
			break;

		nextId++;
		table.setRow(i);
	}

	if (insertBefore >= 0 && insertBefore < table.numRows())
	{
		if (ExecuteSql("UPDATE tag SET seq = seq + 1 WHERE seq >= %d; ", insertBefore) == -1)
		{
			res = db::Error(-1, "Update Failure");
			return -1;
		}
	}
	else
	{
		insertBefore = table.numRows();
	}

	if (ExecuteSql("INSERT INTO tag VALUES(%d,%d,'%s');", nextId, insertBefore, name) == -1)
	{
		res = db::Error(-1, "Insert Failure");
		return -1;
	}

	res = db::Success();
	return 0;
}

int CDbV3::DeleteTag(db::Request &req, db::Result &res)
{
	Lock		lock(m_mutex);

	CppSQLite3Query q;
	int				tagId;

	if (!req.verifyCmd(db::DELETE_TAG))
	{
		res = db::Error(-1, "invalid command");
		return -1;
	}

	if (!req.get("name").empty())
	{
		if (ExecuteSql(q, "SELECT tagId FROM tag WHERE name='%s';",
			req.get("name").c_str()) == -1)
		{
			res = db::Error(-1, "sql failure");
			return -1;
		}
		if (q.eof())
		{
			res = db::Error(-1, "tag doesn't exist");
			return -1;
		}

		tagId = q.getIntField(0);
	}
	else
	{
		tagId = req.getInt("tagId");
	}

	if (ExecuteSql("DELETE FROM tag WHERE tagId=%d;", tagId))
	{
		res = db::Error(-1, "delete Failure");
		return -1;
	}

	res = db::Success();
	return 0;
}

int CDbV3::GetTagList(db::Request &req, db::Result &res)
{
	Lock			lock(m_mutex);
	CppSQLite3Query q;

	if (!req.verifyCmd(db::GET_TAG_LIST))
	{
		res = db::Error(-1, "invalid command");
		return -1;
	}

	if (ExecuteSql(q, "SELECT * FROM tag ORDER BY seq ASC;") == -1)
	{
		res = db::SqlError();
		return -1;
	}

	res.clear();
	while (!q.eof())
	{
		db::Dict	d;

		d.set("id", q.getIntField(0));
		d.set("name", q.getStringField(1));
		res.push_back(d);
		q.nextRow();
	}

	return 0;
}

int CDbV3::RenameTag(db::Request &req, db::Result &res)
{
	Lock		lock(m_mutex);

	if (!req.verifyCmd(db::GET_TAG_LIST))
	{
		res = db::Error(-1, "invalid command");
		return -1;
	}

	if (ExecuteSql("UPDATE tag SET name='%s' WHERE name='%s';",
		req.get("newName").c_str(), req.get("oldName").c_str()))
	{
		res = db::SqlError();
		return -1;
	}

	res = db::Success();
	return 0;
}

int CDbV3::ReorderTag(db::Request &req, db::Result &res)
{
	Lock				lock(m_mutex);
	int					seq = req.getInt("seq");
	db::Dict &m = (db::Dict &) req.getDict("src");
	std::map<int, int>	src;

	if (!req.verifyCmd(db::GET_TAG_LIST))
	{
		res = db::Error(-1, "invalid command");
		return -1;
	}

	for (auto& i : m.dict())
	{
		// seq : sectionId
		src[std::stoi(i.first)] = std::stoi(i.second);
	}

	int			total = m_db.execScalar("select count(*) from tag;");

	if (total == 0 || total == src.size())
	{
		res = db::SqlError();
		return 0;
	}

	if (seq < 0)
		seq = total;

	// id1 is key of src
	int start = src.begin()->first;
	int end = src.end()->first;

	CppSQLite3Query q;

	if (ExecuteSql(q, "SELECT tagId, seq FROM tag WHERE seq >= %d AND seq <= %d ORDER BY seq ASC;",
		start, end) == -1)
		return -1;

	int		notSelected = start;
	int		selected = seq;
	int		moved = 0;

	while (!q.eof())
	{
		int seq = q.getIntField(1);
		int tagId = q.getIntField(0);

		if (src.find(seq) != src.end())
		{
			selected++;
			moved++;
		}
		else if (moved < total)
		{
			seq = notSelected;
			notSelected++;
		}
		else
		{
			seq += notSelected + total;
			notSelected++;
		}
		q.nextRow();

		ExecuteSql("UPDATE tag SET seq=%d WHERE tagId=%d;",
			seq, tagId);
	}

	res = db::Success();
	return 0;
}

/*******************************************************************
Page
*******************************************************************/

int CDbV3::PrepareNewPage()
{
	CppSQLite3Query q;

	if (ExecuteSql(q, "SELECT pageId FROM page ORDER BY pageId ASC;") == -1)
		return -1;

	int nextId = 1;

	while (!q.eof())
	{
		int id = q.getIntField(0);
		if (id != nextId)
			break;

		nextId++;
		q.nextRow();
	}

	CString path;
	
	path.Format("%s%d", m_media_path, nextId);
	Util::CreateDirectory(path);

	return nextId;
}

int CDbV3::CreatePage(Db::Page &p)
{
	Lock		lock(m_mutex);
	char		query[1024];

	_sntprintf(query, 1024, "SELECT count(*) FROM section WHERE sectionId=%d", p.sectionId);
	auto total = m_db.execScalar(query);
	// check sectionId
	if (total == 0)
	{
		return -1;
	}

	// check tagId
	for (auto i = 0; i < p.tags.GetSize(); i++)
	{
		_sntprintf(query, 1024, "SELECT count(*) FROM tag WHERE tagId=%d", p.tags[i]);
		total = m_db.execScalar(query);
		if (total == 0)
		{
			return -1;
		}
	}
	_sntprintf(query, 1024, "SELECT count(*) FROM page WHERE sectionId=%d", p.sectionId);

	total = m_db.execScalar(query);

	if (p.seq >= 0 && p.seq < total)
	{
		if (ExecuteSql("UPDATE page SET seq = seq + 1 WHERE sectionId=%d AND seq >= %d;", 
			p.sectionId, p.seq) == -1)
		{
			return -1;
		}
	}
	else
	{
		p.seq = total;
	}

	// 1      2       3        4         5     6    7       
	// id, mtime,   seq, sectionId, level, title, content
	CppSQLite3Statement stmt = 
		m_db.compileStatement("INSERT INTO page VALUES(?,?,?,?,?,?,?);");

	stmt.bind(1, p.pageId);
	stmt.bind(2, (int)time(NULL));
	stmt.bind(3, p.seq);
	stmt.bind(4, p.sectionId);
	stmt.bind(5, p.level);
	stmt.bind(6, (BYTE *)(LPCTSTR)p.title, p.title.GetLength() + 1);
	stmt.bind(7, (BYTE *)(LPCTSTR)p.content, p.content.GetLength() + 1);

	//	TRACE("%s\n", query);
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

	stmt = m_db.compileStatement("INSERT INTO pageText(docid, title, content) VALUES(?,?,?);");
	stmt.bind(1, p.pageId);
	stmt.bind(2, (LPCTSTR)p.titleText);
	stmt.bind(3, (LPCTSTR)p.contentText);
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

	for (auto i = 0; i < p.tags.GetSize(); i++)
	{
		if (ExecuteSql("INSERT INTO tagList VALUES(%d,%d);", p.tags[i], p.pageId) == -1)
		{
			return -1;
		}
	}

	return 0;
}

int CDbV3::UpdatePage(Db::Page &p)
{
	CString		query, s, text;
	int			i = 1;
	Lock		lock(m_mutex);
	CString		comma;

	if (p.mask == 0)
		return -1;

	s.Format("UPDATE page SET mtime=%u", (int)time(NULL));
	text.Format("UPDATE pageText SET ");

	if (p.mask & CATEGORY_UPDATE)
		s += ", sectionId=?";

	if (p.mask & LEVEL_UPDATE)
		s += ", level=?";

	if (p.mask & KEY_UPDATE)
	{
		s += ", title=?";
		text += comma + "title=?";
		comma = ",";
	}

	if (p.mask & VALUE_UPDATE)
	{
		s += ", content=?";
		text += comma + "content=?";
	}

	query.Format("%s WHERE pageId=%u;", s, p.pageId);

	CppSQLite3Statement stmt = m_db.compileStatement(query);

	if (p.mask & CATEGORY_UPDATE)
		stmt.bind(i++, p.sectionId);

	if (p.mask & LEVEL_UPDATE)
		stmt.bind(i++, p.level);

	if (p.mask & KEY_UPDATE)
		stmt.bind(i++, (BYTE *)(LPCTSTR)p.title, p.title.GetLength() + 1);

	if (p.mask & VALUE_UPDATE)
		stmt.bind(i++, (BYTE *)(LPCTSTR)p.content, p.content.GetLength() + 1);

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

	if ((p.mask & KEY_UPDATE) | (p.mask & VALUE_UPDATE))
	{
		text.Format("%s WHERE docid=%u;", text, p.pageId);
		stmt = m_db.compileStatement(text);

		i = 1;
		if (p.mask & KEY_UPDATE)
			stmt.bind(i++, (LPCTSTR)p.titleText);

		if (p.mask & VALUE_UPDATE)
			stmt.bind(i++, (LPCTSTR)p.contentText);

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
	}

	if (p.mask & TAG_UPDATE)
	{
		ExecuteSql("DELETE FROM tagList WHERE pageId=%d;", p.pageId);
		for (auto i = 0; i < p.tags.GetSize(); i++)
		{
			if (ExecuteSql("INSERT INTO tagList VALUES(%d,%d);", p.tags[i], p.pageId) == -1)
			{
				return -1;
			}
		}
	}

	return 0;
}

int CDbV3::DeletePage(int pageId)
{
	Lock		lock(m_mutex);

	DeletePageMedia(pageId);
	return ExecuteSql("DELETE FROM page WHERE pageId=%u;", pageId);
}

// remove the image files of a page
int CDbV3::DeletePageMedia(int pageId)
{
	CString path;

	path.Format("%s%d", m_media_path, pageId);
	Util::DeleteDirectory(path);

	return 0;
}

/*******************************************************************

*******************************************************************/

/*******************************************************************

*******************************************************************/

/*******************************************************************

*******************************************************************/


#endif
