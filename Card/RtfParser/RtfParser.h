#pragma once

#include <math.h>
#include "RtfImage.h"

#define SPACE_LENGTH		(540/4)
#define TAB_SPACE			4
#define TAG_START			0
#define TAG_END				1
#define TAG_LEFT			0
#define TAG_RIGHT			1

#define TYPE_TAG			0
#define TYPE_TEXT			1
#define TYPE_LINE_FEED		2
#define TYPE_BUF_END		3
#define TYPE_BRACKET_END	4

#define IMAGE_TEXT	"<<image>>"

#define RTF_BULLET			183
#define BULLET_TEMP			0x1F
#define BULLET				0x2022
#define ALT_FONT_LIST		"Courier New,monospace"


/*
		// The number of hundredths of millimeters (0.01 mm) in an inch
		// For more information, see GetImagePrefix() method.
		private const int HMM_PER_INCH = 2540;

		// The number of twips in an inch
		// For more information, see GetImagePrefix() method.
		private const int TWIPS_PER_INCH = 1440;


		/// Units Used
		/// ----------
		/// 1 Twip = 1/20 Point
		/// 1 Point = 1/72 Inch
		/// 1 Twip = 1/1440 Inch
		/// 
		/// 1 Inch = 2.54 cm
		/// 1 Inch = 25.4 mm
		/// 1 Inch = 2540 (0.01)mm

		// Calculate the current width of the image in (0.01)mm
			int picw = (int)Math.Round((_image.Width / xDpi) * HMM_PER_INCH);

			// Calculate the current height of the image in (0.01)mm
			int pich = (int)Math.Round((_image.Height / yDpi) * HMM_PER_INCH);

			// Calculate the target width of the image in twips
			int picwgoal = (int)Math.Round((_image.Width / xDpi) * TWIPS_PER_INCH);

			// Calculate the target height of the image in twips
			int pichgoal = (int)Math.Round((_image.Height / yDpi) * TWIPS_PER_INCH);


*/
inline double round(double r)
{
	return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}

#define PIXEL_TO_PIC(a)			(int)round(((double)a / 96.0 * 2540.0))
#define PIXEL_TO_PICGOAL(a)		(int)round(((double)a / 96.0 * 1440.0))
#define PIC_TO_PIXEL(a)			(int)round(((double)a * 96.0 / 2540.0))
#define PICGOAL_TO_PIXEL(a)		(int)round(((double)a * 96.0 / 1440.0))

struct Rgb
{
	CString		r;
	CString		g;
	CString		b;
};

class CRtfParser
{
public:
	class CListener
	{
	public:
		CListener() : m_sizeTag(TRUE), m_listenerId("") {};
		virtual ~CListener() {};

		virtual void OnStart() {}
		virtual void OnEnd() {}

		virtual void OnLineEnd() {}
		virtual void OnFontSize(int size) {}
		virtual void OnFontColor(Rgb &rgb) {}
		virtual void OnBullet() {}
		virtual void OnSpace(int count) {}
		virtual void OnTab() {}
		virtual void OnFont(int index, CString &font) {}
		virtual void OnBold(int type) {}
		virtual void OnUnderline(int type) {}
		virtual void OnStrike(int type) {}
		virtual void OnItalic(int type) {}
		virtual void OnImage(int id, int owidth, int oheight, int width, int height, LPCTSTR type, 
			CString &path, CString &name) {}
		virtual void OnText(const char *data, int len) {}
		virtual void OnList(CString &data) {}
		virtual void OnSub() {}
		virtual void OnSuper() {}
		virtual void OnEndSubSuper() {}
		virtual void OnCenterAlign() {}

		// default 100%. But use 130% as default.
		virtual void OnLineHeight(int height) {}
        virtual void OnUrl(const char *url, int urlLen, const char *display, int displayLen) {};

		virtual void Reset(LPCTSTR id) 
		{
			m_text = "";
			m_boldStart = FALSE;
			m_underlineStart = FALSE;
			m_italicStart = FALSE;
            m_strikeStart = FALSE;
			m_colorStart = FALSE;
			m_sizeStart = FALSE;
			m_lineStart = TRUE;
			m_fontStart = FALSE;
			m_stop = FALSE;
			m_subStart = FALSE;
			m_superStart = FALSE;
			m_centerAlign = FALSE;
			m_lineHeightStart = FALSE;
			m_images.RemoveAll();
			m_listenerId = id;
		}

		CString & GetText() { return m_text; }
		int GetUtfText(CStringA &text);
		static int GetUtfText(CString &s, CStringA &utf8);
		void Stop() { m_stop = TRUE; }
        int GetCurOffset() { return m_parser->GetCurOffset();  }

		CString		m_text;
		BOOL		m_boldStart;
		BOOL		m_underlineStart;
		BOOL		m_italicStart;
		BOOL		m_strikeStart;
		BOOL		m_colorStart;
		BOOL		m_sizeStart;
		BOOL		m_lineStart;
		BOOL		m_fontStart;
		BOOL		m_sizeTag;
		BOOL		m_subStart;
		BOOL		m_superStart;
		BOOL		m_centerAlign;
		BOOL		m_lineHeightStart;
		CString		m_imagePath;
		CStringArray m_images;
		BOOL		m_stop;
		CString		m_listenerId;

        CRtfParser *m_parser;
	};

	CRtfParser(void);
	~CRtfParser(void);
	
    CStringArray &GetFontTable() { return m_fontTable;}

	int Open(CString file);
	int OpenFromString(const char *p, RtfImageList *imageList);
	int Parse(CRtfParser::CListener *listener, LPCTSTR listenerId);
	int GetFontTable(char *&p);
	int	GetColorTable(char *&p);
	void OnLineFeed();
	void OnTag(const char *p, int len, char *&buffer);
	void OnText(const char *p, int len);
	
	int NextElement(char *&buf, char *element, int &len);
	void SkipBracket(char *&p);

	void ProcessList(char *&p);
	int ParseImage(const char *p);
    int GetCurOffset() { return (int)(m_curPos - m_buf); }

	static int DetachImage(LPCTSTR src, RtfImageList &srcMap, RtfImageList &list, CString &dst);
	static int AttachImage(LPCTSTR src, RtfImageList &list, CString &dst);
    static int ConvertImage(LPCTSTR src, CString &dst);

    int ParseUrl(char *&p);

	BOOL				m_lineHeightCalled;
	int					m_startSpace;
	int					m_newLine;
	char *				m_buf;
	char *				m_dst;
    char *              m_curPos;
	int					m_bufLen;
	int					m_borrowBuffer;
	int					m_imageId;
	int					m_bracket;
	int					m_curFont;
	BOOL				m_prevTag;
	CString				m_prefix;
	RtfImageList *	    m_imageList;

	CListener *	m_listener;
	CStringArray		m_fontTable;
	CArray<Rgb, Rgb>	m_colorTable;
};

inline bool IsTag(char *p1)
{
	if (*p1 == '\\' && 
		(*(p1 + 1) != ' ' && 
		*(p1 + 1) != '\\' &&
		*(p1 + 1) != '{' && 
		*(p1 + 1) != '}' &&
		*(p1 + 1) != '\''))
		return true;
	
	return false;
}

inline int GetNumber(char *p)
{
	int		v = 0;
	for(int i = 0; i < 2; i++)
	{
		if (p[i] >= '0' && p[i] <= '9')
			v = v << 4 | (p[i] - '0');
		else
			v = v << 4 | ((tolower(p[i]) - 'a') + 10);
	}

	return v;
}


void ImageStringConvertTest();