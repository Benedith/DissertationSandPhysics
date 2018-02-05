#include "SandManager.h"
#include "Model.h"
#include "GraphicsClass.h"
#include "d3dclass.h"



bool SandManager::Initialize(char* model_file_path, int instance_count)
{
	SandCount = instance_count;

	m_swarm_models = new Model();
	if(!m_swarm_models->initialize("../DirectX/cube.txt", SandCount))
		return false;

	return true;
}



void SandManager::Render()
{
	Update();
	m_swarm_models->render();
}



void SandManager::Update()
{

	gravity();
}



int SandManager::GetVertexCount()
{
	return m_swarm_models->getVertexCount();
}



int SandManager::GetInstanceCount()
{

	return SandCount;
}

void SandManager::gravity() 
{
	for (int i = 0; i < SandCount; i++)
	{
		if (m_swarm_models->GetPosition(i).y <= -50 || m_swarm_models->isColliding(i) ==true)
		{
			
		}
		else
		{
			
			m_swarm_models->translatePosition(i, 0, down*0.30f);
		}
		
		
	}
	down -= 0.002f;
	
}



void SandManager::Shutdown()
{
	m_swarm_models->shutdown();
}

const DirectX::XMFLOAT3 SandManager::Direction(const DirectX::XMFLOAT3, const DirectX::XMFLOAT3)
{
	return DirectX::XMFLOAT3();
}

const DirectX::XMFLOAT3 SandManager::NormaliseFloat(const DirectX::XMFLOAT3 a)
{
	auto aVec = DirectX::XMLoadFloat3(&a);
	auto aVecNormalised = DirectX::XMVector3Normalize(aVec);

	DirectX::XMFLOAT3 aNormalised;
	DirectX::XMStoreFloat3(&aNormalised, aVecNormalised);

	return aNormalised;
}

const DirectX::XMFLOAT3 SandManager::Force(const DirectX::XMFLOAT3 dir, const float mass)
{
	auto force = 50.f;

	auto x = (dir.x * force) * mass;
	auto y = (dir.y * force) * mass;
	auto z = (dir.z * force) * mass;


	return DirectX::XMFLOAT3(x, y, z);
}
