#include "main.h"

void ESP::make2DBox(int i, float *color, int type)
{
	vector3 Top = vector3();

	if (func::getTagPos(i, vars->cachedBones[j_helmet], Top))
	{
		vector3 bottom = Entities[i].Origin;

		vector2 Screen = vector2();

		vector2 max = vector2();
		func::worldToScreen(Top, max);
		vector2 min = vector2();
		func::worldToScreen(bottom, min);
		vector2 vpjCenter = vector2();
		func::worldToScreen(bottom, vpjCenter);

		float flWidth = fabs((max.y - min.y) / 4.0f);

		float fillColor[4];
		for (int i = 0; i < 4; i++)
			fillColor[i] = color[i];
		fillColor[3] = 0.1f;

		switch (type)
		{
		case BOX_TYPE_CORNER: draw::corner(vpjCenter.x - flWidth - 6.0f, max.y - 4.0f, flWidth * 2.0f + 12.0f, min.y - max.y + 8.0f, color);
			break;
		case BOX_TYPE_2D: draw::emptyRect(vpjCenter.x - flWidth - 6.0f, max.y - 4.0f, flWidth * 2.0f + 12.0f, min.y - max.y + 8.0f, "white", color, 1.0f);
			break;
		case BOX_TYPE_2DFILLED: draw::rect(vpjCenter.x - flWidth - 6.0f, max.y - 4.0f, flWidth * 2.0f + 12.0f, min.y - max.y + 8.0f, "white", fillColor, true, color, 1.0f);
			break;
		}
	}
}

void ESP::drawBones(centity_s *entity, float *color)
{
	for (int i = 0; i < ARRAYSIZE(Bones) - 1; i++)
	{
		vector2 Screen = vector2(), Screen2 = vector2();
		vector3 BonePos1 = vector3(), BonePos2 = vector3();

		if (func::getTagPos(entity->clientNumber, GScr_AllocString(Bones[i]), BonePos1) && func::getTagPos(entity->clientNumber, GScr_AllocString(Bones[i + 1]), BonePos2))
		{
			if (func::worldToScreen(BonePos1, Screen) && func::worldToScreen(BonePos2, Screen2))
				draw::line(Screen.x, Screen.y, Screen2.x, Screen2.y, color, 1);
		}
	}
}

void ESP::RotatePoint2(vector3 point, vector3 center, vector3 angles, vector3 &out)
{
	float zTheta = D3DXToRadian(angles.y);
	float yTheta = D3DXToRadian(angles.x);
	float xTheta = D3DXToRadian(angles.z);
	vector3 rotatedPoint = vector3();
	vector3 dir = point - center;
	rotatedPoint.x = point.x;
	rotatedPoint.y = center.y + (dir.y * cos(xTheta) - dir.z * sin(xTheta));
	rotatedPoint.z = center.z + (dir.y * sin(xTheta) + dir.z * cos(xTheta));
	dir = rotatedPoint - center;
	rotatedPoint.x = center.x + (dir.x * cos(yTheta) + dir.z * sin(yTheta));
	rotatedPoint.z = center.z + (dir.z * cos(yTheta) - dir.x * sin(yTheta));
	dir = rotatedPoint - center;
	out.x = center.x + (dir.x * cos(zTheta) - dir.y * sin(zTheta));
	out.y = center.y + (dir.x * sin(zTheta) + dir.y * cos(zTheta));
	out.z = rotatedPoint.z;
}

void ESP::makeEspLine(vector3 Center, float X1, float Y1, float Z1, float X2, float Y2, float Z2, float *Color, float Width, vector3 angle)
{
	vector2 XY1, XY2;
	vector3 finalOut[2] = { vector3(Center.x + X1, Center.y + Y1, Center.z + Z1), vector3(Center.x + X2, Center.y + Y2, Center.z + Z2) };
	vector3 out[2] = { vector3() };
	RotatePoint2(finalOut[0], Center, angle, out[0]);
	RotatePoint2(finalOut[1], Center, angle, out[1]);

	if (func::worldToScreen(out[0], XY1) &&
		func::worldToScreen(out[1], XY2))
		draw::line(XY1.x, XY1.y, XY2.x, XY2.y, Color, Width);
}

