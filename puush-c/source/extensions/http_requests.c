#include "http_requests.h"

void InitString(String* s) {
    assert(s != NULL);

    s->len = 0;
    s->ptr = malloc(s->len + 1);
    assert(s->ptr && "malloc() failed");
    s->ptr[0] = '\0';
}

size_t WriteFunc(void* ptr, size_t size, size_t nmemb, String* s) {
    size_t new_len = s->len + size * nmemb;
    char* new_ptr = realloc(s->ptr, new_len + 1);

    assert(new_ptr && "realloc() failed");
    s->ptr = new_ptr;
    memcpy(s->ptr + s->len, ptr, size * nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return size * nmemb;  // Return the number of bytes processed
}

CURLcode HttpGet(LPCWSTR url, String* response) {
    assert(url != NULL);
    assert(response != NULL);

    CURL* curl = NULL;
    CURLcode res = CURLE_FAILED_INIT;
    char* url_mb = NULL;

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, url, -1, NULL, 0, NULL, NULL);
    url_mb = malloc(size_needed);
    assert(url_mb && "malloc() failed for url_mb");

    WideCharToMultiByte(CP_UTF8, 0, url, -1, url_mb, size_needed, NULL, NULL);

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    assert(curl && "curl_easy_init() failed");

    curl_easy_setopt(curl, CURLOPT_URL, url_mb);

    InitString(response);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fwprintf(stderr, L"curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

cleanup:
    if (curl) {
        curl_easy_cleanup(curl);
    }
    if (url_mb) {
        free(url_mb);
    }

    curl_global_cleanup();

    return res;
}

CURLcode HttpPost(LPCWSTR url, LPCWSTR data, String* response) {
    assert(url != NULL);
    assert(data != NULL);
    assert(response != NULL);

    CURL* curl = NULL;
    CURLcode res = CURLE_FAILED_INIT;
    char* url_mb = NULL;
    char* data_mb = NULL;

    int size_needed_url = WideCharToMultiByte(CP_UTF8, 0, url, -1, NULL, 0, NULL, NULL);
    url_mb = malloc(size_needed_url);
    assert(url_mb && "malloc() failed for url_mb");
    WideCharToMultiByte(CP_UTF8, 0, url, -1, url_mb, size_needed_url, NULL, NULL);

    int size_needed_data = WideCharToMultiByte(CP_UTF8, 0, data, -1, NULL, 0, NULL, NULL);
    data_mb = malloc(size_needed_data);
    assert(data_mb && "malloc() failed for data_mb");
    WideCharToMultiByte(CP_UTF8, 0, data, -1, data_mb, size_needed_data, NULL, NULL);

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    assert(curl && "curl_easy_init() failed");

    curl_easy_setopt(curl, CURLOPT_URL, url_mb);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data_mb);

    InitString(response);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fwprintf(stderr, L"curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

cleanup:
    if (curl) {
        curl_easy_cleanup(curl);
    }
    if (url_mb) {
        free(url_mb);
    }
    if (data_mb) {
        free(data_mb);
    }

    curl_global_cleanup();

    return res;
}

CURLcode HttpMultipartPost(LPCWSTR url, struct curl_httppost* postdata, String* response, UploadProgressCallback progressCallback) {
    assert(url != NULL);
    assert(postdata != NULL);
    assert(response != NULL);

    CURL* curl = NULL;
    CURLcode res = CURLE_FAILED_INIT;
    char* url_mb = NULL;

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, url, -1, NULL, 0, NULL, NULL);
    url_mb = malloc(size_needed);
    if (!url_mb) {
        fwprintf(stderr, L"malloc() failed for url_mb\n");
        goto cleanup;
    }
    WideCharToMultiByte(CP_UTF8, 0, url, -1, url_mb, size_needed, NULL, NULL);

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if (!curl) {
        fwprintf(stderr, L"curl_easy_init() failed\n");
        goto cleanup;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url_mb);
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, postdata);

    InitString(response);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

    if (progressCallback) {
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progressCallback);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
    }

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fwprintf(stderr, L"curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

cleanup:
    if (curl) {
        curl_easy_cleanup(curl);
    }
    if (url_mb) {
        free(url_mb);
    }

    curl_global_cleanup();

    return res;
}
