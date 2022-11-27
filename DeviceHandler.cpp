#include "DeviceHandler.h"
#include <iostream>
#include <windows.h>
#include <Dbt.h>
#include "Utils.h"
#include "Copyer.h"
#include "Config.h"
#include <unordered_map>
#include <filesystem>
#include <thread>
using namespace std;

#define WAIT_BEFORE_KILL_THREAD 100

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
    printf("[INFO] Device notification registered.\n");
}

unordered_map<char, HANDLE> copyerThreadMap;
unordered_map<char, bool*> exitSignalMap;
void AddDevice(char drive)
{
    bool* exitSign = new bool(false);
    exitSignalMap[drive] = exitSign;

    CopyInfoStruct* copyInfo = new CopyInfoStruct;
    copyInfo->fromDir = string(1, drive) + ":\\";           // like D:\ 
    copyInfo->targetDir = GetSaveDir();                     // like C:\aaa\b\ 
    copyInfo->exitSign = exitSign;

    unsigned int threadId;
    HANDLE copyerThread = (HANDLE)_beginthreadex(NULL, 0, StartCopy, (void*)copyInfo, 0, &threadId);
    if (copyerThread != NULL)
    {
        // start success
        copyerThreadMap[drive] = copyerThread;
    }
    else
        printf("[ERROR] Fail to start copy thread on %c:\\\\ \n", drive);
}

void RemoveDevice(char drive)
{
    auto iter = copyerThreadMap.find(drive);
    if (iter != copyerThreadMap.end())
    {
        bool* exitSignal = exitSignalMap[drive];
        *exitSignal = true;

        DWORD exitCode;
        HANDLE hThread = iter->second;
        GetExitCodeThread(hThread, &exitCode);
        if (exitCode == STILL_ACTIVE)           // if not exited
        {
            printf("[WARN] Copy thread working... ");
            DWORD waitRes = WaitForSingleObject(hThread, WAIT_BEFORE_KILL_THREAD);
            if (waitRes == WAIT_TIMEOUT || waitRes == WAIT_FAILED)
            {
                printf("terminated\n");
                TerminateThread(hThread, 1);
            }
            else
            {
                printf("finished\n");
            }

            delete exitSignal;
            copyerThreadMap.erase(drive);
            exitSignalMap.erase(drive);
        }
    }
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
            char drive = GetFirstDriveFromMask(pDevVolume->dbcv_unitmask);

            if (wParam == DBT_DEVICEARRIVAL)
            {
                printf("[INFO] Device add %c\n", drive);
                AddDevice(drive);
            }
            else
            {
                printf("[INFO] Device remove %c\n", drive);
                RemoveDevice(drive);
            }
        }
    }
    return 0;
}