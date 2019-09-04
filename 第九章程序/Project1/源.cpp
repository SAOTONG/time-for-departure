//---------------------------------------------------------------------------
// 程序名称：GameWnd
// 2019年7月 Create by 侯金成
// 描述：游戏程序框架
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// 描述：包含程序依赖的头文件
//---------------------------------------------------------------------------
#include <windows.h>
#include <tchar.h>
#pragma comment(lib,"winmm.lib")
#pragma  comment(lib,"Msimg32.lib")             //添加使用TransparentBlt函数所需的库文件
//---------------------------------------------------------------------------
// 描述：定义一些辅助宏
//---------------------------------------------------------------------------
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE L"游戏程序框架"
//---------------------------------------------------------------------------
// 描述：全局变量的声明
//---------------------------------------------------------------------------
HDC g_hdc = NULL, g_mdc = NULL, g_bufdc = NULL;//全局设备环境句柄
HBITMAP g_hAngrybird = NULL, g_hBackGround = NULL;//定义位图句柄，用来存储小鸟图和背景图
DWORD g_tPre = 0, g_tNow = 0;//声明两个变量记录时间，前者是上一次绘图时间，后者是本次准备绘图的当前时间
int g_iX = 0, g_iY = 0, g_iXSpeed = 0, g_iYSpeed = 0;//记录小鸟图像的横纵坐标和xy轴的速度分量
int g_iYGravity = 0;//重力加速度
int g_iXFriction = 0, g_iYFriction = 0;//水平方向摩擦力和竖直方向摩擦力
RECT g_rect;//声明一个RECT结构体，用于存储内部窗口区域的坐标
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
	//PlaySound(L"Dia Frampton - Walk Away.flac", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	//消息循环过程
	MSG msg = { 0 };//定义并初始化msg
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))//查看应用程序消息队列，派发消息
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			g_tNow = GetTickCount();
			if (g_tNow - g_tPre >= 40)//上次绘图时间与当前绘图时间间隔80毫秒的话就绘图
			{
				Game_Paint(hWnd);
			}
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
	switch (nMsg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
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
	HBITMAP bmp;
	g_hdc = GetDC(hWnd);//获取设备环境句柄
	g_mdc = CreateCompatibleDC(g_hdc);//建立兼容设备环境的内存DC
	g_bufdc = CreateCompatibleDC(g_hdc);//建立兼容设备环境的缓存DC
	bmp = CreateCompatibleBitmap(g_hdc, WINDOW_WIDTH, WINDOW_HEIGHT); //建一个和窗口兼容的空的位图对象
	SelectObject(g_mdc, bmp);//将空位图对象放到mdc中
	g_hBackGround = (HBITMAP)LoadImage(NULL, L"bg.bmp", IMAGE_BITMAP,
		WINDOW_WIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE);//加载背景图像
	g_hAngrybird = (HBITMAP)LoadImage(NULL, L"angrybird.bmp", IMAGE_BITMAP,
		120, 60, LR_LOADFROMFILE);//加载小鸟图像
	GetClientRect(hWnd, &g_rect);
	g_iX = 100;
	g_iY = 100;
	g_iXSpeed = 30;
	g_iYSpeed = 0;
	g_iYGravity = 3;
	g_iXFriction = -2;
	g_iYFriction = -4;
	Game_Paint(hWnd);//执行绘制操作
	return true;
}
//---------------------------------------------------------------------------
// 描述：绘制函数Game_Paint，进行绘制操作
//---------------------------------------------------------------------------
VOID Game_Paint(HWND hWnd)
{
	//在内存DC中贴背景图
	SelectObject(g_bufdc, g_hBackGround);
	BitBlt(g_mdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_bufdc, 0, 0, SRCCOPY);
	//在内存DC中进行透明处理
	SelectObject(g_bufdc, g_hAngrybird);
	BitBlt(g_mdc, g_iX, g_iY, 60, 60, g_bufdc, 60, 0, SRCAND);
	BitBlt(g_mdc, g_iX, g_iY, 60, 60, g_bufdc, 0, 0, SRCPAINT);
	//将图像显示到窗口
	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);
	g_iX += g_iXSpeed;
	if (g_iX <= 0)
	{
		g_iX = 0;
		if (g_iXSpeed < 0)
			g_iXSpeed -= g_iXFriction;
		else
		{
			g_iXSpeed += g_iXFriction;
			if (g_iXSpeed < 0)
				g_iXSpeed = 0;
		}
		if (g_iYSpeed < 0)
			g_iYSpeed -= g_iYFriction;
		else
		{
			g_iYSpeed += g_iYFriction;
			if (g_iYSpeed < 0)
				g_iYSpeed = 0;
		}
		g_iXSpeed = -g_iXSpeed;
	}
	else if (g_iX >= g_rect.right - 60)
	{
		g_iX = g_rect.right - 60;
		if (g_iXSpeed < 0)
			g_iXSpeed -= g_iXFriction;
		else
		{
			g_iXSpeed += g_iXFriction;
			if (g_iXSpeed < 0)
				g_iXSpeed = 0;
		}
		if (g_iYSpeed < 0)
			g_iYSpeed -= g_iYFriction;
		else
		{
			g_iYSpeed += g_iYFriction;
			if (g_iYSpeed < 0)
				g_iYSpeed = 0;
		}
		g_iXSpeed = -g_iXSpeed;
	}
	g_iYSpeed += g_iYGravity;
	g_iY += g_iYSpeed;
	if (g_iY <= 0)
	{
		g_iY = 0;
		if (g_iXSpeed < 0)
			g_iXSpeed -= g_iXFriction;
		else
		{
			g_iXSpeed += g_iXFriction;
			if (g_iXSpeed < 0)
				g_iXSpeed = 0;
		}
		if (g_iYSpeed < 0)
			g_iYSpeed -= g_iYFriction;
		else
		{
			g_iYSpeed += g_iYFriction;
			if (g_iYSpeed < 0)
				g_iYSpeed = 0;
		}
		g_iYSpeed = -g_iYSpeed;
	}
	else if (g_iY >= g_rect.bottom - 60)
	{
		g_iY = g_rect.bottom - 60;
		if (g_iXSpeed < 0)
			g_iXSpeed -= g_iXFriction;
		else
		{
			g_iXSpeed += g_iXFriction;
			if (g_iXSpeed < 0)
				g_iXSpeed = 0;
		}
		if (g_iYSpeed < 0)
			g_iYSpeed -= g_iYFriction;
		else
		{
			g_iYSpeed += g_iYFriction;
			if (g_iYSpeed < 0)
				g_iYSpeed = 0;
		}

		g_iYSpeed = -g_iYSpeed;
	}
	g_tPre = GetTickCount();
}
//---------------------------------------------------------------------------
// 描述：资源清理Game_CleanUp，进行退出前资源的清理工作
//---------------------------------------------------------------------------
BOOL Game_CleanUp(HWND hWnd)
{
	DeleteObject(g_hBackGround);
	DeleteObject(g_hAngrybird);
	DeleteDC(g_mdc);
	DeleteDC(g_bufdc);
	ReleaseDC(hWnd, g_hdc);//释放设备环境句柄
	return true;
}