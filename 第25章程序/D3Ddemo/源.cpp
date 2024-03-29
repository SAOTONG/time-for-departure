//=================================================================================
// 描述：源.cpp文件，初步实现了摄像机类的使用，并且绘制出了3维地形和天空盒子
//=================================================================================

#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>
#include <time.h> 
#include "DInput.h"
#include "CameraClass.h"
#include "TerrainClass.h"
#include "SkyBoxClass.h"

#pragma comment(lib,"winmm.lib")     // 调用PlaySound函数所需库文件
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib, "dinput8.lib")  // 使用DirectInput必须包含的头文件，注意这里有8
#pragma comment(lib,"dxguid.lib")

#define WINDOW_WIDTH	932	 // 为窗口宽度定义的宏，以方便在此处修改窗口宽度
#define WINDOW_HEIGHT	700  // 为窗口高度定义的宏，以方便在此处修改窗口高度
#define WINDOW_TITLE	L"绘制三维地形和天空"  // 为窗口标题定义的宏

LPDIRECT3DDEVICE9  g_pd3dDevice = NULL;        // Direct3D设备对象
ID3DXFont*		   g_pFont = NULL;             // 字体COM接口
LPD3DXFONT         g_pTextAdapterName = NULL;  // 2D显卡信息
LPD3DXFONT         g_pTextHelper = NULL;       // 2D帮助信息
LPD3DXFONT         g_pTextInfor = NULL;        // 2D绘制信息

float			   g_FPS = 0.0f;                  // 一个浮点型的变量，代表帧速率
wchar_t			   g_strFPS[50];                  // 包含帧速率的字符数组
wchar_t            g_strAdapterName[60] = { 0 };  // 显卡名称字符数组

D3DXMATRIX 			    g_matWorld;
LPD3DXMESH              g_pMesh = NULL;          // 网格对象
D3DMATERIAL9*           g_pMaterial = NULL;      // 网格的材质信息
DWORD                   g_dwNumMtrls = NULL;     // 材质的数量
LPDIRECT3DTEXTURE9*     g_pTexture = NULL;       // 网格的纹理
LPD3DXMESH              g_pCyliderMesh = NULL;   // 柱子网格对象
D3DMATERIAL9            g_CyliderMaterial;       // 柱子材质
DInput*                 g_pDirectInput = NULL;   // DirectInput封装类对象
CameraClass*            g_pCamera = NULL;        // 摄像机封装类对象
TerrainClass*           g_pTerrain = NULL;       // 地形封装类对象
SkyBoxClass*            g_pSkyBox = NULL;        // 天空盒子封装类对象


// 描述：全局函数声明,防止“未声明的标识”系列错误
// 窗口过程函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
HRESULT			 Direct3D_Init(HWND hwnd, HINSTANCE hInstance);  // 在这个函数中进行Direct3D的初始化
HRESULT			 Objects_Init(HWND hwnd);     // 在这个函数中进行要绘制的物体的资源初始化
VOID             Direct3D_Update(HWND hwnd);  // 在这个函数中进行画面更新操作
VOID			 Direct3D_Render(HWND hwnd);  // 在这个函数中进行Direct3D渲染代码的书写
VOID			 Direct3D_CleanUp();		  // 在这个函数中清理COM资源以及其他资源
float		     Get_FPS();					  // 计算帧数的函数
void			 HelpText_Render(HWND hwnd);  // 绘制帮助信息的函数

