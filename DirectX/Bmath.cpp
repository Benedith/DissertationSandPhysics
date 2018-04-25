#include "Bmath.h"
using namespace BMath;

Vector3::Vector3()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
}
Vector3::Vector3(float _x = 0.0, float _y = 0.0, float _z = 0.0)
{
	x = _x;
	y = _y;
	z = _z;
}
Vector3::~Vector3()
{

}




Vector2::Vector2()
{
	x = 0.0;
	y = 0.0;
}
Vector2::Vector2(float _x = 0.0, float _y = 0.0)
{
	x = _x;
	y = _y;
}
Vector2::~Vector2()
{

}



BMath::Quaternion::Quaternion()
{}

BMath::Quaternion::~Quaternion()
{}

Matrix4::Matrix4()
{
	m = new float[16];

	m[0] = 1; m[1] = 0; m[2] = 0; m[3] = 0;
	m[4] = 0; m[5] = 1; m[6] = 0; m[7] = 0;
	m[8] = 0; m[9] = 0; m[10] = 1; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}

Matrix4::Matrix4(float _11, float  _12, float  _13, float  _14,
	float _21, float  _22, float  _23, float  _24,
	float _31, float  _32, float  _33, float  _34,
	float _41, float  _42, float  _43, float  _44)
{
	m = new float[16];

	m[0] = _11; m[1] = _12; m[2] = _13; m[3] = _14;
	m[4] = _21; m[5] = _22; m[6] = _23; m[7] = _24;
	m[8] = _31; m[9] = _32; m[10] = _33; m[11] = _34;
	m[12] = _41; m[13] = _42; m[14] = _43; m[15] = _44;
}

BMath::Matrix4::~Matrix4()
{
	delete[] m;
}

Matrix4 BMath::operator*(Matrix4& a, Matrix4& b)
{
	return Matrix4(
		a.Get(0) * b.Get(0) + a.Get(1) * b.Get(4) + a.Get(2) * b.Get(8) + a.Get(3) * b.Get(12),
		a.Get(0) * b.Get(1) + a.Get(1) * b.Get(5) + a.Get(2) * b.Get(9) + a.Get(3) * b.Get(13),
		a.Get(0) * b.Get(2) + a.Get(1) * b.Get(6) + a.Get(2) * b.Get(10) + a.Get(3) * b.Get(14),
		a.Get(0) * b.Get(3) + a.Get(1) * b.Get(7) + a.Get(2) * b.Get(11) + a.Get(3) * b.Get(15),
		a.Get(4) * b.Get(0) + a.Get(5) * b.Get(4) + a.Get(6) * b.Get(8) + a.Get(7) * b.Get(12),
		a.Get(4) * b.Get(1) + a.Get(5) * b.Get(5) + a.Get(6) * b.Get(9) + a.Get(7) * b.Get(13),
		a.Get(4) * b.Get(2) + a.Get(5) * b.Get(6) + a.Get(6) * b.Get(10) + a.Get(7) * b.Get(14),
		a.Get(4) * b.Get(3) + a.Get(5) * b.Get(7) + a.Get(6) * b.Get(11) + a.Get(7) * b.Get(15),
		a.Get(8) * b.Get(0) + a.Get(9) * b.Get(4) + a.Get(10) * b.Get(8) + a.Get(11) * b.Get(12),
		a.Get(8) * b.Get(1) + a.Get(9) * b.Get(5) + a.Get(10) * b.Get(9) + a.Get(11) * b.Get(13),
		a.Get(8) * b.Get(2) + a.Get(9) * b.Get(6) + a.Get(10) * b.Get(10) + a.Get(11) * b.Get(14),
		a.Get(8) * b.Get(3) + a.Get(9) * b.Get(7) + a.Get(10) * b.Get(11) + a.Get(11) * b.Get(15),
		a.Get(12) * b.Get(0) + a.Get(13) * b.Get(4) + a.Get(14) * b.Get(8) + a.Get(15) * b.Get(12),
		a.Get(12) * b.Get(1) + a.Get(13) * b.Get(5) + a.Get(14) * b.Get(9) + a.Get(15) * b.Get(13),
		a.Get(12) * b.Get(2) + a.Get(13) * b.Get(6) + a.Get(14) * b.Get(10) + a.Get(15) * b.Get(14),
		a.Get(12) * b.Get(3) + a.Get(13) * b.Get(7) + a.Get(14) * b.Get(11) + a.Get(15) * b.Get(15));

}

