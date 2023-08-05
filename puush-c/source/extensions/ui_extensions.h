#pragma once

#include <Windows.h>
#include <CommCtrl.h>
#include <tchar.h>

#define TAB_CONTROL_BASE_ID 100
#define TAB_CONTROL_PAGE_BASE_ID 200
#define GROUP_BOX_BASE_ID 300
#define CONTROL_BASE_ID 400

#define MAX_NAME_LENGTH 256
#define MAX_CLASS_NAME_LENGTH 128
#define MAX_GROUP_BOX_NAME_LENGTH 128
#define MAX_TAB_NAME_LENGTH 128

#define MAX_CONTROLS 500

typedef struct ControlMapping {
    HWND hControl;
    UINT controlId;
    UINT groupBoxId;
    WCHAR controlName[MAX_NAME_LENGTH];
    WCHAR tabPageName[MAX_TAB_NAME_LENGTH];
    WCHAR groupBoxName[MAX_GROUP_BOX_NAME_LENGTH];
    WCHAR controlClassName[MAX_CLASS_NAME_LENGTH];
    WCHAR name[MAX_NAME_LENGTH]; // Add this line
} ControlMapping;

typedef struct WindowContext {
    ControlMapping controls[MAX_CONTROLS];
    UINT currentControlCount;
    HWND hWnd;
    WNDPROC lpfnWndProc;
} WindowContext;

ControlMapping* CreateAndAddControlMapping(WindowContext* pContext, HWND hControl, LPCWSTR controlName, LPCWSTR tabPageName, LPCWSTR groupBoxName);
int FindTabControlIndexByName(WindowContext* pContext, LPCWSTR tabName);
HWND FindGroupBoxHandle(WindowContext* pContext, LPCWSTR tabName, LPCWSTR groupBoxName);
HWND FindControlByName(WindowContext* pContext, LPCWSTR controlName);
HWND FindControlByNameInGroupBox(WindowContext* pContext, LPCWSTR tabName, LPCWSTR groupBoxName, LPCWSTR controlName);
HWND FindControlByNameAndTypeInGroupBox(WindowContext* pContext, LPCWSTR tabName, LPCWSTR groupBoxName, LPCWSTR controlType, LPCWSTR controlName);
HWND FindControlByNameAndType(WindowContext* pContext, LPCWSTR controlType, LPCWSTR controlName);
HWND FindControlHWNDByID(WindowContext* context, UINT controlID);
BOOL GetControlRect(WindowContext* pContext, LPCWSTR controlName, RECT* pRect);
void HandleTabControlTabChange(WindowContext* pContext);
void CreateTabControl(WindowContext* pContext, HINSTANCE hInstance, const LPCWSTR* tabNames);
void CreateAndAddGroupBoxesToTabPage(WindowContext* pContext, LPCWSTR tabPageName, const LPCWSTR* groupBoxNames);
HWND CreateAndAddControlToGroupBox(WindowContext* pContext, LPCWSTR tabName, LPCWSTR groupBoxName, LPCWSTR controlClassName, LPCWSTR controlText, DWORD controlStyle,
    int x, int y, int width, int height);
void GetScaledDimensions(WindowContext* pContext, int x, int y, int width, int height, POINT* scaledPoint, SIZE* scaledSize);
void CreateLabel(WindowContext* pContext, const LPCWSTR tabName, const LPCWSTR groupBoxName, int x, int y, int width, int height, WCHAR* text);
void CreateCheckbox(WindowContext* pContext, const LPCWSTR tabName, const LPCWSTR groupBoxName, int x, int y, int width, int height, WCHAR* text);
void CreateButton(WindowContext* pContext, const LPCWSTR tabName, const LPCWSTR groupBoxName, int x, int y, int width, int height, WCHAR* text);
void CreateRadioButton(WindowContext* pContext, const LPCWSTR tabName, const LPCWSTR groupBoxName, int x, int y, int width, int height, WCHAR* text, BOOL isFirstInGroup);
void CreateTextbox(WindowContext* pContext, const LPCWSTR tabName, const LPCWSTR groupBoxName, int x, int y, int width, int height, WCHAR* text);
void CreateLinkLabel(WindowContext* pContext, const LPCWSTR tabName, const LPCWSTR groupBoxName, int x, int y, int width, int height, WCHAR* text);

void AddTrayIcon(HWND hwnd, UINT ID, UINT iconID, LPCWSTR szTip, UINT uCallbackMessage);

void ShowBalloonTip(HWND hWnd, UINT uID, LPCWSTR szTitle, LPCWSTR szText, DWORD dwInfoFlags, UINT uTimeout);
void ShowInfoBalloonTip(HWND hWnd, UINT uID, LPCWSTR szTitle, LPCWSTR szText, UINT uTimeout);
void ShowWarningBalloonTip(HWND hWnd, UINT uID, LPCWSTR szTitle, LPCWSTR szText, UINT uTimeout);
void ShowErrorBalloonTip(HWND hWnd, UINT uID, LPCWSTR szTitle, LPCWSTR szText, UINT uTimeout);