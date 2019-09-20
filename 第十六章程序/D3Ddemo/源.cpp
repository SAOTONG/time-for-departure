//=================================================================================
// ������Դ.cpp�ļ�������ʵ��3D�����������ӳ�䣬���ҿ���ʹ�������̲���������
//=================================================================================

#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>
#include <time.h> 
#include "DInput.h"

#pragma comment(lib,"winmm.lib")     // ����PlaySound����������ļ�
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib, "dinput8.lib")  // ʹ��DirectInput���������ͷ�ļ���ע��������8
#pragma comment(lib,"dxguid.lib")

#define WINDOW_WIDTH	800	 // Ϊ���ڿ�ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڿ��
#define WINDOW_HEIGHT	600  // Ϊ���ڸ߶ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڸ߶�
#define WINDOW_TITLE	L"3D����������ӳ���ʵ��"  // Ϊ���ڱ��ⶨ��ĺ�

struct CUSTOMVERTEX
{
	FLOAT _x, _y, _z;  //���������
	FLOAT _u, _v;      //���������
	CUSTOMVERTEX(FLOAT x, FLOAT y, FLOAT z, FLOAT u, FLOAT v) :_x(x), _y(y), _z(z),
		_u(u), _v(v) {}
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_TEX1)

LPDIRECT3DDEVICE9  g_pd3dDevice = NULL;        // Direct3D�豸����
ID3DXFont*		   g_pFont = NULL;             // ����COM�ӿ�
LPD3DXFONT         g_pTextAdapterName = NULL;  // 2D�Կ���Ϣ
LPD3DXFONT         g_pTextHelper = NULL;       // 2D������Ϣ
LPD3DXFONT         g_pTextInfor = NULL;        // 2D������Ϣ

float			   g_FPS = 0.0f;                  // һ�������͵ı���������֡����
wchar_t			   g_strFPS[50];                  // ����֡���ʵ��ַ�����
wchar_t            g_strAdapterName[60] = { 0 };  // �Կ������ַ�����

