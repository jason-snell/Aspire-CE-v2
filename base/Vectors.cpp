#include "main.h"

vector2::vector2()
{
	x = 0.0f;
	y = 0.0f;
}

vector2::vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

vector2 &vector2::operator+=(vector2 &vec)
{
	return vector2(this->x += vec.x, this->y += vec.y);
}

vector2 &vector2::operator-(vector2 &vec)
{
	return vector2(this->x - vec.x, this->y - vec.y);
}

vector3::vector3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

vector3::vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

float vector3::dotProduct(vector3 &Vector)
{
	return (this->x * Vector.x) + (this->y * Vector.y) + (this->z * Vector.z);
}

vector3 &vector3::operator-(vector3 &vec)
{
	return vector3(this->x - vec.x, this->y - vec.y, this->z - vec.z);
}

vector3 &vector3::operator+=(vector3 &vec)
{
	return vector3(this->x += vec.x, this->y += vec.y, this->z += vec.z);
}

vector3 &vector3::operator+(vector3 &vec)
{
	return vector3(this->x + vec.x, this->y + vec.y, this->z + vec.z);
}

float vector3::distanceEx(vector3 &Vector)
{
	float _x = this->x - Vector.x, _y = this->y - Vector.y, _z = this->z - Vector.z;
	return ((_x * _x) + (_y * _y) + (_z * _z));
}

float vector3::distance(vector3 &vector)
{
	return sqrtf(distanceEx(vector));
}

void SinCos(const float &r, float &s, float &c)
{
	s = sin(r);
	c = cos(r);
}

void vector3::rotate2d(float ang)
{
	float _x, _y;

	float s, c;

	SinCos(D3DXToRadian(ang), s, c);

	_x = this->x;
	_y = this->y;

	this->x = (_x * c) - (_y * s);
	this->y = (_x * s) + (_y * c);
}

Vector4::Vector4()
{
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
	this->a = 0.0f;
}

Vector4::Vector4(float x, float y, float z, float a)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->a = a;
}