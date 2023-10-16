#undef UNICODE // Pārslēgt projektu ASCII režīmā
#include <windows.h>
#include <stdio.h>
#include "resource.h"

#pragma warning(disable : 4996)

void ShowInfo(HWND wnd);

// Ziņojumu apstrādātāja prototips
BOOL CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
// Galvenā funkcija 
// ja galvenā loga šablonam ir piešķirts resursa identifikators IDD_MAINDIALOG
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    DialogBox(
        hInstance,
        MAKEINTRESOURCE(IDD_MAINDIALOG),
        NULL,
        DLGPROC(MainWndProc)
    );
    return 0;
}
// Ziņojumu apstrādātājs
BOOL CALLBACK MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    switch (Msg) { // pārbaudīt ziņojuma identifikatoru
    case WM_INITDIALOG: // ja tas ir inicializācijas paziņojums
        SetTimer(hWnd, 1, 250, NULL);
        ShowInfo(hWnd);
        return TRUE;

    case WM_COMMAND: // ja tas ir paziņojums no vadības elementa
        switch (LOWORD(wParam)) { // pārbaudīt vadības elementa ID

        case IDOK: // ja bija uzspiesta OK poga
            DestroyWindow(hWnd); // aizvērt logu
            return TRUE;
        }
        return FALSE;
    case WM_DESTROY: // ja tas ir pabeigšanas pieprasījums
        PostQuitMessage(0); // pabeigt programmu
        return TRUE;

    case WM_TIMER:
        ShowInfo(hWnd);
        return TRUE;
    }

    return FALSE;
}

void ShowInfo(HWND wnd) {
    // Saņemt un parādīt uz loga datorvārdu ar funkciju GetComputerName
    char buf[100];
    DWORD size = 100;
    if (!GetComputerName(buf, &size)) strcpy_s(buf, "error");
    SetDlgItemText(wnd, IDC_COMPNAME, buf);

    // Saņemt un parādīt uz loga Windows versiju ar funkciju GetVersionEx
    OSVERSIONINFO ver;
    ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&ver);
    sprintf_s(buf, "%d.%d (build %d) %s", ver.dwMajorVersion,
        ver.dwMinorVersion, ver.dwBuildNumber, ver.szCSDVersion);
    SetDlgItemText(wnd, IDC_VERSION, buf);

    // Saņemt un parādīt uz loga lietotāja vārdu
    // izmantojot funkciju GetUserName
    if (GetUserName(buf, &size))
        SetDlgItemText(wnd, IDC_USERNAME, buf);
    else
        SetDlgItemText(wnd, IDC_USERNAME, "error");

    // Saņemt un parādīt uz loga tekošo datumu un laiku
    // izmantojot funkciju GetLocalTime
    SYSTEMTIME sysTime;
    GetLocalTime(&sysTime);
    sprintf_s(buf, "%02d/%02d/%04d %02d:%02d:%02d",
        sysTime.wMonth, sysTime.wDay, sysTime.wYear,
        sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
    SetDlgItemText(wnd, IDC_DATETIME, buf);


    // Saņemt un parādīt uz loga informāciju par operatīvas atmiņas noslogojumu
    // izmantojot funkciju GlobalMemoryStatusEx
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(MEMORYSTATUSEX);
    if (GlobalMemoryStatusEx(&memStatus)) {
        sprintf_s(buf, "%llu MB", memStatus.ullTotalPhys / (1024 * 1024));
        SetDlgItemText(wnd, IDC_TOTALRAM, buf);

        sprintf_s(buf, "%llu MB", memStatus.ullAvailPhys / (1024 * 1024));
        SetDlgItemText(wnd, IDC_FREERAM, buf);

        int percentFree = static_cast<int>((memStatus.ullAvailPhys * 100) / memStatus.ullTotalPhys);
        sprintf_s(buf, "%d%%", percentFree);
        SetDlgItemText(wnd, IDC_PERCENTRAM, buf);
    }
    else {
        SetDlgItemText(wnd, IDC_TOTALRAM, "N/A");
        SetDlgItemText(wnd, IDC_FREERAM, "N/A");
        SetDlgItemText(wnd, IDC_PERCENTRAM, "N/A");
    }

    // Saņemt un parādīt uz loga informāciju par datora darba laiku
    // izmantojot funkciju GetTickCount
    DWORD uptime = GetTickCount64();
    DWORD seconds = uptime / 1000;
    DWORD minutes = seconds / 60;
    DWORD hours = minutes / 60;
    DWORD days = hours / 24;
    sprintf_s(buf, "%dd %02dh %02dm %02ds", days, hours % 24, minutes % 60, seconds % 60);
    SetDlgItemText(wnd, IDC_TIMEON, buf);
}