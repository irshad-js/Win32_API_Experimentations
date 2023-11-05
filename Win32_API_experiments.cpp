


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
    l_hInst = hInst;
    WNDCLASSEX wndclass;

    wndclass.cbSize = sizeof(wndclass);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = DLGWINDOWEXTRA;
    wndclass.hInstance = hInst;
    wndclass.hIcon = NULL;
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = L"First";
    wndclass.hIconSm = NULL;

    RegisterClassEx(&wndclass);

    return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, WndProc);
}


static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg,
    WPARAM wParam, LPARAM lParam)
{
    switch (iMsg) {

    case WM_CREATE: {
        l_hWnd = hWnd; /* save the window handle */

        /* initialize the owner-drawn buttons...
        * NOTE: must be done *before* the first drawing of the buttons,
        * so WM_INITDIALOG is too late.
        */
        return 0;
    }
    case WM_COMMAND: {
        switch (wParam) {
        case IDOK:
        {

            // HBITMAP hBitmap = (HBITMAP)::LoadImage(NULL, L"bitmap7.bmp", IMAGE_BITMAP,0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);

            HBITMAP hBitmap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SEG2x));
            SendMessage(GetDlgItem(hWnd, IDC_seg0), STM_SETIMAGE,
                (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);
            SendMessage(GetDlgItem(hWnd, IDC_seg1), STM_SETIMAGE,
                (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);

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

        return 0;
    }

    case WM_DESTROY: {
        PostQuitMessage(0);
        return 0;
    }

    }
    return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