// 描述：Windows应用程序的入口函数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//【1】窗口创建四步曲之一：开始设计一个完整的窗口类
	WNDCLASSEX wndClass = { 0 };			   // 用WINDCLASSEX定义了一个窗口类
	wndClass.cbSize = sizeof(WNDCLASSEX);	   // 设置结构体的字节数大小
	wndClass.style = CS_HREDRAW | CS_VREDRAW;  // 设置窗口的样式
	wndClass.lpfnWndProc = WndProc;			   // 设置指向窗口过程函数的指针
	wndClass.cbClsExtra = 0;				   // 窗口类的附加内存，取0就可以了
	wndClass.cbWndExtra = 0;				   // 窗口的附加内存，依然取0就行了
	wndClass.hInstance = hInstance;			   // 指定包含窗口过程的程序的实例句柄。
											   // 本地加载自定义ico图标
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0,
		LR_DEFAULTSIZE | LR_LOADFROMFILE);
	// 指定窗口类的光标句柄。
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	// 为hbrBackground成员指定一个白色画刷句柄	
	wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	// 用一个以空终止的字符串，指定菜单资源的名字。
	wndClass.lpszMenuName = NULL;
	// 用一个以空终止的字符串，指定窗口类的名字。
	wndClass.lpszClassName = L"ForTheDreamOfGameDevelop";

	//【2】窗口创建四步曲之二：注册窗口类
	// 设计完窗口后，需要对窗口类进行注册，这样才能创建该类型的窗口
	if (!RegisterClassEx(&wndClass))
		return -1;

	//【3】窗口创建四步曲之三：正式创建窗口
	// 喜闻乐见的创建窗口函数CreateWindow
	HWND hwnd = CreateWindow(L"ForTheDreamOfGameDevelop", WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

	// Direct3D资源的初始化，调用失败用messagebox予以显示
	if (!(S_OK == Direct3D_Init(hwnd, hInstance)))
	{
		// 使用MessageBox函数，创建一个消息窗口 
		MessageBox(hwnd, _T("Direct3D初始化失败~！"), _T("侯金成的消息窗口"), 0);
	}

	//【4】窗口创建四步曲之四：窗口的移动、显示与更新
	// 调整窗口显示时的位置，使窗口左上角位于（250,80）处
	MoveWindow(hwnd, 250, 80, WINDOW_WIDTH, WINDOW_HEIGHT, true);
	ShowWindow(hwnd, nShowCmd);  // 调用ShowWindow函数来显示窗口
	UpdateWindow(hwnd);			 // 对窗口进行更新，就像我们买了新房子要装修一样

	// 循环播放背景音乐 
	// PlaySound(L"ファングのテーマ.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); 

	g_pDirectInput = new DInput();
	if (!(S_OK == g_pDirectInput->Init(hwnd, hInstance, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		// 使用MessageBox函数，创建一个消息窗口 
		MessageBox(hwnd, _T("DirectInput初始化失败~！"), _T("错误"), 0);
		DestroyWindow(hwnd);
	}


	//【5】消息循环过程
	MSG msg = { 0 };                // 初始化msg
	while (msg.message != WM_QUIT)  // 使用while循环
	{
		// 查看应用程序消息队列，有消息时将队列中的消息派发出去。
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);  // 将虚拟键消息转换为字符消息
			DispatchMessage(&msg);   // 该函数分发一个消息给窗口程序。
		}
		else
		{
			Direct3D_Update(hwnd);   // 进行画面更新
			Direct3D_Render(hwnd);   // 进行渲染
		}
	}
	//【6】窗口类的注销
	// 程序准备结束，注销窗口类
	UnregisterClass(L"ForTheDreamOfGameDevelop", wndClass.hInstance);
	return 0;
}

// 描述：窗口过程函数WndProc,对窗口消息进行处理
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)			   // switch语句开始
	{
	case WM_PAINT:				   // 若是客户区重绘消息
		Direct3D_Render(hwnd);     // 调用Direct3D渲染函数
		ValidateRect(hwnd, NULL);  // 更新客户区的显示
		break;					   // 跳出该switch语句

	case WM_KEYDOWN:			   // 若是键盘按下消息
		if (wParam == VK_ESCAPE)   // 如果被按下的键是ESC
			DestroyWindow(hwnd);   // 销毁窗口, 并发送一条WM_DESTROY消息
		break;					   // 跳出该switch语句

	case WM_DESTROY:			   // 若是窗口销毁消息
		Direct3D_CleanUp();		   // 调用自定义的资源清理函数Game_CleanUp（）进行退出前的资源清理
		PostQuitMessage(0);		   // 向系统表明有个线程有终止请求。用来响应WM_DESTROY消息
		break;					   // 跳出该switch语句

	default:					   // 若上述case条件都不符合，则执行该default语句
		return DefWindowProc(hwnd, message, wParam, lParam);  // 调用缺省的窗口过程
	}

	return 0;					   // 正常退出
}

