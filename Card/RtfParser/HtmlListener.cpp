#include "StdAfx.h"
#include "HtmlListener.h"


void CHtmlListener::OnStart()
{
	char dir[512];
	GetCurrentDirectory(512, dir);
	CString s;
	
	s.Format("%s\\%s", dir, HTML_PATH);
	SHCreateDirectoryEx(NULL, s, NULL);

	m_fi.font = 0;
	m_fi.size = -1;
    m_fi.fontName = m_parser->GetFontTable()[0];
    m_fi.color = "000000";
    m_fi.lineHeight = 130;
    m_fi.index = -1;
    m_text = "";

    m_boldChange = FALSE;
    m_italicChange = FALSE;
    m_underlineChange = FALSE;
    m_subChange = FALSE;
    m_superChange = FALSE;
    m_strikeChange = FALSE;
    m_fiChange = FALSE;
}

void CHtmlListener::OnEnd()
{
    CloseTag();

	if (m_fontStart == TRUE)
		m_text += "</span>";

    if (m_attachCss)
        m_text = GetCss() + m_text;
}

void CHtmlListener::ClearCss()
{
    m_fiIndex = -1;
    m_fontMap.clear();
}

CString CHtmlListener::GetCss()
{
    FontMap::iterator		it;
    CString					s, t;

    for (it = m_fontMap.begin(); it != m_fontMap.end(); it++)
    {
        string  name;

        if (it->second.fontName == "Symbol")
            name = "Arial";
        else
            name = it->second.fontName;

        s.Format(".f%d {"
            "font-size: %dpx;"
            "font-family: %s,%s;"
            "color: #%s;line-height:%d%%}\n",
            it->second.index,
            it->second.size,
            name.c_str(), ALT_FONT_LIST,
            it->second.color.c_str(),
            it->second.lineHeight);

        t += s;
    }

    s.Format("<STYLE TYPE=\"text/css\"><!--\n%s\n-->\n</STYLE>\n",
        t);

    return s;
}

void CHtmlListener::CloseTag()
{
	if (!m_superChange && m_superStart)
		m_text += "</sup>";

    if (!m_subChange && m_subStart)
		m_text += "</sub>";

    if (!m_underlineChange && m_underlineStart)
        m_text += "</u>";

    if (!m_italicChange && m_italicStart)
        m_text += "</i>";

    if (!m_boldChange && m_boldStart)
        m_text += "</b>";

    if (!m_strikeChange && m_strikeStart)
        m_text += "</del>";
}

void CHtmlListener::OpenTag()
{
    if (!m_strikeChange && m_strikeStart)
        m_text += "<del>";

    if (!m_boldChange && m_boldStart)
        m_text += "<b>";

    if (!m_italicChange && m_italicStart)
        m_text += "<i>";

    if (!m_underlineChange && m_underlineStart)
        m_text += "<u>";

    if (!m_subChange && m_subStart)
		m_text += "<sub>";

	if (!m_superChange && m_superStart)
		m_text += "<sup>";
}

void CHtmlListener::HandleFont()
{
    if (m_superChange && !m_superStart)
    {
        m_text += "</sup>";
        m_superChange = FALSE;
    }
    if (m_subChange && !m_subStart)
    {
        m_text += "</sub>";
        m_subChange = FALSE;
    }
    if (m_underlineChange && !m_underlineStart)
    {
        m_text += "</u>";
        m_underlineChange = FALSE;
    }
    if (m_italicChange && !m_italicStart)
    {
        m_text += "</i>";
        m_italicChange = FALSE;
    }
    if (m_boldChange && !m_boldStart)
    {
        m_text += "</b>";
        m_boldChange = FALSE;
    }
    if (m_strikeChange && !m_strikeStart)
    {
        m_text += "</del>";
        m_strikeChange = FALSE;
    }

    if (m_fiChange)
    {
        BOOL    closeTag = FALSE;

	    if (m_fontStart)
	    {
            CloseTag();
            closeTag = TRUE;
		    m_text += "</span>";
	    }

	    CString				s;
	    CString				key;
	    FontMap::iterator	it;

        key.Format("%d-%d-%s-%d", m_fi.font, m_fi.size, m_fi.color.c_str(), 
            m_fi.lineHeight);
    	
	    it = m_fontMap.find((LPCTSTR)key);
	    if (it == m_fontMap.end())
	    {
            m_fi.index = ++m_fiIndex;
		    m_fontMap[(LPCTSTR)key] = m_fi;
            s.Format("<span class=\"f%d\">", m_fi.index);
	    }
        else
            s.Format("<span class=\"f%d\">", it->second.index);

	    m_text += s;
        m_fiChange = FALSE;
	    m_fontStart = TRUE;
        if (closeTag)
            OpenTag();
    }


    if (m_strikeChange && m_strikeStart)
    {
        m_text += "<del>";
        m_strikeChange = FALSE;
    }
    if (m_boldChange && m_boldStart)
    {
        m_text += "<b>";
        m_boldChange = FALSE;
    }
    if (m_italicChange && m_italicStart)
    {
        m_text += "<i>";
        m_italicChange = FALSE;
    }
    if (m_underlineChange && m_underlineStart)
    {
        m_text += "<u>";
        m_underlineChange = FALSE;
    }
    if (m_subChange && m_subStart)
    {
        m_text += "<sub>";
        m_subChange = FALSE;
    }
    if (m_superChange && m_superStart)
    {
        m_text += "<sup>";
        m_superChange = FALSE;
    }
}

void CHtmlListener::OnLineEnd()
{
    HandleFont();
	m_text += "\r\n";
	m_lineStart = TRUE;
}

