#ifndef _CELMODELCLASS_H_
#define _CELMODELCLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
using namespace DirectX;

#include <fstream>
#include <vector>
#include <iostream>
using namespace std;

#include "basicmodelclass.h"
#include "colorshaderclass.h"
#include "textureclass.h"
#include "celshaderclass.h"
#include "lightclass.h"
#include "cameraclass.h"
#include "d3dclass.h"

enum physicsType {STATIC, DYNAMIC, NONE};

class CelModelClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	CelModelClass();
	CelModelClass(const CelModelClass&);
	~CelModelClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, char*);
	void SetBoundingBox();
	void Shutdown();
	void Render(float x, float y, float z, float xr, float yr, float zr, float sx, float sy, float sz);
	void Render();
	void Update();

	void rotateBBox(float angle);

	void CheckCollision(BoundingOrientedBox*);
	int CheckPhysicsType() { return m_physxtype; };
	void SetPhysicsType(int type) { m_physxtype = type; };

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);
	void SetScale(float, float, float);
	void SetPointers(ID3D11DeviceContext&, ID3D11Device&, LightClass&, CelShaderClass&, D3DClass&, CameraClass&);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();

private:
	XMFLOAT3 maxVector, lowVector; //this vectors will be used to create the bounding box
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	VertexType* vertices;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	LightClass* m_light;
	CelShaderClass* m_celShader;
	D3DClass* m_d3d;
	CameraClass* m_cam;

	float xPos, yPos, zPos;
	float lastXpos, lastYpos, lastZpos;
	float xRot, yRot, zRot;
	float xScale, yScale, zScale;

	bool modelNeedsUpdate;
	float BBoxLastRotation;
	int m_physxtype;

	XMMATRIX m_worldMatrix, m_viewMatrix, m_projectionMatrix, translateMatrix;
	XMMATRIX m_celWorldMatrix, m_celViewMatrix, m_celProjectionMatrix, celTranslateMatrix;

	TextureClass* m_Texture;
	ModelType* m_model;
public:

	//XMFLOAT3 m_BBoxCorner[8];

	BoundingBox* m_boundBox;
	BoundingOrientedBox* m_orientedBBox;
};

#endif