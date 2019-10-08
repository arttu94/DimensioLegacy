#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_

#include <Windows.h>
#include <Xinput.h>

#define XINPUT_GAMEPAD_DPAD_UP          0x00000001
#define XINPUT_GAMEPAD_DPAD_DOWN        0x00000002
#define XINPUT_GAMEPAD_DPAD_LEFT        0x00000004
#define XINPUT_GAMEPAD_DPAD_RIGHT       0x00000008
#define XINPUT_GAMEPAD_START            0x00000010
#define XINPUT_GAMEPAD_BACK             0x00000020
#define XINPUT_GAMEPAD_LEFT_THUMB       0x00000040
#define XINPUT_GAMEPAD_RIGHT_THUMB      0x00000080
#define XINPUT_GAMEPAD_LEFT_SHOULDER    0x0100
#define XINPUT_GAMEPAD_RIGHT_SHOULDER   0x0200
#define XINPUT_GAMEPAD_A                0x1000
#define XINPUT_GAMEPAD_B                0x2000
#define XINPUT_GAMEPAD_X                0x4000
#define XINPUT_GAMEPAD_Y                0x8000

class Gamepad
{
public:
	Gamepad();
	Gamepad(int);
	~Gamepad();

	void Update();

	bool LStick_InDeadzone();
	bool RStick_InDeadzone();

	float LeftStick_X();  // Return X axis of left stick
	float LeftStick_Y();  // Return Y axis of left stick
	float RightStick_X(); // Return X axis of right stick
	float RightStick_Y(); // Return Y axis of right stick

	// Trigger functions
	float LeftTrigger();  // Return value of left trigger
	float RightTrigger(); // Return value of right trigger

	void Rumble(float a_fLeftMotor = 0.0f, float a_fRightMotor = 0.0f);

	XINPUT_STATE GetState();
	int GetIndex();          // Return gamepad index
	bool Connected();        // Return true if gamepad is connected


private: 

	XINPUT_STATE m_State;
	int m_iGamepadIndex;
};

#endif //GAMEPAD_H

