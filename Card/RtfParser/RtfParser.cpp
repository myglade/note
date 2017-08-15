#include "StdAfx.h"
#include "RtfParser.h"
#include "RtfImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CRtfParser::CRtfParser(void)
{
    m_curPos = m_buf = NULL;
	m_buf = NULL;
	m_dst = new char[1024 * 1024];
	m_imageId = 0;
    m_imageList = NULL;
}

CRtfParser::~CRtfParser(void)
{
	if (!m_borrowBuffer && m_buf)
		delete m_buf;

	if (m_dst)
		delete m_dst;
}

int CRtfParser::Open(CString name)
{
	CFile		file;

	if (file.Open(name, CFile::modeRead) == FALSE)
	{
		return -1;
	}
	if (m_buf)
		delete m_buf;
	m_bufLen = (int) file.GetLength();
	m_buf = new char[m_bufLen + 1];
    m_curPos = m_buf;

	file.Read(m_buf, m_bufLen);
	m_buf[m_bufLen] = 0;
	file.Close();
	m_borrowBuffer = FALSE;

	return 0;
}

int CRtfParser::OpenFromString(const char *p, RtfImageList *imageList)
{
	if (p == NULL)
		return -1;

	m_buf = (char *) p;
    m_curPos = m_buf;

    m_borrowBuffer = TRUE;
	m_bufLen = strlen(p);
	m_imageList = imageList;

	return 0;
}

int CRtfParser::GetFontTable(char *&p)
{
	char		tmp[255], tmp2[255];
	int			k;
	char *		p1;

	p = strstr(p, "\\fonttbl");
	if (p == NULL)
		return -1;

	for(int i = 0; i < 100; i++)
	{
		_sntprintf(tmp, 255, "\\f%d\\", i);
		p1 = strstr(p, tmp);
		if (p1 == NULL)
			break;
		p = p1;
		while(*p != ' ')
			p++;
		p++;
		k = 0;
		while(*p != ';')
			if (*p == '\\' && *(p + 1) == '\'')
			{
				p += 2;
				tmp2[k++] = GetNumber(p);
				p += 2;
			}
			else
				tmp2[k++] = *p++;

		tmp2[k] = 0;
		_sntprintf(tmp, 255, "f%d", i);
		m_fontTable.Add(tmp2);
        p += 2;
	}
    p++;
    if (*p == '\r')
        p++;
    if (*p == '\n')
        p++;

	return 0;
}

int CRtfParser::GetColorTable(char *&p)
{
	char		tmp[255];
	int			k;
	Rgb			rgb;
	char *		p1;

	p1 = strstr(p, "\\colortbl");
	if (p1 == NULL)
	{
        p1 = strstr(p, "viewkind");
        if (p == NULL)
        {
            return -1;
        }
        p1 += 9;;
        p = p1;
        return 0;
	}
	p = p1;
	p += 10;
	if (*p == ';')
	{
		rgb.r = "00";
		rgb.g = "00";
		rgb.b = "00";
		m_colorTable.Add(rgb);
		p++;
	}
	while(1)
	{
		if (strncmp(p, "\\red", 4) != 0)
		{
			return -1;
		}
		p += 4;
		k = 0;
		while(*p != '\\')
			tmp[k++] = *p++;
		tmp[k] = 0;
		rgb.r.Format("%02X", atoi(tmp));

		if (strncmp(p, "\\green", 6) != 0)
		{
			return -1;
		}
		p += 6;
		k = 0;
		while(*p != '\\')
			tmp[k++] = *p++;
		tmp[k] = 0;
		rgb.g.Format("%02X", atoi(tmp));

		if (strncmp(p, "\\blue", 5) != 0)
		{
			return -1;
		}
		p += 5;
		k = 0;
		while(*p != ';')
			tmp[k++] = *p++;
		tmp[k] = 0;
		rgb.b.Format("%02X", atoi(tmp));
		m_colorTable.Add(rgb);

		p++;
		if (*p == '}')
			break;
	}
	p++;
	return 0;

}

