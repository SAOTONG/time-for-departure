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
// ������ȫ�ֽṹ�嶨��
//---------------------------------------------------------------------------
struct  SwordBullets //SwordBullets������(�ӵ�)
{
	int x, y;//�ӵ�������
	bool exist;//�ӵ��Ƿ����
};

//---------------------------------------------------------------------------
// ������ȫ�ֱ���������
//---------------------------------------------------------------------------
HDC g_hdc = NULL, g_mdc = NULL, g_bufdc = NULL;//ȫ���豸�������
HBITMAP g_hMen = NULL,g_hSwordBlad = NULL, g_hBackGround = NULL;//����λͼ����������洢��ɫͼ���ӵ�ͼ������ͼ
DWORD g_tPre = 0, g_tNow = 0;//��������������¼ʱ�䣬ǰ������һ�λ�ͼʱ�䣬�����Ǳ���׼����ͼ�ĵ�ǰʱ��
int g_iX = 0, g_iY = 0,g_iXmen = 0,g_iYmen = 0;//g_ix��g_iy���������λ�ã�g_iXmen��g_iYmen�����ɫλ�á�
int g_iBGOffset = 0, g_iBulletNum = 0;
SwordBullets Bullet[30];//�洢�ӵ�

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
	PlaySound(L"�ɽ�����ԭ��ս��3.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

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
			if (g_tNow - g_tPre >= 30)//�ϴλ�ͼʱ���뵱ǰ��ͼʱ����80����Ļ��ͻ�ͼ
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
	case WM_MOUSEMOVE:
		//������ƶ���Χ����
		g_iX = LOWORD(lParam);//ȡ�����x����
		if (g_iX>WINDOW_WIDTH-317)
		{
			g_iX = WINDOW_WIDTH - 317;
		} 
		else if(g_iX<0)
		{
			g_iX = 0;
		}

		g_iY = HIWORD(lParam);//ȡ�����y����
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
				Bullet[i].x = g_iXmen;		//�������ӵ���x����
				Bullet[i].y = g_iYmen + 30; //�������ӵ���y����
				Bullet[i].exist = true;
				g_iBulletNum++;			//�ۼӽ������ӵ�����Ŀ
				break;
			}
		}
		break;
	case WM_KEYDOWN:	     //���¼�����Ϣ
	//�жϰ������������
		switch (wParam)
		{
		case VK_ESCAPE:           //���¡�Esc����
			DestroyWindow(hWnd);    // ���ٴ���, ������һ��WM_DESTROY��Ϣ
			PostQuitMessage(0);  //��������
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
	//���������豸����DC�Լ������յ�λͼ����
	HBITMAP bmp;
	g_hdc = GetDC(hWnd);//��ȡ�豸�������
	g_mdc = CreateCompatibleDC(g_hdc);//���������豸�������ڴ�DC
	g_bufdc = CreateCompatibleDC(g_hdc);//���������豸�����Ļ���DC
	bmp = CreateCompatibleBitmap(g_hdc, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(g_mdc, bmp);
	
	//���ظ����ܶ�ͼ������ͼ
	g_hBackGround = (HBITMAP)LoadImage(NULL, L"bg.bmp", IMAGE_BITMAP, WINDOW_WIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE);
	g_hMen = (HBITMAP)LoadImage(NULL, L"swordman.bmp", IMAGE_BITMAP, 317, 283, LR_LOADFROMFILE);
	g_hSwordBlad = (HBITMAP)LoadImage(NULL, L"swordblade.bmp", IMAGE_BITMAP, 100, 26, LR_LOADFROMFILE);
	
	//�趨������ͼ��ʼֵ�����λ�ó�ʼֵ
	g_iX = 300;
	g_iY = 100;
	g_iXmen = 300;
	g_iYmen = 100;

	//�趨�������
	POINT pt, lt, rb;//pt�������λ�á�lt��rb�ֱ𱣴�������ϽǺ����½�����
	RECT rect;
	//�趨���λ��
	pt.x = 300;
	pt.y = 100;
	ClientToScreen(hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
	ShowCursor(false);		//���������
	//����������ƶ�����
	GetClientRect(hWnd, &rect);  //ȡ�ô����ڲ�����
	//���������ϵ��������lt��
	lt.x = rect.left;
	lt.y = rect.top;
	//�����������������rb��
	rb.x = rect.right;
	rb.y = rect.bottom;
	//��lt��rb�Ĵ�������ת��Ϊ��Ļ����
	ClientToScreen(hWnd, &lt);
	ClientToScreen(hWnd, &rb);
	//����Ļ���������趨��������
	rect.left = lt.x;
	rect.top = lt.y;
	rect.right = rb.x;
	rect.bottom = rb.y;
	//����������ƶ�����
	ClipCursor(&rect);

	Game_Paint(hWnd);
	return true;
}

//---------------------------------------------------------------------------
// ���������ƺ���Game_Paint�����л��Ʋ���
//---------------------------------------------------------------------------
VOID Game_Paint(HWND hWnd)
{
	//����mdc�����ϱ���ͼ
	SelectObject(g_bufdc, g_hBackGround);
	BitBlt(g_mdc, 0, 0, g_iBGOffset, WINDOW_HEIGHT, g_bufdc, WINDOW_WIDTH - g_iBGOffset, 0, SRCCOPY);
	BitBlt(g_mdc, g_iBGOffset, 0, WINDOW_WIDTH - g_iBGOffset, WINDOW_HEIGHT, g_bufdc, 0, 0, SRCCOPY);

	wchar_t str[20] = {};

	//���㽣������ͼ���꣬�趨ÿ�ν��н�����ͼʱ������ͼ���꣨g_iXnow��g_iYnow������10����λ��������������ڵ�Ŀ�ĵ㣨x��y���ӽ���ֱ������������ͬΪֹ
	if (g_iXmen < g_iX)//����ǰ��ͼX����С��������X����
	{
		g_iXmen += 40;
		if (g_iXmen > g_iX)
			g_iXmen = g_iX;
	}
	else   //����ǰ��ͼX�������������X����
	{
		g_iXmen -= 40;
		if (g_iXmen < g_iX)
			g_iXmen = g_iX;
	}

	if (g_iYmen < g_iY)  //����ǰ��ͼY����С��������Y����
	{
		g_iYmen += 40;
		if (g_iYmen > g_iY)
			g_iYmen = g_iY;
	}
	else  //����ǰ��ͼY���������������Y����
	{
		g_iYmen -= 40;
		if (g_iYmen < g_iY)
			g_iYmen = g_iY;
	}

	//���Ͻ���ͼ
	SelectObject(g_bufdc, g_hMen);
	TransparentBlt(g_mdc, g_iXmen, g_iYmen, 317, 283, g_bufdc, 0, 0, 317, 283, RGB(0, 0, 0));

	//�������ӵ�������ͼ�����жϽ������ӵ�����Ŀ��g_iBulletNum����ֵ�Ƿ�Ϊ��0��������Ϊ0����Խ������ӵ��������и��������ڵĽ������ӵ������������ڵ����꣨b[i].x,b[i].y��ѭ��������ͼ����
	SelectObject(g_bufdc, g_hSwordBlad);
	if (g_iBulletNum != 0)
		for (int i = 0; i < 30; i++)
			if (Bullet[i].exist)
			{
				//���Ͻ������ӵ���ͼ
				TransparentBlt(g_mdc, Bullet[i].x - 70, Bullet[i].y + 100, 100, 33, g_bufdc, 0, 0, 100, 26, RGB(0, 0, 0));

				//������һ���������ӵ��������ꡣ�������ӵ����Ǵ���������ģ���ˣ�ÿ����X���ϵ�����ֵ�ݼ�10����λ��������ͼ����������ƶ���Ч����������������ӵ����´ε������ѳ������ڵĿɼ���Χ��h[i].x<0��,��ô�������ӵ�����Ϊ�����ڣ������������ӵ�������g_iBulletNum����ֵ��1.
				Bullet[i].x -= 10;
				if (Bullet[i].x < 0)
				{
					g_iBulletNum--;
					Bullet[i].exist = false;
				}
			}

	HFONT hFont;
	hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("΢���ź�"));  //��������
	SelectObject(g_mdc, hFont);  //ѡ�����嵽g_mdc��
	SetBkMode(g_mdc, TRANSPARENT);    //�������ֱ���͸��
	SetTextColor(g_mdc, RGB(255, 255, 0));  //����������ɫ

	//�����Ͻǽ����������
	swprintf_s(str, L"���X����Ϊ%d    ", g_iX);
	TextOut(g_mdc, 0, 0, str, wcslen(str));
	swprintf_s(str, L"���Y����Ϊ%d    ", g_iY);
	TextOut(g_mdc, 0, 20, str, wcslen(str));

	//���ϱ���ͼ
	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);

	g_tPre = GetTickCount();

	g_iBGOffset += 5; //�ñ���������+5
	if (g_iBGOffset == WINDOW_WIDTH)//��������������ﵽ�˱������ֵ��������
		g_iBGOffset = 0;
}

//---------------------------------------------------------------------------
// ��������Դ����Game_CleanUp�������˳�ǰ��Դ��������
//---------------------------------------------------------------------------
BOOL Game_CleanUp(HWND hWnd)
{
	DeleteObject(g_hBackGround);
	DeleteObject(g_hMen);
	DeleteObject(g_hSwordBlad);
	DeleteDC(g_mdc);
	DeleteDC(g_bufdc);
	ReleaseDC(hWnd, g_hdc);//�ͷ��豸�������
	return true;
}
