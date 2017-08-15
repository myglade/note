#include "StdAfx.h"
#include "TextListener.h"

#define STRESS_START "<span class=\"stress\">"
#define STRESS_END   "</span>"

CTextListener::CTextListener(int maxSize, CString lineFeed, bool stressHead)
{
    if (maxSize < 0)
    {
        m_maxSize = -maxSize;
        m_captureFirstLine = true;
    }
    else 
    {
        m_maxSize = maxSize;
        m_captureFirstLine = false;
    }
    m_stressHead = stressHead;
    m_lineFeed = lineFeed;
}

void CTextListener::OnStart()
{
    m_lineEnd = 0;
    m_headStaus = 0;
}


void CTextListener::OnEnd()
{
    int len = m_text.GetLength();

    if (m_stressHead)
    {
        if (m_headStaus == 2)
        {
            len -= (strlen(STRESS_START) + strlen(STRESS_END));
            if (len <= 0) 
            {
                m_text = STRESS_START;
                m_text += "-";
                m_text += STRESS_END;
            }
        }
        else if (m_headStaus == 1)
        {
             len -= strlen(STRESS_START);   
             if (len <= 0)
                 m_text += "-";
        }
        else 
        {
            m_text = STRESS_START;
            m_text += "-";
            m_text += STRESS_END;
            m_headStaus = 2;
        }
    }
    else if (len <= 0)
    {
        m_text = "-";
    }

    if (m_lineEnd == 1)
    {
        if (m_captureFirstLine == false)
            m_text += m_lineFeed + " ";
    }

    if (m_stressHead && m_headStaus == 1)
    {
        m_text += STRESS_END;        
        m_headStaus = 2;
    }
}

void CTextListener::OnBullet()
{
    m_lineEnd = 0;
	m_text += "*";
	if (m_text.GetLength() > m_maxSize)
		this->Stop();
}

void CTextListener::OnTab()
{
    m_lineEnd = 0;
	m_text += "  ";
	if (m_text.GetLength() > m_maxSize)
		this->Stop();
}

void CTextListener::OnLineEnd()
{
    if (m_captureFirstLine) 
    {
        this->Stop();
    }
    else 
    {
        if (m_lineEnd == 1)
        {
            m_text += m_lineFeed;
        }
        else if (m_lineEnd == 0)
        {
            m_text += " " + m_lineFeed;
        }
        else
            return;
    }

    if (m_stressHead && m_headStaus == 1)
    {
        m_text += STRESS_END;        
        m_headStaus = 2;
    }
    m_lineEnd++;
}

void CTextListener::OnSpace(int count)
{
    if (m_lineEnd > 0)
        return;

	CString		s;
	for(int j = 0; j < count; j++)
	{
		s += " ";
	}
	m_text += s;
	if (m_text.GetLength() > m_maxSize)
		this->Stop();
}

void CTextListener::OnUrl(const char *url, int urlLen, const char *display, int displayLen)
{
    OnText(display, displayLen);
}


void CTextListener::OnText(const char *data, int len)
{
	if (m_text.GetLength() >= m_maxSize)
	{
		this->Stop();
		return;
	}

    if (m_stressHead && m_headStaus == 0)
    {
        m_text += STRESS_START;        
        m_headStaus = 1;
    }

    bool blank = false;
    int     i;

    for(i = 0; i < len; i++)
    {
        if (data[i] != ' ')
        {
            blank = false;
            break;
        }
    }

    if (i == len)
        return;

    if (i > 1)
    {
        data += (i - 1);
        len -= (i - 1);
    }
    if (len <= 0)
        return;

    int     count = 0;
    for(i = len - 1; i >= 0; i--)
    {
        if (data[i] != ' ')
        {
            break;
        }
        count++;
    }

    if (count > 1)
    {
        len -= (count - 1);
        if (len <= 0)
            return;

        char *p = (char *) data;
        p[len] = 0;
    }



    if (blank == false)
    {
        if (m_lineEnd == 1)
        {
            m_text += m_lineFeed + " ";
        }
        else if (m_lineEnd > 1)
            m_text += " ";

        m_lineEnd = 0;
    }
    
    if (len + m_text.GetLength() > m_maxSize)
	{
        // due to linefeed processiong, m_text may be prolonged
        if (m_text.GetLength() < m_maxSize)
        {
		    len = m_maxSize - m_text.GetLength();
		    char *p = new char[len + 1];
		    strncpy(p, data, len);
		    p[len] = 0;
		    m_text += p;
 		    delete p;
       }

		m_text += "...";
		this->Stop();
		return;
	}

	m_text += data;

}
