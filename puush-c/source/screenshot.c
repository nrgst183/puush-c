#define TJE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

//These libraries use insecure functions, disable compiler warning just for these:
#define _CRT_SECURE_NO_WARNINGS
#include "libraries/tinyjpeg/tiny_jpeg.h"
#include "libraries/stb/stb_image_write.h"
#undef _CRT_SECURE_NO_WARNINGS

#include "screenshot.h"

void CaptureScreenAndSave(RECT screenBounds, LPCWSTR filepath, enum UploadQuality quality) {
    switch (quality) {
    case Best:
    case High:
        CaptureScreenAndSaveAsPng(screenBounds, filepath);
        break;
    case Medium:
    default:
        CaptureScreenAndSaveAsJpeg(screenBounds, filepath);
        break;
    }
}

void CaptureScreenAndSaveAsBitmap(RECT screenBounds, LPCWSTR filepath) {
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

    hFile = CreateFileW(filepath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
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

void CaptureScreenAndSaveAsJpeg(RECT screenBounds, LPCWSTR filepath) {
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

    // Convert filepath to a narrow string
    char narrowFilePath[MAX_PATH];
    ConvertWideStringToNarrow(filepath, narrowFilePath, MAX_PATH);

    tje_encode_to_file(narrowFilePath, width, height, 4, bits);

    LocalFree(bits);
}

void CaptureScreenAndSaveAsPng(RECT screenBounds, LPCWSTR filepath) {
    DWORD dwSize = 0;
    BYTE* bits = CaptureScreenBitmap(screenBounds, &dwSize);

    if (bits) {
        int x = screenBounds.right - screenBounds.left;
        int y = screenBounds.bottom - screenBounds.top;

        // Transform bits from BGRA to RGBA (assuming the data in bits is in BGRA format)
        for (DWORD i = 0; i < dwSize; i += 4) {
            BYTE temp = bits[i];
            bits[i] = bits[i + 2];
            bits[i + 2] = temp;
        }

        // Convert filepath to a narrow string
        char narrowFilePath[MAX_PATH];
        ConvertWideStringToNarrow(filepath, narrowFilePath, MAX_PATH);

        stbi_write_png(narrowFilePath, x, y, 4, bits, x * 4);

        LocalFree(bits);
    }
}

BYTE* CaptureScreenBitmap(RECT screenBounds, DWORD* dwSize) {
    HDC hScreen = NULL;
    HDC hdcMem = NULL;
    HBITMAP hBitmap = NULL;
    BYTE* bits = NULL;
    DWORD dwBmpSize = 0;

    int x = screenBounds.left;
    int y = screenBounds.top;
    int width = screenBounds.right - screenBounds.left;
    int height = screenBounds.bottom - screenBounds.top;

    hScreen = GetDC(NULL);
    if (!hScreen) {
        assert(hScreen && "Failed to get device context for the screen.");
        goto cleanup;
    }

    hdcMem = CreateCompatibleDC(hScreen);
    if (!hdcMem) {
        assert(hdcMem && "Failed to create a memory device context.");
        goto cleanup;
    }

    hBitmap = CreateCompatibleBitmap(hScreen, width, height);
    if (!hBitmap) {
        assert(hBitmap && "Failed to create a compatible bitmap.");
        goto cleanup;
    }

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
    if (!bits) {
        assert(bits && "Failed to allocate memory for bitmap bits.");
        goto cleanup;
    }

    int dibRes = GetDIBits(hdcMem, hBitmap, 0, height, bits, (BITMAPINFO*)&info, DIB_RGB_COLORS);
    if (dibRes == 0) {
        assert(dibRes != 0 && "Failed to retrieve DIB bits.");
        goto cleanup;
    }

cleanup:
    if (hBitmap) DeleteObject(hBitmap);
    if (hdcMem) DeleteDC(hdcMem);
    if (hScreen) ReleaseDC(NULL, hScreen);

    *dwSize = dwBmpSize;

    return bits;
}