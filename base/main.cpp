#include "main.h"

HANDLE cheatHandle = 0;
PLDR_DATA_TABLE_ENTRY aspireHandle = 0;

void takeDownDetours()
{
	tools::addToList(cheatHandle);
	vars->isUnloading = true;

	int unloadImageAddr = tools::resolveFunction("xboxkrnl.exe", 418);
	void(*XexUnloadImageAndExitThread)(HANDLE moduleHandle, HANDLE threadHandle) = (void(*)(HANDLE, HANDLE))unloadImageAddr;

	R_EndFrameDetour->TakeDownDetour();
	XamInputGetStateDetour->TakeDownDetour();
	CL_WritePacketDetour->TakeDownDetour();
#if defined (BO2)
	BG_AdvanceTraceDetour->TakeDownDetour();
	preGameAntiFreezeDetour->TakeDownDetour();
	DynEntCl_DestoryEventDetour->TakeDownDetour();
	xmemcpyDetour->TakeDownDetour();
	NetDll_XNetGetTitleXnAddrDetour->TakeDownDetour();
#endif
	//R_AddCmdDrawStretchPicDetour->TakeDownDetour();

	delete vars;
	delete addr;
	delete esp;
	delete aimbot;

	*(short*)((int)cheatHandle + 0x40) = 1;
	DWORD exitCode;
	GetExitCodeThread(GetCurrentThread(), &exitCode);
	XexUnloadImageAndExitThread(cheatHandle, (HANDLE)exitCode);
}

char tempBuf[2] = { 0x06, 0x09 };

