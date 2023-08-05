#define TJE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

//These libraries use insecure functions, disable compiler warning just for these:
#define _CRT_SECURE_NO_WARNINGS
#include "libraries/tinyjpeg/tiny_jpeg.h"
#include "libraries/stb/stb_image_write.h"
#undef _CRT_SECURE_NO_WARNINGS

#include "screenshot.h"


void CaptureScreenAndSaveAsBitmap(RECT screenBounds, LPCSTR filepath) {
    HANDLE hFile = NULL;

    DWORD dwBmpSize;
    BYTE* bits = CaptureScreenBitmap(screenBounds, &dwBmpSize);
    if (!bits) return;

    int width = screenBounds.right - screenBounds.left;
    int height = screenBounds.bottom - screenBounds.top;

    BITMAPFILEHEADER bmfh = { 0 };
    bmfh.bfType = 0x4D42;
    bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwBmpSize;
    bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    BITMAPINFOHEADER info = { 0 };
    info.biSize = sizeof(BITMAPINFOHEADER);
    info.biWidth = width;
    info.biHeight = -height;
    info.biPlanes = 1;
    info.biBitCount = 32;
    info.biCompression = BI_RGB;

    hFile = CreateFileA(filepath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        DWORD dwWritten = 0;

        WriteFile(hFile, &bmfh, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
        WriteFile(hFile, &info, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
        WriteFile(hFile, bits, dwBmpSize, &dwWritten, NULL);

        FlushFileBuffers(hFile);
        CloseHandle(hFile);
    }

    LocalFree(bits);
}

void CaptureScreenAndSaveAsJpeg(RECT screenBounds, LPCSTR filepath) {
    DWORD dwBmpSize;
    BYTE* bits = CaptureScreenBitmap(screenBounds, &dwBmpSize);
    if (!bits) return;

    int width = screenBounds.right - screenBounds.left;
    int height = screenBounds.bottom - screenBounds.top;

    // Transform bits from BGRA to RGBA (assuming the data in bits is in BGRA format)
    for (DWORD i = 0; i < dwBmpSize; i += 4) {
        BYTE temp = bits[i];
        bits[i] = bits[i + 2];
        bits[i + 2] = temp;
    }

    tje_encode_to_file(filepath, width, height, 4, bits);

    LocalFree(bits);
}

BYTE* CaptureScreenBitmap(RECT screenBounds, DWORD* dwSize) {
    HDC hScreen = NULL;
    HDC hdcMem = NULL;
    HBITMAP hBitmap = NULL;
    BYTE* bits = NULL;
    DWORD dwBmpSize = 0;  // Initialize dwBmpSize to 0

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

    dwBmpSize = ((width * info.biBitCount + 31) / 32) * 4 * height;

    bits = (BYTE*)LocalAlloc(LPTR, dwBmpSize);
    if (!bits) goto cleanup;

    GetDIBits(hdcMem, hBitmap, 0, height, bits, (BITMAPINFO*)&info, DIB_RGB_COLORS);

cleanup:
    if (hBitmap) DeleteObject(hBitmap);
    if (hdcMem) DeleteDC(hdcMem);
    if (hScreen) ReleaseDC(NULL, hScreen);

    *dwSize = dwBmpSize;  // Set *dwSize to dwBmpSize, which is now always initialized

    return bits;
}