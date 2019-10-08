#include "BasicModelClass.h"

BasicModelClass::BasicModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}

BasicModelClass::BasicModelClass(const BasicModelClass& other)
{
}

BasicModelClass::~BasicModelClass()
{
}

bool BasicModelClass::Initialize(ID3D11Device* device, XMFLOAT3* vertex)
{
	bool result;

	for (int i = 0; i <= 7; i++)
	{
		//corners[i] = vertex[i];
	}

	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	return true;
}

void BasicModelClass::Shutdown()
{
	ShutdownBuffers();

	return;
}

void BasicModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);

	return;
}

int BasicModelClass::GetIndexCount()
{
	return m_indexCount;
}

bool BasicModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	m_vertexCount = 8;

	m_indexCount = 8;

	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array with data.
	//vertices[0].position = corners[0];
	//vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	//vertices[1].position = corners[1];
	//vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	//vertices[2].position = corners[2];
	//vertices[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	//vertices[3].position = corners[3];
	//vertices[3].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	//vertices[4].position = corners[4];
	//vertices[4].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	//vertices[5].position = corners[5];
	//vertices[5].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	//vertices[6].position = corners[6];
	//vertices[6].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	//vertices[7].position = corners[7];
	//vertices[7].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[0].position = XMFLOAT3(-1, -1, -1);
	vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(1, -1, -1);
	vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].position = XMFLOAT3(-1, -1, 1);
	vertices[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[3].position = XMFLOAT3(1, -1, 1);
	vertices[3].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[4].position = XMFLOAT3(-1, 1, -1);
	vertices[4].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[5].position = XMFLOAT3(1, 1, -1);
	vertices[5].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[6].position = XMFLOAT3(1, 1, 1);
	vertices[6].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[7].position = XMFLOAT3(1, 1, 1);
	vertices[7].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	indices[0] = 0;  
	indices[1] = 1;  
	indices[2] = 2;  
	indices[3] = 3;
	indices[4] = 4;
	indices[5] = 5;
	indices[6] = 6;
	indices[7] = 7;

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void BasicModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void BasicModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

	return;
}