void main()
{
	vars = new variables();
	addr = new addresses();
	aimbot = new AIMBOT();
	esp = new Esp();

	int saveIniTimer = 0;

	bool didDriveMount = true;
	//if (tools::mountAspireDrive() != 0)
//	{
	//	didDriveMount = false;
//		printf("mount drive failed\n");
//	}

	funcByName = (int(*)(int num))tools::resolveFunction("Aspire.xex", 1);
	getCpuKey = (void(*)(char *))funcByName(GET_CPU_KEY);
	GetSectionInfo = (BOOL(*)(int, CONST PCHAR, PDWORD, PDWORD))funcByName(GET_SEC_INFO);
	sha1 = (void(*)(void*, unsigned int, unsigned char *))funcByName(GET_SHA_FUNC);
	sha1_starts = (void(*)(sha1_context *))funcByName(GET_SHA_START);
	HvPeekBytes = (HRESULT(*)(__int64, PVOID, DWORD))funcByName(HV_PEEK_BYTES);
	sha1_update = (void(*)(sha1_context *, unsigned char *, unsigned int))funcByName(GET_SHA_UPDATE);
	sha1_finish = (void(*)(sha1_context *ctx, unsigned char digest[20]))funcByName(GET_SHA_FINISH);
	getCheatHash = (void(*)(char *outBuffer))funcByName(GET_FILE_HASH);

	int ADDR[0x100] = { 0 };
	if (!auth::connect(ADDR))
	{
		return;
	}
	else
	{
		addr->R_EndFrame = ADDR[4];
		addr->CL_SendCmd = ADDR[5];
		addr->XamInputGetState = ADDR[6];
		addr->BG_AdvanceTrace = ADDR[7];
		addr->NetDll_XNetGetTitleXnAddr = ADDR[8];
		addr->R_AddCmdDrawText_ = ADDR[9];
		addr->R_AddCmdDrawStretchPic_ = ADDR[10];
		addr->R_RegisterFont_ = ADDR[11];
		addr->Material_RegisterHandle_ = ADDR[12];
		addr->R_TextWidth_ = ADDR[13];
		addr->CG_DrawRotatedPicPhysical_ = ADDR[14];
		addr->BG_GetWeaponDef_ = ADDR[15];
		addr->Cbuf_AddText_ = ADDR[16];
		addr->Com_GetClientDObj_ = ADDR[17];
		addr->CG_DObjGetWorldTagPos_ = ADDR[18];
		addr->GScr_AllocString_ = ADDR[19];
		addr->CG_GetPlayerViewOrigin_ = ADDR[20];
		addr->CG_LocationalTrace_ = ADDR[21];
		addr->BG_GetSpreadForWeapon_ = ADDR[22];
		addr->BG_GetWeaponIndexForName_ = ADDR[23];
		addr->Dvar_FindVar_ = ADDR[24];
		addr->UI_GetGameTypeOnMapName_ = ADDR[25];
		addr->CL_AllClientsDisconnected_ = ADDR[26];
		addr->FireBulletPenetrate_ = ADDR[27];
		addr->CG_CanSeeFriendlyHeadTrace_ = ADDR[28];
		addr->CG_GetPredictedPlayerState_ = ADDR[29];
		addr->DObjPhysicsGetBounds_ = ADDR[30];
		addr->ingame = ADDR[31];
		addr->screenWidth = ADDR[32];
		addr->screenHeight = ADDR[33];
		addr->screenplacement = ADDR[34];
		addr->FPS = ADDR[35];
		addr->netInfoGamertag = ADDR[36];
		addr->netInfoIP = ADDR[37];
		addr->netInfoXUID = ADDR[38];
		addr->netInfoSize = ADDR[39];
		addr->FOV = ADDR[40];
		addr->dvarAdditive = ADDR[41];
		addr->noRecoil = ADDR[42];
		addr->noSway = ADDR[43];
		addr->serverId = ADDR[44];
		addr->autowallPatch[0] = ADDR[45];
		addr->autowallPatch[1] = ADDR[46];
		addr->autowallPatch[2] = ADDR[47];
		addr->autowallPatch[3] = ADDR[48];
		addr->autowallPatch[4] = ADDR[49];
		addr->autowallPatch[5] = ADDR[50];
		addr->autowallPatch[6] = ADDR[51];
		addr->autowallPatch[7] = ADDR[52];
		addr->antiFreeze[0] = ADDR[53];
		addr->antiFreeze[1] = ADDR[54];
		addr->antiFreeze[2] = ADDR[55];
		addr->centity = ADDR[56];
		addr->cg_t = ADDR[57];
		addr->clientActive_t = ADDR[58];
		addr->netInfo = ADDR[59];

		R_AddCmdDrawText = (void(*)(const char *, int, void *, float, float, float, float, float, const float *, int))addr->R_AddCmdDrawText_;
		R_AddCmdDrawStretchPic = (void(*)(float, float, float, float, float, float, float, float, const float *, void *))addr->R_AddCmdDrawStretchPic_;
		CG_DrawRotatedPicPhysical = (void(*)(int, float, float, float, float, float, const float *, void *))addr->CG_DrawRotatedPicPhysical_;

		R_RegisterFont = (void*(*)(const char *, int))addr->R_RegisterFont_;
		Material_RegisterHandle = (void*(*)(const char *, int))addr->Material_RegisterHandle_;
		Cbuf_AddText = (void(*)(int, const char *))addr->Cbuf_AddText_;
		Com_GetClientDObj = (int(*)(int, int))addr->Com_GetClientDObj_;
		CG_DObjGetWorldTagPos = (int(*)(int *, int, int, vector3 &))addr->CG_DObjGetWorldTagPos_;
		GScr_AllocString = (int(*)(const char *))addr->GScr_AllocString_;
		CG_GetPlayerViewOrigin = (bool(*)(int, int, vector3 &))addr->CG_GetPlayerViewOrigin_;
		BG_GetWeaponIndexForName = (int(*)(const char *))addr->BG_GetWeaponIndexForName_;
		Dvar_FindVar = (int(*)(const char *))addr->Dvar_FindVar_;
		CL_AllClientsDisconnected = (bool(*)())addr->CL_AllClientsDisconnected_;
		CG_GetPredictedPlayerState = (int(*)(int))addr->CG_GetPredictedPlayerState_;
	}

	while (!vars->isUnloading)
	{
		if (vars->userBanned)
			break;

		if (vars->iniLoaded && saveIniTimer < 1000)
		{
			saveIniTimer++;
			if (saveIniTimer == 999)
			{
				tools::saveIniFile();
				saveIniTimer = 0;
			}
		}

		if (XamGetCurrentTitleID() == tools::gameTitle())
		{
			if (!vars->isGameLoaded && didDriveMount)
			{
				if (MmIsAddressValid(ADDR[0]))
				{
					tools::XNotify(tools::notifyText());

					if (XamUserGetSigninState(0) == eXUserSigninState_SignedInToLive)
					{
						XamUserGetName(0, vars->originalGamertag, 32);
#if defined (BO2)
						strcpy((char*)ADDR[0], vars->originalGamertag);
						strcpy((char*)ADDR[1], vars->originalGamertag);
#endif
					}

#if defined(BO2)
					*(int*)ADDR[2] = 0x38C00040;
					*(int*)ADDR[3] = 0x3B40FAC7;

					*(long long*)addr->autowallPatch[0] = 0x386000004E800020;
					*(long long*)addr->autowallPatch[1] = 0x386000004E800020;
					*(int*)addr->autowallPatch[2] = 0x48000218;
					*(int*)addr->autowallPatch[3] = 0x38600000;
					*(int*)addr->autowallPatch[4] = 0x38600000;
					*(int*)addr->autowallPatch[5] = 0x38600000;
					*(int*)addr->autowallPatch[6] = 0x38600001;
					*(int*)addr->autowallPatch[7] = 0x60000000;
#elif defined (BO1)
					*(long long*)addr->autowallPatch[0] = 0x386000014E800020;
					*(long long*)addr->autowallPatch[1] = 0x386000004E800020;
					*(long long*)addr->autowallPatch[2] = 0x386000004E800020;
					*(long long*)addr->autowallPatch[3] = 0x386000014E800020;
					*(long long*)addr->autowallPatch[4] = 0x396013372F161337;
#elif defined (MW2)
					*(int*)addr->autowallPatch[0] = 0x60000000;
					*(int*)addr->autowallPatch[1] = 0x60000000;
					*(int*)addr->autowallPatch[2] = 0x60000000;
					*(int*)addr->autowallPatch[3] = 0x60000000;
					*(long long*)addr->autowallPatch[4] = 0x386000004E800020;
					*(long long*)addr->autowallPatch[5] = 0x386000014E800020;
#endif
					for (int i = 0; i < ARRAYSIZE(boneNames); i++)
						vars->cachedBones[i] = GScr_AllocString(boneNames[i]);

					tools::loadIniFile();
					Menu::createMenu();

					vars->width = (float)*(int*)addr->screenWidth;
					vars->height = (float)*(int*)addr->screenHeight;

					R_EndFrameDetour = new Detour<void>;
					R_EndFrameDetour->SetupDetour(addr->R_EndFrame, hooks::R_EndFrameHook);

					XamInputGetStateDetour = new Detour<int>;
					XamInputGetStateDetour->SetupDetour(addr->XamInputGetState, hooks::XamInputGetStateHook);

					CL_WritePacketDetour = new Detour<void>;
					CL_WritePacketDetour->SetupDetour(addr->CL_SendCmd, hooks::CL_WritePacketHook);

#if defined (BO2)
					BG_AdvanceTraceDetour = new Detour<void>;
					BG_AdvanceTraceDetour->SetupDetour(addr->BG_AdvanceTrace, hooks::BG_AdvanceTraceHook);

					preGameAntiFreezeDetour = new Detour<int>;
					preGameAntiFreezeDetour->SetupDetour(addr->antiFreeze[0], hooks::preGameAntiFreezeHook);

					DynEntCl_DestoryEventDetour = new Detour<void>;
					DynEntCl_DestoryEventDetour->SetupDetour(addr->antiFreeze[1], hooks::DynEntCl_DestroyEventHook);

					xmemcpyDetour = new Detour<int>;
					xmemcpyDetour->SetupDetour(addr->antiFreeze[2], hooks::xmemcpyHook);

					NetDll_XNetGetTitleXnAddrDetour = new Detour<int>;
					NetDll_XNetGetTitleXnAddrDetour->SetupDetour(addr->NetDll_XNetGetTitleXnAddr, hooks::NetDll_XNetGetTitleXnAddrHook);
#endif
					//R_AddCmdDrawStretchPicDetour = new Detour<void>;
					//R_AddCmdDrawStretchPicDetour->SetupDetour(addr->R_AddCmdDrawStretchPic_, hooks::R_AddCmdDrawStretchPicHook);

					Sleep(100);
				}
				vars->isGameLoaded = true;
			}

			for (int i = 0; i < 18; i++)
			{
				if (MmIsAddressValid(addr->netInfo))
				{
					unsigned long long XUID = *(unsigned long long*)(addr->netInfo + (i * addr->netInfoSize) + addr->netInfoXUID);
					if (XUID != storedXUIDs[i])
					{
						storedXUIDs[i] = XUID;
						func::downloadGeoLocation(i);
					}
				}
			}

			if (vars->enterCustomTag)
			{
				vars->enterCustomTag = false;
				XOVERLAPPED Overlapped;
				WCHAR wValue[512] = { 0 };
				ZeroMemory(&Overlapped, sizeof(Overlapped));

				if(vars->enterNumCustomTag)
					XShowKeyboardUI(0, VKBD_DEFAULT, tools::wc(PresetGamertag[menu.Scroll[menu.currentMenu] - 4]), L"Aspire CE - Edit Gamertag", L"Edit/Change your gamertag. (max 32 characters)", wValue, 32, &Overlapped);
				else
					XShowKeyboardUI(0, VKBD_DEFAULT, tools::wc(vars->originalGamertag), L"Aspire CE - Custom Gamertag", L"Edit/Change your gamertag. (max 32 characters)", wValue, 32, &Overlapped);

				while (!XHasOverlappedIoCompleted(&Overlapped))
					Sleep(100);

				if (vars->enterNumCustomTag)
				{
					char tempBuffer[40] = { 0 };
					wcstombs(tempBuffer, wValue, 32);
					strncpy(PresetGamertag[menu.Scroll[menu.currentMenu] - 4], tempBuffer, 32);
				}
				else
				{
					wcstombs(vars->originalGamertag, wValue, 32);
					func::setGamertag(vars->originalGamertag);
				}

				vars->enterNumCustomTag = false;
			}

			if (vars->rainbowGamertag)
			{
				static int color = 0;
				static int timer = 0;
				if (timer < vars->rainbowSpeed * 8)
					timer++;
				else
				{
					if (color < 9)
						color++;
					else color = 0;
					char rainowTag[40] = { 0 };
					sprintf_s(rainowTag, "^%i%s", color, vars->originalGamertag);
					func::setGamertag(rainowTag);
					timer = 0;
				}
			}
		}
		else if(XamGetCurrentTitleID() != tools::gameTitle())
		{
			static bool once = false;
			if (!once)
			{
				once = true;
				takeDownDetours();
			}
		}

		Sleep(1);
	}
}

