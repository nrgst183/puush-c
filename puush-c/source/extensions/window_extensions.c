#include "window_extensions.h"

int FindTabControlIndexByName(HWND hTab, const LPWSTR tabName) {
    int tabCount = TabCtrl_GetItemCount(hTab);
    for (int i = 0; i < tabCount; i++) {
        TCHAR tabText[256] = { 0 }; // Initialize the string with zeros
        TCITEM tie = { TCIF_TEXT, 0, 0, tabText, sizeof(tabText) / sizeof(TCHAR), 0, 0 };
        TabCtrl_GetItem(hTab, i, &tie);
        if (lstrcmp(tabText, tabName) == 0) {
            return i; // Found the tab index
        }
    }
    return -1; // Tab not found
}

HWND FindGroupBoxHandle(HWND hTab, int tabIndex, const LPWSTR groupBoxName) {
    HWND hTabPage = GetDlgItem(hTab, 200 + tabIndex);

    // Enumerate through child windows of the tab page
    HWND hChild = GetWindow(hTabPage, GW_CHILD);
    while (hChild != NULL) {
        TCHAR className[256];
        GetClassName(hChild, className, ARRAYSIZE(className));
        if (lstrcmp(className, L"Button") == 0) { // Group boxes are of class "Button"
            TCHAR groupBoxText[256];
            GetWindowText(hChild, groupBoxText, ARRAYSIZE(groupBoxText));
            if (lstrcmp(groupBoxText, groupBoxName) == 0) {
                return hChild; // Found the group box handle
            }
        }
        hChild = GetWindow(hChild, GW_HWNDNEXT);
    }

    return NULL; // Group box not found
}

HWND FindControlByName(HWND hParent, const LPWSTR controlName) {
    HWND hChild = GetWindow(hParent, GW_CHILD);
    while (hChild != NULL) {
        WCHAR childText[256];
        GetWindowText(hChild, childText, ARRAYSIZE(childText));
        if (lstrcmp(childText, controlName) == 0) {
            return hChild; // Found the control handle
        }
        hChild = GetWindow(hChild, GW_HWNDNEXT);
    }
    return NULL; // Control not found
}

void HandleTabSelectionChange(HWND hwnd, int tabPageCount) {
    HWND hTab = GetDlgItem(hwnd, 100); // Assuming tab control has ID 100
    int selectedPageIndex = TabCtrl_GetCurSel(hTab);

    // Ignore invalid tab indices
    if (selectedPageIndex < 0 || selectedPageIndex >= tabPageCount) {
        return;
    }

    for (int i = 0; i < tabPageCount; i++) {
        HWND hTabPage = GetDlgItem(hTab, 200 + i);
        if (hTabPage != NULL) {
            if (i == selectedPageIndex) {
                ShowWindow(hTabPage, SW_SHOW);
            }
            else {
                ShowWindow(hTabPage, SW_HIDE);
            }
        }
    }
}

void CreateTabControl(HWND hWnd, HINSTANCE hInstance, const LPWSTR* tabNames) {
    // Get the client area of the parent window
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);

    // Create the tab control scaled to the size of the parent window
    HWND hTab = CreateWindowEx(0, WC_TABCONTROL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
        0, 0, clientRect.right, clientRect.bottom,
        hWnd, (HMENU)100, hInstance, NULL);

    // Add the tabs
    int tabIndex = 0;
    while (tabNames[tabIndex] != NULL) {
        TCITEM tie = { TCIF_TEXT, 0, 0, tabNames[tabIndex], 0, 0, 0 };
        TabCtrl_InsertItem(hTab, tabIndex, &tie);

        // Create a child window for each tab page
        DWORD dwStyle = WS_CHILD | WS_CLIPSIBLINGS;
        if (tabIndex == 0) {
            dwStyle |= WS_VISIBLE; // Make the first tab page visible
        }
        HWND hTabPage = CreateWindowEx(0, WC_STATIC, NULL, dwStyle, 0, 30, clientRect.right, clientRect.bottom - 30, hTab, (HMENU)(200 + tabIndex), GetModuleHandle(NULL), NULL);

        tabIndex++;
    }
}

void CreateAndAddGroupBoxesToTabPage(HWND hWnd, const LPWSTR tabPageName, const LPWSTR* groupBoxNames) {
    // Get the tab control handle
    HWND hTab = GetDlgItem(hWnd, 100);

    // Find the tab index
    int tabPageIndex = FindTabControlIndexByName(hTab, tabPageName);
    if (tabPageIndex == -1) {
        return; // Tab not found
    }

    // Get the tab page handle
    HWND hTabPage = GetDlgItem(hTab, 200 + tabPageIndex);
    if (!hTabPage) {
        return; // Tab page not found
    }

    // Calculate the available dimensions for the group boxes
    RECT tabRect;
    GetClientRect(hTabPage, &tabRect);
    int availableHeight = tabRect.bottom - tabRect.top;
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
    while (groupBoxNames[groupBoxIndex] != NULL) {
        groupBoxTop = groupBoxHeight * groupBoxIndex;

        // Add group box to the tab page
        CreateWindowEx(0, WC_BUTTON, groupBoxNames[groupBoxIndex], WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
            5, groupBoxTop + 5, availableWidth - 10, groupBoxHeight - 5, hTabPage, NULL, GetModuleHandle(NULL), NULL);

        groupBoxIndex++;
    }
}

