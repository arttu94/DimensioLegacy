#include "scenemanager.h"

SceneManager::SceneManager() :
	m_Direct3D(nullptr),   m_Camera(nullptr), m_celShader(nullptr),
	m_Light(nullptr),      m_Text(nullptr),   m_player(nullptr),
	CurrentLevel(nullptr), UKRlevel(nullptr), RUSlevel(nullptr),
	isProjected(false)
{

}

SceneManager::SceneManager(const SceneManager& other){}

SceneManager::~SceneManager(){}

bool SceneManager::Initialize(int screenWidth, int screenHeight, HWND hwnd, Input& input)
{
	XMMATRIX baseViewMatrix;
	bool result;

	m_hwnd = hwnd;

	m_Direct3D = new D3D;
	if (!m_Direct3D)
		return false;

	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	m_Camera = new Camera;
	if (!m_Camera)
		return false;

	m_Input = &input;
	m_Input->m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseLastState);

	m_Camera->SetPosition(16, 21, -16);
	m_Camera->SetRotation(-0.7853f, 0.6108f);
	m_Camera->UpdateCamera();
	m_Camera->GetViewMatrix(baseViewMatrix);

	m_celShader = new CelShader;
	if (!m_celShader)
		return false;

	result = m_celShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the cel shader object.", L"Error", MB_OK);
		return false;
	}

	m_Light = new Light;
	if (!m_Light)
		return false;

	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, -1.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);

	UKRlevel = new Scene();
	UKRlevel->SetPointers(*m_Direct3D->GetDeviceContext(), *m_Direct3D->GetDevice(), *m_Light, *m_celShader, *m_Direct3D, *m_Camera);
	UKRlevel->Init("../Dmensio/data/models/scene3.txt");

	RUSlevel = new Scene();
	RUSlevel->SetPointers(*m_Direct3D->GetDeviceContext(), *m_Direct3D->GetDevice(), *m_Light, *m_celShader, *m_Direct3D, *m_Camera);
	RUSlevel->Init("../Dmensio/data/models/scene2.txt");
	RUSlevel->m_player->SetPosition(XMFLOAT3(0, 0.2, 0));

	CurrentLevel = RUSlevel;
	
	m_Camera->GetViewMatrix(baseViewMatrix);

	m_startTime = timeGetTime();

	return true;
}

bool SceneManager::Frame(int fps, int cpu, float frameTime)
{
	// otherwise the input goes crazy, bear moves forward and it projects as fast as the program allows it
	if(GetForegroundWindow() == m_hwnd)
		DetectInput(frameTime);
	Update(frameTime);
	Render(0);

	return true;
}

void SceneManager::Update(float frameTime)
{
	m_Camera->CalculatePositionFromVector();
	m_Camera->UpdateCamera();
	CurrentLevel->Update();

	//m_player->Update(frameTime);
}

bool SceneManager::Render(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, translateMatrix;
	bool result;

	m_Direct3D->BeginScene(0.2f, 0.2f, 0.2f, 1.0f);

	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	m_Direct3D->GetOrthoMatrix(orthoMatrix);
	m_Camera->SetAt(0, 0, 0);
	m_Camera->UpdateCamera();

	m_Direct3D->TurnOnAlphaBlending();

	CurrentLevel->Draw();

	m_Direct3D->EndScene();

	return true;
}

