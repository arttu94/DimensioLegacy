#include "Sphere.h"

Sphere::Sphere():
	m_sphereModel(nullptr), m_boundBox(nullptr), m_orientedBBox(nullptr)
{
	m_position = XMFLOAT3(0, 0, 0);
	m_rotation = XMFLOAT3(0, 0, 0);
	m_scale = XMFLOAT3(1, 1, 1);
}

Sphere::~Sphere()
{

}

Sphere::Sphere(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scl)
{
	m_sphereModel = 0;
	m_boundBox = 0;
	m_orientedBBox = 0;
	m_position = pos;
	m_rotation = rot;
	m_scale = scl;
}

Sphere::Sphere(ID3D11DeviceContext& deviceContext, ID3D11Device& device, Light& light, CelShader& celShader, D3D& d3d, Camera& cam)
{
	m_sphereModel = 0;
	m_boundBox = 0;
	m_orientedBBox = 0;
	m_position = XMFLOAT3(0, 0, 0);
	m_rotation = XMFLOAT3(0, 0, 0);
	m_scale = XMFLOAT3(1, 1, 1);
	m_deviceContext = &deviceContext;
	m_device = &device;
	m_d3d = &d3d;
	m_light = &light;
	m_celShader = &celShader;
	m_cam = &cam;
}

Sphere::Sphere(XMFLOAT3 pos, XMFLOAT3 rotation, XMFLOAT3 scale,
	ID3D11DeviceContext& deviceContext, ID3D11Device& device, Light& light, CelShader& celShader, D3D& d3d, Camera& cam)
{
	m_sphereModel = 0;
	m_boundBox = 0;
	m_orientedBBox = 0;
	m_position = pos;
	m_rotation = rotation;
	m_scale = scale;
	m_deviceContext = &deviceContext;
	m_device = &device;
	m_d3d = &d3d;
	m_light = &light;
	m_celShader = &celShader;
	m_cam = &cam;
}

void Sphere::Init()
{
	m_sphereModel = new CelModel;
	m_sphereModel->Initialize(m_device, m_deviceContext,
		"../Dmensio/data/texture/blue.tga", "../Dmensio/data/models/cube.obj");
}

void Sphere::Init(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scale)
{
	m_position = pos;
	m_rotation = rot;
	m_scale = scale;
	m_sphereModel = new CelModel;
	m_sphereModel->Initialize(m_device, m_deviceContext,
		"../Dmensio/data/texture/blue.tga", "../Dmensio/data/models/cube.obj");
}

void Sphere::SetBoundingBox()
{
	m_boundBox = new BoundingBox();
	BoundingBox::CreateFromPoints(*m_boundBox, FXMVECTOR(XMLoadFloat3(&m_sphereModel->maxVector)), FXMVECTOR(XMLoadFloat3(&m_sphereModel->lowVector)));
	m_boundBox->Extents = XMFLOAT3((m_boundBox->Extents.x * m_scale.x), (m_boundBox->Extents.y * m_scale.y), (m_boundBox->Extents.z * m_scale.z));
	m_orientedBBox = new BoundingOrientedBox();
	BoundingOrientedBox::CreateFromBoundingBox(*m_orientedBBox, *m_boundBox);
	delete m_boundBox;
	m_boundBox = 0;

	for (int i = 0; i < 8; i++)
	{
		BBoxCorners[i] = new Sphere(*m_deviceContext, *m_device, *m_light, *m_celShader, *m_d3d, *m_cam);
		BBoxCorners[i]->Init();
	}
}

void Sphere::DrawBBoxCorners()
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

void Sphere::SetPointers(ID3D11DeviceContext& deviceContext, ID3D11Device& device, Light& light, CelShader& celShader, D3D& d3d, Camera& cam)
{
	m_deviceContext = &deviceContext;
	m_device = &device;
	m_d3d = &d3d;
	m_light = &light;
	m_celShader = &celShader;
	m_cam = &cam;
}

void Sphere::Render()
{
	m_sphereModel->Render(m_position, m_rotation, m_scale, m_celShader, m_d3d, m_light, m_deviceContext, m_cam);
}

void Sphere::Update()
{
	//m_sphereModel->SetPosition(m_position.x, m_position.y, m_position.z);
	//m_sphereModel->SetRotation(m_rotation.x, m_rotation.y, m_rotation.z);
	//m_sphereModel->SetScale(m_scale.x, m_scale.y, m_scale.z);
	m_orientedBBox->Center = m_position;
}

bool Sphere::CheckCollision(BoundingOrientedBox* box)
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

void Sphere::rotateBBox(float angle)
{
	if (BBoxLastRotation != angle)
	{
		BBoxLastRotation = angle;
		m_orientedBBox->Transform(*m_orientedBBox, 1, XMQuaternionRotationNormal(XMLoadFloat3(&XMFLOAT3(0, 1, 0)), angle), XMQuaternionNormalize(XMLoadFloat4(&XMFLOAT4(0, 0, 0, 0))));
	}
}

void Sphere::Shutdown()
{
	//delete m_boundBox;
	//m_boundBox = 0;

	//delete m_orientedBBox;
	//m_orientedBBox = 0;

	m_sphereModel->Shutdown();
	delete m_sphereModel;
	m_sphereModel = 0;

	//for (int i = 0; i < 8; i++)
	//{
	//	BBoxCorners[i]->Shutdown();
	//	delete BBoxCorners[i];
	//	BBoxCorners[i] = 0;
	//}
}