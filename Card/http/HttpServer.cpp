#include "stdafx.h"
#include "WebServer.h"
#include <process.h>
#include "../DbManager.h"
#include "../Util.h"

using namespace std;

void Handler(webserver::http_request* r);
void DbHandler(webserver::http_request* r, CString page);
void HomeHandler(webserver::http_request* r, CString htmlTemplate);
void SummaryHandler(webserver::http_request* r);
void UpdateHandler(webserver::http_request* r, bool onlyBookmark = false);
void UpdateUserFields(webserver::http_request* r);
void MakeSummary(webserver::http_request* r, string &res);
void CreateJsFileForSummary(webserver::http_request* r);
void LoadFile(webserver::http_request* r, string file = "");


std::set<string> g_utf_encode;

char *GetIP(char *pszDest, char *pszBuf, int iLen, unsigned short usPort)
{
	SOCKET sock;
	WSADATA wsaD;
	SOCKADDR_IN sin;
	PHOSTENT pHostEnt;
	char szGET[60], szBuf[1024], *p;
	int i, j;
	const char pszDesc[] = "IP Address=";

	WSAStartup(0x101, &wsaD);

	if(!(pHostEnt = gethostbyname(pszDest))) {
		WSACleanup();

		return NULL;
	}

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sin.sin_family = AF_INET;
	sin.sin_addr = *((PIN_ADDR)pHostEnt->h_addr);
	sin.sin_port = htons(usPort);

	if(connect(sock, (PSOCKADDR)&sin, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
		closesocket(sock);
		WSACleanup();

		return NULL;
	}

	wsprintf(szGET, "GET /ip.php HTTP/1.1\r\nHost: syjin\r\nConnection: close\r\n\r\n");

	if(send(sock, szGET, strlen(szGET) + 1, 0) == SOCKET_ERROR) {
		closesocket(sock);
		WSACleanup();

		return NULL;
	}

	while((i = recv(sock, szBuf, sizeof szBuf - 1, 0)) > 0 && i != SOCKET_ERROR) {
		if(p = strstr(szBuf, pszDesc)) {
			p += strlen(pszDesc);

			for(j = 0; (isdigit(*p) || *p == '.') && j < iLen; p++, j++)
				*(pszBuf + j) = *p;

			*(pszBuf + j) = '\0';
		}
	}

	closesocket(sock);
	WSACleanup();

	return pszBuf;
}

int GetLocalAddress(char *szStr)
{
    char ac[80];
	WSADATA wsaD;

	WSAStartup(0x101, &wsaD);

	szStr[0] = 0;
    if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
        return -1;
    }

    struct hostent *phe = gethostbyname(ac);
    if (phe == 0) {
        cerr << "Yow! Bad host lookup." << endl;
        return -1;
    }

    string t;
    vector<string> address;

    for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
        struct in_addr addr;
        memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
		strcpy(szStr, inet_ntoa(addr));
        address.push_back(szStr);
//		break;
    }
    sort(address.begin(), address.end());
    strcpy(szStr, address[0].c_str());

	WSACleanup();    
    return 0;
}

void SetHostAddr()
{
	char szStr[255];

	GetLocalAddress(szStr);
	webserver::m_internalAddr.Format("http://%s:%d/", szStr, webserver::m_port);
	webserver::m_ip = GetIP("dev.heesung.info", szStr, sizeof szStr - 1, 80);
	webserver::m_externalAddr.Format("http://%s:%d/", webserver::m_ip, webserver::m_port);
}

unsigned int __stdcall HttpServer(void *p)
{
	int		port = webserver::m_port;

	if (p)
	{
		webserver::m_port = (int) p;
	}

	// SetHostAddr()

	webserver::m_mediaType["gif"] = "gif";
	webserver::m_mediaType["jpg"] = "jpg";
	webserver::m_mediaType["mp3"] = "mp3";
	webserver::m_mediaType["bmp"] = "bmp";
	webserver::m_mediaType["png"] = "png";

    g_utf_encode.insert("html");
    g_utf_encode.insert("htm");
    g_utf_encode.insert("txt");
    g_utf_encode.insert("log");

	try
	{
		TRACE("Launch webserver!!!!!!!!!!!!!!!\n");
		webserver(webserver::m_port, Handler);
	}
	catch(...)
	{
		MessageBox(NULL, "Fail to launch web server", "Error", MB_OK);
	}
	return 0;
}

