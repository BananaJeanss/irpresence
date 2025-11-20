#define _WIN32_IE 0x0600 // mingw needs this for whatever reason

#define UNICODE
#define _UNICODE

#include <windows.h>
#include <shellapi.h>
#include <wchar.h>
#include <shellapi.h>
#include <wchar.h>
#include <stdio.h>

NOTIFYICONDATA nid;


LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_USER + 1: // tray icon callback
        if (lParam == WM_RBUTTONUP)
        { // right-click popup
            POINT p;
            GetCursorPos(&p);

            HMENU menu = CreatePopupMenu();
            InsertMenu(menu, -1, MF_BYPOSITION, 1, L"Exit");

            SetForegroundWindow(hwnd);

            TrackPopupMenu(menu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, p.x, p.y, 0, hwnd, NULL);

            DestroyMenu(menu);
        }
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case 1:
            Shell_NotifyIcon(NIM_DELETE, &nid);
            PostQuitMessage(0);
            break;
        }
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void setupTrayIcon()
{
    // Register window class
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"IRPresenceWindow";
    RegisterClassEx(&wc);

    // Create hidden window
    HWND hwnd = CreateWindowEx(
        0, L"IRPresenceWindow", L"", 0,
        0, 0, 0, 0,
        NULL, NULL, GetModuleHandle(NULL), NULL);

    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = 1;

    nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);

    wcscpy(nid.szTip, L"IRPresence");
    nid.uCallbackMessage = WM_USER + 1;

    Shell_NotifyIcon(NIM_ADD, &nid);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void showNotification(const wchar_t *title, const wchar_t *msg)
{
    nid.uFlags = NIF_INFO;
    wcscpy(nid.szInfoTitle, title);
    wcscpy(nid.szInfo, msg);
    nid.dwInfoFlags = NIIF_INFO;

    Shell_NotifyIcon(NIM_MODIFY, &nid);
}

int main()
{
    printf("IRPresence running...\n");
    showNotification(L"IRPresence", L"Application started!");
    setupTrayIcon(); // runs forever until exit
    return 0;
}
