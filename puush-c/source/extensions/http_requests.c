#include "http_requests.h"

void InitString(String* s) {
    s->len = 0;
    s->ptr = malloc(s->len + 1);
    if (s->ptr == NULL) {
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}

size_t WriteFunc(void* ptr, size_t size, size_t nmemb, String* s) {
    size_t new_len = s->len + size * nmemb;
    char* new_ptr = realloc(s->ptr, new_len + 1);
    if (new_ptr == NULL) {
        fprintf(stderr, "realloc() failed\n");
        return 0;  // Return 0 to indicate a problem
    }
    s->ptr = new_ptr;
    memcpy(s->ptr + s->len, ptr, size * nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return size * nmemb;  // Return the number of bytes processed
}

CURLcode HttpGet(LPCWSTR url, String* response) {
    CURL* curl = NULL;
    CURLcode res = CURLE_FAILED_INIT;

    // Convert the LPCWSTR URL to multibyte for use with libcurl
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, url, -1, NULL, 0, NULL, NULL);
    char* url_mb = malloc(size_needed);
    WideCharToMultiByte(CP_UTF8, 0, url, -1, url_mb, size_needed, NULL, NULL);

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "curl_easy_init() failed\n");
        goto cleanup;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url_mb);

    InitString(response);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        goto cleanup;
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
    CURL* curl = NULL;
    CURLcode res = CURLE_FAILED_INIT;

    // Convert the LPCWSTR URL to multibyte for use with libcurl
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, url, -1, NULL, 0, NULL, NULL);
    char* url_mb = malloc(size_needed);
    WideCharToMultiByte(CP_UTF8, 0, url, -1, url_mb, size_needed, NULL, NULL);

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "curl_easy_init() failed\n");
        goto cleanup;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url_mb);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

    InitString(response);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        goto cleanup;
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

CURLcode HttpMultipartPost(LPCWSTR url, struct curl_httppost* postdata, String* response, UploadProgressCallback progressCallback) {
    CURL* curl = NULL;
    CURLcode res = CURLE_FAILED_INIT;

    // Convert the LPCWSTR URL to multibyte for use with libcurl
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, url, -1, NULL, 0, NULL, NULL);
    char* url_mb = malloc(size_needed);
    WideCharToMultiByte(CP_UTF8, 0, url, -1, url_mb, size_needed, NULL, NULL);

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "curl_easy_init() failed\n");
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
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        goto cleanup;
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

