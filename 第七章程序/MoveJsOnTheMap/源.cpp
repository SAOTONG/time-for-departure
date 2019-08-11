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
#pragma  comment(lib,"Msimg32.lib")		//添加使用TransparentBlt函数所需的库文件
//---------------------------------------------------------------------------
// 描述：定义一些辅助宏
//---------------------------------------------------------------------------
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE L"游戏程序框架"

//---------------------------------------------------------------------------
// 描述：全局结构体定义
//---------------------------------------------------------------------------
struct  SwordBullets //SwordBullets代表剑气(子弹)
{
	int x, y;//子弹的坐标
	bool exist;//子弹是否存在
};

//---------------------------------------------------------------------------
// 描述：全局变量的声明
//---------------------------------------------------------------------------
HDC g_hdc = NULL, g_mdc = NULL, g_bufdc = NULL;//全局设备环境句柄
HBITMAP g_hMen = NULL,g_hSwordBlad = NULL, g_hBackGround = NULL;//定义位图句柄，用来存储角色图、子弹图、背景图
DWORD g_tPre = 0, g_tNow = 0;//声明两个变量记录时间，前者是上一次绘图时间，后者是本次准备绘图的当前时间
int g_iX = 0, g_iY = 0,g_iXmen = 0,g_iYmen = 0;//g_ix，g_iy代表鼠标光标位置，g_iXmen、g_iYmen代表角色位置。
int g_iBGOffset = 0, g_iBulletNum = 0;
SwordBullets Bullet[30];//存储子弹

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
	PlaySound(L"仙剑三·原版战斗3.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

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
			if (g_tNow - g_tPre >= 30)//上次绘图时间与当前绘图时间间隔80毫秒的话就绘图
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
	case WM_MOUSEMOVE:
		//对鼠标移动范围限制
		g_iX = LOWORD(lParam);//取得鼠标x坐标
		if (g_iX>WINDOW_WIDTH-317)
		{
			g_iX = WINDOW_WIDTH - 317;
		} 
		else if(g_iX<0)
		{
			g_iX = 0;
		}

		g_iY = HIWORD(lParam);//取得鼠标y坐标
		if (g_iY > WINDOW_HEIGHT - 283)
		{
			g_iY = WINDOW_HEIGHT - 283;
		}
		else if (g_iY < 0)
		{
			g_iY = 0;
		}
		break;
	case WM_LBUTTONDOWN:
		for (int i = 0; i < 30; i++)
		{
			if (!Bullet[i].exist)
			{
				Bullet[i].x = g_iXmen;		//剑气（子弹）x坐标
				Bullet[i].y = g_iYmen + 30; //剑气（子弹）y坐标
				Bullet[i].exist = true;
				g_iBulletNum++;			//累加剑气（子弹）数目
				break;
			}
		}
		break;
	case WM_KEYDOWN:	     //按下键盘消息
	//判断按键的虚拟键码
		switch (wParam)
		{
		case VK_ESCAPE:           //按下【Esc】键
			DestroyWindow(hWnd);    // 销毁窗口, 并发送一条WM_DESTROY消息
			PostQuitMessage(0);  //结束程序
			break;
		}

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
	//创建各种设备环境DC以及声明空的位图对象
	HBITMAP bmp;
	g_hdc = GetDC(hWnd);//获取设备环境句柄
	g_mdc = CreateCompatibleDC(g_hdc);//建立兼容设备环境的内存DC
	g_bufdc = CreateCompatibleDC(g_hdc);//建立兼容设备环境的缓存DC
	bmp = CreateCompatibleBitmap(g_hdc, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(g_mdc, bmp);
	
	//加载各张跑动图及背景图
	g_hBackGround = (HBITMAP)LoadImage(NULL, L"bg.bmp", IMAGE_BITMAP, WINDOW_WIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE);
	g_hMen = (HBITMAP)LoadImage(NULL, L"swordman.bmp", IMAGE_BITMAP, 317, 283, LR_LOADFROMFILE);
	g_hSwordBlad = (HBITMAP)LoadImage(NULL, L"swordblade.bmp", IMAGE_BITMAP, 100, 26, LR_LOADFROMFILE);
	
	//设定人物贴图初始值，鼠标位置初始值
	g_iX = 300;
	g_iY = 100;
	g_iXmen = 300;
	g_iYmen = 100;

	//设定鼠标区域
	POINT pt, lt, rb;//pt保存鼠标位置、lt和rb分别保存矩形左上角和右下角坐标
	RECT rect;
	//设定光标位置
	pt.x = 300;
	pt.y = 100;
	ClientToScreen(hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
	ShowCursor(false);		//隐藏鼠标光标
	//限制鼠标光标移动区域
	GetClientRect(hWnd, &rect);  //取得窗口内部矩形
	//将矩形左上点坐标存入lt中
	lt.x = rect.left;
	lt.y = rect.top;
	//将矩形右下坐标存入rb中
	rb.x = rect.right;
	rb.y = rect.bottom;
	//将lt和rb的窗口坐标转换为屏幕坐标
	ClientToScreen(hWnd, &lt);
	ClientToScreen(hWnd, &rb);
	//以屏幕坐标重新设定矩形区域
	rect.left = lt.x;
	rect.top = lt.y;
	rect.right = rb.x;
	rect.bottom = rb.y;
	//限制鼠标光标移动区域
	ClipCursor(&rect);

	Game_Paint(hWnd);
	return true;
}

//---------------------------------------------------------------------------
// 描述：绘制函数Game_Paint，进行绘制操作
//---------------------------------------------------------------------------
VOID Game_Paint(HWND hWnd)
{
	//先在mdc中贴上背景图
	SelectObject(g_bufdc, g_hBackGround);
	BitBlt(g_mdc, 0, 0, g_iBGOffset, WINDOW_HEIGHT, g_bufdc, WINDOW_WIDTH - g_iBGOffset, 0, SRCCOPY);
	BitBlt(g_mdc, g_iBGOffset, 0, WINDOW_WIDTH - g_iBGOffset, WINDOW_HEIGHT, g_bufdc, 0, 0, SRCCOPY);

	wchar_t str[20] = {};

	//计算剑侠的贴图坐标，设定每次进行剑侠贴图时，其贴图坐标（g_iXnow，g_iYnow）会以10个单位慢慢向鼠标光标所在的目的点（x，y）接近，直到两个坐标相同为止
	if (g_iXmen < g_iX)//若当前贴图X坐标小于鼠标光标的X坐标
	{
		g_iXmen += 40;
		if (g_iXmen > g_iX)
			g_iXmen = g_iX;
	}
	else   //若当前贴图X坐标大于鼠标光标的X坐标
	{
		g_iXmen -= 40;
		if (g_iXmen < g_iX)
			g_iXmen = g_iX;
	}

	if (g_iYmen < g_iY)  //若当前贴图Y坐标小于鼠标光标的Y坐标
	{
		g_iYmen += 40;
		if (g_iYmen > g_iY)
			g_iYmen = g_iY;
	}
	else  //若当前贴图Y坐标大于于鼠标光标的Y坐标
	{
		g_iYmen -= 40;
		if (g_iYmen < g_iY)
			g_iYmen = g_iY;
	}

	//贴上剑侠图
	SelectObject(g_bufdc, g_hMen);
	TransparentBlt(g_mdc, g_iXmen, g_iYmen, 317, 283, g_bufdc, 0, 0, 317, 283, RGB(0, 0, 0));

	//剑气（子弹）的贴图，先判断剑气（子弹）数目“g_iBulletNum”的值是否为“0”。若不为0，则对剑气（子弹）数组中各个还存在的剑气（子弹）按照其所在的坐标（b[i].x,b[i].y）循环进行贴图操作
	SelectObject(g_bufdc, g_hSwordBlad);
	if (g_iBulletNum != 0)
		for (int i = 0; i < 30; i++)
			if (Bullet[i].exist)
			{
				//贴上剑气（子弹）图
				TransparentBlt(g_mdc, Bullet[i].x - 70, Bullet[i].y + 100, 100, 33, g_bufdc, 0, 0, 100, 26, RGB(0, 0, 0));

				//设置下一个剑气（子弹）的坐标。剑气（子弹）是从右向左发射的，因此，每次其X轴上的坐标值递减10个单位，这样贴图会产生往左移动的效果。而如果剑气（子弹）下次的坐标已超出窗口的可见范围（h[i].x<0）,那么剑气（子弹）设为不存在，并将剑气（子弹）总数g_iBulletNum变量值减1.
				Bullet[i].x -= 10;
				if (Bullet[i].x < 0)
				{
					g_iBulletNum--;
					Bullet[i].exist = false;
				}
			}

	HFONT hFont;
	hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("微软雅黑"));  //创建字体
	SelectObject(g_mdc, hFont);  //选入字体到g_mdc中
	SetBkMode(g_mdc, TRANSPARENT);    //设置文字背景透明
	SetTextColor(g_mdc, RGB(255, 255, 0));  //设置文字颜色

	//在左上角进行文字输出
	swprintf_s(str, L"鼠标X坐标为%d    ", g_iX);
	TextOut(g_mdc, 0, 0, str, wcslen(str));
	swprintf_s(str, L"鼠标Y坐标为%d    ", g_iY);
	TextOut(g_mdc, 0, 20, str, wcslen(str));

	//贴上背景图
	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);

	g_tPre = GetTickCount();

	g_iBGOffset += 5; //让背景滚动量+5
	if (g_iBGOffset == WINDOW_WIDTH)//如果背景滚动量达到了背景宽度值，就置零
		g_iBGOffset = 0;
}

//---------------------------------------------------------------------------
// 描述：资源清理Game_CleanUp，进行退出前资源的清理工作
//---------------------------------------------------------------------------
BOOL Game_CleanUp(HWND hWnd)
{
	DeleteObject(g_hBackGround);
	DeleteObject(g_hMen);
	DeleteObject(g_hSwordBlad);
	DeleteDC(g_mdc);
	DeleteDC(g_bufdc);
	ReleaseDC(hWnd, g_hdc);//释放设备环境句柄
	return true;
}