bool WINAPI DllMain(HANDLE hInstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    switch (fdwReason)
	{
    case DLL_PROCESS_ATTACH:
		cheatHandle = hInstDLL;
		HANDLE hThread; DWORD hThreadID;
		aspireHandle = (PLDR_DATA_TABLE_ENTRY)hInstDLL;
		ExCreateThread(&hThread, 0, &hThreadID, (PVOID)XapiThreadStartup, (LPTHREAD_START_ROUTINE)main, NULL, 0x2);
		XSetThreadProcessor(hThread, 4);
		ResumeThread(hThread);
		CloseHandle(hThread);
        break;
    case DLL_PROCESS_DETACH:
#if defined (debug)
		takeDownDetours();
		Sleep(1000);
#endif
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    }
	return true;
}

extern "C" static int __proc_attached;
extern "C" int _CRT_INIT(...);
extern "C" int __CppXcptFilter(...);

unsigned char branchReplaceData[0x2C] = { 'x', 'b', 'O', 'b', 'f', 'u', 0, 0, (sizeof(branchReplaceData) >> 24) & 0xFF, (sizeof(branchReplaceData) >> 16) & 0xFF, (sizeof(branchReplaceData) >> 8) & 0xFF, sizeof(branchReplaceData) & 0xFF };

#pragma code_seg(push, r1, ".ptext")
#pragma optimize("", off)
void xbObfuserDoPatches()
{
	// location of the patch data
	DWORD procPatchData = (DWORD)branchReplaceData + 0x10;
	DWORD dwSeed;

	// if there is obfuscation
	if (*(DWORD*)branchReplaceData != 'xbOb') {

		dwSeed = ~*(DWORD *)procPatchData;
		procPatchData += 4;

		while (true) {
			DWORD patchOffset = *(DWORD *)procPatchData;
			DWORD patchSize = *(DWORD *)(procPatchData + 4);

			// if we are at the end of the patches break out
			if (patchOffset == 0)
				break;

			patchOffset = ~(patchOffset ^ 0x4B3287AA);
			patchSize = ~(patchSize ^ 0x2F379A2D);

			// set all the data in the array
			for (int i = 0; i < patchSize; i += 4)
			{
				*(DWORD *)(patchOffset + i) ^= (dwSeed ^ 0x72BF219A);
				dwSeed *= 0x32;

				// random stuff to annoy people :P
				if ((dwSeed & 0x1) != 0) dwSeed |= 0x1A26F37F;
				else if ((dwSeed & 0x2) != 0) dwSeed &= 0x9AFB4862;
				else if ((dwSeed & 0x4) != 0) dwSeed += 0x3A762F9B;
				else if ((dwSeed & 0x8) != 0) dwSeed ^= 0xF92A6472;
				if (dwSeed == 0)
					dwSeed = 0x13333337;
				dwSeed %= (dwSeed / 2) * 21;
			}

			// increment the patch data pointer
			procPatchData += 8;
		}

		// do the RC4 last because it's not in the .ptext
		if (*(DWORD *)branchReplaceData)
			tools::RC4(branchReplaceData + 8, 8, (PVOID)((~*(DWORD *)branchReplaceData) ^ 0x589B2), (~*(DWORD *)(branchReplaceData + 4)) ^ 0x6A221);

		// set the header back so we don't re-call it
		*(DWORD *)branchReplaceData = 'xbOb';
	}
}

