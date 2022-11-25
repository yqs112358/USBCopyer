#pragma once
#include <Windows.h>

void RegisterDeviceNotify(HWND hWnd);
LRESULT DeviceChange(UINT message, WPARAM wParam, LPARAM lParam);