#include "main.h"

float maxUp = 69.8889f;
float maxDown = -69.8889f;

bool flipX = false, flipY = false;
float xAmount = 0, yAmount = 0, zAmount = 0;

void spinbot::spinX(usercmd_s *cmd, int speed)
{
	switch (aimbot->spinbotX)
	{
	case SPINBOT_UP:
		xAmount = maxDown;
		break;
	case SPINBOT_DOWN:
		xAmount = maxUp;
		break;
	case SPINBOT_UPDOWN:
		if (xAmount < maxUp && !flipX)
		{
			xAmount += (float)speed;
			if (xAmount >= maxUp)
				flipX = true;
		}
		else if (xAmount >= maxDown && flipX)
		{
			xAmount -= (float)speed;
			if (xAmount <= maxDown)
				flipX = false;
		}
		break;
	}

	if (xAmount >= maxUp)
		xAmount = maxUp - 0.5f;
	else if (xAmount <= maxDown)
		xAmount = maxDown + 0.5f;

	if (aimbot->spinbotX == SPINBOT_UP)
		cmd->ViewAngles[0] = AngleToShort(maxDown - ca->spawnAngles.x);

	if (aimbot->spinbotX == SPINBOT_DOWN)
		cmd->ViewAngles[0] = AngleToShort(maxUp - ca->spawnAngles.x);
}

void spinbot::spinY(usercmd_s *cmd, int speed, int closestClient)
{
	int client = 18;
	vector3 bonePos = vector3();

	switch (aimbot->spinbotY)
	{
	case SPINBOT_Y_LEFT:
		yAmount += (float)speed;
		break;
	case SPINBOT_Y_RIGHT:
		yAmount -= (float)speed;
		break;
	case SPINBOT_Y_JITTER:

		flipY = !flipY;
		if (!flipY)
			yAmount = 180.0f;
		else if (flipY)
			yAmount = -180.0f;

		break;
	case SPINBOT_Y_ANTIAIM:
		vector3 bonePos = vector3();
		client = vars->clientToFace == -1 ? aimbot->closestClient : vars->clientToFace;
		if (func::getTagPos(client, vars->cachedBones[j_head], bonePos))
		{
			vector3 vector = VectorToAngles(bonePos - func::getViewOrigin());
			yAmount = vector.y + 180.0f;
		}
		break;
	}

	if (aimbot->spinbotY != SPINBOT_Y_ANTIAIM)
	{
		if (yAmount >= 360.0f)
			yAmount = 0.1f;
		else if (yAmount <= 0.0f)
			yAmount = 359.0f;
	}

	cmd->ViewAngles[1] = AngleToShort(yAmount - ca->spawnAngles.y);
}

float spinbot::negate_number(float f)
{
	union { unsigned x; float y; } u;
	u.y = f;
	u.x ^= 0x80000000;
	return u.y;
}

void spinbot::spinZ(usercmd_s *cmd, int speed)
{
#if defined (BO1)
	if (aimbot->isAutoShooting)
	{
		cmd->ViewAngles[2] = AngleToShort(0.0f - ca->spawnAngles.z);
		return;
	}
#endif
	ca->viewAngles.z = negate_number(ca->spawnAngles.z);

	switch (aimbot->spinbotZ)
	{
	case SPINBOT_Z_LEFT:
		zAmount -= (float)speed;
		break;
	case SPINBOT_Z_RIGHT:
		zAmount += (float)speed;
		break;
	case SPINBOT_Z_RANDOM:

		flipY = !flipY;
		if (!flipY)
			zAmount = 180.0f;
		else if (flipY)
			zAmount = -180.0f;

		break;
	}

	if (zAmount >= 360.0f)
		zAmount = 0.1f;
	else if (zAmount <= 0.0f)
		zAmount = 359.0f;

	cmd->ViewAngles[2] = AngleToShort(zAmount - ca->spawnAngles.z);
}

void spinbot::handleSpinbot(usercmd_s *cmd, int closestClient)
{
	if (tools::keyIsDown(XINPUT_GAMEPAD_RIGHT_SHOULDER) || tools::keyIsDown(XINPUT_GAMEPAD_LEFT_SHOULDER))
		return;

	static int timeToWait = 0;
	if (cg->Health == 0)
		timeToWait = 10;

	if (cg->Health > 0)
	{
		if (timeToWait > 0)
			timeToWait--;

		if (timeToWait == 0)
		{
			if (aimbot->spinbotX != 0)
				spinX(cmd, aimbot->xSpeed);

			if (aimbot->spinbotY != 0)
				spinY(cmd, aimbot->ySpeed, closestClient);;

			if (aimbot->spinbotZ != 0)
				spinZ(cmd, aimbot->zSpeed);
		}
	}
}