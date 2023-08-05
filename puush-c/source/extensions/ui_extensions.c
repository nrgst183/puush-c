#include "ui_extensions.h"

ControlMapping* CreateAndAddControlMapping(WindowContext* pContext, HWND hControl, LPCWSTR controlName, LPCWSTR tabName, LPCWSTR groupBoxName) {
    ControlMapping* newControlMapping = &pContext->controls[pContext->currentControlCount];
    newControlMapping->hControl = hControl;
    newControlMapping->controlId = CONTROL_BASE_ID + pContext->currentControlCount;

    wcsncpy_s(newControlMapping->controlName, MAX_NAME_LENGTH, controlName, _TRUNCATE); // Use wcsncpy_s
    wcsncpy_s(newControlMapping->tabPageName, MAX_TAB_NAME_LENGTH, tabName, _TRUNCATE); // Use wcsncpy_s
    wcsncpy_s(newControlMapping->groupBoxName, MAX_GROUP_BOX_NAME_LENGTH, groupBoxName, _TRUNCATE); // Use wcsncpy_s

    // Get and store the class name
    WCHAR className[MAX_CLASS_NAME_LENGTH];
    GetClassName(hControl, className, MAX_CLASS_NAME_LENGTH);
    wcsncpy_s(newControlMapping->controlClassName, MAX_CLASS_NAME_LENGTH, className, _TRUNCATE);

    pContext->currentControlCount++;

    return newControlMapping;
}

int FindTabControlIndexByName(WindowContext* pContext, LPCWSTR tabName) {
    HWND hTab = GetDlgItem(pContext->hWnd, TAB_CONTROL_BASE_ID);
    if (hTab == NULL) {
        return -1; // Tab control not found
    }

    // Get the count of tabs in the tab control
    int tabCount = TabCtrl_GetItemCount(hTab);

    for (int i = 0; i < tabCount; i++) {
        WCHAR currentTabName[MAX_NAME_LENGTH];
        TCITEM tie;
        tie.mask = TCIF_TEXT;
        tie.pszText = currentTabName;
        tie.cchTextMax = sizeof(currentTabName) / sizeof(WCHAR);
        if (TabCtrl_GetItem(hTab, i, &tie) && wcscmp(currentTabName, tabName) == 0) {
            return i; // Found the tab index
        }
    }

    return -1; // Tab not found
}

HWND FindGroupBoxHandle(WindowContext* pContext, LPCWSTR tabName, LPCWSTR groupBoxName) {
    for (UINT i = 0; i < pContext->currentControlCount; i++) {
        if (wcscmp(pContext->controls[i].tabPageName, tabName) == 0 &&
            wcscmp(pContext->controls[i].groupBoxName, groupBoxName) == 0) {
            return pContext->controls[i].hControl; // Found the group box handle
        }
    }
    return NULL; // Group box not found
}

HWND FindControlByName(WindowContext* pContext, LPCWSTR controlName) {
    for (UINT i = 0; i < pContext->currentControlCount; i++) {
        if (wcscmp(pContext->controls[i].controlName, controlName) == 0) {
            // Found the control
            return pContext->controls[i].hControl;
        }
    }
    return NULL; // Control not found
}

HWND FindControlByNameInGroupBox(WindowContext* pContext, LPCWSTR tabName, LPCWSTR groupBoxName, LPCWSTR controlName) {
    for (UINT i = 0; i < pContext->currentControlCount; i++) {
        if (wcscmp(pContext->controls[i].controlName, controlName) == 0 &&
            wcscmp(pContext->controls[i].tabPageName, tabName) == 0 &&
            wcscmp(pContext->controls[i].groupBoxName, groupBoxName) == 0) {
            // Found the control
            return pContext->controls[i].hControl;
        }
    }
    return NULL; // Control not found
}

HWND FindControlByNameAndType(WindowContext* pContext, LPCWSTR controlType, LPCWSTR controlName) {
    for (UINT i = 0; i < pContext->currentControlCount; i++) {
        if (wcscmp(pContext->controls[i].controlName, controlName) == 0 &&
            wcscmp(pContext->controls[i].controlClassName, controlType) == 0) {
            // Found the control
            return pContext->controls[i].hControl;
        }
    }
    return NULL; // Control not found
}

HWND FindControlByNameAndTypeInGroupBox(WindowContext* pContext, LPCWSTR tabName, LPCWSTR groupBoxName, LPCWSTR controlType, LPCWSTR controlName) {
    for (UINT i = 0; i < pContext->currentControlCount; i++) {
        if (wcscmp(pContext->controls[i].controlName, controlName) == 0 &&
            wcscmp(pContext->controls[i].controlClassName, controlType) == 0 &&
            wcscmp(pContext->controls[i].tabPageName, tabName) == 0 &&
            wcscmp(pContext->controls[i].groupBoxName, groupBoxName) == 0) {
            // Found the control
            return pContext->controls[i].hControl;
        }
    }
    return NULL; // Control not found
}

HWND FindControlHWNDByID(WindowContext* context, UINT controlID) {
    for (UINT i = 0; i < context->currentControlCount; i++) {
        if (context->controls[i].controlId == controlID) {
            return context->controls[i].hControl;
        }
    }
    return NULL; // Control not found
}

BOOL GetTabNameByIndex(WindowContext* pContext, int tabIndex, WCHAR* tabName, int tabNameMaxLength) {
    HWND hTab = GetDlgItem(pContext->hWnd, TAB_CONTROL_BASE_ID);
    int tabCount = TabCtrl_GetItemCount(hTab);

    // Ignore invalid tab indices
    if (tabIndex < 0 || tabIndex >= tabCount) {
        return FALSE;
    }

    // Get the name of the tab
    TCITEM tie;
    tie.mask = TCIF_TEXT;
    tie.pszText = tabName;
    tie.cchTextMax = tabNameMaxLength;

    if (TabCtrl_GetItem(hTab, tabIndex, &tie) == FALSE) {
        return FALSE; // Failed to get tab item
    }

    return TRUE;
}

BOOL GetControlRect(WindowContext* pContext, LPCWSTR controlName, RECT* pRect) {
    // Get the handle to the control
    HWND hwndControl = FindControlByName(pContext, controlName);
    if (hwndControl == NULL) {
        return FALSE;
    }

    // Get the rectangle of the control in screen coordinates
    if (!GetWindowRect(hwndControl, pRect)) {
        return FALSE;
    }

    // Convert the screen coordinates to client coordinates relative to the settings window
    POINT pt = { pRect->left, pRect->top };
    if (!ScreenToClient(pContext->hWnd, &pt)) {
        return FALSE;
    }

    pRect->left = pt.x;
    pRect->top = pt.y;

    pt.x = pRect->right;
    pt.y = pRect->bottom;
    if (!ScreenToClient(pContext->hWnd, &pt)) {
        return FALSE;
    }

    pRect->right = pt.x;
    pRect->bottom = pt.y;

    return TRUE;
}

void HandleTabControlTabChange(WindowContext* pContext) {
    HWND hTab = GetDlgItem(pContext->hWnd, TAB_CONTROL_BASE_ID);
    if (hTab == NULL) {
        return; // Failed to get the tab control
    }

    int iPage = TabCtrl_GetCurSel(hTab);
    if (iPage == -1) {
        return; // Failed to get the current tab index
    }

    WCHAR tabName[MAX_TAB_NAME_LENGTH];
    if (!GetTabNameByIndex(pContext, iPage, tabName, MAX_TAB_NAME_LENGTH)) {
        return; // Failed to get the current tab name
    }

    for (UINT i = 0; i < pContext->currentControlCount; i++) {
        if (wcscmp(pContext->controls[i].tabPageName, tabName) == 0) {
            // The control belongs to the current tab, so it should be visible
            ShowWindow(pContext->controls[i].hControl, SW_SHOW);
        }
        else {
            // The control does not belong to the current tab, so it should be hidden
            ShowWindow(pContext->controls[i].hControl, SW_HIDE);
        }
    }
}

