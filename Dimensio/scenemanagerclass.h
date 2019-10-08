#ifndef _SCENEMANAGERCLASS_H_
#define _SCENEMANAGERCLASS_H_

#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "celmodelclass.h"
#include "lightclass.h"
#include "bitmapclass.h"
#include "textureshaderclass.h"
#include "celshaderclass.h"
#include "textclass.h"
#include "inputclass.h"
#include <iostream>

using namespace std;

const bool FULL_SCREEN = false; //don't turn on!
const bool VSYNC_ENABLED = false; //false == no fps cap, true == 60fps cap // don't turn on!
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class SceneManagerClass
{
public:
	SceneManagerClass();
	SceneManagerClass(const SceneManagerClass&);
	~SceneManagerClass();

	bool Initialize(int, int, HWND, InputClass&);
	void Shutdown();
	bool Frame(int, int, float);
	void Update();
	void DetectInput(double time);
	bool Render(float);

private:

	float playerX, playerY, playerZ;
	float playerYrot, playerXrot, playerZrot;
	float camX, camY, camZ;
	float camXrot, camYrot, camZrot;
	float xSpeed = 0.01f, zSpeed = 0.01f;

private:
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	CelModelClass* m_celModel;
	CelModelClass* m_celSphere;
	CelModelClass* m_BBoxCorners[8];
	CelModelClass* m_floor;
	CelModelClass* m_zWall;
	CelModelClass* m_xWall;
	ModelClass* m_Model;
	CelShaderClass* m_celShader;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	InputClass* m_Input;

	DIMOUSESTATE mouseLastState;

	TextureShaderClass* m_TextureShader;
	BitmapClass* m_Bitmap;

	TextClass* m_Text;

	bool CameraControl;
};

#endif