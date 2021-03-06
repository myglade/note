#include "StdAfx.h"
#include "FcListener.h"

#define tab_size 2

void CFcListener::ClearFolder()
{
	char			dir[512];
	CFileFind		finder;
	CString			path;

	GetCurrentDirectory(512, dir);
	path.Format("%s\\%s*.*", dir, FC_PATH);
	BOOL bWorking = finder.FindFile(path);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		DeleteFile(finder.GetFilePath());
	}
}

void CFcListener::OnStart()
{
	char dir[512];
	GetCurrentDirectory(512, dir);
	CString s;
	
	s.Format("%s\\%s", dir, FC_PATH);
	SHCreateDirectoryEx(NULL, s, NULL);
}

void CFcListener::OnEnd()
{
	if (m_centerAlign)
		m_text += "</center>";

	if (m_colorStart == TRUE)
		m_text += "</color>";

	if (m_sizeStart == TRUE)
		m_text += "</size>";

	if (m_boldStart == TRUE)
		m_text += "</b>";

	if (m_underlineStart == TRUE)
		m_text += "</u>";

	if (m_italicStart == TRUE)
		m_text += "</i>";


//	printf("%s\n", m_text);
}


void CFcListener::OnLineEnd()
{
//	m_text += "<br>";
	if (m_centerAlign)
	{
		m_text += "</center>";
		m_centerAlign = FALSE;
	}
	m_text += "|";
	m_lineStart = TRUE;
}

void CFcListener::OnFontSize(int size)
{
	if (m_sizeTag == FALSE)
		return;

	if (m_sizeStart)
	{
		m_text += "</size>";
	}
	m_sizeStart = TRUE; 
	CString		s;

	s.Format("<size %d>", size + 0);
	m_text += s;
}

void CFcListener::OnFontColor(Rgb &rgb)
{
	if (m_colorStart)
	{
		m_text += "</color>";
	}
	m_colorStart = TRUE;
	CString		s;

	s.Format("<color #%s%s%s>", rgb.r, rgb.g, rgb.b);

	// mac and window palette is reverse
	if (s == "<color #000000>")
		s = "<color #ffffff>";
	else if (s == "<color #ffffff>")
		s = "<color #000000>";

	m_text += s;
}

void CFcListener::OnBullet()
{
	m_text += (char) BULLET_TEMP;
}

void CFcListener::OnTab()
{
	if (m_lineStart)
	{
		m_text += "<sp>";
	}
	else
		m_text += "    ";
}

void CFcListener::OnFont(int index, CString &font)
{

}

void CFcListener::OnCenterAlign()
{
	m_text += "<center>";
	m_centerAlign = TRUE;
}


void CFcListener::OnBold(int type)
{
	if (type == TAG_START)
	{
		if (m_boldStart)
		{
			m_text += "</b>";
		}
	
		m_text += "<b>";
		m_boldStart = TRUE;
	}
	else
	{
		m_text += "</b>";
		m_boldStart = FALSE;
	}
}

void CFcListener::OnUnderline(int type)
{
	if (type == TAG_START)
	{
		if (m_underlineStart)
		{
			m_text += "</u>";
		}

		m_text += "<u>";
		m_underlineStart = TRUE;
	}
	else
	{
		m_text += "</u>";
		m_underlineStart = FALSE;
	}
}

void CFcListener::OnItalic(int type)
{
	if (type == TAG_START)
	{
		if (m_italicStart)
		{
			m_text += "</i>";
		}

		m_text += "<i>";
		m_italicStart = TRUE;
	}
	else
	{
		m_text += "</i>";
		m_italicStart = FALSE;
	}
}

void CFcListener::OnSub()
{
	m_text += "<sub>";
	m_subStart = TRUE;
}

void CFcListener::OnSuper()
{
	m_text += "<sup>";
	m_superStart = TRUE;
}

void CFcListener::OnEndSubSuper()
{
	if (m_subStart)
		m_text += "</sub>";
	if (m_superStart)
		m_text += "</sup>";
	m_superStart = FALSE;
	m_subStart = FALSE;
}

void CFcListener::OnSpace(int count)
{
	CString		s;
	for(int j = 0; j < count / tab_size; j++)
	{
		//s += "<sp>";
        s += "&nbsp;";
	}
	m_text += s;
}

void CFcListener::OnText(const char *data, int len)
{
	int		i;
	CString	str;

	if (m_lineStart)
	{
		for(i = 0; i < len; i++)
		{
			if (data[i] != ' ')
				break;
		}
		CString		s;
		for(int j = 0; j < i / tab_size; j++)
		{
			//s += "<sp>";
            s += "&nbsp;";
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

void CFcListener::OnImage(int id, int owidth, int oheight, int width, int height, 
						  LPCTSTR type, CString &path, CString &name)
{
	path = FC_PATH;
	name.Format("%s_%d.%s", m_listenerId, id, type);
	m_images.Add(path + name);
}
