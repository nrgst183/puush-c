#pragma once

#include <Windows.h>
#include <locale.h>
#include <shlobj.h>
#include <tchar.h>
#include <windows.h>
#include <tchar.h>
#include "hotkey.h"
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <assert.h>
#include "extensions/utility_functions.h"
#include "extensions/ui_extensions.h"

#define MAX_BUFFER_SIZE 256
#define MAX_KEY_LENGTH 128
#define MAX_VALUE_LENGTH 128
#define MAX_LINE_LENGTH 128

#define PUUSH_SETTNGS_FILE_PATH L"puush-c.ini"

enum UploadQuality
{
    Best,
    High,
    Medium
};

enum FullscreenCaptureMode
{
    AllScreens,
    ScreenContainingMouseCursor,
    PrimaryScreen
};

enum DoubleClickBehaviour
{
    OpenSettings,
    ScreenSelect,
    UploadFile
};

typedef struct {
    int contextMenu;
    int contextMenuAttempted;
    int startup;
    WCHAR proxyServer[MAX_KEY_LENGTH];
    WCHAR username[MAX_KEY_LENGTH];
    Hotkey screenSelectionKey;
    WCHAR lastUpdate[MAX_KEY_LENGTH];
    int disableUpload;
    Hotkey fullscreenScreenshotKey;
    Hotkey currentWindowScreenshotKey;
    Hotkey uploadFileKey;
    Hotkey uploadClipboardKey;
    Hotkey toggleKey;
    int experimental;
    WCHAR key[MAX_KEY_LENGTH];
    int type;
    int usage;
    WCHAR expiry[MAX_KEY_LENGTH];
    int openBrowser;
    int notificationSound;
    int copyToClipboard;
    int saveImages;
    WCHAR saveImagePath[MAX_KEY_LENGTH];
    enum DoubleClickBehaviour doubleClickBehaviour;
    enum FullscreenCaptureMode fullscreenMode;
    enum UploadQuality uploadQuality;
    int selectionRectangle;
} PuushSettings;

extern PuushSettings puushSettings;

void LoadSettings(LPCWSTR iniFilePath);
void SaveSettings(const WCHAR* iniFilePath);
BOOL GetPuushIniFilePath(WCHAR* buffer, size_t bufferSize);

