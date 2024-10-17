// DiShui_App.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "DiShui_App.h"

#define MAX_LOADSTRING 300

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


void CreatBuuton(HWND hwnd)
{
	HWND buuton = CreateWindowW(
		TEXT("BUTTON"),
		TEXT("按钮"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_DEFPUSHBUTTON | BS_TEXT,
		10,10,80,20,
		hwnd,
		(HMENU)1001,
        hInst,
		nullptr
	);
	HWND hwndCheckBox = CreateWindow(
		TEXT("button"),
		TEXT("复选框"),
		//WS_CHILD | WS_VISIBLE | BS_CHECKBOX | BS_AUTOCHECKBOX,						
		WS_CHILD | WS_VISIBLE | BS_CHECKBOX | BS_AUTOCHECKBOX,
		10, 40,
		80, 20,
		hwnd,
		(HMENU)1002,		//子窗口ID				
        hInst,
		nullptr);

	HWND hwndRadio = CreateWindow(
		TEXT("button"),
		TEXT("单选按钮"),
		//WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON | BS_AUTORADIOBUTTON,						
		WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
		10, 70,
		80, 20,
		hwnd,
		(HMENU)1003,		//子窗口ID				
        hInst,
		nullptr);


}

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

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      400, 400, 550, 550, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   GloBal_hWnd = hWnd;
   CreatBuuton(hWnd);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

        // 创建窗口消息
        case WM_CREATE:
        {
			OutputDebugStringF("WM_CREATE->message--->%d\n", message);
			CREATESTRUCTA* createstructa = (CREATESTRUCTA*)lParam;
			OutputDebugStringW((LPCWSTR)createstructa->lpszClass);
			OutputDebugStringW((LPCWSTR)createstructa->lpszName);
			return 0;
        }

        // 菜单栏消息
        case WM_COMMAND:
        {
            OutputDebugStringF("WM_COMMAND->message--->%d\n", message);
            int wmId = LOWORD(wParam);
            OutputDebugStringF("wmId-----> %d \n", wmId);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
            {
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            }
            case IDM_EXIT:
            {
                DestroyWindow(hWnd);
                break;
            }
            case 1001:
            {
                // 获取到按钮的句柄
                HWND hButton = (HWND)lParam;
                SetWindowText(hButton, TEXT("文本"));
                MessageBox(GloBal_hWnd, L"按钮已触发,按钮文本已被修改", L"TIP", MB_OK);
                break;
            }
            case 1002:
            {
                // 通过BM_GETCHECK获取到复选框的选取状态，1是被选中，0是未被选中，2是不确定状态。
                if (LOWORD(wParam) == 1002)
                {
                    HWND hwndCheckBox = (HWND)lParam;
                    int state = SendMessage(hwndCheckBox, BM_GETCHECK, 0, 0);
                    OutputDebugStringF("state----->%d\n", state);
                    if (state == BST_CHECKED)
                    {
                        MessageBox(GloBal_hWnd, L"复选框被选中", L"TIP",MB_OK);

                    }else if (state == BST_UNCHECKED)
                    {
						MessageBox(GloBal_hWnd, L"复选框未被选中", L"TIP", MB_OK);
                    }
                }
             
                break;
            }
            case 1003:
            {
                if (LOWORD(wParam))
                {
                    int state = SendMessage((HWND)lParam, BM_GETCHECK, 0, 0);
                    if (state == BST_CHECKED)
                    {
                        MessageBox(hWnd, TEXT("单选按钮被选中"), TEXT("TIP"), MB_OK);
                    }
                    break;
                }
            }
            // 如果都没有则调用DefWindowProc让系统处理.
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
    
        // 绘制消息
        case WM_PAINT:
        {
			OutputDebugStringF("WM_PAINT->message--->%d\n", message);
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            RECT rect;
            GetClientRect(hWnd, &rect);

            // 调用绘制函数

            EndPaint(hWnd, &ps);
             return 0;

        }

         // X 消息
        case WM_DESTROY:
        {
			OutputDebugStringF("WM_DESTROY->message--->%d\n", message);
			PostQuitMessage(0);
			OutputDebugStringF("wmId-----> %d \n", message);
			return 0;
        }
        // 移动窗口消息
        case WM_MOVE:
        {
            //OutputDebugStringF("WM_MOVE->message--->%d\n", message);
            // lParam 低位X坐标，高位Y坐标
            POINTS  points = MAKEPOINTS(lParam);
            //OutputDebugStringF("xPos---->%d yPos---->%d\n", points.x, points.y);
            return 0;
         }
        // 窗口大小
        case WM_SIZE:
        {
            OutputDebugStringF("WM_SIZE->message--->%d\n", message);
            // wParam
            // SIZE_MAXIMIZED-->最大化
            // SIZE_MINIMIZED--最小化
            // lParam 低位->新的宽度，高位->新的高度
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);
            OutputDebugStringF("width--->%d height--->%d\n", width, height);
            return 0;
        }

        // 鼠标左键被单机消息
         case WM_LBUTTONDOWN:
        {
            MessageBox(GloBal_hWnd, TEXT("鼠标左键被点击"), TEXT("TIP"), MB_OK);
            return 0;
         }

        // 鼠标左键抬起
        case WM_LBUTTONUP:
        {
			MessageBox(GloBal_hWnd, TEXT("鼠标左键已抬起"), TEXT("TIP"), MB_OK);

			return 0;
         }

        // 鼠标右键被点击
        case WM_RBUTTONDOWN:
        {
            MessageBox(GloBal_hWnd, TEXT("鼠标右键已按下"), TEXT("TIP"), MB_OK);

            return 0;
        }

        // 鼠标右键被抬起
        case WM_RBUTTONUP:
        {
            MessageBox(GloBal_hWnd, TEXT("鼠标右键已抬起"), TEXT("TIP"), MB_OK);

            return 0;
        }
        // 移动光标消息
        case WM_MOUSEMOVE:
        {
            //lParam 低位X坐标，高位Y坐标
            //OutputDebugStringF("%d\n", wParam);
            UINT xPors = LOWORD(lParam);
            UINT yPors = HIWORD(lParam);
            //OutputDebugStringF("xPors----->%d yPors------>%d\n", xPors, yPors);

            return 0;
        }
        // 键盘消息，除了ALT
        case WM_KEYDOWN:
        {
            // 用于存放按键名称的缓冲区
            WCHAR keyName[256];
            GetKeyNameText(lParam, keyName, 256);
            MessageBox(GloBal_hWnd, keyName, L"TIP", MB_OK);

            return 0;
        }
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
