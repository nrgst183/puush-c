#pragma once
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <tchar.h>
#include <mmsystem.h>

#pragma comment(lib, "Winmm.lib")

void ConvertNarrowStringToTChar(const char* source, TCHAR* dest, size_t destSize);
void ConvertTCharToNarrowString(const TCHAR* source, char* dest, size_t destSize);
void SaveDataToFile(LPCSTR filePath, const void* data, size_t dataSize);
void OpenUrlInDefaultBrowser(const TCHAR* url);
int CreateFilenameTimestamp(TCHAR* filename, size_t bufferSize);
void PlaySoundFromResource(LPCTSTR resourceName, HMODULE hModule);