int CRtfParser::Parse(CRtfParser::CListener *listener, LPCTSTR listenerId)
{
	char *	p1;
	int		l;
	int		type;

	if (m_bufLen == 0 || !listener)
		return -1;

	m_fontTable.RemoveAll();
	m_colorTable.RemoveAll();

	m_dst[0] = 0;
	m_listener = listener;
    p1 = m_curPos = m_buf;
	m_startSpace = 0;
	m_bracket = 0;
	m_curFont = 0;
	m_prevTag = FALSE;
	m_imageId = 0;
	m_lineHeightCalled = false;

	if (GetFontTable(p1) == -1)
		return -1;
	if (GetColorTable(p1) == -1)
		return -1;
	
    m_listener->m_parser = this;
	m_listener->Reset(listenerId);
	m_listener->OnStart();

	while(*p1 && !m_listener->m_stop)
	{
		while(*p1 == '\r' || *p1 == '\n')
		{
			m_prevTag = FALSE;
			p1++;
		}
		if (*p1 == '{')
		{
			m_bracket++;
			p1++;
		}
		else if (*p1 == '}')
		{
			m_bracket--;
			p1++;
		}
		
        m_curPos = p1;
		type = NextElement(p1, m_dst, l);
		switch(type)
		{
		case TYPE_TAG:
			OnTag(m_dst, l, p1);
			break;
		case TYPE_TEXT:
			OnText(m_dst, l);
			break;
		case TYPE_LINE_FEED:
			break;
		case TYPE_BUF_END:
			break;
		}
	}
	m_listener->OnEnd();
	if (m_borrowBuffer)
		m_buf = 0;

	return 0;
}

void CRtfParser::SkipBracket(char *&p)
{
	while(m_bracket > 0 && *p)
	{
		if (*p == '}')
			m_bracket--;
		else if (*p== '{')
			m_bracket++;

		p++;
	}
}

int CRtfParser::NextElement(char *&buf, char *element, int &l)
{
	if (*buf == '}')
	{
 //       m_bracket--;
		buf++;
		element[0] = 0;
		return TYPE_BRACKET_END;
	}

	if (IsTag(buf))
	{
		buf++;
		l = 0;
		if (*buf == '~' || *buf == '-')
		{
			element[l++] = *buf++;
			element[l] = 0;
			m_prevTag = FALSE;
			return TYPE_TAG;
		}

        while(*buf != '\\' && *buf != ' ' && *buf != '\r' && *buf != '{' && *buf != '}')
		{
			element[l++] = *buf++;
		}
		if (*buf == '\n')
		{
			buf++;
			return TYPE_LINE_FEED;
		}
		element[l] = 0;
		m_prevTag = TRUE;
		return TYPE_TAG;
	}

	if (m_prevTag && *buf == ' ')
		buf++;
	// data
    int     specialChar = 0;

	l = 0;
	while(*buf)
	{
        if ((specialChar % 2 == 1) && 
            !(*buf == '\\' && *(buf+1) == '\''))
        {
            if (l > 1)
            {
                while(*buf != '\\')
                {
                    buf--;
                }
 			    element[l-1] = 0;
		        return TYPE_TEXT;
           }
			element[l] = 0;
		    return TYPE_TAG;
        }

		if (IsTag(buf))
		{
			element[l] = 0;
			return TYPE_TEXT;
		}
		if (*buf == '\\')
		{
			buf++;
			if (*buf == '\'')
			{
				buf++;
				element[l++] = GetNumber(buf);
				buf += 2;
                specialChar++;
				continue;
			}
		}
		else if (*buf == '\r')
		{
			buf += 2;
			element[l] = 0;
			return TYPE_LINE_FEED;
		}
        else if (*buf == '}' || *buf == '{')
		{
			element[l] = 0;
			return TYPE_TEXT;
		}

		element[l++] = *buf++;
	}

	return TYPE_BUF_END;
}

void CRtfParser::OnLineFeed()
{
	printf("line feed\n");
}

