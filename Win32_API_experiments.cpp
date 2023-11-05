


#include <windows.h>

#include "resource.h" 

#include "framework.h"
#include "Win32_API_experiments.h"
#include "qwin_gui.h"

LRESULT CALLBACK AboutDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static HINSTANCE l_hInst;   /* this application instance */

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

    l_hInst = hInstance;

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

static SegmentDisplay   l_scoreBoard; /* segment display for the score */


static int count = 0;
static void update_seg(int n) {
    /* update the score in the l_scoreBoard SegmentDisplay */
    SegmentDisplay_setSegment(&l_scoreBoard, 0U, n++);
    n /= 10U;
    SegmentDisplay_setSegment(&l_scoreBoard, 1U, n++);
    n /= 10U;
}

LRESULT CALLBACK AboutDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch (Message)
    {
    case WM_INITDIALOG: {

        SegmentDisplay_init(&l_scoreBoard,
            2U,   /* 4 "segments" (digits 0-3) */
            10U); /* 10 bitmaps (for 0-9 states) */
        SegmentDisplay_initSegment(&l_scoreBoard, 0U, IDC_seg0);
        SegmentDisplay_initSegment(&l_scoreBoard, 1U, IDC_seg1);
        SegmentDisplay_initBitmap(&l_scoreBoard,
            0U, LoadBitmap(l_hInst, MAKEINTRESOURCE(IDB_BITMAP1)));
        SegmentDisplay_initBitmap(&l_scoreBoard,
            1U, LoadBitmap(l_hInst, MAKEINTRESOURCE(IDB_BITMAP2)));
        SegmentDisplay_initBitmap(&l_scoreBoard,
            2U, LoadBitmap(l_hInst, MAKEINTRESOURCE(IDB_BITMAP3)));
        SegmentDisplay_initBitmap(&l_scoreBoard,
            3U, LoadBitmap(l_hInst, MAKEINTRESOURCE(IDB_BITMAP4)));
        SegmentDisplay_initBitmap(&l_scoreBoard,
            4U, LoadBitmap(l_hInst, MAKEINTRESOURCE(IDB_BITMAP5)));
        SegmentDisplay_initBitmap(&l_scoreBoard,
            5U, LoadBitmap(l_hInst, MAKEINTRESOURCE(IDB_BITMAP7)));
        SegmentDisplay_initBitmap(&l_scoreBoard,
            6U, LoadBitmap(l_hInst, MAKEINTRESOURCE(IDB_BITMAP8)));
        SegmentDisplay_initBitmap(&l_scoreBoard,
            7U, LoadBitmap(l_hInst, MAKEINTRESOURCE(IDB_BITMAP9)));
        SegmentDisplay_initBitmap(&l_scoreBoard,
            8U, LoadBitmap(l_hInst, MAKEINTRESOURCE(IDB_BITMAP10)));
        SegmentDisplay_initBitmap(&l_scoreBoard,
            9U, LoadBitmap(l_hInst, MAKEINTRESOURCE(IDB_BITMAP11)));

        return 0;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            count++;
            update_seg(count);
            break;
            break;
        case IDCANCEL:
            EndDialog(hwnd, IDCANCEL);
            break;
        }
        break;

    case WM_KEYDOWN: {
        switch (wParam) {
        case VK_SPACE:
            count++;
            update_seg(count);
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
        /* Perform initialization after all child windows have been created */

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

