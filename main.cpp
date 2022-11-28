#define _CRT_SECURE_NO_WARNINGS
#include "DeviceHandler.h"
#include <iostream>
#include <filesystem>
#include "Config.h"
using namespace std;

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

void InitLog()
{
    // debug console
    //AllocConsole();
    //freopen("CONOUT$", "w", stdout);

    std::error_code ec;
    filesystem::create_directories(".saved", ec);
    SetFileAttributes(L".saved", FILE_ATTRIBUTE_HIDDEN);
    freopen(".saved/output.log", "w", stdout);
    setbuf(stdout, NULL);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    InitLog();

    // create window
    if (!CreateWnd(hInstance))
    {
        printf("Fail to create window\n");
        return -1;
    }

    // load config
    InitConfig();

    // register notification
    RegisterDeviceNotify(hWnd);

    // event loop
    printf("[INFO] ready\n");
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}