void CRtfParser::OnTag(const char *p, int len, char *&buffer)
{
	int			v;

	if (m_listener == NULL)
		return;

	if (strcmp(p, "pard") == 0)
	{
		m_startSpace = 0;
		m_lineHeightCalled = FALSE;
	}
	else if ((strcmp(p, "par") == 0) || (strcmp(p, "line") == 0))
	{
		m_listener->OnLineEnd();
		m_newLine = TRUE;
		m_prefix = "";
	}
	else if (strncmp(p, "slmult", 6) == 0)	// skip
	{
	}
	else if (strncmp(p, "sl", 2) == 0)	// line height
	{
		v = atoi(p + 2) * 100 / 240;
		m_listener->OnLineHeight(v);
		m_lineHeightCalled = true;
	}
	else if (strncmp(p, "field", 5) == 0)
	{
		ParseUrl(buffer);
	}
	else if (strncmp(p, "fi-", 3) == 0)	// skip
	{
	}
	else if (strncmp(p, "fi", 2) == 0)	// ident
	{
		m_startSpace += 4;
	}
	else if (strncmp(p, "fs", 2) == 0)
	{
		m_listener->OnFontSize(atoi(p + 2));
	}
	else if (strncmp(p, "cf", 2) == 0)
	{
		v = atoi(p + 2);
		m_listener->OnFontColor(m_colorTable[v]);
	}
	else if (strncmp(p, "pict", 4) == 0)
	{
		ParseImage(buffer);
	}
	else if (strncmp(p, "$$PICT", 6) == 0)
	{
		ParseImage(p);
	}
	else if (strncmp(p, "li", 2) == 0)
	{	// li1080 li540 li1620
		// only if it is bigger than 540, put space.
		v = atoi(p + 2);
		if (v >= 540)
		{
			v = v / SPACE_LENGTH;
			m_startSpace += v;
		}
	}
	else if (strcmp(p, "~") == 0)
	{
		if (m_newLine && m_startSpace > 0)
			m_listener->OnSpace(m_startSpace);
		m_newLine = FALSE;

		m_listener->OnSpace(1);
	}
	else if (strcmp(p, "bullet") == 0 || 
        (unsigned char) p[0] == RTF_BULLET)
	{
		if (m_newLine && m_startSpace > 0)
			m_listener->OnSpace(m_startSpace);
		m_newLine = FALSE;

		m_listener->OnBullet();
	}
	else if (strcmp(p, "tab") == 0)
	{
		if (m_newLine && m_startSpace > 0)
			m_listener->OnSpace(m_startSpace);
		m_newLine = FALSE;

		m_listener->OnTab();
	}
	else if (p[0] == 'f')
	{
		v = atoi(p + 1);
		m_curFont = v;
		m_listener->OnFont(v, m_fontTable[v]);
	}
	else if (strcmp(p, "qc") == 0)
	{
		m_listener->OnCenterAlign();
	}
	else if (strcmp(p, "b") == 0)
	{
		m_listener->OnBold(TAG_START);
	}
	else if (strcmp(p, "b0") == 0)
	{
		m_listener->OnBold(TAG_END);
	}
	else if (strcmp(p, "i") == 0)
	{
		m_listener->OnItalic(TAG_START);
	}
	else if (strcmp(p, "i0") == 0)
	{
		m_listener->OnItalic(TAG_END);
	}
	else if (strcmp(p, "strike") == 0)
	{
		m_listener->OnStrike(TAG_START);
	}
	else if (strcmp(p, "strike0") == 0)
	{
		m_listener->OnStrike(TAG_END);
	}
	else if (strcmp(p, "ul") == 0)
	{
		m_listener->OnUnderline(TAG_START);
	}
	else if (strcmp(p, "ulnone") == 0)
	{
		m_listener->OnUnderline(TAG_END);
	}
	else if (strcmp(p, "super") == 0)
	{
		m_listener->OnSuper();
	}
	else if (strcmp(p, "sub") == 0)
	{
		m_listener->OnSub();
	}
	else if (strcmp(p, "nosupersub") == 0)
	{
		m_listener->OnEndSubSuper();
	}
	else if (strcmp(p, "ldblquote") == 0 || strcmp(p, "rdblquote") == 0)
	{
		if (m_newLine && m_startSpace > 0)
			m_listener->OnSpace(m_startSpace);
		m_newLine = FALSE;

		m_listener->OnText("\"", 1);
	}
	else if (strcmp(p, "lquote") == 0 || strcmp(p, "rquote") == 0)
	{
		if (m_newLine && m_startSpace > 0)
			m_listener->OnSpace(m_startSpace);
		m_newLine = FALSE;

		m_listener->OnText("'", 1);
	}
	else if (strcmp(p, "emdash") == 0)
	{
		if (m_newLine && m_startSpace > 0)
			m_listener->OnSpace(m_startSpace);
		m_newLine = FALSE;

		m_listener->OnText("--", 2);
	}
	else if (strcmp(p, "endash") == 0)
	{
		if (m_newLine && m_startSpace > 0)
			m_listener->OnSpace(m_startSpace);
		m_newLine = FALSE;

		m_listener->OnText("-", 1);
	}
	else if (strcmp(p, "emspace") == 0 || strcmp(p, "enspace") == 0)
	{
		if (m_newLine && m_startSpace > 0)
			m_listener->OnSpace(m_startSpace);
		m_newLine = FALSE;

		m_listener->OnText(" ", 1);
	}
	else if (strcmp(p, "pntext") == 0 && m_bracket > 0)
	{
		ProcessList(buffer);
	}
	else if (strcmp(p, "lang1042") == 0)
	{
	}
	// unsupported tag {}
	else if (m_bracket > 0)
	{
		SkipBracket(buffer);
	}
}

