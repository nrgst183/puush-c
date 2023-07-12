#pragma once

#include <Windows.h>
#include <CommCtrl.h>

int FindTabControlIndexByName(HWND hTab, const LPWSTR tabName);
HWND FindGroupBoxHandle(HWND hTab, int tabIndex, const LPWSTR groupBoxName);
void HandleTabSelectionChange(HWND hwnd, int tabPageCount);
void CreateTabControl(HWND hWnd, HINSTANCE hInstance, const LPWSTR* tabNames);
void CreateAndAddGroupBoxesToTabPage(HWND hWnd, const LPWSTR tabPageName, const LPWSTR* groupBoxNames);
void CreateAndAddControlToGroupBox(HWND hWnd, const LPWSTR tabName, const LPWSTR groupBoxName, const LPWSTR controlClassName, const LPWSTR controlText, DWORD controlStyle,
    int x, int y, int width, int height);

void CreateLabel(HWND hParent, HINSTANCE hInstance, const LPWSTR tabName, const LPWSTR groupBoxName, int x, int y, int width, int height, TCHAR* text);
void CreateCheckbox(HWND hParent, HINSTANCE hInstance, const LPWSTR tabName, const LPWSTR groupBoxName, int x, int y, int width, int height, TCHAR* text);
void CreateButton(HWND hParent, HINSTANCE hInstance, const LPWSTR tabName, const LPWSTR groupBoxName, int x, int y, int width, int height, TCHAR* text);
void CreateRadioButton(HWND hParent, HINSTANCE hInstance, const LPWSTR tabName, const LPWSTR groupBoxName, int x, int y, int width, int height, TCHAR* text);
void CreateTextbox(HWND hParent, HINSTANCE hInstance, const LPWSTR tabName, const LPWSTR groupBoxName, int x, int y, int width, int height, TCHAR* text);

