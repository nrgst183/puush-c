#pragma once

#include <Windows.h>
#include "settings_file.h"
#include <stdio.h>

void CaptureScreenAndSave(RECT screenBounds, LPCSTR filepath);
BOOL GetScreenBounds(enum FullscreenCaptureMode mode, RECT* bounds);