D3DXMATRIX 			    g_matWorld;
LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;  // ���㻺�����
LPDIRECT3DINDEXBUFFER9  g_pIndexBuffer = NULL;   // �����������
LPDIRECT3DTEXTURE9      g_pTexture = NULL;       // ���������
DInput*                 g_pDirectInput = NULL;   // DirectInput��װ�����


												 // ������ȫ�ֺ�������,��ֹ��δ�����ı�ʶ��ϵ�д���
												 // ���ڹ��̺���
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
HRESULT			 Direct3D_Init(HWND hwnd, HINSTANCE hInstance);  // ����������н���Direct3D�ĳ�ʼ��
HRESULT			 Objects_Init(HWND hwnd);     // ����������н���Ҫ���Ƶ��������Դ��ʼ��
VOID             Direct3D_Update(HWND hwnd);  // ����������н��л�����²���
VOID			 Direct3D_Render(HWND hwnd);  // ����������н���Direct3D��Ⱦ�������д
VOID			 Direct3D_CleanUp();		  // ���������������COM��Դ�Լ�������Դ
float		     Get_FPS();					  // ����֡���ĺ���
VOID			 Matrix_Set();                // ��װ���Ĵ�任�ĺ���

											  // ������WindowsӦ�ó������ں���
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//��1�����ڴ����Ĳ���֮һ����ʼ���һ�������Ĵ�����
	WNDCLASSEX wndClass = { 0 };			   // ��WINDCLASSEX������һ��������
	wndClass.cbSize = sizeof(WNDCLASSEX);	   // ���ýṹ����ֽ�����С
	wndClass.style = CS_HREDRAW | CS_VREDRAW;  // ���ô��ڵ���ʽ
	wndClass.lpfnWndProc = WndProc;			   // ����ָ�򴰿ڹ��̺�����ָ��
	wndClass.cbClsExtra = 0;				   // ������ĸ����ڴ棬ȡ0�Ϳ�����
	wndClass.cbWndExtra = 0;				   // ���ڵĸ����ڴ棬��Ȼȡ0������
	wndClass.hInstance = hInstance;			   // ָ���������ڹ��̵ĳ����ʵ�������
											   // ���ؼ����Զ���icoͼ��
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0,
		LR_DEFAULTSIZE | LR_LOADFROMFILE);
	// ָ��������Ĺ������
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	// ΪhbrBackground��Աָ��һ����ɫ��ˢ���	
	wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	// ��һ���Կ���ֹ���ַ�����ָ���˵���Դ�����֡�
	wndClass.lpszMenuName = NULL;
	// ��һ���Կ���ֹ���ַ�����ָ������������֡�
	wndClass.lpszClassName = L"ForTheDreamOfGameDevelop";

	//��2�����ڴ����Ĳ���֮����ע�ᴰ����
	// ����괰�ں���Ҫ�Դ��������ע�ᣬ�������ܴ��������͵Ĵ���
	if (!RegisterClassEx(&wndClass))
		return -1;

	//��3�����ڴ����Ĳ���֮������ʽ��������
	// ϲ���ּ��Ĵ������ں���CreateWindow
	HWND hwnd = CreateWindow(L"ForTheDreamOfGameDevelop", WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

	// Direct3D��Դ�ĳ�ʼ��������ʧ����messagebox������ʾ
	if (!(S_OK == Direct3D_Init(hwnd, hInstance)))
	{
		// ʹ��MessageBox����������һ����Ϣ���� 
		MessageBox(hwnd, _T("Direct3D��ʼ��ʧ��~��"), _T("ǳī����Ϣ����"), 0);
	}

	//��4�����ڴ����Ĳ���֮�ģ����ڵ��ƶ�����ʾ�����
	// ����������ʾʱ��λ�ã�ʹ�������Ͻ�λ�ڣ�250,80����
	MoveWindow(hwnd, 250, 80, WINDOW_WIDTH, WINDOW_HEIGHT, true);
	ShowWindow(hwnd, nShowCmd);  // ����ShowWindow��������ʾ����
	UpdateWindow(hwnd);			 // �Դ��ڽ��и��£��������������·���Ҫװ��һ��

								 // ѭ�����ű������� 
								 //PlaySound(L"�ե��󥰤ΥƩ`��.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); 

	g_pDirectInput = new DInput();
	if (!(S_OK == g_pDirectInput->Init(hwnd, hInstance, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		//ʹ��MessageBox����������һ����Ϣ���� 
		MessageBox(hwnd, _T("DirectInput��ʼ��ʧ��~��"), _T("����"), 0);
		DestroyWindow(hwnd);
	}


	//��5����Ϣѭ������
	MSG msg = { 0 };                // ��ʼ��msg
	while (msg.message != WM_QUIT)  // ʹ��whileѭ��
	{
		// �鿴Ӧ�ó�����Ϣ���У�����Ϣʱ�������е���Ϣ�ɷ���ȥ��
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);  // ���������Ϣת��Ϊ�ַ���Ϣ
			DispatchMessage(&msg);   // �ú����ַ�һ����Ϣ�����ڳ���
		}
		else
		{
			Direct3D_Update(hwnd);   // ���л������
			Direct3D_Render(hwnd);   // ������Ⱦ
		}
	}
	//��6���������ע��
	// ����׼��������ע��������
	UnregisterClass(L"ForTheDreamOfGameDevelop", wndClass.hInstance);
	return 0;
}

