#pragma once
#include "d3dclass.h"
class Model;

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
	void gravity();

	void Shutdown();
	
	const DirectX::XMFLOAT3 Direction(const DirectX::XMFLOAT3, const DirectX::XMFLOAT3);
	const DirectX::XMFLOAT3 NormaliseFloat(const DirectX::XMFLOAT3);
	const DirectX::XMFLOAT3 Force(const DirectX::XMFLOAT3, const float mass);
private:
	int SandCount = 1000;
	Model* m_swarm_models = nullptr;
	D3DClass* m_D3D;
	float down = -1.f;

};