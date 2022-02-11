// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"

#include <wil/com.h>
#include <wrl.h>
#include "WebView2EnvironmentOptions.h"
#include "WebView2.h"
#include <WinSock2.h>


#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes

//#define _AFXWIN_INLINE

#endif //PCH_H
