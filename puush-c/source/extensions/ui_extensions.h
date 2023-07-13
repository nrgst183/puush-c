#pragma once

#include <Windows.h>
#include <CommCtrl.h>
#include <tchar.h>

int FindTabControlIndexByName(HWND hTab, const LPCTSTR tabName);
HWND FindGroupBoxHandle(HWND hTab, int tabIndex, const LPCTSTR groupBoxName);
void HandleTabSelectionChange(HWND hwnd, int tabPageCount);
void CreateTabControl(HWND hWnd, HINSTANCE hInstance, const LPCTSTR* tabNames);
void CreateAndAddGroupBoxesToTabPage(HWND hWnd, const LPCTSTR tabPageName, const LPCTSTR* groupBoxNames);
void CreateAndAddControlToGroupBox(HWND hWnd, const LPCTSTR tabName, const LPCTSTR groupBoxName, const LPCTSTR controlClassName, const LPCTSTR controlText, DWORD controlStyle,
    int x, int y, int width, int height);

void CreateLabel(HWND hParent, const LPCTSTR tabName, const LPCTSTR groupBoxName, int x, int y, int width, int height, TCHAR* text);
void CreateCheckbox(HWND hParent, const LPCTSTR tabName, const LPCTSTR groupBoxName, int x, int y, int width, int height, TCHAR* text);
void CreateButton(HWND hParent, const LPCTSTR tabName, const LPCTSTR groupBoxName, int x, int y, int width, int height, TCHAR* text);
void CreateRadioButton(HWND hParent, const LPCTSTR tabName, const LPCTSTR groupBoxName, int x, int y, int width, int height, TCHAR* text);
void CreateTextbox(HWND hParent, const LPCTSTR tabName, const LPCTSTR groupBoxName, int x, int y, int width, int height, TCHAR* text);

void AddTrayIcon(HWND hwnd, UINT ID, UINT iconID, LPCTSTR szTip, UINT uCallbackMessage);

void ShowBalloonTip(HWND hWnd, LPCTSTR szTitle, LPCTSTR szText, DWORD dwInfoFlags, UINT uTimeout);
void ShowInfoBalloonTip(HWND hWnd, LPCTSTR szTitle, LPCTSTR szText, UINT uTimeout);
void ShowWarningBalloonTip(HWND hWnd, LPCTSTR szTitle, LPCTSTR szText, UINT uTimeout);
void ShowErrorBalloonTip(HWND hWnd, LPCTSTR szTitle, LPCTSTR szText, UINT uTimeout);