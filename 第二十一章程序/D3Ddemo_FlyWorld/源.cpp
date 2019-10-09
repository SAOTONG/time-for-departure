//=================================================================================
// ������Դ.cpp�ļ�������ʵ������������ʹ�ã��������������ά�ռ����ɰ���
//=================================================================================

#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>
#include <time.h> 
#include "DInput.h"
#include "CameraClass.h"

#pragma comment(lib,"winmm.lib")     // ����PlaySound����������ļ�
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib, "dinput8.lib")  // ʹ��DirectInput���������ͷ�ļ���ע��������8
#pragma comment(lib,"dxguid.lib")

#define WINDOW_WIDTH	932	 // Ϊ���ڿ�ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڿ��
#define WINDOW_HEIGHT	700  // Ϊ���ڸ߶ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڸ߶�
#define WINDOW_TITLE	L"�������������ά�ռ����ɰ���"  // Ϊ���ڱ��ⶨ��ĺ�

struct CUSTOMVERTEX
{
	FLOAT _x, _y, _z;     // ���������
	FLOAT _nx, _ny, _nz;  // 
	FLOAT _u, _v;         // ���������
						  // ���캯��
	CUSTOMVERTEX(FLOAT x, FLOAT y, FLOAT z, FLOAT nx, FLOAT ny, FLOAT nz,
		FLOAT u, FLOAT v) :_x(x), _y(y), _z(z), _nx(nx), _ny(ny), _nz(nz),
		_u(u), _v(v) {}
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

LPDIRECT3DDEVICE9  g_pd3dDevice = NULL;        // Direct3D�豸����
ID3DXFont*		   g_pFont = NULL;             // ����COM�ӿ�
LPD3DXFONT         g_pTextAdapterName = NULL;  // 2D�Կ���Ϣ
LPD3DXFONT         g_pTextHelper = NULL;       // 2D������Ϣ
LPD3DXFONT         g_pTextInfor = NULL;        // 2D������Ϣ

float			   g_FPS = 0.0f;                  // һ�������͵ı���������֡����
wchar_t			   g_strFPS[50];                  // ����֡���ʵ��ַ�����
wchar_t            g_strAdapterName[60] = { 0 };  // �Կ������ַ�����

D3DXMATRIX 			    g_matWorld;
LPD3DXMESH              g_pMesh = NULL;          // �������
D3DMATERIAL9*           g_pMaterial = NULL;      // ����Ĳ�����Ϣ
DWORD                   g_dwNumMtrls = NULL;     // ���ʵ�����
LPDIRECT3DTEXTURE9*     g_pTexture = NULL;       // ���������
LPD3DXMESH              g_pCyliderMesh = NULL;       // �����������
D3DMATERIAL9            g_CyliderMaterial;       // ���Ӳ���
DInput*                 g_pDirectInput = NULL;   // DirectInput��װ�����
CameraClass*            g_pCamera = NULL;        // �������װ�����
LPDIRECT3DVERTEXBUFFER9 g_pGrassVertexBuffer = NULL;  // �������Ʋ�ƺ�Ķ��㻺��
LPDIRECT3DTEXTURE9      g_pGrassTexture = NULL;       // �������Ʋ�ƺ���������

													  // ������ȫ�ֺ�������,��ֹ��δ�����ı�ʶ��ϵ�д���
													  // ���ڹ��̺���
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
HRESULT			 Direct3D_Init(HWND hwnd, HINSTANCE hInstance);  // ����������н���Direct3D�ĳ�ʼ��
HRESULT			 Objects_Init(HWND hwnd);     // ����������н���Ҫ���Ƶ��������Դ��ʼ��
VOID             Direct3D_Update(HWND hwnd);  // ����������н��л�����²���
VOID			 Direct3D_Render(HWND hwnd);  // ����������н���Direct3D��Ⱦ�������д
VOID			 Direct3D_CleanUp();		  // ���������������COM��Դ�Լ�������Դ
float		     Get_FPS();					  // ����֡���ĺ���
void			 HelpText_Render(HWND hwnd);  // ���ư�����Ϣ�ĺ���

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
								 // PlaySound(L"�ե��󥰤ΥƩ`��.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); 