void CreateTabControl(WindowContext* pContext, HINSTANCE hInstance, const LPCWSTR* tabNames) {
    HWND hWnd = pContext->hWnd;
    // Get the client area of the parent window
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);

    // Create the tab control scaled to the size of the parent window
    HWND hTab = CreateWindowEx(0, WC_TABCONTROL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
        0, 0, clientRect.right, clientRect.bottom,
        hWnd, (HMENU)TAB_CONTROL_BASE_ID, hInstance, NULL);

    // Add the tabs
    int tabIndex = 0;

    WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
    wcex.lpfnWndProc = pContext->lpfnWndProc;
    wcex.hInstance = hInstance;
    wcex.lpszClassName = L"TabPageClass";
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);

    RegisterClassEx(&wcex);

    while (tabNames[tabIndex] != NULL) {
        TCITEM tie = { TCIF_TEXT, 0, 0, (LPTSTR)tabNames[tabIndex], 0, 0, 0 };
        TabCtrl_InsertItem(hTab, tabIndex, &tie);

        // Create a child window for each tab page
        DWORD dwStyle = WS_CHILD | WS_CLIPSIBLINGS;
        if (tabIndex == 0) {
            dwStyle |= WS_VISIBLE; // Make the first tab page visible
        }
        CreateWindowEx(0, wcex.lpszClassName, NULL, dwStyle, 0, 30, clientRect.right, clientRect.bottom - 30, hTab, (HMENU)(TAB_CONTROL_PAGE_BASE_ID + tabIndex), GetModuleHandle(NULL), NULL);

        tabIndex++;
    }
}

void CreateAndAddGroupBoxesToTabPage(WindowContext* pContext, LPCWSTR tabPageName, const LPCWSTR* groupBoxNames) {
    HWND hWnd = pContext->hWnd;

    // Get the tab control handle
    HWND hTab = GetDlgItem(hWnd, TAB_CONTROL_BASE_ID);

    // Find the tab index
    int tabPageIndex = FindTabControlIndexByName(pContext, tabPageName);
    if (tabPageIndex == -1) {
        return; // Tab not found
    }

    // Get the tab control item rect to determine the height of the tabs
    RECT itemRect;
    TabCtrl_GetItemRect(hTab, 0, &itemRect);
    int tabHeight = itemRect.bottom - itemRect.top;

    // Calculate the available dimensions for the group boxes using the window rect
    RECT tabRect;
    GetWindowRect(hTab, &tabRect);
    ScreenToClient(hWnd, (POINT*)&tabRect.left); // Convert to client coordinates
    ScreenToClient(hWnd, (POINT*)&tabRect.right); // Convert to client coordinates
    int availableHeight = tabRect.bottom - tabRect.top - tabHeight; // subtract tabHeight
    int availableWidth = tabRect.right - tabRect.left;

    // Calculate the height for each group box
    int numGroupBoxes = 0;
    while (groupBoxNames[numGroupBoxes] != NULL) {
        numGroupBoxes++;
    }
    int groupBoxHeight = (availableHeight - 40) / numGroupBoxes;

    // Create the group boxes with evenly divided height and full width
    int groupBoxIndex = 0;
    int groupBoxTop = 0;

    BOOL initiallyVisible = (FindTabControlIndexByName(pContext, tabPageName) == 0);

    // Create the control
    DWORD controlStyle = WS_CHILD | BS_GROUPBOX;
    if (initiallyVisible) {
        controlStyle |= WS_VISIBLE;
    }

    while (groupBoxNames[groupBoxIndex] != NULL) {
        groupBoxTop = groupBoxHeight * groupBoxIndex + tabHeight + 15; // add tabHeight

        // Create the group box control
        HWND hGroupBox = CreateWindowEx(0, _T("BUTTON"), groupBoxNames[groupBoxIndex], controlStyle,
            5, groupBoxTop, availableWidth - 10, groupBoxHeight - 5, hWnd, (HMENU)(GROUP_BOX_BASE_ID + pContext->currentControlCount), GetModuleHandle(NULL), NULL);


        // Store the group box immediately after creating it
        CreateAndAddControlMapping(pContext, hGroupBox, groupBoxNames[groupBoxIndex], tabPageName, groupBoxNames[groupBoxIndex]);

        groupBoxIndex++;
    }
}

