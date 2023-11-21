#if 1

#include <stdint.h>
#include "framework.h"
#include "resource.h"
#include "BSP.h"
#include <mmsystem.h>

// Both works well
// HBITMAP hBitmap = (HBITMAP)::LoadImage(NULL, L"bitmap7.bmp", IMAGE_BITMAP,0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
// HBITMAP hBitmap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SEG4));

void DisplayTime(HDC hdc, BOOL f24Hour, BOOL fSuppress);
void DisplayDigit(HDC hdc, int iNumber);

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
    static BOOL f24Hour = 0;
    static BOOL fSuppress =0;
    static HBRUSH hBrushRed;
    static int cxClient, cyClient;
    HDC hdc;
    PAINTSTRUCT ps;
    TCHAR szBuffer[2];
    RECT rc;
    POINT aptStar[6] = { 50,2, 2,98, 98,33, 2,33, 98,98, 50,2 };

    switch (iMsg) {

    case WM_TIMER:
        InvalidateRect(hWnd, NULL, TRUE);
        return 0;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        int w = 100;
        int h = 100;

        //create memory device context
        HDC memdc = CreateCompatibleDC(0);

        //create bitmap
        HBITMAP hbitmap = CreateCompatibleBitmap(memdc, 455, 455);

        //select bitmap in to memory device context
       SelectObject(memdc, hbitmap);

        //begine drawing:
        HPEN hpen = CreatePen(0, 4, 255);
        SelectObject(memdc, hpen);
        DisplayDigit(memdc, 5);

        BitBlt(GetDC(hWnd), 0, 0, 455, 455, memdc, 0, 0, SRCCOPY);

//        Rectangle(memdc, 10, 10, 90, 90);

        DeleteObject(hbitmap);
        DeleteObject(hpen);
        DeleteDC(memdc);

        EndPaint(hWnd, &ps);
        return 0;

    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        return 0;

    case WM_INITDIALOG: {
        hBrushRed = CreateSolidBrush(RGB(0, 0, 0));
        local_hwnd_instance = hWnd;
        /* --> Spawn the application thread to run main_gui() */
        
        //CreateThread(NULL, 0, &appThread, NULL, 0, NULL);

        return 0;
    }

    case WM_DESTROY: {
        KillTimer(local_hwnd_instance, ID_TIMER);
        PostQuitMessage(0);
        DeleteObject(hBrushRed);
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

#endif



#define length_reduction 37
#define height_reduction 12
#define offset 20



void DisplayDigit(HDC hdc, int iNumber)
{
    static BOOL fSevenSegment[10][7] = {
    1, 1, 1, 0, 1, 1, 1, // 0
    0, 0, 1, 0, 0, 1, 0, // 1
    1, 0, 1, 1, 1, 0, 1, // 2
    1, 0, 1, 1, 0, 1, 1, // 3
    0, 1, 1, 1, 0, 1, 0, // 4
    1, 1, 0, 1, 0, 1, 1, // 5
    1, 1, 0, 1, 1, 1, 1, // 6
    1, 0, 1, 0, 0, 1, 0, // 7
    1, 1, 1, 1, 1, 1, 1, // 8
    1, 1, 1, 1, 0, 1, 1 }; // 9

    #define variable_len_2 (99-offset - length_reduction)
    #define variable_len_3 (87-offset - length_reduction)
    #define variable_len_1 ((variable_len_2 - variable_len_3)/2)+variable_len_3
    

#define variable_height_1 (24 - height_reduction)

#define cord_x_2 26 - offset
#define cord_y_2 variable_height_1/2
#define cord_x_3 32 - offset
#define cord_y_3 variable_height_1

#define half_x_point_1_SEG1
#define half_y_point_1_SEG1

#define half_x_point_2_SEG1
#define half_y_point_2_SEG1
    
    static POINT ptSegment[7][6] = {


 37, 2, 
 38, 2, 
 31, 10, 
 10,10, 
 4, 2, 
 4, 2,

    2, 2, 
    10, 12, 
    10, 31,
    6, 35, 
    2, 31, 
    2, 11,

    40, 2,
    40, 31, 
    36, 35, 
    32, 31, 
    32, 12,
        32, 11,

    7, 36, 11, 32, 31, 32, 35, 36, 31, 40, 11, 40,
    
    6, 37, 
    10, 41, 
    10, 61, 
    10, 61,
    2, 70, 
    2, 41,

    36, 37, 
    40, 41, 
    40, 61, 
    40,70, 
    32, 61,
    32, 41,
    
    11,62, 
    11, 62, 
    31, 62, 
    31, 62,
    38, 70, 
    4, 70 };
    int iSeg;
    for (iSeg = 0; iSeg < 7; iSeg++)
        if (fSevenSegment[iNumber][iSeg])
            Polygon(hdc, ptSegment[iSeg], 6);
}
void DisplayTwoDigits(HDC hdc, int iNumber, BOOL fSuppress)
{
    if (!fSuppress || (iNumber / 10 != 0))
        DisplayDigit(hdc, iNumber / 10);
    OffsetWindowOrgEx(hdc, -42, 0, NULL);
    DisplayDigit(hdc, iNumber % 10);
    OffsetWindowOrgEx(hdc, -42, 0, NULL);
}

void DisplayTime(HDC hdc, BOOL f24Hour, BOOL fSuppress)
{
    SYSTEMTIME st;
    GetLocalTime(&st);
    st.wHour = 88;

    if (f24Hour)
        DisplayTwoDigits(hdc, st.wHour, fSuppress);
    else
        DisplayTwoDigits(hdc, (st.wHour %= 12) ? st.wHour : 12, fSuppress);

}

#if 0
#include <windows.h>
#define ID_TIMER 1
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR szCmdLine, int iCmdShow)
{
    static TCHAR szAppName[] = TEXT("DigClock");
    HWND hwnd;
    MSG msg;
    WNDCLASS wndclass;
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;
    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("Program requires Windows NT!"),
            szAppName, MB_ICONERROR);
        return 0;
    }
    hwnd = CreateWindow(szAppName, TEXT("Digital Clock"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static BOOL f24Hour, fSuppress;
    static HBRUSH hBrushRed;
    static int cxClient, cyClient;
    HDC hdc;
    PAINTSTRUCT ps;
    TCHAR szBuffer[2];
    switch (message)
    {
    case WM_CREATE:
        hBrushRed = CreateSolidBrush(RGB(0, 0, 0));
        SetTimer(hwnd, ID_TIMER, 1000, NULL);
        // fall through
    case WM_SETTINGCHANGE:
        GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITIME, szBuffer, 2);
        f24Hour = (szBuffer[0] == '1');
        GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITLZERO, szBuffer, 2);
        fSuppress = (szBuffer[0] == '0');
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        return 0;
    case WM_TIMER:
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        SetMapMode(hdc, MM_ISOTROPIC);
        SetWindowExtEx(hdc, 276, 72, NULL);
        SetViewportExtEx(hdc, cxClient, cyClient, NULL);
        SetWindowOrgEx(hdc, 138, 36, NULL);
        SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, NULL);
        SelectObject(hdc, GetStockObject(NULL_PEN));
        SelectObject(hdc, hBrushRed);
        DisplayTime(hdc, f24Hour, fSuppress);
        EndPaint(hwnd, &ps);
        return 0;
    case WM_DESTROY:
        KillTimer(hwnd, ID_TIMER);
        DeleteObject(hBrushRed);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

#endif