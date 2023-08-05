#pragma once

#include <Windows.h>
#include "settings_file.h"
#include <stdio.h>

void CaptureScreenAndSaveAsJpeg(RECT screenBounds, LPCSTR filepath);
void CaptureScreenAndSaveAsBitmap(RECT screenBounds, LPCSTR filepath);
BYTE* CaptureScreenBitmap(RECT screenBounds, DWORD* dwSize);

