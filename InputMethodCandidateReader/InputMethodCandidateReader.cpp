//InputMethodCandidateReader.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "InputMethodCandidateReader.h"


#include "CandidateReader.h"


#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
HFONT g_hfont = 0;

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_INPUTMETHODCANDIDATEREADER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_INPUTMETHODCANDIDATEREADER));

    MSG msg;

    // 主消息循环: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    ImeUi_Uninitialize();

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_INPUTMETHODCANDIDATEREADER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_INPUTMETHODCANDIDATEREADER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   g_hfont = CreateFont(18, 0, 0, 0, FW_NORMAL,
       FALSE, FALSE, FALSE, DEFAULT_CHARSET,
       OUT_TT_PRECIS, CLIP_TT_ALWAYS, ANTIALIASED_QUALITY,
       DEFAULT_PITCH, _T("宋体"));

   HWND hEdit = CreateWindowEx(0, L"Edit", L"", WS_BORDER | ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_VISIBLE | WS_CHILD, 50, 20, 300, 25, hWnd, NULL, hInstance, NULL);
   if (hEdit)
   {
       SetFocus(hEdit);
       SendMessage(hEdit, WM_SETFONT, (WPARAM)g_hfont, (LPARAM)0);
   }

   HWND hEdit2 = CreateWindowEx(0, L"Edit", L"1.在文本框中开启输入法，输入编码可见效果;\r\n\r\n2.TSF输入法状态、编码、候选获取；\r\n\r\n3.支持\"选中候选\"识别，中文输入法中使用←→可改变\"选中候选\"\r\n\r\n4.候选翻页与输入法设置相同",
       ES_AUTOVSCROLL | WS_VISIBLE | WS_CHILD | ES_READONLY | ES_MULTILINE | ES_WANTRETURN, 50, 80, 300, 300, hWnd, NULL, hInstance, NULL);
   if (hEdit2)
   {
       SendMessage(hEdit2, WM_SETFONT, (WPARAM)g_hfont, (LPARAM)0);
   }


   ImeUi_Initialize(hWnd);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...

            HFONT holdf = (HFONT)SelectObject(hdc, g_hfont);


            int px = 380;
            int py = 20;

            wchar_t szt[512];
            swprintf_s(szt, L"转换候选列表:       %s", g_Candidate.dwState == IMEUI_STATE_ON ? L"开启" : L"关闭");
            TextOut(hdc, px, py, szt, wcslen(szt));  py += 22;

            swprintf_s(szt, L"输入编码:            %s", g_Candidate.szComposing);
            TextOut(hdc, px, py, szt, wcslen(szt));  py += 22;

            swprintf_s(szt, L"总候选个数:          %d", g_Candidate.uCount);
            TextOut(hdc, px, py, szt, wcslen(szt));  py += 22;

            swprintf_s(szt, L"总候选页数:          %d", g_Candidate.uPageCnt);
            TextOut(hdc, px, py, szt, wcslen(szt));  py += 22;

            swprintf_s(szt, L"当前候选页序号:      %d", g_Candidate.uCurrentPage);
            TextOut(hdc, px, py, szt, wcslen(szt));  py += 22;

            swprintf_s(szt, L"当前候选序号:        %d", g_Candidate.uIndex);
            TextOut(hdc, px, py, szt, wcslen(szt));  py += 22;

            swprintf_s(szt, L"当前页首候选序号:    %d", g_Candidate.dwPageStart);
            TextOut(hdc, px, py, szt, wcslen(szt));  py += 22;

            swprintf_s(szt, L"当前页候选个数:      %d", g_Candidate.dwPageSize);
            TextOut(hdc, px, py, szt, wcslen(szt));  py += 22;

            py += 22;
            szt[0] = 0;
            for (UINT i = 0; i < g_Candidate.dwPageSize && i < ARRAYSIZE(g_Candidate.szCandidate); i++)
            {
                wchar_t cur[64];
                swprintf_s(cur, L"%d.%s  ", i + 1, g_Candidate.szCandidate[i]);
                wcscat_s(szt, cur);
            }
            TextOut(hdc, px, py, szt, wcslen(szt));  py += 22;
            py += 22;

            swprintf_s(szt, L"当前候选:   %s", g_Candidate.szCandidate[g_Candidate.uIndex - g_Candidate.dwPageStart]);
            TextOut(hdc, px, py, szt, wcslen(szt));  py += 22;

            SelectObject(hdc, holdf);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
