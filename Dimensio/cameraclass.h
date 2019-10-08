#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

#include <DirectXMath.h>
using namespace DirectX;

class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	//void SetPosition(float, float, float);
	//void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	//XMFLOAT3 GetRotation();

	XMFLOAT3 GetEye();
	XMFLOAT3 GetAt();

	//void Render();
	void UpdateCamera();
	void GetViewMatrix(XMMATRIX&);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	float m_lookAtX, m_lookAtY, m_lookAtZ;
	float m_upX, m_upY, m_upZ;
	XMMATRIX m_viewMatrix;

	XMVECTOR Eye = XMVectorSet(0, 5, -15, 0);
	XMVECTOR At = XMVectorSet(0, 0, 0, 0);
	XMVECTOR Up = XMVectorSet(0, 1, 0, 0);

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