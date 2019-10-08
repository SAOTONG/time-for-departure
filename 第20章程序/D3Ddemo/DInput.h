//=================================================================================
// 描述：DInput.h头文件，定义了类DInput
//=================================================================================
#ifndef DINPUT_H
#define DINPUT_H

#include <dinput.h>
#include "D3DUtil.h"
#include <vector>

using namespace std;

// DInput类实现了对DirectInput接口的封装，使得用户应用DirectInput接口功能变得更方便、简单
// 实例：
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

	// 为指向DirectInput接口、键盘和鼠标设备对象的指针设置NULL初始值
	// 并清空存储键盘值的数组和存储鼠标数据的结构体
	DInput();

	// 初始化DirectInput接口、键盘及鼠标输入设备对象。
	//
	// 参数: HWND hWnd               与当前设备相关联窗口句柄，不能为空。
	//       HINSTANCE hInstance     包含窗口过程的程序的实例句柄，不能为空。
	//       DWORD keyboardCoopFlags 指定键盘设备的协作级别，不能为空。
	//       DWORD mouseCoopFlags    指定鼠标设备的协作级别，不能为空。
	//
	// 返回值：返回一个HRESULT类型的值。
	// 函数执行成功时可能返回的值如下所示：
	// S_OK            0x00000000  成功
	// S_FALSE         0x00000001  函数成功执行完成，但返回时出现错误
	// 函数执行失败时可能返回的值如下所示：
	// E_INVALIDARG    0x80070057  参数有错误
	// E_OUTOFMEMORY   0x8007000E  内存申请错误
	// E_UNEXPECTED    0x8000FFFF  未知的异常
	// E_NOTIMPL       0x80004001  未实现功能
	// E_FAIL          0x80004005  没有详细说明的错误。一般需要取得 Rich Error 错误信息(注1)
	// E_POINTER       0x80004003  无效的指针
	// E_HANDLE        0x80070006  无效的句柄
	// E_ABORT         0x80004004  终止操作
	// E_ACCESSDENIED  0x80070005  访问被拒绝
	// E_NOINTERFACE   0x80004002  不支持接口
	HRESULT Init(HWND hWnd, HINSTANCE hInstance, DWORD keyboardCoopFlags,
		DWORD mouseCoopFlags);

	// 用于获取键盘或者鼠标的输入信息。
	void GetInput();

	// 判断键盘上某键是否按下。
	// 
	// 参数：输入键盘值对应的宏，例如键盘上的1对应DIK_1。
	// 
	// 返回值：true表示键按下。
	bool IsKeyButtonDown(int keyButton);

	// 判断鼠标上某键是否按下。
	// 
	// 参数：输入鼠标键值对应的数字，数字可以为0或1,分别代表左和右键。
	// 
	// 返回值：true表示键按下。
	bool IsMouseButtonDown(int mouseButton);

	// 返回鼠标的X轴坐标值
	float MouseDX();

	// 返回鼠标的Y轴坐标值
	float MouseDY();

	// 返回鼠标的Z轴坐标值
	float MouseDZ();

	// 释放对键盘和鼠标设备的控制权
	// 释放对COM资源的引用
	~DInput();

private:
	IDirectInput8 * m_pDirectInput;          // DirectInput8接口对象
	IDirectInputDevice8 * m_pKeyboardDevice; // 键盘设备接口对象
	IDirectInputDevice8 * m_pMouseDevice;    // 鼠标设备接口对象
	char m_KeyBuffer[256];                   // 存储键盘键值的数组
	DIMOUSESTATE m_MouseState;               // 存储鼠标键值的结构体
};

#endif // !DInput_H
