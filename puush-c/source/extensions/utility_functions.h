#pragma once

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <tchar.h>
#include <mmsystem.h>

#include "../settings_file.h"

#pragma comment(lib, "Winmm.lib")

void ConvertNarrowStringToWide(const char* source, WCHAR* dest, size_t destSize);
void ConvertWideStringToNarrow(const WCHAR* source, char* dest, size_t destSize);
void SaveDataToFile(LPCWSTR filePath, const void* data, size_t dataSize);
void OpenUrlInDefaultBrowser(LPCWSTR url);
int CreateFilenameTimestamp(WCHAR* filename, size_t bufferSize);
BOOL GetScreenBounds(enum FullscreenCaptureMode mode, RECT* bounds);