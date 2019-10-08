#include "CelModel.h"

CelModel::CelModel()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_model = 0;
	xPos = 0;
	yPos = 0;
	zPos = 0;
	lastXpos = xPos;
	lastYpos = yPos;
	lastZpos = zPos;
	xRot = 0;
	yRot = 0;
	zRot = 0;
	xScale = 1;
	yScale = 1;
	zScale = 1;
	m_colored = true;
}

CelModel::CelModel(const CelModel& other)
{
}

CelModel::~CelModel()
{
}

void CelModel::SetPosition(float x, float y, float z)
{
	xPos = x;
	yPos = y;
	zPos = z;
}

void CelModel::SetRotation(float x, float y, float z)
{
	xRot = x;
	yRot = y;
	zRot = z;
}

void CelModel::SetScale(float x, float y, float z)
{
	xScale = x;
	yScale = y;
	zScale = z;
}

void CelModel::Render(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scale, CelShader* shader, 
	D3D* d3d, Light* light, ID3D11DeviceContext* deviceContext, Camera* cam)
{
	bool result;

	RenderBuffers(deviceContext);

	d3d->GetWorldMatrix(m_worldMatrix);
	cam->GetViewMatrix(m_viewMatrix);
	d3d->GetProjectionMatrix(m_projectionMatrix);

	d3d->GetWorldMatrix(m_celWorldMatrix);
	cam->GetViewMatrix(m_celViewMatrix);
	d3d->GetProjectionMatrix(m_celProjectionMatrix);

	m_celWorldMatrix = XMMatrixScaling(scale.x * 1.03f, scale.y * 1.03f, scale.z *1.03f);
	m_celWorldMatrix = m_celWorldMatrix * XMMatrixRotationX(rot.x);
	m_celWorldMatrix = m_celWorldMatrix * XMMatrixRotationY(rot.y);
	m_celWorldMatrix = m_celWorldMatrix * XMMatrixRotationZ(rot.z);
	m_celWorldMatrix = m_celWorldMatrix * XMMatrixTranslation(pos.x, pos.y, pos.z);

	m_worldMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);
	m_worldMatrix = m_worldMatrix * XMMatrixRotationX(rot.x);
	m_worldMatrix = m_worldMatrix * XMMatrixRotationY(rot.y);
	m_worldMatrix = m_worldMatrix * XMMatrixRotationZ(rot.z);
	m_worldMatrix = m_worldMatrix * XMMatrixTranslation(pos.x, pos.y, pos.z);

	d3d->ChangeCullingMode(2);

	shader->Render(deviceContext, this->GetIndexCount(), m_celWorldMatrix, m_celViewMatrix, m_celProjectionMatrix, m_Texture->GetTexture(),
	light->GetDirection(), light->GetAmbientColor(), light->GetDiffuseColor(), true, m_colored, m_transparent);

	d3d->ChangeCullingMode(1);

	shader->Render(deviceContext, this->GetIndexCount(), m_worldMatrix, m_viewMatrix, m_projectionMatrix, m_Texture->GetTexture(),
		light->GetDirection(), light->GetAmbientColor(), light->GetDiffuseColor(), false, m_colored, m_transparent);


	return;
}

void CelModel::Update()
{
	lastXpos = xPos;
	lastYpos = yPos;
	lastZpos = zPos;
}

int CelModel::GetIndexCount()
{
	return m_indexCount;
}

bool CelModel::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFilename, char* modelFilename)
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

void CelModel::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();

	ReleaseModel();

	return;
}

ID3D11ShaderResourceView* CelModel::GetTexture()
{
	return m_Texture->GetTexture();
}

bool CelModel::InitializeBuffers(ID3D11Device* device)
{
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

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

bool CelModel::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;

	m_Texture = new Texture;
	if (!m_Texture)
		return false;

	result = m_Texture->Initialize(device, deviceContext, filename);
	if (!result)
		return false;

	return true;
}

void CelModel::ReleaseTexture()
{
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

void CelModel::ShutdownBuffers()
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

void CelModel::RenderBuffers(ID3D11DeviceContext* deviceContext)
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

bool CelModel::LoadModel(char* filename)
{
	vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	vector<XMFLOAT3> temp_vertices;
	vector<XMFLOAT2> temp_uvs;
	vector<XMFLOAT3> temp_normals;
	float highestX, highestY, highestZ;
	float lowestX, lowestY, lowestZ;

	highestX = 0;
	highestY = 0;
	highestZ = 0;
	lowestX = 0;
	lowestY = 0;
	lowestZ = 0;

	FILE * file = fopen(filename, "r");
	if (file == NULL)
	{
		return false;
	}

	m_vertexCount = 0;
	m_indexCount = 0;

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

			if (vertex.x > highestX)
				highestX = vertex.x;
			if (vertex.y > highestY)
				highestY = vertex.y;
			if (vertex.z > highestZ)
				highestZ = vertex.z;

			if (vertex.x < lowestX)
				lowestX = vertex.x;
			if (vertex.y < lowestY)
				lowestY = vertex.y;
			if (vertex.z < lowestZ)
				lowestZ = vertex.z;

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
				printf("File can't be read by the simple parser : ( Try exporting with other options\n");
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

	maxVector = XMFLOAT3(highestX, highestY, highestZ);
	lowVector = XMFLOAT3(lowestX, lowestY, lowestZ);
}

void CelModel::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}