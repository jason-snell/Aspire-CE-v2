#pragma once

class ESP
{
public:
	static void make2DBox(int i, float *color, int type);
	static void drawBones(centity_s *entity, float *color);
	static void RotatePoint2(vector3 point, vector3 center, vector3 angles, vector3 &out);
	static void makeEspLine(vector3 Center, float X1, float Y1, float Z1, float X2, float Y2, float Z2, float *Color, float Width, vector3 angle);
	static void make3DBox(vector3 Center, float W, float H, float *Color, vector3 angle);
	static void drawBoundingBox(int c, float *color);
	static void worldPosToCompassPos(rectDef_s* radar, vector3 world, vector2 *compass);
	static void drawEntities();
	static void drawESP();
};