// 描述：Direct3D初始化函数，进行Direct3D的初始化
HRESULT Direct3D_Init(HWND hwnd, HINSTANCE hInstance)
{

	//【Direct3D初始化四步曲之一,创接口】
	// 创建Direct3D接口对象,以便用该Direct3D对象创建Direct3D设备对象
	LPDIRECT3D9  pD3D = NULL;  // Direct3D接口对象的创建
	// 初始化Direct3D接口对象,并进行DirectX版本协商
	if (NULL == (pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	//【Direct3D初始化四步曲之二,取信息】
	// 获取硬件设备信息
	D3DCAPS9 caps; int vp = 0;
	if (FAILED(pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
	{
		return E_FAIL;
	}
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		// 支持硬件顶点运算,采用硬件顶点运算
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		// 不支持硬件顶点运算,采用软件顶点运算
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//【Direct3D初始化四步曲之三,填内容】：填充D3DPRESENT_PARAMETERS结构体
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


	//【Direct3D初始化四步曲之四,创设备】：创建Direct3D设备接口
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		hwnd, vp, &d3dpp, &g_pd3dDevice)))
		return E_FAIL;

	// 获取显卡信息
	wchar_t HeadName[60] = L"当前显卡型号：";
	D3DADAPTER_IDENTIFIER9 Adapter;
	pD3D->GetAdapterIdentifier(0, 0, &Adapter);
	int len = MultiByteToWideChar(0, 0, Adapter.Description, -1, NULL, 0);
	MultiByteToWideChar(0, 0, Adapter.Description, -1, g_strAdapterName, len);
	wcscat_s(HeadName, g_strAdapterName);
	wcscpy_s(g_strAdapterName, HeadName);

	SAFE_RELEASE(pD3D); // LPDIRECT3D9接口对象的使命完成，我们将其释放掉

						// 调用一次Objects_Init，进行渲染资源的初始化
	if (!(S_OK == Objects_Init(hwnd))) return E_FAIL;

	return S_OK;
}



// 描述：渲染资源初始化函数,在此函数中进行要被渲染的物体的资源的初始化
HRESULT Objects_Init(HWND hwnd)
{
	// 创建字体
	if (FAILED(D3DXCreateFont(g_pd3dDevice, 20, 0, 0, 1000, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("Calibri"),
		&g_pFont)))
		return E_FAIL;
	if (FAILED(D3DXCreateFont(g_pd3dDevice, 20, 0, 0, 1, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("华文中宋"),
		&g_pTextAdapterName)))
		return E_FAIL;
	if (FAILED(D3DXCreateFont(g_pd3dDevice, 20, 0, 0, 1, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("微软雅黑"),
		&g_pTextHelper)))
		return E_FAIL;
	if (FAILED(D3DXCreateFont(g_pd3dDevice, 20, 0, 0, 1, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("黑体"),
		&g_pTextInfor)))
		return E_FAIL;

	// 从X文件中加载网格数据
	LPD3DXBUFFER pAdjBuffer = NULL;
	LPD3DXBUFFER pMtrlBuffer = NULL;
	D3DXLoadMeshFromX(L"WYJ.X", D3DXMESH_MANAGED, g_pd3dDevice,
		&pAdjBuffer, &pMtrlBuffer, NULL, &g_dwNumMtrls, &g_pMesh);

	// 读取材质和纹理数据
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
	}
	pAdjBuffer->Release();
	pMtrlBuffer->Release();

	// 创建地形
	g_pTerrain = new TerrainClass(g_pd3dDevice);
	g_pTerrain->LoadTerrainFromFile(L"heighmap.raw", L"GameMedia\\terrainstone.jpg");
	g_pTerrain->InitTerrain(200, 200, 600.0f, 40.0f);
	
	// 创建天空
	g_pSkyBox = new SkyBoxClass(g_pd3dDevice);
	g_pSkyBox->LoadSkyTextureFromFile(L"GameMedia\\frontsnow1.jpg", 
		L"GameMedia\\backsnow1.jpg", L"GameMedia\\leftsnow1.jpg", 
		L"GameMedia\\rightsnow1.jpg", L"GameMedia\\topsnow1.jpg");
	g_pSkyBox->InitSkyBox(120000);

	// 创建并初始化虚拟摄像机
	g_pCamera = new CameraClass(g_pd3dDevice);
	g_pCamera->SetCameraPosition(&D3DXVECTOR3(0.0f, 20000.0f, -20000.0f));
	g_pCamera->SetTargetPosition(&D3DXVECTOR3(0.0f, 20000.0f, 0.0f));
	g_pCamera->SetViewMatrix();
	g_pCamera->SetProjMatrix();

	// 创建柱子
	D3DXCreateCylinder(g_pd3dDevice, 280.0f, 10.0f, 3000.0f, 60, 60, &g_pCyliderMesh, 0);
	g_CyliderMaterial.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	g_CyliderMaterial.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	g_CyliderMaterial.Specular = D3DXCOLOR(0.5f, 0.0f, 0.3f, 0.3f);
	g_CyliderMaterial.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	// 设置环境光
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Specular = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
	light.Direction = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_pd3dDevice->SetLight(0, &light);
	g_pd3dDevice->LightEnable(0, true);
	g_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, true);

	// 设置纹理过滤和纹理寻址方式
	/*g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);*/

	return S_OK;
}

