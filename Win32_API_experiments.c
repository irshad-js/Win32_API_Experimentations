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
void DisplayTwoDigits(HDC hdc, int iNumber, BOOL fSuppress);

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
    static HBRUSH hBrushRed;
    static int cxClient, cyClient;
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rc;

    switch (iMsg) {

    case WM_TIMER:
        InvalidateRect(hWnd, NULL, TRUE);
        return 0;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        GetClientRect(hWnd, &rc);
        //create memory device context
        HDC memdc = CreateCompatibleDC(hdc);

        //create bitmap
        HBITMAP hbitmap = CreateCompatibleBitmap(memdc, 3100, 3100);

        //select bitmap in to memory device context
        SelectObject(memdc, hbitmap);
        SetMapMode(memdc, MM_ISOTROPIC);
        SetWindowExtEx(memdc, 5000/2,6000/2, NULL);
        SetViewportExtEx(memdc, 3165, 3165, NULL);
        SetViewportOrgEx(memdc, 30, 0, NULL);
        SetWindowOrgEx(memdc, 0, 0, NULL);
        SelectObject(memdc, GetStockObject(BLACK_PEN));

        DisplayTwoDigits(memdc, 88, 0);

        //BitBlt(GetDC(hWnd), 160, 170, 500, 500, memdc, 0, 0, SRCPAINT);
        StretchBlt(GetDC(hWnd), 165, 220, 1000, 1000, memdc, -30, -675, 3550, 3750, SRCPAINT);

        DeleteObject(hbitmap);
        DeleteDC(memdc);

        EndPaint(hWnd, &ps);
        return 0;

    case WM_INITDIALOG: {
        
        local_hwnd_instance = hWnd;
        /* --> Spawn the application thread to run main_gui() */
        
        //CreateThread(NULL, 0, &appThread, NULL, 0, NULL);

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




void DisplayDigit(HDC hdc, int iNumber)
{
    static BOOL fSevenSegment[10][15] = {
    1, 1, 1, 0, 1, 1, 1,0,0,0,0,0,0,0,0, // 0
    0, 0, 1, 0, 0, 1, 0,0,0,0,0,0,0,0,0, // 1
    1, 0, 1, 1, 1, 0, 1,0,0,0,0,0,0,0,0, // 2
    1, 0, 1, 1, 0, 1, 1,0,0,0,0,0,0,0,0, // 3
    0, 1, 1, 1, 0, 1, 0,0,0,0,0,0,0,0,0, // 4
    1, 1, 0, 1, 0, 1, 1,0,0,0,0,0,0,0,0, // 5
    1, 1, 0, 1, 1, 1, 1,0,0,0,0,0,0,0,0, // 6
    1, 0, 1, 0, 0, 1, 0,0,0,0,0,0,0,0,0, // 7
    1, 1, 1, 0, 1, 1, 1,1,1,1,1,1,1,1,1, // 8
    1, 1, 1, 1, 0, 1, 1,0,0,0,0,0,0,0,0 }; // 9

    static POINT ptSegment[15][6] = {


39, 3,
234, 3,
200,50,
68,50,
68,50,
68,50,

33, 12,
62, 57,
51, 201,
35, 225,
20, 201,
20, 201,

240,11,
228, 201,
209, 225,
194, 201,
205, 57,
205, 57,

// normal center // wrong - need change
7, 36, 11, 32, 31, 32, 35, 36, 31, 40, 11, 40,

33, 241,
47, 264,
38, 407,
2, 454,
16, 264,
16, 264,

191, 264,
209, 241,
222, 264,
209,454,
180, 407,
180, 407,

43,416,
173, 417,
203, 462,
9, 462,
9, 462,
9, 462,

//left middle
39,233,
57,210,
92,210,
115,233,
89,256,
55,256,

//right middle
128,233,
153,210,
189,210,
202,233,
184,256,
149,256,

// top left small
72,59,
80,59,
105,134,
115,219,
97,198,
67,112,

//top right small
186,59,
194,59,
191,112,
153,198,
130,219,
151,134,

// top middle small
120,59,
148,59,
141,134,
122,207,
114,134,
114,134,


// bottom left small
52,354,
92,266,
112,247,
91,330,
58,407,
47,407,

// bottom center
96,407,
123,407,
128,330,
120,258,
101,330,
101,330,

//bottom right
162,407,
171,407,
175,354,
146,266,
128,247,
139,330,

    };
    int iSeg;
    for (iSeg = 0; iSeg < 15; iSeg++)
        if (fSevenSegment[iNumber][iSeg])
            Polygon(hdc, ptSegment[iSeg], 6);
}
void DisplayTwoDigits(HDC hdc, int iNumber, BOOL fSuppress)
{
    OffsetViewportOrgEx(hdc, -0, 0, NULL);
    DisplayDigit(hdc, iNumber / 10);
    OffsetViewportOrgEx(hdc, 260, 0, NULL);
    DisplayDigit(hdc, 8);    
    OffsetViewportOrgEx(hdc, 260, 0, NULL);
    DisplayDigit(hdc, 8);    
    OffsetViewportOrgEx(hdc, 260, 0, NULL);
    DisplayDigit(hdc, 8);    
    OffsetViewportOrgEx(hdc, 260, 0, NULL);
    DisplayDigit(hdc, 8);    
    OffsetViewportOrgEx(hdc, 260, 0, NULL);
    DisplayDigit(hdc, 8);    
    OffsetViewportOrgEx(hdc, 260, 0, NULL);
    DisplayDigit(hdc, 8);

    OffsetViewportOrgEx(hdc, -5, 660, NULL);
    DisplayDigit(hdc, 8);
    OffsetViewportOrgEx(hdc, -260, 0, NULL);
    DisplayDigit(hdc, 8);
    OffsetViewportOrgEx(hdc, -260, 0, NULL);
    DisplayDigit(hdc, 8);
    OffsetViewportOrgEx(hdc, -260, 0, NULL);
    DisplayDigit(hdc, 8);
    OffsetViewportOrgEx(hdc, -260, 0, NULL);
    DisplayDigit(hdc, 8);
    OffsetViewportOrgEx(hdc, -260, 0, NULL);
    DisplayDigit(hdc, 8);
    OffsetViewportOrgEx(hdc, -260, 0, NULL);
    DisplayDigit(hdc, 8);
}

#endif


#if 0
#include <windows.h>
#define ID_TIMER 1
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


void DisplayDigit(HDC hdc, int iNumber)
{
    static BOOL fSevenSegment[10][15] = {
    1, 1, 1, 0, 1, 1, 1,0,0,0,0,0,0,0,0, // 0
    0, 0, 1, 0, 0, 1, 0,0,0,0,0,0,0,0,0, // 1
    1, 0, 1, 1, 1, 0, 1,0,0,0,0,0,0,0,0, // 2
    1, 0, 1, 1, 0, 1, 1,0,0,0,0,0,0,0,0, // 3
    0, 1, 1, 1, 0, 1, 0,0,0,0,0,0,0,0,0, // 4
    1, 1, 0, 1, 0, 1, 1,0,0,0,0,0,0,0,0, // 5
    1, 1, 0, 1, 1, 1, 1,0,0,0,0,0,0,0,0, // 6
    1, 0, 1, 0, 0, 1, 0,0,0,0,0,0,0,0,0, // 7
    1, 1, 1, 0, 1, 1, 1,1,1,1,1,1,1,1,1, // 8
    1, 1, 1, 1, 0, 1, 1,0,0,0,0,0,0,0,0 }; // 9

    static POINT ptSegment[15][6] = {


39, 3,
234, 3,
200,50,
68,50,
68,50,
68,50,

33, 12,
62, 57,
51, 201,
35, 225,
20, 201,
20, 201,

240,11,
228, 201,
209, 225,
194, 201,
205, 57,
205, 57,

// normal center // wrong - need change
7, 36, 11, 32, 31, 32, 35, 36, 31, 40, 11, 40,

33, 241,
47, 264,
38, 407,
2, 454,
16, 264,
16, 264,

191, 264,
209, 241,
222, 264,
209,454,
180, 407,
180, 407,

43,416,
173, 417,
203, 462,
9, 462,
9, 462,
9, 462,

//left middle
39,233,
57,210,
92,210,
115,233,
89,256,
55,256,

//right middle
128,233,
153,210,
189,210,
202,233,
184,256,
149,256,

// top left small
72,59,
80,59,
105,134,
115,219,
97,198,
67,112,

//top right small
186,59,
194,59,
191,112,
153,198,
130,219,
151,134,

// top middle small
120,59,
148,59,
141,134,
122,207,
114,134,
114,134,


// bottom left small
52,354,
92,266,
112,247,
91,330,
58,407,
47,407,

// bottom center
96,407,
123,407,
128,330,
120,258,
101,330,
101,330,

//bottom right
162,407,
171,407,
175,354,
146,266,
128,247,
139,330,

    };
    int iSeg;
    for (iSeg = 0; iSeg < 15; iSeg++)
        if (fSevenSegment[iNumber][iSeg])
            Polygon(hdc, ptSegment[iSeg], 6);
}
void DisplayTwoDigits(HDC hdc, int iNumber, BOOL fSuppress)
{

    DisplayDigit(hdc, iNumber / 10);
    OffsetViewportOrgEx(hdc, 220, 0, NULL);
    DisplayDigit(hdc, 7);
    OffsetViewportOrgEx(hdc, 220, 0, NULL);
    DisplayDigit(hdc, 1);
    OffsetViewportOrgEx(hdc, 220, 0, NULL);
    DisplayDigit(hdc, 7);
    OffsetViewportOrgEx(hdc, 220, 0, NULL);
    DisplayDigit(hdc, 1);
    OffsetViewportOrgEx(hdc, 220, 0, NULL);
    DisplayDigit(hdc, 7);
    OffsetViewportOrgEx(hdc, 220, 0, NULL);
    DisplayDigit(hdc, 1);

    OffsetViewportOrgEx(hdc, 0, 500, NULL);
    DisplayDigit(hdc, 1);
    OffsetViewportOrgEx(hdc, -220, 0, NULL);
    DisplayDigit(hdc, 1);
    OffsetViewportOrgEx(hdc, -220, 0, NULL);
    DisplayDigit(hdc, 1);
    //OffsetViewportOrgEx(hdc, -220, 0, NULL);
    //DisplayDigit(hdc, 1);
    //OffsetViewportOrgEx(hdc, -220, 0, NULL);
    //DisplayDigit(hdc, 1);
    //OffsetViewportOrgEx(hdc, -220, 0, NULL);
    //DisplayDigit(hdc, 1);
    //OffsetViewportOrgEx(hdc, -220, 0, NULL);
    //DisplayDigit(hdc, 1);
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
        /*
        hdc = BeginPaint(hwnd, &ps);
        SetMapMode(hdc, MM_ISOTROPIC);
        SetWindowExtEx(hdc, 800, 800, NULL);
        SetViewportExtEx(hdc, 130, 130, NULL);
        SetViewportOrgEx(hdc, 50, 50, NULL);
        SelectObject(hdc, GetStockObject(NULL_PEN));
        SelectObject(hdc, hBrushRed);
        DisplayTime(hdc, f24Hour, fSuppress);
        EndPaint(hwnd, &ps);
        */

        RECT rc;
        hdc = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &rc);
        //create memory device context
        HDC memdc = CreateCompatibleDC(hdc);

        //create bitmap
        HBITMAP hbitmap = CreateCompatibleBitmap(memdc, rc.right, rc.bottom);

        //select bitmap in to memory device context
        SelectObject(memdc, hbitmap);
        SetMapMode(memdc, MM_ISOTROPIC);
        SetWindowExtEx(memdc, rc.right, rc.bottom, NULL);
        SetViewportExtEx(memdc, rc.right, rc.bottom, NULL);
        SetViewportOrgEx(memdc, 125, 0, NULL);
        SetWindowOrgEx(memdc, 0, 0, NULL);
        DisplayTwoDigits(memdc, 88, 0);

        //BitBlt(GetDC(hWnd), 160, 170, 500, 500, memdc, 0, 0, SRCPAINT);
        StretchBlt(GetDC(hwnd), 160, 170, 800, 800, memdc, -1000, -800, 3000, 3000, SRCCOPY);

        DeleteObject(hbitmap);
        DeleteDC(memdc);

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