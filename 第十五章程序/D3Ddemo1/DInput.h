//=================================================================================
//描述：DInput.h头文件，对DirectInput接口的封装类
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
		DWORD mouseCoopFlags);//初始化DirectInput键盘及鼠标输入设备
	void GetInput();//用于获取输入信息的函数
	bool IsKeyButtonDown(int keyButton);//判断键盘上某键是否按下
	bool IsMouseButtonDown(int mouseButton);//判断鼠标上某键是否按下
	float MouseDX();//返回鼠标的X轴坐标值
	float MouseDY();//返回鼠标的Y轴坐标值
	float MouseDZ();//返回鼠标的Z轴坐标值
private:
	IDirectInput8 * m_pDirectInput;//DirectInput8接口
	IDirectInputDevice8 * m_pKeyboardDevice;//键盘设备接口对象
	IDirectInputDevice8 * m_pMouseDevice;//鼠标设备接口对象
	//vector<char> m_KeyBuffer;//存储键盘键值的vector
	char m_KeyBuffer[256];
	DIMOUSESTATE m_MouseState;//存储鼠标键值的结构体
};


#endif // !DInput_H
