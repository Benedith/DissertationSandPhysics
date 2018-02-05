#pragma once
#include <DirectXMath.h>
#include <vector>

class Node;


class GridGeneration
{
public:
	GridGeneration() = default;
	~GridGeneration();

	bool Initialize(int field_width = 100, int field_height = 100, int node_width = 10, int node_height = 10,
					int target_x = 0, int target_y = 0, int target_z = 0,
					int start_x = 0, int start_y = 0, int start_z = 0, int end_x = 0, int end_y = 0, int end_z = 0);

	bool Initialize(int field_width = 100, int field_height = 100, int node_width = 10, int node_height = 10,
					DirectX::XMINT3 target = DirectX::XMINT3(0, 0, 0), 
					DirectX::XMINT3 start = DirectX::XMINT3(0, 0, 0), DirectX::XMINT3 end = DirectX::XMINT3(0, 0, 0));

	void GenerateGrid(int target_x = 0, int target_y = 0, int target_z = 0);
	void GenerateGrid(DirectX::XMINT3 target = DirectX::XMINT3(0, 0, 0));

	void GetTarget(int& target_x, int& target_y, int& target_z);
	void GetTarget(DirectX::XMINT3& target);
	
	void GetStart(int& start_x, int& start_y, int& start_z);
	void GetStart(DirectX::XMINT3& start);

	void GetEnd(int& end_x, int& end_y, int& end_z);
	void GetEnd(DirectX::XMINT3& end);

private:
	//Private Variables
	int m_field_width = 100;
	int m_field_height = 100;

	int m_node_width = 10;
	int m_node_height = 10;

	int m_nodes_wide = 0;
	int m_nodes_tall = 0;
	int m_num_nodes = 0;

	int m_target_index = 0;
	DirectX::XMINT3 m_target = DirectX::XMINT3(0, 0, 0);
	DirectX::XMINT3 m_start = DirectX::XMINT3(0, 0, 0);
	DirectX::XMINT3 m_end = DirectX::XMINT3(0, 0, 0);

	std::vector<Node*> m_nodes;



	//Private Functions
	void GenerateNeighbours();
	void GenerateDistanceValues(int index);
	void GenerateDirectionValues();
};