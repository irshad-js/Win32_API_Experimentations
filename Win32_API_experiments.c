#include <stdint.h>
#include "framework.h"
#include "resource.h"
#include "BSP.h"
#include <mmsystem.h>

// Both works well
// HBITMAP hBitmap = (HBITMAP)::LoadImage(NULL, L"bitmap7.bmp", IMAGE_BITMAP,0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
// HBITMAP hBitmap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SEG4));

static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg,
    WPARAM wParam, LPARAM lParam);

static int last_lcd_display_value;

enum {
    NO_LED = 10,
    LEFT_LED,
    RIGHT_LED,
    CENTER_LED
};

const int ID_TIMER = 1;
static int counter = 0;
HBITMAP hBitmap[14];
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

    hBitmap[NO_LED] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SST1A));
    hBitmap[LEFT_LED] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_Left_LED));
    hBitmap[RIGHT_LED] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_Right_LED));
    hBitmap[CENTER_LED] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_Alive_LED));
}

static void initialize_timer(HWND hWnd) {
    UINT ret = SetTimer(hWnd, ID_TIMER, 250, NULL);
    if (ret == 0)
        MessageBox(hWnd, L"Could not SetTimer()!", L"Error", MB_OK | MB_ICONEXCLAMATION);
}

/*..........................................................................*/
/* thread function for running the application main_gui() */
static DWORD WINAPI appThread(LPVOID par) {
    (void)par;         /* unused parameter */
    return main_gui(); /* run the application */

}


static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg,
    WPARAM wParam, LPARAM lParam)
{
    switch (iMsg) {

    case WM_TIMER:
    {
        counter++;
        if (counter == 9999)counter = 0;
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
        /* --> Spawn the application thread to run main_gui() */
        CreateThread(NULL, 0, &appThread, NULL, 0, NULL);

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

//    PlaySound("C:\\Users\\jsirs\\Desktop\\Other_Temp\\Win32_API_experiments\\scoop.mp3", NULL, SND_SYNC);
//    PlaySoundW(L"C:\\Users\\jsirs\\Desktop\\Other_Temp\\Win32_API_experiments\\scoop.mp3", NULL, SND_LOOP);
//    PlaySound(TEXT("song"), GetModuleHandle(NULL), SND_RESOURCE | SND_LOOP | SND_ASYNC);


void BSP_Set_TIMER(int milliseconds) {
    initialize_timer(local_hwnd_instance);
}
/*..........................................................................*/
void BSP_sleep(uint32_t ticks) {
    Sleep(ticks);
}

void BSP_init(void) {
    initialize_lcd();
}

void BSP_terminate(int result) {
    //empty
}

void BSP_LED_Activity(int activity) {

    if (activity == 0) {
        SendMessage(GetDlgItem(local_hwnd_instance, IDC_sst1), STM_SETIMAGE,
            (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap[NO_LED]);
    }
    else if (activity == 1) {
        SendMessage(GetDlgItem(local_hwnd_instance, IDC_sst1), STM_SETIMAGE,
            (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap[LEFT_LED]);
    }
    else if (activity == 2) {
        SendMessage(GetDlgItem(local_hwnd_instance, IDC_sst1), STM_SETIMAGE,
            (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap[RIGHT_LED]);
    }
    else if (activity == 3) {
        SendMessage(GetDlgItem(local_hwnd_instance, IDC_sst1), STM_SETIMAGE,
            (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap[CENTER_LED]);
    }

}

void BSP_DisplayNumber(int count) {
    SendMessage(GetDlgItem(local_hwnd_instance, IDC_seg0), STM_SETIMAGE,
        (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap[((UINT)(count / 1000) % 10)]);
    SendMessage(GetDlgItem(local_hwnd_instance, IDC_seg1), STM_SETIMAGE,
        (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap[((UINT)(count / 100) % 10)]);
    SendMessage(GetDlgItem(local_hwnd_instance, IDC_seg2), STM_SETIMAGE,
        (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap[((UINT)(count / 10) % 10)]);
    SendMessage(GetDlgItem(local_hwnd_instance, IDC_seg3), STM_SETIMAGE,
        (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap[((UINT)(count / 1) % 10)]);

    last_lcd_display_value = count;
}