void SceneManager::DetectInput(double time)
{

	MovePlayerInScene();
	MoveCameraInScene();
	ProjectScene();
	ResetCameraToIsometricView();

	/*
	//DIMOUSESTATE mouseCurrState;
	//BYTE keyboardState[256];

	//m_Input->m_mouse->Acquire();
	//m_Input->m_keyboard->Acquire();

	//m_Input->m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);

	//m_Input->m_keyboard->GetDeviceState(sizeof(keyboardState), &keyboardState);

	float speed = 0.008f * time;
	*/

	/*

	//put this code as ProjectScene function
	if (keyboardState[DIK_R] & 0x80 || m_Input->m_Gamepad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_X)
	{
		if (!CurrentLevel->isProjected)
			CurrentLevel->m_player->setProjectionType(pt_zy);
	}
	if (keyboardState[DIK_T] & 0x80 || m_Input->m_Gamepad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_Y)
	{
		if (!CurrentLevel->isProjected)
			CurrentLevel->m_player->setProjectionType(pt_xz);
	}
	if (m_Input->m_Gamepad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A || keyboardState[DIK_E] & 0x80)
	{
		if (timeGetTime() >= (m_startTime + 300))
		{
			if (!CurrentLevel->isProjected)
			{
				CurrentLevel->ProjectSceneOn();
			}
			else if (CurrentLevel->isProjected)
			{
				CurrentLevel->ProjectSceneOff();
			}
			m_startTime = timeGetTime();
		}
	}
	//put ths code as ProjectScene function

	//put this code as MovePlayerInScene function
	if (m_Input->m_Gamepad->LeftStick_Y() > 0.5f || keyboardState[DIK_UP] & 0x80)
	{
		if (m_Camera->camYaw >= -1.2962f)
			CurrentLevel->m_player->MoveForward();
		else
			CurrentLevel->m_player->MoveLeft();
	}
	else if (m_Input->m_Gamepad->LeftStick_Y() < -0.5f || keyboardState[DIK_DOWN] & 0x80)
	{
		if (m_Camera->camYaw >= -1.2962f)
			CurrentLevel->m_player->MoveBackward();
		else
			CurrentLevel->m_player->MoveRight();
	}
	else if (m_Input->m_Gamepad->LeftStick_X() > 0.5f || keyboardState[DIK_RIGHT] & 0x80)
	{
		if (m_Camera->camYaw >= -1.2962f)
			CurrentLevel->m_player->MoveRight();
		else
			CurrentLevel->m_player->MoveForward();
	}
	else if (m_Input->m_Gamepad->LeftStick_X() < -0.5f || keyboardState[DIK_LEFT] & 0x80)
	{
		if (m_Camera->camYaw >= -1.2962f)
			CurrentLevel->m_player->MoveLeft();
		else
			CurrentLevel->m_player->MoveBackward();
	}
	//put this code as MovePlayerInScene function


	if (m_Input->m_Gamepad->RightStick_X() > 0.5 || keyboardState[DIK_A] & 0x80)
	{
		if (m_Camera->camYaw > -1.5707f)//-0.8726f
			m_Camera->camYaw -= 0.01f;
	}
	else if (m_Input->m_Gamepad->RightStick_X() < -0.5 || keyboardState[DIK_D] & 0x80)
	{
		if (m_Camera->camYaw < -0.0f)
			m_Camera->camYaw += 0.01f;
	}

	if (m_Input->m_Gamepad->RightStick_Y() > 0.5 || keyboardState[DIK_W] & 0x80)
	{
		if (m_Camera->camPitch < 1.5607f)
			m_Camera->camPitch += 0.01f;
	}
	else if (m_Input->m_Gamepad->RightStick_Y() < -0.5 || keyboardState[DIK_S] & 0x80)
	{
		if (m_Camera->camPitch > 0.0f)//0.5235
			m_Camera->camPitch -= 0.01f;
	}


	//put this code as ResetCameraToIsometricView function
	if(m_Input->m_Gamepad->RStick_InDeadzone())
	{
		if (m_Camera->camPitch != fixedIsometricPitch)
		{
			//cout << "cam pitch : " << m_Camera->camPitch << endl;
			if (m_Camera->camPitch > 0.6108f)
			{
				m_Camera->camPitch -= 0.01f;
			}
			else if (m_Camera->camPitch < 0.6108)
			{
				m_Camera->camPitch += 0.01f;
			}
		}

		if (m_Camera->camYaw != fixedIsometrixYaw)
		{
			//cout << "cam yaw : " << m_Camera->camYaw << endl;
			if (m_Camera->camYaw > -0.7853f)
			{
				m_Camera->camYaw -= 0.01f;
			}
			else if (m_Camera->camYaw < -0.7853f)
			{
				m_Camera->camYaw += 0.01f;
			}
		}
	}
	//put this code as ResetCameraToIsometricView function

	*/
	return;
}