void CreateAndAddControlToGroupBox(HWND hWnd, const LPWSTR tabName, const LPWSTR groupBoxName, const LPWSTR controlClassName, const LPWSTR controlText, DWORD controlStyle,
    int x, int y, int width, int height) {
    // Get the tab control handle
    HWND hTab = GetDlgItem(hWnd, 100);

    // Find the tab index
    int tabIndex = FindTabControlIndexByName(hTab, tabName);
    if (tabIndex == -1) {
        return; // Tab not found
    }

    // Find the group box handle
    HWND hGroupBox = FindGroupBoxHandle(hTab, tabIndex, groupBoxName);
    if (hGroupBox == NULL) {
        return; // Group box not found
    }

    // Create the control
    HWND hControl = CreateWindowEx(0, controlClassName, controlText, controlStyle | WS_CHILD | WS_VISIBLE,
        x, y, width, height, hGroupBox, NULL, GetModuleHandle(NULL), NULL);
}

void GetScaledDimensions(HWND hParent, int x, int y, int width, int height, POINT* scaledPoint, SIZE* scaledSize) {
    HDC hDC = GetDC(hParent);
    int dpiX = GetDeviceCaps(hDC, LOGPIXELSX);
    int dpiY = GetDeviceCaps(hDC, LOGPIXELSY);
    ReleaseDC(hParent, hDC);

    scaledPoint->x = MulDiv(x, dpiX, 96);
    scaledPoint->y = MulDiv(y, dpiY, 96);
    scaledSize->cx = MulDiv(width, dpiX, 96);
    scaledSize->cy = MulDiv(height, dpiY, 96);
}

void CreateLabel(HWND hParent, HINSTANCE hInstance, const LPWSTR tabName, const LPWSTR groupBoxName, int x, int y, int width, int height, TCHAR* text)
{
    POINT scaledPoint;
    SIZE scaledSize;
    GetScaledDimensions(hParent, x, y, width, height, &scaledPoint, &scaledSize);
    CreateAndAddControlToGroupBox(hParent, tabName, groupBoxName, TEXT("STATIC"), text, WS_CHILD | WS_VISIBLE, scaledPoint.x, scaledPoint.y, scaledSize.cx, scaledSize.cy);
}

void CreateCheckbox(HWND hParent, HINSTANCE hInstance, const LPWSTR tabName, const LPWSTR groupBoxName, int x, int y, int width, int height, TCHAR* text)
{
    POINT scaledPoint;
    SIZE scaledSize;
    GetScaledDimensions(hParent, x, y, width, height, &scaledPoint, &scaledSize);
    CreateAndAddControlToGroupBox(hParent, tabName, groupBoxName, TEXT("BUTTON"), text, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, scaledPoint.x, scaledPoint.y, scaledSize.cx, scaledSize.cy);
}

void CreateButton(HWND hParent, HINSTANCE hInstance, const LPWSTR tabName, const LPWSTR groupBoxName, int x, int y, int width, int height, TCHAR* text)
{
    POINT scaledPoint;
    SIZE scaledSize;
    GetScaledDimensions(hParent, x, y, width, height, &scaledPoint, &scaledSize);
    CreateAndAddControlToGroupBox(hParent, tabName, groupBoxName, TEXT("BUTTON"), text, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, scaledPoint.x, scaledPoint.y, scaledSize.cx, scaledSize.cy);
}

void CreateRadioButton(HWND hParent, HINSTANCE hInstance, const LPWSTR tabName, const LPWSTR groupBoxName, int x, int y, int width, int height, TCHAR* text)
{
    POINT scaledPoint;
    SIZE scaledSize;
    GetScaledDimensions(hParent, x, y, width, height, &scaledPoint, &scaledSize);
    CreateAndAddControlToGroupBox(hParent, tabName, groupBoxName, TEXT("BUTTON"), text, WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, scaledPoint.x, scaledPoint.y, scaledSize.cx, scaledSize.cy);
}

void CreateTextbox(HWND hParent, HINSTANCE hInstance, const LPWSTR tabName, const LPWSTR groupBoxName, int x, int y, int width, int height, TCHAR* text)
{
    POINT scaledPoint;
    SIZE scaledSize;
    GetScaledDimensions(hParent, x, y, width, height, &scaledPoint, &scaledSize);
    CreateAndAddControlToGroupBox(hParent, tabName, groupBoxName, TEXT("EDIT"), text, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, scaledPoint.x, scaledPoint.y, scaledSize.cx, scaledSize.cy);
}

void CreateLinkLabel(HWND hParent, HINSTANCE hInstance, const LPWSTR tabName, const LPWSTR groupBoxName, int x, int y, int width, int height, TCHAR* text)
{
    POINT scaledPoint;
    SIZE scaledSize;
    GetScaledDimensions(hParent, x, y, width, height, &scaledPoint, &scaledSize);
    CreateAndAddControlToGroupBox(hParent, tabName, groupBoxName, TEXT("SYSLINK"), text, WS_CHILD | WS_VISIBLE, scaledPoint.x, scaledPoint.y, scaledSize.cx, scaledSize.cy);

    // Add a notification handler for the link label if needed
    // Example: 
    // SetWindowSubclass(hLinkLabel, LinkLabelProc, 0, 0);
}
