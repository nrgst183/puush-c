#include "settings_window.h"

BOOL capturedKeys[256] = { FALSE };
BOOL isKeyCapturing = FALSE;
HWND hButtonCaptureKeys;

LRESULT CALLBACK SettingsWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam) {
    switch (Message) {
    case WM_NOTIFY: {
        LPNMHDR lpnm = (LPNMHDR)lParam;
        switch (lpnm->code) {
        case TCN_SELCHANGE: {
            HandleTabControlTabChange(&wContext);
            break;
        }
        }
        break;
    }
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case 1: // Replace with your button's ID
            // Handle button 1 click
            break;
        default:
            return DefWindowProc(hWnd, Message, wParam, lParam);
        }
    }
    break;
    default:
        return DefWindowProc(hWnd, Message, wParam, lParam);
    }
    return 0;
}


LRESULT CALLBACK SettingsKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;

        if (wParam == WM_KEYDOWN)
        {
            KeyMapping* keyMapping = VkCodeToKeyMapping(p->vkCode);
            if (keyMapping != NULL)
            {
                // If this is a new key event and we are not currently capturing
                if (!isKeyCapturing)
                {
                    isKeyCapturing = TRUE;
                    memset(capturedKeys, 0, sizeof(capturedKeys));
                }

                // Check if key already exists in captured keys
                if (capturedKeys[p->vkCode])
                {
                    return CallNextHookEx(NULL, nCode, wParam, lParam);
                }

                // Add key to captured keys
                capturedKeys[p->vkCode] = TRUE;

                // Update button text to show captured keys
                Hotkey capturedHotkey;
                MakeHotkeyFromCapturedKeys(&capturedHotkey);
                UpdateKeyCaptureButton(hButtonCaptureKeys, capturedHotkey);

                // End capture if non-modifier key is pressed
                if (!keyMapping->isModifier)
                {
                    isKeyCapturing = FALSE;
                }
            }
        }

    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

