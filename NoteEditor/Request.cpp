#include "pch.h"

#pragma comment(lib, "wldap32.lib" )
#pragma comment(lib, "crypt32.lib" )
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "libcurl_a.lib")

#define CURL_STATICLIB
#include <curl/curl.h>
#include <string>

#include "util.h"

using namespace std;

void send_request(const string& data) {
    CURL* curl;
    CURLcode res;
    struct curl_slist* slist1;
    /* get a curl handle */
    curl = curl_easy_init();
    if (curl == nullptr) {
        MsgBox("Fail to init curl");
        return;
    }

    slist1 = NULL;
    slist1 = curl_slist_append(slist1, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, SERVER_URL.c_str());
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist1);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if (res != CURLE_OK) {
        string msg = "curl_easy_perform() failed: " + string(curl_easy_strerror(res));
        MsgBox(msg);
    };

    /* always cleanup */
    curl_easy_cleanup(curl);
    curl_slist_free_all(slist1);
}

