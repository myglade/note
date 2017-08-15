#pragma once

#include "HtmlListener.h"
#include <map>
#include <string>



class CHtmlListener2 : public CHtmlListener
{
public:
    CHtmlListener2() {};
    virtual ~CHtmlListener2() {};

    virtual void OnImage(int id, int owidth, int oheight, int width, int height,
        LPCTSTR type, CString &path, CString &name);

};