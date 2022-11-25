#include "DeviceHandler.h"
#include <iostream>
#include <windows.h>
#include <Dbt.h>
#include "Utils.h"

static const GUID GUID_DEVINTERFACE_USB_DEVICE = { 0xA5DCBF10, 0x6530, 0x11D2, {0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED} };
void RegisterDeviceNotify(HWND hWnd)
{
    HDEVNOTIFY hDevNotify;
    DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
    ZeroMemory(&NotificationFilter, sizeof(NotificationFilter));
    NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    NotificationFilter.dbcc_classguid = GUID_DEVINTERFACE_USB_DEVICE;
    hDevNotify = RegisterDeviceNotification(hWnd, &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
}

void AddDevice(char deviceId)
{

}

void RemoveDevice(char deviceId)
{

}

// device change callback
LRESULT DeviceChange(UINT message, WPARAM wParam, LPARAM lParam)
{
    if (wParam == DBT_DEVICEARRIVAL || wParam == DBT_DEVICEQUERYREMOVE || wParam == DBT_DEVICEREMOVECOMPLETE)
    {
        PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)lParam;
        if (pHdr->dbch_devicetype == DBT_DEVTYP_VOLUME)
        {
            PDEV_BROADCAST_VOLUME pDevVolume = (PDEV_BROADCAST_VOLUME)lParam;
            char driverId = GetFirstDriveFromMask(pDevVolume->dbcv_unitmask);
            if (wParam == DBT_DEVICEARRIVAL)
            {
                printf("add %c\r\n", driverId);
                AddDevice(driverId);
            }
            else
            {
                printf("remove %c\r\n", driverId);
                RemoveDevice(driverId);
            }
        }
    }
    return 0;
}