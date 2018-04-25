#include "Model.h"
#include "GraphicsClass.h"
#include <random>
#include "SandManager.h"

bool Model::initialize(char* model_file_name, int instance_count)
{
	
	
	bool result;
	
	if (!LoadModel(model_file_name))
		return false;

	if (!initializeBuffers(GraphicsClass::m_direct_3d->getRawDevice()))
		return false;
	

	return true;
}



void Model::shutdown()
{
	shutdownBuffers();

	ReleaseModel();
}



void Model::render()
{
	renderBuffers(GraphicsClass::m_direct_3d->getRawDeviceContext());
}



int Model::getVertexCount()
{
	return m_vertex_count;
}



int Model::getInstanceCount()
{
	return m_instance_count;
}

ID3D11ShaderResourceView * Model::GetTexture()
{
	return m_Texture->GetTexture();
}



bool Model::initializeBuffers(ID3D11Device * device)
{
	VertexType* vertices = nullptr;

	D3D11_BUFFER_DESC vertex_buffer_description;
	D3D11_BUFFER_DESC instance_buffer_description;


	HRESULT result;

	vertices = new VertexType[m_vertex_count];
	if (!vertices)
		return false;

	for (int i = 0; i < m_vertex_count; i++)
	{
		vertices[i].position = DirectX::XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].color = DirectX::XMFLOAT4( 1.000f, 0.843f, 0.000f, 1.0f);
	}

	
	vertex_buffer_description.Usage = D3D11_USAGE_DEFAULT;
	vertex_buffer_description.ByteWidth = sizeof(VertexType) * m_vertex_count;
	vertex_buffer_description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertex_buffer_description.CPUAccessFlags = 0;
	vertex_buffer_description.MiscFlags = 0;
	vertex_buffer_description.StructureByteStride = 0;

	
	m_vertex_data.pSysMem = vertices;
	m_vertex_data.SysMemPitch = 0;
	m_vertex_data.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertex_buffer_description, &m_vertex_data, &m_vertex_buffer);
	if (FAILED(result))
	{
		return false;
	}

	if (vertices)
		delete[] vertices;
	vertices = nullptr;

	m_instances = new InstanceType[m_instance_count];
	if (!m_instances)
		return false;

	instance_buffer_description.Usage = D3D11_USAGE_DYNAMIC;
	instance_buffer_description.ByteWidth = sizeof(InstanceType) * m_instance_count;
	instance_buffer_description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instance_buffer_description.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	instance_buffer_description.MiscFlags = 0;
	instance_buffer_description.StructureByteStride = 0;

	result = device->CreateBuffer(&instance_buffer_description, 0, &m_instance_buffer);
	
	initInstances();
	
	if (FAILED(result))
		return false;

	return true;
}



void Model::shutdownBuffers()
{
	if (m_instance_buffer)
		m_instance_buffer->Release();

	m_instance_buffer = nullptr;

	if (m_vertex_buffer)
		m_vertex_buffer->Release();

	m_vertex_buffer = nullptr;
}



