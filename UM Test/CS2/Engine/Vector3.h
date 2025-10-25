#pragma once

class Vector3
{
public:
	Vector3(float x = 0.f, float y = 0.f, float z = 0.f) :
		x(x),
		y(y),
		z(z)
	{
	}

	Vector3(const Vector3&& other) noexcept
	{
		x = other.x;
		y = other.y;
		z = other.z;
	}

	float DistanceSqr(Vector3& vec)
	{
		return
			((vec.x - x) * (vec.x - x)) +
			((vec.y - y) * (vec.y - y)) +
			((vec.z - z) * (vec.z - z));
	}

	Vector3& Sub(const Vector3& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		return *this;
	}

	Vector3& Add(const Vector3& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}

	Vector3 operator+(const Vector3& vec)
	{
		return { x + vec.x, y + vec.y, z + vec.z };
	}

	Vector3 operator-(const Vector3& vec)
	{
		return { x - vec.x, y - vec.y, z - vec.z };
	}

	Vector3 ToScreen();

	float x, y, z;
};