VOID Direct3D_Update(HWND hwnd)
{
	g_pDirectInput->GetInput();

	// 沿摄像机各分量平移视角
	if (g_pDirectInput->IsKeyButtonDown(DIK_W))g_pCamera->MoveAlongUpVec(10.0f);
	if (g_pDirectInput->IsKeyButtonDown(DIK_A))g_pCamera->MoveAlongRightVec(-10.0f);
	if (g_pDirectInput->IsKeyButtonDown(DIK_S))g_pCamera->MoveAlongUpVec(-10.0f);
	if (g_pDirectInput->IsKeyButtonDown(DIK_D))g_pCamera->MoveAlongRightVec(10.0f);
	if (g_pDirectInput->IsKeyButtonDown(DIK_R))g_pCamera->MoveAlongLookVec(10.0f);
	if (g_pDirectInput->IsKeyButtonDown(DIK_F))g_pCamera->MoveAlongLookVec(-10.0f);

	// 沿摄像机各分量旋转视角
	if (g_pDirectInput->IsKeyButtonDown(DIK_UP))g_pCamera->RotationRightVec(-0.003f);
	if (g_pDirectInput->IsKeyButtonDown(DIK_DOWN))g_pCamera->RotationRightVec(0.003f);
	if (g_pDirectInput->IsKeyButtonDown(DIK_LEFT))g_pCamera->RotationUpVec(-0.003f);
	if (g_pDirectInput->IsKeyButtonDown(DIK_RIGHT))g_pCamera->RotationUpVec(0.003f);
	if (g_pDirectInput->IsKeyButtonDown(DIK_Q))g_pCamera->RotationLookVec(-0.001f);
	if (g_pDirectInput->IsKeyButtonDown(DIK_E))g_pCamera->RotationLookVec(0.001f);

	// 鼠标控制上向量和右向量的旋转
	g_pCamera->RotationUpVec(g_pDirectInput->MouseDX()*0.001f);
	g_pCamera->RotationRightVec(g_pDirectInput->MouseDY()*0.001f);

	// 鼠标滚轮控制观察点收缩操作
	static FLOAT fPosZ = 0.0f;
	fPosZ += (g_pDirectInput->MouseDZ())*0.04f;

	// 计算并设置取景变换矩阵
	D3DXMATRIX matView;
	g_pCamera->CalculateViewMatrix(&matView);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	// 得到正确的世界矩阵
	D3DXMatrixTranslation(&g_matWorld, 0.0f, 0.0f, fPosZ);

	//以下这段代码用于限制鼠标光标移动区域
	POINT lt, rb;
	RECT rect;
	GetClientRect(hwnd, &rect);  //取得窗口内部矩形
								 //将矩形左上点坐标存入lt中
	lt.x = rect.left;
	lt.y = rect.top;
	//将矩形右下坐标存入rb中
	rb.x = rect.right;
	rb.y = rect.bottom;
	//将lt和rb的窗口坐标转换为屏幕坐标
	ClientToScreen(hwnd, &lt);
	ClientToScreen(hwnd, &rb);
	//以屏幕坐标重新设定矩形区域
	rect.left = lt.x;
	rect.top = lt.y;
	rect.right = rb.x;
	rect.bottom = rb.y;
	//限制鼠标光标移动区域
	ClipCursor(&rect);

	ShowCursor(false);	//隐藏鼠标光标

}

