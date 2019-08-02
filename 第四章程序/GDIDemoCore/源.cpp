//---------------------------------------------------------------------------
// 程序名称：GameWnd
// 2019年7月 Create by 侯金成
// 描述：游戏程序框架
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// 描述：包含程序依赖的头文件
//---------------------------------------------------------------------------
#include <windows.h>
#include <time.h>
#pragma comment(lib,"winmm.lib")
//---------------------------------------------------------------------------
// 描述：定义一些辅助宏
//---------------------------------------------------------------------------
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE L"游戏程序框架"

//---------------------------------------------------------------------------
// 描述：全局变量的声明
//---------------------------------------------------------------------------
HDC g_hdc = NULL;//全局设备环境句柄
HPEN g_hPen[7] = { 0 };//定义画笔句柄的数组
HBRUSH g_hBrush[7] = { 0 };//定义画刷句柄的数组
int g_iPenStyle[7] = { PS_SOLID ,PS_DASH,PS_DOT ,PS_DASHDOT ,PS_DASHDOTDOT,
PS_NULL ,PS_INSIDEFRAME };//定义并初始化画笔样式的数组
int g_iBrushStyle[6] = { HS_BDIAGONAL ,HS_CROSS ,HS_DIAGCROSS,HS_FDIAGONAL,
HS_HORIZONTAL,HS_VERTICAL};//定义并初始化画刷样式的数组

//---------------------------------------------------------------------------
// 描述：全局函数声明
//---------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL Game_Init(HWND hWnd);//资源初始化
VOID Game_Paint(HWND hWnd);//进行绘图
BOOL Game_CleanUp(HWND hWnd);//资源清理

//---------------------------------------------------------------------------
// 描述：主函数定义
//---------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//设计一个完整的窗口类
	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);//设置结构体的字节数
	wndClass.style = CS_HREDRAW | CS_VREDRAW;//设置窗口的样式
	wndClass.lpfnWndProc = WndProc;//指向回调函数的远指针
	wndClass.cbClsExtra = 0;//窗口类附加内存
	wndClass.cbWndExtra = 0;//窗口附加内存
	wndClass.hInstance = hInstance;//指定包含窗口过程的程序的实例句柄
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"icon.ico", IMAGE_ICON,
		0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = L"GameWindow";

	//注册窗口类
	if (!RegisterClassEx(&wndClass))
	{
		return -1;
	}

	//正式创建窗口
	HWND hWnd = CreateWindow(L"GameWindow", WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

	//窗口的移动、显示与更新
	MoveWindow(hWnd, 250, 80, WINDOW_WIDTH, WINDOW_HEIGHT, true);//调整窗口显示位置
	ShowWindow(hWnd, nCmdShow);//显示窗口
	UpdateWindow(hWnd);//更新窗口

	//资源初始化，若初始失败，则返回false
	if (!Game_Init(hWnd))
	{
		MessageBox(hWnd, L"资源初始化失败", L"消息窗口", 0);
		return false;
	}
	
	//窗口初始化后，播放音乐
	PlaySound(L"Dia Frampton - Walk Away.flac", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    //消息循环过程
	MSG msg = { 0 };//定义并初始化msg
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))//查看应用程序消息队列，派发消息
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	//窗口类的注销
	UnregisterClass(L"GameWindow", wndClass.hInstance);
	return 0;
}

//---------------------------------------------------------------------------
// 描述：窗口过程函数WndProc，对窗口消息进行处理
//---------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;//声明一个PAINTSTRUCT结构体来记录一些绘图信息
	switch (nMsg)
	{
	case WM_PAINT:
		g_hdc = BeginPaint(hWnd, &paintStruct);//指定窗口进行绘图操作的准备，用与绘图有关的信息填充到paintStruct
		Game_Paint(hWnd);
		EndPaint(hWnd, &paintStruct);//标记窗口绘图的结束
		ValidateRect(hWnd, NULL);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		Game_CleanUp(hWnd);//调用自定义的资源清理函数，在窗口退出前对资源进行清理
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, nMsg, wParam, lParam);
	}
	return 0;
}

//---------------------------------------------------------------------------
// 描述：资源初始化函数Game_Init，进行一些简单的初始化
//---------------------------------------------------------------------------
BOOL Game_Init(HWND hWnd)
{
	g_hdc = GetDC(hWnd);//获取设备环境句柄
	srand((unsigned)time(NULL));//初始化随机种子
	//随机初始化画笔和画刷的颜色值
	for (int i = 0; i <=6; i++)
	{
		g_hPen[i] = CreatePen(g_iPenStyle[i], 1, RGB(rand() % 256, 
			rand() % 256, rand() % 256));
		if (i == 6)
			g_hBrush[i] = CreateSolidBrush(RGB(rand() % 256,
				rand() % 256, rand() % 256));
		else
			g_hBrush[i] = CreateHatchBrush(g_iBrushStyle[i], RGB(rand() % 256,
				rand() % 256, rand() % 256));
	}
	Game_Paint(hWnd);//执行绘制操作
	ReleaseDC(hWnd, g_hdc);//释放设备环境句柄
	return true;
}

//---------------------------------------------------------------------------
// 描述：绘制函数Game_Paint，进行绘制操作
//---------------------------------------------------------------------------
VOID Game_Paint(HWND hWnd)
{
	int y = 0;//定义一个y坐标值

	//for循环用七种不同的画笔绘制线条
	for (int i = 0; i <= 6; i++)
	{
		y = (i + 1) * 70;
		SelectObject(g_hdc, g_hPen[i]);//选择画笔
		MoveToEx(g_hdc, 30, y, NULL);//光标移动到点(30，y)处
		LineTo(g_hdc, 100, y);//从点（30，y）向点（100，y）画线
	}
	//定义两个x坐标值
	int x1 = 120;
	int x2 = 190;
	//用七种不同的画刷填充矩形
	for (int i = 0; i <=6; i++)
	{
		SelectObject(g_hdc, g_hBrush[i]);
		Rectangle(g_hdc, x1, 70, x2, y);
		x1 += 90;
		x2 += 90;
	}
}

//---------------------------------------------------------------------------
// 描述：资源清理Game_CleanUp，进行退出前资源的清理工作
//---------------------------------------------------------------------------
BOOL Game_CleanUp(HWND hWnd)
{
	for (int i = 0; i <=6; i++)
	{
		DeleteObject(g_hPen[i]);
		DeleteObject(g_hBrush[i]);
	}
	return true;
}
