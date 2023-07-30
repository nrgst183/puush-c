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

int WriteFunc(void* ptr, size_t size, size_t nmemb, String* s) {
    size_t new_len = s->len + size * nmemb;
    char* new_ptr = realloc(s->ptr, new_len + 1);
    if (new_ptr == NULL) {
        fprintf(stderr, "realloc() failed\n");
        return CURLE_OUT_OF_MEMORY;  // Return error code
    }
    s->ptr = new_ptr;
    memcpy(s->ptr + s->len, ptr, size * nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return CURLE_OK;  // Return success code
}

CURLcode HttpGet(LPCSTR url, String* response) {
    CURL* curl = NULL;
    CURLcode res = CURLE_FAILED_INIT;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "curl_easy_init() failed\n");
        goto cleanup;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);

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

    curl_global_cleanup();

    return res;
}

CURLcode HttpPost(LPCSTR url, LPCSTR data, String* response) {
    CURL* curl = NULL;
    CURLcode res = CURLE_FAILED_INIT;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "curl_easy_init() failed\n");
        goto cleanup;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
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

    curl_global_cleanup();

    return res;
}
