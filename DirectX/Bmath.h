#pragma once
#include <math.h>
#include <DirectXMath.h>
namespace BMath
{
	class Matrix4;
	class Vector3;
	class Vector2;
	class Quaternion;

	class Vector3
	{
	public:
		Vector3();
		Vector3(float _x, float _y, float _z);
		~Vector3();

		inline float const GetX() const
		{
			return x;
		}
		inline float const GetY() const
		{
			return y;
		}
		inline float const GetZ() const
		{
			return z;
		}

		inline float magnitude()
		{
			return sqrt(x*x + y * y + z * z);
		}

		//add two vectors
		Vector3 operator+(const Vector3 &vec) const
		{
			return Vector3(x + vec.x, y + vec.y, z + vec.z);
		}

		//Multiply
		Vector3 operator*(const Vector3 &vec) const
		{
			return Vector3(x * vec.x, y * vec.y, z * vec.z);
		}

		//Multiply
		Vector3 operator*(const float &ot) const
		{
			return Vector3(x * ot, y * ot, z * ot);
		}

		//Divide
		Vector3 operator/(const float &ot) const
		{
			return Vector3(x / ot, y / ot, z / ot);
		}

		//subtract two vector
		Vector3 operator-(const Vector3 &vec) const
		{
			return Vector3(x - vec.x, y - vec.y, z - vec.z);
		}

		bool operator==(const Vector3 &vec) const
		{
			return x == vec.x && y == vec.y && z == vec.z;
		}

		//normalize this vector
		void normalize()
		{
			float magnitude = sqrtf(x * x + y * y + z * z);
			x /= magnitude;
			y /= magnitude;
			z /= magnitude;
		}

		static Vector3 normalize(Vector3 &vec)
		{
			float magnitude = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

			return Vector3(vec.x / magnitude, vec.y / magnitude, vec.z / magnitude);
		}

		//calculate and return dot product
		float dot(const Vector3 &vec) const
		{
			return x * vec.x + y * vec.y + z * vec.z;
		}

		//calculate and return cross product
		Vector3 cross(const Vector3 &vec) const
		{
			return Vector3(y * vec.z - z * vec.y,
				z * vec.x - x * vec.z,
				x * vec.y - y * vec.x);
		}
		//calculate and return dot product
		static float dot(const Vector3 &a, const Vector3 &b)
		{
			return a.x * b.x + a.y * b.y + a.z * b.z;
		}
		

		//calculate and return cross product
		static Vector3 cross(const Vector3 &a, const Vector3 &b)
		{
			return Vector3(a.y * b.z - a.z * b.y,
				a.z * b.x - a.x * b.z,
				a.x * b.y - a.y * b.x);
		}
		static DirectX::XMFLOAT3 convertVec3toXMFLOAT3(const Vector3 &a)
		{
			float x = a.x;
			float y = a.y;
			float z = a.z;
			return DirectX::XMFLOAT3(x, y, z);
		}
		static Vector3 convertXMFLOAT3toVec3(const DirectX::XMFLOAT3 &a)
		{
			float x = a.x;
			float y = a.y;
			float z = a.z;
			return Vector3(x,y,z);
		}
		float x;
		float y;
		float z;
	protected:
	private:

	};

	class Vector2
	{
	public:
		Vector2();
		Vector2(float _x, float _y);
		~Vector2();
		float x;
		float y;

		Vector2 operator-(const Vector2 &vec) const
		{
			return Vector2(x - vec.x, y - vec.y);
		}
	protected:
	private:

	};

	class Quaternion
	{
	public:
		Quaternion();
		~Quaternion();

		inline float const GetX() const
		{
			return x;
		}
		inline float const GetY() const
		{
			return y;
		}
		inline float const GetZ() const
		{
			return z;
		}
		inline float const GetW() const
		{
			return w;
		}

	private:
		float x;
		float y;
		float z;
		float w;
	};

	Matrix4 translate(Vector3);
	Matrix4 rotateAroundXAxis(float);
	Matrix4 rotateAroundYAxis(float);
	Matrix4 rotateAroundZAxis(float);
	Matrix4 rotate(Quaternion);
	Matrix4 scale(Vector3);
	Matrix4* perspective(float fov, float aspect, float zNear, float zFar);
	Matrix4 lookAt(Vector3 position, Vector3 forward, Vector3 up);

	class Matrix4
	{
	public:
		Matrix4();
		Matrix4(float _11, float  _12, float  _13, float  _14,
			float _21, float  _22, float  _23, float  _24,
			float _31, float  _32, float  _33, float  _34,
			float _41, float  _42, float  _43, float  _44);

		~Matrix4();

		friend Matrix4 operator*(Matrix4&, Matrix4&);

		inline float* Get() const
		{
			return m;
		}

		inline float Get(int point) const
		{
			return m[point];
		}

	private:
		float* m;
	};

}