void CRtfParser::OnText(const char *p, int len)
{
	if (m_listener == NULL)
		return;

	if (m_newLine)
	{
		if (m_startSpace > 0)
			m_listener->OnSpace(m_startSpace);

		if (m_lineHeightCalled == FALSE)
		{
			m_listener->OnLineHeight(100);
			m_lineHeightCalled = TRUE;
		}

		if (m_prefix != "")
			m_listener->OnText((LPCTSTR) m_prefix, m_prefix.GetLength());
		m_newLine = FALSE;
	}

	m_listener->OnText(p, len);

}

void CRtfParser::ProcessList(char *&p)
{
	char	element[128];
	int		len;
	int		type;

	while((type = NextElement(p, element, len)) != TYPE_BRACKET_END)
	{
		if (type == TYPE_TEXT)
		{
			if ((BYTE) element[0] == RTF_BULLET)
				element[0] = BULLET_TEMP;
			
			m_prefix = element;
			m_newLine = TRUE;
		}
        else if (type == TYPE_TAG)
        {
			if ((BYTE) element[0] == RTF_BULLET)
		        m_listener->OnBullet();

        }
	}
	m_bracket--;
}

int CRtfParser::ParseImage(const char *p)
{
    RtfImageList::iterator  it;
//	CString		    path;
	CString		    name;
	CFile		    file;
    unsigned int    id;

    if (m_imageList == NULL)
        return 0;

    while(*p != '_')
        p++;
    
    p++;

    id = atoi(p);
    it = (*m_imageList).find(id);
    if (it == (*m_imageList).end())
        return -1;

	m_listener->OnImage(id, it->second.owidth, it->second.oheight, 
        it->second.width, it->second.height, it->second.type, it->second.http_path, name);

/*
	if (path != "" && path[path.GetLength() - 1] != '\\')
		path += "\\";

	path += name;
	if (file.Open(path, CFile::modeCreate | CFile::modeWrite) == FALSE)
		return -1;

	file.Write(it->second.data.get(), it->second.len);
	file.Close();
*/
	m_imageId++;

	return 0;
}



