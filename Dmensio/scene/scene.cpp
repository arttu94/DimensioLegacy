#include "scene.h"

Scene::Scene()
{
	isProjected = false;
}

Scene::Scene(const Scene&)
{}

Scene::~Scene()
{}

void Scene::SetPointers(ID3D11DeviceContext& deviceContext, ID3D11Device& device, Light& light, CelShader& celShader, D3D& d3d, Camera& cam)
{
	m_deviceContext = &deviceContext;
	m_device = &device;
	m_d3d = &d3d;
	m_light = &light;
	m_celShader = &celShader;
	m_cam = &cam;
}

void Scene::Init(char* levelname)
{
	m_player = new Player(*m_d3d->GetDeviceContext(), *m_d3d->GetDevice(), *m_light, *m_celShader, *m_d3d, *m_cam);
	m_player->SetPosition(XMFLOAT3(m_player->GetPosition().x, 1.5, -10));
	m_player->setProjectionType(pt_xz);
	m_player->Init();
	m_player->SetBoundingBox();

	int delimeter = SceneLoader::GetNumberOfObjinScene(levelname);

	for (int i = 0; i < delimeter; i++)
	{
		m_obstacles.push_back(new Cube(*m_d3d->GetDeviceContext(), *m_d3d->GetDevice(), *m_light, *m_celShader, *m_d3d, *m_cam));
	}

	SceneLoader::LoadLevel(levelname, m_obstacles);

	for (int i = 0; i < m_obstacles.size(); i++)
	{
		m_obstacles[i]->Init();
		m_obstacles[i]->SetBoundingBox();
	}
}

void Scene::Update()
{
	m_player->Update(0);

	if (!m_player->IsProjected())
	{
		ProjectSceneOff();
	}

	for (int i = 0; i < m_obstacles.size(); i++)
	{
		float xd = m_player->GetPosition().x - m_cam->GetEye().x;
		float yd = m_player->GetPosition().y - m_cam->GetEye().y;
		float zd = m_player->GetPosition().z - m_cam->GetEye().z;
		float dist = sqrtf(xd*xd + yd*yd + zd*zd);

		//raycast to make geometry clear so you can see the player model.
		XMVECTOR v_dir = XMVectorSubtract(FXMVECTOR(XMLoadFloat3(&m_player->GetPosition())), FXMVECTOR(XMLoadFloat3(&m_cam->GetEye())));
		v_dir = XMVector3Normalize(v_dir);

		if (m_obstacles[i]->GetBBox()->Intersects(XMLoadFloat3(&m_cam->GetEye()), v_dir, dist))
		{
			if (m_obstacles[i]->GetProjectionType() != pt_wall)
				m_obstacles[i]->Transparent(true);
		}
		else
		{
			m_obstacles[i]->Transparent(false);
		}
		m_obstacles[i]->Update();
		if (m_obstacles[i]->CheckCollision(m_player->GetBBox()))
		{
			m_player->MoveToLastPos();
		}
	}
}

void Scene::Draw()
{
	m_player->Render();

	for (int i = 0; i < m_obstacles.size(); i++)
	{
		m_obstacles[i]->Render();
	}
}

void Scene::CheckCollision()
{

}

void Scene::ProjectSceneOn()
{
	isProjected = true;

	switch (m_player->ProjectionType())
	{
	case pt_xz:
		m_player->projectGeometry();
		for (int i = 0; i < m_obstacles.size(); i++)
		{
			if (m_obstacles[i]->GetProjectionType() == pt_xz)
			{
				m_obstacles[i]->projectGeometry();
			}
		}
		break;
	case pt_zy:
		m_player->projectGeometry();
		for (int i = 0; i < m_obstacles.size(); i++)
		{
			if (m_obstacles[i]->GetProjectionType() == pt_zy)
			{
				m_obstacles[i]->projectGeometry();
			}
		}
		break;
	default:
		break;
	}

	/*for (int i = 0; i < m_obstacles.size(); i++)
	{
		m_obstacles[i]->projectGeometry();
	}*/
}

void Scene::ProjectSceneOff()
{
	isProjected = false;

	m_player->restoreGeometry();
	for (int i = 0; i < m_obstacles.size(); i++)
	{
		m_obstacles[i]->restoreGeometry();
	}
}

void Scene::Destroy()
{

}