#pragma once
#include <windows.h>
#include <string.h>

void ConvertNarrowStringToTChar(const char* source, TCHAR* dest, size_t destSize);
void ConvertTCharToNarrowString(const TCHAR* source, char* dest, size_t destSize);