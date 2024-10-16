// DiShui_App.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "DiShui_App.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例，当前窗口句柄
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

//主窗口句柄
HWND GloBal_hWnd;

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);

// 消息处理函数
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
// 关于处理函数
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

   
    // 初始化全局字符串
    // 标题名
    wcscpy_s(szTitle, sizeof(szTitle) / sizeof(wchar_t),L"HelloWorld");
    // 类名
    wcscpy_s(szWindowClass, sizeof(szWindowClass) / sizeof(wchar_t), L"My_Class_Name");
   
    // 调用注册窗口函数
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	// 创建按钮函数   
	CreateWindow(TEXT("BUTTON"), TEXT("打开"), WS_VISIBLE | WS_CHILD, 50, 50, 80, 25, GloBal_hWnd, (HMENU)1, hInst, nullptr);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DISHUIAPP));

    MSG msg;
    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg); //  翻译消息
            DispatchMessage(&msg);  //  分发消息
        }
    }

    return (int) msg.wParam;
}

//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    // 窗口类，窗口的一些基本信息
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DISHUIAPP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DISHUIAPP);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
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
      400, 400, 550, 550, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   GloBal_hWnd = hWnd;

   return TRUE;
}

// 绘图逻辑，由WM_PAINT调用;
void DrawContent(HDC hdc, RECT* rect) {
	// 处理所有绘图逻辑
	FillRect(hdc, rect, (HBRUSH)(COLOR_WINDOW + 1));
	TextOut(hdc, 50, 50, TEXT("Hello, Windows!"), 15);
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
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
            OutputDebugStringF("wmId-----> %d \n", wmId);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
                return 0;
            case IDM_EXIT:
                DestroyWindow(hWnd);

                break;
                return 0;
            // 如果都没有则调用DefWindowProc让系统处理.
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
			RECT rect;
			GetClientRect(hWnd, &rect);

            // 调用绘制函数
            DrawContent(hdc, &rect);

            EndPaint(hWnd, &ps);
        }
        return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
		OutputDebugStringF("wmId-----> %d \n", message);
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
	OutputDebugStringF("About_message------> %d\n", message);

    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            OutputDebugStringF("LOWORD(wParam)----->%d\n", LOWORD(wParam));
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
