#include "puush_http_api.h"

CURLcode Upload(PuushApi* api, LPCSTR filePath, String* response, UploadProgressCallback progressCallback) {
    // Build the URL for the API endpoint
    char url[256];
    snprintf(url, sizeof(url), "%s/api/up", api->baseUrl);

    // Build the form data
    struct curl_httppost* post = NULL;
    struct curl_httppost* last = NULL;
    curl_formadd(&post, &last, CURLFORM_COPYNAME, "k", CURLFORM_COPYCONTENTS, api->apiKey, CURLFORM_END);
    curl_formadd(&post, &last, CURLFORM_COPYNAME, "z", CURLFORM_COPYCONTENTS, "poop", CURLFORM_END);
    curl_formadd(&post, &last, CURLFORM_COPYNAME, "f", CURLFORM_FILE, filePath, CURLFORM_END);

    // Make the HTTP request
    CURLcode res = HttpPost(url, post, response, progressCallback);

    // Clean up
    curl_formfree(post);

    return res;
}

CURLcode Authenticate(PuushApi* api, LPCSTR username, LPCSTR apiKey, RequestCompleteHandler onFinish) {
    // Build the URL for the API endpoint
    char url[256];
    snprintf(url, sizeof(url), "%s%s", api->baseUrl, "/api/auth");

    // Build the form data
    struct curl_httppost* post = NULL;
    struct curl_httppost* last = NULL;
    curl_formadd(&post, &last, CURLFORM_COPYNAME, "e", CURLFORM_COPYCONTENTS, username, CURLFORM_END);
    curl_formadd(&post, &last, CURLFORM_COPYNAME, "k", CURLFORM_COPYCONTENTS, apiKey, CURLFORM_END);
    curl_formadd(&post, &last, CURLFORM_COPYNAME, "z", CURLFORM_COPYCONTENTS, "poop", CURLFORM_END);

    // Make the HTTP request
    String response;
    CURLcode res = HttpPost(url, post, &response, NULL);

    // Invoke the callback
    if (onFinish) {
        onFinish(res, &response);
    }

    // Clean up
    curl_formfree(post);

    return res;
}

CURLcode AuthenticateWithPassword(PuushApi* api, LPCSTR username, LPCSTR password, RequestCompleteHandler onFinish) {
    // Build the URL for the API endpoint
    char url[256];
    snprintf(url, sizeof(url), "%s%s", api->baseUrl, "/api/auth");

    // Build the form data
    struct curl_httppost* post = NULL;
    struct curl_httppost* last = NULL;
    curl_formadd(&post, &last, CURLFORM_COPYNAME, "e", CURLFORM_COPYCONTENTS, username, CURLFORM_END);
    curl_formadd(&post, &last, CURLFORM_COPYNAME, "p", CURLFORM_COPYCONTENTS, password, CURLFORM_END);
    curl_formadd(&post, &last, CURLFORM_COPYNAME, "z", CURLFORM_COPYCONTENTS, "poop", CURLFORM_END);

    // Make the HTTP request
    String response;
    CURLcode res = HttpPost(url, post, &response);

    // Invoke the callback
    if (onFinish) {
        onFinish(res, &response);
    }

    // Clean up
    curl_formfree(post);

    return res;
}

CURLcode History(PuushApi* api, int limit, RequestCompleteHandler onFinish) {
    // Build the URL for the API endpoint
    char url[256];
    snprintf(url, sizeof(url), "%s%s", api->baseUrl, "/api/hist");

    // Build the form data
    struct curl_httppost* post = NULL;
    struct curl_httppost* last = NULL;
    curl_formadd(&post, &last, CURLFORM_COPYNAME, "k", CURLFORM_COPYCONTENTS, api->apiKey, CURLFORM_END);
    char limit_str[11];  // enough to hold all numbers up to 32-bits
    snprintf(limit_str, sizeof(limit_str), "%d", limit);
    curl_formadd(&post, &last, CURLFORM_COPYNAME, "l", CURLFORM_COPYCONTENTS, limit_str, CURLFORM_END);

    // Make the HTTP request
    String response;
    CURLcode res = HttpPost(url, post, &response);

    // Invoke the callback
    if (onFinish) {
        onFinish(res, &response);
    }

    // Clean up
    curl_formfree(post);

    return res;
}

CURLcode Delete(PuushApi* api, LPCSTR id, RequestCompleteHandler onFinish) {
    // Build the URL for the API endpoint
    char url[256];
    snprintf(url, sizeof(url), "%s%s", api->baseUrl, "/api/del");

    // Build the form data
    struct curl_httppost* post = NULL;
    struct curl_httppost* last = NULL;
    curl_formadd(&post, &last, CURLFORM_COPYNAME, "k", CURLFORM_COPYCONTENTS, api->apiKey, CURLFORM_END);
    curl_formadd(&post, &last, CURLFORM_COPYNAME, "i", CURLFORM_COPYCONTENTS, id, CURLFORM_END);
    curl_formadd(&post, &last, CURLFORM_COPYNAME, "z", CURLFORM_COPYCONTENTS, "poop", CURLFORM_END);
    curl_formadd(&post, &last, CURLFORM_COPYNAME, "l", CURLFORM_COPYCONTENTS, "0", CURLFORM_END);
    curl_formadd(&post, &last, CURLFORM_COPYNAME, "o", CURLFORM_COPYCONTENTS, "0", CURLFORM_END);

    // Make the HTTP request
    String response;
    CURLcode res = HttpPost(url, post, &response, NULL);

    // Invoke the callback
    if (onFinish) {
        onFinish(res, &response);
    }

    // Clean up
    curl_formfree(post);

    return res;
}