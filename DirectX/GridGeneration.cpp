#include "GridGeneration.h"
#include "Node.h"



GridGeneration::~GridGeneration()
{
	for each(Node* node in m_nodes)
	{
		if (node)
			delete node;

		node = nullptr;
	}

	m_nodes.clear();
}



bool GridGeneration::Initialize(int field_width, int field_height, int node_width, int node_height,
								int target_x, int target_y, int target_z,
								int start_x, int start_y, int start_z, int end_x, int end_y, int end_z)
{
	return Initialize(field_width, field_height, node_width, node_height, DirectX::XMINT3(target_x, target_y, target_z),
						DirectX::XMINT3(start_x, start_y, start_z), DirectX::XMINT3(end_x, end_y, end_z));
}



bool GridGeneration::Initialize(int field_width, int field_height, int node_width, int node_height,
								DirectX::XMINT3 target, DirectX::XMINT3 start, DirectX::XMINT3 end)
{
	m_field_width = field_width;
	m_field_height = field_height;
	m_node_width = node_width;
	m_node_height = node_height;

	m_start = start;
	m_end = end;

	GenerateGrid(target);

	return true;
}



void GridGeneration::GenerateGrid(int target_x, int target_y, int target_z)
{
	GenerateGrid(DirectX::XMINT3(target_x, target_y, target_z));
}



void GridGeneration::GenerateGrid(DirectX::XMINT3 target)
{
	m_target = target;

	m_nodes_wide = m_field_width / m_node_width;
	m_nodes_tall = m_field_height / m_node_height;

	m_num_nodes = m_nodes_wide * m_nodes_tall;
	m_nodes.reserve(m_num_nodes);

	int index = 0;
	for each(Node* node in m_nodes)
	{
		node = new Node();
		node->SetIndexValue(index);

		index++;
	}
}


void GridGeneration::GetTarget(int& target_x, int& target_y, int& target_z)
{
	target_x = m_target.x;
	target_y = m_target.y;
	target_z = m_target.z;
}



void GridGeneration::GetTarget(DirectX::XMINT3& target)
{
	target = m_target;
}



void GridGeneration::GetStart(int& start_x, int& start_y, int& start_z)
{
	start_x = m_start.x;
	start_y = m_start.y;
	start_z = m_start.z;
}



void GridGeneration::GetStart(DirectX::XMINT3& start)
{
	start = m_start;
}



void GridGeneration::GetEnd(int& end_x, int& end_y, int& end_z)
{
	end_x = m_end.x;
	end_y = m_end.y;
	end_z = m_end.z;
}



void GridGeneration::GetEnd(DirectX::XMINT3& end)
{
	end = m_end;
}



void GridGeneration::GenerateNeighbours()
{
	for (int i = 0; i < m_num_nodes; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			int width_offset = 0;
			int height_offset = 0;

			switch (j)
			{
			case 0:
				width_offset = 1;
				break;

			case 1:
				width_offset = -1;
				break;

			case 2:
				height_offset = 1;
				break;

			case 3:
				height_offset = -1;
				break;
			}

			int index_x = i % m_nodes_wide;
			int index_y = i / m_nodes_wide;

			index_x += width_offset;
			index_y += height_offset;

			if (index_x < 0 || index_x >= m_nodes_wide)
				continue;
			if (index_y < 0 || index_y >= m_nodes_tall)
				continue;

			int neighbour_index = index_y + m_nodes_wide * index_x;

			m_nodes[i]->AddNeighbour(m_nodes[neighbour_index]);
		}
	}
}



void GridGeneration::GenerateDistanceValues(int index)
{
	int potential_distance_from_goal = m_nodes[index]->GetGoalDistance() + 1;
	potential_distance_from_goal++;

	for each(Node* node in m_nodes[index]->GetNeighbours())
	{
		if (potential_distance_from_goal < node->GetGoalDistance())
		{
			node->SetGoalDistance(potential_distance_from_goal);
			node->SetParent(m_nodes[index]);

			GenerateDistanceValues(node->GetIndexValue());
		}
	}
}



void GridGeneration::GenerateDirectionValues()
{
}