// {\field{\*\fldinst{HYPERLINK "http://www.apple.com "}}{\fldrslt{\ul jim@dunnes.net}}}
int CRtfParser::ParseUrl(char *&p)
{
    char    url[8192];
    int     urlLen = 0;
    int     orgBracket = m_bracket - 1;

    while(*p != ' ')
    {
        if (*p == '{')
            m_bracket++;
        p++;
    }

    while(*p == ' ')    // skip blank
        p++;

    if (*p == '"')    
        p++;

    while(*p != '"' && *p != ' ')
    {
        if (*p == '\\')
        {
            p++;
            if (*p == '\\')
                p += 2;
        }
        url[urlLen++] = *p++;
    }
    url[urlLen] = 0;
    // after {HYPERLINK "http://www.apple.com ", strange chars may be inserted. skip them
    while(*(p-1) == '\\' || *p != '}')
        p++;
    p--;

	char		tag[32];
	int			len = 0;
	int         type = NextElement(p, tag, len);

    while(m_bracket > orgBracket)
    {
		if (*(p-1) != '\\' && *p == '{')
		{
			m_bracket++;
			p++;
		}
        type = NextElement(p, m_dst, len);
		switch(type)
		{
		case TYPE_TAG:
	        if (strncmp(m_dst, "pict", 4) == 0)
	        {
		        ParseImage(p);
	        }
            else if (strncmp(m_dst, "ulnone", 6) == 0)  // trick.  
            {
                m_listener->OnText(" ", 1);
            }
			break;
		case TYPE_TEXT:
            m_listener->OnUrl(url, urlLen, m_dst, len);
			break;
 		case TYPE_BRACKET_END:
            m_bracket--;
			break;
       }
    }

    return 0;
}

int CRtfParser::CListener::GetUtfText(CString &s, CStringA &utf8)
{
	utf8 = "";
	if (s == "")
		return 0;

	WCHAR *		strW = new WCHAR[s.GetLength() + 1];

	int len = MultiByteToWideChar(CP_ACP, 0, s, -1, strW, s.GetLength() + 1);
	if (len <= 0)
	{
		delete strW;
		return -1;
	}
	utf8 = CW2A(strW, CP_UTF8);

	delete strW;
	return 0;
}

int CRtfParser::CListener::GetUtfText(CStringA &utf8)
{
	return GetUtfText(m_text, utf8);
}

void ImageStringConvertTest()
{
	CFile		file;

	return;
	if (file.Open("c:\\a.txt", CFile::modeRead) == FALSE)
		return;

	char *		buf;

	buf = new char[(int) file.GetLength() + 1];
	int len =(int)  file.GetLength();
	file.Read(buf, len);
	file.Close();
	buf[len] = 0;

		char *buf2 = new char[len];

		int		i = 0;
		char * p = buf + 3;

		while(*p)
		{
			if (*p == ' ' || *p == '\r' || *p == '\n')
			{
				p++;
				continue;
			}

			buf2[i++] = GetNumber(p);
			p += 2;
		}
	file.Open("c:\\a.wmf", CFile::modeCreate | CFile::modeWrite);
	file.Write(buf2, i);
	file.Close();


	RtfImageList list;
	CString	dst;

//	CRtfParser::DetachImage(buf, list, dst);

#if 0
	CFile		file;

//	ConvertTest();
	if (file.Open("e:\\a1.jpg", CFile::modeRead) == FALSE)
		return;

	char *		buf;

	buf = new char[(int) file.GetLength()];
	int len =(int)  file.GetLength();
	file.Read(buf, len);
	file.Close();
	
	BYTE *dst;
	int dstLen;
	int	width, height;

	if (RtfImage::ConvertImage(buf, len, dst, dstLen, width, height, "wmf") == -1)
	{
		delete buf;
		return;
	}
	delete buf;

	CString str;
	ToHexStr(dst, dstLen, str);

	file.Open("e:\\result.txt", CFile::modeCreate | CFile::modeWrite);
	file.Write(str, str.GetLength() + 1);
	file.Close();
	width = (int) ((float) width * (float)1440 / (float)96);
	height = (int) ((float) height * (float)1440 / (float)96);
	delete dst;
#endif
}