void ESP::make3DBox(vector3 Center, float W, float H, float *Color, vector3 angle)
{
	// Bottom
	makeEspLine(Center, -W, -W, 0.0f, W, -W, 0.0f, Color, 1.0f, angle);
	makeEspLine(Center, -W, -W, 0.0f, -W, W, 0.0f, Color, 1.0f, angle);
	makeEspLine(Center, W, W, 0.0f, W, -W, 0.0f, Color, 1.0f, angle);
	makeEspLine(Center, W, W, 0.0f, -W, W, 0.0f, Color, 1.0f, angle);

	// Corners
	makeEspLine(Center, -W, -W, 0.0f, -W, -W, H, Color, 1.0f, angle);
	makeEspLine(Center, -W, W, 0.0f, -W, W, H, Color, 1.0f, angle);
	makeEspLine(Center, W, -W, 0.0f, W, -W, H, Color, 1.0f, angle);
	makeEspLine(Center, W, W, 0.0f, W, W, H, Color, 1.0f, angle);

	// Top
	makeEspLine(Center, -W, -W, H, W, -W, H, Color, 1.0f, angle);
	makeEspLine(Center, -W, -W, H, -W, W, H, Color, 1.0f, angle);
	makeEspLine(Center, W, W, H, W, -W, H, Color, 1.0f, angle);
	makeEspLine(Center, W, W, H, -W, W, H, Color, 1.0f, angle);
}

void ESP::drawBoundingBox(int c, float *color)
{
	if (!Entities[c].isAlive())
		return;

	vector3 min1 = vector3(), max1 = vector3(), absMin = vector3(), absMax = vector3();

	vector3 helmetCoord = vector3();

	if (c < 18)
	{
		if (!func::getTagPos(c, vars->cachedBones[j_helmet], helmetCoord))
			return;

		min1 = vector3(-10.0f, -10.0f, 0.0f);
		max1 = vector3(10.0f, 10.0f, (helmetCoord.z - Entities[c].Origin.z));
	}
	else if (c > 18)
	{
		int dobj = Com_GetClientDObj(c, 0);
		if (!dobj)
			return;

#if defined (MW2)
		Bounds bounds, absBounds;
		auto DObjPhysicsGetBounds = (void(*)(int DObj, Bounds *bounds))0x822AD570;
		DObjPhysicsGetBounds(dobj, &bounds);
		min1 = bounds.midPoint;
		max1 = bounds.halfSize;
		max1 += Entities[c].Origin;
		min1 += Entities[c].Origin - (max1 - Entities[c].Origin);
#else
		auto DObjPhysicsGetBounds = (void(*)(int dobj, vector3 &mins, vector3 &maxs))addr->DObjPhysicsGetBounds_;
		DObjPhysicsGetBounds(dobj, min1, max1);
#endif
	}

	min1 += Entities[c].Origin;
	max1 += Entities[c].Origin;

	vector3 center = Entities[c].Origin;

	vector3 crnr[] =
	{
		vector3(max1.x, min1.y, min1.z),
		vector3(max1.x, min1.y, max1.z),
		vector3(min1.x, min1.y, max1.z),
		vector3(min1.x, max1.y, max1.z),
		vector3(min1.x, max1.y, min1.z),
		vector3(max1.x, max1.y, min1.z)
	};

	vector3 angle = func::getClientViewAngles(c);

	for (int i = 0; i < 6; i++)
		RotatePoint2(crnr[i], center, angle, crnr[i]);

	RotatePoint2(min1, center, angle, min1);
	RotatePoint2(max1, center, angle, max1);

	vector2 min, max, crnr2, crnr3, crnr4, crnr5, crnr6, crnr7;

	if (func::worldToScreen(min1, min)
		&& func::worldToScreen(max1, max)
		&& func::worldToScreen(crnr[0], crnr2)
		&& func::worldToScreen(crnr[1], crnr3)
		&& func::worldToScreen(crnr[2], crnr4)
		&& func::worldToScreen(crnr[3], crnr5)
		&& func::worldToScreen(crnr[4], crnr6)
		&& func::worldToScreen(crnr[5], crnr7)) {

		// From min to 2, 4 and 6 
		draw::line(min.x, min.y, crnr2.x, crnr2.y, color, 1.0f);
		draw::line(min.x, min.y, crnr4.x, crnr4.y, color, 1.0f);
		draw::line(min.x, min.y, crnr6.x, crnr6.y, color, 1.0f);

		// From max to 5, 7 and 3 
		draw::line(max.x, max.y, crnr5.x, crnr5.y, color, 1.0f);
		draw::line(max.x, max.y, crnr7.x, crnr7.y, color, 1.0f);
		draw::line(max.x, max.y, crnr3.x, crnr3.y, color, 1.0f);

		// From 2 to 7 and 3 
		draw::line(crnr2.x, crnr2.y, crnr7.x, crnr7.y, color, 1.0f);
		draw::line(crnr2.x, crnr2.y, crnr3.x, crnr3.y, color, 1.0f);

		// From 4 to 5 and 3 
		draw::line(crnr4.x, crnr4.y, crnr5.x, crnr5.y, color, 1.0f);
		draw::line(crnr4.x, crnr4.y, crnr3.x, crnr3.y, color, 1.0f);

		// From 6 to 5 and 7 
		draw::line(crnr6.x, crnr6.y, crnr5.x, crnr5.y, color, 1.0f);
		draw::line(crnr6.x, crnr6.y, crnr7.x, crnr7.y, color, 1.0f);
	}
}

