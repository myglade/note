#pragma once

#include "afxwin.h"
#include <string>
#include <map>

class Mutex
{
public:
	Mutex() 
	{
		InitializeCriticalSection(&m_cs);
	}

	~Mutex()
	{
		DeleteCriticalSection(&m_cs);
	}

	void lock()
	{
		EnterCriticalSection(&m_cs);
	}

	void unlock()
	{
		LeaveCriticalSection(&m_cs);
	}
   private:
      Mutex (const Mutex&);
      Mutex& operator= (const Mutex&);

public:
	CRITICAL_SECTION	m_cs;	
};

class Lock
{
public:
	Lock(Mutex &mutex) : m_mutex(mutex) 
	{
		m_mutex.lock();
	}
	~Lock()
	{
		m_mutex.unlock();
	}

	Mutex &		m_mutex;
};

inline int ConvUtf8(CString &s, CString &dst)
{
	int			len;
	WCHAR *		strW;
	
	if (s.GetLength() == 0)
		return 0;

	strW = new WCHAR[s.GetLength() + 1];
	len = MultiByteToWideChar(CP_ACP, 0, s, -1, strW, s.GetLength() + 1);
	if (len <= 0)
	{
		return -1;
	}

	dst = CW2A(strW, CP_UTF8);
	delete strW;
	return 0;
}

inline int ConvUtf8WithReplace(CString &s, int oldChar, int newChar, CString &dst)
{
	int			len;
	WCHAR *		strW;
	
	if (s.GetLength() == 0)
		return 0;

	strW = new WCHAR[s.GetLength() + 1];
	len = MultiByteToWideChar(CP_ACP, 0, s, -1, strW, s.GetLength() + 1);
	if (len <= 0)
	{
		return -1;
	}
	WCHAR *p = strW;

	while(*p)
	{
		if (*p == oldChar)
			*p = newChar;
		p++;
	}

	dst = CW2A(strW, CP_UTF8);
	delete strW;
	return 0;
}

inline int ConvUtf8(std::string &s, std::string &dst)
{
	int			len;
	WCHAR *		strW;
	
	if (s.size() == 0)
		return 0;

	strW = new WCHAR[s.size() + 1];
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, strW, s.size() + 1);
	if (len <= 0)
	{
		return -1;
	}

	dst = CW2A(strW, CP_UTF8);
	delete strW;
	return 0;
}

inline int ConvUtf8(CString &s, std::string &dst)
{
	int			len;
	WCHAR *		strW;
	
	if (s.GetLength() == 0)
		return 0;

	strW = new WCHAR[s.GetLength() + 1];
	len = MultiByteToWideChar(CP_ACP, 0, s, -1, strW, s.GetLength() + 1);
	if (len <= 0)
	{
		return -1;
	}

	dst = CW2A(strW, CP_UTF8);
	delete strW;
	return 0;
}

inline int ConvUtf8(std::string &s, CString &dst)
{
	int			len;
	WCHAR *		strW;
	
	if (s.size() == 0)
		return 0;

	strW = new WCHAR[s.size() + 1];
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, strW, s.size() + 1);
	if (len <= 0)
	{
		return -1;
	}

	dst = CW2A(strW, CP_UTF8);
	delete strW;
	return 0;
}

/*
		len = MultiByteToWideChar(CP_ACP, 0, s, -1, strW, maxLenW );
		if (len <= 0)
		{
			file.Close();
			delete strW;
			return -1;
		}

		WCHAR *p = strW;

		while(*p)
		{
			if (*p == BULLET_TEMP)
				*p = BULLET;
			p++;
		}
		
		CStringA utf8 = CW2A(strW, CP_UTF8);

*/

/*
<!--{type}-->
<!--{section}-->
<!--{category}-->
<!--{tag}-->
<!--{bookmark}-->
*/

class CTemplateParser
{
public:
	CTemplateParser()
	{
		m_pos = 0; m_pos2 = 0; m_processed = TRUE;
	}
	~CTemplateParser() {}
	void Init(char *src)
	{
		m_src = src; m_pos = 0; m_pos2 = 0; m_dst = "";
		m_processed = TRUE;
	}

