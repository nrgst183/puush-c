#include "screenshot.h"

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

void CaptureScreenAndSave(RECT screenBounds, LPCSTR filepath) {
    HDC hScreen = NULL;
    HDC hdcMem = NULL;
    HBITMAP hBitmap = NULL;
    HANDLE hFile = NULL;
    BYTE* bits = NULL;

    int x = screenBounds.left;
    int y = screenBounds.top;
    int width = screenBounds.right - screenBounds.left;
    int height = screenBounds.bottom - screenBounds.top;

    hScreen = GetDC(NULL);
    if (!hScreen) goto cleanup;

    hdcMem = CreateCompatibleDC(hScreen);
    if (!hdcMem) goto cleanup;

    hBitmap = CreateCompatibleBitmap(hScreen, width, height);
    if (!hBitmap) goto cleanup;

    SelectObject(hdcMem, hBitmap);

    BitBlt(hdcMem, 0, 0, width, height, hScreen, x, y, SRCCOPY);

    BITMAPINFOHEADER info = { 0 };
    info.biSize = sizeof(BITMAPINFOHEADER);
    info.biWidth = width;
    info.biHeight = -height;
    info.biPlanes = 1;
    info.biBitCount = 32;
    info.biCompression = BI_RGB;

    DWORD dwBmpSize = ((width * info.biBitCount + 31) / 32) * 4 * height;

    BITMAPFILEHEADER bmfh = { 0 };
    bmfh.bfType = 0x4D42;
    bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwBmpSize;
    bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    hFile = CreateFileA(filepath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) goto cleanup;

    DWORD dwWritten = 0;

    WriteFile(hFile, &bmfh, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
    WriteFile(hFile, &info, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);

    bits = (BYTE*)LocalAlloc(LPTR, dwBmpSize);
    if (!bits) goto cleanup;

    GetDIBits(hdcMem, hBitmap, 0, height, bits, (BITMAPINFO*)&info, DIB_RGB_COLORS);
    WriteFile(hFile, bits, dwBmpSize, &dwWritten, NULL);

    FlushFileBuffers(hFile);

cleanup:
    if (bits) LocalFree(bits);
    if (hBitmap) DeleteObject(hBitmap);
    if (hdcMem) DeleteDC(hdcMem);
    if (hScreen) ReleaseDC(NULL, hScreen);
    if (hFile && hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
}

