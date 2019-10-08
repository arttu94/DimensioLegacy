#include "Entity.h"

Entity::Entity()
{
	m_position = XMFLOAT3(0, 0, 0);
	m_rotation = XMFLOAT3(0, 0, 0);
	m_scale = XMFLOAT3(1, 1, 1);
	m_entityModel = 0;
	m_boundBox = 0;
	m_orientedBBox = 0;
	m_physxtype = 0;
}

Entity::~Entity()
{

}

Entity::Entity(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scl)
{
	m_entityModel = 0;
	m_boundBox = 0;
	m_orientedBBox = 0;
	m_position = pos;
	m_rotation = rot;
	m_scale = scl;
	m_physxtype = 0;
}

Entity::Entity(ID3D11DeviceContext& deviceContext, ID3D11Device& device, Light& light, CelShader& celShader, D3D& d3d, Camera& cam)
{
	m_entityModel = 0;
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
	m_physxtype = 0;
}

Entity::Entity(XMFLOAT3 pos, XMFLOAT3 rotation, XMFLOAT3 scale,
	ID3D11DeviceContext& deviceContext, ID3D11Device& device, Light& light, CelShader& celShader, D3D& d3d, Camera& cam)
{
	m_entityModel = 0;
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
	m_physxtype = 0;
}

void Entity::Init()
{
	m_entityModel = new CelModel;
	m_entityModel->Initialize(m_device, m_deviceContext,
		"../Dmensio/data/texture/tgafilehere.tga", "../Dmensio/data/models/obfilehere.obj");
}

void Entity::Init(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scale)
{
	m_position = pos;
	m_rotation = rot;
	m_scale = scale;
	m_entityModel = new CelModel;
	m_entityModel->Initialize(m_device, m_deviceContext,
		"../Dmensio/data/texture/tgafilehere.tga", "../Dmensio/data/models/obfilehere.obj");
}

void Entity::SetBoundingBox()
{
	m_boundBox = new BoundingBox();
	BoundingBox::CreateFromPoints(*m_boundBox, FXMVECTOR(XMLoadFloat3(&m_entityModel->maxVector)), FXMVECTOR(XMLoadFloat3(&m_entityModel->lowVector)));
	m_boundBox->Extents = XMFLOAT3((m_boundBox->Extents.x * m_scale.x), (m_boundBox->Extents.y * m_scale.y), (m_boundBox->Extents.z * m_scale.z));
	m_orientedBBox = new BoundingOrientedBox();
	BoundingOrientedBox::CreateFromBoundingBox(*m_orientedBBox, *m_boundBox);
	delete m_boundBox;
	m_boundBox = 0;
}

void Entity::SetPointers(ID3D11DeviceContext& deviceContext, ID3D11Device& device, Light& light, CelShader& celShader, D3D& d3d, Camera& cam)
{
	m_deviceContext = &deviceContext;
	m_device = &device;
	m_d3d = &d3d;
	m_light = &light;
	m_celShader = &celShader;
	m_cam = &cam;
}

void Entity::Render()
{
	m_entityModel->Render(m_position, m_rotation, m_scale, m_celShader, m_d3d, m_light, m_deviceContext, m_cam);
}

void Entity::Update()
{
	
}

bool Entity::CheckCollision(BoundingOrientedBox* box)
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

void Entity::rotateBBox(float angle)
{
	if (BBoxLastRotation != angle)
	{
		BBoxLastRotation = angle;
		m_orientedBBox->Transform(*m_orientedBBox, 1, XMQuaternionRotationNormal(XMLoadFloat3(&XMFLOAT3(0, 1, 0)), angle), XMQuaternionNormalize(XMLoadFloat4(&XMFLOAT4(0, 0, 0, 0))));
	}
}

void Entity::Shutdown()
{
	delete m_boundBox;
	m_boundBox = 0;

	delete m_orientedBBox;
	m_orientedBBox = 0;

	m_entityModel->Shutdown();
	delete m_entityModel;
	m_entityModel = 0;

}