HWND CreateSettingsWindow(HINSTANCE hInstance, PuushSettings* settings) {
    // Register settings window class
    const TCHAR szSettingsClassName[] = TEXT("Settings");
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_VREDRAW | CS_HREDRAW, SettingsWndProc, 0, 0, hInstance, NULL, LoadCursor(NULL, IDC_ARROW), NULL, NULL, szSettingsClassName, NULL };
    RegisterClassEx(&wc);
    // Create settings window
    HWND hwnd = CreateWindowEx(0, szSettingsClassName, TEXT("puush settings"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);

    wContext.currentControlCount = 0;
    wContext.hWnd = hwnd;
    wContext.lpfnWndProc = SettingsWndProc;

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
    CreateTabControl(&wContext, hInstance, tabNames);

    // General tab
    LPWSTR groupBoxNamesGeneral[] = { L"General Settings", L"On successful puush", L"Tray Icon Behavior", NULL };
    CreateAndAddGroupBoxesToTabPage(&wContext, L"General", groupBoxNamesGeneral);

    // Key Bindings tab
    LPWSTR groupBoxNamesKeyBindings[] = { L"Keyboard Bindings", NULL };
    CreateAndAddGroupBoxesToTabPage(&wContext, L"Key Bindings", groupBoxNamesKeyBindings);

    // Account tab
    LPWSTR groupBoxNamesAccount[] = { L"Account Setup", L"Account settings", NULL };
    CreateAndAddGroupBoxesToTabPage(&wContext, L"Account", groupBoxNamesAccount);

    // Updates tab
    LPWSTR groupBoxNamesUpdates[] = { L"Update settings", L"Update Management", NULL };
    CreateAndAddGroupBoxesToTabPage(&wContext, L"Updates", groupBoxNamesUpdates);

    // Advanced tab
    LPWSTR groupBoxNamesAdvanced[] = { L"Screen Capture Quality", L"Context Menu", L"Fullscreen Capture", L"Dangerous Stuff", NULL };
    CreateAndAddGroupBoxesToTabPage(&wContext, L"Advanced", groupBoxNamesAdvanced);

    CreateCheckbox(&wContext, L"General", L"General Settings", 36, 21, 143, 19, L"Start puush on startup");

    CreateCheckbox(&wContext, L"General", L"On successful puush", 35, 20, 157, 19, L"Play a notification sound");
    CreateCheckbox(&wContext, L"General", L"On successful puush", 35, 41, 143, 19, L"Copy link to clipboard");
    CreateCheckbox(&wContext, L"General", L"On successful puush", 35, 64, 135, 19, L"Open link in browser");

    // Creates the Checkbox inside the GroupBox
    CreateCheckbox(&wContext, L"General", L"On successful puush", 223, 20, 166, 19, L"Save a local copy of image");
    CreateTextbox(&wContext, L"General", L"On successful puush", 246, 43, 160, 20, L"");
    CreateButton(&wContext, L"General", L"On successful puush", 412, 42, 23, 23, L"...");

    CreateRadioButton(&wContext, L"General", L"Tray Icon Behavior", 35, 20, 134, 19, L"Show settings dialog");
    CreateRadioButton(&wContext, L"General", L"Tray Icon Behavior", 35, 44, 169, 19, L"Begin screen capture mode");
    CreateRadioButton(&wContext, L"General", L"Tray Icon Behavior", 35, 68, 149, 19, L"Open upload file dialog");

    // Buffer to store the hotkey string
    TCHAR hotkeyString[256];

    CreateLabel(&wContext, L"Key Bindings", L"Keyboard Bindings", 49, 31, 109, 15, L"Capture full screen:");
    HotkeyToPrettyString(&settings->fullscreenScreenshotKey, hotkeyString, sizeof(hotkeyString) / sizeof(TCHAR));
    CreateButton(&wContext, L"Key Bindings", L"Keyboard Bindings", 211, 26, 150, 23, hotkeyString);
    memset(hotkeyString, 0, sizeof(hotkeyString));

    CreateLabel(&wContext, L"Key Bindings", L"Keyboard Bindings", 48, 60, 138, 15, L"Capture current window:");
    HotkeyToPrettyString(&settings->currentWindowScreenshotKey, hotkeyString, sizeof(hotkeyString) / sizeof(TCHAR));
    CreateButton(&wContext, L"Key Bindings", L"Keyboard Bindings", 211, 55, 150, 23, hotkeyString);
    memset(hotkeyString, 0, sizeof(hotkeyString));

    CreateLabel(&wContext, L"Key Bindings", L"Keyboard Bindings", 49, 89, 79, 15, L"Capture Area:");
    HotkeyToPrettyString(&settings->screenSelectionKey, hotkeyString, sizeof(hotkeyString) / sizeof(TCHAR));
    CreateButton(&wContext, L"Key Bindings", L"Keyboard Bindings", 211, 84, 150, 23, hotkeyString);
    memset(hotkeyString, 0, sizeof(hotkeyString));

    CreateLabel(&wContext, L"Key Bindings", L"Keyboard Bindings", 49, 118, 69, 15, L"Upload File:");
    HotkeyToPrettyString(&settings->uploadFileKey, hotkeyString, sizeof(hotkeyString) / sizeof(TCHAR));
    CreateButton(&wContext, L"Key Bindings", L"Keyboard Bindings", 211, 113, 150, 23, hotkeyString);
    memset(hotkeyString, 0, sizeof(hotkeyString));

    CreateLabel(&wContext, L"Key Bindings", L"Keyboard Bindings", 66, 141, 361, 36, L"Use this shortcut in Windows Explorer to quickly upload selected files.");

    CreateLabel(&wContext, L"Key Bindings", L"Keyboard Bindings", 49, 171, 103, 15, L"Upload Clipboard:");
    HotkeyToPrettyString(&settings->uploadClipboardKey, hotkeyString, sizeof(hotkeyString) / sizeof(TCHAR));
    CreateButton(&wContext, L"Key Bindings", L"Keyboard Bindings", 211, 166, 150, 23, hotkeyString);
    memset(hotkeyString, 0, sizeof(hotkeyString));

    CreateLabel(&wContext, L"Key Bindings", L"Keyboard Bindings", 49, 200, 153, 15, L"Toggle puush functionality:");
    HotkeyToPrettyString(&settings->toggleKey, hotkeyString, sizeof(hotkeyString) / sizeof(TCHAR));
    CreateButton(&wContext, L"Key Bindings", L"Keyboard Bindings", 211, 195, 150, 23, hotkeyString);

    // Add elements to "Account Setup" GroupBox
    CreateLabel(&wContext, L"Account", L"Account Setup", 9, 19, 433, 40, L"You need to login before you can make full use of puush. If you don't already have an account, you can register for free via the link below.");
    CreateLabel(&wContext, L"Account", L"Account Setup", 97, 69, 39, 15, L"Email:");
    CreateTextbox(&wContext, L"Account", L"Account Setup", 138, 66, 149, 20, L"");
    CreateLabel(&wContext, L"Account", L"Account Setup", 76, 97, 60, 15, L"Password:");
    CreateTextbox(&wContext, L"Account", L"Account Setup", 138, 94, 149, 20, L"");
    CreateButton(&wContext, L"Account", L"Account Setup", 308, 66, 127, 48, L"Login");

    // Add elements to "Update Management" GroupBox
    CreateLabel(&wContext, L"Updates", L"Update Management", 32, 46, 80, 15, L"Last Checked:");
    CreateLabel(&wContext, L"Updates", L"Update Management", 114, 46, 38, 15, L"Never");
    CreateButton(&wContext, L"Updates", L"Update Management", 288, 22, 152, 54, L"Check for Updates");

    // Create radio buttons for "Screen Capture Quality" GroupBox
    CreateRadioButton(&wContext, L"Advanced", L"Screen Capture Quality", 17, 44, 273, 19, L"Smart (use JPG unless PNG is smaller in filesize)");
    CreateRadioButton(&wContext, L"Advanced", L"Screen Capture Quality", 17, 20, 187, 19, L"No Compression (always PNG)");

    // Create radio buttons for "Fullscreen Capture" GroupBox
    CreateRadioButton(&wContext, L"Advanced", L"Fullscreen Capture", 17, 19, 124, 19, L"Capture all screens");
    CreateRadioButton(&wContext, L"Advanced", L"Fullscreen Capture", 17, 38, 239, 19, L"Capture screen containing mouse cursor");
    CreateRadioButton(&wContext, L"Advanced", L"Fullscreen Capture", 17, 57, 186, 19, L"Always capture primary screen");

    // Add checkbox to "Context Menu" GroupBox
    CreateCheckbox(&wContext, L"Advanced", L"Context Menu", 16, 21, 203, 19, L"Show explorer context menu item");

    UpdateSettingsUI(settings);

    ShowWindow(hwnd, SW_SHOW);

    return hwnd;
}