// ���������ڹ��̺���WndProc,�Դ�����Ϣ���д���
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)			   // switch��俪ʼ
	{
	case WM_PAINT:				   // ���ǿͻ����ػ���Ϣ
		Direct3D_Render(hwnd);     // ����Direct3D��Ⱦ����
		ValidateRect(hwnd, NULL);  // ���¿ͻ�������ʾ
		break;					   // ������switch���

	case WM_KEYDOWN:			   // ���Ǽ��̰�����Ϣ
		if (wParam == VK_ESCAPE)   // ��������µļ���ESC
			DestroyWindow(hwnd);   // ���ٴ���, ������һ��WM_DESTROY��Ϣ
		break;					   // ������switch���

	case WM_DESTROY:			   // ���Ǵ���������Ϣ
		Direct3D_CleanUp();		   // �����Զ������Դ������Game_CleanUp���������˳�ǰ����Դ����
		PostQuitMessage(0);		   // ��ϵͳ�����и��߳�����ֹ����������ӦWM_DESTROY��Ϣ
		break;					   // ������switch���

	default:					   // ������case�����������ϣ���ִ�и�default���
		return DefWindowProc(hwnd, message, wParam, lParam);  // ����ȱʡ�Ĵ��ڹ���
	}

	return 0;					   // �����˳�
}