void Model::renderBuffers(ID3D11DeviceContext* device_context)
{
	D3D11_MAPPED_SUBRESOURCE mapped_resource;
	bool result = false;

	result = device_context->Map(m_instance_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
	if (FAILED(result))
		return;

	InstanceType* instances = reinterpret_cast<InstanceType*>(mapped_resource.pData);
	
	bool positive = true;
	for (int i = 0; i < m_instance_count; i++)
	{
		instances[i].world_matrix = m_instances[i].world_matrix;
	}

	device_context->Unmap(m_instance_buffer, 0);

	unsigned int stride[2] = { sizeof(VertexType), sizeof(InstanceType) };
	unsigned int offset[2] = { 0, 0 };
	ID3D11Buffer* buffer_ptr[2] = { m_vertex_buffer, m_instance_buffer };

	device_context->IASetVertexBuffers(0, 2, buffer_ptr, stride, offset);
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}




void Model::initInstances()
{
	srand(time(NULL));
	D3D11_MAPPED_SUBRESOURCE mapped_resource;
	bool result = false;

	result = GraphicsClass::m_direct_3d->getRawDeviceContext()->Map(m_instance_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
	if (FAILED(result))
		return;

	InstanceType* instances = reinterpret_cast<InstanceType*>(mapped_resource.pData);
	DirectX::XMFLOAT3 spawn = { 0,-1,0 };
	int w = 0;
	for (int i = 0; i < m_instance_count; i++)
	{
		auto randomspawn = CreateRandomPosition();
		float x = 0, y = 0, z = 0;
		
		DirectX::XMMATRIX world_matrix = DirectX::XMMatrixIdentity();
		
		world_matrix *= DirectX::XMMatrixTranslation(randomspawn.x, randomspawn.y, randomspawn.z);
		world_matrix = DirectX::XMMatrixTranspose(world_matrix);
		
		m_instances[i].world_matrix = world_matrix;
		m_instances[i].mass = rand() % 3 + 1; // diffo massooos

		instances[i].world_matrix = m_instances[i].world_matrix;
		m_instances[i].m_position = (randomspawn);
	
		
		spawn.y += 5;
		w++;
		if (w == 10)
		{
			spawn.x += 3;
			spawn.z -= 5;
			w = 0;
		}
		m_instances[40].radius = 10.0;
		m_instances[40].mass = 10.0;
	}
	

	GraphicsClass::m_direct_3d->getRawDeviceContext()->Unmap(m_instance_buffer, 0);
}



void Model::translatePosition(int index, float x, float y, float z)
{
	D3D11_MAPPED_SUBRESOURCE mapped_resource;
	bool result = false;
	DirectX::XMFLOAT3 temppos = { GetPosition(index).x + x, GetPosition(index).y + y, GetPosition(index).z + z };
	
	SetPosition(index, temppos);
	result = GraphicsClass::m_direct_3d->getRawDeviceContext()->Map(m_instance_buffer, 0, 
		D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
	if (FAILED(result))
		return;

	InstanceType* instances = reinterpret_cast<InstanceType*>(mapped_resource.pData);

	DirectX::XMMATRIX world_matrix = m_instances[index].world_matrix;
	world_matrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(x, y, z));
	m_instances[index].m_position.x = x;
	m_instances[index].m_position.y = y;
	m_instances[index].m_position.z = z;
	//world_matrix = DirectX::XMMatrixTranspose(world_matrix);

	m_instances[index].world_matrix = world_matrix;
	instances[index].world_matrix = m_instances[index].world_matrix;

	GraphicsClass::m_direct_3d->getRawDeviceContext()->Unmap(m_instance_buffer, 0);

}



void Model::translateRotation(int index, float x, float y, float z)
{
	D3D11_MAPPED_SUBRESOURCE mapped_resource;
	bool result = false;

	result = GraphicsClass::m_direct_3d->getRawDeviceContext()->Map(m_instance_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
	if (FAILED(result))
		return;

	InstanceType* instances = reinterpret_cast<InstanceType*>(mapped_resource.pData);

	DirectX::XMMATRIX world_matrix = m_instances[index].world_matrix;
	world_matrix *= DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationRollPitchYaw(x, y, z));
	//world_matrix = DirectX::XMMatrixTranspose(world_matrix);

	m_instances[index].world_matrix = world_matrix;
	instances[index].world_matrix = m_instances[index].world_matrix;

	GraphicsClass::m_direct_3d->getRawDeviceContext()->Unmap(m_instance_buffer, 0);
}



void Model::translateScale(int index, float x, float y, float z)
{
	x++;
	y++;
	z++;

	D3D11_MAPPED_SUBRESOURCE mapped_resource;
	bool result = false;

	result = GraphicsClass::m_direct_3d->getRawDeviceContext()->Map(m_instance_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
	if (FAILED(result))
		return;
	InstanceType* instances = reinterpret_cast<InstanceType*>(mapped_resource.pData);
	
	DirectX::XMMATRIX world_matrix = m_instances[index].world_matrix;
	world_matrix *= DirectX::XMMatrixTranspose(DirectX::XMMatrixScaling(x,y,z));

	m_instances[index].world_matrix = world_matrix;
	instances[index].world_matrix = m_instances[index].world_matrix;

	GraphicsClass::m_direct_3d->getRawDeviceContext()->Unmap(m_instance_buffer, 0);
}

const DirectX::XMFLOAT3 Model::CreateRandomPosition()
{
	std::random_device randDevice;
	std::mt19937 generate(randDevice());
	std::uniform_real_distribution<> distribute(static_cast<double>(100), static_cast<double>(150));


	auto x = static_cast<float>(distribute(generate));
	auto y = static_cast<float>(distribute(generate));
	auto z = static_cast<float>(distribute(generate));

	return { x, y, z};
}

const DirectX::XMFLOAT3 Model::GetPosition(int index)
{
	return m_instances[index].m_position;
}

const DirectX::XMFLOAT3 Model::SetPosition(int index,DirectX::XMFLOAT3 pos)
{

	return m_instances[index].m_position = pos;
}

bool Model::isColliding(int index)
{
	DirectX::XMFLOAT3 tempMin = { m_instances[index].m_position.x - 1.f, m_instances[index].m_position.y - 1.f,
		m_instances[index].m_position.z - 1.f };
	DirectX::XMFLOAT3 tempMax = { m_instances[index].m_position.x + 1.f, m_instances[index].m_position.y + 1.f,
		m_instances[index].m_position.z + 1.f };


	for (int i = 0; i < m_instance_count; i++)
	{
		DirectX::XMFLOAT3 tempMin2 = { m_instances[i].m_position.x - 1.f, m_instances[i].m_position.y - 1.f,
			m_instances[i].m_position.z - 1.f };
		DirectX::XMFLOAT3 tempMax2 = { m_instances[i].m_position.x + 1.f, m_instances[i].m_position.y + 1.f,
			m_instances[i].m_position.z + 1.f };
		
		if ((tempMin2.x <= tempMax.x && tempMax2.x >= tempMin.x) &&
			(tempMin2.y <= tempMax.y && tempMax2.y >= tempMin.y) &&
			(tempMin2.z <= tempMax.z && tempMax2.z >= tempMin.z))
		{
			if (index == i)
			{
				return false;
			}
			return true;
		}

	}
	return false;
} //Box collider


DirectX::XMFLOAT3 Model::getCollisionImpulse()
{
	
	float x = impulse.x;
	float y = impulse.y;
	float z = impulse.z;
	return (DirectX::XMFLOAT3(x,y,z));
}

void Model::sphereCollisionResponse(int index, int i)
{
	
	float m1, m2, x1, x2;
	Vector3 temp;
	Vector3 v1temp, vector1, vector2, vector1x, vector2x, vector1y, vector2y, x(temp.convertXMFLOAT3toVec3(
		MinusFloat3(m_instances[index].m_position, m_instances[i].m_position)));
	
	x.normalize();
	
	float a, b, c;
	
	vector1 = Vector3(temp.convertXMFLOAT3toVec3(m_instances[index].m_velocity));
	x1 = x.dot(vector1);
	vector1x = x * x1;
	vector1y = vector1 - vector1x;
	m1 = m_instances[index].mass;

	x = x * -1;
	vector2 = (temp.convertXMFLOAT3toVec3(m_instances[i].m_velocity));
	x2 = x.dot(vector2);
	vector2x = x * x2;
	vector2y = vector2 - vector2x;
	m2 = m_instances[i].mass;

	Vector3 newIndexVec = Vector3(vector1x*(m1 - m2) / (m1 + m2) + vector2x * (2 * m2) / (m1 + m2) + vector1y);
	Vector3 newIVec = Vector3(vector1x*(2 * m1) / (m1 + m2) + vector2x * (m2 - m1) / (m1 + m2) + vector2y);

	newIndexVel = temp.convertVec3toXMFLOAT3(newIndexVec);
	newIVel = temp.convertVec3toXMFLOAT3(newIVec);
	


	
}

bool Model::sphereColliding(int index)
{
	
	for (int i = 0; i < m_instance_count; i++)
	{
		float result = sqrtf(pow((m_instances[index].m_position.x - m_instances[i].m_position.x), 2)+
			pow((m_instances[index].m_position.y - m_instances[i].m_position.y), 2)+
				pow((m_instances[index].m_position.z - m_instances[i].m_position.z), 2));

		
		if (result <= (m_instances[i].radius + m_instances[index].radius))
		{
			if (index == i)
			{
				return false;
			}
			sphereCollisionResponse(index, i);
			return true;
		}
	}

	return false;


	
}

void Model::sphereCollisionCalc(int index, int i)
{

	float deltaX = m_instances[i].m_position.x - m_instances[index].m_position.x +m_instances[i].radius;
	float deltaY = m_instances[i].m_position.y - m_instances[index].m_position.y + m_instances[i].radius;
	float deltaZ = m_instances[i].m_position.z - m_instances[index].m_position.z+ m_instances[i].radius;
	float SqaureValueCheck = ((deltaX + deltaY + deltaZ)) * (m_instances[i].m_velocity.x +
		m_instances[i].m_velocity.y +
		m_instances[i].m_velocity.z);
	if (SqaureValueCheck < 0)
	{
		SqaureValueCheck*-1;
	}
	float rootedVal = (sqrt(SqaureValueCheck));
	float value = ((deltaX * m_instances[i].m_velocity.x) +       // gives angle of i for 2 colliding spheres i and index
		(deltaY * m_instances[i].m_velocity.y) +
		(deltaZ * m_instances[i].m_velocity.z));
	
	
	float angle = acos(((deltaX * m_instances[i].m_velocity.x) +       // gives angle of i for 2 colliding spheres i and index
		(deltaY * m_instances[i].m_velocity.y) +
		(deltaZ * m_instances[i].m_velocity.z)) /
		(sqrt(SqaureValueCheck/*(deltaX + deltaY + deltaZ )) * (m_instances[i].m_velocity.x +
			m_instances[i].m_velocity.y +
			m_instances[i].m_velocity.z)*/)));
	
	
	Vector3 centerVel1 = Vector3((m_instances[i].m_velocity.x * cos(angle)),
		(m_instances[i].m_velocity.y * cos(angle)),
		(m_instances[i].m_velocity.z * cos(angle)));

	Vector3 centerVel2 = Vector3((m_instances[index].m_velocity.x * cos(angle)),
		(m_instances[index].m_velocity.y * cos(angle)),
		(m_instances[index].m_velocity.z * cos(angle)));

	

	Vector3 vnormalI = (vnormalI.convertXMFLOAT3toVec3(m_instances[i].m_velocity)) - centerVel1;
	Vector3 newVel = vnormalI + centerVel2;
	
	newIndexVel = newVel.convertVec3toXMFLOAT3(newVel) ;
	/*newFloatVel = newVel.convertVec3toXMFLOAT3(newVel);*/

}

void Model::resolveCollision(int index, int i)
{
	const float cor = 0.7f;
	Vector3 v = Vector3((m_instances[index].m_velocity.x - m_instances[i].m_velocity.x),
		(m_instances[index].m_velocity.y - m_instances[i].m_velocity.y),
		(m_instances[index].m_velocity.z - m_instances[i].m_velocity.z));
	Vector3 collidingS = Vector3((m_instances[i].m_velocity.x),
		(m_instances[i].m_velocity.x),
		(m_instances[i].m_velocity.x));

	float vn = v.dot(v,collidingS);
	float imp = ((-(1.0f + cor)* vn) / (m_instances[index].mass + m_instances[i].mass)); //divided by the mass of the two spheres
	Vector3 pulse = collidingS * imp;
impulse = pulse.convertVec3toXMFLOAT3(pulse);
	
	

}

void Model::setVeolcity(int index, DirectX::XMFLOAT3 velocity)
{
	m_instances[index].m_velocity = velocity;

}

void Model::setCollidedSphere(int i)
{
	collidedSphere = i;
}

int Model::getCollidedSphere()
{

	return collidedSphere;
}

void Model::spacialHashing()
{


}

void Model::createGrid()
{
	
		for (x = 0; x<20; x++) 
		{
			for (y = 0; y<20; y++) 
			{
				for (int z = 0; z < 20; z++) 
				{

				}
						
			}
				
		}

}

int Model::getMass(int i)
{
	return m_instances[i].mass;
}

void Model::setMass(int i, int massNum)
{
	m_instances[i].mass = massNum;

}

int Model::GetCellCount()
{
	return cellCount;
}

int Model::getRadius(int i)
{
	return m_instances[i].radius;
}

bool Model::LoadModel(char * file_name)
{
	std::ifstream file_in;
	char input;

	//Opening the model file
	file_in.open(file_name);
	if (file_in.fail())
		return false;

	//Reading in vertex count
	file_in.get(input);
	while (input != ':')
		file_in.get(input);

	file_in >> m_vertex_count;

	//Loading memory for the model
	m_model = new ModelType[m_vertex_count];
	if (!m_model)
		return false;

	file_in.get(input);
	while (input != ':')
		file_in.get(input);

	file_in.get(input);
	file_in.get(input);

	for (int i = 0; i < m_vertex_count; i++)
	{
		file_in >> m_model[i].x;
		file_in >> m_model[i].y;
		file_in >> m_model[i].z;

		file_in >> m_model[i].tu;
		file_in >> m_model[i].tv;

		file_in >> m_model[i].nx;
		file_in >> m_model[i].ny;
		file_in >> m_model[i].nz;
	}

	file_in.close();

	return true;
}



void Model::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = nullptr;
	}
}
DirectX::XMFLOAT3 Model::getNewVelIndex()
{
	
	return newIndexVel;
}
DirectX::XMFLOAT3 Model::getNewVelI()
{
	return newIVel;
}

DirectX::XMFLOAT3 Model::MinusFloat3(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b)
{
	DirectX::XMFLOAT3 minusF = DirectX::XMFLOAT3((a.x - b.x), (a.y - b.y), (a.z - b.z));
	return minusF;
}

DirectX::XMFLOAT3 Model::PlusFloat3(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b)
{
	DirectX::XMFLOAT3 plusF = DirectX::XMFLOAT3((a.x + b.x), (a.y + b.y), (a.z + b.z));

	return plusF;
}

