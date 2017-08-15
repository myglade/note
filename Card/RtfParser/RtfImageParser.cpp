#include "StdAfx.h"
#include "RtfParser.h"
#include "RtfImage.h"
#include "..\Util.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

char        g_tag[8192];

struct MyMemoryFile
{
	BYTE *		m_buf;
	int			m_bufLen;
	int			m_pos;
	BOOL		m_borrow;

	MyMemoryFile()
	{
		m_bufLen = 100 * 1024;
		m_buf = new BYTE[m_bufLen]; 
		m_borrow = FALSE;
		m_pos = 0;
	}
	MyMemoryFile(BYTE *p, int len)
	{
		m_buf = p; m_bufLen = len; m_borrow = TRUE;
		m_pos = 0;
	}
	~MyMemoryFile()
	{
		if (m_borrow == FALSE && m_buf)
			delete m_buf;
	}
	void SetEnd()
	{
		m_buf[m_pos] = 0;
	}

	int Write(BYTE *p, int len)
	{
		if (!p || len <= 0)
			return 0;

		if (m_pos + len > m_bufLen)
		{
			int		newLen = m_bufLen;
			while(newLen < m_pos + len)
			{
				if (newLen >= 1024 * 1000)
					newLen = newLen * 3;
				else if (newLen >= 1024)
					newLen = newLen * 10;
			}

			BYTE *newBuf = new BYTE[newLen];
			memcpy(newBuf, m_buf, m_pos);
			delete m_buf;
			m_buf = newBuf;
			m_bufLen = newLen;
		}

		memcpy(m_buf + m_pos, p, len);
		m_pos += len;
		return len;
	}

	int Read(BYTE *p, int len)
	{
		if (m_buf == NULL || m_bufLen <= 0 || !p || len <= 0 ||
			m_pos >= m_bufLen)
			return 0;

		if (m_pos + len > m_bufLen)
		{
			len = m_bufLen - m_pos;
		}
		memcpy(p, m_buf + m_pos, len);
		m_pos += len;

		return len;
	}

};

void ToHexStr(const BYTE *data, int len, MyMemoryFile &file)
{
	if (data == NULL || len <= 0)
		return;

	const unsigned char *p = data;
	BYTE	c[3];
	unsigned int		v;

	c[2] = 0;
	for(int i = 0; i < len; i++)
	{
		v = p[i] >> 4;
		if (v >= 10)
			c[0] = (v - 10) + 'a';
		else
			c[0] = v + '0';

		v = p[i] & 0x0F;
		if (v >= 10)
			c[1] = (v - 10) + 'a';
		else
			c[1] = v + '0';

		file.Write(c, 2);
		if ((i + 1) % 39 == 0)
		{
			file.Write((BYTE *) "\r\n", 2);
		}
	}
	if (file.m_buf[file.m_pos - 1] != '\n')
        file.Write((BYTE *) "\r\n", 2);
}


void GetImageInfo(char *str, int len, int &width, int &height)
{
	width = height = 0;
	if (!str || len <= 0)
		return;

	char *p = strtok(str, "\\\r\n");
	while(p)
	{
		if (strncmp(p, "picwgoal", 8) == 0)
			width = PICGOAL_TO_PIXEL(atoi(p + 8)); 
		else if (strncmp(p, "pichgoal", 8) == 0)
			height = PICGOAL_TO_PIXEL(atoi(p + 8)); 
	//	else if (strncmp(p, "picw", 4) == 0)
	//		width = PIC_TO_PIXEL(atoi(p + 4)); 
	//	else if (strncmp(p, "pich", 4) == 0)
	//		height = PIC_TO_PIXEL(atoi(p + 4)); 

		p = strtok(NULL, "\\\r\n");
	}
}

