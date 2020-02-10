#pragma once

#define XM_2PI              6.283185307f

class vector2
{
public:
	float x, y;

	vector2();
	vector2(float x, float y);
	vector2 &operator+= (vector2 &vec);
	vector2 &operator-(vector2 &vec);
};

class vector3
{
public:
	float x, y, z;

	vector3();
	vector3(float x, float y, float z);
	float dotProduct(vector3 &Vector);
	vector3 &operator-(vector3 &vec);
	vector3 &operator+=(vector3 &vec);
	vector3 &operator+(vector3 &vec);
	float distanceEx(vector3 &Vector);
	float distance(vector3 &vector);
	void rotate2d(float ang);
};

class Vector4
{
public:
	float x, y, z, a;
	Vector4();
	Vector4(float x, float y, float z, float a);
};

static vector3 VectorToAngles(vector3 &Delta)
{
	float Forward, Yaw, Pitch;

	if (Delta.x == 0 && Delta.y == 0)
	{
		Yaw = 0;
		if (Delta.z > 0)
			Pitch = 90.0f;
		else
			Pitch = 270.0f;
	}
	else
	{
		if (Delta.x)
			Yaw = (atan2(Delta.y, Delta.x) * 57.2957804f);
		else if (Delta.y > 0)
			Yaw = 90.0f;
		else
			Yaw = 270.0f;

		if (Yaw < 0)
			Yaw += 360.0f;

		Forward = sqrt((Delta.x * Delta.x) + (Delta.y * Delta.y));
		Pitch = (atan2(Delta.z, Forward) * 57.2957804f);

		if (Pitch < 0)
			Pitch += 360;
	}
	return vector3(-Pitch, Yaw, 0);
}

static void AngleVectors(vector3 angles, vector3 *forward, vector3 *right, vector3 *up) {

	float angle;
	float sr, sp, sy, cr, cp, cy;

	angle = angles.y * 0.0174532f;
	sy = sin(angle);
	cy = cos(angle);
	angle = angles.x * 0.0174532f;
	sp = sin(angle);
	cp = cos(angle);
	angle = angles.z * 0.0174532f;
	sr = sin(angle);
	cr = cos(angle);


	if (forward) {
		forward->x = cp*cy;
		forward->y = cp*sy;
		forward->z = -sp;
	}
	if (right) {
		right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
		right->y = (-1 * sr*sp*sy + -1 * cr*cy);
		right->z = -1 * sr*cp;
	}

	if (up) {
		up->x = (cr*sp*cy + -sr*-sy);
		up->y = (cr*sp*sy + -sr*cy);
		up->z = cr*cp;
	}
}

static float VectorSize(vector3 a)
{
	return a.x + a.y + a.z;
}

static vector3 VectorSubtract(vector3 a, vector3 b)
{
	vector3 c = vector3();
	if (VectorSize(a) > VectorSize(b)) {
		c.x = a.x - b.x;
		c.y = a.y - b.y;
		c.z = a.z - b.z;
	}
	else {
		c.x = b.x - a.x;
		c.y = b.y - a.y;
		c.z = b.z - a.z;
	}
	return c;
}

static void AnglesToAxis(vector3 Angles, vector3 Axis[3])
{
	vector3 Right;

	AngleVectors(Angles, &Axis[0], &Right, &Axis[2]);
	VectorSubtract(Right, Axis[1]);
}