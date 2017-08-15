#pragma once

#include "gdiplus.h"
#include <string>
#include <vector>
#include <map>
#include <memory>

using namespace Gdiplus;


struct RtfImage
{
	RtfImage() : data(NULL) {};

	unsigned int	id;
	std::shared_ptr<BYTE>	data;
	int				len;
	int 			width;      // view width
	int 			height;     // view height
	int 			owidth;     // original width
	int 			oheight;    // original height
	CString			type;
    CString         http_path;

    void Reset()
    {
        id = 0; data = NULL; len = 0; width = height = owidth = oheight = 0;
        type = "";
        http_path = "";
    }
};

//typedef std::vector<RtfImage>	RtfImageList;
typedef std::map<unsigned int, RtfImage>	    RtfImageList;

struct DbRtfImages
{
    RtfImageList    add;
    RtfImageList    remove;
    RtfImageList    update;
    RtfImageList    nochange;

    int size() { return add.size() + update.size() + nochange.size(); }
};

int ConvertWmfToImageFile(const char *buf, int len, int width, int height, CString &path);
int ConvertWmfToImageStream(const char *buf, int len, RtfImage &info);
int ConvertImageToWmf(LPCTSTR type, RtfImage &info);
void ConvertTest();

inline void ResetImageList(RtfImageList &imageList)
{
	imageList.clear();
}

#if 0
int ProcessImage(const char *buf, int len, int width, int height, CString &path);

class RtfImage
{
public:
	RtfImage();
	virtual ~RtfImage();

	int Load(const char *buf, int len);
	int Save(LPCTSTR path);
	int SaveToStream(BYTE *&dst, int &dstLen);

	static int ConvertImage(const unsigned char *src, int srcLen, LPCTSTR type, RtfImageInfo &info);
	
	IStream*		m_pIStream;
	Image *			m_image;
	int				m_width;
	int				m_height;
};
#endif

