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
#pragma  comment(lib,"Msimg32.lib")		//���ʹ��TransparentBlt��������Ŀ��ļ�
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
HBITMAP g_hMen[4]{NULL};//����һ����Ž�ɫλͼ������
HBITMAP g_hBackGround = NULL;//����λͼ����������洢����ͼ
DWORD g_tPre = 0, g_tNow = 0;//��������������¼ʱ�䣬ǰ������һ�λ�ͼʱ�䣬�����Ǳ���׼����ͼ�ĵ�ǰʱ��
int g_iNum = 0, g_iX = 0, g_iY = 0;//��¼ͼ�š���ɫͼ��ĺ�������
int g_iDirection = 0;//��¼�����ɫ�ķ���0�ϣ�1�£�2��3�ҡ�

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
			if (g_tNow - g_tPre >= 80)//�ϴλ�ͼʱ���뵱ǰ��ͼʱ����80����Ļ��ͻ�ͼ
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
		switch (wParam)
		{
		case VK_UP:
			g_iY -= 10;
			g_iDirection = 0;
			if (g_iY < 0)
				g_iY = 0;
			break;
		case VK_DOWN:
			g_iY += 10;
			g_iDirection = 1;
			if (g_iY > WINDOW_HEIGHT - 108)
				g_iY = WINDOW_HEIGHT - 108;
			break;
		case VK_LEFT:
			g_iX -= 10;
			g_iDirection = 2;
			if (g_iX < 0)
				g_iX = 0;
			break;
		case VK_RIGHT:
			g_iX += 10;
			g_iDirection = 3;
			if (g_iX > WINDOW_WIDTH - 60)
				g_iX = WINDOW_WIDTH - 60;
			break;
		}
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
	g_hdc = GetDC(hWnd);//��ȡ�豸�������
	g_mdc = CreateCompatibleDC(g_hdc);//���������豸�������ڴ�DC
	g_bufdc = CreateCompatibleDC(g_hdc);//���������豸�����Ļ���DC
	g_hBackGround = (HBITMAP)LoadImage(NULL, L"bg.bmp", IMAGE_BITMAP,
		WINDOW_WIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE);//���ر���ͼ��
	g_hMen[0] = (HBITMAP)LoadImage(NULL, L"go1.bmp", IMAGE_BITMAP,
		480, 216, LR_LOADFROMFILE);//��������ͼ��
	g_hMen[1] = (HBITMAP)LoadImage(NULL, L"go2.bmp", IMAGE_BITMAP,
		480, 216, LR_LOADFROMFILE);//��������ͼ��
	g_hMen[2] = (HBITMAP)LoadImage(NULL, L"go3.bmp", IMAGE_BITMAP,
		480, 216, LR_LOADFROMFILE);//��������ͼ��
	g_hMen[3] = (HBITMAP)LoadImage(NULL, L"go4.bmp", IMAGE_BITMAP,
		480, 216, LR_LOADFROMFILE);//��������ͼ��
	Game_Paint(hWnd);//ִ�л��Ʋ���

	return true;
}

//---------------------------------------------------------------------------
// ���������ƺ���Game_Paint�����л��Ʋ���
//---------------------------------------------------------------------------
VOID Game_Paint(HWND hWnd)
{
	if (g_iNum == 8)
	{
		g_iNum = 0;
	}
	//���ڴ�DC��������ͼ
	SelectObject(g_bufdc, g_hBackGround);
	BitBlt(g_mdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_bufdc, 0, 0, SRCCOPY);
	//���ڴ�DC�н���͸������
	SelectObject(g_bufdc, g_hMen[g_iDirection]);
	BitBlt(g_mdc, g_iX, g_iY, 60, 108, g_bufdc, g_iNum*60, 108, SRCAND);
	BitBlt(g_mdc, g_iX, g_iY, 60, 108, g_bufdc, g_iNum*60, 0, SRCPAINT);
	//��ͼ����ʾ������
	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);
	g_tPre = GetTickCount();
	g_iNum++;
}

//---------------------------------------------------------------------------
// ��������Դ����Game_CleanUp�������˳�ǰ��Դ��������
//---------------------------------------------------------------------------
BOOL Game_CleanUp(HWND hWnd)
{
	DeleteObject(g_hBackGround);
	DeleteObject(g_hMen);
	DeleteDC(g_mdc);
	DeleteDC(g_bufdc);
	ReleaseDC(hWnd, g_hdc);//�ͷ��豸�������
	return true;
}