void ESP::worldPosToCompassPos(rectDef_s* radar, vector3 world, vector2 *compass)
{
	float pixPerInch = radar->w / *(float*)(*(int*)(0x886F3C) + 0xC);
	float ViewAngle = *(float*)(0x90F5AC) / 180.0f * XM_PI;

	vector2 Difference(pixPerInch * (cg->RefDef.viewOrigin.x - world.x),
		pixPerInch * (cg->RefDef.viewOrigin.y - world.y));

	vector2 Screen(radar->x + (Difference.y * cosf(ViewAngle) - Difference.x * sinf(ViewAngle)),
		radar->y + (Difference.x * cosf(ViewAngle) + Difference.y * sinf(ViewAngle)));

	if (Screen.x < radar->x - (radar->w / 2)) Screen.x = radar->x - (radar->w / 2);
	if (Screen.x > radar->x + (radar->w / 2)) Screen.x = radar->x + (radar->w / 2);
	if (Screen.y < radar->y - (radar->h / 2)) Screen.y = radar->y - (radar->h / 2);
	if (Screen.y > radar->y + (radar->h / 2)) Screen.y = radar->y + (radar->h / 2);

	compass->x = Screen.x + (radar->w / 2);
	compass->y = Screen.y + (radar->h / 2);
}

int CG_GetLocalClientWeaponInfo(int weapon)
{
#if defined(BO2)
	return *(int*)(0x82B1D5B8) + (weapon & 0xff) * 0x24;
#elif defined (BO1)
	return *(int*)(0x828CE9CC) + weapon * 0x24;
#elif defined (MW2)
	return *(int*)(0x892440) + weapon * 0x20;
#endif
}

void ESP::drawEntities()
{
	if (func::inGame())
	{
		for (int i = 18; i < 1024; i++)
		{
			if (Entities[i].isAlive())
			{
				if (Entities[i].eType == 4 && esp->explosiveEsp)
				{
					vector2 screen1 = vector2();
					drawBoundingBox(i, mint);
					vector2 screen = vector2();
					if (func::worldToScreen(Entities[i].Origin, screen))
					{
						char buffer[64] = { 0 };
						sprintf_s(buffer, "%s", (char*)(*(int*)(CG_GetLocalClientWeaponInfo(Entities[i].WeaponID) + 0x18)));
						draw::text(buffer, screen.x, screen.y, menu.returnFont(), 0.6f, white, 'C', 0.0f, 6);
					}
				}

				int shader = func::getWeaponShader(Entities[i].groundWeaponId());
				vector2 screen2 = vector2();
				if (shader != -1 && func::worldToScreen(Entities[i].Origin, screen2) && esp->droppedWeaponEsp)
				{
					char buffer[64] = { 0 };
					sprintf_s(buffer, "%s", (char*)(*(int*)(CG_GetLocalClientWeaponInfo(Entities[i].groundWeaponId()) + 0x18)));
					draw::text(buffer, screen2.x, screen2.y, menu.returnFont(), 0.6f, white, 'C', 0.0f, 6);
					drawBoundingBox(i, yellow);
				}
			}
		}
	}
}

