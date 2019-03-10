#pragma once
#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#pragma comment(lib, "dinput8.lib")

enum class MKMouse { 
	LEFT,	//鼠标右键
	RIGHT	//鼠标左键
};

class MKInput {
private:
	enum class MouseClick
	{
		MOUSE_CLICK_DEFAULT = 0,
		MOUSE_CLICK_DOWN = 1,
		MOUSE_CLICK_KEEP,
		MOUSE_CLICK_UP
	};
public:

	// 初始化键盘和鼠标
	static void Initialize(HWND hWnd);
	// 释放键盘和鼠标
	static void Release();
	// 获取键鼠对象的指针
	static MKInput* GetMKInput();
	// 更新键鼠事件
	void Update();

	// 判断鼠标按键是否松开
	bool IsMouseUp(MKMouse mouseEvent) const { return m_MouseStatus[(int)mouseEvent] == MouseClick::MOUSE_CLICK_UP; }
	// 判断鼠标按键是否按下
	bool IsMouseDown(MKMouse mouseEvent) const { return m_MouseStatus[(int)mouseEvent] == MouseClick::MOUSE_CLICK_DOWN; }
	// 鼠标单击
	bool Mouse_SingleClick(MKMouse mouseEvent) const;
	// 检测鼠标按键是否按住
	bool Mouse_LongClick(MKMouse mouseEvent) const;
	// 输出鼠标按键坐标
	POINT GetMouseClickPos(MKMouse mouseEvent) const;
	// 输出鼠标坐标
	POINT GetMousePos() const { return m_MousePos; }
	// 获取鼠标相对坐标位移XY
	POINT GetMouseLXY() const 
	{ 
		return POINT{ m_MouseState.lX, m_MouseState.lY }; 
	}
	// 获取鼠标滚轮量（上方向为正）
	int GetMouseLRoll() const { return m_MouseState.lZ / 120; }
	// 检测键盘按键
	bool Key_Down(int key) const { return (bool)(m_keys[key] & 0x80); }

private:
	MKInput() :m_MouseStatus{ MouseClick::MOUSE_CLICK_DEFAULT,MouseClick::MOUSE_CLICK_DEFAULT ,MouseClick::MOUSE_CLICK_DEFAULT } {}
	~MKInput() {}

	void UpdateMouseButtonStatus();

	static MKInput*					m_MKInput;
	static LPDIRECTINPUT8			m_DInput;
	static HWND						m_hWnd;
	static LPDIRECTINPUTDEVICE8		m_pKeyboard;
	static LPDIRECTINPUTDEVICE8		m_pMouse;

	char							m_keys[256];

	RECT							m_WndRect;
	DIMOUSESTATE					m_MouseState;
	POINT							m_MousePos;
	POINT							m_OnClickLeftPos;
	POINT							m_OnClickRightPos;
	bool							m_SingleClickLeft;
	bool							m_SingleClickRight;
	bool							m_LongClickLeft;
	bool							m_LongClickRight;

	//鼠标按钮的操作状态
	MouseClick m_MouseStatus[3];
};