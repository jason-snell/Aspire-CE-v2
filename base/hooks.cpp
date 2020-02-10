#include "main.h"

int hooks::XamInputGetStateHook(int dwUserIndex, int r4, PXINPUT_STATE pState)
{
	int result = XamInputGetStateDetour->CallOriginal(dwUserIndex, r4, pState);

	if (menu.menuOpen())
	{
		pState->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_UP;
		pState->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_DOWN;
		pState->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_LEFT;
		pState->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_RIGHT;
		pState->Gamepad.wButtons &= ~XINPUT_GAMEPAD_A;
		pState->Gamepad.wButtons &= ~XINPUT_GAMEPAD_B;
		pState->Gamepad.wButtons &= XINPUT_GAMEPAD_RIGHT_SHOULDER;
		pState->Gamepad.wButtons &= XINPUT_GAMEPAD_LEFT_SHOULDER;
	}
	else if (vars->editSafeArea)
	{
		pState->Gamepad.wButtons &= ~XINPUT_GAMEPAD_A;
		pState->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_UP;
		pState->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_DOWN;
		pState->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_LEFT;
		pState->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_RIGHT;
		pState->Gamepad.wButtons &= ~XINPUT_GAMEPAD_B;
	}

	return result;
}

void hooks::R_EndFrameHook()
{
	if (func::inGame())
	{
		if(aimbot->aimbotOn)
			aimbot->closestClient = ragebot::getClosestClient();
		ESP::drawESP();
	}

	menu.buttonMonitoring();

	draw::visuals();

	if (vars->menuOpen)
		Menu::drawMenu();

	static bool once = false;
	static int noSwayBytes = 0, noRecoilBytes = 0;
	if (!once)
	{
		once = true;
		noSwayBytes = *(int*)addr->noSway;
		noRecoilBytes = *(int*)addr->noRecoil;
	}

	if(addr->noSway != 0)
		*(int*)addr->noSway = aimbot->noSway ? 0x60000000 : noSwayBytes;
	if(addr->noRecoil != 0)
		*(int*)addr->noRecoil = aimbot->noRecoil ? 0x60000000 : noRecoilBytes;

	if(func::inGame() && addr->FOV != 0)
		*(float*)(*(int*)(addr->FOV) + addr->dvarAdditive) = vars->FOV;

	for (int i = 0; i < 3; i++)
	{
		if (vars->primaryColorRainbow)
			darkMenuColor[i] = func::callRainbow(vars->pimaryRainbowSpeed, 0)[i];

		if (vars->secondaryColorRainbow)
			secondaryColor[i] = func::callRainbow(vars->secondaryRainbowSpeed, 1)[i];

		if (vars->backgroundColorRainbow)
			lightMenuColor[i] = func::callRainbow(vars->backgroundRainbowSpeed, 2)[i];
	}

	R_EndFrameDetour->CallOriginal();
}

void hooks::CL_WritePacketHook(int localClientNum)
{
	if (func::inGame() && cg->Health > 0)
	{
		usercmd_s *cmd = ca->GetCmd(ca->cmdNumber + 1);
		usercmd_s *oldCmd = ca->GetCmd(ca->cmdNumber);

		if (aimbot->aimbotOn || aimbot->spinbotOn)
		{
			ca->cmdNumber++;
			*cmd = *oldCmd;
			cmd->serverTime += 1;

			if (aimbot->aimbotOn)
#if defined (MW2)
				ragebot::handleRagebot(oldCmd, oldCmd);
#else
				ragebot::handleRagebot(cmd, oldCmd);
#endif

			if (aimbot->spinbotOn)
				spinbot::handleSpinbot(cmd, aimbot->closestClient);
		}
	}

	CL_WritePacketDetour->CallOriginal(localClientNum);
	aimbot->isAutoShooting = false;
}

#if defined (BO2)
void hooks::BG_AdvanceTraceHook(BulletFireParams *bp, BulletTraceResults *br, float dist)
{
	vars->partGroup = br->trace.partGroup;
	BG_AdvanceTraceDetour->CallOriginal(bp, br, dist);
}

int hooks::preGameAntiFreezeHook(int r3, int r4, int r5, int r6, int r7, int r8)
{
	int newint = 0;
	if (r8 == 0)
		return preGameAntiFreezeDetour->CallOriginal(r3, r4, r5, r6, r7, &newint);

	return preGameAntiFreezeDetour->CallOriginal(r3, r4, r5, r6, r7, r8);
}

void hooks::DynEntCl_DestroyEventHook(int r3, short r4, int r5, vector3 &hitPos, vector3 &hitDir)
{
	if (func::inGame())
	{
		int** someShitBoi = (int**)0x83798218;
		if (!someShitBoi[r5] || !someShitBoi[r5][r4])
			return;
	}

}

int hooks::xmemcpyHook(void *Dst, const void *Source, int Size)
{
	DWORD rAddr;
	__asm mflr rAddr
	if (rAddr == 0x82717C8C && !func::inGame())
		return;


	if (Size >= INT_MAX || Size <= 0)
		return;

	return xmemcpyDetour->CallOriginal(Dst, Source, Size);
}

int hooks::NetDll_XNetGetTitleXnAddrHook(int xnc, XNADDR *xna)
{
	int result = NetDll_XNetGetTitleXnAddrDetour->CallOriginal(xnc, xna);

	if (!vars->spoofIP)
		return result;

	unsigned char IP[] = { 1, 3, 3, 7 };
	xna->inaOnline.S_un.S_addr = *(int*)&IP[0];
	xna->wPortOnline = 1337;

	return result;
}
#endif

void hooks::R_AddCmdDrawStretchPicHook(float x, float y, float w, float h, float s0, float t0, float s1, float t1, const float *color, void *material)
{
	char *mat = (char*)(*(int*)material);
	if (strstr(mat, "scav"))
		printf("material: %s | w: %f | h: %f\n", mat, w, h);
	R_AddCmdDrawStretchPicDetour->CallOriginal(x, y, w, h, s0, t0, s1, t1, color, material);
}