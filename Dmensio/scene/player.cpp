#include "player.h"

Player::~Player()
{

}

Player::Player()
{
	m_position = XMFLOAT3(0, 0, 0);
	m_rotation = XMFLOAT3(0, 0, 0);
	//m_scale = XMFLOAT3(0.3, 0.3, 0.25); //cat scale
	m_scale = XMFLOAT3(0.2, 0.2, 0.13);
	m_playerModel = nullptr;
	m_boundBox = nullptr;
	m_orientedBBox = nullptr;
	BBoxLastRotation = 1.5707;
	collisionDeteced = false;
	m_isProjected = false;
}

Player::Player(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scl)
{
	m_playerModel = nullptr;
	m_boundBox = nullptr;
	m_orientedBBox = nullptr;
	m_position = pos;
	m_rotation = rot;
	m_scale = scl;
	BBoxLastRotation = 1.5707;
	collisionDeteced = false;
	m_isProjected = false;
}

Player::Player(ID3D11DeviceContext& deviceContext, ID3D11Device& device, Light& light, CelShader& celShader, D3D& d3d, Camera& cam)
{
	m_playerModel = nullptr;
	m_boundBox = nullptr;
	m_orientedBBox = nullptr;
	m_position = XMFLOAT3(0, 0, 0);
	m_rotation = XMFLOAT3(0, 0, 0);
	//m_scale = XMFLOAT3(0.3, 0.3, 0.25); //cat scale
	m_scale = XMFLOAT3(0.2, 0.2, 0.13);
	m_deviceContext = &deviceContext;
	m_device = &device;
	m_d3d = &d3d;
	m_light = &light;
	m_celShader = &celShader;
	m_cam = &cam;
	BBoxLastRotation = 1.5707;
	collisionDeteced = false;
	m_isProjected = false;
}

Player::Player(XMFLOAT3 pos, XMFLOAT3 rotation, XMFLOAT3 scale,
	ID3D11DeviceContext& deviceContext, ID3D11Device& device, Light& light, CelShader& celShader, D3D& d3d, Camera& cam)
{
	m_playerModel = nullptr;
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
	BBoxLastRotation = 1.5707;
	collisionDeteced = false;
	m_isProjected = false;
}

void Player::Init()
{
	m_lastPosition = m_position;
	m_startTime = timeGetTime();
	m_playerModel = new CelModel;
	//bear model / russian themed
	m_playerModel->Initialize(m_device, m_deviceContext,  
		"../Dmensio/data/texture/skateBear2.tga", "../Dmensio/data/models/skateBear2.obj");

	//ukrainian cat model
	/*m_playerModel->Initialize(m_device, m_deviceContext,
		"../Dmensio/data/texture/catText.tga", "../Dmensio/data/models/cat.obj");*/

	m_backupPosition = m_position;
	m_backupScale = m_scale;
}

void Player::Init(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scale)
{
	m_position = pos;
	m_rotation = rot;
	m_scale = scale;
	m_startTime = timeGetTime();
	m_playerModel = new CelModel;
	//bear model / russian themed
	m_playerModel->Initialize(m_device, m_deviceContext,  
		"../Dmensio/data/texture/skateBear2.tga", "../Dmensio/data/models/skateBear2.obj");
	
	//ukrainian cat model
	/*m_playerModel->Initialize(m_device, m_deviceContext,
		"../Dmensio/data/texture/catText.tga", "../Dmensio/data/models/cat.obj");*/

	m_backupPosition = m_position;
	m_backupScale = m_scale;
}
void Player::DrawBBoxCorners()
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

