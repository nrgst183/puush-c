#pragma once

#include <curl/curl.h>

typedef struct {
    LPSTR ptr;
    size_t len;
} String;

typedef int (*UploadProgressCallback)(void* clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);
typedef void(*RequestCompleteHandler)(CURLcode, String*);

void InitString(String* s);
size_t WriteFunc(void* ptr, size_t size, size_t nmemb, String* s);

CURLcode HttpGet(LPCWSTR url, String* response);
CURLcode HttpPost(LPCWSTR url, LPCWSTR data, String* response);
CURLcode HttpMultipartPost(LPCWSTR url, struct curl_httppost* postdata, String* response, UploadProgressCallback progressCallback);