void MakeErrorResponse(webserver::http_request* r)
{
	std::string title;
	std::string body;

	r->status_ = "404 Not Found";
	title      = "404 Not Found";
	body       = "<h1>404 Not Found</h1>";

	r->answer_  = "<html><head><title>";
	r->answer_ += title;
	r->answer_ += "</title></head><body>";
	r->answer_ += body;
	r->answer_ += "</body></html>";
}

bool CheckMedia(string &path)
{
	StringMap::iterator	iter;
	size_t				pos;
	string				ext;

	pos = path.rfind(".");
	if (pos != string::npos)
	{
		ext = path.substr(pos + 1);
	}
	else 
		ext = "";

    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

	iter = webserver::m_mediaType.find(ext);
	if (iter != webserver::m_mediaType.end())
		return true;
	else
		return false;
}

bool utfEncoding(string path)
{
	size_t				pos;
	string				ext;

	pos = path.rfind(".");
	if (pos != string::npos)
	{
		ext = path.substr(pos + 1);
	}
	else 
		ext = "";

    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
	if (g_utf_encode.find(ext) != g_utf_encode.end())
		return true;
	else
		return false;
}

CString getPath(string webUrl)
{
	TCHAR			szTemp[512];
	int				i;

	GetModuleFileName(NULL, szTemp, 512);

	CString curPath = szTemp;
	for(i = curPath.GetLength() - 1; i >= 0; i--)
	{
		if (curPath[i] == _T('\\'))
			break;
	}
    curPath = curPath.Left(i + 1);
    // Instead of relative path, if want to use absolute path or other root path, use above module

    CString path = webUrl.c_str();
    path.Replace("/", "\\");
    path = "." + path;

    return path;
}


void Handler(webserver::http_request* r)
{
	StringMap::iterator	iter;
    string path;

	if (r->path_.substr(0, 9) == "/db/query")
	{
        DbHandler(r, getPath(r->path_));
		return;
	}
	else if (r->path_ == "/db/home")
	{
		HomeHandler(r, getPath(r->path_));
		return;
	}
    else if (r->path_ == "/db/home2")
    {
        HomeHandler(r, getPath(r->path_));
        return;
    }
    else if (r->path_ == "/db/fhome")
    {
        HomeHandler(r, getPath(r->path_));
        return;
    }
    else if (r->path_ == "/db/summary")
	{
		SummaryHandler(r);
		return;
	}
	else if (r->path_ == "/db/update")
	{
		UpdateHandler(r);
		return;
	}
	else if (r->path_ == "/db/updatebk")
	{
		UpdateHandler(r, true);
		return;
	}
	else if (r->path_ == "/db/updateox")
	{
        UpdateUserFields(r);
		return;
	}
    else if (r->path_ == "/")
    {
        path = "/index.html";
    }

    LoadFile(r, path);
	return;
}

void LoadFile(webserver::http_request* r, string path)
{
	CFile				file;
	char *				buf;
	int					len;
	string				ext;
	size_t				pos;

    if (path == "")
        path = r->path_;

    pos = path.rfind(".");
	if (pos != string::npos)
	{
		ext = path.substr(pos + 1);
	}

    if (ext == "")
        return MakeErrorResponse(r); 

    bool media = false;
	if (CheckMedia(path))
	{
    	r->resHeader_["Content-Type"] = "image/" + ext;
        media = true;
	}

	CString s = path.substr(1).c_str();
	s.Replace("/", "\\");

	if (file.Open(s, CFile::modeRead) == FALSE)
		return MakeErrorResponse(r); 

	len = (int) file.GetLength();
	buf = new char[len + 1];
	file.Read(buf, len);
	file.Close();

    if (!utfEncoding((LPCTSTR) s)) 
    {
	    r->response = buf;
	    r->responseLen = len;

        return;
    }
    else
    {
        buf[len] = 0;
        string s = buf;
	    ConvUtf8(s, r->answer_);
        delete buf;
    }

}

