#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "../render/materials/celshader.h"
#include "../scene/model/celmodel.h"
#include "../render/camera.h"
#include "../render/light.h"
#include "../render/d3d.h"
#include <iostream>

//enum physicsType { STATIC, DYNAMIC, NONE };

class Sphere
{
public:
	Sphere();
	Sphere(XMFLOAT3 pos, XMFLOAT3 rotation, XMFLOAT3 scale);
	Sphere(XMFLOAT3 pos, XMFLOAT3 rotation, XMFLOAT3 scale,
		ID3D11DeviceContext& deviceContext, ID3D11Device& device, Light& light, CelShader& celShader, D3D& d3d, Camera& cam);
	Sphere(ID3D11DeviceContext& deviceContext, ID3D11Device& device, Light& light, CelShader& celShader, D3D& d3d, Camera& cam);
	Sphere(const Sphere&);
	~Sphere();

	void SetPosition(XMFLOAT3 newPos) { m_position = newPos; }
	void SetRotation(XMFLOAT3 newRot) { m_rotation = newRot; }
	void SetScale(XMFLOAT3 newScale) { m_scale = newScale; }
	void SetBoundingBox();
	void SetPointers(ID3D11DeviceContext&, ID3D11Device&, Light&, CelShader&, D3D&, Camera&);

	XMFLOAT3 GetPosition() { return m_position; }
	XMFLOAT3 GetRotation() { return m_rotation; }
	XMFLOAT3 GetScale() { return m_scale; }

	void Init(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scale);
	void Init();
	void Render();
	void Update();
	void Shutdown();
	bool CheckCollision(BoundingOrientedBox* box);
	void rotateBBox(float angle);
	void DrawBBoxCorners();

public:

private:
	CelModel* m_sphereModel;
	Sphere* BBoxCorners[8];

	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
	XMFLOAT3 m_scale;

	BoundingBox* m_boundBox;
	BoundingOrientedBox* m_orientedBBox;

	bool modelNeedsUpdate;
	float BBoxLastRotation;
	const int m_physxtype = 0;

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	Light* m_light;
	CelShader* m_celShader;
	D3D* m_d3d;
	Camera* m_cam;
};

#endif