int NextElement(char *&buf, char *element, int &l, int &prevTag, int *bracket = NULL)
{
	if (*buf == '}')
	{
        if (bracket) 
            --*bracket;

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
			prevTag = FALSE;
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
		prevTag = TRUE;
		return TYPE_TAG;
	}

	if (prevTag && *buf == ' ')
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

int ParseImageUrl(char *&p, unsigned int &id)
{
    char        hid[20];
    int         len = 0;
    int         type;
    int         bracket = 1;        // already 'field' bracket

    id = (unsigned int) -1;

    while(*p != ' ')
    {
        if (*p == '{')
            bracket++;
        p++;
    }

    while(*p == ' ')    // skip blank
        p++;

    if (*p == '"')    
        p++;

    if (strncmp(p, "hpict:\\\\", 8) == 0)
    {
        p += 10;
        while(*p != '"')
            hid[len++] = *p++;
        hid[len] = 0;

       // id = _atoi64(hid);
        id = atoi(hid);

    }
    else 
    {
        while(*p != '"' && *p != ' ')
        {
            if (*p == '\\')
            {
                p++;
                if (*p == '\\')
                    p += 2;
            }

            p++;
        }    
    }

    // after {HYPERLINK "http://www.apple.com ", strange chars may be inserted. skip them
    while(*(p-1) == '\\' || *p != '}')
        p++;
    p--;

    BOOL        prevTag = FALSE;
	
    type = NextElement(p, g_tag, len, prevTag, &bracket);
    len = 0;
    while(*p && bracket > 0)    // until field is processed completed
    {
		if (*(p-1) != '\\' && *p == '{')
		{
			bracket++;
			p++;
		}
        type = NextElement(p, g_tag, len, prevTag);
		switch(type)
		{
		case TYPE_TAG:
	        if (strncmp(g_tag, "pict", 4) == 0)
                return 2;
            break;
 		case TYPE_BRACKET_END:
            bracket--;
			break;
       }
    }

    return 0;
}


int ParseImage(char *& p, RtfImage &info, bool newImage)
{
	char *				start;
	char *				end;
	int					len;
	char				temp[255];
	int					k;
    int                 bracket = 0;

	k = 0;
    
	while(*p && *p != '\n')
	{
        if (*p == '{')
        {
            k = 0;
            do
            {
                if (*p == '{')
                    bracket++;
                else if (*p == '}')
                    bracket--;

                p++;
            } while (*p && bracket > 0);
        }
	    temp[k++] = *p++;
	}
	temp[k] = 0;
	GetImageInfo(temp, k, info.width, info.height);
    info.oheight = info.height;
    info.owidth = info.width;

	if (*p == 0 || info.width <= 0 || info.height <= 0 ||
	    info.width > 1280 || info.height > 1024)
	{
		return 100;
	}

	p++;
	start = p;
		
	while(*p != '}')
	    p++;

	end = p;
	len = (int) (end - start);

	if (len == 0)
        return -1;

    if (newImage == false)
    {
        info.data = NULL;
        return 0;
    }
	char *buf = new char[len];
	if (!buf)
	{
		return -1;
	}

	int		i = 0;
	while(start != end)
	{
		if (*start == ' ' || *start == '\r' || *start == '\n')
		{
			start++;
			continue;
		}

		buf[i++] = GetNumber(start);
		start += 2;
	}
		
	if (ConvertWmfToImageStream(buf, i, info) == -1)
	{
		delete buf;
		return -1;
	}
    delete buf;

	return 0;
}

int CRtfParser::DetachImage(LPCTSTR src, RtfImageList &srcMap, RtfImageList &list, CString &dst)
{
	RtfImage    		info;
	char *				p;
	char				t;
	int					count = 0;
	int					len;
	CString				s;
    RtfImageList::iterator  it;
    int                 type;
    int                 res, res2;
    char                *end, *start;
    BOOL                prevTag;
    char *              buf;
    unsigned int        id;
    bool                newImage;
    int                 bracket;
    int                 b = 0;

	list.clear();
	if (src == NULL)
		return -1;

    buf = new char[1024 * 1024];

    end = p = (char *) src;
	dst = "";
    prevTag = FALSE;

	while(*p)
	{
		while(*p == '\r' || *p == '\n')
		{
			prevTag = FALSE;
			p++;
		}

		if (*p == '{')
        {
            b++;
			p++;
        }
		else if (*p == '}')
        {
            b--;
			p++;
        }
		
        type = ::NextElement(p, buf, len, prevTag, &b);
		switch(type)
		{
		case TYPE_TAG:
			if (strncmp(buf, "field", 5) && strncmp(buf, "pict", 4))
                break;

            res = 0;
            id = (unsigned int) -1;
            bool startPict = false;

            if (strncmp(buf, "field", 5) == 0)
            {
                // rtf starts with '{' and ends with '}'
                // If { appears before, move back to it.  remove all.
                while(b > 1)
                {
                    p--;
                    if (*p == '{')
                        b--;
                }
                start = p++;
                res = ParseImageUrl(p, id);
                if (res == 0)
                    break;
            }
            else
            {
                start = p - 6;
                startPict = true;
            }
            t = *start; *start = 0;
            dst += end;
            *start = t;

            info.Reset();

            if (id == (unsigned int) -1)
            {
                id = (unsigned int) (time(NULL) % 65536) + count++;
                newImage = true;
            }
            else
            {
                it = srcMap.find(id);
                if (it == srcMap.end())
                {
                    newImage = true;
                }
                else
                    newImage = false;

                it = list.find(id);
                if (it != list.end())
                {
                    id = (unsigned int) (time(NULL) % 65536) + count++;
                    newImage = true;
                }
            }
            info.id = id;
            res2 = ::ParseImage(p, info, newImage);
            if (res == 1)
            {
                bracket = 0;
                while(bracket < 4)
                {
                    if (*p == '}')
                        bracket++;

                    p++;
                }
            }
            else
            {
                bracket = 0;
                int totalBracket;

                if (startPict)
                    totalBracket = 1;
                else
                    totalBracket = 2;

                while(bracket < totalBracket)
                {
                    if (*p == '}')
                        bracket++;

                    p++;
                }
            }

            if (res2 == -1)
            {
                ResetImageList(list);
                delete buf;
                return -1;
            }
            else if (res2 == 100)
            {
	            MessageBox(NULL, "Image is too big. Please, use smaller imager (max 1280x1024)", "Error", MB_OK);
                ResetImageList(list);
                delete buf;
                return -1;
            }
            list[info.id] = info;
		    s.Format("\\$$PICT_%u ", info.id);
		    dst += s;

            char *t = p;

            while(*t != '\n' && *t != '{')
            {
                if (*t == '}')
                {
                    p = t + 1;
                }
                t++;
            }

            /*
            while(*p == ' ' || *p == '}') 
            {
                TRACE("INVALID!!! \n");
                p++;
            }
            */
            end = p;
		    break;
	    }
    }
    dst += end;
    delete buf;

	return 0;
}

int CRtfParser::AttachImage(LPCTSTR src, RtfImageList &list, CString &dst)
{
	//RtfImage    		info;
	char *				start;
	char *				p;
	char				t;
	CString				s;
	MyMemoryFile		file;
    unsigned long       id;
    RtfImageList::iterator it;
    char                tmp[256];
    int                 len;

	if (src == NULL)
		return -1;

	dst = "";
	p = (char *)src;
	start = p;
	while(p = strstr(p, "\\$$PICT_"))
	{
		t = *p; *p = 0;
		file.Write((BYTE *) start, p - start);
		*p = t;

        char *p1 = p;
		while(*p1 != ' ')
            p1++;
        t = *p1; *p1 = 0;
        id = (unsigned int) _atoi64(p + 8);
        *p1 = t;

        it = list.find(id);
        if (it == list.end())
            return -1;
        
        len = _snprintf(tmp, 255, 
            "{\\field{\\*\\fldinst{HYPERLINK \"hpict:\\\\%u\" }}"
            "{\\fldrslt{{\\pict\\wmetafile8\\picwgoal%d\\pichgoal%d \r\n", 
            id, PIXEL_TO_PICGOAL(it->second.width), PIXEL_TO_PICGOAL(it->second.height));
        file.Write((BYTE *) tmp, len);
        if (ConvertImageToWmf("wmf", it->second) == -1)
			return -1;

		ToHexStr(it->second.data.get(), it->second.len, file);

        len = _snprintf(tmp, 255, "}}}}");
        file.Write((BYTE *) tmp, len);
        
		p = p1;
		start = p;
	}
	file.Write((BYTE *) start, strlen(start));
	file.SetEnd();
	dst = file.m_buf;

	return 0;
}

int CRtfParser::ConvertImage(LPCTSTR src, CString &dst)
{
	char *				p, *p1;
	char				t;
    int                 type;
    char                *end, *start;
    BOOL                prevTag;
    char *              buf;
    char                tmp[255];
    int                 len;

	if (src == NULL)
		return -1;

    buf = new char[1024 * 1024];

    end = p = (char *) src;
	dst = "";
    prevTag = FALSE;

	while(*p)
	{
		while(*p == '\r' || *p == '\n')
		{
			prevTag = FALSE;
			p++;
		}

		if (*p == '{')
			p++;
		else if (*p == '}')
			p++;
		
        type = ::NextElement(p, buf, len, prevTag);
		switch(type)
		{
		case TYPE_TAG:
			if (strncmp(buf, "field", 5) && strncmp(buf, "pict", 4))
                break;

            if (strncmp(buf, "field", 5) == 0)
                start = p - 7;
            else
                start = p - 6;

            int bracket = 1;
            while(bracket > 0)
            {
                if (*p == '{')
                    bracket++;
                else if (*p == '}')
                    bracket--;

                p++;
            }
            t = *p; *p = 0;
            p1 = strstr(start, "\\$$PICT_");
            *p = t;
            if (p1 == NULL)
                break;

            t = *start;
            *start = 0;
            dst += end;
            *start = t;

            int k = 0;
            while(*p1 != '}')
            {
                tmp[k++] = *p1++;
            }
            tmp[k++] = ' ';
            tmp[k] = 0;

		    dst += tmp;

            end = p;
		    break;
	    }
    }
    dst += end;
    delete buf;

	return 0;
}