void GetRange(string t, pair<int, int>& r) {
    int v;

    auto pos = t.find(',');
    if (pos == -1) {
        v = stoi(t);
        r.first = r.second = v;
    }
    else {
        string t1 = t.substr(0, pos);
        string t2 = t.substr(pos + 1);

        if (!t1.empty() && !t2.empty()) {
            r.first = stoi(t1);
            r.second = stoi(t2);
        }
        else if (!t1.empty()) {
            r.second = r.first = stoi(t1);
        }
        else
            r.second = r.first = stoi(t2);
    }
}


/*
/db/query?type=key&sec=it&cate=1&bk=1&tag_1=1&tag_2=2&sort=1&start=0&count=1

type=0 key
type=1 content
type=2 both
*/

void DbHandler(webserver::http_request* r, CString page)
{
	StringMap::const_iterator iter;
	CString				section;
	int					cate = -1;
	int					bookmark = 0;
	CUIntArray			tags;
	CString				sort = "ASC";
	int					start = 0;
	int					count = 1;
	CDbManager *		db;
	int					type;
	CString				tagStr;
    int                 searchMode = 0;
	CTemplateParser		parser;
	StringMapArray		result;
	CFile				file;
	char *				buf;
	CString				s;
    CString             id;
    pair<int, int>       user1 = { -1,-1 };
    pair<int, int>       user2 = { -1,-1 };

	db = CDbManager::GetInstance();
	iter = r->params_.find("type");
	if (iter == r->params_.end())
		return MakeErrorResponse(r);
	type = atoi(iter->second.c_str());
	if (type != CONTENT_TYPE_KEY && type != CONTENT_TYPE_VALUE &&
		type != CONTENT_TYPE_BOTH)
		return MakeErrorResponse(r);

	iter = r->params_.find("sec");
	if (iter == r->params_.end())
		return MakeErrorResponse(r);
	section = iter->second.c_str();

	iter = r->params_.find("cate");
	if (iter != r->params_.end())
		cate = atoi(iter->second.c_str());

	iter = r->params_.find("bk");
	if (iter != r->params_.end())
		bookmark = atoi(iter->second.c_str());

	iter = r->params_.find("id");
	if (iter != r->params_.end())
		id = iter->second.c_str();

	for(iter = r->params_.begin(); iter != r->params_.end(); iter++)
	{
		if (iter->first.substr(0,4) == "tag_")
		{
			int id = atoi(iter->second.c_str());
			tags.Add(id);

			if (tagStr == "")
				tagStr = iter->second.c_str();
			else
			{
				tagStr += ",";
				tagStr += iter->second.c_str();
			}
		}
	}
	iter = r->params_.find("smode");
	if (iter != r->params_.end())
		searchMode = atoi(iter->second.c_str());

    iter = r->params_.find("user1");
    if (iter != r->params_.end()) {
        GetRange(iter->second, user1);
    }
    
    iter = r->params_.find("user2");
    if (iter != r->params_.end()) {
        GetRange(iter->second, user2);
    }

	iter = r->params_.find("sort");
	if (iter != r->params_.end())
	{
		if (iter->second != "0")
			sort = "DESC";
	}

	iter = r->params_.find("start");
	if (iter != r->params_.end())
	{
		start = atoi(iter->second.c_str());
		if (start < 1)
			start = 1;
	}
	iter = r->params_.find("count");
	if (iter != r->params_.end())
		count = atoi(iter->second.c_str());

	int		total;
    
    if (id == "")
        total = db->Query(result, type, section, cate, bookmark, tags, user1, user2,
            searchMode, sort, start - 1, count);
    else
    {
        total = db->Query(result, section, id, TRUE, sort, start);
        if (total == -1)
	    {
		    return MakeErrorResponse(r);
	    }
        s.Format("%s?type=%d&sec=%s&cate=%s&start=%d", r->path_.c_str(), type, section, result[0]["category"].c_str(), start+1);
        r->resHeader_["Location"] = s;
        r->status_ = "302 Found";
        return;
    }
	if (total == -1)
	{
		return MakeErrorResponse(r);
	}

    string fileName = page + ".html";

    if (file.Open(fileName.c_str(), CFile::modeRead) == FALSE)
	{
		return MakeErrorResponse(r);
	}
	buf = new char[(int) file.GetLength() + 1];
	file.Read(buf, (int) file.GetLength());
	buf[file.GetLength()] = 0;
	file.Close();

	parser.Init(buf);
	while(parser.NextKeyword(s))
	{
	//	TRACE("%s\n", s);
		if (s == "type")
			parser.AddString(type);
		else if (s == "id")
		{
			if (result.size() > 0)
				parser.AddString(result[0]["id"].c_str());
		}
		else if (s == "itime")
		{
			if (result.size() > 0)
				parser.AddString(result[0]["itime"].c_str());
		}
		else if (s == "section")
			parser.AddString(section);
		else if (s == "category")
        {
		    parser.AddString(cate);
        }
		else if (s == "itemcategory")
		{
			if (result.size() > 0)
				parser.AddString(result[0]["category"].c_str());
		}
		else if (s == "querytag")
			parser.AddString(tagStr);
		else if (s == "querybookmark")
			parser.AddString(bookmark);
		else if (s == "querymode")
			parser.AddString(searchMode);
		else if (s == "tag")	
		{
			if (result.size() > 0)
			{
				StringMap	&sm = result[0];
				iter = sm.find("tag");
				if (iter != sm.end())
					parser.AddString(iter->second.c_str());
			}
		}
        else if (s == "user1")
        {
            if (result.size() > 0)
            {
                StringMap	&sm = result[0];
                iter = sm.find("user1");
                if (iter != sm.end())
                    parser.AddString(iter->second.c_str());
            }
        }
        else if (s == "user2")
        {
            if (result.size() > 0)
            {
                StringMap	&sm = result[0];
                iter = sm.find("user2");
                if (iter != sm.end())
                    parser.AddString(iter->second.c_str());
            }
        }
        else if (s == "bookmark")
		{
			if (result.size() > 0)
			{
				StringMap	&sm = result[0];
				iter = sm.find("bookmark");
				if (iter != sm.end())
					parser.AddString(iter->second.c_str());
			}
		}
		else if (s == "total")
			parser.AddString(total);
		else if (s == "start")
			parser.AddString(start);
		else if (s == "count")
			parser.AddString(count);
		else if (s == "sort")
			parser.AddString(sort);
		else if (s == "info")
		{
			string		info;

			db->GetContentInfoAsJson(info);
			parser.AddString(info.c_str());
		}
        else if (s == "css")
        {
            if (result.size() > 0)
            {
                StringMap	&sm = result[0];
                iter = sm.find("css");
                if (iter != sm.end())
                    parser.AddString(iter->second.c_str());
            }
        }
        else if (s == "key")
		{
			if (result.size() > 0)
			{
				StringMap	&sm = result[0];
				iter = sm.find("key");
				if (iter != sm.end())
					parser.AddString(iter->second.c_str());
			}
		}
		else if (s == "content")
		{
			if (result.size() > 0)
			{
				StringMap	&sm = result[0];
				iter = sm.find("content");
				if (iter != sm.end())
					parser.AddString(iter->second.c_str());
			}
		}
		else if (s.Left(7) == "summary")
		{

		}
        else if (s == "curUrl")
        {
            parser.AddString(r->url_.c_str());
        }
        else if (s == "prevUrl")
        {
            if (start > 1)
            {
                CString t = r->url_.c_str();
                CString cur, newStr;
                cur.Format("start=%d", start);
                newStr.Format("start=%d", start - 1);
                t.Replace(cur, newStr);
                parser.AddString(t);
            }
            else
                parser.AddString("");
            
        }
        else if (s == "nextUrl")
        {
            if (start < total)
            {
                CString t = r->url_.c_str();
                CString cur, newStr;
                cur.Format("start=%d", start);
                newStr.Format("start=%d", start + 1);
                t.Replace(cur, newStr);
                parser.AddString(t);
            }
            else
                parser.AddString("");        
        }
        else if (s == "homeUrl")
        {
            CString t = r->url_.c_str();
            CString startStr, bkStr;

            bkStr.Format("bk=%d", bookmark);
            startStr.Format("start=%d", start);
            t.Replace("query2", "home");
            t.Replace("query", "home");
            t.Replace(startStr, bkStr);
            parser.AddString(t);
        }
    }

	delete buf;
	ConvUtf8(parser.m_dst, r->answer_);
//	r->answer_ = str;
	return;
}

