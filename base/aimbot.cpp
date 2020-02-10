#include "main.h"

bool ragebot::clientHasShield(int i)
{
#if defined (BO2)
	if (Entities[i].WeaponID2 == BG_GetWeaponIndexForName("riotshield_mp"))
		return true;

	if (Entities[i].WeaponID == BG_GetWeaponIndexForName("riotshield_mp"))
		return true;

	return false;
#else
	return false;
#endif
}

float ragebot::BG_random(int *pHoldrand)
{
#if defined (BO2)
	unsigned int v1;
	double v2;
	float result;

	v1 = 214013 * *pHoldrand + 2531011;
	*pHoldrand = v1;
	v2 = (double)(v1 >> 17) * 0.000030517578;
	result = v2;
	return result;
#else
	auto CG_GoodRandomFloat = (float(*)(int *idum))addr->CG_GoodRandomFloat_;
	return CG_GoodRandomFloat(pHoldrand);
#endif;
}

bool ragebot::canBypassShield(int clientNum, short tagId)
{
	if (!clientHasShield(clientNum))
		return true;

	vector3 pos;

	if (!func::getTagPos(clientNum, tagId, pos))
		return false;

	return (vars->partGroup != 0x14);
}

void ragebot::BG_seedRandWithGameTime(int *pHoldrand)
{
	int v1;

	v1 = *(char*)((int)pHoldrand + 3) << 8;
	*pHoldrand ^= v1;
	*pHoldrand ^= (v1 << 8) ^ (v1 << 16);
}

bool ragebot::canKill(int index, short tag)
{
	vector3 TagPos = vector3(), Angles = vector3(), dir = vector3(), Hax[3] = { vector3() }, viewPos = vector3(), Start = vector3();

	if (!func::getTagPos(index, tag, TagPos))
		return false;

	viewPos = func::getViewOrigin();

	BulletFireParams bulletFireParams = { 0 };
	char bulletTraceResults[200] = { 0 };
	bulletFireParams.ignoreEntIndex = cg->clientNumber;
	bulletFireParams.weaponEntIndex = 1022;
	bulletFireParams.damageMultiplier = 1.0f;
	bulletFireParams.methodOfDeath = 2;
	bulletFireParams.origStart = viewPos;
	bulletFireParams.start = viewPos;
	bulletFireParams.end = TagPos;
	dir = TagPos - viewPos;
	Angles = VectorToAngles(dir);

	AngleVectors(Angles, &Hax[0], NULL, NULL);
	bulletFireParams.dir = Hax[0];

#if defined(BO2)
	auto FireBulletPenetrate = (void(*)(int, int *, int, int *, vector3 &, bool, bool, int))addr->FireBulletPenetrate_;
	FireBulletPenetrate(0, (int*)&bulletFireParams, Entities[cg->clientNumber].WeaponID, (int*)&Entities[cg->clientNumber], viewPos, 0, 1, (int)bulletTraceResults);
#elif defined (BO1)
	auto FireBulletPenetrate = (void(*)(int, BulletFireParams *, int, centity_s *, vector3 &, bool, bool, int, bool))addr->FireBulletPenetrate_;
	FireBulletPenetrate(0, &bulletFireParams, Entities[cg->clientNumber].WeaponID, &Entities[cg->clientNumber], viewPos, 0, 1, (int)bulletTraceResults, 0);
#elif defined (MW2)
	auto FireBulletPenetrate = (void(*)(int, BulletFireParams *, int, centity_s *, int, vector3 &, int))addr->FireBulletPenetrate_;
	FireBulletPenetrate(0, &bulletFireParams, Entities[cg->clientNumber].WeaponID, &Entities[cg->clientNumber], -1, viewPos, 0);
#endif

	return (bulletFireParams.ignoreEntIndex == index);
}

