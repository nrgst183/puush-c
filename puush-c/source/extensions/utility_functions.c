#include "utility_functions.h"

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

void SaveDataToFile(LPCSTR filePath, const void* data, size_t dataSize) {
    HANDLE hFile = NULL;
    DWORD bytesWritten;

    // Create the file
    hFile = CreateFileA(filePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) goto cleanup;

    // Write data to the file
    if (!WriteFile(hFile, data, dataSize, &bytesWritten, NULL)) goto cleanup;

    // Ensure all data is written to the disk
    if (!FlushFileBuffers(hFile)) goto cleanup;

cleanup:
    if (hFile && hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
}

void OpenUrlInDefaultBrowser(LPCTSTR url) {
    ShellExecute(NULL, _T("open"), url, NULL, NULL, SW_SHOWNORMAL);
}

int CreateFilenameTimestamp(LPTSTR filename, size_t bufferSize) {
    time_t now;
    if (time(&now) == (time_t)-1) {
        return 1;
    }

    struct tm timeinfo;
    if (localtime_s(&timeinfo, &now) != 0) {
        return 1;
    }

    if (_tcsftime(filename, bufferSize, _T(" (%Y-%m-%d at %H.%M.%S)"), &timeinfo) == 0) {
        return 1;
    }

    return 0;
}

