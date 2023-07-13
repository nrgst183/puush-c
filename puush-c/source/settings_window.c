#include "settings_window.h"

LRESULT CALLBACK SettingsWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam) {
    switch (Message) {
    case WM_NOTIFY: {
        LPNMHDR lpnm = (LPNMHDR)lParam;
        switch (lpnm->code) {
        case TCN_SELCHANGE: {
            HWND hTab = GetDlgItem(hWnd, 100);
            int iPage = TabCtrl_GetCurSel(hTab);
            int tabCount = TabCtrl_GetItemCount(hTab);
            for (int i = 0; i < tabCount; i++) {
                ShowWindow(GetDlgItem(hTab, 200 + i), SW_HIDE);
            }
            ShowWindow(GetDlgItem(hTab, 200 + iPage), SW_SHOW);
            break;
        }
        }
        break;
    }
    default:
        return DefWindowProc(hWnd, Message, wParam, lParam);
    }
    return 0;
}

HWND CreateSettingsWindow(HINSTANCE hInstance, PuushSettings* settings) {
    // Register settings window class
    const TCHAR szSettingsClassName[] = TEXT("Settings");
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_VREDRAW | CS_HREDRAW, SettingsWndProc, 0, 0, hInstance, NULL, LoadCursor(NULL, IDC_ARROW), NULL, NULL, szSettingsClassName, NULL };
    RegisterClassEx(&wc);

    // Create settings window
    HWND hwnd = CreateWindowEx(0, szSettingsClassName, TEXT("puush settings"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);

    // Adjust the font size based on DPI scaling
    HDC hDC = GetDC(hwnd);
    int dpiX = GetDeviceCaps(hDC, LOGPIXELSX);
    int dpiY = GetDeviceCaps(hDC, LOGPIXELSY);
    ReleaseDC(hwnd, hDC);

    int fontSize = MulDiv(16, dpiY, 96); // Adjust the font size as needed

    HFONT hFont = CreateFont(fontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Segoe UI"));

    // Set the font for the settings window
    SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));

    LPWSTR tabNames[] = { L"General", L"Key Bindings", L"Account", L"Updates", L"Advanced", NULL };
    CreateTabControl(hwnd, hInstance, tabNames);

    // General tab
    LPWSTR groupBoxNamesGeneral[] = { L"General Settings", L"On successful puush", L"Tray Icon Behavior", NULL };
    CreateAndAddGroupBoxesToTabPage(hwnd, L"General", groupBoxNamesGeneral);

    // Key Bindings tab
    LPWSTR groupBoxNamesKeyBindings[] = { L"Keyboard Bindings", NULL };
    CreateAndAddGroupBoxesToTabPage(hwnd, L"Key Bindings", groupBoxNamesKeyBindings);

    // Account tab
    LPWSTR groupBoxNamesAccount[] = { L"Account Setup", L"Account settings", NULL };
    CreateAndAddGroupBoxesToTabPage(hwnd, L"Account", groupBoxNamesAccount);

    // Updates tab
    LPWSTR groupBoxNamesUpdates[] = { L"Update settings", L"Update Management", NULL };
    CreateAndAddGroupBoxesToTabPage(hwnd, L"Updates", groupBoxNamesUpdates);

    // Advanced tab
    LPWSTR groupBoxNamesAdvanced[] = { L"Screen Capture Quality", L"Context Menu", L"Fullscreen Capture", L"Dangerous Stuff", NULL };
    CreateAndAddGroupBoxesToTabPage(hwnd, L"Advanced", groupBoxNamesAdvanced);

    CreateCheckbox(hwnd, L"General", L"General Settings", 36, 21, 143, 19, L"Start puush on startup");

    CreateCheckbox(hwnd, L"General", L"On successful puush", 35, 20, 157, 19, L"Play a notification sound");
    CreateCheckbox(hwnd, L"General", L"On successful puush", 35, 41, 143, 19, L"Copy link to clipboard");
    CreateCheckbox(hwnd, L"General", L"On successful puush", 35, 64, 135, 19, L"Open link in browser");

    // Creates the Checkbox inside the GroupBox
    CreateCheckbox(hwnd, L"General", L"On successful puush", 223, 20, 166, 19, L"Save a local copy of image");
    CreateTextbox(hwnd, L"General", L"On successful puush", 246, 43, 160, 20, L"");
    CreateButton(hwnd, L"General", L"On successful puush", 412, 42, 23, 23, L"...");

    CreateRadioButton(hwnd, L"General", L"Tray Icon Behavior", 35, 20, 134, 19, L"Show settings dialog");
    CreateRadioButton(hwnd, L"General", L"Tray Icon Behavior", 35, 44, 169, 19, L"Begin screen capture mode");
    CreateRadioButton(hwnd, L"General", L"Tray Icon Behavior", 35, 68, 149, 19, L"Open upload file dialog");

    // Buffer to store the hotkey string
    TCHAR hotkeyString[256];

    CreateLabel(hwnd, L"Key Bindings", L"Keyboard Bindings", 49, 31, 109, 15, L"Capture full screen:");
    HotkeyToPrettyString(&settings->fullscreenScreenshotKey, hotkeyString, sizeof(hotkeyString) / sizeof(TCHAR));
    CreateButton(hwnd, L"Key Bindings", L"Keyboard Bindings", 211, 26, 150, 23, hotkeyString);
    memset(hotkeyString, 0, sizeof(hotkeyString));

    CreateLabel(hwnd, L"Key Bindings", L"Keyboard Bindings", 48, 60, 138, 15, L"Capture current window:");
    HotkeyToPrettyString(&settings->currentWindowScreenshotKey, hotkeyString, sizeof(hotkeyString) / sizeof(TCHAR));
    CreateButton(hwnd, L"Key Bindings", L"Keyboard Bindings", 211, 55, 150, 23, hotkeyString);
    memset(hotkeyString, 0, sizeof(hotkeyString));

    CreateLabel(hwnd, L"Key Bindings", L"Keyboard Bindings", 49, 89, 79, 15, L"Capture Area:");
    HotkeyToPrettyString(&settings->screenSelectionKey, hotkeyString, sizeof(hotkeyString) / sizeof(TCHAR));
    CreateButton(hwnd, L"Key Bindings", L"Keyboard Bindings", 211, 84, 150, 23, hotkeyString);
    memset(hotkeyString, 0, sizeof(hotkeyString));

    CreateLabel(hwnd, L"Key Bindings", L"Keyboard Bindings", 49, 118, 69, 15, L"Upload File:");
    HotkeyToPrettyString(&settings->uploadFileKey, hotkeyString, sizeof(hotkeyString) / sizeof(TCHAR));
    CreateButton(hwnd, L"Key Bindings", L"Keyboard Bindings", 211, 113, 150, 23, hotkeyString);
    memset(hotkeyString, 0, sizeof(hotkeyString));

    CreateLabel(hwnd, L"Key Bindings", L"Keyboard Bindings", 66, 141, 361, 36, L"Use this shortcut in Windows Explorer to quickly upload selected files.");

    CreateLabel(hwnd, L"Key Bindings", L"Keyboard Bindings", 49, 171, 103, 15, L"Upload Clipboard:");
    HotkeyToPrettyString(&settings->uploadClipboardKey, hotkeyString, sizeof(hotkeyString) / sizeof(TCHAR));
    CreateButton(hwnd, L"Key Bindings", L"Keyboard Bindings", 211, 166, 150, 23, hotkeyString);
    memset(hotkeyString, 0, sizeof(hotkeyString));

    CreateLabel(hwnd, L"Key Bindings", L"Keyboard Bindings", 49, 200, 153, 15, L"Toggle puush functionality:");
    HotkeyToPrettyString(&settings->toggleKey, hotkeyString, sizeof(hotkeyString) / sizeof(TCHAR));
    CreateButton(hwnd, L"Key Bindings", L"Keyboard Bindings", 211, 195, 150, 23, hotkeyString);


    // Add elements to "Account Setup" GroupBox
    CreateLabel(hwnd, L"Account", L"Account Setup", 9, 19, 433, 40, L"You need to login before you can make full use of puush. If you don't already have an account, you can register for free via the link below.");
    CreateLabel(hwnd, L"Account", L"Account Setup", 97, 69, 39, 15, L"Email:");
    CreateTextbox(hwnd, L"Account", L"Account Setup", 138, 66, 149, 20, L"");
    CreateLabel(hwnd, L"Account", L"Account Setup", 76, 97, 60, 15, L"Password:");
    CreateTextbox(hwnd, L"Account", L"Account Setup", 138, 94, 149, 20, L"");
    CreateButton(hwnd, L"Account", L"Account Setup", 308, 66, 127, 48, L"Login");

    // Add elements to "Update Management" GroupBox
    CreateLabel(hwnd, L"Updates", L"Update Management", 32, 46, 80, 15, L"Last Checked:");
    CreateLabel(hwnd, L"Updates", L"Update Management", 114, 46, 38, 15, L"Never");
    CreateButton(hwnd, L"Updates", L"Update Management", 288, 22, 152, 54, L"Check for Updates");

    // Create radio buttons for "Screen Capture Quality" GroupBox
    CreateRadioButton(hwnd, L"Advanced", L"Screen Capture Quality", 17, 44, 273, 19, L"Smart (use JPG unless PNG is smaller in filesize)");
    CreateRadioButton(hwnd, L"Advanced", L"Screen Capture Quality", 17, 20, 187, 19, L"No Compression (always PNG)");

    // Create radio buttons for "Fullscreen Capture" GroupBox
    CreateRadioButton(hwnd, L"Advanced", L"Fullscreen Capture", 17, 19, 124, 19, L"Capture all screens");
    CreateRadioButton(hwnd, L"Advanced", L"Fullscreen Capture", 17, 38, 239, 19, L"Capture screen containing mouse cursor");
    CreateRadioButton(hwnd, L"Advanced", L"Fullscreen Capture", 17, 57, 186, 19, L"Always capture primary screen");

    // Add checkbox to "Context Menu" GroupBox
    CreateCheckbox(hwnd, L"Advanced", L"Context Menu", 16, 21, 203, 19, L"Show explorer context menu item");

    return hwnd;
}
