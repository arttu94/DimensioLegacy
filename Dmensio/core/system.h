#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <cstdio>

using namespace std;

#include "input.h"
#include "../scene/scenemanager.h"
#include "sound.h"

#include "../core/cpu.h"
#include "../core/timer/timer.h"
#include "../core/fps.h"

const bool ActiveConsole = false;

class System
{
public:
	System();
	System(const System&);
	~System();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	Input* m_Input;
	SceneManager* m_Graphics;

	Sound* m_Sound;

	Fps* m_Fps;
	Cpu* m_Cpu;
	Timer* m_Timer;

	POINT pt;
	RECT windowSize;

};
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static System* ApplicationHandle = 0;

#endif