	g_pDirectInput = new DInput();
	if (!(S_OK == g_pDirectInput->Init(hwnd, hInstance, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		// ʹ��MessageBox����������һ����Ϣ���� 
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

	SAFE_RELEASE(pD3D); // LPDIRECT3D9�ӿڶ����ʹ����ɣ����ǽ����ͷŵ�

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

	// ��X�ļ��м�����������
	LPD3DXBUFFER pAdjBuffer = NULL;
	LPD3DXBUFFER pMtrlBuffer = NULL;
	D3DXLoadMeshFromX(L"WYJ.X", D3DXMESH_MANAGED, g_pd3dDevice,
		&pAdjBuffer, &pMtrlBuffer, NULL, &g_dwNumMtrls, &g_pMesh);

	// ��ȡ���ʺ���������
	D3DXMATERIAL *pMtrls = (D3DXMATERIAL*)pMtrlBuffer->GetBufferPointer();
	g_pTexture = new LPDIRECT3DTEXTURE9[g_dwNumMtrls];
	g_pMaterial = new D3DMATERIAL9[g_dwNumMtrls];

	for (DWORD i = 0; i<g_dwNumMtrls; i++)
	{
		g_pMaterial[i] = pMtrls[i].MatD3D;
		g_pMaterial[i].Ambient = g_pMaterial[i].Diffuse;
		g_pTexture[i] = NULL;
		D3DXCreateTextureFromFileA(g_pd3dDevice,
			pMtrls[i].pTextureFilename, &g_pTexture[i]);
		//g_pMaterial[i].Diffuse.a = 0.3f;  // ���ò��ʵ�Alpha�ķ���

	}
	pAdjBuffer->Release();
	pMtrlBuffer->Release();

	// ����һƬ��ƺ,50x50=250������
	g_pd3dDevice->CreateVertexBuffer(4 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_MANAGED, &g_pGrassVertexBuffer, 0);

	CUSTOMVERTEX* pVertex = NULL;
	g_pGrassVertexBuffer->Lock(0, 0, (void**)&pVertex, 0);
	pVertex[0] = CUSTOMVERTEX(-500.0f, 0.0f, -500.0f, 0.0f, 1.0f, 0.0f, 0.0f, 50.0f);
	pVertex[1] = CUSTOMVERTEX(-500.0f, 0.0f, 500.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	pVertex[2] = CUSTOMVERTEX(500.0f, 0.0f, -500.0f, 0.0f, 1.0f, 0.0f, 50.0f, 50.0f);
	pVertex[3] = CUSTOMVERTEX(500.0f, 0.0f, 500.0f, 0.0f, 1.0f, 0.0f, 50.0f, 0.0f);
	g_pGrassVertexBuffer->Unlock();

	// �����ذ�����
	D3DXCreateTextureFromFile(g_pd3dDevice, L"grass.jpg", &g_pGrassTexture);

	// ��������
	D3DXCreateCylinder(g_pd3dDevice, 10.0f, 10.0f, 500.0f, 60, 60, &g_pCyliderMesh, 0);
	g_CyliderMaterial.Ambient = D3DXCOLOR(0.9f, 0.0f, 0.8f, 1.0f);
	g_CyliderMaterial.Diffuse = D3DXCOLOR(0.9f, 0.0f, 0.8f, 1.0f);
	g_CyliderMaterial.Specular = D3DXCOLOR(0.9f, 0.2f, 0.9f, 0.9f);
	g_CyliderMaterial.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.9f, 1.0f);

	// ���û�����
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Specular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	light.Direction = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	g_pd3dDevice->SetLight(0, &light);
	g_pd3dDevice->LightEnable(0, true);
	g_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, true);

	// ��������ʼ�����������
	g_pCamera = new CameraClass(g_pd3dDevice);
	g_pCamera->SetCameraPosition(&D3DXVECTOR3(0.0f, 200.0f, -300.0f));
	g_pCamera->SetTargetPosition(&D3DXVECTOR3(0.0f, 300.0f, 0.0f));
	g_pCamera->SetViewMatrix();
	g_pCamera->SetProjMatrix();

	// ����������˺�����Ѱַ��ʽ
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

VOID Direct3D_Update(HWND hwnd)
{
	g_pDirectInput->GetInput();

	// �������������ƽ���ӽ�
	if (g_pDirectInput->IsKeyButtonDown(DIK_W))g_pCamera->MoveAlongUpVec(0.3f);
	if (g_pDirectInput->IsKeyButtonDown(DIK_A))g_pCamera->MoveAlongRightVec(-0.3f);
	if (g_pDirectInput->IsKeyButtonDown(DIK_S))g_pCamera->MoveAlongUpVec(-0.3f);
	if (g_pDirectInput->IsKeyButtonDown(DIK_D))g_pCamera->MoveAlongRightVec(0.3f);
	if (g_pDirectInput->IsKeyButtonDown(DIK_R))g_pCamera->MoveAlongLookVec(0.3f);
	if (g_pDirectInput->IsKeyButtonDown(DIK_F))g_pCamera->MoveAlongLookVec(-0.3f);

	// ���������������ת�ӽ�
	if (g_pDirectInput->IsKeyButtonDown(DIK_UP))g_pCamera->RotationRightVec(-0.003f);
	if (g_pDirectInput->IsKeyButtonDown(DIK_DOWN))g_pCamera->RotationRightVec(0.003f);
	if (g_pDirectInput->IsKeyButtonDown(DIK_LEFT))g_pCamera->RotationUpVec(-0.003f);
	if (g_pDirectInput->IsKeyButtonDown(DIK_RIGHT))g_pCamera->RotationUpVec(0.003f);
	if (g_pDirectInput->IsKeyButtonDown(DIK_Q))g_pCamera->RotationLookVec(-0.001f);
	if (g_pDirectInput->IsKeyButtonDown(DIK_E))g_pCamera->RotationLookVec(0.001f);

	// ������������������������ת
	g_pCamera->RotationUpVec(g_pDirectInput->MouseDX()*0.001f);
	g_pCamera->RotationRightVec(g_pDirectInput->MouseDY()*0.001f);

	// �����ֿ��ƹ۲����������
	static FLOAT fPosZ = 0.0f;
	fPosZ += (g_pDirectInput->MouseDZ())*0.04f;

	// ���㲢����ȡ���任����
	D3DXMATRIX matView;
	g_pCamera->CalculateViewMatrix(&matView);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	// �õ���ȷ���������
	D3DXMatrixTranslation(&g_matWorld, 0.0f, 0.0f, fPosZ);

	ShowCursor(false);  // ���������

}

// ������ʹ��Direct3D������Ⱦ
void Direct3D_Render(HWND hwnd)
{
	//��Direct3D��Ⱦ�岽��֮һ������������
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		D3DCOLOR_XRGB(50, 100, 250), 1.0f, 0);

	//��Direct3D��Ⱦ�岽��֮��������ʼ����
	g_pd3dDevice->BeginScene();  // ��ʼ����

	g_pd3dDevice->SetTransform(D3DTS_WORLD, &g_matWorld);

	for (DWORD i = 0; i < g_dwNumMtrls; i++)
	{
		g_pd3dDevice->SetMaterial(&g_pMaterial[i]);
		g_pd3dDevice->SetTexture(0, g_pTexture[i]);
		g_pMesh->DrawSubset(i);
	}

	// ���Ʋ�ƺ
	D3DXMATRIX matWorld;
	D3DXMatrixTranslation(&matWorld, 0.0f, 0.0f, 0.0f);
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
	g_pd3dDevice->SetTexture(0, g_pGrassTexture);
	g_pd3dDevice->SetStreamSource(0, g_pGrassVertexBuffer, 0, sizeof(CUSTOMVERTEX));
	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// ��������
	D3DXMATRIX TransMatrix, RotMatrix, FinalMatrix;
	D3DXMatrixRotationX(&RotMatrix, -D3DX_PI * 0.5f);
	g_pd3dDevice->SetMaterial(&g_CyliderMaterial);
	for (int i = 0; i < 6; i++)
	{
		D3DXMatrixTranslation(&TransMatrix, -100.0f, 0.0f, -150.0f + (i * 75.0f));
		FinalMatrix = RotMatrix * TransMatrix;
		g_pd3dDevice->SetTransform(D3DTS_WORLD, &FinalMatrix);
		g_pCyliderMesh->DrawSubset(0);

		D3DXMatrixTranslation(&TransMatrix, 100.0f, 0.0f, -150.0f + (i * 75.0f));
		FinalMatrix = RotMatrix * TransMatrix;
		g_pd3dDevice->SetTransform(D3DTS_WORLD, &FinalMatrix);
		g_pCyliderMesh->DrawSubset(0);
	}

	HelpText_Render(hwnd);

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

void HelpText_Render(HWND hwnd)
{
	RECT formatRect;
	GetClientRect(hwnd, &formatRect);
	//�ڴ������ϽǴ�����ʾÿ��֡��
	formatRect.top = 5;
	int charCount = swprintf_s(g_strFPS, 20, _T("FPS:%0.3f"), Get_FPS());
	g_pFont->DrawText(NULL, g_strFPS, charCount, &formatRect, DT_TOP | DT_RIGHT, D3DCOLOR_RGBA(0, 239, 136, 255));

	//��ʾ�Կ�������
	g_pTextAdapterName->DrawText(NULL, g_strAdapterName, -1, &formatRect,
		DT_TOP | DT_LEFT, D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f));

	// ���������Ϣ
	formatRect.left = 0, formatRect.top = 380;
	g_pTextInfor->DrawText(NULL, L"����˵��:", -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(235, 123, 230, 255));
	formatRect.top += 35;
	g_pTextHelper->DrawText(NULL, L"    W����ǰ����     S�������� ", -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255, 200, 0, 255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    A���������     D�����ҷ���", -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255, 200, 0, 255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    R����ֱ���Ϸ���     F����ֱ���·���", -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255, 200, 0, 255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    Q��������б       E��������б", -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255, 200, 0, 255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    �ϡ��¡����ҷ����������ƶ����ӽǱ仯 ", -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255, 200, 0, 255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"     �����֣�����ģ��Y�᷽���ƶ�", -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255, 200, 0, 255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    ESC�� : �˳�����", -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255, 200, 0, 255));
}

// ��������Դ���������ڴ˺����н��г����˳�ǰ��Դ��������
void Direct3D_CleanUp()
{
	//�ͷ�COM�ӿڶ���
	for (DWORD i = 0; i<g_dwNumMtrls; i++)
		SAFE_RELEASE(g_pTexture[i]);

	SAFE_DELETE(g_pTexture);
	SAFE_DELETE(g_pMaterial);
	SAFE_DELETE(g_pDirectInput);
	SAFE_RELEASE(g_pMesh);
	SAFE_RELEASE(g_pd3dDevice);
	SAFE_RELEASE(g_pTextAdapterName);
	SAFE_RELEASE(g_pTextHelper);
	SAFE_RELEASE(g_pTextInfor);
	SAFE_RELEASE(g_pFont);
	SAFE_RELEASE(g_pd3dDevice);
	SAFE_RELEASE(g_pCyliderMesh);
}
