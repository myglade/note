#include "StdAfx.h"
#include "RtfImage.h"
#include <math.h>

using namespace Gdiplus;


int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT  num = 0;          // number of image encoders
   UINT  size = 0;         // size of the image encoder array in bytes

   ImageCodecInfo* pImageCodecInfo = NULL;

   GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  // Failure

   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;  // Failure

   GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j = 0; j < num; ++j)
   {
	   CStringW s = pImageCodecInfo[j].MimeType;
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;  // Success
      }    
   }

   free(pImageCodecInfo);
   return -1;  // Failure
}

int ConvertWmfToImageFile(const char *buf, int len, int width, int height, CString &path)
{
	IStream* pIStream	= NULL;
	if(CreateStreamOnHGlobal(NULL, TRUE, (LPSTREAM*)&pIStream) != S_OK)
	{
		AfxMessageBox(_T("Failed to create stream on global memory!"));
		return -1;
	}

	ULONG ulBytesRead;

	if (pIStream->Write(buf, len, &ulBytesRead) != S_OK)
	{
		AfxMessageBox(_T("Failed to write image to stream!"));
		pIStream->Release();
		return -1;
	}

	LARGE_INTEGER lnOffset;
	lnOffset.QuadPart = 0;
	if(pIStream->Seek(lnOffset, STREAM_SEEK_SET, NULL) != S_OK)
	{
		pIStream->Release();
		AfxMessageBox(_T("Failed to smove the file pointer to the beginning of the stream!"));
		return -1;
	}

	CLSID pngClsid;
	GetEncoderClsid(L"image/jpeg", &pngClsid);

	Image image(pIStream);
	Bitmap bitmap(width, height, image.GetPixelFormat());
	Graphics *	g = Graphics::FromImage(&bitmap); 
	g->SetCompositingQuality(CompositingQualityHighQuality);
	g->SetSmoothingMode(SmoothingModeHighQuality);
	g->SetInterpolationMode(InterpolationModeHighQualityBicubic);

	RectF r(0, 0, (Gdiplus::REAL) width + 0, (Gdiplus::REAL) height + 0);

	g->DrawImage(&image, r);

	CStringW	pathW;
	
	pathW = path;
	bitmap.Save(pathW, &pngClsid);	
	pIStream->Release();
	
	delete g;
	return 0;
}

int ConvertWmfToImageStream(const char *buf, int len, RtfImage &info)
{
	IStream*			pIStream	= NULL;
	IStream*			pIStream2	= NULL;
	ULONG				ulBytesRead;
	LARGE_INTEGER		lnOffset;
	CLSID				pngClsid;
	ULARGE_INTEGER		ulnSize;
	Status				SaveStatus;

	if(CreateStreamOnHGlobal(NULL, TRUE, (LPSTREAM*)&pIStream) != S_OK)
	{
		return -1;
	}
	if(CreateStreamOnHGlobal(NULL, TRUE, (LPSTREAM*)&pIStream2) != S_OK)
	{
		pIStream->Release();
		return -1;
	}

	if (pIStream->Write(buf, len, &ulBytesRead) != S_OK)
	{
		pIStream->Release();
		pIStream2->Release();
		return -1;
	}

	lnOffset.QuadPart = 0;
	if(pIStream->Seek(lnOffset, STREAM_SEEK_SET, NULL) != S_OK)
	{
		pIStream->Release();
		pIStream2->Release();
		return -1;
	}
	if (len > 100 * 1024)
	{
		GetEncoderClsid(L"image/jpeg", &pngClsid);
		info.type = "jpg";
	}
	else
	{
		GetEncoderClsid(L"image/png", &pngClsid);
		info.type = "png";
	}

	Image image(pIStream);
	//PixelFormat a = image.GetPixelFormat();
	Bitmap bitmap(info.width, info.height, PixelFormat24bppRGB);
	Graphics *	g = Graphics::FromImage(&bitmap); 
	g->SetCompositingQuality(CompositingQualityHighQuality);
	g->SetSmoothingMode(SmoothingModeHighQuality);
	g->SetInterpolationMode(InterpolationModeHighQualityBicubic);

	RectF r(0, 0, (Gdiplus::REAL) info.width + 1, (Gdiplus::REAL) info.height + 1);

	g->DrawImage(&image, r);

	SaveStatus = bitmap.Save(pIStream2, &pngClsid);

	lnOffset.QuadPart = 0;
	if(pIStream2->Seek(lnOffset, STREAM_SEEK_END, &ulnSize) != S_OK)
	{
		pIStream->Release();
		pIStream2->Release();
		delete g;
		return -1;
	}

	if(pIStream2->Seek(lnOffset, STREAM_SEEK_SET, NULL) != S_OK)
	{
		pIStream->Release();
		pIStream2->Release();
		delete g;
		return -1;
	}
    info.data = std::shared_ptr<BYTE>(new BYTE[(int) ulnSize.QuadPart]);
    // Read the stream directly into the buffer
	if (pIStream2->Read(info.data.get(), (ULONG)ulnSize.QuadPart, &ulBytesRead) != S_OK)
    {
		pIStream->Release();
		delete g;
		return -1;
    }
	info.len = (int) ulBytesRead;

	pIStream->Release();
	pIStream2->Release();
	
	delete g;
	return 0;
}

int ConvertImageToWmf(LPCTSTR type, RtfImage &info)
{
	IStream*			pIStream = NULL;
	IStream*			pDstIStream = NULL;
	IStream*			bitMapStream = NULL;
	ULONG				ulBytesRead;
	CLSID				pngClsid;
	LARGE_INTEGER		lnOffset;
	ULARGE_INTEGER		ulnSize;
	Status				SaveStatus;
	BOOL				wmf = FALSE;
	Image *				image = NULL;
//	Unit				u;
	Graphics *			g;
    RectF	            r;

    if (info.data == NULL || info.len <= 0)
        return -1;

	if(CreateStreamOnHGlobal(NULL, TRUE, (LPSTREAM*)&pIStream) != S_OK)
		goto kFail;
	if(CreateStreamOnHGlobal(NULL, TRUE, (LPSTREAM*)&pDstIStream) != S_OK)
		goto kFail;

	if (pIStream->Write(info.data.get(), info.len, &ulBytesRead) != S_OK)
		goto kFail;

	lnOffset.QuadPart = 0;
	if(pIStream->Seek(lnOffset, STREAM_SEEK_SET, NULL) != S_OK)
		goto kFail;

	if (type == "jpg")
		GetEncoderClsid(L"image/jpeg", &pngClsid);
	else if (type == "gif")
		GetEncoderClsid(L"image/gif", &pngClsid);
	else if (type == "wmf")
	{
		GetEncoderClsid(L"image/bmp", &pngClsid);
		wmf = TRUE;
	}

	image = new Image(pIStream);

//	image->GetBounds(&r, &u);
//	info.width = r.Width;
//	info.height = r.Height;
    r.X = 0;
    r.Y = 0;
    r.Width = (Gdiplus::REAL)info.width + (int)((info.width - 1) / info.owidth);
    r.Height = (Gdiplus::REAL)info.height + (int)((info.height - 1) / info.oheight); 
//    TRACE("%f %f   %f %f\n", info.width, info.height, r.Width, r.Height);

	if (wmf == FALSE)
	{
		EncoderParameters encoderParameters;
		encoderParameters.Count = 1;
		encoderParameters.Parameter[0].Guid = EncoderQuality;
		encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
		encoderParameters.Parameter[0].NumberOfValues = 1;

		// setup compression level

		ULONG quality = 100;
		encoderParameters.Parameter[0].Value = &quality;
		SaveStatus = image->Save(pDstIStream, &pngClsid, &encoderParameters);
	}
	else	// process wmf
	{
		if(CreateStreamOnHGlobal(NULL, TRUE, (LPSTREAM*)&bitMapStream) != S_OK)
			goto kFail;

		Bitmap bitmap(info.width, info.height, image->GetPixelFormat());
		g = Graphics::FromImage(&bitmap); 
		g->SetCompositingQuality(CompositingQualityHighQuality);
		g->SetSmoothingMode(SmoothingModeHighQuality);
		g->SetInterpolationMode(InterpolationModeHighQuality);
		g->DrawImage(image, r);
		bitmap.Save(bitMapStream, &pngClsid);
		delete g;

		Image image2(bitMapStream);

		Metafile *mt = new Metafile(pDstIStream, GetDC(0));
		g = new Graphics(mt);
		SaveStatus = g->DrawImage(&image2, r);

		delete mt;
		delete g;

		lnOffset.QuadPart = 0;
		if(pDstIStream->Seek(lnOffset, STREAM_SEEK_SET, NULL) != S_OK)
			goto kFail;

		mt = new Metafile(pDstIStream);
		HENHMETAFILE hEmf = mt->GetHENHMETAFILE();

		if (hEmf == NULL)
		{
			delete mt;
			goto kFail;
		}
		// Determine the size of the buffer that will receive the converted records.
		info.len = Metafile::EmfToWmfBits(
										hEmf, 
										0, 
										NULL,
										MM_ANISOTROPIC,
										0);
		if (info.len == 0)
		{
			DeleteEnhMetaFile(hEmf);
			delete mt;
			goto kFail;
		}

		// Allocate a buffer to receive the converted records.
   //     delete info.data;
		BYTE *p = new BYTE[info.len];
		     
		// Convert the EMF records to WMF records.
		INT convertedSize = Metafile::EmfToWmfBits(
										hEmf, 
										info.len,
										p, 
										MM_ANISOTROPIC,
										0);
		info.data = std::shared_ptr<BYTE>(p);
		if (pIStream)
			pIStream->Release();
		if (pDstIStream)
			pDstIStream->Release();
		if (bitMapStream)
			bitMapStream->Release();
		DeleteEnhMetaFile(hEmf);

		delete mt;
		return 0;		
	}

	if(SaveStatus != Ok)
		goto kFail;

	lnOffset.QuadPart = 0;
	if(pDstIStream->Seek(lnOffset, STREAM_SEEK_END, &ulnSize) != S_OK)
		goto kFail;

	// now move the pointer to the beginning of the file
	if(pDstIStream->Seek(lnOffset, STREAM_SEEK_SET, NULL) != S_OK)
		goto kFail;

   // delete info.data;
    info.data = std::shared_ptr<BYTE>(new BYTE[(int) ulnSize.QuadPart]);
    // Read the stream directly into the buffer
	if (pDstIStream->Read(info.data.get(), (ULONG)ulnSize.QuadPart, &ulBytesRead) != S_OK)
		goto kFail;
	info.len = (int) ulBytesRead;

	if (pIStream)
		pIStream->Release();
	if (pDstIStream)
		pDstIStream->Release();
	if (bitMapStream)
		bitMapStream->Release();
	if (image)
		delete image;

	return 0;

kFail:
	if (pIStream)
		pIStream->Release();
	if (pDstIStream)
		pDstIStream->Release();
	if (bitMapStream)
		bitMapStream->Release();
	if (image)
		delete image;
	AfxMessageBox(_T("Failed to move the file pointer to "
		"the beginning of the stream!"));
	return -1;
}

