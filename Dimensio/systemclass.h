#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <cstdio>

using namespace std;

#include "inputclass.h"
#include "scenemanagerclass.h"
#include "soundclass.h"

#include "cpuclass.h"
#include "timerclass.h"
#include "fpsclass.h"

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

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

	InputClass* m_Input;
	SceneManagerClass* m_Graphics;

	SoundClass* m_Sound;

	FpsClass* m_Fps;
	CpuClass* m_Cpu;
	TimerClass* m_Timer;

	POINT pt;
	RECT windowSize;
};
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;

#endif