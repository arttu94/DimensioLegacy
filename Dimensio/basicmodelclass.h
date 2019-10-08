#ifndef _BASICMODELCLASS_H_
#define _BASICMODELCLASS_H_

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

class BasicModelClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

public:
	BasicModelClass();
	BasicModelClass(const BasicModelClass&);
	~BasicModelClass();

	bool Initialize(ID3D11Device*, XMFLOAT3*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

public:
	XMFLOAT3 corners[8];
};

#endif