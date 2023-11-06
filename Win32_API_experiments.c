#include "framework.h"
#include "resource.h"
#include "Win32_API_experiments.h"

// Both works well
// HBITMAP hBitmap = (HBITMAP)::LoadImage(NULL, L"bitmap7.bmp", IMAGE_BITMAP,0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
// HBITMAP hBitmap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SEG4));

static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg,
    WPARAM wParam, LPARAM lParam);

const int ID_TIMER = 1;
static int counter = 0;
HBITMAP hBitmap[11];
static HWND local_hwnd_instance = NULL;

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
)
{
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, WndProc);
}

void API_Update_Lcd(int counter) {

    SendMessage(GetDlgItem(local_hwnd_instance, IDC_seg0), STM_SETIMAGE,
        (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap[((UINT)(counter/1000)%10)]);
    SendMessage(GetDlgItem(local_hwnd_instance, IDC_seg1), STM_SETIMAGE,
        (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap[((UINT)(counter / 100) % 10)]);
    SendMessage(GetDlgItem(local_hwnd_instance, IDC_seg2), STM_SETIMAGE,
        (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap[((UINT)(counter / 10) % 10)]);
    SendMessage(GetDlgItem(local_hwnd_instance, IDC_seg3), STM_SETIMAGE,
        (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap[((UINT)(counter / 1) % 10)] );
}

static void initialize_lcd(void) {
    hBitmap[0] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SEG0));
    hBitmap[1] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SEG1));
    hBitmap[2] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SEG2));
    hBitmap[3] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SEG3));
    hBitmap[4] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SEG4));
    hBitmap[5] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SEG5));
    hBitmap[6] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SEG6));
    hBitmap[7] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SEG7));
    hBitmap[8] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SEG8));
    hBitmap[9] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SEG9));
}

static void initialize_timer(HWND hWnd) {
    UINT ret = SetTimer(hWnd, ID_TIMER, 25, NULL);
    if (ret == 0)
        MessageBox(hWnd, L"Could not SetTimer()!", L"Error", MB_OK | MB_ICONEXCLAMATION);
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg,
    WPARAM wParam, LPARAM lParam)
{
    switch (iMsg) {

    case WM_TIMER:
    {
        counter++;
        if (counter == 9999)counter = 0;
        API_Update_Lcd(counter);
    }
    break;

    case WM_COMMAND: {
        switch (wParam) {
        case IDOK:
        {
            PostQuitMessage(0);
            break;
        }
        case IDCANCEL: {
            PostQuitMessage(0);
            break;
        }
        }
        return 0;
    }
    case WM_INITDIALOG: {
        local_hwnd_instance = hWnd;
        initialize_lcd();
        initialize_timer(local_hwnd_instance);
        return 0;
    }

    case WM_DESTROY: {
        KillTimer(local_hwnd_instance, ID_TIMER);
        PostQuitMessage(0);
        return 0;
    }

    }
    return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

