//=================================================================================
// 描述：DInput.cpp文件，DInput类函数的实现
//=================================================================================
#include "DInput.h"

DInput::DInput()
{
	m_pDirectInput = NULL;
	m_pMouseDevice = NULL;
	m_pKeyboardDevice = NULL;
	ZeroMemory(&m_KeyBuffer, sizeof(m_KeyBuffer));
	ZeroMemory(&m_MouseState, sizeof(m_MouseState));
}

DInput::~DInput()
{
	if (m_pKeyboardDevice != NULL)
		m_pKeyboardDevice->Unacquire();
	if (m_pMouseDevice != NULL)
		m_pMouseDevice->Unacquire();
	SAFE_RELEASE(m_pKeyboardDevice);
	SAFE_RELEASE(m_pMouseDevice);
	SAFE_RELEASE(m_pDirectInput);
}

//-----------------------------------【Init( )函数】-------------------------------------------
// 描述：初始化DirectInput接口、键盘和鼠标设备三个对象
//------------------------------------------------------------------------------------------------
HRESULT DInput::Init(HWND hWnd, HINSTANCE hInstance, DWORD keyboardCoopFlags,
	DWORD mouseCoopFlags)
{
	HRESULT hr;
	// 初始化一个IDirectInput8接口对象
	HR(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&m_pDirectInput, NULL));
	// 对键盘设备进行初始化
	HR(m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboardDevice, NULL));
	HR(m_pKeyboardDevice->SetCooperativeLevel(hWnd, keyboardCoopFlags));
	HR(m_pKeyboardDevice->SetDataFormat(&c_dfDIKeyboard));
	HR(m_pKeyboardDevice->Acquire());
	HR(m_pKeyboardDevice->Poll());
	// 对鼠标设备进行初始化
	HR(m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouseDevice, NULL));
	HR(m_pMouseDevice->SetCooperativeLevel(hWnd, mouseCoopFlags));
	HR(m_pMouseDevice->SetDataFormat(&c_dfDIMouse));
	HR(m_pMouseDevice->Acquire());
	HR(m_pMouseDevice->Poll());
	return S_OK;
}

//-----------------------------------【GetInput( )函数】-------------------------------------------
// 描述：获取输入键盘和鼠标的信息
// 实现：该函数先从设备中检索即时数据，如果检索失败则会重新申请
// 设备控制权并再次检索即时数据
//------------------------------------------------------------------------------------------------
void DInput::GetInput()
{
	HRESULT hr = m_pKeyboardDevice->GetDeviceState(sizeof(m_KeyBuffer),
		(void**)&m_KeyBuffer);
	// 获取键盘输入信息
	if (hr)
	{
		m_pKeyboardDevice->Acquire();
		m_pKeyboardDevice->GetDeviceState(sizeof(m_KeyBuffer), 
			(void**)&m_KeyBuffer);
	}
	hr = m_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE),
		(void**)&m_MouseState);
	// 获取鼠标输入信息
	if (hr)
	{
		m_pMouseDevice->Acquire();
		m_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE),
			(void**)&m_MouseState);
	}
}

//-----------------------------------【IsKeyButtonDown( )函数】-------------------------------------------
// 描述：判断键盘上某个键是否被按下
//------------------------------------------------------------------------------------------------
bool DInput::IsKeyButtonDown(int keyButton)
{
	if (m_KeyBuffer[keyButton] & 0x80)
		return true;
	else
		return false;
}

//-----------------------------------【IsMouseButtonDown( )函数】-------------------------------------------
// 描述：判断鼠标上某个键是否被按下
//------------------------------------------------------------------------------------------------
bool DInput::IsMouseButtonDown(int mouseButton)
{
	if ((m_MouseState.rgbButtons[mouseButton] & 0x80) != 0)
		return true;
	else
		return false;
}

//-----------------------------------【MouseDX( )函数】-------------------------------------------
// 描述：返回鼠标指针的x轴坐标值
// 实现：使用了数据类型的显示转换，从long到float的转换
//---------------------------------------------------------------------------------------------------------
float DInput::MouseDX()
{
	return (float)m_MouseState.lX;
}

//-----------------------------------【MouseDY( )函数】-------------------------------------------
// 描述：返回鼠标指针的y轴坐标值
// 实现：使用了数据类型的显示转换，从long到float的转换
//---------------------------------------------------------------------------------------------------------
float DInput::MouseDY()
{
	return (float)m_MouseState.lY;
}

//-----------------------------------【MouseDZ( )函数】-------------------------------------------
// 描述：返回鼠标指针的z轴坐标值
// 实现：使用了数据类型的显示转换，从long到float的转换
//---------------------------------------------------------------------------------------------------------
float DInput::MouseDZ()
{
	return (float)m_MouseState.lZ;
}
