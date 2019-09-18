//=================================================================================
// ������DInput.hͷ�ļ�����������DInput
//=================================================================================
#ifndef DINPUT_H
#define DINPUT_H

#include <dinput.h>
#include "D3DUtil.h"
#include <vector>

using namespace std;

// DInput��ʵ���˶�DirectInput�ӿڵķ�װ��ʹ���û�Ӧ��DirectInput�ӿڹ��ܱ�ø����㡢��
// ʵ����
//     DInput* g_pDirectInput = new DInput();
//     ..........
//     HRESULT hr = g_pDirectInput->Init(hwnd, hInstance, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE,
//                                       DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
//     ..........
//     g_pDirectInput->GetInput();
//     ..........
//     bool bKeyStatus = g_pDirectInput->IsKeyButtonDown(DIK_1)
//     ..........
//     bool bMouseButtonStatus = g_pDirectInput->IsMouseButtonDown(0);
//     ..........
//     float fPosX = g_pDirectInput->MouseDX();
//     ..........
//     float fPosY = g_pDirectInput->MouseDY();
//     ..........
//     float fPosZ = g_pDirectInput->MouseDZ();
class DInput
{
public:

	// Ϊָ��DirectInput�ӿڡ����̺�����豸�����ָ������NULL��ʼֵ
	// ����մ洢����ֵ������ʹ洢������ݵĽṹ��
	DInput();

	// ��ʼ��DirectInput�ӿڡ����̼���������豸����
	//
	// ����: HWND hWnd               �뵱ǰ�豸��������ھ��������Ϊ�ա�
	//       HINSTANCE hInstance     �������ڹ��̵ĳ����ʵ�����������Ϊ�ա�
	//       DWORD keyboardCoopFlags ָ�������豸��Э�����𣬲���Ϊ�ա�
	//       DWORD mouseCoopFlags    ָ������豸��Э�����𣬲���Ϊ�ա�
	//
	// ����ֵ������һ��HRESULT���͵�ֵ��
	// ����ִ�гɹ�ʱ���ܷ��ص�ֵ������ʾ��
	// S_OK            0x00000000  �ɹ�
	// S_FALSE         0x00000001  �����ɹ�ִ����ɣ�������ʱ���ִ���
	// ����ִ��ʧ��ʱ���ܷ��ص�ֵ������ʾ��
	// E_INVALIDARG    0x80070057  �����д���
	// E_OUTOFMEMORY   0x8007000E  �ڴ��������
	// E_UNEXPECTED    0x8000FFFF  δ֪���쳣
	// E_NOTIMPL       0x80004001  δʵ�ֹ���
	// E_FAIL          0x80004005  û����ϸ˵���Ĵ���һ����Ҫȡ�� Rich Error ������Ϣ(ע1)
	// E_POINTER       0x80004003  ��Ч��ָ��
	// E_HANDLE        0x80070006  ��Ч�ľ��
	// E_ABORT         0x80004004  ��ֹ����
	// E_ACCESSDENIED  0x80070005  ���ʱ��ܾ�
	// E_NOINTERFACE   0x80004002  ��֧�ֽӿ�
	HRESULT Init(HWND hWnd, HINSTANCE hInstance, DWORD keyboardCoopFlags,
		DWORD mouseCoopFlags);

	// ���ڻ�ȡ���̻�������������Ϣ��
	void GetInput();

	// �жϼ�����ĳ���Ƿ��¡�
	// 
	// �������������ֵ��Ӧ�ĺ꣬��������ϵ�1��ӦDIK_1��
	// 
	// ����ֵ��true��ʾ�����¡�
	bool IsKeyButtonDown(int keyButton);

	// �ж������ĳ���Ƿ��¡�
	// 
	// ��������������ֵ��Ӧ�����֣����ֿ���Ϊ0��1,�ֱ��������Ҽ���
	// 
	// ����ֵ��true��ʾ�����¡�
	bool IsMouseButtonDown(int mouseButton);

	// ��������X������ֵ
	float MouseDX();

	// ��������Y������ֵ
	float MouseDY();

	// ��������Z������ֵ
	float MouseDZ();

	// �ͷŶԼ��̺�����豸�Ŀ���Ȩ
	// �ͷŶ�COM��Դ������
	~DInput();

private:
	IDirectInput8 * m_pDirectInput;          // DirectInput8�ӿڶ���
	IDirectInputDevice8 * m_pKeyboardDevice; // �����豸�ӿڶ���
	IDirectInputDevice8 * m_pMouseDevice;    // ����豸�ӿڶ���
	char m_KeyBuffer[256];                   // �洢���̼�ֵ������
	DIMOUSESTATE m_MouseState;               // �洢����ֵ�Ľṹ��
};

#endif // !DInput_H
