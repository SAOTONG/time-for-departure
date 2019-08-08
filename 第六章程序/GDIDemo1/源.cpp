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
//---------------------------------------------------------------------------
// ����������һЩ������
//---------------------------------------------------------------------------
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE L"��Ϸ������"

//---------------------------------------------------------------------------
// ������ȫ�ֱ���������
//---------------------------------------------------------------------------
HDC g_hdc = NULL, g_mdc = NULL;//ȫ���豸�������
HBITMAP g_hGirl[11];//����λͼ���飬������������λͼ
DWORD g_tPre = 0,g_tNow = 0;//��������������¼ʱ�䣬ǰ������һ�λ�ͼʱ�䣬�����Ǳ���׼����ͼ�ĵ�ǰʱ��
int g_iNum = 0;//��¼Ŀǰ��ʾ��ͼ��

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
	wchar_t filename[20];//���ڴ���ļ����Ƶ��ַ�����
	for (int i = 0; i < 11; i++)
	{
		memset(filename, 0, sizeof(filename));//��ʼ���ַ�����
		swprintf_s(filename, L"%d.bmp", i);//�õ���Ӧͼ������
		g_hGirl[i] = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 
			WINDOW_WIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE);//���ظ�ͼ��
	}
	g_mdc = CreateCompatibleDC(g_hdc);//���������豸�������ڴ�DC

	Game_Paint(hWnd);//ִ�л��Ʋ���
	
	return true;
}

//---------------------------------------------------------------------------
// ���������ƺ���Game_Paint�����л��Ʋ���
//---------------------------------------------------------------------------
VOID Game_Paint(HWND hWnd)
{
	if (g_iNum ==11)
	{
		g_iNum = 0;
	}
	SelectObject(g_mdc, g_hGirl[g_iNum]);
	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);
	g_tPre = GetTickCount();
	g_iNum++;
}

//---------------------------------------------------------------------------
// ��������Դ����Game_CleanUp�������˳�ǰ��Դ��������
//---------------------------------------------------------------------------
BOOL Game_CleanUp(HWND hWnd)
{
	for (int i = 0; i < 11; i++)
	{
		DeleteObject(g_hGirl[i]);
	}
	DeleteDC(g_mdc);
	ReleaseDC(hWnd, g_hdc);//�ͷ��豸�������
	return true;
}