// 描述：使用Direct3D进行渲染
void Direct3D_Render(HWND hwnd)
{
	//【Direct3D渲染五步曲之一】：清屏操作
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		D3DCOLOR_XRGB(0, 108, 255), 1.0f, 0);

	//【Direct3D渲染五步曲之二】：开始绘制
	g_pd3dDevice->BeginScene();  // 开始绘制

    // 绘制人物模型
	D3DXMATRIX mScal, mRot1, mRot2, mTrans, mFinal;  // 定义一些矩阵，准备对大黄蜂进行矩阵变换
	D3DXMatrixScaling(&mScal, 100.0f, 100.0f, 100.0f);
	D3DXMatrixTranslation(&mTrans, 0, 8000, 0);

	mFinal = mScal *mTrans*g_matWorld;
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &mFinal);  // 设置模型的世界矩阵，为绘制做准备


	for (DWORD i = 0; i < g_dwNumMtrls; i++)
	{
		g_pd3dDevice->SetMaterial(&g_pMaterial[i]);
		g_pd3dDevice->SetTexture(0, g_pTexture[i]);
		g_pMesh->DrawSubset(i);
	}

	// 绘制地形
	g_pTerrain->RenderTerrain(&g_matWorld, false);

	// 绘制天空
	D3DXMATRIX matSky, matTransSky, matRotSky;
	D3DXMatrixTranslation(&matTransSky, 0.0f, -3500.0f, 0.0f);
	D3DXMatrixRotationY(&matRotSky, -0.000005f*timeGetTime());
	matSky = matTransSky * matRotSky;
	g_pSkyBox->RenderSkyBox(&matSky, false);

	// 绘制柱子
	D3DXMATRIX TransMatrix, RotMatrix, FinalMatrix;
	D3DXMatrixRotationX(&RotMatrix, -D3DX_PI * 0.5f);
	g_pd3dDevice->SetMaterial(&g_CyliderMaterial);
	for (int i = 0; i < 4; i++)
	{
		D3DXMatrixTranslation(&TransMatrix, -10000.0f, 0.0f, -15000.0f + (i * 20000.0f));
		FinalMatrix = RotMatrix * TransMatrix;
		g_pd3dDevice->SetTransform(D3DTS_WORLD, &FinalMatrix);
		g_pCyliderMesh->DrawSubset(0);

		D3DXMatrixTranslation(&TransMatrix, 10000.0f, 0.0f, -15000.0f + (i * 20000.0f));
		FinalMatrix = RotMatrix * TransMatrix;
		g_pd3dDevice->SetTransform(D3DTS_WORLD, &FinalMatrix);
		g_pCyliderMesh->DrawSubset(0);
	}

	HelpText_Render(hwnd);

	//【Direct3D渲染五步曲之四】：结束绘制
	g_pd3dDevice->EndScene();                       // 结束绘制
													//【Direct3D渲染五步曲之五】：显示翻转
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);  // 翻转与显示
}

// 描述：用于计算每秒帧速率的一个函数
float Get_FPS()
{
	// 定义四个静态变量
	static float  fps = 0;             // 我们需要计算的FPS值
	static int     frameCount = 0;     // 帧数
	static float  currentTime = 0.0f;  // 当前时间
	static float  lastTime = 0.0f;     // 持续时间

	frameCount++;  // 每调用一次Get_FPS()函数，帧数自增1
	currentTime = timeGetTime()*0.001f;  // 获取系统时间，其中timeGetTime函数返回的是以毫秒为单位的系统时间，所以需要乘以0.001，得到单位为秒的时间

										 // 如果当前时间减去持续时间大于了1秒钟，就进行一次FPS的计算和持续时间的更新，并将帧数值清零
	if (currentTime - lastTime > 1.0f)   // 将时间控制在1秒钟
	{
		fps = (float)frameCount / (currentTime - lastTime);  // 计算这1秒钟的FPS值
															 // 将当前时间currentTime赋给持续时间lastTime，作为下一秒的基准时间
		lastTime = currentTime;
		frameCount = 0;  // 将本次帧数frameCount值清零
	}

	return fps;
}

void HelpText_Render(HWND hwnd)
{
	RECT formatRect;
	GetClientRect(hwnd, &formatRect);
	//在窗口右上角处，显示每秒帧数
	formatRect.top = 5;
	int charCount = swprintf_s(g_strFPS, 20, _T("FPS:%0.3f"), Get_FPS());
	g_pFont->DrawText(NULL, g_strFPS, charCount, &formatRect, DT_TOP | DT_RIGHT, D3DCOLOR_RGBA(0, 239, 136, 255));

	// 显示显卡类型名
	g_pTextAdapterName->DrawText(NULL, g_strAdapterName, -1, &formatRect,
		DT_TOP | DT_LEFT, D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f));

	// 输出帮助信息
	formatRect.left = 0, formatRect.top = 380;
	g_pTextInfor->DrawText(NULL, L"控制说明:", -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(235, 123, 230, 255));
	formatRect.top += 35;
	g_pTextHelper->DrawText(NULL, L"    W：向前飞翔     S：向后飞翔 ", -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255, 200, 0, 255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    A：向左飞翔     D：向右飞翔", -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255, 200, 0, 255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    R：垂直向上飞翔     F：垂直向下飞翔", -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255, 200, 0, 255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    Q：向左倾斜       E：向右倾斜", -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255, 200, 0, 255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    上、下、左、右方向键、鼠标移动：视角变化 ", -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255, 200, 0, 255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"     鼠标滚轮：人物模型Y轴方向移动", -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255, 200, 0, 255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    ESC键 : 退出程序", -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255, 200, 0, 255));
}

// 描述：资源清理函数，在此函数中进行程序退出前资源的清理工作
void Direct3D_CleanUp()
{
	// 释放COM接口对象
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
