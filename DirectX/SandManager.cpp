#include "SandManager.h"
#include "Model.h"
#include "GraphicsClass.h"
#include "d3dclass.h"
#include <d3d11.h>
#include <time.h>







bool SandManager::Initialize(char* model_file_path, int instance_count)
{

	averageSandPos = Vector3();
	myBar = TwNewBar("Sand Simulation");
	AntTweakBar();
	srand(time(NULL));
	Vector3 temp;
	SandCount = instance_count;
	sandParticles = new SandStruct[SandCount];
	
	m_sand_models = new Model();
	
	/*m_impact_model = new Model();*/
	graphicsclass = new GraphicsClass();
	
	if(!m_sand_models->initialize("../DirectX/cube.txt", SandCount))
		return false;
	for (int i = 0; i < SandCount; i++)
	{
		
			DirectX::XMFLOAT3 tempPos = m_sand_models->GetPosition(i);
			sandParticles->sandVelocities.push_back(Vector3(rand()%3+1, ((rand() % 4 + 1)*-1), 0));
			sandParticles->sandAccelerations.push_back(Vector3(0, 0, 0));
			sandParticles->sandPositions.push_back(Vector3(tempPos.x, tempPos.y, tempPos.z));
			sandParticles->gridNum.push_back(int());
			m_sand_models->setVeolcity(i, temp.convertVec3toXMFLOAT3(sandParticles->sandVelocities[i]));
			if (i <= m_sand_models->GetCellCount())
			{
				sandParticles->gridVec.push_back(std::vector<int>()); //create cellcount size vector
			}
	
		

		
	}
	
			sandParticles->sandPositions[40] = Vector3(100000, 100000, 10000); // spawn the impact object somewhere off screen
			
		return true;
}



void SandManager::Render()
{
	Update();
	
	
	m_sand_models->render();
	
}



void SandManager::Update() //Controls all movement and calculations of the particles
{

	Vector3 temp;
	gravity();
	
	for (int i = 0; i < SandCount; i++)
	{	
		/*SpatialPartitioning(i);*/
		sandParticles->sandVelocities[i] = sandParticles->sandVelocities[i] + sandParticles->sandAccelerations[i];
		sandParticles->sandAccelerations[i] = Vector3();
		sandParticles->sandPositions[i] = sandParticles->sandPositions[i] + sandParticles->sandVelocities[i];
		
		sandParticles->sandVelocities[i] = sandParticles->sandVelocities[i] * 0.9f;
		m_sand_models->setVeolcity(i, temp.convertVec3toXMFLOAT3(sandParticles->sandVelocities[i]));
		
		m_sand_models->translatePosition(i, sandParticles->sandPositions[i].x, sandParticles->sandPositions[i].y,
			sandParticles->sandPositions[i].z);
		
		averageSandPos = averageSandPos + sandParticles->sandPositions[i];
		averageSandVel = averageSandVel + sandParticles->sandVelocities[i];
		//border
		if (sandParticles->sandPositions[i].x > maxPos)
		{
			sandParticles->sandVelocities[i].x = pushforceNegative;
		}
		if (sandParticles->sandPositions[i].x < minPos)
		{
			sandParticles->sandVelocities[i].x = pushForce;
		}
		if (sandParticles->sandPositions[i].y > maxPos)
		{
			sandParticles->sandVelocities[i].y = pushforceNegative;
		}
		if (sandParticles->sandPositions[i].y < minPos)
		{
			sandParticles->sandVelocities[i].y = pushForce;
		}
		if (sandParticles->sandPositions[i].z > maxPos)
		{
			sandParticles->sandVelocities[i].z = pushforceNegative;
		}
		if (sandParticles->sandPositions[i].z < minPos)
		{
			sandParticles->sandVelocities[i].z = pushForce;
		}
		if (resetCollision == true)
		{
			sandParticles->sandPositions[40] = Vector3((rand() % 100 + 1) + 100, 150, (rand() % 100 + 1));
			resetCollision = false;
		}
	}
	FindAverages();
	numCounter = 0;
}



int SandManager::GetVertexCount()
{
	return m_sand_models->getVertexCount();
}

int SandManager::GetInstanceCount()
{

	return SandCount;
}

void SandManager::FindAverages()
{
	averageSandPos.x = ((averageSandPos.x - sandParticles->sandPositions[40].x) / (SandCount - 1));

	averageSandPos.y = ((averageSandPos.y - sandParticles->sandPositions[40].y) / (SandCount - 1));

	averageSandPos.z = ((averageSandPos.z - sandParticles->sandPositions[40].z) / (SandCount - 1));

	averageSandVel = ((averageSandVel - sandParticles->sandVelocities[40]) / (SandCount - 1));
}

