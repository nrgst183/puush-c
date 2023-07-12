#pragma once

#include <Windows.h>
#include <Windowsx.h>
#include <shellapi.h>
#include "screenshot.h"
#include "screen_region_select_window.h"
#include "settings_window.h"
#include "settings_file.h"
#include "hotkey.h"
#include "../resource.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
void AddTrayIcon(HWND hwnd);
void ShowBalloonTip(HWND hwnd, LPCTSTR szTitle, LPCTSTR szMessage);
