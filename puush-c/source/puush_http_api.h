#include <windows.h>
#include <curl/curl.h>
#include "extensions/http_requests.h"

typedef void(*RequestCompleteHandler)(CURLcode, String*);

enum puush_error_code {
    PUUSHE_SUCCESS = 0,         // 0 success
    PUUSHE_FAILED_REQUEST,      // 1 raw request failed (check errno)
    PUUSHE_INVALID_CREDENTIALS, // 2 your user/pass combo was incorrect
    PUUSHE_INVALID_API_KEY,     // 3 your api key was invalid
    PUUSHE_NOT_AUTHED,          // 4 you haven't run puush_auth(_p) yet
    PUUSHE_NOT_INITIALZED,      // 5 you haven't run puush_init yet
    PUUSHE_BAD_DATA,            // 6 libpuush somehow sent invalid data
    PUUSHE_BAD_HASH,            // 7 libpuush somehow sent invalid checksum
    PUUSHE_UNKNOWN_ERROR,       // 8 puush gave unparseable results
};

struct puush {
    CURL* curl_handle;
    int is_premium;
    char* api_key;
    long quota_used; // bytes
};

struct puush_object {
    char* url;
    char* id;
    char* filename;
    time_t timestamp;
    int views;
    int remaining;
    struct puush_object* next;
};

typedef struct {
    CURL* curl;
    LPCSTR apiKey;
    LPCSTR baseUrl;
} PuushApi;

typedef void(*UploadProgressCallback)(double);
typedef void(*RequestCompleteHandler)(CURLcode, String*);


CURLcode Authenticate(PuushApi* api, LPCSTR username, LPCSTR apiKey, RequestCompleteHandler onFinish);
CURLcode AuthenticateWithPassword(PuushApi* api, LPCSTR username, LPCSTR password, RequestCompleteHandler onFinish);
CURLcode History(PuushApi* api, int limit, RequestCompleteHandler onFinish);
CURLcode Delete(PuushApi* api, LPCSTR id, RequestCompleteHandler onFinish);