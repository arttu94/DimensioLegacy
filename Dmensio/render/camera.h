#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <DirectXMath.h>
#include <Windows.h>
#include <iostream>
using namespace DirectX;
using namespace std;

class Camera //Considering renaming it KillMePlease.h , camera programming never again.
{
public:
	Camera();
	Camera(const Camera&);
	~Camera();

	void SetPosition(float, float, float);
	void SetAt(float x, float y, float z) { At.m128_f32[0] = x;  At.m128_f32[1] = y; At.m128_f32[2] = z; }
	void SetAt(XMFLOAT3 newAt){ At.m128_f32[0] = newAt.x;  At.m128_f32[1] = newAt.y; At.m128_f32[2] = newAt.z; }
	void SetPosition(XMFLOAT3);
	void SetHeight(float newheight) { Eye.m128_f32[1] = newheight; }
	void SetRotation(float yaw, float pitch);

	XMFLOAT3 GetPosition();
	//XMFLOAT3 GetRotation();

	XMFLOAT3 GetEye();
	XMFLOAT3 GetAt();

	//void Render();
	void Render();
	void UpdateCamera();
	void GetViewMatrix(XMMATRIX&);

	void CalculatePositionFromVector();

private:
	//repaced with sexier and easier to use XMVECTOR
	//float m_positionX, m_positionY, m_positionZ;
	//float m_rotationX, m_rotationY, m_rotationZ;
	//float m_lookAtX, m_lookAtY, m_lookAtZ;
	//float m_upX, m_upY, m_upZ;
	XMMATRIX m_viewMatrix;

	unsigned long m_startTime;

	XMVECTOR Eye = XMVectorSet(0, 5, -15, 0);
	XMVECTOR At = XMVectorSet(1, 0, 0, 0);
	XMVECTOR Up = XMVectorSet(0, 1, 0, 0);

	XMVECTOR default2DXZaxisPos = XMVectorSet(0, 20, 0, 0);
	XMVECTOR perspectivePos = XMVectorSet(0, 5, -15, 0);

	XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);


	XMMATRIX camRotationMatrix;
	XMMATRIX groundWorld;

public:
	float moveLeftRight = 0.0f;
	float moveBackForward = 0.0f;

	float camYaw = 0.0f;
	float camPitch = 0.0f;
};

#endif