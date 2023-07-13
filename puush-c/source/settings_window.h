#pragma once

#include <windows.h>
#include <commctrl.h>
#include "extensions/ui_extensions.h"
#include "settings_file.h"

#define TAB_CONTROL_ID 100
#define TAB_CONTROL_BASE_ID 200

WindowContext wContext;

LRESULT CALLBACK SettingsWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SettingsKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
HWND CreateSettingsWindow(HINSTANCE hInstance, PuushSettings* settings);
void MakeHotkeyFromCapturedKeys(Hotkey* hotkey);
void UpdateKeyCaptureButton(HWND button, Hotkey key);
void EndKeyCapture();
void StartKeyCapture(HWND button);