void Player::SetBoundingBox()
{
	m_boundBox = new BoundingBox();
	BoundingBox::CreateFromPoints(*m_boundBox, FXMVECTOR(XMLoadFloat3(&m_playerModel->maxVector)), FXMVECTOR(XMLoadFloat3(&m_playerModel->lowVector)));
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
	
void Player::SetPointers(ID3D11DeviceContext& deviceContext, ID3D11Device& device, Light& light, CelShader& celShader, D3D& d3d, Camera& cam)
{
	m_deviceContext = &deviceContext;
	m_device = &device;
	m_d3d = &d3d;
	m_light = &light;
	m_celShader = &celShader;
	m_cam = &cam;
}

void Player::Render()
{
	m_playerModel->Render(m_position, m_rotation, m_scale, m_celShader, m_d3d, m_light, m_deviceContext, m_cam);

	if (ActiveGizmos)
		DrawBBoxCorners();
}

void Player::Update(float frametime)
{
	if (m_position.x > 8)
		MoveToLastPos();
	if (m_position.z < -12)
		MoveToLastPos();

	if (m_isProjected)
	{
		switch (m_projectionType)
		{
		case pt_xz:

			break;
		case pt_zy:
			if (m_position.x > -7.7f)
			{
				restoreGeometry();
			}
			break;
		default:
			break;
		}
	}

	m_orientedBBox->Center = m_position;
}

void Player::projectGeometry()
{
	switch (m_projectionType)
	{
	case pt_none:
		break;
	case pt_xz:
		m_scale.y *= 0;
		m_position.y = 0.2f;
		m_colored = false;
		m_playerModel->colored(false);
		m_orientedBBox->Extents.y = 0.1f;
		m_isProjected = true;
		break;
	case pt_zy:
		m_rotation.y = 3.1415f;
		rotateBBox(1.5707);
		m_scale.x *= 0;
		m_position.x = -7.7f;
		m_colored = false;
		m_playerModel->colored(false);
		m_orientedBBox->Extents.x = 0.1f;
		m_isProjected = true;
		break;
	default:
		break;
	}
}

void Player::restoreGeometry()
{
	switch (m_projectionType)
	{
	case pt_none:
		break;
	case pt_xz:
		m_scale.y = m_backupScale.y;
		m_position.y = m_backupPosition.y;
		m_colored = true;
		m_playerModel->colored(true);
		m_orientedBBox->Extents.y = m_backupExtents.y;
		m_isProjected = false;
		break;
	case pt_zy:
		m_scale.x = m_backupScale.x;
		m_position.x = m_backupPosition.x;
		m_colored = true;
		m_playerModel->colored(true);
		m_orientedBBox->Extents.x = m_backupExtents.x;
		m_isProjected = false;
		break;
	default:
		break;
	}
}

void Player::MoveForward()
{
	m_lastPosition = m_position;
	m_position.z += zSpeed;
	m_rotation.y = 3.1415f;
	rotateBBox(1.5707);
}

void Player::MoveBackward()
{
	m_lastPosition = m_position;
	m_position.z -= zSpeed;
	m_rotation.y = 0.0f;
	rotateBBox(1.5707);
}

void Player::MoveRight()
{
	if (m_projectionType != pt_zy || !m_isProjected)
	{
		m_lastPosition = m_position;
		m_position.x += xSpeed;
		m_rotation.y = -1.5707f;
		rotateBBox(-1.5707);
	}
}

void Player::MoveLeft()
{
	if (m_projectionType != pt_zy || !m_isProjected)
	{
		m_lastPosition = m_position;
		m_position.x -= xSpeed;
		m_rotation.y = 1.5707f;
		rotateBBox(-1.5707);
	}
}

bool Player::CheckCollision(BoundingOrientedBox* box)
{
	if (m_orientedBBox->Intersects(*box) && m_physxtype == DYNAMIC)
	{
		m_position = m_lastPosition;
		collisionDeteced = true;
	}
	else
	{
		collisionDeteced = false;
		return false;
	}
}

void Player::rotateBBox(float angle)
{
	if (BBoxLastRotation != angle)
	{
		BBoxLastRotation = angle;
		m_orientedBBox->Transform(*m_orientedBBox, 1, XMQuaternionRotationNormal(XMLoadFloat3(&XMFLOAT3(0, 1, 0)), angle), XMQuaternionNormalize(XMLoadFloat4(&XMFLOAT4(0, 0, 0, 0))));
	}
}

void Player::Shutdown()
{
	delete m_boundBox;
	m_boundBox = 0;

	delete m_orientedBBox;
	m_orientedBBox = 0;

	m_playerModel->Shutdown();
	delete m_playerModel;
	m_playerModel = 0;

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