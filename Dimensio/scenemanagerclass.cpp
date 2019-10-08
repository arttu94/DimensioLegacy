#include "scenemanagerclass.h"

SceneManagerClass::SceneManagerClass()
{
	m_Direct3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_celModel = 0;
	m_LightShader = 0;
	m_celSphere;
	m_celShader = 0;
	m_floor = 0;
	m_Light = 0;
	m_Bitmap = 0;
	m_TextureShader = 0;
	m_Text = 0;

}

SceneManagerClass::SceneManagerClass(const SceneManagerClass& other)
{
}

SceneManagerClass::~SceneManagerClass()
{
}

bool SceneManagerClass::Initialize(int screenWidth, int screenHeight, HWND hwnd, InputClass& input)
{
	XMMATRIX baseViewMatrix;
	bool result;

	m_Direct3D = new D3DClass;
	if (!m_Direct3D)
	{
		return false;
	}

	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	m_Input = &input;
	m_Input->m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseLastState);

	m_Camera->UpdateCamera();
	m_Camera->GetViewMatrix(baseViewMatrix);

	m_celShader = new CelShaderClass;
	if (!m_celShader)
		return false;

	result = m_celShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the cel shader object.", L"Error", MB_OK);
		return false;
	}

	m_Light = new LightClass;
	if (!m_Light)
		return false;

	playerX = 0;
	playerY = 3;
	playerZ = 0;
	playerYrot = 0;
	playerXrot = 0;
	playerZrot = 0;

	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.5f, -1.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);
	
	m_xWall = new CelModelClass;
	if (!m_xWall)
		return false;

	result = m_xWall->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(),
		"../Dimensio/data/red.tga", "../Dimensio/data/cube.obj");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_xWall->SetPointers(*m_Direct3D->GetDeviceContext(), *m_Direct3D->GetDevice(), *m_Light, *m_celShader, *m_Direct3D, *m_Camera);

	m_zWall = new CelModelClass;
	if (!m_zWall)
		return false;

	result = m_zWall->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(),
		"../Dimensio/data/red.tga", "../Dimensio/data/cube.obj");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_xWall->SetScale(8.0, 5.0, 0.1);
	m_xWall->SetBoundingBox();
	m_xWall->SetPhysicsType(STATIC);

	m_zWall->SetPointers(*m_Direct3D->GetDeviceContext(), *m_Direct3D->GetDevice(), *m_Light, *m_celShader, *m_Direct3D, *m_Camera);

	m_floor = new CelModelClass;
	if (!m_floor)
		return false;

	result = m_floor->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(),
		"../Dimensio/data/red.tga", "../Dimensio/data/cube.obj");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_zWall->SetScale(0.1, 5.0, 12.0);
	m_zWall->SetBoundingBox();
	m_zWall->SetPhysicsType(STATIC);

	m_floor->SetPointers(*m_Direct3D->GetDeviceContext(), *m_Direct3D->GetDevice(), *m_Light, *m_celShader, *m_Direct3D, *m_Camera);

	m_celSphere = new CelModelClass;
	if (!m_celSphere)
		return false;

	result = m_celSphere->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(),
		"../Dimensio/data/blue.tga", "../Dimensio/data/hpSphere.obj");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_celSphere->SetPointers(*m_Direct3D->GetDeviceContext(), *m_Direct3D->GetDevice(), *m_Light, *m_celShader, *m_Direct3D, *m_Camera);
	m_celSphere->SetPhysicsType(STATIC);
	m_celSphere->SetBoundingBox();

	m_celModel = new CelModelClass;
	if (!m_celModel)
		return false;

	result = m_celModel->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(),
		"../Dimensio/data/bear.tga", "../Dimensio/data/bear.obj");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_celModel->SetPointers(*m_Direct3D->GetDeviceContext(), *m_Direct3D->GetDevice(), *m_Light, *m_celShader, *m_Direct3D, *m_Camera);
	m_celModel->SetScale(0.3, 0.3, 0.3);
	m_celModel->SetPhysicsType(DYNAMIC);
	m_celModel->SetBoundingBox();

	for (int i = 0; i <= 7; i++)
	{
		m_BBoxCorners[i] = new CelModelClass();
		m_BBoxCorners[i]->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(),
			"../Dimensio/data/blue.tga", "../Dimensio/data/hpSphere.obj");
		m_BBoxCorners[i]->SetPointers(*m_Direct3D->GetDeviceContext(), *m_Direct3D->GetDevice(), *m_Light, *m_celShader, *m_Direct3D, *m_Camera);
	}

	m_Bitmap = new BitmapClass;
	if (!m_Bitmap)
		return false;

	result = m_Bitmap->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, "../Dimensio/data/red.tga", 100, 100);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	m_Text = new TextClass;
	if (!m_Text)
		return false;
	
	m_Camera->GetViewMatrix(baseViewMatrix);

	result = m_Text->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}


	return true;
}

