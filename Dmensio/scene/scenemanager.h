#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

#include "../render/d3d.h"
#include "../render/camera.h"
#include "../scene/model/celmodel.h"
#include "../render/light.h"
#include "../scene/model/bitmap.h"
#include "../render/materials/celshader.h"
#include "../render/text/text.h"
#include "../core/input.h"
#include "../scene/player.h"
#include "../scene/cube.h"
#include "../scene/sphere.h"
#include "sceneloader.h"
#include "scene.h"
#include <iostream>

using namespace std;

const int OBST_AMM = 8;

const bool FULL_SCREEN = false; //don't turn on if resolution is different from 800x600
const bool VSYNC_ENABLED = true; //false == no fps cap, true == 60fps cap // don't turn on if resolution isn't 800 x 600
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class SceneManager
{
public:
	SceneManager();
	SceneManager(const SceneManager&);
	~SceneManager();

	bool Initialize(int, int, HWND, Input&);
	void Shutdown();
	bool Frame(int, int, float);
	void Update(float);
	void DetectInput(double time);
	bool Render(float);
	void projectObjects();
	void restoreObjects();
	void loadLevel(int level);

private:
	//float camX, camY, camZ;
	//float camXrot, camYrot, camZrot;
	void ProjectScene();
	void MovePlayerInScene();
	void MoveCameraInScene();
	void ResetCameraToIsometricView();

private:
	D3D* m_Direct3D;
	Camera* m_Camera;
	CelShader* m_celShader;
	Light* m_Light;
	Input* m_Input;

	Player* m_player;
	Cube* m_Walls[3];
	vector<Cube*> m_obstacles;
	Sphere* m_sphere;

	Scene* UKRlevel;
	Scene* RUSlevel;
	Scene* CurrentLevel;

	HWND m_hwnd;

	DIMOUSESTATE mouseLastState;

	Text* m_Text;

	//isometrix view positon and rotations for the camera.
	const XMFLOAT3 fixedIsometricCameraPosition = XMFLOAT3(16, 21, -16);
	const float fixedIsometricPitch = 0.6108f, fixedIsometrixYaw = -0.7853f;

	bool isProjected;
	unsigned long m_startTime;
	bool CameraControl;
};

#endif