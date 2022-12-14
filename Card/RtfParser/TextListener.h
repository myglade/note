#pragma once

#include "RtfParser.h"

class CTextListener :
	public CRtfParser::CListener
{
public:
	CTextListener(int maxSize, CString lineFeed, bool stressHead);
	virtual ~CTextListener(void) {}

	virtual void OnStart();
	virtual void OnEnd();

	virtual void OnBullet();
	virtual void OnTab();
	virtual void OnSpace(int count);
    virtual void OnLineEnd();
    virtual void OnUrl(const char *url, int urlLen, const char *display, int displayLen);

	virtual void OnText(const char *data, int len);

	int			m_maxSize;
    int         m_lineEnd;
    int         m_headStaus;
    bool        m_stressHead;
    CString     m_lineFeed;
    bool        m_captureFirstLine;
	bool		m_focusStatus;  // user emphasize
};

