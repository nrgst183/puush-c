#include "settings_file.h"

PuushSettings puushSettings = { 0 };

typedef struct {
    WCHAR key[MAX_KEY_LENGTH];
    WCHAR value[MAX_VALUE_LENGTH];
} KeyValue;

void RemoveSpacesW(WCHAR* str) {
    WCHAR* i = str;
    WCHAR* j = str;
    while (*j != L'\0') {
        *i = *j++;
        if (!iswspace(*i)) {
            i++;
        }
    }
    *i = L'\0';
}

int LoadIniKeyValues(LPCWSTR filePath, KeyValue* keyValues, int size) {
    FILE* file;
    WCHAR line[MAX_LINE_LENGTH];
    int i = 0;

    errno_t err = _wfopen_s(&file, filePath, L"r");
    assert(err == 0 && "Failed to open the INI file.");
    if (!file) {
        return -1;
    }

    while (fgetws(line, MAX_LINE_LENGTH, file) && i < size) {
        WCHAR* context = NULL;
        WCHAR* key = wcstok_s(line, L"=", &context);

        if (key) {
            WCHAR* value = wcstok_s(NULL, L"\n", &context);
            assert(value && "Could not parse value for the given line.");
            if (value) {
                RemoveSpacesW(key);
                RemoveSpacesW(value);

                wcsncpy_s(keyValues[i].key, MAX_KEY_LENGTH, key, _TRUNCATE);
                wcsncpy_s(keyValues[i].value, MAX_VALUE_LENGTH, value, _TRUNCATE);

                i++;
            }
        }
        else {
            assert(key && "Could not parse key for the given line.");
        }
    }

    fclose(file);
    return i;
}

void SaveIniKeyValues(LPCWSTR filePath, const KeyValue* keyValues, int size) {
    FILE* file;
    int i;

    errno_t err = _wfopen_s(&file, filePath, L"w");
    if (err != 0 || !file) {
        return;
    }

    for (i = 0; i < size; i++) {
        // Write the key-value pair to the file in the format: key=value
        fwprintf(file, L"%s=%s\n", keyValues[i].key, keyValues[i].value);
    }

    fclose(file);
}

