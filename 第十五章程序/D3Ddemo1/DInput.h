//=================================================================================
//������DInput.hͷ�ļ�����DirectInput�ӿڵķ�װ��
//=================================================================================
#ifndef DINPUT_H
#define DINPUT_H
#include <dinput.h>
#include "D3DUtil.h"
#include <vector>

using namespace std;
class DInput
{
public:
	DInput();
	~DInput();
	HRESULT Init(HWND hWnd, HINSTANCE hInstance, DWORD keyboardCoopFlags,
		DWORD mouseCoopFlags);//��ʼ��DirectInput���̼���������豸
	void GetInput();//���ڻ�ȡ������Ϣ�ĺ���
	bool IsKeyButtonDown(int keyButton);//�жϼ�����ĳ���Ƿ���
	bool IsMouseButtonDown(int mouseButton);//�ж������ĳ���Ƿ���
	float MouseDX();//��������X������ֵ
	float MouseDY();//��������Y������ֵ
	float MouseDZ();//��������Z������ֵ
private:
	IDirectInput8 * m_pDirectInput;//DirectInput8�ӿ�
	IDirectInputDevice8 * m_pKeyboardDevice;//�����豸�ӿڶ���
	IDirectInputDevice8 * m_pMouseDevice;//����豸�ӿڶ���
	//vector<char> m_KeyBuffer;//�洢���̼�ֵ��vector
	char m_KeyBuffer[256];
	DIMOUSESTATE m_MouseState;//�洢����ֵ�Ľṹ��
};


#endif // !DInput_H
