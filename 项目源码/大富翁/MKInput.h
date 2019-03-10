#pragma once
#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#pragma comment(lib, "dinput8.lib")

enum class MKMouse { 
	LEFT,	//����Ҽ�
	RIGHT	//������
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

	// ��ʼ�����̺����
	static void Initialize(HWND hWnd);
	// �ͷż��̺����
	static void Release();
	// ��ȡ��������ָ��
	static MKInput* GetMKInput();
	// ���¼����¼�
	void Update();

	// �ж���갴���Ƿ��ɿ�
	bool IsMouseUp(MKMouse mouseEvent) const { return m_MouseStatus[(int)mouseEvent] == MouseClick::MOUSE_CLICK_UP; }
	// �ж���갴���Ƿ���
	bool IsMouseDown(MKMouse mouseEvent) const { return m_MouseStatus[(int)mouseEvent] == MouseClick::MOUSE_CLICK_DOWN; }
	// ��굥��
	bool Mouse_SingleClick(MKMouse mouseEvent) const;
	// �����갴���Ƿ�ס
	bool Mouse_LongClick(MKMouse mouseEvent) const;
	// �����갴������
	POINT GetMouseClickPos(MKMouse mouseEvent) const;
	// ����������
	POINT GetMousePos() const { return m_MousePos; }
	// ��ȡ����������λ��XY
	POINT GetMouseLXY() const 
	{ 
		return POINT{ m_MouseState.lX, m_MouseState.lY }; 
	}
	// ��ȡ�����������Ϸ���Ϊ����
	int GetMouseLRoll() const { return m_MouseState.lZ / 120; }
	// �����̰���
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

	//��갴ť�Ĳ���״̬
	MouseClick m_MouseStatus[3];
};