void CHtmlListener::OnFontColor(Rgb &rgb)
{
	string		s;

    s = rgb.r + rgb.g + rgb.b;
    if (s == m_fi.color)
        return;

    m_fiChange = TRUE;
    m_fi.color = s;

	// mac and window palette is reverse
//	if (s == "<font color=\"#000000\">")
//		s = "<font color=\"#ffffff\">";
//	else if (s == "<font color=\"#ffffff\">")
//		s = "<font color=\"#000000\">";
}

void CHtmlListener::OnBullet()
{
    HandleFont();
	m_text += "&bull;";
}

void CHtmlListener::OnTab()
{
    HandleFont();
    m_text += "    ";
}

void CHtmlListener::OnFont(int index, CString &font)
{
    if (index == m_fi.font)
        return;

    m_fiChange = TRUE;
    m_fi.font = index;
    m_fi.fontName = font;
}

void CHtmlListener::OnFontSize(int size)
{
	int     fontSize = size / 2 + 3;

    if (fontSize == m_fi.size)
        return;
    
    m_fiChange = TRUE;
    m_fi.size = fontSize;
}


void CHtmlListener::OnLineHeight(int height)
{
	CString		s;

    height = abs(height);
	if (height == 100)
		height += 10;
	else
		height += 30;

    height = (height + 9) / 10 * 10;
    if (height == m_fi.lineHeight)
        return;

    m_fiChange = TRUE;
    m_fi.lineHeight = height;
    return;
}

void CHtmlListener::OnBold(int type)
{
    m_boldChange = TRUE;
	if (type == TAG_START)
	{
//		m_text += "<b>";
		m_boldStart = TRUE;
	}
	else
	{
//		m_text += "</b>";
		m_boldStart = FALSE;
	}
}

void CHtmlListener::OnUnderline(int type)
{
    m_underlineChange = TRUE;
	if (type == TAG_START)
	{
//		m_text += "<u>";
		m_underlineStart = TRUE;
	}
	else
	{
//		m_text += "</u>";
		m_underlineStart = FALSE;
	}
}

void CHtmlListener::OnItalic(int type)
{
    m_italicChange = TRUE;
	if (type == TAG_START)
	{
//		m_text += "<i>";
		m_italicStart = TRUE;
	}
	else
	{
//		m_text += "</i>";
		m_italicStart = FALSE;
	}
}

void CHtmlListener::OnStrike(int type)
{
    m_strikeChange = TRUE;
	if (type == TAG_START)
	{
//		m_text += "<i>";
		m_strikeStart = TRUE;
	}
	else
	{
//		m_text += "</i>";
		m_strikeStart = FALSE;
	}
}

void CHtmlListener::OnSub()
{
    m_subChange = TRUE;
//	m_text += "<sub>";
	m_subStart = TRUE;
}

void CHtmlListener::OnSuper()
{
    m_superChange = TRUE;
//	m_text += "<sup>";
	m_superStart = TRUE;
}

void CHtmlListener::OnEndSubSuper()
{
	if (m_subStart)
    {
        m_subChange = TRUE;
//		m_text += "</sub>";
	    m_subStart = FALSE;
    }
	if (m_superStart)
    {
        m_superChange = TRUE;
//		m_text += "</sup>";
	    m_superStart = FALSE;
    }
}

void CHtmlListener::OnSpace(int count)
{
    HandleFont();

    CString		s;
	for(int j = 0; j < count; j++)
	{
		s += " ";
	}
	m_text += s;
}


void CHtmlListener::OnUrl(const char *url, int urlLen, const char *display, int displayLen)
{
	CString		s;

    HandleFont();
    if (strncmp(url, "card://", 7) == 0)
    {
        s.Format("<a href=\"/db/query2?type=2&%s\" target=\"_blank\">%s</a>", url + 7, display);
    }
    else if (strncmp(url, "http", 4) == 0)
        s.Format("<a href=\"%s\" target=\"_blank\">%s</a>", url, display);
    else if (strncmp(url, "onenote:", 8) == 0)
        s.Format("<a href=\"%s\">%s</a>", url, display);
    else
        s = display;

    m_text += s;
}

void CHtmlListener::OnText(const char *data, int len)
{
	int		i;
	CString	str;

    HandleFont();
	if (m_lineStart)
	{
		for(i = 0; i < len; i++)
		{
			if (data[i] != ' ')
				break;
		}
		CString		s;
		for(int j = 0; j < i; j++)
		{
			s += " ";
		}
		m_text += s;
		if (i >= len)
			return;

		m_lineStart = FALSE;
		str = (data + i);
		str.Replace("&", "&amp;");
		str.Replace("<", "&lt;");
		str.Replace(">", "&gt;");
		m_text += str;
	}
	else
	{
		str = data;
		str.Replace("&", "&amp;");
		str.Replace("<", "&lt;");
		str.Replace(">", "&gt;");

		m_text += str;
	}
}

void CHtmlListener::OnImage(int id, int owidth, int oheight, int width, int height, 
							LPCTSTR type, CString &path, CString &name)
{
	CString		s;

    /*
	path = HTML_PATH;
	name.Format("%s_%d.%s", m_listenerId, id, type);
	s.Format("<img src=\"html_media/%s\" alt=\"&lt;&lt;image&gt;&gt;\" "
		"width=\"%d\" height=\"%d\" />", name, width, height);
*/

	s.Format("<img src=\"%s\" alt=\"&lt;&lt;image&gt;&gt;\" "
		"width=\"%d\" height=\"%d\" />", path, width, height);

	m_text += s;
}