void ESP::drawESP()
{
	if (func::inGame())
	{
		for (int i = 0; i < 18; i++)
		{
			if (Entities[i].isAlive() && cg->clientNumber != i)
			{
				float *friendlyColor = friendColor;
				float *enemyColor = enemColor;
				if (aimbot->prioritized[i])
					enemyColor == targetedClient;
				else if (aimbot->whitelisted[i])
					enemyColor = ignoredColor;

				if (aimbot->hittable[i])
					enemyColor = hittableColor;
				else if (aimbot->visible[i])
					enemyColor = visibleColor;

				vector3 helmetCoord = vector3();
				vector2 originLoc = vector2(), helmetLoc = vector2();

				if (func::worldToScreen(Entities[i].Origin, originLoc) && func::getTagPos(i, vars->cachedBones[j_helmet], helmetCoord) && func::worldToScreen(helmetCoord, helmetLoc))
				{
					if (func::notTeam(i))
					{
						if (esp->enemy)
						{
							if (esp->bones)
								drawBones(&Entities[i], enemyColor);

							switch (esp->boxType)
							{
							case BOX_TYPE_3D:
								//make3DBox(Entities[i].Origin, 13.0f, (helmetCoord.z - Entities[i].Origin.z), enemyColor, func::getClientViewAngles(i).y);
								drawBoundingBox(i, enemyColor);
								break;
							case BOX_TYPE_2D:
								make2DBox(i, enemyColor, BOX_TYPE_2D);
								break;
							case BOX_TYPE_2DFILLED:
								make2DBox(i, enemyColor, BOX_TYPE_2DFILLED);
								break;
							case BOX_TYPE_CORNER:
								make2DBox(i, enemyColor, BOX_TYPE_CORNER);
								break;
							}

							if (esp->names)
							{
								char buffer[64] = { 0 };
								sprintf_s(buffer, "%s [%.0fm]", cg->clientInfo_t[i].clientName, Entities[i].Origin.distance(Entities[cg->clientNumber].Origin) * 0.0254);
								draw::text(buffer, originLoc.x, helmetLoc.y - 25.0f, menu.returnFont(), 0.6f, white, 'C', 0.0f, 6);
							}

							switch (esp->snaplines)
							{
							case SNAP_TYPE_TOP:
								draw::line(vars->width / 2.0f, 0.0f, originLoc.x, originLoc.y, enemyColor, 1.0f);
								break;
							case SNAP_TYPE_CENTER:
								draw::line(vars->width / 2.0f, vars->height / 2.0f, originLoc.x, originLoc.y, enemyColor, 1.0f);
								break;
							case SNAP_TYPE_BOTTOM:
								draw::line(vars->width / 2.0f, vars->height, originLoc.x, originLoc.y, enemyColor, 1.0f);
								break;
							}

							if (esp->weaponIcons)
							{
								int shader = func::getWeaponShader(Entities[i].WeaponID);
								float iW = 35.0f, iH = 25.0f;
								if (shader != -1)
									draw::rect(originLoc.x - (iW / 2.0f), originLoc.y + 5.0f, iW, iH, (char*)shader, enemyColor);
							}
						}
					}
					else
					{
						if (esp->friendly)
						{
							if (esp->bones)
								drawBones(&Entities[i], friendlyColor);

							switch (esp->boxType)
							{
							case BOX_TYPE_3D:
								//make3DBox(Entities[i].Origin, 13.0f, (helmetCoord.z - Entities[i].Origin.z), friendlyColor, func::getClientViewAngles(i).y);
								drawBoundingBox(i, friendlyColor);
								break;
							case BOX_TYPE_2D:
								make2DBox(i, friendlyColor, BOX_TYPE_2D);
								break;
							case BOX_TYPE_2DFILLED:
								make2DBox(i, friendlyColor, BOX_TYPE_2DFILLED);
								break;
							case BOX_TYPE_CORNER:
								make2DBox(i, friendlyColor, BOX_TYPE_CORNER);
								break;
							}

							if (esp->names)
							{
								char buffer[64] = { 0 };
								sprintf_s(buffer, "%s [%.0fm]", cg->clientInfo_t[i].clientName, Entities[i].Origin.distance(Entities[cg->clientNumber].Origin) * 0.0254);
								draw::text(buffer, originLoc.x, helmetLoc.y - 25.0f, menu.returnFont(), 0.6f, white, 'C', 0.0f, 6);
							}

							switch (esp->snaplines)
							{
							case SNAP_TYPE_TOP:
								draw::line(vars->width / 2.0f, 0.0f, originLoc.x, originLoc.y, friendlyColor, 1.0f);
								break;
							case SNAP_TYPE_CENTER:
								draw::line(vars->width / 2.0f, vars->height / 2.0f, originLoc.x, originLoc.y, friendlyColor, 1.0f);
								break;
							case SNAP_TYPE_BOTTOM:
								draw::line(vars->width / 2.0f, vars->height, originLoc.x, originLoc.y, friendlyColor, 1.0f);
								break;
							}

							if (esp->weaponIcons)
							{
								int shader = func::getWeaponShader(Entities[i].WeaponID);
								float iW = 35.0f, iH = 25.0f;
								if (shader != -1)
									draw::rect(originLoc.x - (iW / 2.0f), originLoc.y + 5.0f, iW, iH, (char*)shader, friendlyColor);
							}
						}
					}
				}
			}
		}
		drawEntities();
	}
}