/*
/db/home?sec=it&cate=1&bk=1&tag_1=1&tag_2=2
/db/home
*/


void HomeHandler(webserver::http_request* r, CString htmlTemplate)
{
	StringMap::const_iterator iter;
	CString				section;
	CString				cate = "-1";
	CString				bookmark = "0";
	CString				searchMode = "0";
	CString				tagStr;
	CDbManager *		db;
	CString				s;
	string				info;
	CFile				file;
	char *				buf;
	CTemplateParser		parser;
    CString             type = "0";
    CString             id;
	int					start = 0;
	int					count = 1;

	db = CDbManager::GetInstance();

	iter = r->params_.find("type");
	if (iter != r->params_.end())
		type = iter->second.c_str();
//	int v = atoi(type);
//	if (v != CONTENT_TYPE_KEY && v != CONTENT_TYPE_VALUE &&
//		v != CONTENT_TYPE_BOTH)
//		return MakeErrorResponse(r);

	iter = r->params_.find("sec");
	if (iter != r->params_.end())
		section = iter->second.c_str();

	iter = r->params_.find("cate");
	if (iter != r->params_.end())
		cate = iter->second.c_str();

	iter = r->params_.find("bk");
	if (iter != r->params_.end())
		bookmark = iter->second.c_str();

	iter = r->params_.find("smode");
	if (iter != r->params_.end())
		searchMode = iter->second.c_str();

	iter = r->params_.find("id");
	if (iter != r->params_.end())
		id = iter->second.c_str();

	iter = r->params_.find("start");
	if (iter != r->params_.end())
	{
		start = atoi(iter->second.c_str());
	}
	iter = r->params_.find("count");
	if (iter != r->params_.end())
		count = atoi(iter->second.c_str());


	tagStr = "";
	for(iter = r->params_.begin(); iter != r->params_.end(); iter++)
	{
		if (iter->first.substr(0,4) == "tag_")
		{
			if (tagStr == "")
				tagStr = iter->second.c_str();
			else
			{
				tagStr += ",";
				tagStr += iter->second.c_str();
			}
		}
	}

    if (file.Open(htmlTemplate + ".html", CFile::modeRead) == FALSE)
	{
		return MakeErrorResponse(r);
	}
	buf = new char[(int) file.GetLength() + 1];
	file.Read(buf, (int) file.GetLength());
	buf[file.GetLength()] = 0;
	file.Close();

	parser.Init(buf);
	while(parser.NextKeyword(s))
	{
		if (s == "section")
			parser.AddString(section);
		else if (s == "type")
			parser.AddString(type);
		else if (s == "category")
			parser.AddString(cate);
		else if (s == "id")
			parser.AddString(id);
		else if (s == "querytag")
			parser.AddString(tagStr);
		else if (s == "querybookmark")
			parser.AddString(bookmark);
		else if (s == "querymode")
			parser.AddString(searchMode);
		else if (s == "info")
		{
			string		info;

			db->GetContentInfoAsJson(info);
			parser.AddString(info.c_str());
		}
		else if (s == "start")
			parser.AddString(start);
		else if (s == "count")
			parser.AddString(count);
        else if (s == "curUrl")
        {
            parser.AddString(r->url_.c_str());
        }
	}		
	delete buf;
	
	ConvUtf8(parser.m_dst, r->answer_);
//    CreateJsFileForSummary(r);

//	r->answer_ = str;
	return;
}

