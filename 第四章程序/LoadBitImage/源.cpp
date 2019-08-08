//---------------------------------------------------------------------------
// �������ƣ�GameWnd
// 2019��7�� Create by ����
// ��������Ϸ������
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ��������������������ͷ�ļ�
//---------------------------------------------------------------------------
#include <windows.h>

//---------------------------------------------------------------------------
// ����������һЩ������
//---------------------------------------------------------------------------
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE L"��Ϸ������"

//---------------------------------------------------------------------------
// ������ȫ�ֱ���������
//---------------------------------------------------------------------------
HDC g_hdc = NULL,g_mdc = NULL;//ȫ���豸�������
//HBITMAP g_hBitMap = NULL;
HBITMAP g_hBitMap = NULL;

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
	PAINTSTRUCT paintStruct;//����һ��PAINTSTRUCT�ṹ������¼һЩ��ͼ��Ϣ
	switch (nMsg)
	{
	case WM_PAINT:
		g_hdc = BeginPaint(hWnd, &paintStruct);//ָ�����ڽ��л�ͼ������׼���������ͼ�йص���Ϣ��䵽paintStruct
		Game_Paint(hWnd);
		EndPaint(hWnd, &paintStruct);//��Ǵ��ڻ�ͼ�Ľ���
		ValidateRect(hWnd, NULL);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(hWnd);
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
	g_hBitMap = (HBITMAP)LoadImage(NULL, L"Naruto.bmp", IMAGE_BITMAP, 800, 600, LR_LOADFROMFILE);
	g_mdc = CreateCompatibleDC(g_hdc);

	Game_Paint(hWnd);//ִ�л��Ʋ���
	ReleaseDC(hWnd, g_hdc);//�ͷ��豸�������
	return true;
}

//---------------------------------------------------------------------------
// ���������ƺ���Game_Paint�����л��Ʋ���
//---------------------------------------------------------------------------
VOID Game_Paint(HWND hWnd)
{
	SelectObject(g_mdc, g_hBitMap);
	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);
}

//---------------------------------------------------------------------------
// ��������Դ����Game_CleanUp�������˳�ǰ��Դ��������
//---------------------------------------------------------------------------
BOOL Game_CleanUp(HWND hWnd)
{
	DeleteObject(g_hBitMap);
	DeleteDC(g_mdc);
	return true;
}
