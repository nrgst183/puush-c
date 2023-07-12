#pragma once

#include <Windows.h>
#include <stdio.h>

typedef enum {
    CAPTURE_ALL_SCREENS,
    CAPTURE_SCREEN_CONTAINING_MOUSE_CURSOR,
    CAPTURE_PRIMARY_SCREEN
} ScreenCaptureMode;

void CaptureScreenAndSave(RECT screenBounds, LPCSTR filepath);
BOOL GetScreenBounds(ScreenCaptureMode mode, RECT* bounds);

