#pragma once

#include "RtfParser.h"

#define FC_PATH		"db\\fc_media\\"

class CFcListener : public CRtfParser::CListener
{
public:
	CFcListener() {};
	virtual ~CFcListener() {};

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
	virtual void OnSub();
	virtual void OnSuper();
	virtual void OnEndSubSuper();
	virtual void OnCenterAlign();

	virtual void OnText(const char *data, int len);

	virtual void OnImage(int id, int owidth, int oheight, int width, int height, 
		LPCTSTR type, CString &path, CString &name);

	static void ClearFolder();
};