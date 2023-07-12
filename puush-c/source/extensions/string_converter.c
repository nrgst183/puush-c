#include "string_converter.h"

// Convert a narrow string to a TCHAR*
void ConvertNarrowStringToTChar(const char* source, TCHAR* dest, size_t destSize) {
#ifdef UNICODE
    MultiByteToWideChar(CP_ACP, 0, source, -1, dest, destSize);
#else
    strcpy_s(dest, destSize, source);
#endif
}

// Convert a TCHAR* to a narrow string
void ConvertTCharToNarrowString(const TCHAR* source, char* dest, size_t destSize) {
#ifdef UNICODE
    WideCharToMultiByte(CP_ACP, 0, source, -1, dest, destSize, NULL, NULL);
#else
    strcpy_s(dest, destSize, source);
#endif
}