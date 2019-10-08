#include "Cube.h"

Cube::Cube() :
	m_position(XMFLOAT3(0, 0, 0)), m_rotation(XMFLOAT3(0, 0, 0)), m_scale(XMFLOAT3(1, 1, 1)),
	m_cubeModel(nullptr), m_boundBox(nullptr), m_orientedBBox(nullptr),
	m_projected(false), m_colored(true), m_clear(false),
	m_projectionType(pt_none), m_isProjected(false), textureName("darkgrey")
{}

Cube::~Cube()
{}

Cube::Cube(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scl)
{
	m_cubeModel = nullptr;
	m_boundBox = nullptr;
	m_orientedBBox = nullptr;
	m_position = pos;
	m_rotation = rot;
	m_scale = scl;
	m_projected = false;
	m_colored = true;
	m_clear = false;
	m_projectionType = pt_none;
	m_isProjected = false;
	textureName = "darkgrey";
}

Cube::Cube(ID3D11DeviceContext& deviceContext, ID3D11Device& device, Light& light, CelShader& celShader, D3D& d3d, Camera& cam)
{
	m_cubeModel = nullptr;
	m_boundBox = nullptr;
	m_orientedBBox = nullptr;
	m_position = XMFLOAT3(0, 0, 0);
	m_rotation = XMFLOAT3(0, 0, 0);
	m_scale = XMFLOAT3(1, 1, 1);
	m_deviceContext = &deviceContext;
	m_device = &device;
	m_d3d = &d3d;
	m_light = &light;
	m_celShader = &celShader;
	m_cam = &cam;
	m_projected = false;
	m_colored = true;
	m_clear = false;
	m_projectionType = pt_none;
	m_isProjected = false;
	textureName = "darkgrey";
}

Cube::Cube(XMFLOAT3 pos, XMFLOAT3 rotation, XMFLOAT3 scale,
	ID3D11DeviceContext& deviceContext, ID3D11Device& device, Light& light, CelShader& celShader, D3D& d3d, Camera& cam)
{
	m_cubeModel = nullptr;
	m_boundBox = nullptr;
	m_orientedBBox = nullptr;
	m_position = pos;
	m_rotation = rotation;
	m_scale = scale;
	m_deviceContext = &deviceContext;
	m_device = &device;
	m_d3d = &d3d;
	m_light = &light;
	m_celShader = &celShader;
	m_cam = &cam;
	textureName = "darkgrey";
}

void Cube::Init()
{
	switch (m_projectionType)
	{
	case pt_wall:
		m_projectionType = pt_wall;
		textureName = "darkgrey";
		break;
	case pt_none:
		m_projectionType = pt_none;
		textureName = "white";
		break;
	case pt_xz:
		m_projectionType = pt_xz;
		textureName = "blue"; // russian blue
		//textureName = "ukrblue"; // ukraine blue
		break;
	case pt_zy:
		m_projectionType = pt_zy; 
		textureName = "red"; // russian red
		//textureName = "ukryellow"; //ukraine yellow
		break;
	default:
		break;
	}

	std::string StringFile = "../Dmensio/data/texture/" + textureName + ".tga";
	char *cstr = new char[StringFile.length() + 1];
	strcpy(cstr, StringFile.c_str());

	m_cubeModel = new CelModel;
	m_cubeModel->Initialize(m_device, m_deviceContext,
		cstr, "../Dmensio/data/models/cube.obj");

	delete[] cstr;

	m_backupPosition = m_position;
	m_backupScale = m_scale;
}

void Cube::Init(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scale)
{
	std::string StringFile = "../Dmensio/data/texture/" + textureName + ".tga";
	char *cstr = new char[StringFile.length() + 1];
	strcpy(cstr, StringFile.c_str());

	m_position = pos;
	m_rotation = rot;
	m_scale = scale;
	m_cubeModel = new CelModel;
	m_cubeModel->Initialize(m_device, m_deviceContext,
		cstr, "../Dmensio/data/models/cube.obj");

	delete[] cstr;

	m_backupPosition = m_position;
	m_backupScale = m_scale;
}

