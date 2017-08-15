#pragma once

#include "RtfParser.h"
#include <map>
#include <string>

#define HTML_PATH		"db\\html_media\\"

using namespace std;

struct FontInfo
{
	int		font;
	int		size;
	string	fontName;
    string  color;
    int     lineHeight;
    int     index;
};

typedef map<string, FontInfo>	FontMap;

class CHtmlListener : public CRtfParser::CListener
{
public:
    CHtmlListener(BOOL attachCss = FALSE) { m_attachCss = attachCss; m_fiIndex = -1;  };
	virtual ~CHtmlListener() {};

	void EnableSizeTag(BOOL on) { m_sizeTag = on; }
	virtual void OnStart();
	virtual void OnEnd();

	virtual void OnLineEnd();
	virtual void OnFontSize(int size);
	virtual void OnFontColor(Rgb &rgb);
	virtual void OnBullet();
	virtual void OnTab();
	virtual void OnSpace(int count);
	virtual void OnFont(int index, CString &font);
	virtual void OnBold(int type);
	virtual void OnUnderline(int type);
	virtual void OnItalic(int type);
    virtual void OnStrike(int type);
	virtual void OnSub();
	virtual void OnSuper();
	virtual void OnEndSubSuper();
	virtual void OnUrl(const char *url, int urlLen, const char *display, int displayLen);
	virtual void OnLineHeight(int height);

	virtual void OnText(const char *data, int len);

	virtual void OnImage(int id, int owidth, int oheight, int width, int height, 
		LPCTSTR type, CString &path, CString &name);

    void HandleFont();
    void CloseTag();
    void OpenTag();
    virtual CString GetCss();
    void ClearCss();

    FontInfo    m_fi;
	FontMap	    m_fontMap;
    BOOL        m_fiChange;
    int         m_fiIndex;

    BOOL        m_boldChange;
    BOOL        m_italicChange;
    BOOL        m_underlineChange;
    BOOL        m_subChange;
    BOOL        m_superChange;
    BOOL        m_strikeChange;
    BOOL        m_attachCss;

};