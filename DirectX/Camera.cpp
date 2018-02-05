#include "Camera.h"
#include "Constants.h"

void Camera::setPosition(float x, float y, float z)
{
	m_position_x = x;
	m_position_y = y;
	m_position_z = z;
}



void Camera::setPosition(DirectX::XMFLOAT3 new_position)
{
	setPosition(new_position.x, new_position.y, new_position.z);
}



void Camera::setRotation(float x, float y, float z)
{
	m_rotation_x = x;
	m_rotation_y = y;
	m_rotation_z = z;
}



void Camera::setRotation(DirectX::XMFLOAT3 new_rotation)
{
	setRotation(new_rotation.x, new_rotation.y, new_rotation.z);
}



DirectX::XMFLOAT3 Camera::getPosition()
{
	return DirectX::XMFLOAT3(m_position_x, m_position_y, m_position_z);
}



DirectX::XMFLOAT3 Camera::getRotation()
{
	return DirectX::XMFLOAT3(m_rotation_x, m_rotation_y, m_rotation_z);
}



void Camera::render()
{
	DirectX::XMFLOAT3 up;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 look_at;

	DirectX::XMVECTOR up_vector;
	DirectX::XMVECTOR position_vector;
	DirectX::XMVECTOR look_at_vector;
	
	DirectX::XMMATRIX rotation_matrix;
	float yaw = 0.0f;
	float pitch = 0.0f;
	float roll = 0.0f;

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;
	up_vector = DirectX::XMLoadFloat3(&up);

	position.x = m_position_x;
	position.y = m_position_y;
	position.z = m_position_z;
	position_vector = DirectX::XMLoadFloat3(&position);

	look_at.x = 0.0f;
	look_at.y = 0.0f;
	look_at.z = 1.0f;
	look_at_vector = DirectX::XMLoadFloat3(&look_at);

	pitch = m_rotation_x * Constant::RAD;
	yaw = m_rotation_y * Constant::RAD;
	roll = m_rotation_z * Constant::RAD;

	rotation_matrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	look_at_vector = DirectX::XMVector3TransformCoord(look_at_vector, rotation_matrix);
	up_vector = DirectX::XMVector3TransformCoord(up_vector, rotation_matrix);

	look_at_vector = DirectX::XMVectorAdd(position_vector, look_at_vector);

	m_view_matrix = DirectX::XMMatrixLookAtLH(position_vector, look_at_vector, up_vector);
}



void Camera::getViewMatrix(DirectX::XMMATRIX& v_matrix)
{
	v_matrix = m_view_matrix;
}
