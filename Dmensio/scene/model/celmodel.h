#ifndef _CELMODEL_H_
#define _CELMODEL_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
using namespace DirectX;

#include <fstream>
#include <vector>
#include <iostream>
using namespace std;

#include "../../render/materials/texture.h"
#include "../../render/materials/celshader.h"
#include "../../render/light.h"
#include "../../render/camera.h"
#include "../../render/d3d.h"

class CelModel
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
	CelModel();
	CelModel(const CelModel&);
	~CelModel();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, char*);
	void Shutdown();
	void Render(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scale, CelShader* shader, 
		D3D* d3d, Light* light, ID3D11DeviceContext* deviceContext, Camera* cam);
	void Update();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);
	void SetScale(float, float, float);
	void colored(bool change) { m_colored = change; }
	void trasnparent(bool change) { m_transparent = change; }

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
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	VertexType* vertices;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	Light* m_light;
	CelShader* m_celShader;
	D3D* m_d3d;
	Camera* m_cam;

	float xPos, yPos, zPos;
	float lastXpos, lastYpos, lastZpos;
	float xRot, yRot, zRot;
	float xScale, yScale, zScale;
	float m_colored, m_transparent;

	XMMATRIX m_worldMatrix, m_viewMatrix, m_projectionMatrix, translateMatrix;
	XMMATRIX m_celWorldMatrix, m_celViewMatrix, m_celProjectionMatrix, celTranslateMatrix;

	Texture* m_Texture;
	ModelType* m_model;
public:

	XMFLOAT3 maxVector, lowVector; //this vectors will be used to create the bounding box
};

#endif