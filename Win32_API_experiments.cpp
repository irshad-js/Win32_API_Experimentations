

#include <windows.h>

#include "resource.h" 

#include "framework.h"
#include "Win32_API_experiments.h"

LRESULT CALLBACK AboutDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
)
{

    const wchar_t class_name[] = L"First window class";

    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = class_name;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowExW(
        0,
        class_name,
        L"Learning project",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );
   

    if (hwnd == NULL)return 0;

    ShowWindow(hwnd, nShowCmd);

    MSG msg = {};

    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);

    }
    return 0;
}


LRESULT CALLBACK AboutDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch (Message)
    {
    case WM_INITDIALOG:

        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            EndDialog(hwnd, IDOK);
            break;
        case IDCANCEL:
            EndDialog(hwnd, IDCANCEL);
            break;
        }
        break;

    case WM_KEYDOWN: {
        switch (wParam) {
        case VK_SPACE:
            break;


        }
        return 0;
    }

    default:
        return FALSE;
    }
    return TRUE;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_INITDIALOG: 
    {
        int ret = DialogBoxA(GetModuleHandle(NULL),
            (LPCSTR)MAKEINTRESOURCE(IDD_DIALOG1), hwnd, AboutDlgProc);

        if (ret == IDOK) {
            MessageBox(hwnd, L"Dialog exited with IDOK.", L"Notice",
                MB_OK | MB_ICONINFORMATION);
        }
        else if (ret == IDCANCEL) {
            MessageBox(hwnd, L"Dialog exited with IDCANCEL.", L"Notice",
                MB_OK | MB_ICONINFORMATION);
        }
        else if (ret == -1) {
            MessageBox(hwnd, L"Dialog failed!", L"Error",
                MB_OK | MB_ICONINFORMATION);
        }
    }
    break;
    case WM_DESTROY:
    {
        int ret = DialogBoxA(GetModuleHandle(NULL),
            (LPCSTR)MAKEINTRESOURCE(IDD_DIALOG1), hwnd, AboutDlgProc);

        if (ret == IDOK) {
            MessageBox(hwnd, L"Dialog exited with IDOK.", L"Notice",
                MB_OK | MB_ICONINFORMATION);
        }
        else if (ret == IDCANCEL) {
            MessageBox(hwnd, L"Dialog exited with IDCANCEL.", L"Notice",
                MB_OK | MB_ICONINFORMATION);
        }
        else if (ret == -1) {
            MessageBox(hwnd, L"Dialog failed!", L"Error",
                MB_OK | MB_ICONINFORMATION);
        }
    }
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // All painting occurs here, between BeginPaint and EndPaint.

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps);
    }
    return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

