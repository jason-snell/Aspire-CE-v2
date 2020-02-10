#pragma once
class ragebot
{
public:
	static bool clientHasShield(int i);
	static float BG_random(int *pHoldrand);
	static bool canBypassShield(int clientNum, short tagId);
	static void BG_seedRandWithGameTime(int *pHoldrand);
	static bool canKill(int index, short tag);
	static bool Visible(int i, short bone);
	static void BG_srand(int *pHoldrand);
	static void RandomBulletDir(int randSeed, float *x, float *y);
	static int getClosestClient();
	static void handleSpread(usercmd_s *cmd, usercmd_s *newCmd);
	static void movementFix(usercmd_s *cmd, int cc);
	static void handleRagebot(usercmd_s *currentCmd, usercmd_s *oldCmd);
};

