#ifndef _CUBE_H_
#define _CUBE_H_

#include "../render/materials/celshader.h"
#include "../scene/model/celmodel.h"
#include "../render/camera.h"
#include "../render/light.h"
#include "../render/d3d.h"
#include "entity.h"
#include "sphere.h"
#include <iostream>
#include <string>

class Cube
{
public:
	Cube();
	Cube(XMFLOAT3 pos, XMFLOAT3 rotation, XMFLOAT3 scale);
	Cube(XMFLOAT3 pos, XMFLOAT3 rotation, XMFLOAT3 scale,
		ID3D11DeviceContext& deviceContext, ID3D11Device& device, Light& light, CelShader& celShader, D3D& d3d, Camera& cam);
	Cube(ID3D11DeviceContext& deviceContext, ID3D11Device& device, Light& light, CelShader& celShader, D3D& d3d, Camera& cam);
	Cube(const Cube&);
	~Cube();

	void SetPosition(XMFLOAT3 newPos) { m_position = newPos; }
	void SetRotation(XMFLOAT3 newRot) { m_rotation = newRot; }
	void SetScale(XMFLOAT3 newScale) { m_scale = newScale; }
	void SetBoundingBox();
	void SetPointers(ID3D11DeviceContext&, ID3D11Device&, Light&, CelShader&, D3D&, Camera&);
	void SetTextureName(char* tn) { textureName = tn; }

	XMFLOAT3 GetPosition() { return m_position; }
	XMFLOAT3 GetRotation() { return m_rotation; }
	XMFLOAT3 GetScale() { return m_scale; }

	BoundingOrientedBox* GetBBox() { return m_orientedBBox; }

	void Init(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scale);
	void Init();
	void Render();
	void Update();
	void Shutdown();
	bool CheckCollision(BoundingOrientedBox* box);
	void rotateBBox(float angle);
	void DrawBBoxCorners();

	void renderSettings(bool transparency, bool colored, bool projected);
	void SetProjectionType(int type) { m_projectionType = type; }
	int GetProjectionType() { return m_projectionType; }
	void projectGeometry();
	void restoreGeometry();
	void Transparent(bool var) { var ? m_cubeModel->trasnparent(true) : m_cubeModel->trasnparent(false); }

public:

private:
	Sphere* BBoxCorners[8];
	CelModel* m_cubeModel;

	XMFLOAT3 m_position, m_backupPosition;
	XMFLOAT3 m_scale, m_backupScale;
	XMFLOAT3 m_rotation;
	XMFLOAT3 m_backupExtents;

	BoundingBox* m_boundBox;
	BoundingOrientedBox* m_orientedBBox;

	bool modelNeedsUpdate;
	float BBoxLastRotation;
	const int m_physxtype = 0;

	bool m_clear, m_colored, m_projected;
	int m_projectionType;
	bool m_isProjected;

	std::string textureName;

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	Light* m_light;
	CelShader* m_celShader;
	D3D* m_d3d;
	Camera* m_cam;

	const bool ActiveGizmos = false;
};

#endif