#pragma once

#include "Db.h"
#include "DbQuery.h"

#if 0
class CDbV3 : public CDb
{
public:
	CDbV3();
	~CDbV3();

	virtual int Init(LPCTSTR file, LPCTSTR dbName, BOOL dictMode = FALSE);
	virtual int CreateTable();

	virtual int CreateSection(db::Request &req, db::Result &res);
	virtual int DeleteSection(db::Request &req, db::Result &res);
	virtual int GetSectionList(db::Request &req, db::Result &res);
	virtual int RenameSection(db::Request &req, db::Result &res);
	virtual int ReorderSection(db::Request &req, db::Result &res);

	virtual int CreateTag(db::Request &req, db::Result &res);
	virtual int DeleteTag(db::Request &req, db::Result &res);
	virtual int GetTagList(db::Request &req, db::Result &res);
	virtual int RenameTag(db::Request &req, db::Result &res);
	virtual int ReorderTag(db::Request &req, db::Result &res);

	// page
	virtual int PrepareNewPage();
	virtual int CreatePage(Db::Page &p);
	virtual int UpdatePage(Db::Page &p);
	virtual int DeletePage(int pageId);
	virtual int DeletePageMedia(int pageId);

};


/* 
how to set media path

db_path/media/<pageId>/
MUST not use sectionId.

media/<sectionId>/<pageId>/   '
When the section of page is changed, page should be moved to new section.
It causes troubles - move page, change link of page.
*/

#endif
