


#include <windows.h>

#include "resource.h" 

#include "framework.h"
#include "Win32_API_experiments.h"

static HINSTANCE l_hInst;   /* this application instance */
static HWND      l_hWnd;    /* main window handle */
static LPSTR     l_cmdLine; /* the command line string */

static int l_paused;

/* Local functions ---------------------------------------------------------*/
static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg,
    WPARAM wParam, LPARAM lParam);

/*..........................................................................*/
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
    LPSTR cmdLine, int iCmdShow)
{
    return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, WndProc);
}

const int ID_TIMER = 1;
static int counter = 1111;

HBITMAP hBitmap[4];

static void update_lcd(HWND hWnd,int counter) {


    SendMessage(GetDlgItem(hWnd, IDC_seg0), STM_SETIMAGE,
        (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap[(counter/1000)%4]);
    SendMessage(GetDlgItem(hWnd, IDC_seg1), STM_SETIMAGE,
        (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap[(counter / 100) % 4]);
    SendMessage(GetDlgItem(hWnd, IDC_seg2), STM_SETIMAGE,
        (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap[(counter / 10) % 4]);
    SendMessage(GetDlgItem(hWnd, IDC_seg3), STM_SETIMAGE,
        (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap[(counter / 1) % 4] );
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg,
    WPARAM wParam, LPARAM lParam)
{
    switch (iMsg) {

    case WM_CREATE: {
        l_hWnd = hWnd;

        return 0;
    }
    case WM_TIMER:
    {
        counter++;
        update_lcd(hWnd,counter);
    }
    break;

    case WM_COMMAND: {
        switch (wParam) {
        case IDOK:
        {

            // HBITMAP hBitmap = (HBITMAP)::LoadImage(NULL, L"bitmap7.bmp", IMAGE_BITMAP,0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
            // HBITMAP hBitmap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SEG4));

        }
            break;

        case IDCANCEL: {
            PostQuitMessage(0);
            break;
        }
        }
        return 0;
    }
                  /* Perform initialization after all child windows have been created */
    case WM_INITDIALOG: {
        hBitmap[0] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SEG1));
        hBitmap[1] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SEG2));
        hBitmap[2] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SEG3));
        hBitmap[3] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SEG4));

        UINT ret = SetTimer(hWnd, ID_TIMER, 50, NULL);
        if (ret == 0)
            MessageBox(hWnd, L"Could not SetTimer()!", L"Error", MB_OK | MB_ICONEXCLAMATION);
        return 0;
    }

    case WM_DESTROY: {
        KillTimer(hWnd, ID_TIMER);
        PostQuitMessage(0);
        return 0;
    }

    }
    return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

