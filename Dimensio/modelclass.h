#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

#include <fstream>
#include <vector>
using namespace std;

#include "textureclass.h"
#include "lightshaderclass.h"
#include "celshaderclass.h"
#include "lightclass.h"
#include "cameraclass.h"
#include "d3dclass.h"

class ModelClass
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
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMFLOAT3 cameraPos, LightShaderClass* lightShader);
	void Render(float x, float y, float z, float xr, float yr, float zr, float sx, float sy, float sz, float);

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
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	LightClass* m_light;
	LightShaderClass* m_lightShader;
	CelShaderClass* m_celShader;
	D3DClass* m_d3d;
	CameraClass* m_cam;

	float xPos, yPos, zPos;
	float xRot, yRot, zRot;
	float xScale, yScale, zScale;

	XMMATRIX m_worldMatrix, m_viewMatrix, m_projectionMatrix, translateMatrix;

	TextureClass* m_Texture;
	ModelType* m_model;
public:
};

#endif