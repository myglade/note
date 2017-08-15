/*
   WebServer.cpp

   Copyright (C) 2003-2007 Ren?Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   Ren?Nyffenegger rene.nyffenegger@adp-gmbh.ch

   Thanks to Tom Lynn who pointed out an error in this source code.
*/
#include "stdafx.h"

#include <ctime>
#include <process.h>
#include <iostream>
#include <string>
#include <map>
#include <sstream>


#include "webserver.h"
#include "socket.h"
#include "UrlHelper.h"
#include "base64.h"

using namespace std;

int webserver::m_stop = 0;
int webserver::m_port = 9999;
CString webserver::m_ip;
CString webserver::m_internalAddr;
CString webserver::m_externalAddr;
StringMap webserver::m_mediaType;
CRITICAL_SECTION		g_cs;

webserver::request_func webserver::request_func_=0;

unsigned webserver::Request(void* ptr_s) {
	RequestParam *	rp = reinterpret_cast<RequestParam*>(ptr_s);
	std::string		line;
	unsigned long	timeout = 60 * 1000;

	int		id = rp->id;
	Socket	s = *(rp->s);
	unsigned long	start = GetTickCount();

	while(!rp->stop)
	{
		int		res = s.ReceiveLine(line);
		if (res < 0) 
		{
			break;
		}
		else if (res == 1)
		{
			if (GetTickCount() - start > timeout)
			{
				TRACE("Keep-alive timeout\n");
				break;
			}
			Sleep(50);
			continue;
		}

		http_request req;

		if (line.find("GET") == 0) {
			req.method_="GET";
		}
		else if (line.find("POST") == 0) {
			req.method_="POST";
		}

		std::string path, url;
		std::map<std::string, std::string> params;

		size_t posStartPath = line.find_first_not_of(" ",3);

		SplitGetReq(line.substr(posStartPath), url, path, params);

		req.status_ = "200 OK";
		req.s_      = &s;
        req.url_    = url;
		req.path_   = path;
		req.params_ = params;

		static const std::string authorization   = "Authorization: Basic ";
		static const std::string accept          = "Accept: "             ;
		static const std::string accept_language = "Accept-Language: "    ;
		static const std::string accept_encoding = "Accept-Encoding: "    ;
		static const std::string user_agent      = "User-Agent: "         ;
		static const std::string keep_alive      = "Keep-Alive: "         ;

		while(1) 
		{
			s.ReceiveLine(line);

			if (line.empty()) 
				break;

			unsigned int pos_cr_lf = line.find_first_of("\x0a\x0d");
			if (pos_cr_lf == 0) 
				break;

			line = line.substr(0,pos_cr_lf);

			if (line.substr(0, authorization.size()) == authorization) 
			{
				req.authentication_given_ = true;
				std::string encoded = line.substr(authorization.size());
				std::string decoded = base64_decode(encoded);

				unsigned int pos_colon = decoded.find(":");

				req.username_ = decoded.substr(0, pos_colon);
				req.password_ = decoded.substr(pos_colon+1 );
			}
			else if (line.substr(0, accept.size()) == accept) 
			{
				req.accept_ = line.substr(accept.size());
			}
			else if (line.substr(0, accept_language.size()) == accept_language) 
			{
				req.accept_language_ = line.substr(accept_language.size());
			}
			else if (line.substr(0, accept_encoding.size()) == accept_encoding) 
			{
				req.accept_encoding_ = line.substr(accept_encoding.size());
			}
			else if (line.substr(0, user_agent.size()) == user_agent) 
			{
				req.user_agent_ = line.substr(user_agent.size());
			}
			else if (line.substr(0, keep_alive.size()) == keep_alive) 
			{
				req.keep_alive = atoi(line.substr(keep_alive.size()).c_str());
				start = GetTickCount();
				timeout = req.keep_alive * 1000;
			}
		}
		static std::string const serverName = "RenesWebserver (Windows)";

		time_t ltime;
		time(&ltime);
		tm* gmt= gmtime(&ltime);

		char* asctime_remove_nl = std::asctime(gmt);
		asctime_remove_nl[24] = 0;

		req.resHeader_.clear();
		req.resHeader_["Date"] = string(asctime_remove_nl) + " GMT";
		req.resHeader_["Server"] = serverName;
		//req.resHeader_["Connection"] = "close";
//		req.resHeader_["Content-Type"] = "text/html; charset=ISO-8859-1";
		req.resHeader_["Content-Type"] = "text/html; charset=utf-8";

		req.response = NULL;
		req.responseLen = 0;
		request_func_(&req);


		std::stringstream str_str;
		if (req.response)
			str_str << req.responseLen;
		else
			str_str << req.answer_.size();

		s.SendBytes("HTTP/1.1 ");

		if (! req.auth_realm_.empty() ) {
			s.SendLine("401 Unauthorized");
			s.SendBytes("WWW-Authenticate: Basic Realm=\"");
			s.SendBytes(req.auth_realm_);
			s.SendLine("\"");
		}
		else {
			s.SendLine(req.status_);
		}
		
	//	s.SendLine("200 OK");

		map<string, string>::iterator	iter;
		std::string str;

		for(iter = req.resHeader_.begin(); iter != req.resHeader_.end(); iter++)
		{
			if (iter->first == "Content-Length")
				continue;

			str = iter->first + ": " + iter->second;
//			TRACE("%s\n", str.c_str());
			s.SendLine(str);
		}
		s.SendLine("Content-Length: " + str_str.str());
//		TRACE("Content-Length: %s\n", str_str.str().c_str());
		s.SendLine("");
		if (req.response)
		{
			s.Send(req.response, req.responseLen);
			delete req.response;
			req.response = 0;
		}
		else
		{
			s.SendLine(req.answer_);
		}

		req.resHeader_.clear();
	}
	RequestParam *param;

	EnterCriticalSection(rp->cs);
	if (rp->map->Lookup(rp->s, (void *&)param))
	{
		rp->map->RemoveKey(rp->s);
	}
	LeaveCriticalSection(rp->cs);

	delete rp;

	return 0;
}

