#pragma once

//System includes
#include <DirectXMath.h>


//Custom includes



class Camera
{
public:
	Camera() = default;
	~Camera() = default;

	void setPosition(float x, float y, float z);
	void setPosition(DirectX::XMFLOAT3 new_position);

	void setRotation(float x, float y, float z);
	void setRotation(DirectX::XMFLOAT3 new_rotation);

	DirectX::XMFLOAT3 getPosition();
	DirectX::XMFLOAT3 getRotation();

	void render();
	void getViewMatrix(DirectX::XMMATRIX& v_matrix);

private:
	float m_position_x = 0.0f;
	float m_position_y = 0.0f;
	float m_position_z = 0.0f;

	float m_rotation_x = 0.0f;
	float m_rotation_y = 0.0f;
	float m_rotation_z = 0.0f;

	DirectX::XMMATRIX m_view_matrix;
};