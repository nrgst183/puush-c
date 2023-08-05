#include "utility_functions.h"

// Convert a narrow string to a TCHAR*
void ConvertNarrowStringToWide(const char* source, WCHAR* dest, size_t destSize) {
    MultiByteToWideChar(CP_ACP, 0, source, -1, dest, destSize);
}

void ConvertWideStringToNarrow(const WCHAR* source, char* dest, size_t destSize) {
    WideCharToMultiByte(CP_ACP, 0, source, -1, dest, destSize, NULL, NULL);
}


BOOL GetScreenBounds(enum FullscreenCaptureMode mode, RECT* bounds) {
    if (!bounds) return FALSE;

    switch (mode) {
    case AllScreens:
        bounds->left = GetSystemMetrics(SM_XVIRTUALSCREEN);
        bounds->top = GetSystemMetrics(SM_YVIRTUALSCREEN);
        bounds->right = bounds->left + GetSystemMetrics(SM_CXVIRTUALSCREEN);
        bounds->bottom = bounds->top + GetSystemMetrics(SM_CYVIRTUALSCREEN);
        return TRUE;

    case ScreenContainingMouseCursor:
    {
        POINT cursorPos;
        GetCursorPos(&cursorPos);
        HMONITOR hMonitor = MonitorFromPoint(cursorPos, MONITOR_DEFAULTTONEAREST);
        MONITORINFO monitorInfo = { sizeof(MONITORINFO) };
        if (GetMonitorInfo(hMonitor, &monitorInfo)) {
            *bounds = monitorInfo.rcMonitor;
            return TRUE;
        }
        return FALSE;
    }

    case PrimaryScreen:
    {
        bounds->left = 0;
        bounds->top = 0;
        bounds->right = GetSystemMetrics(SM_CXSCREEN);
        bounds->bottom = GetSystemMetrics(SM_CYSCREEN);
        return TRUE;
    }
    }
    return FALSE;
}

void SaveDataToFile(LPCWSTR filePath, const void* data, size_t dataSize) {
    HANDLE hFile = NULL;
    DWORD bytesWritten;

    // Create the file
    hFile = CreateFileW(filePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) goto cleanup;

    // Write data to the file
    if (!WriteFile(hFile, data, dataSize, &bytesWritten, NULL)) goto cleanup;

    // Ensure all data is written to the disk
    if (!FlushFileBuffers(hFile)) goto cleanup;

cleanup:
    if (hFile && hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
}

void OpenUrlInDefaultBrowser(LPCWSTR url) {
    ShellExecuteW(NULL, L"open", url, NULL, NULL, SW_SHOWNORMAL);
}

int CreateFilenameTimestamp(WCHAR* filename, size_t bufferSize) {
    time_t now;
    if (time(&now) == (time_t)-1) {
        return 1;
    }

    struct tm timeinfo;
    if (localtime_s(&timeinfo, &now) != 0) {
        return 1;
    }

    if (wcsftime(filename, bufferSize, L" (%Y-%m-%d at %H.%M.%S)", &timeinfo) == 0) {
        return 1;
    }

    return 0;
}

