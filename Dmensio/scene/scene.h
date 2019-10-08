#ifndef _SCENE_H_
#define _SCENE_H_

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
#include <iostream>
#include <string>

class Scene
{
public:
	Scene();
	Scene(const Scene&);
	~Scene();

	void Init(char* levelname);
	void SetPointers(ID3D11DeviceContext&, ID3D11Device&, Light&, CelShader&, D3D&, Camera&);
	void Destroy();
	void Load(std::string filename);
	void Draw();
	void Update();
	void ProjectSceneOn();
	void ProjectSceneOff();

	void CheckCollision();

	bool isProjected;
	Player* m_player;
private:
	Cube* m_Walls[3];
	vector<Cube*> m_obstacles;

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	Light* m_light;
	CelShader* m_celShader;
	D3D* m_d3d;
	Camera* m_cam;

	const XMFLOAT3 fixedIsometricCameraPosition = XMFLOAT3(16, 21, -16);
	const float fixedIsometricPitch = 0.6108f, fixedIsometrixYaw = -0.7853f;
};

#endif