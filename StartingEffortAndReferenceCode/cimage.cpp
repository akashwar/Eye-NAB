#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include "resource.h"

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

char szClassName[] = "BmpTestApp";

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
    HWND hwnd;
    MSG messages;
    WNDCLASSEX wincl;

    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof(WNDCLASSEX);

    wincl.hIcon = NULL;
    wincl.hIconSm = NULL;
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;

    wincl.hbrBackground = (HBRUSH)COLOR_WINDOW;

    if(!RegisterClassEx(&wincl))
        return 0;

    DEVMODE display;
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &display);

    hwnd = CreateWindowEx(WS_OVERLAPPED, szClassName, "Bitmap Display Test", WS_OVERLAPPEDWINDOW, 0, 0, 500, 500, HWND_DESKTOP, NULL, hThisInstance, NULL);
    ShowWindow(hwnd, nCmdShow);

    while(GetMessage(&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    return messages.wParam;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        HWND logo;
        case WM_CREATE:
            RECT hwndRect;
            GetWindowRect(hwnd, &hwndRect);
            int winWidth;
            int winHeight;
            winWidth = hwndRect.right - hwndRect.left;
            winHeight = hwndRect.bottom - hwndRect.top;

            HBITMAP logoImage;
            logoImage = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_LOGO), IMAGE_BITMAP, winWidth - 180, winHeight - 530, LR_DEFAULTCOLOR);
            logo = CreateWindow(WC_STATIC, NULL, SS_BITMAP | SS_CENTER | WS_CHILD | WS_VISIBLE, 0, 0, winWidth - 180, winHeight - 530, hwnd, NULL, NULL, NULL);
            SendMessage(logo, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)logoImage);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}


// resource.rc
IDB_LOGO BITMAP "c:\\tv\\lighthouse.jpg";

// resource.h
#define IDB_LOGO 300