// ������Direct3D��ʼ������������Direct3D�ĳ�ʼ��
HRESULT Direct3D_Init(HWND hwnd, HINSTANCE hInstance)
{

	//��Direct3D��ʼ���Ĳ���֮һ,���ӿڡ�
	// ����Direct3D�ӿڶ���,�Ա��ø�Direct3D���󴴽�Direct3D�豸����
	LPDIRECT3D9  pD3D = NULL;  // Direct3D�ӿڶ���Ĵ���
							   // ��ʼ��Direct3D�ӿڶ���,������DirectX�汾Э��
	if (NULL == (pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	//��Direct3D��ʼ���Ĳ���֮��,ȡ��Ϣ��
	// ��ȡӲ���豸��Ϣ
	D3DCAPS9 caps; int vp = 0;
	if (FAILED(pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
	{
		return E_FAIL;
	}
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		// ֧��Ӳ����������,����Ӳ����������
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		// ��֧��Ӳ����������,���������������
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//��Direct3D��ʼ���Ĳ���֮��,�����ݡ������D3DPRESENT_PARAMETERS�ṹ��
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = WINDOW_WIDTH;
	d3dpp.BackBufferHeight = WINDOW_HEIGHT;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;


	//��Direct3D��ʼ���Ĳ���֮��,���豸��������Direct3D�豸�ӿ�
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		hwnd, vp, &d3dpp, &g_pd3dDevice)))
		return E_FAIL;

	// ��ȡ�Կ���Ϣ
	wchar_t HeadName[60] = L"��ǰ�Կ��ͺţ�";
	D3DADAPTER_IDENTIFIER9 Adapter;
	pD3D->GetAdapterIdentifier(0, 0, &Adapter);
	int len = MultiByteToWideChar(0, 0, Adapter.Description, -1, NULL, 0);
	MultiByteToWideChar(0, 0, Adapter.Description, -1, g_strAdapterName, len);
	wcscat_s(HeadName, g_strAdapterName);
	wcscpy_s(g_strAdapterName, HeadName);

	SAFE_RELEASE(pD3D) // LPDIRECT3D9�ӿڶ����ʹ����ɣ����ǽ����ͷŵ�

					   // ����һ��Objects_Init��������Ⱦ��Դ�ĳ�ʼ��
		if (!(S_OK == Objects_Init(hwnd))) return E_FAIL;

	return S_OK;
}



// ��������Ⱦ��Դ��ʼ������,�ڴ˺����н���Ҫ����Ⱦ���������Դ�ĳ�ʼ��
HRESULT Objects_Init(HWND hwnd)
{
	// ��������
	if (FAILED(D3DXCreateFont(g_pd3dDevice, 20, 0, 0, 1000, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("Calibri"),
		&g_pFont)))
		return E_FAIL;
	if (FAILED(D3DXCreateFont(g_pd3dDevice, 20, 0, 0, 1, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("��������"),
		&g_pTextAdapterName)))
		return E_FAIL;
	if (FAILED(D3DXCreateFont(g_pd3dDevice, 20, 0, 0, 1, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("΢���ź�"),
		&g_pTextHelper)))
		return E_FAIL;
	if (FAILED(D3DXCreateFont(g_pd3dDevice, 20, 0, 0, 1, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("����"),
		&g_pTextInfor)))
		return E_FAIL;

	// TODO(1053001374@qq.com): Use a "*" here for concatenation operator.
	// TODO(����) change this to use relations.
	// TODO(���ƴ��룺�������㻺�棬��Ӧ�鱾p464): remove the "Last visitors" feature
	// �������㻺��
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(24 * sizeof(CUSTOMVERTEX), 0,
		D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT,
		&g_pVertexBuffer, NULL)))
		return E_FAIL;
	// ������������
	if (FAILED(g_pd3dDevice->CreateIndexBuffer(36 * sizeof(WORD), 0, D3DFMT_INDEX16,
		D3DPOOL_DEFAULT, &g_pIndexBuffer, NULL)))
		return E_FAIL;

	// ��䶥�㻺��
	CUSTOMVERTEX* pVertex;
	if (FAILED(g_pVertexBuffer->Lock(0, 24 * sizeof(CUSTOMVERTEX), (void**)&pVertex, 0)))
		return E_FAIL;
	// ���涥������
	pVertex[0] = CUSTOMVERTEX(-10.0f, 10.0f, -10.0f, 0.0f, 0.0f);
	pVertex[1] = CUSTOMVERTEX(10.0f, 10.0f, -10.0f, 1.0f, 0.0f);
	pVertex[2] = CUSTOMVERTEX(10.0f, -10.0f, -10.0f, 1.0f, 1.0f);
	pVertex[3] = CUSTOMVERTEX(-10.0f, -10.0f, -10.0f, 0.0f, 1.0f);

	// ���涥������
	pVertex[4] = CUSTOMVERTEX(10.0f, 10.0f, 10.0f, 0.0f, 0.0f);
	pVertex[5] = CUSTOMVERTEX(-10.0f, 10.0f, 10.0f, 1.0f, 0.0f);
	pVertex[6] = CUSTOMVERTEX(-10.0f, -10.0f, 10.0f, 1.0f, 1.0f);
	pVertex[7] = CUSTOMVERTEX(10.0f, -10.0f, 10.0f, 0.0f, 1.0f);

	// ���涥������
	pVertex[8] = CUSTOMVERTEX(-10.0f, 10.0f, 10.0f, 0.0f, 0.0f);
	pVertex[9] = CUSTOMVERTEX(10.0f, 10.0f, 10.0f, 1.0f, 0.0f);
	pVertex[10] = CUSTOMVERTEX(10.0f, 10.0f, -10.0f, 1.0f, 1.0f);
	pVertex[11] = CUSTOMVERTEX(-10.0f, 10.0f, -10.0f, 0.0f, 1.0f);

	// ���涥������
	pVertex[12] = CUSTOMVERTEX(-10.0f, -10.0f, -10.0f, 0.0f, 0.0f);
	pVertex[13] = CUSTOMVERTEX(10.0f, -10.0f, -10.0f, 1.0f, 0.0f);
	pVertex[14] = CUSTOMVERTEX(10.0f, -10.0f, 10.0f, 1.0f, 1.0f);
	pVertex[15] = CUSTOMVERTEX(-10.0f, -10.0f, 10.0f, 0.0f, 1.0f);

	// ���涥������
	pVertex[16] = CUSTOMVERTEX(-10.0f, 10.0f, 10.0f, 0.0f, 0.0f);
	pVertex[17] = CUSTOMVERTEX(-10.0f, 10.0f, -10.0f, 1.0f, 0.0f);
	pVertex[18] = CUSTOMVERTEX(-10.0f, -10.0f, -10.0f, 1.0f, 1.0f);
	pVertex[19] = CUSTOMVERTEX(-10.0f, -10.0f, 10.0f, 0.0f, 1.0f);

	// ���涥������
	pVertex[20] = CUSTOMVERTEX(10.0f, 10.0f, -10.0f, 0.0f, 0.0f);
	pVertex[21] = CUSTOMVERTEX(10.0f, 10.0f, 10.0f, 1.0f, 0.0f);
	pVertex[22] = CUSTOMVERTEX(10.0f, -10.0f, 10.0f, 1.0f, 1.0f);
	pVertex[23] = CUSTOMVERTEX(10.0f, -10.0f, -10.0f, 0.0f, 1.0f);
	g_pVertexBuffer->Unlock();

	// �����������
	WORD* p_Index = NULL;
	if (FAILED(g_pIndexBuffer->Lock(0, 0, (void**)&p_Index, 0)))
		return E_FAIL;
	p_Index[0] = 0; p_Index[1] = 1; p_Index[2] = 2;
	p_Index[3] = 0; p_Index[4] = 2; p_Index[5] = 3;
	p_Index[6] = 4; p_Index[7] = 5; p_Index[8] = 6;
	p_Index[9] = 4; p_Index[10] = 6; p_Index[11] = 7;
	p_Index[12] = 8; p_Index[13] = 9; p_Index[14] = 10;
	p_Index[15] = 8; p_Index[16] = 10; p_Index[17] = 11;
	p_Index[18] = 12; p_Index[19] = 13; p_Index[20] = 14;
	p_Index[21] = 12; p_Index[22] = 14; p_Index[23] = 15;
	p_Index[24] = 16; p_Index[25] = 17; p_Index[26] = 18;
	p_Index[27] = 16; p_Index[28] = 18; p_Index[29] = 19;
	p_Index[30] = 20; p_Index[31] = 21; p_Index[32] = 22;
	p_Index[33] = 20; p_Index[34] = 22; p_Index[35] = 23;
	g_pIndexBuffer->Unlock();

	// ����Ĵ���
	D3DXCreateTextureFromFile(g_pd3dDevice, L"pal5q.jpg", &g_pTexture);

	// ���ò���
	D3DMATERIAL9 mtrl;
	::ZeroMemory(&mtrl, sizeof(mtrl));
	mtrl.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mtrl.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mtrl.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_pd3dDevice->SetMaterial(&mtrl);

	// ���ù���
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));
	light.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	g_pd3dDevice->SetLight(0, &light);   // ���ù���
	g_pd3dDevice->LightEnable(0, true);  // ���ù���

										 // ������Ⱦ״̬
	g_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);  // ��ʼ�����㷨��
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);   // ������������
																 // ���û�����
	g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(36, 36, 36));

	return S_OK;
}

