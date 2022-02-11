- webview install
https://docs.microsoft.com/en-us/microsoft-edge/webview2/get-started/win32

In the NuGet window, click the Browse tab.

In the search bar, type Microsoft.Windows.ImplementationLibrary > choose Microsoft.Windows.ImplementationLibrary.
In the search bar, type Microsoft.Web.WebView2 and choose Microsoft.Web.WebView2.

- Webview 2 : code project
https://www.codeproject.com/Tips/5287858/WebView2-Edge-Browser-in-MFC-Cplusplus-Application


- curl install
https://stackoverflow.com/questions/53861300/how-do-you-properly-install-libcurl-for-use-in-visual-studio-2017

1. Download curl zip package from https://curl.haxx.se/download.html
2. go to winbuild\ in extracted folder
3. open Developer Command Prompt for VS in visual studio
4. nmake /f Makefile.vc mode=static
5. In Project Properties -> VC++ Directories -> Include Directories add curl-7.80.0\builds\libcurl-vc-x86-release-static-ipv6-sspi-schannel\include
6. In Project Properties -> VC++ Directories -> Library Directories add ..\curl-7.80.0\builds\libcurl-vc-x86-release-static-ipv6-sspi-schannel\lib
7. Properties -> Linker -> Input -> Additional Dependencies add libcurl_a.lib

libcurl_a.lib;Ws2_32.lib;Crypt32.lib;Wldap32.lib;Normaliz.lib

 
Install is okay. but, link error.
see https://www.youtube.com/watch?v=q_mXVZ6VJs4

1. open x64 Native Tools Command Prompt for VS 2022 in vc window menu
2. cd C:\Source\note\curl-7.80.0\winbuild
3. set RTLIBCFG=static
4. nmake /f Makefile.vc mode=static vc=17 debug=no

17 is shell version 
**********************************************************************
** Visual Studio 2022 Developer Command Prompt v17.0.2
** Copyright (c) 2021 Microsoft Corporation
**********************************************************************






- json : nlohmann/json
https://github.com/nlohmann/json














