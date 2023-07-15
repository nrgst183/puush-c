#include "main_tray_window.h"

#pragma comment(lib, "Msimg32.lib")

#define WM_TRAYICON     (WM_USER + 1)
#define WM_CAPTURE      (WM_USER + 2)

#define IDM_MY_ACCOUNT 1001
#define IDM_RECENT_UPLOADS 1002
#define IDM_CAPTURE_WINDOW 1003
#define IDM_CAPTURE_DESKTOP 1004
#define IDM_CAPTURE_AREA 1005
#define IDM_UPLOAD_CLIPBOARD 1006
#define IDM_UPLOAD_FILE 1007
#define IDM_DISABLE_PUUSH 1008
#define IDM_SETTINGS 1009
#define IDM_CANCEL_UPLOAD 1010
#define IDM_EXIT 1011

// Global Variables
static HHOOK keyboardHook;
static HWND hMainTrayWnd;
static HWND hSettingsWnd;
static BOOL puushingDisabled;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static NOTIFYICONDATA nid = { 0 };

    switch (msg) {
    case WM_CREATE:
        AddTrayIcon(hwnd, 6969, IDI_TRAY_ICON, L"puush-c", WM_TRAYICON);
        break;

    case WM_TRAYICON:
        if (lParam == WM_RBUTTONDOWN) {
            POINT pt;
            GetCursorPos(&pt);
            HMENU hMenu = CreatePopupMenu();
            if (hMenu) {
                AppendMenu(hMenu, MF_STRING, IDM_MY_ACCOUNT, L"My Account");
                AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
                AppendMenu(hMenu, MF_STRING, IDM_RECENT_UPLOADS, L"Recent Uploads");
                AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
                AppendMenu(hMenu, MF_STRING, IDM_CAPTURE_WINDOW, L"Capture Current Window");
                AppendMenu(hMenu, MF_STRING, IDM_CAPTURE_DESKTOP, L"Capture Desktop");
                AppendMenu(hMenu, MF_STRING, IDM_CAPTURE_AREA, L"Capture Area...");
                AppendMenu(hMenu, MF_STRING, IDM_UPLOAD_CLIPBOARD, L"Upload Clipboard");
                AppendMenu(hMenu, MF_STRING, IDM_UPLOAD_FILE, L"Upload File...");
                AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
                AppendMenu(hMenu, MF_STRING, IDM_DISABLE_PUUSH, L"Disable puushing");
                AppendMenu(hMenu, MF_STRING, IDM_SETTINGS, L"Settings...");
                AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
                AppendMenu(hMenu, MF_STRING, IDM_CANCEL_UPLOAD, L"Cancel Upload");
                AppendMenu(hMenu, MF_STRING, IDM_EXIT, L"Exit");

                SetForegroundWindow(hwnd);
                TrackPopupMenu(hMenu, TPM_BOTTOMALIGN, pt.x, pt.y, 0, hwnd, NULL);
                DestroyMenu(hMenu);
            }
        }
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDM_SETTINGS:
            hSettingsWnd = CreateSettingsWindow(GetModuleHandle(NULL), &puushSettings);
            break;
        case IDM_EXIT:
            PostMessage(hwnd, WM_CLOSE, 0, 0);
            break;
        }
        break;
    case WM_TIMER:
        KillTimer(hwnd, 1);
        nid.hWnd = hwnd;
        nid.uID = 100; // Unique identifier for the icon
        Shell_NotifyIcon(NIM_DELETE, &nid);
        break;
    case WM_CLOSE:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && wParam == WM_KEYDOWN) {
        KBDLLHOOKSTRUCT* kbdStruct = (KBDLLHOOKSTRUCT*)lParam;

        if (IsHotkeyMatch(kbdStruct, &puushSettings.currentWindowScreenshotKey) && !puushingDisabled) {
            HWND hwndActive = GetForegroundWindow();
            RECT rc;
            if (hwndActive && GetWindowRect(hwndActive, &rc)) {
                CaptureScreenAndSave(rc, "activewindow.bmp");
                PlaySuccessSound();
                ShowPuushCompleteBalloonTip(TEXT("Screenshot saved to path/to/screenshot.png"));
            }
        }
        else if (IsHotkeyMatch(kbdStruct, &puushSettings.fullscreenScreenshotKey) && !puushingDisabled) {
            RECT screenRect = { 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
            CaptureScreenAndSave(screenRect, "fullscreen.bmp");
            PlaySuccessSound();
            ShowPuushCompleteBalloonTip(TEXT("Screenshot saved to path/to/screenshot.png"));
        }
        else if (IsHotkeyMatch(kbdStruct, &puushSettings.screenSelectionKey) && !puushingDisabled) {
            SelectionRectangle selection = CreateScreenRegionSelectWindow();
            RECT selectionRect = { selection.left, selection.top, selection.right, selection.bottom };
            CaptureScreenAndSave(selectionRect, "selection.bmp");
            PlaySuccessSound();
            ShowPuushCompleteBalloonTip(TEXT("Screenshot saved to path/to/screenshot.png"));
        }
        else if (IsHotkeyMatch(kbdStruct, &puushSettings.toggleKey)) {
            TogglePuushFunctionality();
            ShowPuushToggleBalloonTip(puushingDisabled);
        }
    }
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

void TogglePuushFunctionality() {
    puushingDisabled = !puushingDisabled;
}

void ShowPuushToggleBalloonTip(BOOL puushDisabled) {
    ShowInfoBalloonTip(hMainTrayWnd,
        puushDisabled ? TEXT("puush was disabled!") : TEXT("puush was enabled!"),
        puushDisabled ? TEXT("Shortcut keys will no longer be accepted.") : TEXT("Shortcut keys will now be accepted."),
        2000);
}

void ShowPuushCompleteBalloonTip(const TCHAR* url) {
    ShowBalloonTip(hMainTrayWnd, TEXT("puush complete!"), url, NIIF_INFO, 5000);
}

void PlaySuccessSound() {
    PlaySound(MAKEINTRESOURCE(IDR_SUCCESS_WAVE), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    const wchar_t* CLASS_NAME = L"TrayAppWindowClass";
    TCHAR iniFilePath[1024];

    if (!GetPuushIniFilePath(iniFilePath, sizeof(iniFilePath) / sizeof(TCHAR))) {
        wprintf(L"Failed to get the settings file path.\n");
        return 1;
    }
    else 
    {
        LoadSettings(iniFilePath);
    }

    puushingDisabled = FALSE;

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);
    
    hMainTrayWnd = CreateWindowEx(0, CLASS_NAME, L"", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, hInstance, NULL);

    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInstance, 0);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(keyboardHook);

    return (int)msg.wParam;
}
