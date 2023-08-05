#pragma once

#include <Windows.h>
#include "settings_file.h"
#include <stdio.h>

#include "settings_file.h"

void CaptureScreenAndSave(RECT screenBounds, LPCWSTR filepath, enum UploadQuality quality);
void CaptureScreenAndSaveAsJpeg(RECT screenBounds, LPCWSTR filepath);
void CaptureScreenAndSaveAsPng(RECT screenBounds, LPCWSTR filepath);
void CaptureScreenAndSaveAsBitmap(RECT screenBounds, LPCWSTR filepath);
BYTE* CaptureScreenBitmap(RECT screenBounds, DWORD* dwSize);

