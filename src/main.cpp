#define UNICODE
#define _UNICODE

#include <stdio.h>
#include <windows.h>
#include <shellapi.h>
#include <wchar.h>

NOTIFYICONDATA nid;

bool IsDebug = true; // enables console and maybe some other stuff later on

void HideConsole() {
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);
}

bool IsConsoleHidden() {
    HWND hWnd = GetConsoleWindow();
    return !IsWindowVisible(hWnd);
}

void setupTrayIcon() {
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = GetConsoleWindow();
    nid.uID = 1;

    nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;

    nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);

    wcscpy(nid.szTip, L"IRPresence");

    nid.uCallbackMessage = WM_USER + 1;

    Shell_NotifyIcon(NIM_ADD, &nid);
}

int main() {
    printf("IRPresence running...\n");
    setupTrayIcon();

    if (!IsConsoleHidden()) {
        HideConsole();
    }

    Sleep(100000);

    return 0;
}
