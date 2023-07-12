#pragma once

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

typedef struct {
    int left;
    int top;
    int right;
    int bottom;
} SelectionRectangle;

SelectionRectangle CreateScreenRegionSelectWindow();
LRESULT CALLBACK ScreenRegionSelectWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
HWND CreateOverlayWindow();
LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);
void ResetSelectionState();