void CreateJsFileForSummary(webserver::http_request* r)
{
    CFile		    file;
    CString         name = "summary.js";
    string          res;
    CString         t;

    SHCreateDirectoryEx(NULL, "db/js", NULL);

    MakeSummary(r, res);
    t = res.c_str();

    // json in javascript problem
    // double quotation mark
    // '{"result":true,"count":"abc  <span class=\\"1234\\">"}';
    t.Replace("\\", "\\\\");
    t.Replace("'", "\\'");
    t = "var gSummary = '" + t;
    t = t + "';";

    file.Open("db/js/" + name, CFile::modeCreate | CFile::modeReadWrite);
    file.Write(t, t.GetLength());
    file.Close();
}


void MakeSummary(webserver::http_request* r, string &res)
{
	StringMap::const_iterator iter;
	CString				section;
	int					cate = -1;
	int					bookmark = 0;
	CDbManager *		db;
	string				s;
	CUIntArray			tags;
	CString				sort = "ASC";
    int                 searchMode = 0;
    int                 len = 100;
    pair<int, int>       user1 = { -1, -1 }, user2 = { -1, -1 };

	db = CDbManager::GetInstance();

	iter = r->params_.find("sec");
	if (iter == r->params_.end())
		return MakeErrorResponse(r);
	section = iter->second.c_str();

	iter = r->params_.find("cate");
	if (iter != r->params_.end())
		cate = atoi(iter->second.c_str());

	iter = r->params_.find("bk");
	if (iter != r->params_.end())
		bookmark = atoi(iter->second.c_str());

	iter = r->params_.find("len");
	if (iter != r->params_.end())
		len = atoi(iter->second.c_str());

	for(iter = r->params_.begin(); iter != r->params_.end(); iter++)
	{
		if (iter->first.substr(0,4) == "tag_")
		{
			int id = atoi(iter->second.c_str());
			tags.Add(id);
		}
	}

    iter = r->params_.find("smode");
	if (iter != r->params_.end())
		searchMode = atoi(iter->second.c_str());

    iter = r->params_.find("user1");
    if (iter != r->params_.end()) {
        GetRange(iter->second, user1);
    }

    iter = r->params_.find("user2");
    if (iter != r->params_.end()) {
        GetRange(iter->second, user2);
    }

    iter = r->params_.find("sort");
	if (iter != r->params_.end())
	{
		if (iter->second != "0")
			sort = "DESC";
	}

	int total = db->GetSummaryAsJson(s, section, cate, bookmark, tags, user1, user2, 
        searchMode, sort, LINE_FEED, true, len);

	ConvUtf8(s, res);

//	r->answer_ = str;
	return;
}


/*
/db/summary?sec=it&cate=1&bk=1&tag_1=1&tag_2=2&sort=1

return json
*/

void SummaryHandler(webserver::http_request* r)
{
    MakeSummary(r, r->answer_);
	return;
}


/*
/db/update?sec=IT&itime=111111&bk=1&tag_1=1&tag_2=2

*/

void UpdateHandler(webserver::http_request* r, bool onlyBookmark)
{
	StringMap::const_iterator iter;
	CDbManager *		db;
	CString				s;
	CUIntArray			tags;
	unsigned int		id;
	int					bookmark = -1;
	int					mask = 0;
	CString				section;

	db = CDbManager::GetInstance();

	iter = r->params_.find("sec");
	if (iter == r->params_.end())
		return MakeErrorResponse(r);
	section = iter->second.c_str();

	iter = r->params_.find("id");
	if (iter == r->params_.end())
		return MakeErrorResponse(r);
	id = (unsigned int) atoi(iter->second.c_str());

	iter = r->params_.find("bk");
	if (iter != r->params_.end())
	{
		bookmark = atoi(iter->second.c_str());
		mask = BOOKMARK_UPDATE;
	}

	for(iter = r->params_.begin(); iter != r->params_.end(); iter++)
	{
		if (iter->first.substr(0,4) == "tag_")
		{
			int id = atoi(iter->second.c_str());
			tags.Add(id);
		}
	}
	//if (tags.GetCount() > 0)
    if (onlyBookmark == false)
		mask |= TAG_UPDATE;

	if (db->UpdateTag(section, id, mask, bookmark, tags) == 0)
	{
		r->answer_ = "success";
	}
	else
	{
		r->answer_ = "fail";
	}

	return;
}


void UpdateUserFields(webserver::http_request* r)
{
    StringMap::const_iterator iter;
    CDbManager *		db;
    CString				s;
    CUIntArray			tags;
    unsigned int		id;
    int					bookmark = -1;
    int					mask = 0;
    CString				section;

    db = CDbManager::GetInstance();

    iter = r->params_.find("sec");
    if (iter == r->params_.end())
        return MakeErrorResponse(r);
    section = iter->second.c_str();

    iter = r->params_.find("id");
    if (iter == r->params_.end())
        return MakeErrorResponse(r);
    id = (unsigned int)atoi(iter->second.c_str());

    int user1 = -1, user2 = -1;

    iter = r->params_.find("user1");
    if (iter != r->params_.end())
        user1 = atoi(iter->second.c_str());

    iter = r->params_.find("user2");
    if (iter != r->params_.end())
        user2 = atoi(iter->second.c_str());

    if (db->UpdateUserFields(section, id, user1, user2) == 0)
    {
        r->answer_ = "success";
    }
    else
    {
        r->answer_ = "fail";
    }

    return;
}

