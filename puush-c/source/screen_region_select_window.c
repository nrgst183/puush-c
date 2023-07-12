#include "screen_region_select_window.h"

static HWND hOverlayWnd;
static HBRUSH hBrush;
static HPEN hPen;

static BOOL isSelecting = FALSE;
static POINT selectionStartPoint, selectionEndPoint;

// Main function for creating a screen region selection window
SelectionRectangle CreateScreenRegionSelectWindow() {

    // Get the current instance handle and set a low level mouse hook
    HINSTANCE hInstance = GetModuleHandle(NULL);
    HHOOK mouseHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, hInstance, 0);

    // Create the overlay window and show it
    hOverlayWnd = CreateOverlayWindow();
    ShowWindow(hOverlayWnd, SW_SHOWNORMAL);

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Unhook the mouse hook before destroying the window
    UnhookWindowsHookEx(mouseHook);

    // Define the selected region
    SelectionRectangle selection;
    selection.left = min(selectionStartPoint.x, selectionEndPoint.x);
    selection.top = min(selectionStartPoint.y, selectionEndPoint.y);
    selection.right = max(selectionStartPoint.x, selectionEndPoint.x);
    selection.bottom = max(selectionStartPoint.y, selectionEndPoint.y);

    // Reset the state and destroy the window
    ResetSelectionState();
    DestroyWindow(hOverlayWnd);
    UnregisterClass(TEXT("ScreenCaptureOverlay"), hInstance);

    return selection;
}

// Window procedure for the screen region selection window
LRESULT CALLBACK ScreenRegionSelectWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        // Create black border pen and grey brush
        hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
        hBrush = CreateSolidBrush(RGB(200, 200, 200));
        break;

    case WM_SETCURSOR:
        // Set the cursor to a cross if selecting
        if (isSelecting) {
            SetCursor(LoadCursor(NULL, IDC_CROSS));
            return TRUE; // Prevent the system from setting the cursor
        }
        break;

    case WM_MOUSEMOVE:
        // If the left button is down, update the selection end point
        if (wParam & MK_LBUTTON) {
            selectionEndPoint.x = GET_X_LPARAM(lParam);
            selectionEndPoint.y = GET_Y_LPARAM(lParam);
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;

    case WM_PAINT:
        // Draw the selection rectangle
        if (selectionStartPoint.x >= 0 && selectionStartPoint.y >= 0) {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hOverlayWnd, &ps);

            // Select the pen and brush into the device context
            HPEN oldPen = SelectObject(hdc, hPen);
            HBRUSH oldBrush = SelectObject(hdc, hBrush);

            // Draw the rectangle
            Rectangle(hdc,
                min(selectionStartPoint.x, selectionEndPoint.x),
                min(selectionStartPoint.y, selectionEndPoint.y),
                max(selectionStartPoint.x, selectionEndPoint.x),
                max(selectionStartPoint.y, selectionEndPoint.y));

            // Restore the old pen and brush
            SelectObject(hdc, oldPen);
            SelectObject(hdc, oldBrush);

            EndPaint(hWnd, &ps);
        }
        break;

    case WM_DESTROY:
        // Clean up the pen and brush
        DeleteObject(hPen);
        DeleteObject(hBrush);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Low level mouse procedure to handle mouse input
LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {

        MSLLHOOKSTRUCT* mhs;

        switch (wParam) {
        case WM_LBUTTONDOWN:
            // Start selecting and initialize selection points
            isSelecting = TRUE;
            mhs = (MSLLHOOKSTRUCT*)lParam;
            selectionStartPoint = mhs->pt;
            selectionEndPoint = mhs->pt;
            InvalidateRect(hOverlayWnd, NULL, TRUE);
            break;

        case WM_MOUSEMOVE:
            // Update the selection end point
            if (isSelecting) {
                mhs = (MSLLHOOKSTRUCT*)lParam;
                selectionEndPoint = mhs->pt;
                InvalidateRect(hOverlayWnd, NULL, TRUE);
            }
            break;

        case WM_LBUTTONUP:
            // End selecting and update the selection end point
            isSelecting = FALSE;
            mhs = (MSLLHOOKSTRUCT*)lParam;
            selectionEndPoint = mhs->pt;
            InvalidateRect(hOverlayWnd, NULL, TRUE);
            PostQuitMessage(0);
            break;
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

// Create an overlay window for selection
HWND CreateOverlayWindow() {
    WNDCLASSEX wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpfnWndProc = ScreenRegionSelectWndProc;
    wcex.hInstance = GetModuleHandle(NULL);
    wcex.lpszClassName = TEXT("ScreenCaptureOverlay");
    RegisterClassEx(&wcex);

    HWND hWnd = CreateWindowEx(WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
        TEXT("ScreenCaptureOverlay"), NULL, WS_POPUP,
        0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
        NULL, NULL, wcex.hInstance, NULL);

    SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 128, LWA_COLORKEY | LWA_ALPHA);

    return hWnd;
}

// Reset the selection state
void ResetSelectionState() {
    isSelecting = FALSE;

    selectionStartPoint.x = -1;
    selectionStartPoint.y = -1;
    selectionEndPoint.x = -1;
    selectionEndPoint.y = -1;
}