webserver::webserver(unsigned int port_to_listen, request_func r) 
{
	CMapPtrToPtr			map;

	SocketServer in(port_to_listen,5, NonBlockingSocket);
//	SocketServer in(port_to_listen,5, BlockingSocket);
	int		id = 0;
	request_func_ = r;

	InitializeCriticalSection(&g_cs);
	while (!m_stop) 
	{
		Socket* ptr_s=in.Accept();

		if (ptr_s == 0)
		{
			Sleep(50);
			continue;
		}

		unsigned ret;
		RequestParam *param = new RequestParam();
		param->id = id++;
		param->s = ptr_s;
		param->cs = &g_cs;
		param->map = &map;

		EnterCriticalSection(&g_cs);
		map[ptr_s] = param;
		LeaveCriticalSection(&g_cs);

		//_beginthreadex(0,0,Request,(void*) ptr_s,0,&ret);
		// Thanks to Frank M. Hoffmann for fixing a HANDLE leak
		HANDLE hHandle = reinterpret_cast<HANDLE>(_beginthreadex(0,0,Request,(void*) param,0,&ret));
		CloseHandle(hHandle);
	}

	EnterCriticalSection(&g_cs);
	POSITION pos;
	Socket*	key;
	RequestParam *value;
	for (pos = map.GetStartPosition(); pos != NULL;)
	{
		map.GetNextAssoc(pos, (void *&)key, (void *&)value);
		value->stop = TRUE;
	}
	LeaveCriticalSection(&g_cs);
	while(1)
	{
		EnterCriticalSection(&g_cs);
		if (map.GetCount() == 0)
		{
			LeaveCriticalSection(&g_cs);
			break;
		}
		LeaveCriticalSection(&g_cs);

		Sleep(1);
	}
//	DeleteCriticalSection(&cs);

}