VOID Direct3D_Update(HWND hwnd)
{
	g_pDirectInput->GetInput();

	if (g_pDirectInput->IsKeyButtonDown(DIK_1))
		g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	if (g_pDirectInput->IsMouseButtonDown(DIK_2))
		g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	static FLOAT fPosX = 0.0f, fPosY = 0.0f, fPosZ = 0.0f;

	if (g_pDirectInput->IsMouseButtonDown(0))
	{
		fPosX += (g_pDirectInput->MouseDX()) *  0.08f;
		fPosY += (g_pDirectInput->MouseDY()) * -0.08f;
	}
	fPosZ += (g_pDirectInput->MouseDZ())*0.04f;

	if (g_pDirectInput->IsKeyButtonDown(DIK_W))fPosY += 0.1f;
	if (g_pDirectInput->IsKeyButtonDown(DIK_A))fPosX -= 0.1f;
	if (g_pDirectInput->IsKeyButtonDown(DIK_S))fPosY -= 0.1f;
	if (g_pDirectInput->IsKeyButtonDown(DIK_D))fPosX += 0.1f;

	D3DXMatrixTranslation(&g_matWorld, fPosX, fPosY, fPosZ);

	static float fAngleX = 0.15f, fAngleY = -(float)D3DX_PI;
	if (g_pDirectInput->IsMouseButtonDown(1))
	{
		fAngleX += (g_pDirectInput->MouseDY()) * -0.01f;
		fAngleY += (g_pDirectInput->MouseDX()) * -0.01f;
	}

	if (g_pDirectInput->IsKeyButtonDown(DIK_UP))fAngleX += 0.025f;
	if (g_pDirectInput->IsKeyButtonDown(DIK_DOWN))fAngleX -= 0.025f;
	if (g_pDirectInput->IsKeyButtonDown(DIK_LEFT))fAngleY -= 0.025f;
	if (g_pDirectInput->IsKeyButtonDown(DIK_RIGHT))fAngleY += 0.025f;

	D3DXMATRIX Rx, Ry;
	D3DXMatrixRotationX(&Rx, fAngleX);
	D3DXMatrixRotationY(&Ry, fAngleY);
	g_matWorld = Rx * Ry*g_matWorld;
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &g_matWorld);
	Matrix_Set();
}

