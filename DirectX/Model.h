#pragma once

//System Includes
#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include <vector>
#include <map>

//Custom Includes
#include "Constants.h"
#include "Texture.h"
//#include <d3dx9shape.h>
#include "Bmath.h"


class SandManager;

class Model
{
public:
	Model() = default;
	~Model() = default;

	bool initialize(char* model_file_name, int instance_count);
	void shutdown();
	void render();

	int getVertexCount();
	int getInstanceCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

	struct InstanceType
	{
		DirectX::XMMATRIX world_matrix;
		DirectX::XMFLOAT3 m_position;
		DirectX::XMFLOAT3 m_velocity;
		float mass = 1.0f;
		int radius = 1.0f;
	};

	struct ModelType
	{
		float x;
		float y;
		float z;

		float tu;
		float tv;
		
		float nx;
		float ny;
		float nz;
	};
	


	bool initializeBuffers(ID3D11Device* device);
	void shutdownBuffers();
	void renderBuffers(ID3D11DeviceContext* device_context);

	/*bool LoadTexture(ID3D11Device*, CHAR*);
	void ReleaseTexture();*/
	
public:
	
	void initInstances();
	void translatePosition(int index = -1, float x = 0.0f, float y = 0.0f, float z = 0.0f);
	void translateRotation(int index = -1, float x = 0.0f, float y = 0.0f, float z = 0.0f);
	void translateScale(int index = -1, float x = 0.0f, float y = 0.0f, float z = 0.0f);
	const DirectX::XMFLOAT3 CreateRandomPosition();
	const DirectX::XMFLOAT3 GetPosition(int);
	const DirectX::XMFLOAT3 SetPosition(int,DirectX::XMFLOAT3);
	bool isColliding(int);

	
	DirectX::XMFLOAT3 getCollisionImpulse();
	DirectX::XMFLOAT3 getNewVelIndex();
	DirectX::XMFLOAT3 MinusFloat3(DirectX::XMFLOAT3, DirectX::XMFLOAT3);
	DirectX::XMFLOAT3 PlusFloat3(DirectX::XMFLOAT3, DirectX::XMFLOAT3);
	DirectX::XMFLOAT3 getNewVelI();
	void sphereCollisionResponse(int, int);
	bool sphereColliding(int);
	void sphereCollisionCalc(int, int);
	void resolveCollision(int, int);

	void setVeolcity(int,DirectX::XMFLOAT3);
	int getCollidedSphere();
	void setCollidedSphere(int i);

	void spacialHashing();
	void createGrid();
	int getMass(int);
	void setMass(int, int);
	int GetCellCount();
	int getRadius(int);
private:
	bool LoadModel(char* file_name);
	void ReleaseModel();
	

	ID3D11Buffer* m_vertex_buffer = nullptr;
	ID3D11Buffer* m_instance_buffer = nullptr;

	int m_vertex_count = 0;
	int m_instance_count = 12000;

	
	float x = 0, y = 0, z = 0;
	ModelType* m_model = nullptr;
	Texture* m_Texture;
	InstanceType* m_instances = nullptr;
	SandManager* sandManager = nullptr;
	D3D11_SUBRESOURCE_DATA m_vertex_data;
	D3D11_SUBRESOURCE_DATA m_instance_data;
	int grid[20][20][20];
	DirectX::XMFLOAT3 impulse;
	DirectX::XMFLOAT3 newIVel;
	DirectX::XMFLOAT3 newIndexVel;
	int collidedSphere;
	int cellCount = 225;
};
template <typename T>
	class hash3
	{
	
		std::map<DirectX::XMFLOAT3, std::vector<Model> > box;
	};