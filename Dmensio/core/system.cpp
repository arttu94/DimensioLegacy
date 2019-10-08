#include "system.h"
#include <cassert>
#include <cstdio>
#include <fcntl.h>
#include <io.h>
#include <tchar.h>
#include <iostream>

System::System()
{
	m_Input = 0;
	m_Graphics = 0;
	m_Sound = 0;
	m_Fps = 0;
	m_Cpu = 0;
	m_Timer = 0;
}

System::System(const System& other)
{
}

System::~System()
{
}

bool System::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	screenWidth = 0;
	screenHeight = 0;

	InitializeWindows(screenWidth, screenHeight);

	m_Input = new Input;
	if (!m_Input)
		return false;

	result = m_Input->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	m_Graphics = new SceneManager;
	if (!m_Graphics)
		return false;

	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd, *m_Input);
	if (!result)
		return false;

	m_Fps = new Fps;
	if (!m_Fps)
		return false;

	m_Fps->Initialize();

	m_Cpu = new Cpu;
	if (!m_Cpu)
		return false;

	m_Cpu->Initialize();

	m_Timer = new Timer;
	if (!m_Timer)
		return false;

	result = m_Timer->Initialize();
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the Timer object.", L"Error", MB_OK);
		return false;
	}


	m_Sound = new Sound;
	if (!m_Sound)
		return false;

	result = m_Sound->Initialize(m_hwnd);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize Direct Sound.", L"Error", MB_OK);
		return false;
	}

	if (ActiveConsole)
	{
		BOOL f = AllocConsole();
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);

		AllocConsole();
		int m_nCRTIn = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
		FILE* m_fpCRTIn = _fdopen(m_nCRTIn, "w");
		FILE fOldStdIn = *stdout;
		*stdout = *m_fpCRTIn;
		std::cout.clear();
	}
	
	//ShowWindow(GetConsoleWindow(), SW_HIDE); //hides the console

	return true;
}

void System::Shutdown()
{
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	if (m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = 0;
	}

	if (m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	if (m_Sound)
	{
		m_Sound->Shutdown();
		delete m_Sound;
		m_Sound = 0;
	}

	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}
	
	if (m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	ShutdownWindows();

	return;
}

void System::Run()
{
	MSG msg;
	bool done, result;

	ZeroMemory(&msg, sizeof(MSG));

	done = false;
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
		if (m_Input->IsEscapePressed() == true)
		{
			done = true;
		}
	}

	return;
}

bool System::Frame()
{
	bool result;
	int mouseX, mouseY;

	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();

	result = m_Input->Frame();
	if (!result)
		return false;

	m_Input->GetMouseLocation(mouseX, mouseY);

	result = m_Graphics->Frame(m_Fps->GetFps(), m_Cpu->GetCpuPercentage(), m_Timer->GetTime());
	if (!result)
	{
		return false;
	}

	// Finally render the graphics to the screen.
	//result = m_Graphics->Render();
	if (!result)
	{
		return false;
	}

	return true;
}

LRESULT CALLBACK System::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		case WM_KEYDOWN:
		{
			return 0;
		}
		case WM_KEYUP:
		{
			return 0;
		}
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

void System::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	//external pointer
	ApplicationHandle = this;

	//instance of the application
	m_hinstance = GetModuleHandle(NULL);

	m_applicationName = L"Dmensio";

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		//screenWidth = 1200;
		//screenHeight = (screenWidth * 9) / 16;

		screenWidth = 800;
		screenHeight = 600;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		/*WS_OVERLAPPEDWINDOW |*/ WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU | WS_CAPTION /*WS_POPUP*/ /*activate WS_POPUP for a borderless window*/,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

		// Bring the window up on the screen and set it as main focus.
		ShowWindow(m_hwnd, SW_SHOW);
		SetForegroundWindow(m_hwnd);
		SetFocus(m_hwnd);

		// Hide the mouse cursor.
		ShowCursor(false);

		return;
}

void System::ShutdownWindows()
{
	ShowCursor(true);

	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}