Matrix4 BMath::translate(Vector3 v)
{
	return Matrix4(1, 0, 0, v.GetX(),
		0, 1, 0, v.GetY(),
		0, 0, 1, v.GetZ(),
		0, 0, 0, 1);
}

Matrix4 BMath::rotateAroundXAxis(float q)
{
	return Matrix4(1, 0, 0, 0,
		0, cos(q), sin(q), 0,
		0, -sin(q), cos(q), 0,
		0, 0, 0, 1);

}

Matrix4 BMath::rotateAroundYAxis(float q)
{
	return Matrix4(cos(q), 0, -sin(q), 0,
		0, 1, 0, 0,
		sin(q), 0, cos(q), 0,
		0, 0, 0, 1);
}

Matrix4 BMath::rotateAroundZAxis(float q)
{
	return Matrix4(cos(q), sin(q), 0, 0,
		-sin(q), cos(q), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
}

Matrix4 BMath::rotate(Quaternion quaternion)
{
	float qw = quaternion.GetX();
	float qx = quaternion.GetY();
	float qy = quaternion.GetZ();
	float qz = quaternion.GetW();

	const float n = 1.0f / sqrt(qx*qx + qy * qy + qz * qz + qw * qw);
	qx *= n;
	qy *= n;
	qz *= n;
	qw *= n;
	return Matrix4(
		1.0f - 2.0f*qy*qy - 2.0f*qz*qz, 2.0f*qx*qy - 2.0f*qz*qw, 2.0f*qx*qz + 2.0f*qy*qw, 0.0f,
		2.0f*qx*qy + 2.0f*qz*qw, 1.0f - 2.0f*qx*qx - 2.0f*qz*qz, 2.0f*qy*qz - 2.0f*qx*qw, 0.0f,
		2.0f*qx*qz - 2.0f*qy*qw, 2.0f*qy*qz + 2.0f*qx*qw, 1.0f - 2.0f*qx*qx - 2.0f*qy*qy, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}
Matrix4 BMath::scale(Vector3 v)
{
	return Matrix4(v.GetX(), 0, 0, 0,
		0, v.GetY(), 0, 0,
		0, 0, v.GetZ(), 0,
		0, 0, 0, 1);
}

Matrix4* BMath::perspective(float fov, float aspect, float zNear, float zFar)
{
	float const tanHalfFovy = tan(fov / 2.0f);

	float _00 = 1.0f / (aspect * tanHalfFovy);
	float _11 = 1.0f / (tanHalfFovy);
	float _22 = zFar / (zNear - zFar);
	float _23 = -1.0f;
	float _32 = -(zFar * zNear) / (zFar - zNear);
	return new Matrix4(_00, 0, 0, 0,
		0, _11, 0, 0,
		0, 0, _22, _32,
		0, 0, _23, 1);
}
Matrix4 BMath::lookAt(Vector3 position, Vector3 forward, Vector3 up)
{
	Vector3 zaxis = forward;
	Vector3	xaxis = Vector3::normalize(Vector3::cross(up, zaxis));
	Vector3	yaxis = Vector3::cross(zaxis, xaxis);

	Matrix4 Rotation(xaxis.GetX(), yaxis.GetX(), zaxis.GetX(), 0,
		xaxis.GetY(), yaxis.GetY(), zaxis.GetY(), 0,
		xaxis.GetZ(), yaxis.GetZ(), zaxis.GetZ(), 0,
		0, 0, 0, 1);
	Matrix4 Translation(1, 0, 0, -position.GetX(),
		0, 1, 0, -position.GetY(),
		0, 0, 1, position.GetZ(),
		0, 0, 0, 1);
	return Rotation * Translation;

}
