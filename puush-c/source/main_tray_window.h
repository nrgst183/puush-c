#pragma once

#include <Windows.h>
#include <Windowsx.h>
#include <shellapi.h>
#include <tchar.h>
#include "../resource.h"
#include "extensions/ui_extensions.h"
#include "screenshot.h"
#include "settings_window.h"
#include "settings_file.h"
#include "hotkey.h"
#include "screen_region_select_window.h"

#pragma comment(lib, "Msimg32.lib")

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
void ShowPuushToggleBalloonTip(BOOL puushDisabled);
void ShowPuushCompleteBalloonTip(const TCHAR* url);
void TogglePuushFunctionality();
void PlaySuccessSound();
int CreateScreenshotFilePath(WCHAR* fileName, size_t bufferSize, WCHAR* folderPath);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow);