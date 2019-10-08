#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "../render/materials/celshader.h"
#include "../scene/model/celmodel.h"
#include "../render/camera.h"
#include "../render/light.h"
#include "../render/d3d.h"
#include "entity.h"
#include "sphere.h"
#include <iostream>
#include <Windows.h>
#include <mmsystem.h>

enum physicsType { STATIC, DYNAMIC, NONE };

class Player
{
public:
	Player();
	Player(XMFLOAT3 pos, XMFLOAT3 rotation, XMFLOAT3 scale);
	Player(XMFLOAT3 pos, XMFLOAT3 rotation, XMFLOAT3 scale,
		ID3D11DeviceContext& deviceContext, ID3D11Device& device, Light& light, CelShader& celShader, D3D& d3d, Camera& cam);
	Player(ID3D11DeviceContext& deviceContext, ID3D11Device& device, Light& light, CelShader& celShader, D3D& d3d, Camera& cam);
	Player(const Player&);
	~Player();

	void SetPosition(XMFLOAT3 newPos) { m_position = newPos; }
	void SetRotation(XMFLOAT3 newRot) { m_rotation = newRot; }
	void SetScale(XMFLOAT3 newScale) { m_scale = newScale; }
	void SetBoundingBox();
	void SetPointers(ID3D11DeviceContext&, ID3D11Device&, Light&, CelShader&, D3D&, Camera&);

	XMFLOAT3 GetPosition() { return m_position; }
	XMFLOAT3 GetRotation() { return m_rotation; }
	XMFLOAT3 GetScale() { return m_scale; }
	BoundingOrientedBox* GetBBox() { return m_orientedBBox; }
	bool IsColliding() { return collisionDeteced; }

	void Init(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scale);
	void Init();
	void Render();
	void Update(float);
	void Shutdown();
	bool CheckCollision(BoundingOrientedBox* box);
	void rotateBBox(float angle);
	void DrawBBoxCorners();

	void MoveLeft();
	void MoveRight();
	void MoveForward();
	void MoveBackward();
	void MoveToLastPos() { m_position = m_lastPosition; }

	int ProjectionType() { return m_projectionType; }
	void setProjectionType(int newType) { m_projectionType = newType; }
	void projectGeometry();
	void SetIsProjected(bool change) { m_isProjected = change; }
	bool IsProjected() { return m_isProjected; }
	void restoreGeometry();

public:
	bool collisionDeteced;
	bool isCorrectlyProjected = true;

private:
	CelModel* m_playerModel;

	Sphere* BBoxCorners[8];

	XMFLOAT3 m_position, m_backupPosition, m_lastPosition;
	XMFLOAT3 m_scale, m_backupScale;
	XMFLOAT3 m_rotation;
	XMFLOAT3 m_backupExtents;

	float xSpeed = 0.1f;
	float zSpeed = 0.1f;

	BoundingBox* m_boundBox;
	BoundingOrientedBox* m_orientedBBox;

	bool modelNeedsUpdate;
	float BBoxLastRotation;
	const int m_physxtype = DYNAMIC;

	unsigned long m_startTime;

	bool m_clear, m_colored, m_projected;
	int m_projectionType;
	bool m_isProjected;

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	Light* m_light;
	CelShader* m_celShader;
	D3D* m_d3d;
	Camera* m_cam;

	const bool ActiveGizmos = false;

};

#endif