#include "StdAfx.h"
#include "HtmlListener2.h"



void CHtmlListener2::OnImage(int id, int owidth, int oheight, int width, int height,
    LPCTSTR type, CString &path, CString &name)
{
    CString		s;

    path = "export_media\\";
    name.Format("%s_%d.%s", m_listenerId, id, type);
    s.Format("<img src=\"./export_media/%s\" alt=\"&lt;&lt;image&gt;&gt;\" "
        "width=\"%d\" height=\"%d\" />", name, width, height);

    m_text += s;
}