void LoadSettings(LPCWSTR iniFilePath) {
    KeyValue* keyValues = (KeyValue*)malloc(MAX_BUFFER_SIZE * sizeof(KeyValue));

    assert(keyValues != NULL && "Failed to allocate memory for keyValues.");

    int size = LoadIniKeyValues(iniFilePath, keyValues, MAX_BUFFER_SIZE);

    assert(size >= 0 && "Failed to load key-value pairs from the INI file.");

    for (int i = 0; i < size; i++) {
        if (wcscmp(keyValues[i].key, L"contextmenu") == 0) {
            puushSettings.contextMenu = _wtoi(keyValues[i].value);
        }
        else if (wcscmp(keyValues[i].key, L"contextmenuattempted") == 0) {
            puushSettings.contextMenuAttempted = _wtoi(keyValues[i].value);
        }
        else if (wcscmp(keyValues[i].key, L"startup") == 0) {
            puushSettings.startup = _wtoi(keyValues[i].value);
        }
        else if (wcscmp(keyValues[i].key, L"ProxyServer") == 0) {
            wcsncpy_s(puushSettings.proxyServer, sizeof(puushSettings.proxyServer) / sizeof(wchar_t), keyValues[i].value, _TRUNCATE);
        }
        else if (wcscmp(keyValues[i].key, L"username") == 0) {
            wcsncpy_s(puushSettings.username, sizeof(puushSettings.username) / sizeof(wchar_t), keyValues[i].value, _TRUNCATE);
        }
        else if (wcscmp(keyValues[i].key, L"b_ScreenSelection") == 0) {
            puushSettings.screenSelectionKey = ParseHotkeyFromIniFileString(keyValues[i].value);
        }
        else if (wcscmp(keyValues[i].key, L"b_FullscreenScreenshot") == 0) {
            puushSettings.fullscreenScreenshotKey = ParseHotkeyFromIniFileString(keyValues[i].value);
        }
        else if (wcscmp(keyValues[i].key, L"b_CurrentWindowScreenshot") == 0) {
            puushSettings.currentWindowScreenshotKey = ParseHotkeyFromIniFileString(keyValues[i].value);
        }
        else if (wcscmp(keyValues[i].key, L"b_UploadFile") == 0) {
            puushSettings.uploadFileKey = ParseHotkeyFromIniFileString(keyValues[i].value);
        }
        else if (wcscmp(keyValues[i].key, L"b_UploadClipboard") == 0) {
            puushSettings.uploadClipboardKey = ParseHotkeyFromIniFileString(keyValues[i].value);
        }
        else if (wcscmp(keyValues[i].key, L"b_Toggle") == 0) {
            puushSettings.toggleKey = ParseHotkeyFromIniFileString(keyValues[i].value);
        }
        else if (wcscmp(keyValues[i].key, L"Experimental") == 0) {
            puushSettings.experimental = _wtoi(keyValues[i].value);
        }
        else if (wcscmp(keyValues[i].key, L"key") == 0) {
            wcsncpy_s(puushSettings.key, sizeof(puushSettings.key) / sizeof(wchar_t), keyValues[i].value, _TRUNCATE);
        }
        else if (wcscmp(keyValues[i].key, L"type") == 0) {
            puushSettings.type = _wtoi(keyValues[i].value);
        }
        else if (wcscmp(keyValues[i].key, L"usage") == 0) {
            puushSettings.usage = _wtoi(keyValues[i].value);
        }
        else if (wcscmp(keyValues[i].key, L"expiry") == 0) {
            wcsncpy_s(puushSettings.expiry, sizeof(puushSettings.expiry) / sizeof(wchar_t), keyValues[i].value, _TRUNCATE);
        }
        else if (wcscmp(keyValues[i].key, L"openbrowser") == 0) {
            puushSettings.openBrowser = _wtoi(keyValues[i].value);
        }
        else if (wcscmp(keyValues[i].key, L"notificationsound") == 0) {
            puushSettings.notificationSound = _wtoi(keyValues[i].value);
        }
        else if (wcscmp(keyValues[i].key, L"copytoclipboard") == 0) {
            puushSettings.copyToClipboard = _wtoi(keyValues[i].value);
        }
        else if (wcscmp(keyValues[i].key, L"saveimages") == 0) {
            puushSettings.saveImages = _wtoi(keyValues[i].value);
        }
        else if (wcscmp(keyValues[i].key, L"saveimagepath") == 0) {
            wcsncpy_s(puushSettings.saveImagePath, sizeof(puushSettings.saveImagePath) / sizeof(wchar_t), keyValues[i].value, _TRUNCATE);
        }
        else if (wcscmp(keyValues[i].key, L"doubleclickbehaviour") == 0) {
            puushSettings.doubleClickBehaviour = _wtoi(keyValues[i].value);
        }
        else if (wcscmp(keyValues[i].key, L"FullscreenMode") == 0) {
            puushSettings.fullscreenMode = _wtoi(keyValues[i].value);
        }
        else if (wcscmp(keyValues[i].key, L"uploadquality") == 0) {
            puushSettings.uploadQuality = _wtoi(keyValues[i].value);
        }
        else if (wcscmp(keyValues[i].key, L"selectionrectangle") == 0) {
            puushSettings.selectionRectangle = _wtoi(keyValues[i].value);
        }
    }

    free(keyValues); // Free the dynamically allocated memory
}
/*
void SaveSettings(const TCHAR* iniFilePath) {
    TCHAR buffer[MAX_PATH];

    WritePrivateProfileString(NULL, L"contextmenu", puushSettings.contextMenu ? L"1" : L"0", iniFilePath);
    WritePrivateProfileString(NULL, L"contextmenuattempted", puushSettings.contextMenuAttempted ? L"1" : L"0", iniFilePath);
    WritePrivateProfileString(NULL, L"startup", puushSettings.startup ? L"1" : L"0", iniFilePath);
    WritePrivateProfileString(NULL, L"ProxyServer", puushSettings.proxyServer, iniFilePath);
    WritePrivateProfileString(NULL, L"username", puushSettings.username, iniFilePath);

    HotkeyToIniFileString(&puushSettings.fullscreenScreenshotKey, buffer, sizeof(buffer) / sizeof(TCHAR));
    char narrowBuffer[MAX_PATH];
    wcstombs_s(NULL, narrowBuffer, MAX_PATH, buffer, MAX_PATH);
    WritePrivateProfileString(NULL, L"b_FullscreenScreenshot", narrowBuffer, iniFilePath);

    HotkeyToIniFileString(&puushSettings.currentWindowScreenshotKey, buffer, sizeof(buffer) / sizeof(TCHAR));
    wcstombs_s(NULL, narrowBuffer, MAX_PATH, buffer, MAX_PATH);
    WritePrivateProfileString(NULL, L"b_CurrentWindowScreenshot", narrowBuffer, iniFilePath);

    HotkeyToIniFileString(&puushSettings.screenSelectionKey, buffer, sizeof(buffer) / sizeof(TCHAR));
    wcstombs_s(NULL, narrowBuffer, MAX_PATH, buffer, MAX_PATH);
    WritePrivateProfileString(NULL, L"b_ScreenSelection", narrowBuffer, iniFilePath);

    HotkeyToIniFileString(&puushSettings.uploadFileKey, buffer, sizeof(buffer) / sizeof(TCHAR));
    wcstombs_s(NULL, narrowBuffer, MAX_PATH, buffer, MAX_PATH);
    WritePrivateProfileString(NULL, L"b_UploadFile", narrowBuffer, iniFilePath);

    HotkeyToIniFileString(&puushSettings.uploadClipboardKey, buffer, sizeof(buffer) / sizeof(TCHAR));
    wcstombs_s(NULL, narrowBuffer, MAX_PATH, buffer, MAX_PATH);
    WritePrivateProfileString(NULL, L"b_UploadClipboard", narrowBuffer, iniFilePath);

    HotkeyToIniFileString(&puushSettings.toggleKey, buffer, sizeof(buffer) / sizeof(TCHAR));
    wcstombs_s(NULL, narrowBuffer, MAX_PATH, buffer, MAX_PATH);
    WritePrivateProfileString(NULL, L"b_Toggle", narrowBuffer, iniFilePath);

    _itot_s(puushSettings.experimental, buffer, 12, 10);
    WritePrivateProfileString(NULL, L"Experimental", buffer, iniFilePath);

    WritePrivateProfileString(NULL, L"key", puushSettings.key, iniFilePath);

    _itot_s(puushSettings.type, buffer, 12, 10);
    WritePrivateProfileString(NULL, L"type", buffer, iniFilePath);

    _itot_s(puushSettings.usage, buffer, 12, 10);
    WritePrivateProfileString(NULL, L"usage", buffer, iniFilePath);

    WritePrivateProfileString(NULL, L"expiry", puushSettings.expiry, iniFilePath);

    _itot_s(puushSettings.openBrowser, buffer, 12, 10);
    WritePrivateProfileString(NULL, L"openbrowser", buffer, iniFilePath);

    _itot_s(puushSettings.notificationSound, buffer, 12, 10);
    WritePrivateProfileString(NULL, L"notificationsound", buffer, iniFilePath);

    _itot_s(puushSettings.copyToClipboard, buffer, 12, 10);
    WritePrivateProfileString(NULL, L"copytoclipboard", buffer, iniFilePath);

    _itot_s(puushSettings.saveImages, buffer, 12, 10);
    WritePrivateProfileString(NULL, L"saveimages", buffer, iniFilePath);

    WritePrivateProfileString(NULL, L"saveimagepath", puushSettings.saveImagePath, iniFilePath);

    _itot_s(puushSettings.doubleClickBehaviour, buffer, 12, 10);
    WritePrivateProfileString(NULL, L"doubleclickbehaviour", buffer, iniFilePath);

    _itot_s(puushSettings.fullscreenMode, buffer, 12, 10);
    WritePrivateProfileString(NULL, L"FullscreenMode", buffer, iniFilePath);

    _itot_s(puushSettings.uploadQuality, buffer, 12, 10);
    WritePrivateProfileString(NULL, L"uploadquality", buffer, iniFilePath);

    _itot_s(puushSettings.selectionRectangle, buffer, 12, 10);
    WritePrivateProfileString(NULL, L"selectionrectangle", buffer, iniFilePath);
}
*/
BOOL GetPuushIniFilePath(WCHAR* buffer, size_t bufferSize) {
    if (buffer == NULL || bufferSize < MAX_PATH) {
        return FALSE; // Buffer is null or not large enough.
    }

    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, buffer))) {
        wcscat_s(buffer, bufferSize, L"\\puush\\puush.ini");
        return TRUE;
    }
    else {
        return FALSE; // Failed to get AppData folder path.
    }
}