#pragma once

#include <windows.h>
#include <commctrl.h>
#include "extensions/window_extensions.h"
#include "settings_file.h"

enum ControlIDs {
    IDC_CHK_STARTUP = 300,
    IDC_CHK_NOTIFICATION_SOUND,
    IDC_CHK_COPY_LINK,
    IDC_CHK_OPEN_LINK,
    IDC_CHK_SAVE_LOCAL_COPY,
    IDC_TXT_LOCAL_COPY_PATH,
    IDC_BTN_BROWSE_LOCAL_COPY,
    IDC_BTN_OK,
    IDC_BTN_CANCEL,
    IDC_RAD_OPTION1,
    IDC_RAD_OPTION2,
};

LRESULT CALLBACK SettingsWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
HWND CreateSettingsWindow(HINSTANCE hInstance, PuushSettings* settings);