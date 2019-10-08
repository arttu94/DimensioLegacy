#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>
#include "Gamepad.h"

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	void GetMouseLocation(int&, int&);
	int mouseKeyIsDown(unsigned char ucKey);

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
public:
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;
	IDirectInput8* m_directInput;

	static const int MOUSE_LEFT = 0;
	static const int MOUSE_RIGHT = 1;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;
	Gamepad* m_Gamepad;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
};

#endif