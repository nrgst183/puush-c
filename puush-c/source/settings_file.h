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
#include "extensions/utility_functions.h"
#include "extensions/ui_extensions.h"

#define MAX_BUFFER_SIZE 512
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
    Mouse,
    Primary
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
    TCHAR proxyServer[MAX_KEY_LENGTH];
    TCHAR username[MAX_KEY_LENGTH];
    Hotkey screenSelectionKey;
    TCHAR lastUpdate[MAX_KEY_LENGTH];
    int disableUpload;
    Hotkey fullscreenScreenshotKey;
    Hotkey currentWindowScreenshotKey;
    Hotkey uploadFileKey;
    Hotkey uploadClipboardKey;
    Hotkey toggleKey;
    int experimental;
    TCHAR key[MAX_KEY_LENGTH];
    int type;
    int usage;
    TCHAR expiry[MAX_KEY_LENGTH];
    int openBrowser;
    int notificationSound;
    int copyToClipboard;
    int saveImages;
    TCHAR saveImagePath[MAX_KEY_LENGTH];
    enum DoubleClickBehaviour doubleClickBehaviour;
    enum FullscreenCaptureMode fullscreenMode;
    enum UploadQuality uploadQuality;
    int selectionRectangle;
} PuushSettings;

extern PuushSettings puushSettings;

void LoadSettings(LPCTSTR iniFilePath);
void SaveSettings(const TCHAR* iniFilePath);
BOOL GetPuushIniFilePath(TCHAR* buffer, size_t bufferSize);

