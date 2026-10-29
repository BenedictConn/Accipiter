#include <windows.h>
#include <stdio.h>

#define ID_BUTTON 1
#define ID_LOG    2
#define ID_LOG2    3
#define ID_DRAWING    4

static HWND hButton, hLog, hLog2, hDrawing;
static int  clickCount = 0;

static void Log_Append(const char *text) {
    int len = GetWindowTextLengthA(hLog);
    SendMessageA(hLog, EM_SETSEL, len, len);
    SendMessageA(hLog, EM_REPLACESEL, FALSE, (LPARAM)text);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        HINSTANCE hInst = ((CREATESTRUCT *)lParam)->hInstance;
        HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

        hButton = CreateWindowA("BUTTON", "Add line",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            10, 10, 100, 28, hwnd, (HMENU)ID_BUTTON, hInst, NULL);

        hLog = CreateWindowExA(0, "EDIT", "",
            WS_CHILD | WS_VISIBLE | WS_VSCROLL |
            ES_MULTILINE | ES_AUTOVSCROLL,
            10, 48, 300, 75, hwnd, (HMENU)ID_LOG, hInst, NULL);
        
        hLog2 = CreateWindowExA(0, "EDIT", "",
            WS_CHILD | WS_VISIBLE | WS_VSCROLL |
            ES_MULTILINE | ES_AUTOVSCROLL,
            10, 48, 300, 75, hwnd, (HMENU)ID_LOG2, hInst, NULL);

        hDrawing = CreateWindowExA(
            0, "DRAWINGAREA", "",
            WS_CHILD | WS_VISIBLE,
            10, 48, 300, 75,
            hwnd, (HMENU)ID_DRAWING, hInst, NULL);

        SendMessageA(hButton, WM_SETFONT, (WPARAM)font, TRUE);
        SendMessageA(hLog,    WM_SETFONT, (WPARAM)font, TRUE);
        return 0;
    }

    case WM_SIZE: {
        int vx = LOWORD(lParam);
        int vy = HIWORD(lParam);
        int w = (vx - 10);
        int h = (vy - 58) / 3;

        if (w > 0 && h > 0)
            // if (hLog)
            //     MoveWindow(hLog, 10, 48, w, h, TRUE);
            // if (hLog2)
            //     MoveWindow(hLog2, 10, 48 + h + 10, w, h, TRUE);
            // if (hDrawing)
            //     MoveWindow(hLog2, 10, 48 + 2 * h + 20, w, h, TRUE);
            if (hLog)
                MoveWindow(hLog, 10, 48, w, 10, TRUE);
            if (hLog2)
                MoveWindow(hLog2, 10, 48 + 10, w, 10, TRUE);
            if (hDrawing)
                MoveWindow(hDrawing, 10, 48 + 20, w, h, TRUE);
        return 0;
    }

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_BUTTON && HIWORD(wParam) == BN_CLICKED) {
            char line[64];
            clickCount++;
            snprintf(line, sizeof(line), "Button clicked (%d)\r\n", clickCount);
            Log_Append(line);
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK DrawingProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        MoveToEx(hdc, 10, 70, NULL);
        LineTo(hdc, 200, 50);

        EndPaint(hwnd, &ps);
        return 0;
    }
    }

    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR cmd, int show) {
    static const char CLASS_NAME[] = "MainWindowClass";

    WNDCLASSA wc = {0};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInst;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);

    if (!RegisterClassA(&wc)) {
        MessageBoxA(NULL, "RegisterClass failed", "Error", MB_ICONERROR);
        return 1;
    }

    WNDCLASSA drawingClass = {0};
    drawingClass.lpfnWndProc = DrawingProc;
    drawingClass.hInstance = hInst;
    drawingClass.lpszClassName = "DRAWINGAREA";
    drawingClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    drawingClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClassA(&drawingClass)) {
        MessageBoxA(NULL, "Drawing class registration failed", "Error", MB_ICONERROR);
        return 1;
    }

    HWND hwnd = CreateWindowA(CLASS_NAME, "Accipiter",
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 350,
        NULL, NULL, hInst, NULL);

    if (!hwnd) {
        MessageBoxA(NULL, "CreateWindow failed", "Error", MB_ICONERROR);
        return 1;
    }

    ShowWindow(hwnd, show);
    UpdateWindow(hwnd);

    MSG m;
    while (GetMessage(&m, NULL, 0, 0)) {
        TranslateMessage(&m);
        DispatchMessage(&m);
    }
    return (int)m.wParam;
}