void UpdateSettingsUI(PuushSettings* settings)
{
    HWND hCheckbox = FindControlByName(&wContext, L"Start puush on startup");
    if (hCheckbox != NULL) {
        SendMessage(hCheckbox, BM_SETCHECK, settings->startup, 0);
    }

    hCheckbox = FindControlByName(&wContext, L"Play a notification sound");
    if (hCheckbox != NULL) {
        SendMessage(hCheckbox, BM_SETCHECK, settings->notificationSound, 0);
    }

    hCheckbox = FindControlByName(&wContext, L"Copy link to clipboard");
    if (hCheckbox != NULL) {
        SendMessage(hCheckbox, BM_SETCHECK, settings->copyToClipboard, 0);
    }

    hCheckbox = FindControlByName(&wContext, L"Open link in browser");
    if (hCheckbox != NULL) {
        SendMessage(hCheckbox, BM_SETCHECK, settings->openBrowser, 0);
    }

    hCheckbox = FindControlByName(&wContext, L"Save a local copy of image");
    if (hCheckbox != NULL) {
        SendMessage(hCheckbox, BM_SETCHECK, settings->saveImages, 0);
    }
    
    hCheckbox = FindControlByName(&wContext, L"Show explorer context menu item");
    if (hCheckbox != NULL) {
        SendMessage(hCheckbox, BM_SETCHECK, settings->contextMenu, 0);
    }

    HWND hRadio = NULL;

    switch (settings->doubleClickBehaviour) {
    case OpenSettings:
        hRadio = FindControlByName(&wContext, L"Show settings dialog");
        break;
    case ScreenSelect:
        hRadio = FindControlByName(&wContext, L"Begin screen capture mode");
        break;
    case UploadFile:
        hRadio = FindControlByName(&wContext, L"Open upload file dialog");
        break;
    }

    if (hRadio != NULL) {
        SendMessage(hRadio, BM_SETCHECK, BST_CHECKED, 0);
    }

    switch (settings->fullscreenMode) {
    case AllScreens:
        hRadio = FindControlByName(&wContext, L"Capture all screens");
        break;
    case ScreenContainingMouseCursor:
        hRadio = FindControlByName(&wContext, L"Capture screen containing mouse cursor");
        break;
    case PrimaryScreen:
        hRadio = FindControlByName(&wContext, L"Always capture primary screen");
        break;
    }

    if (hRadio != NULL) {
        SendMessage(hRadio, BM_SETCHECK, BST_CHECKED, 0);
    }

    switch (settings->uploadQuality) {
    case Best:
        hRadio = FindControlByName(&wContext, L"No Compression (always PNG)");
        break;
    case High:
    case Medium:
        hRadio = FindControlByName(&wContext, L"Smart (use JPG unless PNG is smaller in filesize)");
        break;
    }

    if (hRadio != NULL) {
        SendMessage(hRadio, BM_SETCHECK, BST_CHECKED, 0);
    }
}

void StartKeyCapture(HWND button)
{
    isKeyCapturing = TRUE;
    hButtonCaptureKeys = button;
    memset(capturedKeys, 0, sizeof(capturedKeys));
    SetWindowText(hButtonCaptureKeys, L"Press some keys...");
}

void EndKeyCapture()
{
    if (!isKeyCapturing)
        return;

    isKeyCapturing = FALSE;
    Hotkey capturedHotkey;
    MakeHotkeyFromCapturedKeys(&capturedHotkey);
    UpdateKeyCaptureButton(hButtonCaptureKeys, capturedHotkey);
    hButtonCaptureKeys = NULL;
}

void UpdateKeyCaptureButton(HWND button, Hotkey key)
{
    TCHAR buttonText[256] = { 0 };
    HotkeyToPrettyString(&key, buttonText, sizeof(buttonText) / sizeof(TCHAR));
    SetWindowText(hButtonCaptureKeys, buttonText);
}

void MakeHotkeyFromCapturedKeys(Hotkey* hotkey)
{
    hotkey->keyCount = 0;

    for (int i = 0; i < 256; ++i)
    {
        if (capturedKeys[i] && hotkey->keyCount < 3)
        {
            hotkey->keys[hotkey->keyCount++] = i;
        }
    }
}