HWND CreateAndAddControlToGroupBox(WindowContext* pContext, LPCWSTR tabName, LPCWSTR groupBoxName, LPCWSTR controlClassName, LPCWSTR controlText, DWORD controlStyle,
    int x, int y, int width, int height) {

    // Find the tab index
    int tabIndex = FindTabControlIndexByName(pContext, tabName);
    if (tabIndex == -1) {
        return NULL; // Tab not found
    }

    // Find the group box handle
    HWND hGroupBox = FindGroupBoxHandle(pContext, tabName, groupBoxName);
    if (hGroupBox == NULL) {
        return NULL; // Group box not found
    }

    // Get the rectangle of the group box relative to the settings window
    RECT rectGroupBox;
    if (!GetControlRect(pContext, groupBoxName, &rectGroupBox)) {
        return NULL; // Could not get rectangle
    }

    // Adjust the x and y coordinates of the control to be relative to the settings window
    x += rectGroupBox.left;
    y += rectGroupBox.top;

    BOOL initiallyVisible = tabIndex == 0;

    // Create the control
    DWORD finalControlStyle = controlStyle | WS_CHILD;
    if (initiallyVisible) {
        finalControlStyle |= WS_VISIBLE;
    }

    // Create the control with the settings window as the parent
    HWND hControl = CreateWindowEx(0, controlClassName, controlText, finalControlStyle,
        x, y, width, height, pContext->hWnd, (HMENU)(CONTROL_BASE_ID + pContext->currentControlCount), GetModuleHandle(NULL), NULL);

    SetWindowPos(hControl, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    // Add the control mapping
    CreateAndAddControlMapping(pContext, hControl, controlText, tabName, groupBoxName);

    return hControl;
}

void GetScaledDimensions(WindowContext* pContext, int x, int y, int width, int height, POINT* scaledPoint, SIZE* scaledSize) {
    HWND hParent = pContext->hWnd;
    HDC hDC = GetDC(hParent);
    int dpiX = GetDeviceCaps(hDC, LOGPIXELSX);
    int dpiY = GetDeviceCaps(hDC, LOGPIXELSY);
    ReleaseDC(hParent, hDC);

    scaledPoint->x = MulDiv(x, dpiX, 96);
    scaledPoint->y = MulDiv(y, dpiY, 96);
    scaledSize->cx = MulDiv(width, dpiX, 96);
    scaledSize->cy = MulDiv(height, dpiY, 96);
}

void CreateLabel(WindowContext* pContext, const LPCWSTR tabName, const LPCWSTR groupBoxName, int x, int y, int width, int height, WCHAR* text)
{
    POINT scaledPoint;
    SIZE scaledSize;
    GetScaledDimensions(pContext, x, y, width, height, &scaledPoint, &scaledSize);
    CreateAndAddControlToGroupBox(pContext, tabName, groupBoxName, TEXT("STATIC"), text, WS_CHILD, scaledPoint.x, scaledPoint.y, scaledSize.cx, scaledSize.cy);
}

void CreateCheckbox(WindowContext* pContext, const LPCWSTR tabName, const LPCWSTR groupBoxName, int x, int y, int width, int height, WCHAR* text)
{
    POINT scaledPoint;
    SIZE scaledSize;
    GetScaledDimensions(pContext, x, y, width, height, &scaledPoint, &scaledSize);
    CreateAndAddControlToGroupBox(pContext, tabName, groupBoxName, TEXT("BUTTON"), text, BS_AUTOCHECKBOX, scaledPoint.x, scaledPoint.y, scaledSize.cx, scaledSize.cy);
}

void CreateButton(WindowContext* pContext, const LPCWSTR tabName, const LPCWSTR groupBoxName, int x, int y, int width, int height, WCHAR* text)
{
    POINT scaledPoint;
    SIZE scaledSize;
    GetScaledDimensions(pContext, x, y, width, height, &scaledPoint, &scaledSize);
    CreateAndAddControlToGroupBox(pContext, tabName, groupBoxName, TEXT("BUTTON"), text, BS_PUSHBUTTON, scaledPoint.x, scaledPoint.y, scaledSize.cx, scaledSize.cy);
}

void CreateRadioButton(WindowContext* pContext, const LPCWSTR tabName, const LPCWSTR groupBoxName, int x, int y, int width, int height, WCHAR* text, BOOL isFirstInGroup)
{
    POINT scaledPoint;
    SIZE scaledSize;

    DWORD style = BS_AUTORADIOBUTTON;
    if (isFirstInGroup) {
        style |= WS_GROUP;
    }

    GetScaledDimensions(pContext, x, y, width, height, &scaledPoint, &scaledSize);

    CreateAndAddControlToGroupBox(pContext, tabName, groupBoxName, TEXT("BUTTON"), text, style, scaledPoint.x, scaledPoint.y, scaledSize.cx, scaledSize.cy);
}

void CreateTextbox(WindowContext* pContext, const LPCWSTR tabName, const LPCWSTR groupBoxName, int x, int y, int width, int height, WCHAR* text)
{
    POINT scaledPoint;
    SIZE scaledSize;
    GetScaledDimensions(pContext, x, y, width, height, &scaledPoint, &scaledSize);
    CreateAndAddControlToGroupBox(pContext, tabName, groupBoxName, TEXT("EDIT"), text, WS_BORDER | ES_AUTOHSCROLL, scaledPoint.x, scaledPoint.y, scaledSize.cx, scaledSize.cy);
}

void CreateLinkLabel(WindowContext* pContext, const LPCWSTR tabName, const LPCWSTR groupBoxName, int x, int y, int width, int height, WCHAR* text)
{
    POINT scaledPoint;
    SIZE scaledSize;
    GetScaledDimensions(pContext, x, y, width, height, &scaledPoint, &scaledSize);
    CreateAndAddControlToGroupBox(pContext, tabName, groupBoxName, TEXT("SYSLINK"), text, WS_CHILD, scaledPoint.x, scaledPoint.y, scaledSize.cx, scaledSize.cy);

    // Add a notification handler for the link label if needed
    // Example: 
    // SetWindowSubclass(hLinkLabel, LinkLabelProc, 0, 0);
}

void AddTrayIcon(HWND hwnd, UINT uID, UINT iconID, LPCWSTR szTip, UINT uCallbackMessage)
{
    NOTIFYICONDATA nid = { 0 };
    nid.cbSize = NOTIFYICONDATA_V3_SIZE; // use legacy size
    nid.hWnd = hwnd;
    nid.uID = uID;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = uCallbackMessage;
    nid.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(iconID));
    _tcscpy_s(nid.szTip, ARRAYSIZE(nid.szTip), szTip);

    Shell_NotifyIcon(NIM_ADD, &nid);
}