// ��������װ��Direct3D�Ĵ�任�ĺ�����������任��ȡ���任��ͶӰ�任���ӿڱ任������
VOID Matrix_Set()
{
	//���Ĵ�任֮һ��������任���������

	//���Ĵ�任֮������ȡ���任���������
	D3DXMATRIX matView;  // ����һ������
	D3DXVECTOR3 vEye(0.0f, 0.0f, -50.0f);  // �������λ��
	D3DXVECTOR3 vAt(0.0f, 0.0f, 0.0f);       // �۲���λ��
	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);       // ���ϵ�����
	D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);   // �����ȡ���任����
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);  // Ӧ��ȡ���任����

													   //���Ĵ�任֮������ͶӰ�任���������
	D3DXMATRIX matProj;  // ����һ������
						 // ����ͶӰ�任����
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f,
		(float)((double)WINDOW_WIDTH / WINDOW_HEIGHT),
		1.0f, 1000.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);  // ����ͶӰ�任����

															 //���Ĵ�任֮�ġ����ӿڱ任������
	D3DVIEWPORT9 vp;  // ʵ����һ��D3DVIEWPORT9�ṹ�壬Ȼ��������������������ֵ�Ϳ�����
	vp.X = 0;		  // ��ʾ�ӿ�����ڴ��ڵ�X����
	vp.Y = 0;		  // �ӿ���ԶԴ��ڵ�Y����
	vp.Width = WINDOW_WIDTH;	// �ӿڵĿ��
	vp.Height = WINDOW_HEIGHT;  // �ӿڵĸ߶�
	vp.MinZ = 0.0f;  // �ӿ�����Ȼ����е���С���ֵ
	vp.MaxZ = 1.0f;	 // �ӿ�����Ȼ����е�������ֵ
	g_pd3dDevice->SetViewport(&vp);  // �ӿڵ�����

}