void SandManager::gravity() //Physics calculations before dual layer is activated
{
	if (nCount == 0)
	{

		for (int i = 0; i < SandCount; i++)
		{
			if (averageSandVel.y >= -0.001f && averageSandVel.y <= 0.001f)
			{
				setImpact();
				SandLayer(i);
				nCount++;
			}
			if (applyWind == true)
			{
				sandParticles->sandAccelerations[i] = sandParticles->sandAccelerations[i] + ((Vector3(100, 180, 0)/100000) *windForce);
			}
			
			/*bool collided = sphereCollisions(i, sandParticles->gridNum[i]);*/ //spatial partitioning attempt
			bool collided = m_sand_models->sphereColliding(i);
			if (sandParticles->sandPositions[i].y <= 50)
			{
				sandParticles->sandVelocities[i].y =0;
				sandParticles->sandAccelerations[i].y = 0;
			}
			else if (collided == true)
			{
				Vector3 newIVel = newIVel.convertXMFLOAT3toVec3(m_sand_models->getNewVelI());
				Vector3 newIndexVel = newIndexVel.convertXMFLOAT3toVec3(m_sand_models->getNewVelIndex());
				int w = m_sand_models->getCollidedSphere();
				sandParticles->sandVelocities[i] = newIndexVel;

				sandParticles->sandVelocities[w] = newIndexVel;
				/*sandAccelerations[i].y = sandAccelerations[i].y - GravitationalForce;*/
				
				if (sandParticles->sandVelocities[i].y <= 0.0001 || sandParticles->sandVelocities[w].y <= 0.0001 /*&& averageSandPos.y <= 80.0f*/)
				{
				sandParticles->sandVelocities[i].y = 0;
					sandParticles->sandVelocities[w].y = sandParticles->sandVelocities[w].y-0.5f;
				}
				
			
			}
			else
			{
				sandParticles->sandAccelerations[i].y = sandParticles->sandAccelerations[i].y - GravitationalForce;
			}

		}
		
		
		
	}
	else
	{
		LayerGravity();
	}
	averageSandPos = Vector3();
	averageSandVel = Vector3();
}

void SandManager::SandLayer(int i)//Calculating the sand Layers
{
	float topLayerY = averageSandPos.y + (averageSandPos.y*0.1);
	float topLayerXMin = averageSandPos.x - (averageSandPos.x*0.1);
	float topLayerXMax = averageSandPos.x + (averageSandPos.x*0.1);
	float topLayerZMin = averageSandPos.z - (averageSandPos.z*0.1);
	float topLayerZMax = averageSandPos.z + (averageSandPos.z*0.1);
	

	if (sandParticles->sandPositions[i].y >= topLayerY || sandParticles->sandPositions[i].x <= topLayerXMin ||
		sandParticles->sandPositions[i].x >= topLayerXMax || sandParticles->sandPositions[i].z <= topLayerZMin ||
		sandParticles->sandPositions[i].z >= topLayerZMax)
	{
		sandNums.push_back (i);
		 numCounter++;
	}
	
	

} 

void SandManager::LayerGravity()//Dual layer physics calculations
{
	
	for (const int &i : sandNums)
	{
		
		if (applyWind == true)
		{
			sandParticles->sandAccelerations[i] = sandParticles->sandAccelerations[i] + ((Vector3(100, 180, 0) / 100000) * windForce);
		}
		/*bool collided = m_sand_models->sphereColliding(i,sandParticles->gridNum);*/ 
		
		
	/*	bool collided = sphereCollisions(i, sandParticles->gridNum[i]);*/
		bool collided = m_sand_models->sphereColliding(i);
		if (sandParticles->sandPositions[i].y <= 50)
		{
			sandParticles->sandVelocities[i].y = 0;
			sandParticles->sandAccelerations[i].y = 0;
		}
		else if (collided == true)
		{
			Vector3 newIVel = newIVel.convertXMFLOAT3toVec3(m_sand_models->getNewVelI());
			Vector3 newIndexVel = newIndexVel.convertXMFLOAT3toVec3(m_sand_models->getNewVelIndex());
			int w = m_sand_models->getCollidedSphere();
			sandParticles->sandVelocities[i] = newIndexVel;

			sandParticles->sandVelocities[w] = newIndexVel;
			if (sandParticles->sandVelocities[i].y <= 0.1 || sandParticles->sandVelocities[w].y <= 0.1)
			{
				sandParticles->sandVelocities[i].y = 0;
				sandParticles->sandVelocities[w].y = 0;
			}
			
		}
		else
		{
			sandParticles->sandAccelerations[i].y = sandParticles->sandAccelerations[i].y - GravitationalForce;
		}
		Vector3 ImpactPos = averageSandPos - sandParticles->sandPositions[40];
		

		sandParticles->sandAccelerations[40] = sandParticles->sandAccelerations[i] + (ImpactPos/100);
	}


} 



