//---------------------------------------------------------------------------
// �������ƣ�GameWnd
// 2019��7�� Create by ����
// ��������Ϸ������
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// ��������������������ͷ�ļ�
//---------------------------------------------------------------------------
#include <windows.h>
#include <tchar.h>
#pragma comment(lib,"winmm.lib")
#pragma  comment(lib,"Msimg32.lib")             //���ʹ��TransparentBlt��������Ŀ��ļ�
//---------------------------------------------------------------------------
// ����������һЩ������
//---------------------------------------------------------------------------
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE L"��Ϸ������"
//---------------------------------------------------------------------------
// ������ȫ�ֱ���������
//---------------------------------------------------------------------------
HDC g_hdc = NULL, g_mdc = NULL, g_bufdc = NULL;//ȫ���豸�������
HBITMAP g_hAngrybird = NULL, g_hBackGround = NULL;//����λͼ����������洢С��ͼ�ͱ���ͼ
DWORD g_tPre = 0, g_tNow = 0;//��������������¼ʱ�䣬ǰ������һ�λ�ͼʱ�䣬�����Ǳ���׼����ͼ�ĵ�ǰʱ��
int g_iX = 0, g_iY = 0, g_iXSpeed = 0, g_iYSpeed = 0;//��¼С��ͼ��ĺ��������xy����ٶȷ���
int g_iYGravity = 0;//�������ٶ�
int g_iXFriction = 0, g_iYFriction = 0;//ˮƽ����Ħ��������ֱ����Ħ����
RECT g_rect;//����һ��RECT�ṹ�壬���ڴ洢�ڲ��������������
//---------------------------------------------------------------------------
// ������ȫ�ֺ�������
//---------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL Game_Init(HWND hWnd);//��Դ��ʼ��
VOID Game_Paint(HWND hWnd);//���л�ͼ
BOOL Game_CleanUp(HWND hWnd);//��Դ����
												 //---------------------------------------------------------------------------
												 // ����������������
												 //---------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//���һ�������Ĵ�����
	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);//���ýṹ����ֽ���
	wndClass.style = CS_HREDRAW | CS_VREDRAW;//���ô��ڵ���ʽ
	wndClass.lpfnWndProc = WndProc;//ָ��ص�������Զָ��
	wndClass.cbClsExtra = 0;//�����฽���ڴ�
	wndClass.cbWndExtra = 0;//���ڸ����ڴ�
	wndClass.hInstance = hInstance;//ָ���������ڹ��̵ĳ����ʵ�����
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"icon.ico", IMAGE_ICON,
		0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = L"GameWindow";
	//ע�ᴰ����
	if (!RegisterClassEx(&wndClass))
	{
		return -1;
	}
	//��ʽ��������
	HWND hWnd = CreateWindow(L"GameWindow", WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);
	//���ڵ��ƶ�����ʾ�����
	MoveWindow(hWnd, 250, 80, WINDOW_WIDTH, WINDOW_HEIGHT, true);//����������ʾλ��
	ShowWindow(hWnd, nCmdShow);//��ʾ����
	UpdateWindow(hWnd);//���´���
								  //��Դ��ʼ��������ʼʧ�ܣ��򷵻�false
	if (!Game_Init(hWnd))
	{
		MessageBox(hWnd, L"��Դ��ʼ��ʧ��", L"��Ϣ����", 0);
		return false;
	}
	//���ڳ�ʼ���󣬲�������
	//PlaySound(L"Dia Frampton - Walk Away.flac", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	//��Ϣѭ������
	MSG msg = { 0 };//���岢��ʼ��msg
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))//�鿴Ӧ�ó�����Ϣ���У��ɷ���Ϣ
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			g_tNow = GetTickCount();
			if (g_tNow - g_tPre >= 40)//�ϴλ�ͼʱ���뵱ǰ��ͼʱ����80����Ļ��ͻ�ͼ
			{
				Game_Paint(hWnd);
			}
		}
	}
	//�������ע��
	UnregisterClass(L"GameWindow", wndClass.hInstance);
	return 0;
}
//---------------------------------------------------------------------------
// ���������ڹ��̺���WndProc���Դ�����Ϣ���д���
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
		Game_CleanUp(hWnd);//�����Զ������Դ���������ڴ����˳�ǰ����Դ��������
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, nMsg, wParam, lParam);
	}
	return 0;
}
//---------------------------------------------------------------------------
// ��������Դ��ʼ������Game_Init������һЩ�򵥵ĳ�ʼ��
//---------------------------------------------------------------------------
BOOL Game_Init(HWND hWnd)
{
	HBITMAP bmp;
	g_hdc = GetDC(hWnd);//��ȡ�豸�������
	g_mdc = CreateCompatibleDC(g_hdc);//���������豸�������ڴ�DC
	g_bufdc = CreateCompatibleDC(g_hdc);//���������豸�����Ļ���DC
	bmp = CreateCompatibleBitmap(g_hdc, WINDOW_WIDTH, WINDOW_HEIGHT); //��һ���ʹ��ڼ��ݵĿյ�λͼ����
	SelectObject(g_mdc, bmp);//����λͼ����ŵ�mdc��
	g_hBackGround = (HBITMAP)LoadImage(NULL, L"bg.bmp", IMAGE_BITMAP,
		WINDOW_WIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE);//���ر���ͼ��
	g_hAngrybird = (HBITMAP)LoadImage(NULL, L"angrybird.bmp", IMAGE_BITMAP,
		120, 60, LR_LOADFROMFILE);//����С��ͼ��
	GetClientRect(hWnd, &g_rect);
	g_iX = 100;
	g_iY = 100;
	g_iXSpeed = 30;
	g_iYSpeed = 0;
	g_iYGravity = 3;
	g_iXFriction = -2;
	g_iYFriction = -4;
	Game_Paint(hWnd);//ִ�л��Ʋ���
	return true;
}
//---------------------------------------------------------------------------
// ���������ƺ���Game_Paint�����л��Ʋ���
//---------------------------------------------------------------------------
VOID Game_Paint(HWND hWnd)
{
	//���ڴ�DC��������ͼ
	SelectObject(g_bufdc, g_hBackGround);
	BitBlt(g_mdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_bufdc, 0, 0, SRCCOPY);
	//���ڴ�DC�н���͸������
	SelectObject(g_bufdc, g_hAngrybird);
	BitBlt(g_mdc, g_iX, g_iY, 60, 60, g_bufdc, 60, 0, SRCAND);
	BitBlt(g_mdc, g_iX, g_iY, 60, 60, g_bufdc, 0, 0, SRCPAINT);
	//��ͼ����ʾ������
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
// ��������Դ����Game_CleanUp�������˳�ǰ��Դ��������
//---------------------------------------------------------------------------
BOOL Game_CleanUp(HWND hWnd)
{
	DeleteObject(g_hBackGround);
	DeleteObject(g_hAngrybird);
	DeleteDC(g_mdc);
	DeleteDC(g_bufdc);
	ReleaseDC(hWnd, g_hdc);//�ͷ��豸�������
	return true;
}