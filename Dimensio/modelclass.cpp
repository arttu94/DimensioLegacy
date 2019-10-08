#include "modelclass.h"

ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_model = 0;
	xPos = 0;
	yPos = 0;
	zPos = 0;
	xRot = 0;
	yRot = 0;
	zRot = 0;
}

ModelClass::ModelClass(const ModelClass& other)
{
}

ModelClass::~ModelClass()
{
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMFLOAT3 cameraPos, LightShaderClass* lightShader)
{
	bool result;

	RenderBuffers(m_deviceContext);

	m_d3d->GetWorldMatrix(m_worldMatrix);
	m_cam->GetViewMatrix(m_viewMatrix);
	m_d3d->GetProjectionMatrix(m_projectionMatrix);

	m_worldMatrix = XMMatrixRotationX(xRot);
	m_worldMatrix = m_worldMatrix * XMMatrixRotationY(yRot);
	m_worldMatrix = m_worldMatrix * XMMatrixRotationZ(zRot);
	m_worldMatrix = m_worldMatrix * XMMatrixTranslation(xPos, yPos, zPos);

	/*result = m_lightShader->Render(m_deviceContext, this->GetIndexCount(), m_worldMatrix, viewMatrix, projectionMatrix, m_Texture->GetTexture(), XMFLOAT3(0.5f, 0.5f, 1.0f),
		XMFLOAT4(0.15f, 0.15f, 0.15f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), cameraPos, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 32.0f);
	if (!result)
		return;*/

	m_lightShader->Render(m_deviceContext, this->GetIndexCount(), m_worldMatrix, m_viewMatrix, m_projectionMatrix, m_Texture->GetTexture(),
		m_light->GetDirection(), m_light->GetAmbientColor(), m_light->GetDiffuseColor(), m_cam->GetPosition(),
		m_light->GetSpecularColor(), m_light->GetSpecularPower(), 0);

	return;
}

void ModelClass::SetPointers(ID3D11DeviceContext& deviceContext, ID3D11Device& device, LightClass& light, CelShaderClass& celShader, D3DClass& d3d, CameraClass& cam)
{
	m_deviceContext = &deviceContext;
	m_device = &device;
	m_d3d = &d3d;
	m_light = &light;
	m_celShader = &celShader;
	m_cam = &cam;
}

void ModelClass::SetPosition(float x, float y, float z)
{
	xPos = x;
	yPos = y;
	zPos = z;
}

void ModelClass::SetRotation(float x, float y, float z)
{
	xRot = x;
	yRot = y;
	zRot = z;
}

void ModelClass::SetScale(float x, float y, float z)
{
	xScale = x;
	yScale = y;
	zScale = z;
}

void ModelClass::Render(float x, float y, float z, float xr, float yr, float zr, float sx, float sy, float sz, float edge)
{
	bool result;

	xPos = x;
	yPos = y;
	zPos = z;

	xRot = xr;
	yRot = yr;
	zRot = zr;

	xScale = sx;
	yScale = sy;
	zScale = sz;

	RenderBuffers(m_deviceContext);

	m_d3d->GetWorldMatrix(m_worldMatrix);
	m_cam->GetViewMatrix(m_viewMatrix);
	m_d3d->GetProjectionMatrix(m_projectionMatrix);

	m_worldMatrix = XMMatrixScaling(xScale, yScale, zScale);
	m_worldMatrix = m_worldMatrix * XMMatrixRotationX(xRot);
	m_worldMatrix = m_worldMatrix * XMMatrixRotationY(yRot);
	m_worldMatrix = m_worldMatrix * XMMatrixRotationZ(zRot);
	m_worldMatrix = m_worldMatrix * XMMatrixTranslation(xPos, yPos, zPos);

	/*m_lightShader->Render(m_deviceContext, this->GetIndexCount(), m_worldMatrix, m_viewMatrix, m_projectionMatrix, m_Texture->GetTexture(),
		m_light->GetDirection(), m_light->GetAmbientColor(), m_light->GetDiffuseColor(), m_cam->GetPosition(),
		m_light->GetSpecularColor(), edge, edge);*/

	m_celShader->Render(m_deviceContext, this->GetIndexCount(), m_worldMatrix, m_viewMatrix, m_projectionMatrix, m_Texture->GetTexture(),
		m_light->GetDirection(), m_light->GetAmbientColor(), m_light->GetDiffuseColor(), edge);

	return;
}

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

bool ModelClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFilename, char* modelFilename)
{
	bool result;

	result = LoadModel(modelFilename);
	if (!result)
		return false;

	result = InitializeBuffers(device);
	if (!result)
		return false;

	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result)
		return false;

	return true;
}

void ModelClass::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();

	ReleaseModel();

	return;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	m_vertexCount = m_vertexCount;
	m_indexCount = m_vertexCount;

	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	for (int i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		XMFLOAT3 modeldebug = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);

		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		XMFLOAT2 texdebug = XMFLOAT2(m_model[i].tu, m_model[i].tv);

		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);
		XMFLOAT3 normaldebug = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}

bool ModelClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;

	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
		return false;

	// Initialize the texture object.
	result = m_Texture->Initialize(device, deviceContext, filename);
	if (!result)
		return false;

	return true;
}

void ModelClass::ReleaseTexture()
{
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

void ModelClass::ShutdownBuffers()
{
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool ModelClass::LoadModel(char* filename)
{
	vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	vector<XMFLOAT3> temp_vertices;
	vector<XMFLOAT2> temp_uvs;
	vector<XMFLOAT3> temp_normals;

	FILE * file = fopen(filename, "r");
	if (file == NULL)
	{
		return false;
	}

	m_vertexCount = 0;
	m_indexCount  = 0;

	while (1)
	{
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);

		if (res == EOF)
			break;

		//parse the vertex
		if (strcmp(lineHeader, "f") == 0)
		{
			m_vertexCount += 3;
			m_indexCount += 3;
		}
	}

	file = fopen(filename, "r");

	m_model = new ModelType[m_vertexCount];
	if (!m_model)
		return false;

	while (1)
	{
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);

		if (res == EOF)
			break;

		//parse the vertex
		if (strcmp(lineHeader, "v") == 0) 
		{
			XMFLOAT3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		//parse the uv coords
		else if (strcmp(lineHeader, "vt") == 0)
		{
			XMFLOAT2 vertex;
			fscanf(file, "%f %f\n", &vertex.x, &vertex.y);
			temp_uvs.push_back(vertex);
		}
		//parse the normals
		else if (strcmp(lineHeader, "vn") == 0)
		{
			XMFLOAT3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}

		//parse triangle info
		else if (strcmp(lineHeader, "f") == 0)
		{
			std::string vertex1, vertex2, vertex3;

			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);

			if (matches != 9)
			{
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}

			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);

			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);

			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}
	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		m_model[i].x = temp_vertices[vertexIndex - 1].x;  
		m_model[i].y = temp_vertices[vertexIndex - 1].y; 
		m_model[i].z = temp_vertices[vertexIndex - 1].z /* - 1.0f*/;

		m_model[i].tu = temp_uvs[uvIndex - 1].x; 
		m_model[i].tv = /*1.0f -*/ temp_uvs[uvIndex - 1].y;

		m_model[i].nx = temp_normals[normalIndex - 1].x;
		m_model[i].ny = temp_normals[normalIndex - 1].y;
		m_model[i].nz = temp_normals[normalIndex - 1].z /* - 1.0f*/;

		ModelType debug = m_model[i];
		int y = 0;
	}
}

void ModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}