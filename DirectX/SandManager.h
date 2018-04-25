#pragma once
#include "d3dclass.h"
#include <vector>
#include "Bmath.h"
#include <AntTweakBar.h>
#include <time.h>
#include "GraphicsClass.h"
using namespace BMath;
class Model;
class GraphicsClass;


class SandManager
{
public:
	SandManager() = default;
	~SandManager() = default;

	bool Initialize(char* model_file_path, int instance_count);
	void Render();
	void Update();

	int GetVertexCount();
	int GetInstanceCount();
	void FindAverages();
	void gravity();
	void SandLayer(int);
	void LayerGravity();

	void Shutdown();
	void AntTweakBar();
	void setImpact();
	void SpatialPartitioning(int i);
	bool sphereCollisions(int, int);
private:
	int SandCount = 4000;
	int impactCount = 100;
	int runCount = 0;
	Model* m_sand_models = nullptr;
	Model* m_impact_model = nullptr;
	D3DClass* m_D3D;
	TwBar *myBar;
	GraphicsClass* graphicsclass;
	
	double  averageFrameTimeMilliseconds;
	
	struct SandStruct
	{
		std::vector<Vector3> sandPositions;
		std::vector<Vector3> sandVelocities;
		std::vector<Vector3> sandAccelerations;
		
		std::vector<std::vector<int>> gridVec;
		std::vector<int> gridNum;
	};

	std::vector<int> sandNums;
	SandStruct* sandParticles;
	int numCounter = 0;
	int nCount = 0;

	float GravitationalForce = 0.1f;

	//Border
	float maxPos = 200.0f;
	float minPos = 50.0f;
	float pushForce = 0.5f;
	float pushforceNegative = -0.5f;

	//Averages
	Vector3 averageSandPos = Vector3();
	Vector3 averageSandVel = Vector3(10,-10,10); //-10 y to stop it activating imediatly
	bool applyWind = true;
	int windForce = 1;
	int whyWindWhy = 0;
	bool resetCollision = false;
	float newMass;

	
	int cellX = 60;
	int cellY = 60;
	int cellSize = 10;
	int cellCount;

	bool gridChosen = false;
	
};