void SetTrayTooltip(HWND hwnd, LPCWSTR szTip)
{
    NOTIFYICONDATA nid = { sizeof(NOTIFYICONDATA) };
    nid.hWnd = hwnd;
    nid.uFlags = NIF_TIP;
    _tcscpy_s(nid.szTip, ARRAYSIZE(nid.szTip), szTip);
    Shell_NotifyIcon(NIM_MODIFY, &nid);
}

void ShowBalloonTip(HWND hWnd, UINT uID, LPCWSTR szTitle, LPCWSTR szText, DWORD dwInfoFlags, UINT uTimeout) {
    NOTIFYICONDATA nid = { sizeof(NOTIFYICONDATA) };
    nid.hWnd = hWnd;
    nid.uID = uID;  // Set the uID
    nid.uVersion = NOTIFYICON_VERSION_4;  // Set the version
    nid.uFlags = NIF_INFO;
    nid.uTimeout = uTimeout;
    nid.dwInfoFlags = dwInfoFlags;
    _tcscpy_s(nid.szInfoTitle, ARRAYSIZE(nid.szInfoTitle), szTitle);
    _tcscpy_s(nid.szInfo, ARRAYSIZE(nid.szInfo), szText);

    Shell_NotifyIcon(NIM_MODIFY, &nid);
}

void ShowInfoBalloonTip(HWND hWnd, UINT uID, LPCWSTR szTitle, LPCWSTR szText, UINT uTimeout)
{
    ShowBalloonTip(hWnd, uID, szTitle, szText, NIIF_INFO, uTimeout);
}

void ShowWarningBalloonTip(HWND hWnd, UINT uID, LPCWSTR szTitle, LPCWSTR szText, UINT uTimeout)
{
    ShowBalloonTip(hWnd, uID, szTitle, szText, NIIF_WARNING, uTimeout);
}

void ShowErrorBalloonTip(HWND hWnd, UINT uID, LPCWSTR szTitle, LPCWSTR szText, UINT uTimeout)
{
    ShowBalloonTip(hWnd, uID, szTitle, szText, NIIF_ERROR, uTimeout);
}
