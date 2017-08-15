#pragma once

#include "RtfParser.h"

class CNumberingListener :
    public CRtfParser::CListener
{
public:
    CNumberingListener();
    virtual ~CNumberingListener();

    virtual void OnText(const char *data, int len);

    int m_offset = 0;
};