	BOOL NextKeyword(CString &keyword)
	{
		if (!m_processed)
		{
			m_dst += m_src.Mid(m_pos, m_pos2 - m_pos);
		}
		m_pos = m_pos2;

		int pos1 = m_src.Find("<!--{", m_pos);
		if (pos1 == -1)
		{
			m_dst += m_src.Mid(m_pos);
			return FALSE;
		}

		m_pos2 = m_src.Find("}-->", pos1 + 5);
		if (m_pos2 == -1)
		{
			m_dst += m_src.Mid(m_pos);
			return FALSE;
		}

		keyword = m_src.Mid(pos1 + 5, m_pos2 - (pos1 + 5));
		m_dst += m_src.Mid(m_pos, pos1 - m_pos);
		m_pos = pos1;
		m_pos2 += 4;
		m_processed = FALSE;

		return TRUE;
	}
	void AddString(LPCTSTR str)
	{
		m_dst += str;
		m_processed = TRUE;
	}
	void AddString(int v)
	{
		CString	str;

		str.Format("%d", v);
		m_dst += str;
		m_processed = TRUE;
	}

	CString		m_dst;
	CString		m_src;
	int			m_pos;
	int			m_pos2;
	BOOL		m_processed;
};

inline void GetKeyValue(LPCTSTR str, std::map<std::string, std::string>& keyMap)
{
    CString     key, value;
    int         curPos = 0;
    LPCTSTR     p = str;

    keyMap.clear();

    while(*p != ' ')
        curPos++;
    
    CString     s(str, curPos);
    curPos = 0;
    CString resToken= s.Tokenize(_T("&"),curPos);
    while (resToken != _T(""))
    {
        int     pos = 0;
        key = resToken.Tokenize(_T("="), pos);
        value = resToken.Tokenize(_T("="), pos);

        keyMap[(LPCTSTR)key] = (LPCTSTR)value;
        resToken = s.Tokenize(_T("% #"), curPos);
    }; 
}

inline int HexPairValue(const char * code) 
{
	int value = 0;
	const char * pch = code;
	for (;;) 
	{
		int digit = *pch++;
		if (digit >= '0' && digit <= '9') 
		{
			value += digit - '0';
		}
		else if (digit >= 'A' && digit <= 'F') 
		{
			value += digit - 'A' + 10;
		}
		else if (digit >= 'a' && digit <= 'f') 
		{
			value += digit - 'a' + 10;
		}
		else 
		{
			return -1;
		}
		if (pch == code + 2)
			return value;
		value <<= 4;
	}
}

inline int UrlDecode(const char *source, char *dest)
{
	char * start = dest;

	while (*source) 
	{
		switch (*source) 
		{
		case '+':
			*(dest++) = ' ';
			break;
		case '%':
			if (source[1] && source[2]) 
			{
				int value = HexPairValue(source + 1);
				if (value >= 0) {
					*(dest++) = value;
					source += 2;
				}
				else {
					*dest++ = '?';
				}
			}
			else {
				*dest++ = '?';
			}
			break;
		default:
			*dest++ = *source;
		}
		source++;
	}
  
	*dest = 0;
	return dest - start;
}  


namespace Util
{
	inline void CreateDirectory(LPCTSTR path)
	{
		SHCreateDirectoryEx(NULL, path, NULL);
	}

	inline bool DeleteDirectory(LPCTSTR lpszDir, bool noRecycleBin = true)
	{
		int len = _tcslen(lpszDir);
		TCHAR *pszFrom = new TCHAR[len + 2];
		_tcscpy(pszFrom, lpszDir);
		pszFrom[len] = 0;
		pszFrom[len + 1] = 0;

		SHFILEOPSTRUCT fileop;
		fileop.hwnd = NULL;    // no status display
		fileop.wFunc = FO_DELETE;  // delete operation
		fileop.pFrom = pszFrom;  // source file name as double null terminated string
		fileop.pTo = NULL;    // no destination needed
		fileop.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;  // do not prompt the user

		if (!noRecycleBin)
			fileop.fFlags |= FOF_ALLOWUNDO;

		fileop.fAnyOperationsAborted = FALSE;
		fileop.lpszProgressTitle = NULL;
		fileop.hNameMappings = NULL;

		int ret = SHFileOperation(&fileop);
		delete[] pszFrom;
		return (ret == 0);
	}


}