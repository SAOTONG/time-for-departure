//---------------------------------------------------------------------------
// 程序名称：D3DdemoCore
// 2019年7月 Create by 侯金成
// 描述：游戏程序框架
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// 描述：包含程序依赖的头文件
//---------------------------------------------------------------------------
#include <d3d9.h>

//---------------------------------------------------------------------------
// 描述：添加程序所依赖的库文件
//---------------------------------------------------------------------------
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")

//---------------------------------------------------------------------------
// 描述：定义一些辅助宏
//---------------------------------------------------------------------------
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE L"游戏程序框架"
#define SAFE_RELEASE(p) {if(p){(p)->Release();(p) = NULL;}}//定义一个安全释放宏，便于后面COM接口指针的释放

//---------------------------------------------------------------------------
// 描述：全局变量的声明
//---------------------------------------------------------------------------
LPDIRECT3DDEVICE9  g_pd3dDevice = NULL;//Direct3D设备对象
//---------------------------------------------------------------------------
// 描述：全局函数声明
//---------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Direct3D_Init(HWND hWnd);//Direct3D初始化
HRESULT Objects_Init(HWND hWnd);//需要绘制的物体资源初始化
VOID Direct3D_Render(HWND hWnd);//Direct3D渲染代码的书写
VOID Direct3D_CleanUp(HWND hWnd);//清理COM资源以及其他资源

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

	//资源初始化，若初始失败，则返回false
	if (S_OK == Direct3D_Init(hWnd))
		MessageBox(hWnd, L"Direct3d初始化完成~", L"侯金成的消息窗口", 0);
	else
		MessageBox(hWnd, L"Direct3d初始化失败~", L"侯金成的消息窗口", 0);

	//窗口的移动、显示与更新
	MoveWindow(hWnd, 250, 80, WINDOW_WIDTH, WINDOW_HEIGHT, true);//调整窗口显示位置
	ShowWindow(hWnd, nCmdShow);//显示窗口
	UpdateWindow(hWnd);//更新窗口

	//窗口初始化后，播放音乐
	//PlaySound(L"Dia Frampton - Walk Away.flac", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	MessageBox(hWnd, L"嘿嘿，终于到了这一天", L"Direct3D", 0);

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
			Direct3D_Render(hWnd);
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
	case WM_PAINT:
		Direct3D_Render(hWnd);
		ValidateRect(hWnd, NULL);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		Direct3D_CleanUp(hWnd);//调用自定义的资源清理函数，在窗口退出前对资源进行清理
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, nMsg, wParam, lParam);
	}
	return 0;
}

//---------------------------------------------------------------------------
// 描述：Direct3D_Init函数，负责Direct3D初始化
//---------------------------------------------------------------------------
HRESULT Direct3D_Init(HWND hWnd)
{
	//创建Direct3D接口对象
	LPDIRECT3D9 pD3D = NULL;
	if (NULL == (pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	//获取硬件设备信息
	D3DCAPS9 caps;
	int vp = 0;
	if (FAILED(pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
		return E_FAIL;
	if (caps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;//支持硬件顶点运算
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;//不支持硬件顶点运算

	//填充D3DPRESENT_PARAMETERS结构体
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = WINDOW_WIDTH;
	d3dpp.BackBufferHeight = WINDOW_HEIGHT;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	//创建Direct3D设备接口
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, vp, &d3dpp, &g_pd3dDevice)))
		return E_FAIL;
	SAFE_RELEASE(pD3D);

	if (!(S_OK == Objects_Init(hWnd)))return E_FAIL;//调用Objects_Init，对需要绘制的物体资源初始化
	return S_OK;
}

//---------------------------------------------------------------------------
// 描述：Objects_Init函数，对需要绘制的物体资源初始化
//---------------------------------------------------------------------------
HRESULT Objects_Init(HWND hWnd)
{
	return S_OK;
}

//---------------------------------------------------------------------------
// 描述：Direct3D_Render函数，使用Direct3D渲染
//---------------------------------------------------------------------------
VOID Direct3D_Render(HWND hWnd)
{
	return VOID();
}

//---------------------------------------------------------------------------
// 描述：函数Direct3D_CleanUp，负责资源清理，在程序退出之前进行资源清理
//---------------------------------------------------------------------------
VOID Direct3D_CleanUp(HWND hWnd)
{
	return VOID();
}