void Cube::SetBoundingBox()
{
	m_boundBox = new BoundingBox();
	BoundingBox::CreateFromPoints(*m_boundBox, FXMVECTOR(XMLoadFloat3(&m_cubeModel->maxVector)), FXMVECTOR(XMLoadFloat3(&m_cubeModel->lowVector)));
	m_boundBox->Extents = XMFLOAT3((m_boundBox->Extents.x * m_scale.x), (m_boundBox->Extents.y * m_scale.y), (m_boundBox->Extents.z * m_scale.z));
	m_orientedBBox = new BoundingOrientedBox();
	BoundingOrientedBox::CreateFromBoundingBox(*m_orientedBBox, *m_boundBox);
	delete m_boundBox;
	m_boundBox = 0;

	m_backupExtents = m_orientedBBox->Extents;

	if (ActiveGizmos)
	{
		for (int i = 0; i < 8; i++)
		{
			BBoxCorners[i] = new Sphere(*m_deviceContext, *m_device, *m_light, *m_celShader, *m_d3d, *m_cam);
			BBoxCorners[i]->Init();
		}
	}
}

void Cube::DrawBBoxCorners()
{
	if (ActiveGizmos)
	{
		for (int i = 0; i < 8; i++)
		{
			XMFLOAT3 m_BBoxCorner[8];
			m_orientedBBox->GetCorners(m_BBoxCorner);
			BBoxCorners[i]->SetPosition(XMFLOAT3(m_BBoxCorner[i].x, m_BBoxCorner[i].y, m_BBoxCorner[i].z));
			BBoxCorners[i]->SetRotation(XMFLOAT3(0, 0, 0));
			BBoxCorners[i]->SetScale(XMFLOAT3(0.05, 0.05, 0.05));
			BBoxCorners[i]->Render();
		}
	}
}

void Cube::SetPointers(ID3D11DeviceContext& deviceContext, ID3D11Device& device, Light& light, CelShader& celShader, D3D& d3d, Camera& cam)
{
	m_deviceContext = &deviceContext;
	m_device = &device;
	m_d3d = &d3d;
	m_light = &light;
	m_celShader = &celShader;
	m_cam = &cam;
}

void Cube::Render()
{
	m_cubeModel->Render(m_position, m_rotation, m_scale, m_celShader, m_d3d, m_light, m_deviceContext, m_cam);

	if (ActiveGizmos)
		DrawBBoxCorners();
}

void Cube::renderSettings(bool transparency, bool colored, bool projected)
{
	m_clear = transparency;
	m_colored = colored;
	m_projected = projected;
}

void Cube::projectGeometry()
{
	switch (m_projectionType)
	{
	case pt_none:
		break;
	case pt_xz:
		m_scale.y *= 0;
		m_position.y = 0.1f;
		m_orientedBBox->Extents.y = 0.1f;
		m_colored = false;
		m_cubeModel->colored(false);
		break;
	case pt_zy:
		m_scale.x *= 0;
		m_position.x = -7.85f;
		m_colored = false;
		m_cubeModel->colored(false);
		m_orientedBBox->Extents.x = 0.1f;
		break;
	default:
		break;
	}
}

void Cube::restoreGeometry()
{
	switch (m_projectionType)
	{
	case pt_none:
		break;
	case pt_xz:
		m_scale.y = m_backupScale.y;
		m_position.y = m_backupPosition.y;
		m_colored = true;
		m_cubeModel->colored(true);
		m_orientedBBox->Extents.y = m_backupExtents.y;
		break;
	case pt_zy:
		m_scale.x = m_backupScale.x;
		m_position.x = m_backupPosition.x;
		m_colored = true;
		m_cubeModel->colored(true);
		m_orientedBBox->Extents.x = m_backupExtents.x;
		break;
	default:
		break;
	}
}

void Cube::Update()
{
	m_orientedBBox->Center = m_position;
}

bool Cube::CheckCollision(BoundingOrientedBox* box)
{
	if (m_orientedBBox->Intersects(*box))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Cube::rotateBBox(float angle)
{
	if (BBoxLastRotation != angle)
	{
		BBoxLastRotation = angle;
		m_orientedBBox->Transform(*m_orientedBBox, 1, XMQuaternionRotationNormal(XMLoadFloat3(&XMFLOAT3(0, 1, 0)), angle), XMQuaternionNormalize(XMLoadFloat4(&XMFLOAT4(0, 0, 0, 0))));
	}
}

void Cube::Shutdown()
{
	//delete m_boundBox;
	m_boundBox = 0;

	//delete m_orientedBBox;
	m_orientedBBox = 0;

	m_cubeModel->Shutdown();
	delete m_cubeModel;
	m_cubeModel = 0;

	if (ActiveGizmos)
	{
		for (int i = 0; i < 8; i++)
		{
			BBoxCorners[i]->Shutdown();
			delete BBoxCorners[i];
			BBoxCorners[i] = 0;
		}
	}
}