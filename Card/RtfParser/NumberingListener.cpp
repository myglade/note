#include "stdafx.h"
#include "NumberingListener.h"


CNumberingListener::CNumberingListener()
{
}


CNumberingListener::~CNumberingListener()
{
}

void CNumberingListener::OnText(const char *data, int len)
{
    m_offset = GetCurOffset();
    this->Stop();
    return;
}