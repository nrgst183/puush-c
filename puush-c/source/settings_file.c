#include "settings_file.h"

PuushSettings puushSettings = { 0 };

typedef struct {
    wchar_t key[MAX_KEY_LENGTH];
    wchar_t value[MAX_VALUE_LENGTH];
} KeyValue;

void RemoveSpaces(char* str) {
    char* i = str;
    char* j = str;
    while (*j != '\0') {
        *i = *j++;
        if (!isspace(*i)) {
            i++;
        }
    }
    *i = '\0';
}

int LoadIniKeyValues(LPCTSTR filePath, KeyValue* keyValues, int size) {
    FILE* file;
    char line[MAX_LINE_LENGTH];
    int i = 0;

    char narrowFilePath[MAX_PATH];
    ConvertTCharToNarrowString(filePath, narrowFilePath, MAX_PATH);

    errno_t err = fopen_s(&file, narrowFilePath, "r");

    if (err != 0 || !file) {
        return -1;
    }

    while (fgets(line, MAX_LINE_LENGTH, file) && i < size) {
        char* context = NULL;
        char* key = strtok_s(line, "=", &context);

        if (key) {
            char* value = strtok_s(NULL, "\n", &context);
            if (value) {
                RemoveSpaces(key);
                RemoveSpaces(value);

                // Convert narrow strings to wide strings
                ConvertNarrowStringToTChar(key, keyValues[i].key, MAX_KEY_LENGTH);
                ConvertNarrowStringToTChar(value, keyValues[i].value, MAX_VALUE_LENGTH);

                i++;
            }
            else {
                printf("Could not parse value for line: %s\n", line);
            }
        }
        else {
            printf("Could not parse key for line: %s\n", line);
        }
    }

    fclose(file);
    return i;
}

void SaveIniKeyValues(LPCTSTR filePath, const KeyValue* keyValues, int size) {
    FILE* file;
    int i;

    char narrowFilePath[MAX_PATH];
    ConvertTCharToNarrowString(filePath, narrowFilePath, MAX_PATH);

    errno_t err = fopen_s(&file, narrowFilePath, "w");
    if (err != 0 || !file) {
        return;
    }

    for (i = 0; i < size; i++) {
        char narrowKey[MAX_KEY_LENGTH];
        ConvertTCharToNarrowString(keyValues[i].key, narrowKey, MAX_KEY_LENGTH);

        char narrowValue[MAX_VALUE_LENGTH];
        ConvertTCharToNarrowString(keyValues[i].value, narrowValue, MAX_VALUE_LENGTH);

        // Write the key-value pair to the file in the format: key=value
        fprintf(file, "%s=%s\n", narrowKey, narrowValue);
    }

    fclose(file);
}


void LoadSettings(const TCHAR* iniFilePath) {
    KeyValue* keyValues = (KeyValue*)malloc(MAX_BUFFER_SIZE * sizeof(KeyValue));
    if (keyValues == NULL) {
        // Error: Failed to allocate memory
        return;
    }

    int size = LoadIniKeyValues(iniFilePath, keyValues, MAX_BUFFER_SIZE);

    if (size < 0) {
        // Error: Failed to load key-value pairs
        free(keyValues);
        return;
    }

    for (int i = 0; i < size; i++) {
        if (wcscmp(keyValues[i].key, L"contextmenu") == 0) {
            puushSettings.contextMenu = atoi(keyValues[i].value);
        }
        if (wcscmp(keyValues[i].key, L"contextmenuattempted") == 0) {
            puushSettings.contextMenuAttempted = atoi(keyValues[i].value);
        }
        else if (wcscmp(keyValues[i].key, L"startup") == 0) {
            puushSettings.startup = atoi(keyValues[i].value);
        }
        else if (wcscmp(keyValues[i].key, L"ProxyServer") == 0) {
            mbstowcs_s(NULL, puushSettings.proxyServer, sizeof(puushSettings.proxyServer) / sizeof(wchar_t), keyValues[i].value, _TRUNCATE);
        }
        else if (wcscmp(keyValues[i].key, L"username") == 0) {
            mbstowcs_s(NULL, puushSettings.username, sizeof(puushSettings.username) / sizeof(wchar_t), keyValues[i].value, _TRUNCATE);
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
            puushSettings.experimental = atoi(keyValues[i].value);
        }
        else if (wcscmp(keyValues[i].key, L"key") == 0) {
            mbstowcs_s(NULL, puushSettings.key, sizeof(puushSettings.key) / sizeof(wchar_t), keyValues[i].value, _TRUNCATE);
        }
        else if (wcscmp(keyValues[i].key, L"type") == 0) {
            puushSettings.type = atoi(keyValues[i].value);
        }
        else if (wcscmp(keyValues[i].key, L"usage") == 0) {
            puushSettings.usage = atoi(keyValues[i].value);
        }
        else if (wcscmp(keyValues[i].key, L"expiry") == 0) {
            mbstowcs_s(NULL, puushSettings.expiry, sizeof(puushSettings.expiry) / sizeof(wchar_t), keyValues[i].value, _TRUNCATE);
        }
        else if (wcscmp(keyValues[i].key, L"openbrowser") == 0) {
            puushSettings.openBrowser = atoi(keyValues[i].value);
        }
        else if (wcscmp(keyValues[i].key, L"notificationsound") == 0) {
            puushSettings.notificationSound = atoi(keyValues[i].value);
        }
        else if (wcscmp(keyValues[i].key, L"copytoclipboard") == 0) {
            puushSettings.copyToClipboard = atoi(keyValues[i].value);
        }
        else if (wcscmp(keyValues[i].key, L"saveimages") == 0) {
            puushSettings.saveImages = atoi(keyValues[i].value);
        }
        else if (wcscmp(keyValues[i].key, L"saveimagepath") == 0) {
            mbstowcs_s(NULL, puushSettings.saveImagePath, sizeof(puushSettings.saveImagePath) / sizeof(wchar_t), keyValues[i].value, _TRUNCATE);
        }
        else if (wcscmp(keyValues[i].key, L"doubleclickbehaviour") == 0) {
            puushSettings.doubleClickBehaviour = atoi(keyValues[i].value);
        }
        else if (wcscmp(keyValues[i].key, L"FullscreenMode") == 0) {
            puushSettings.fullscreenMode = atoi(keyValues[i].value);
        }
        else if (wcscmp(keyValues[i].key, L"uploadquality") == 0) {
            puushSettings.uploadQuality = atoi(keyValues[i].value);
        }
        else if (wcscmp(keyValues[i].key, L"selectionrectangle") == 0) {
            puushSettings.selectionRectangle = atoi(keyValues[i].value);
        }
    }

    free(keyValues); // Free the dynamically allocated memory

}

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

BOOL GetPuushIniFilePath(TCHAR* buffer, size_t bufferSize) {
    if (buffer == NULL || bufferSize < MAX_PATH) {
        return FALSE; // Buffer is null or not large enough.
    }

    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, buffer))) {
        _tcscat_s(buffer, bufferSize, _T("\\puush\\puush.ini"));
        return TRUE;
    }
    else {
        return FALSE; // Failed to get AppData folder path.
    }
}