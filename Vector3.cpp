#include "Vector3.h"

float Vector3::length() const
{
	return sqrt((x * x) + (y * y) + (z * z));
}

Vector3& Vector3::normalize()
{
	float len = length();
	if (len != 0) {
		return *this /= len;
	}
	return *this;
}