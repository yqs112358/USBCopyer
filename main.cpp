#define _CRT_SECURE_NO_WARNINGS
#include "DeviceHandler.h"
#include <iostream>

HINSTANCE hInst;
HWND hWnd;
WCHAR szTitle[100] = L"copyertitle";
WCHAR szWindowClass[100] = L"copyerwndclass";

// window proc
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DEVICECHANGE:
        return DeviceChange(message, wParam, lParam);       // device change event
    case WM_DESTROY:
    case WM_QUIT:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

bool CreateWnd(HINSTANCE hInstance)
{
    hInst = hInstance;

    // register windows class
    WNDCLASSEXW wcex;
    ZeroMemory(&wcex, sizeof(wcex));
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.lpszClassName  = szWindowClass;

    RegisterClassExW(&wcex);

    // create hidden window
    HWND hwnd = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOPMOST,
        szWindowClass, szTitle, WS_POPUP, 0, 0, 0, 0, NULL, NULL, hInstance, nullptr);

    if (!hwnd)
        return false;
    hWnd = hwnd;

    UpdateWindow(hwnd);
    return true;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create window
    if (!CreateWnd(hInstance))
        return -1;

    // register notification
    RegisterDeviceNotify(hWnd);

    AllocConsole();
    freopen("CONOUT$", "w", stdout);

    // event loop
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}