void SandManager::Shutdown()
{
	m_sand_models->shutdown();
}

void SandManager::AntTweakBar()
{
	
	
	TwAddVarRW(myBar, "GravitationalForce", TW_TYPE_FLOAT, &GravitationalForce, "min=0.01 max= 1.5 step=0.01");
	/*TwAddVarRW(myBar, "WindForce", TW_TYPE_FLOAT, &windForce, "min=-40.0 max= 40.0 step=0.1");*/
	TwAddVarRW(myBar, "resetCollision", TW_TYPE_BOOLCPP, &resetCollision, "true = enabled, false= press to reset");
	
}
void SandManager::setImpact()
{
	
	sandParticles->sandPositions[40] = Vector3((rand()%100 + 1) +100, 150, (rand() % 100 + 1));
	m_sand_models->setMass(40, 10);
	
	

}

void SandManager::SpatialPartitioning(int index)
{
	int w = 0;
	int originalCellY = cellY;
	int cellCount = m_sand_models->GetCellCount();
	Vector3 a = sandParticles->sandPositions[index];
	for (int i = 0; i < cellCount; i++)
	{
		if ((i % 15) == 0 && i != 0)
		{
			w = 0;
			cellY = cellY + cellSize;
		}
		if (a.x  < cellX + (w*cellSize) && a.y < cellY && a.x >((cellX + (w*cellSize)) - cellSize) && a.y > cellY - cellSize)
		{
			sandParticles->gridVec[i].push_back(index); //fill vector with index
			sandParticles->gridNum[index] = i;
		}
		//if ((a.x + m_sand_models->getRadius(index)) < cellX + (w*cellSize) && a.y < cellY &&
		//	(a.x + m_sand_models->getRadius(index)) >((cellX + (w*cellSize)) - cellSize) && a.y > cellY - cellSize)
		//{
		//	
		//	sandParticles->gridVec[i].push_back (index); //fill vector with index
		//	sandParticles->gridNum[index] = i;
		//	gridChosen = true;
		//}
		//if ((a.x - m_sand_models->getRadius(index)) < cellX + (w*cellSize) && a.y < cellY &&
		//	(a.x - m_sand_models->getRadius(index)) >((cellX + (w*cellSize)) - cellSize) && a.y > cellY - cellSize && gridChosen == false)
		//{
		//	sandParticles->gridVec[i].push_back(index); //fill vector with index
		//	sandParticles->gridNum.push_back(i);
		//	gridChosen = true;
		//}
		//if (a.x < cellX + (w*cellSize) && (a.y + m_sand_models->getRadius(index)) < cellY && a.x >((cellX + (w*cellSize)) - cellSize) &&
		//	(a.y + m_sand_models->getRadius(index)) > cellY - cellSize && gridChosen == false)
		//{
		//	sandParticles->gridVec[i].push_back(index); //fill vector with index
		//	sandParticles->gridNum[index] = i;
		//	gridChosen = true;
		//}
		//if (a.x < cellX + (w*cellSize) && (a.y - m_sand_models->getRadius(index)) < cellY && a.x >((cellX + (w*cellSize)) - cellSize) &&
		//	(a.y - m_sand_models->getRadius(index)) > cellY - cellSize && gridChosen == false)
		//{
		//	sandParticles->gridVec[i].push_back(index); //fill vector with index
		//	sandParticles->gridNum[index] = i;
		//}
		w++;
	}
	cellY = originalCellY;
}

bool SandManager::sphereCollisions(int index, int gridNum) // Collision system for spacial partitioning attempt
{
	

	
	for (int i = 0; i<sandParticles->gridVec[gridNum].size(); i++)
	{
		int gridPos = sandParticles->gridVec[gridNum][i];
		float result = sqrtf(pow((sandParticles->sandPositions[index].x - sandParticles->sandPositions[gridPos].x), 2) +
			pow((sandParticles->sandPositions[index].y - sandParticles->sandPositions[gridPos].y), 2) +
			pow((sandParticles->sandPositions[index].z - sandParticles->sandPositions[gridPos].z), 2));


		if (result <= 2/*m_sand_models->getRadius(gridPos) + m_sand_models->getRadius(index)*/)
		{
			if (index == gridPos)
			{
				return false;
			}
			m_sand_models->sphereCollisionResponse(index, gridPos);
			return true;
		}
	}

	return false;
}