bool ragebot::Visible(int i, short bone)
{
	vector3 bonePos = vector3();
	if (!func::getTagPos(i, bone, bonePos))
		return false;

#if defined (BO2)
	auto CG_CanSeeFriendlyHeadTrace = (bool(*)(int, centity_s *, vector3 &, vector3 &))addr->CG_CanSeeFriendlyHeadTrace_;
	return CG_CanSeeFriendlyHeadTrace(0, &Entities[i], func::getViewOrigin(), bonePos);
#elif defined (BO1)
	auto CG_CanSeeFriendlyHeadTrace = (bool(*)(int, centity_s *, vector3 &, vector3 &))addr->CG_CanSeeFriendlyHeadTrace_;
	return CG_CanSeeFriendlyHeadTrace(0, &Entities[i], func::getViewOrigin(), bonePos);
#elif defined (MW2)
	trace_t trace = { 0 };
	auto CG_LocationalTrace = (void(*)(void *results, vector3 &start, vector3 &end, int passEntityNum, int contentMask))0x82136E88;
	CG_LocationalTrace(&trace, func::getViewOrigin(), bonePos, cg->clientNumber, 0x280B001);
	return trace.fraction >= 0.95f;
#endif
}

void __declspec(noinline) ragebot::BG_srand(int *pHoldrand)
{
	int r10 = 0x2200;
	int r11 = *(int*)pHoldrand;
	int r9 = 0x985;
	r10 |= 0xAFAF;
	r9 |= 0x20C4;
	r11 *= r10;
	r11 -= r9;
	*(int*)pHoldrand = r11;
	//*pHoldrand = 214013 * (214013 * (214013 * (214013 * *pHoldrand + 2531011) + 2531011) + 2531011) + 2531011;
}

void ragebot::RandomBulletDir(int randSeed, float *x, float *y)
{
#if defined (BO2)
	BG_seedRandWithGameTime(&randSeed);
#endif
	float theta = (BG_random(&randSeed) * XM_2PI);
#if defined (BO2)
	BG_srand(&randSeed);
#endif
	float radius = BG_random(&randSeed);
	*x = radius * cos(theta);
	*y = radius * sin(theta);
}

int ragebot::getClosestClient()
{
	int closest = -1;
	float currentDistance = FLT_MAX;

	for (int i = 0; i < 18; i++)
	{
		aimbot->visible[i] = false;
		aimbot->hittable[i] = false;
		aimbot->bestBone[i] = 0;

		if (aimbot->whitelisted[i] || i == cg->clientNumber || !Entities[i].isAlive() || !func::notTeam(i) || cg->Health <= 0)
			continue;

		if (clientHasShield(i) || aimbot->prioritized[i])
		{
			int bone = -1;
			for (int c = 0; c < ARRAYSIZE(boneNames); c++)
			{
				bone = vars->cachedBones[c];

				if (canKill(i, bone) && canBypassShield(i, bone))
				{
					if (aimbot->targetType == TARGET_TYPE_HITTABLE)
					{
						aimbot->hittable[i] = true;
						aimbot->bestBone[i] = bone;
						float distance = Entities[i].Origin.distance(Entities[cg->clientNumber].Origin);
						if (distance < currentDistance)
						{
							closest = i;
							currentDistance = distance;
						}
						break;
					}
				}

				if (Visible(i, bone) && canBypassShield(i, bone))
				{
					if (aimbot->targetType == TARGET_TYPE_VISIBLE)
					{
						aimbot->visible[i] = true;
						aimbot->bestBone[i] = bone;
						float distance = Entities[i].Origin.distance(Entities[cg->clientNumber].Origin);
						if (distance < currentDistance)
						{
							closest = i;
							currentDistance = distance;
						}
						break;
					}
				}
			}
		}
		else
		{
			int bone = vars->cachedBones[j_head];
			switch (aimbot->targetType)
			{
			case TARGET_TYPE_VISIBLE:
				if (Visible(i, bone))
				{
					aimbot->visible[i] = true;
					aimbot->bestBone[i] = bone;

					float distance = Entities[i].Origin.distance(Entities[cg->clientNumber].Origin);
					if (distance < currentDistance)
					{
						closest = i;
						currentDistance = distance;
					}

					break;
				}
				break;
			case TARGET_TYPE_HITTABLE:
				if (canKill(i, bone))
				{
					aimbot->hittable[i] = true;
					aimbot->bestBone[i] = bone;

					float distance = Entities[i].Origin.distance(Entities[cg->clientNumber].Origin);
					if (distance < currentDistance)
					{
						closest = i;
						currentDistance = distance;
					}

					break;
				}
				break;
			}
		}
	}

	return closest;
}