__declspec(noinline) BOOL __cdecl realEntryPoint(HANDLE hDllHandle, DWORD dwReason, LPVOID lpreserved)
{
	BOOL retcode = TRUE;

	xbObfuserDoPatches();

	if ((dwReason == DLL_PROCESS_DETACH) && (__proc_attached == 0))

		return FALSE;

	__try {
		if (dwReason == DLL_PROCESS_ATTACH || dwReason == DLL_THREAD_ATTACH)
		{
			retcode = _CRT_INIT(hDllHandle, dwReason, lpreserved);

			if (!retcode)
				return FALSE;
		}

		retcode = DllMain(hDllHandle, dwReason, lpreserved);

		if ((dwReason == DLL_PROCESS_ATTACH) && !retcode)
		{

			DllMain(hDllHandle, DLL_PROCESS_DETACH, lpreserved);
			_CRT_INIT(hDllHandle, DLL_PROCESS_DETACH, lpreserved);
		}

		if ((dwReason == DLL_PROCESS_DETACH) ||
			(dwReason == DLL_THREAD_DETACH))
		{
			if (_CRT_INIT(hDllHandle, dwReason, lpreserved) == FALSE)
				retcode = FALSE;

		}
	}
	__except (__CppXcptFilter(GetExceptionCode(), GetExceptionInformation())) {
		return FALSE;
	}

	return retcode;
}
#pragma optimize("", on)
#pragma code_seg(pop, r1)