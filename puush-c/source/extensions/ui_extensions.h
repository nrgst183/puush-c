#pragma once

#include <Windows.h>
#include <CommCtrl.h>
#include <tchar.h>

#define TAB_CONTROL_BASE_ID 100
#define TAB_CONTROL_PAGE_BASE_ID 200
#define CONTROL_BASE_ID 300

#define MAX_NAME_LENGTH 256
#define MAX_TAB_NAME_LENGTH 128

#define MAX_CONTROLS 256

typedef struct ControlMapping {
    HWND hControl;
    UINT controlId;
    TCHAR controlName[MAX_NAME_LENGTH];
    TCHAR tabPageName[MAX_TAB_NAME_LENGTH];
    TCHAR groupBoxName[MAX_NAME_LENGTH];
} ControlMapping;

typedef struct WindowContext {
    ControlMapping controls[MAX_CONTROLS];
    UINT currentControlCount;
    HWND hWnd;
    WNDPROC lpfnWndProc; // Callback procedure for the window
} WindowContext;

ControlMapping* CreateAndAddControlMapping(WindowContext* pContext, HWND hControl, LPCTSTR controlName, LPCTSTR tabPageName, LPCTSTR groupBoxName);
int FindTabControlIndexByName(WindowContext* pContext, LPCTSTR tabName);
HWND FindGroupBoxHandle(WindowContext* pContext, LPCTSTR tabName, LPCTSTR groupBoxName);
HWND FindControlByName(WindowContext* pContext, LPCTSTR controlName);
HWND FindControlHWNDByID(WindowContext* context, UINT controlID);
BOOL GetControlRect(WindowContext* pContext, LPCTSTR controlName, RECT* pRect);
void HandleTabControlTabChange(WindowContext* pContext);
void CreateTabControl(WindowContext* pContext, HINSTANCE hInstance, const LPCTSTR* tabNames);
void CreateAndAddGroupBoxesToTabPage(WindowContext* pContext, LPCTSTR tabPageName, const LPCTSTR* groupBoxNames);
HWND CreateAndAddControlToGroupBox(WindowContext* pContext, LPCTSTR tabName, LPCTSTR groupBoxName, LPCTSTR controlClassName, LPCTSTR controlText, DWORD controlStyle,
    int x, int y, int width, int height);
void GetScaledDimensions(WindowContext* pContext, int x, int y, int width, int height, POINT* scaledPoint, SIZE* scaledSize);
void CreateLabel(WindowContext* pContext, const LPCTSTR tabName, const LPCTSTR groupBoxName, int x, int y, int width, int height, TCHAR* text);
void CreateCheckbox(WindowContext* pContext, const LPCTSTR tabName, const LPCTSTR groupBoxName, int x, int y, int width, int height, TCHAR* text);
void CreateButton(WindowContext* pContext, const LPCTSTR tabName, const LPCTSTR groupBoxName, int x, int y, int width, int height, TCHAR* text);
void CreateRadioButton(WindowContext* pContext, const LPCTSTR tabName, const LPCTSTR groupBoxName, int x, int y, int width, int height, TCHAR* text);
void CreateTextbox(WindowContext* pContext, const LPCTSTR tabName, const LPCTSTR groupBoxName, int x, int y, int width, int height, TCHAR* text);
void CreateLinkLabel(WindowContext* pContext, const LPCTSTR tabName, const LPCTSTR groupBoxName, int x, int y, int width, int height, TCHAR* text);

void AddTrayIcon(HWND hwnd, UINT ID, UINT iconID, LPCTSTR szTip, UINT uCallbackMessage);

void ShowBalloonTip(HWND hWnd, LPCTSTR szTitle, LPCTSTR szText, DWORD dwInfoFlags, UINT uTimeout);
void ShowInfoBalloonTip(HWND hWnd, LPCTSTR szTitle, LPCTSTR szText, UINT uTimeout);
void ShowWarningBalloonTip(HWND hWnd, LPCTSTR szTitle, LPCTSTR szText, UINT uTimeout);
void ShowErrorBalloonTip(HWND hWnd, LPCTSTR szTitle, LPCTSTR szText, UINT uTimeout);