void ConvertTest()
{
#if 0
	CFile		file;

	if (file.Open("e:\\a.gif", CFile::modeRead) == FALSE)
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
	file.Open("e:\\result1.wmf", CFile::modeCreate | CFile::modeWrite);

	file.Write(dst, dstLen);
	file.Close();
	delete dst;
	delete buf;
#endif
}

#if 0

RtfImage::RtfImage()
{
	m_pIStream = NULL;
	m_image = NULL;
	m_width = m_height = 0;
}

RtfImage::~RtfImage()
{
	if (m_pIStream)
		m_pIStream->Release();

	if (m_image)
		delete m_image;
}

int RtfImage::Load(const char *buf, int len)
{
	ULONG ulBytesRead;
	LARGE_INTEGER lnOffset;

	if (buf == NULL)
		return -1;

	if (m_pIStream)
	{
		m_pIStream->Release();
		m_pIStream = NULL;
	}

	if (m_image)
	{
		delete m_image;
		m_image = NULL;
	}

	if(CreateStreamOnHGlobal(NULL, TRUE, (LPSTREAM*)&m_pIStream) != S_OK)
	{
		return -1;
	}

	if (m_pIStream->Write(buf, len, &ulBytesRead) != S_OK)
	{
		return -1;
	}

	lnOffset.QuadPart = 0;
	if(m_pIStream->Seek(lnOffset, STREAM_SEEK_SET, NULL) != S_OK)
	{
		return -1;
	}

	m_image = new Image(m_pIStream);
	if (m_image == NULL)
		return -1;

	m_width = m_image->GetWidth();
	m_height = m_image->GetHeight();

	return 0;
}

