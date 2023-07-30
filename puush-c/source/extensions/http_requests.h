#pragma once

#include <curl/curl.h>

typedef struct {
    LPSTR ptr;
    size_t len;
} String;

void InitString(String* s);
size_t WriteFunc(void* ptr, size_t size, size_t nmemb, String* s);

CURLcode HttpGet(LPCSTR url, String* response);
CURLcode HttpPost(LPCSTR url, LPCSTR data, String* response);
