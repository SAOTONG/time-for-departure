//---------------------------------------------------------------------------
// �������ƣ�D3DdemoCore
// 2019��7�� Create by ����
// ��������Ϸ������
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ��������������������ͷ�ļ�
//---------------------------------------------------------------------------
#include <d3d9.h>

//---------------------------------------------------------------------------
// ��������ӳ����������Ŀ��ļ�
//---------------------------------------------------------------------------
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")

//---------------------------------------------------------------------------
// ����������һЩ������
//---------------------------------------------------------------------------
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE L"��Ϸ������"
#define SAFE_RELEASE(p) {if(p){(p)->Release();(p) = NULL;}}//����һ����ȫ�ͷź꣬���ں���COM�ӿ�ָ����ͷ�

//---------------------------------------------------------------------------
// ������ȫ�ֱ���������
//---------------------------------------------------------------------------
LPDIRECT3DDEVICE9  g_pd3dDevice = NULL;//Direct3D�豸����
//---------------------------------------------------------------------------
// ������ȫ�ֺ�������
//---------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Direct3D_Init(HWND hWnd);//Direct3D��ʼ��
HRESULT Objects_Init(HWND hWnd);//��Ҫ���Ƶ�������Դ��ʼ��
VOID Direct3D_Render(HWND hWnd);//Direct3D��Ⱦ�������д
VOID Direct3D_CleanUp(HWND hWnd);//����COM��Դ�Լ�������Դ

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

	//��Դ��ʼ��������ʼʧ�ܣ��򷵻�false
	if (S_OK == Direct3D_Init(hWnd))
		MessageBox(hWnd, L"Direct3d��ʼ�����~", L"���ɵ���Ϣ����", 0);
	else
		MessageBox(hWnd, L"Direct3d��ʼ��ʧ��~", L"���ɵ���Ϣ����", 0);

	//���ڵ��ƶ�����ʾ�����
	MoveWindow(hWnd, 250, 80, WINDOW_WIDTH, WINDOW_HEIGHT, true);//����������ʾλ��
	ShowWindow(hWnd, nCmdShow);//��ʾ����
	UpdateWindow(hWnd);//���´���

	//���ڳ�ʼ���󣬲�������
	//PlaySound(L"Dia Frampton - Walk Away.flac", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	MessageBox(hWnd, L"�ٺ٣����ڵ�����һ��", L"Direct3D", 0);

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
			Direct3D_Render(hWnd);
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
	case WM_PAINT:
		Direct3D_Render(hWnd);
		ValidateRect(hWnd, NULL);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		Direct3D_CleanUp(hWnd);//�����Զ������Դ���������ڴ����˳�ǰ����Դ��������
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, nMsg, wParam, lParam);
	}
	return 0;
}

//---------------------------------------------------------------------------
// ������Direct3D_Init����������Direct3D��ʼ��
//---------------------------------------------------------------------------
HRESULT Direct3D_Init(HWND hWnd)
{
	//����Direct3D�ӿڶ���
	LPDIRECT3D9 pD3D = NULL;
	if (NULL == (pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	//��ȡӲ���豸��Ϣ
	D3DCAPS9 caps;
	int vp = 0;
	if (FAILED(pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
		return E_FAIL;
	if (caps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;//֧��Ӳ����������
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;//��֧��Ӳ����������

	//���D3DPRESENT_PARAMETERS�ṹ��
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

	//����Direct3D�豸�ӿ�
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, vp, &d3dpp, &g_pd3dDevice)))
		return E_FAIL;
	SAFE_RELEASE(pD3D);

	if (!(S_OK == Objects_Init(hWnd)))return E_FAIL;//����Objects_Init������Ҫ���Ƶ�������Դ��ʼ��
	return S_OK;
}

//---------------------------------------------------------------------------
// ������Objects_Init����������Ҫ���Ƶ�������Դ��ʼ��
//---------------------------------------------------------------------------
HRESULT Objects_Init(HWND hWnd)
{
	return S_OK;
}

//---------------------------------------------------------------------------
// ������Direct3D_Render������ʹ��Direct3D��Ⱦ
//---------------------------------------------------------------------------
VOID Direct3D_Render(HWND hWnd)
{
	return VOID();
}

//---------------------------------------------------------------------------
// ����������Direct3D_CleanUp��������Դ�����ڳ����˳�֮ǰ������Դ����
//---------------------------------------------------------------------------
VOID Direct3D_CleanUp(HWND hWnd)
{
	return VOID();
}