void SceneManager::MoveCameraInScene()
{
	if (m_Input->m_Gamepad->RightStick_X() > 0.5)
	{
		if (m_Camera->camYaw > -1.5707f)//-0.8726f
			m_Camera->camYaw -= 0.01f;
	}
	else if (m_Input->m_Gamepad->RightStick_X() < -0.5)
	{
		if (m_Camera->camYaw < -0.0f)
			m_Camera->camYaw += 0.01f;
	}

	if (m_Input->m_Gamepad->RightStick_Y() > 0.5)
	{
		if (m_Camera->camPitch < 1.5607f)
			m_Camera->camPitch += 0.01f;
	}
	else if (m_Input->m_Gamepad->RightStick_Y() < -0.5)
	{
		if (m_Camera->camPitch > 0.0f)//0.5235
			m_Camera->camPitch -= 0.01f;
	}
}

void SceneManager::MovePlayerInScene()
{
	if (m_Input->m_Gamepad->LeftStick_Y() > 0.5f)
	{
		if (m_Camera->camYaw >= -1.2962f)
			CurrentLevel->m_player->MoveForward();
		else
			CurrentLevel->m_player->MoveLeft();
	}
	else if (m_Input->m_Gamepad->LeftStick_Y() < -0.5f)
	{
		if (m_Camera->camYaw >= -1.2962f)
			CurrentLevel->m_player->MoveBackward();
		else
			CurrentLevel->m_player->MoveRight();
	}
	else if (m_Input->m_Gamepad->LeftStick_X() > 0.5f)
	{
		if (m_Camera->camYaw >= -1.2962f)
			CurrentLevel->m_player->MoveRight();
		else
			CurrentLevel->m_player->MoveForward();
	}
	else if (m_Input->m_Gamepad->LeftStick_X() < -0.5f)
	{
		if (m_Camera->camYaw >= -1.2962f)
			CurrentLevel->m_player->MoveLeft();
		else
			CurrentLevel->m_player->MoveBackward();
	}
}

void SceneManager::ResetCameraToIsometricView()
{
	if (m_Input->m_Gamepad->RStick_InDeadzone())
	{
		if (m_Camera->camPitch != fixedIsometricPitch)
		{
			//cout << "cam pitch : " << m_Camera->camPitch << endl;
			if (m_Camera->camPitch > 0.6108f)
			{
				m_Camera->camPitch -= 0.01f;
			}
			else if (m_Camera->camPitch < 0.6108)
			{
				m_Camera->camPitch += 0.01f;
			}
		}

		if (m_Camera->camYaw != fixedIsometrixYaw)
		{
			//cout << "cam yaw : " << m_Camera->camYaw << endl;
			if (m_Camera->camYaw > -0.7853f)
			{
				m_Camera->camYaw -= 0.01f;
			}
			else if (m_Camera->camYaw < -0.7853f)
			{
				m_Camera->camYaw += 0.01f;
			}
		}
	}
}

void SceneManager::ProjectScene()
{
	if (m_Input->m_Gamepad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_X)
	{
		if (!CurrentLevel->isProjected)
			CurrentLevel->m_player->setProjectionType(pt_zy);
	}
	if (m_Input->m_Gamepad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_Y)
	{
		if (!CurrentLevel->isProjected)
			CurrentLevel->m_player->setProjectionType(pt_xz);
	}
	if (m_Input->m_Gamepad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A)
	{
		if (timeGetTime() >= (m_startTime + 300))
		{
			if (!CurrentLevel->isProjected)
			{
				CurrentLevel->ProjectSceneOn();
			}
			else if (CurrentLevel->isProjected)
			{
				CurrentLevel->ProjectSceneOff();
			}
			m_startTime = timeGetTime();
		}
	}
}

void SceneManager::Shutdown()
{
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	if (m_celShader)
	{
		delete m_celShader;
		m_celShader = 0;
	}

	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
	
	if (m_player)
	{
		m_player->Shutdown();
		delete m_player;
		m_player = 0;
	}

	if (CurrentLevel)
	{
		CurrentLevel->Destroy();
		delete CurrentLevel;
	}

	return;
}