// ������ʹ��Direct3D������Ⱦ
void Direct3D_Render(HWND hwnd)
{
	//��Direct3D��Ⱦ�岽��֮һ������������
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(100, 100, 100), 1.0f, 0);

	// ����һ�����Σ����ڻ�ȡ�����ھ���
	RECT formatRect;
	GetClientRect(hwnd, &formatRect);
	//��Direct3D��Ⱦ�岽��֮��������ʼ����

	g_pd3dDevice->BeginScene();  // ��ʼ����

								 // ���ö��㻺�����ͼ��
								 // �Ѱ����˼�������Ϣ�Ķ��㻺������Ⱦ��ˮ�������
	g_pd3dDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(CUSTOMVERTEX));
	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);  // ���������ʽ
	g_pd3dDevice->SetIndices(g_pIndexBuffer);   // ������������

												// ��������
	g_pd3dDevice->SetTexture(0, g_pTexture);
	// ���ö��㻺����������������ͼ��
	g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12);

	g_pTextAdapterName->DrawText(NULL, g_strAdapterName, -1,
		&formatRect, DT_TOP | DT_LEFT,
		D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f));

	formatRect.top = 30;
	static wchar_t strInfo[256] = { 0 };
	swprintf_s(strInfo, -1, L"ģ�����꣺(%.2f,%.2f,%.2f)", g_matWorld._41,
		g_matWorld._42, g_matWorld._43);
	g_pTextHelper->DrawText(NULL, strInfo, -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT,
		D3DCOLOR_RGBA(135, 239, 136, 255));
	// ���������Ϣ
	formatRect.left = 0, formatRect.top = 380;
	g_pTextInfor->DrawText(NULL, L"����˵��:", -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT,
		D3DCOLOR_RGBA(235, 123, 230, 255));
	formatRect.top += 35;
	g_pTextHelper->DrawText(NULL, L"    ��ס���������϶���ƽ��ģ��", -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT,
		D3DCOLOR_RGBA(255, 200, 0, 255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    ��ס����Ҽ����϶�����תģ��", -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT,
		D3DCOLOR_RGBA(255, 200, 0, 255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    ���������֣�����ģ��", -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT,
		D3DCOLOR_RGBA(255, 200, 0, 255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    W��S��A��D����ƽ��ģ�� ", -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT,
		D3DCOLOR_RGBA(255, 200, 0, 255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    �ϡ��¡����ҷ��������תģ�� ", -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT,
		D3DCOLOR_RGBA(255, 200, 0, 255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    ESC�� : �˳�����", -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT,
		D3DCOLOR_RGBA(255, 200, 0, 255));

	// �ڴ������ϽǴ�����ʾÿ��֡��
	int charCount = swprintf_s(g_strFPS, 20, _T("FPS:%0.3f"), Get_FPS());
	g_pFont->DrawText(NULL, g_strFPS, charCount, &formatRect, DT_TOP | DT_RIGHT,
		D3DCOLOR_XRGB(255, 39, 136));

	//��Direct3D��Ⱦ�岽��֮�ġ�����������
	g_pd3dDevice->EndScene();                       // ��������
													//��Direct3D��Ⱦ�岽��֮�塿����ʾ��ת
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);  // ��ת����ʾ
}

// ���������ڼ���ÿ��֡���ʵ�һ������
float Get_FPS()
{
	// �����ĸ���̬����
	static float  fps = 0;             // ������Ҫ�����FPSֵ
	static int     frameCount = 0;     // ֡��
	static float  currentTime = 0.0f;  // ��ǰʱ��
	static float  lastTime = 0.0f;     // ����ʱ��

	frameCount++;  // ÿ����һ��Get_FPS()������֡������1
	currentTime = timeGetTime()*0.001f;  // ��ȡϵͳʱ�䣬����timeGetTime�������ص����Ժ���Ϊ��λ��ϵͳʱ�䣬������Ҫ����0.001���õ���λΪ���ʱ��

										 // �����ǰʱ���ȥ����ʱ�������1���ӣ��ͽ���һ��FPS�ļ���ͳ���ʱ��ĸ��£�����֡��ֵ����
	if (currentTime - lastTime > 1.0f)   // ��ʱ�������1����
	{
		fps = (float)frameCount / (currentTime - lastTime);  // ������1���ӵ�FPSֵ
															 // ����ǰʱ��currentTime��������ʱ��lastTime����Ϊ��һ��Ļ�׼ʱ��
		lastTime = currentTime;
		frameCount = 0;  // ������֡��frameCountֵ����
	}

	return fps;
}

// ��������Դ���������ڴ˺����н��г����˳�ǰ��Դ��������
void Direct3D_CleanUp()
{
	SAFE_DELETE(g_pDirectInput);
	SAFE_RELEASE(g_pVertexBuffer);
	SAFE_RELEASE(g_pIndexBuffer);
	SAFE_RELEASE(g_pTexture);
	SAFE_RELEASE(g_pTextAdapterName);
	SAFE_RELEASE(g_pTextHelper);
	SAFE_RELEASE(g_pTextInfor);
	SAFE_RELEASE(g_pFont);
	SAFE_RELEASE(g_pd3dDevice);
}