void SceneManagerClass::Shutdown()
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

	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	if (m_celModel)
	{
		m_celModel->Shutdown();
		delete m_celModel;
		m_celModel = 0;
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

	return;
}

bool SceneManagerClass::Frame(int fps, int cpu, float frameTime)
{
	bool result;
	static float rotation = 0.0f;

	rotation += (float)XM_PI * 1.1f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	result = m_Text->SetFps(fps, m_Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Set the cpu usage.
	result = m_Text->SetCpu(cpu, m_Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	DetectInput(frameTime);
	Update();
	Render(rotation);
	if (!result)
		return false;

	return true;
}

void SceneManagerClass::Update()
{
	m_Camera->UpdateCamera();
	//m_celModel->SetRotation(0, playerYrot, 0);
	//m_celModel->SetPosition(playerX, playerY, playerZ);
	m_celModel->Update();
	m_celSphere->Update();
	m_zWall->Update();
	m_xWall->Update();
	m_celModel->CheckCollision(m_celSphere->m_orientedBBox);
	m_celModel->CheckCollision(m_xWall->m_orientedBBox);
	m_celModel->CheckCollision(m_zWall->m_orientedBBox);
}

void SceneManagerClass::DetectInput(double time)
{
	DIMOUSESTATE mouseCurrState;

	BYTE keyboardState[256];

	m_Input->m_mouse->Acquire();
	m_Input->m_keyboard->Acquire();

	m_Input->m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);

	m_Input->m_keyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

	float speed = 0.008f * time;

	if (keyboardState[DIK_D] & 0x80)
	{
		m_Camera->moveLeftRight += speed;
	}
	if (keyboardState[DIK_A] & 0x80)
	{
		m_Camera->moveLeftRight -= speed;
	}
	if (keyboardState[DIK_W] & 0x80)
	{
		m_Camera->moveBackForward += speed;
	}
	if (keyboardState[DIK_S] & 0x80)
	{
		m_Camera->moveBackForward -= speed;
	}
	if (keyboardState[DIK_R] & 0x80)
	{
		
	}
	if (m_Input->mouseKeyIsDown(m_Input->MOUSE_LEFT) || m_Input->mouseKeyIsDown(m_Input->MOUSE_RIGHT))
	{
		if ((mouseCurrState.lX != mouseLastState.lX) || (mouseCurrState.lY != mouseLastState.lY))
		{
			 m_Camera->camYaw += mouseLastState.lX * 0.03f;

			 m_Camera->camPitch += mouseCurrState.lY * 0.03f;

			mouseLastState = mouseCurrState;
		}
	}

	//camera movement
	if (m_Input->m_Gamepad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
	{
		m_Camera->moveBackForward += speed;
	}
	if (m_Input->m_Gamepad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
	{
		m_Camera->moveBackForward -= speed;
	}
	if (m_Input->m_Gamepad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
	{
		m_Camera->moveLeftRight -= speed;
	}
	if (m_Input->m_Gamepad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		m_Camera->moveLeftRight += speed;
	}

	if (m_Input->m_Gamepad->LeftStick_Y() > 0.5f)
	{
		playerZ += 0.01f;
		playerYrot = 3.1415f;
		m_celModel->rotateBBox(1.5707);
	}
	else if (m_Input->m_Gamepad->LeftStick_Y() < -0.5f)
	{
		playerZ -= 0.01f;
		playerYrot = 0;
		m_celModel->rotateBBox(1.5707);
	}
	else if (m_Input->m_Gamepad->LeftStick_X() > 0.5f)
	{
		playerX += 0.01f;
		playerYrot = -1.5707f;
		m_celModel->rotateBBox(-1.5707);
	}
	else if (m_Input->m_Gamepad->LeftStick_X() < -0.5f)
	{
		playerX -= 0.01f;
		playerYrot = 1.5707f;
		m_celModel->rotateBBox(-1.5707);		
	}
	if (m_Input->m_Gamepad->RightStick_X() > 0.5)
	{
		//playerXrot += 0.5f;
		m_Camera->camYaw += 1 * 0.003f;
	}
	if (m_Input->m_Gamepad->RightStick_X() < -0.5)
	{
		//playerXrot -= 0.5f;
		m_Camera->camYaw -= 1 * 0.003f;
	}
	if (m_Input->m_Gamepad->RightStick_Y() > 0.5)
	{
		//playerZrot += 0.5f;
		m_Camera->camPitch -= 1 * 0.003f;
	}
	if (m_Input->m_Gamepad->RightStick_Y() < -0.5)
	{
		//playerZrot -= 0.5f;
		m_Camera->camPitch += 1 * 0.003f;
	}

	return;
}

bool SceneManagerClass::Render(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, translateMatrix;
	bool result;

	m_Direct3D->BeginScene(0.3f, 0.3f, 0.3f, 1.0f);

	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	m_Direct3D->TurnZBufferOff();
	m_Direct3D->TurnOnAlphaBlending();

	m_Text->m_baseViewMatrix = viewMatrix;

	result = m_Text->Render(m_Direct3D->GetDeviceContext(), worldMatrix, orthoMatrix, viewMatrix);
	if (!result)
		return false;

	m_Direct3D->TurnOffAlphaBlending();

	m_Direct3D->TurnZBufferOn();

	//m_celModel->Render(playerX, 2.5, playerZ, playerXrot, playerYrot, playerZrot, 0.3, 0.3, 0.3);
	m_celModel->Render();
	m_celSphere->Render(0, 3, 3, 0, 0, 0, 1.0, 1.0, 1.0);
	m_floor->Render(0, 0, 0, 0, 0, 0, 8.0, 0.1, 12.0);
	m_zWall->Render(-8, 5, 0, 0, 0, 0, 0.1, 5.0, 12.0);
	m_xWall->Render(0, 5, 12, 0, 0, 0, 8.0, 5.0, 0.1);

	for (int i = 0; i <= 7; i++)
	{
		XMFLOAT3 m_BBoxCorner[8];
		m_celModel->m_orientedBBox->GetCorners(m_BBoxCorner);
		m_BBoxCorners[i]->Render(m_BBoxCorner[i].x, m_BBoxCorner[i].y, m_BBoxCorner[i].z, 0, 0, 0, 0.05, 0.05, 0.05);
	}

	for (int i = 0; i <= 7; i++)
	{
		XMFLOAT3 m_BBoxCorner[8];
		m_celSphere->m_orientedBBox->GetCorners(m_BBoxCorner);
		m_BBoxCorners[i]->Render(m_BBoxCorner[i].x, m_BBoxCorner[i].y, m_BBoxCorner[i].z, 0, 0, 0, 0.05, 0.05, 0.05);
	}

	for (int i = 0; i <= 7; i++)
	{
		XMFLOAT3 m_BBoxCorner[8];
		m_zWall->m_orientedBBox->GetCorners(m_BBoxCorner);
		m_BBoxCorners[i]->Render(m_BBoxCorner[i].x, m_BBoxCorner[i].y, m_BBoxCorner[i].z, 0, 0, 0, 0.05, 0.05, 0.05);
	}

	for (int i = 0; i <= 7; i++)
	{
		XMFLOAT3 m_BBoxCorner[8];
		m_xWall->m_orientedBBox->GetCorners(m_BBoxCorner);
		m_BBoxCorners[i]->Render(m_BBoxCorner[i].x, m_BBoxCorner[i].y, m_BBoxCorner[i].z, 0, 0, 0, 0.05, 0.05, 0.05);
	}

	m_Direct3D->EndScene();

	return true;
}