#pragma once
#include <DirectXMath.h>
#include <vector>

class Node
{
public:
	Node() = default;
	~Node();

	void SetVelocity(DirectX::XMFLOAT3 velocity);
	void SetVelocity(float x = 0.0f, float y =  0.0f, float z = 0.0f);
	DirectX::XMFLOAT3 GetVelocity();
	void GetVelocity(float& x, float& y, float& z);

	void SetGoalDistance(int distance_from_goal = 0);
	int GetGoalDistance();

	void SetParent(Node* parent);
	Node* GetParent();

	void AddNeighbour(Node* neighbour);
	std::vector<Node*> GetNeighbours();

	void SetWalkable(bool walkable = true);
	bool GetWalkable();

	void SetIndexValue(int index);
	int GetIndexValue();

private:
	int m_distance_from_goal = 0;
	DirectX::XMFLOAT3 m_velocity = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	Node* m_parent = nullptr;
	std::vector<Node*> m_neighbours;
	bool m_walkable = false;

	int m_index = 9999;
};