int RtfImage::Save(LPCTSTR path)
{
	Status				SaveStatus;
	CLSID				pngClsid;

	if (m_image == NULL || path == NULL)
		return -1;

	GetEncoderClsid(L"image/jpeg", &pngClsid);

	EncoderParameters encoderParameters;
	encoderParameters.Count = 1;
	encoderParameters.Parameter[0].Guid = EncoderQuality;
	encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
	encoderParameters.Parameter[0].NumberOfValues = 1;

	ULONG quality = 100;
	encoderParameters.Parameter[0].Value = &quality;
	SaveStatus = m_image->Save(CStringW(path), &pngClsid, &encoderParameters);

	return 0;
}

int RtfImage::SaveToStream(BYTE *&dst, int &dstLen)
{
	Status				SaveStatus;
	CLSID				pngClsid;
	IStream*			pIStream = NULL;
	ULONG				ulBytesRead;
	LARGE_INTEGER		lnOffset;
	ULARGE_INTEGER		ulnSize;

	dst = NULL;
	dstLen = 0;

	if (m_image == NULL)
		return -1;

	if(CreateStreamOnHGlobal(NULL, TRUE, (LPSTREAM*)&pIStream) != S_OK)
	{
		return -1;
	}

	GetEncoderClsid(L"image/jpeg", &pngClsid);

	EncoderParameters encoderParameters;
	encoderParameters.Count = 1;
	encoderParameters.Parameter[0].Guid = EncoderQuality;
	encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
	encoderParameters.Parameter[0].NumberOfValues = 1;

	ULONG quality = 100;
	encoderParameters.Parameter[0].Value = &quality;
	SaveStatus = m_image->Save(pIStream, &pngClsid, &encoderParameters);

	lnOffset.QuadPart = 0;
	if(pIStream->Seek(lnOffset, STREAM_SEEK_END, &ulnSize) != S_OK)
	{
		pIStream->Release();
		return -1;
	}

	// now move the pointer to the beginning of the file
	if(pIStream->Seek(lnOffset, STREAM_SEEK_SET, NULL) != S_OK)
	{
		pIStream->Release();
		return -1;
	}

    dst = new BYTE[(int) ulnSize.QuadPart];
    // Read the stream directly into the buffer
    if(pIStream->Read(dst, (ULONG) ulnSize.QuadPart, &ulBytesRead) != S_OK)
    {
		pIStream->Release();
		return -1;
    }
	dstLen = (int) ulBytesRead;
	pIStream->Release();

	return 0;
} 

int ProcessImage(const char *buf, int len, int width, int height, CString &path)
{
	IStream* pIStream	= NULL;
	if(CreateStreamOnHGlobal(NULL, TRUE, (LPSTREAM*)&pIStream) != S_OK)
	{
		AfxMessageBox(_T("Failed to create stream on global memory!"));
		return -1;
	}

	ULONG ulBytesRead;

	if (pIStream->Write(buf, len, &ulBytesRead) != S_OK)
	{
		AfxMessageBox(_T("Failed to write image to stream!"));
		pIStream->Release();
		return -1;
	}

	LARGE_INTEGER lnOffset;
	lnOffset.QuadPart = 0;
	if(pIStream->Seek(lnOffset, STREAM_SEEK_SET, NULL) != S_OK)
	{
		pIStream->Release();
		AfxMessageBox(_T("Failed to smove the file pointer to the beginning of the stream!"));
		return -1;
	}

	CLSID pngClsid;
	GetEncoderClsid(L"image/jpeg", &pngClsid);

	Image image(pIStream);
	Bitmap bitmap(width, height, image.GetPixelFormat());
	Graphics *	g = Graphics::FromImage(&bitmap); 
	g->SetCompositingQuality(CompositingQualityHighQuality);
	g->SetSmoothingMode(SmoothingModeHighQuality);
	g->SetInterpolationMode(InterpolationModeHighQualityBicubic);

	RectF r( 
		0, 
		0, 
		(Gdiplus::REAL) width + 1, 
		(Gdiplus::REAL) height + 1);

	g->DrawImage(&image, r);

	CStringW	pathW;
	
	pathW = path;
	bitmap.Save(pathW, &pngClsid);	
	pIStream->Release();
	
	delete g;
	return 0;
}
#endif
