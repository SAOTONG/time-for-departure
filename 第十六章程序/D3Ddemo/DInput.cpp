//=================================================================================
// ������DInput.cpp�ļ���DInput�ຯ����ʵ��
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

//-----------------------------------��Init( )������-------------------------------------------
// ��������ʼ��DirectInput�ӿڡ����̺�����豸��������
//------------------------------------------------------------------------------------------------
HRESULT DInput::Init(HWND hWnd, HINSTANCE hInstance, DWORD keyboardCoopFlags,
	DWORD mouseCoopFlags)
{
	HRESULT hr;
	// ��ʼ��һ��IDirectInput8�ӿڶ���
	HR(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&m_pDirectInput, NULL));
	// �Լ����豸���г�ʼ��
	HR(m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboardDevice, NULL));
	HR(m_pKeyboardDevice->SetCooperativeLevel(hWnd, keyboardCoopFlags));
	HR(m_pKeyboardDevice->SetDataFormat(&c_dfDIKeyboard));
	HR(m_pKeyboardDevice->Acquire());
	HR(m_pKeyboardDevice->Poll());
	// ������豸���г�ʼ��
	HR(m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouseDevice, NULL));
	HR(m_pMouseDevice->SetCooperativeLevel(hWnd, mouseCoopFlags));
	HR(m_pMouseDevice->SetDataFormat(&c_dfDIMouse));
	HR(m_pMouseDevice->Acquire());
	HR(m_pMouseDevice->Poll());
	return S_OK;
}

//-----------------------------------��GetInput( )������-------------------------------------------
// ��������ȡ������̺�������Ϣ
// ʵ�֣��ú����ȴ��豸�м�����ʱ���ݣ��������ʧ�������������
// �豸����Ȩ���ٴμ�����ʱ����
//------------------------------------------------------------------------------------------------
void DInput::GetInput()
{
	HRESULT hr = m_pKeyboardDevice->GetDeviceState(sizeof(m_KeyBuffer),
		(void**)&m_KeyBuffer);
	// ��ȡ����������Ϣ
	if (hr)
	{
		m_pKeyboardDevice->Acquire();
		m_pKeyboardDevice->GetDeviceState(sizeof(m_KeyBuffer), 
			(void**)&m_KeyBuffer);
	}
	hr = m_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE),
		(void**)&m_MouseState);
	// ��ȡ���������Ϣ
	if (hr)
	{
		m_pMouseDevice->Acquire();
		m_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE),
			(void**)&m_MouseState);
	}
}

//-----------------------------------��IsKeyButtonDown( )������-------------------------------------------
// �������жϼ�����ĳ�����Ƿ񱻰���
//------------------------------------------------------------------------------------------------
bool DInput::IsKeyButtonDown(int keyButton)
{
	if (m_KeyBuffer[keyButton] & 0x80)
		return true;
	else
		return false;
}

//-----------------------------------��IsMouseButtonDown( )������-------------------------------------------
// �������ж������ĳ�����Ƿ񱻰���
//------------------------------------------------------------------------------------------------
bool DInput::IsMouseButtonDown(int mouseButton)
{
	if ((m_MouseState.rgbButtons[mouseButton] & 0x80) != 0)
		return true;
	else
		return false;
}

//-----------------------------------��MouseDX( )������-------------------------------------------
// �������������ָ���x������ֵ
// ʵ�֣�ʹ�����������͵���ʾת������long��float��ת��
//---------------------------------------------------------------------------------------------------------
float DInput::MouseDX()
{
	return (float)m_MouseState.lX;
}

//-----------------------------------��MouseDY( )������-------------------------------------------
// �������������ָ���y������ֵ
// ʵ�֣�ʹ�����������͵���ʾת������long��float��ת��
//---------------------------------------------------------------------------------------------------------
float DInput::MouseDY()
{
	return (float)m_MouseState.lY;
}

//-----------------------------------��MouseDZ( )������-------------------------------------------
// �������������ָ���z������ֵ
// ʵ�֣�ʹ�����������͵���ʾת������long��float��ת��
//---------------------------------------------------------------------------------------------------------
float DInput::MouseDZ()
{
	return (float)m_MouseState.lZ;
}
