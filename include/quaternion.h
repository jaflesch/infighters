#pragma once
#include <cmath>
#include <hmath.h>

#ifndef PI
#define PI 3.14159265
#endif

using namespace hm;

inline float ToRadians(float degree)
{
	return  degree * (float)PI / 180.0f;
}

struct Quaternion
{
	float x, y, z, w;

	Quaternion()
		: x(0), y(0), z(0), w(0)
	{
	}

	Quaternion(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w)
	{
	}

	Quaternion(float angle)
	{
		x = sinf(angle) / 2.0f;
		y = sinf(angle) / 2.0f;
		z = sinf(angle) / 2.0f;
		w = cosf(angle) / 2.0f;
	}

	inline Quaternion operator*(const Quaternion r)
	{
		Quaternion res;
		res.w = this->w*r.w - this->x*r.x - this->y*r.y - this->z*r.z;
		res.x = this->w*r.x + this->x*r.w + this->y*r.z - this->z*r.y;
		res.y = this->w*r.y + this->y*r.w + this->z*r.x - this->x*r.z;
		res.z = this->w*r.z + this->z*r.w + this->x*r.y - this->y*r.x;

		return res;
	}

	inline Quaternion normalize()
	{
		Quaternion q;
		float length = sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);

		q.x /= length;
		q.y /= length;
		q.z /= length;
		q.w /= length;

		return(q);
	}

	inline Quaternion conjugate()
	{
		Quaternion result;
		result.x = -x;
		result.y = -y;
		result.z = -z;
		return (result);
	}
};

inline Quaternion QuatFromAxisAngle(vec3 axis, float angle)
{
	axis = vec3::normalize(axis);
	float sang = sinf(ToRadians(angle) / 2.0f);

	Quaternion quat;
	quat.w = cosf(ToRadians(angle / 2.0f));
	quat.x = axis.x * sang;
	quat.y = axis.y * sang;
	quat.z = axis.z * sang;

	return quat;
}

inline mat4 RotFromQuat(const Quaternion& quat)
{
	mat4 result;

	result.m[0][0] = 1.0f - 2.0f * quat.y * quat.y - 2.0f * quat.z * quat.z;
	result.m[1][0] = 2.0f * quat.x * quat.y + 2.0f * quat.w * quat.z;
	result.m[2][0] = 2.0f * quat.x * quat.z - 2.0f * quat.w * quat.y;
	result.m[3][0] = 0;

	result.m[0][1] = 2.0f * quat.x * quat.y - 2.0f * quat.w * quat.z;
	result.m[1][1] = 1.0f - (2.0f * quat.x * quat.x) - (2.0f * quat.z * quat.z);
	result.m[2][1] = 2.0f * quat.y * quat.z + 2.0f * quat.w * quat.x;
	result.m[3][1] = 0;

	result.m[0][2] = 2.0f * quat.x * quat.z + 2.0f * quat.w * quat.y;
	result.m[1][2] = 2.0f * quat.y * quat.z - 2.0f * quat.w * quat.x;
	result.m[2][2] = 1.0f - (2.0f * quat.x * quat.x) - (2.0f * quat.y * quat.y);
	result.m[3][2] = 0;

	result.m[0][3] = 0;
	result.m[1][3] = 0;
	result.m[2][3] = 0;
	result.m[3][3] = 1;

	return(result);
}