void ragebot::handleSpread(usercmd_s *cmd, usercmd_s *oldCmd)
{
#if defined (BO2)
	float spreadMultiplier = *(float*)((int)cg + 0x80B80);
#elif defined (BO1)
	float spreadMultiplier = *(float*)((int)cg + 0x69264);
#else
	float spreadMultiplier = cg->aimSpreadScale;
#endif

	float min = 0.0f, max = 0.0f;
	BG_GetSpreadForWeapon(func::playerState(), Entities[cg->clientNumber].WeaponID, &min, &max);

	float Spread = min + ((max - min) * spreadMultiplier * 0.0039215f);

	vector2 spread = vector2();
#if defined(MW2)
	int serverTime = cmd->serverTime;
	BG_srand(&serverTime);
	RandomBulletDir(serverTime, &spread.x, &spread.y);
#else
	RandomBulletDir(cmd->serverTime, &spread.x, &spread.y);
#endif

	oldCmd->ViewAngles[0] += AngleToShort(spread.y * Spread);
	oldCmd->ViewAngles[1] += AngleToShort(spread.x * Spread);
}

void ragebot::movementFix(usercmd_s *cmd, int cc)
{
	vector3 Vector = VectorToAngles(Entities[cc].Origin - func::getViewOrigin());
	vector3 aimAngles = Vector - ca->spawnAngles;

	float differenceY = (aimAngles.y - ca->viewAngles.y);
	int forward = cos(differenceY * 0.017453292f) * cmd->MoveButtonX + cos((differenceY + 90.0f) * 0.017453292f) * cmd->MoveButtonY;
	int right = sin(differenceY * 0.017453292f) * cmd->MoveButtonX + sin((differenceY + 90.0f) * 0.017453292f) * cmd->MoveButtonY;
	cmd->MoveButtonX = func::clampChar(forward);
	cmd->MoveButtonY = func::clampChar(right);
}

void ragebot::handleRagebot(usercmd_s *currentCmd, usercmd_s *oldCmd)
{
	vector3 bonePos = vector3();
	vector2 boneLoc = vector2();

	if (aimbot->closestClient != -1 &&
		func::getTagPos(aimbot->closestClient, aimbot->bestBone[aimbot->closestClient], bonePos))
	{
		vector3 aimAngles = VectorToAngles(bonePos - func::getViewOrigin()) - ca->spawnAngles;

		if (aimbot->aimbotType == AIMBOT_TYPE_NORMAL)
		{
			oldCmd->ViewAngles[0] = AngleToShort(aimAngles.x);
			oldCmd->ViewAngles[1] = AngleToShort(aimAngles.y);
			ca->viewAngles = aimAngles;
		}
		else if (aimbot->aimbotType == AIMBOT_TYPE_SILENT)
		{
			oldCmd->ViewAngles[0] = AngleToShort(aimAngles.x);
			oldCmd->ViewAngles[1] = AngleToShort(aimAngles.y);
			movementFix(oldCmd, aimbot->closestClient);
		}

		if (aimbot->noSpread)
#if defined (MW2)
			handleSpread(oldCmd, oldCmd);
#else
			handleSpread(currentCmd, oldCmd);
#endif

		if (aimbot->autoShoot)
		{
			aimbot->isAutoShooting = true;
#if defined (BO2)
			currentCmd->buttons |= 0x80000000;
#elif defined (MW2)
			currentCmd->buttons |= 1;
#else
			currentCmd->buttons |= 0x80000000;
#endif
		}
	}
}