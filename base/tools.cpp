#include "main.h"

void(*XNotifyQueueUI)(int dwType, int dwUserIndex, int dwPriority, wchar_t *pwszStringParam, unsigned long long qwParam)
	= (void(*)(int, int, int, wchar_t *, unsigned long long))tools::resolveFunction("xam.xex", 0x290);

int(*XamGetCurrentTitleID)() = (int(*)())tools::resolveFunction("xam.xex", 0x1CF);
int(*MmIsAddressValid)(int address) = (int(*)(int))tools::resolveFunction("xboxkrnl.exe", 191);

XINPUT_STATE state;
unsigned int Wait = 0, tickCount = 0;
PLIST_ENTRY PsLoadedModuleList = 0;

void tools::patchInJump(int *address, int destination, bool linked)
{
	address[0] = (destination & 0x8000) ? 0x3D600000 + (((destination >> 16) & 0xFFFF) + 1) : 0x3D600000 + ((destination >> 16) & 0xFFFF);
	address[1] = 0x396B0000 + (destination & 0xFFFF);
	address[2] = 0x7D6903A6;
	address[3] = linked ? 0x4E800421 : 0x4E800420;
}

void __declspec(naked) GLPR(void)
{
	__asm
	{
		std     r14, -0x98(sp)
		std     r15, -0x90(sp)
		std     r16, -0x88(sp)
		std     r17, -0x80(sp)
		std     r18, -0x78(sp)
		std     r19, -0x70(sp)
		std     r20, -0x68(sp)
		std     r21, -0x60(sp)
		std     r22, -0x58(sp)
		std     r23, -0x50(sp)
		std     r24, -0x48(sp)
		std     r25, -0x40(sp)
		std     r26, -0x38(sp)
		std     r27, -0x30(sp)
		std     r28, -0x28(sp)
		std     r29, -0x20(sp)
		std     r30, -0x18(sp)
		std     r31, -0x10(sp)
		stw     r12, -0x8(sp)
		blr
	}
}

int tools::relinkGPLR(int SFSOffset, int *saveStubAddress, int *originalAddress)
{
	int instruction = 0, replacing = 0;
	int *saver = (int*)GLPR;

	if (SFSOffset & 0x2000000)
		SFSOffset = SFSOffset | 0xFC000000;
	replacing = originalAddress[SFSOffset / 4];
	for (int i = 0; i < 20; i++)
	{
		if (replacing == saver[i])
		{
			int newOffset = (int)&saver[i] - (int)saveStubAddress;
			instruction = 0x48000001 | (newOffset & 0x3FFFFFC);
		}
	}
	return instruction;
}

void tools::hookFunctionStart(int *Address, int *saveStub, int Destination)
{
	if ((saveStub != NULL) && (Address != NULL))
	{
		int AddressRelocation = (int)(&Address[4]);
		saveStub[0] = (AddressRelocation & 0x8000) ? 0x3D600000 + (((AddressRelocation >> 16) & 0xFFFF) + 1) : 0x3D600000 + ((AddressRelocation >> 16) & 0xFFFF);
		saveStub[1] = 0x396B0000 + (AddressRelocation & 0xFFFF);
		saveStub[2] = 0x7D6903A6;

		for (int i = 0; i < 4; i++)
		{
			saveStub[i + 3] = ((Address[i] & 0x48000003) == 0x48000001) ? relinkGPLR((Address[i] & ~0x48000003), &saveStub[i + 3], &Address[i]) : Address[i];
		}
		saveStub[7] = 0x4E800420;
		__dcbst(0, saveStub); 
		__sync();
		__isync();
		patchInJump(Address, Destination, false);
	}
}

int tools::resolveFunction(char *moduleName, int ordinal)
{
	HANDLE hModule; DWORD Address;
	XexGetModuleHandle(moduleName, &hModule);
	XexGetProcedureAddress(hModule, ordinal, &Address);
	return Address;
}

DWORD tools::PatchModuleImport(PLDR_DATA_TABLE_ENTRY Module, PCHAR ImportedModuleName, DWORD Ordinal, DWORD PatchAddress)
{
	DWORD address = (DWORD)resolveFunction(ImportedModuleName, Ordinal);

	PVOID headerBase = Module->XexHeaderBase;
	PXEX_IMPORT_DESCRIPTOR importDesc = (PXEX_IMPORT_DESCRIPTOR)RtlImageXexHeaderField(headerBase, 0x000103FF);

	DWORD result = 2;

	PCHAR stringTable = (PCHAR)(importDesc + 1);

	XEX_IMPORT_TABLE_ORG* importTable = (XEX_IMPORT_TABLE_ORG*)(stringTable + importDesc->NameTableSize);

	for (DWORD x = 0; x < importDesc->ModuleCount; x++) {
		DWORD* importAdd = (PDWORD)(importTable + 1);
		for (DWORD y = 0; y < importTable->ImportTable.ImportCount; y++) {
			DWORD value = *((PDWORD)importAdd[y]);
			if (value == address) {
				memcpy((PDWORD)importAdd[y], &PatchAddress, 4);
				DWORD newCode[4];
				tools::patchInJump((int*)&newCode[0], PatchAddress, FALSE);
				memcpy((PDWORD)importAdd[y + 1], newCode, 16);
				result = S_OK;
			}
		}
		importTable = (XEX_IMPORT_TABLE_ORG*)(((PBYTE)importTable) + importTable->TableSize);
	}
	return result;
}

int tools::patchModuleImport(char *module, char *importedModuleName, int ordinal, int patchAddress)
{
	PLDR_DATA_TABLE_ENTRY moduleHandle = (PLDR_DATA_TABLE_ENTRY)GetModuleHandle(module);
	return PatchModuleImport(moduleHandle, importedModuleName, ordinal, patchAddress);
}

wchar_t *tools::wc(char *c)
{
	size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}

void tools::xnotifyThread(wchar_t *text)
{
	XNotifyQueueUI(0xE, XUSER_INDEX_ANY, 2, text, NULL);
}

void tools::XNotify(char *text)
{
	if (KeGetCurrentProcessType() == 1)
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)xnotifyThread, (LPVOID)wc(text), NULL, NULL);
	else XNotifyQueueUI(0xE, XUSER_INDEX_ANY, 2, wc(text), NULL);
}

bool tools::keyIsDown(int key)
{
	XINPUT_STATE state;
	XInputGetState(0, &state);
	if (key == NULL)
		return true;

	if (key == 1337 && state.Gamepad.bLeftTrigger > 50)
		return true;
	else if (key == -1337 && state.Gamepad.bRightTrigger > 50)
		return true;
	else
		return (bool)(state.Gamepad.wButtons & key);

	return false;
}

int tools::gameTitle()
{
#if defined(BO1)
	return COD_BO1;
#elif defined(BO2)
	return COD_BO2;
#elif defined (MW2)
	return COD_MW2;
#elif defined (MW3)
	return COD_MW3;
#endif
}

char *tools::notifyText()
{
#if defined(BO1)
	return "Aspire CE - BO1 Cheats Loaded!";
#elif defined(BO2)
	return "Aspire CE - BO2 Cheats Loaded!";
#elif defined (MW2)
	return "Aspire CE - MW2 Cheats Loaded!";
#elif defined (MW3)
	return "Aspire CE - MW3 Cheats Loaded!";
#endif
}

void tools::saveIniFile()
{
	CSimpleIniA ini;
	ini.SetUnicode();

	ini.SetBoolValue("aimbot", "aimbot", aimbot->aimbotOn);
	ini.SetLongValue("aimbot", "aimbot_type", aimbot->aimbotType);
	ini.SetLongValue("aimbot", "target_type", aimbot->targetType);
	ini.SetBoolValue("aimbot", "no_spread", aimbot->noSpread);
	ini.SetBoolValue("aimbot", "auto_shoot", aimbot->autoShoot);
	ini.SetBoolValue("aimbot", "no_recoil", aimbot->noRecoil);
	ini.SetBoolValue("aimbot", "no_sway", aimbot->noSway);
	ini.SetBoolValue("aimbot", "auto_bone", aimbot->autoBone);

	ini.SetBoolValue("spinbot", "spinbot", aimbot->spinbotOn);
	ini.SetLongValue("spinbot", "spinbot_x", aimbot->spinbotX);
	ini.SetLongValue("spinbot", "spinbot_x_speed", aimbot->xSpeed);
	ini.SetLongValue("spinbot", "spinbot_y", aimbot->spinbotY);
	ini.SetLongValue("spinbot", "spinbot_y_speed", aimbot->ySpeed);
	ini.SetLongValue("spinbot", "spinbot_z", aimbot->spinbotZ);
	ini.SetLongValue("spinbot", "spinbot_z_speed", aimbot->zSpeed);

	ini.SetBoolValue("esp", "enemy", esp->enemy);
	ini.SetBoolValue("esp", "friendly", esp->friendly);
	ini.SetLongValue("esp", "snapline_type", esp->snaplines);
	ini.SetBoolValue("esp", "bones", esp->bones);
	ini.SetBoolValue("esp", "names", esp->names);
	ini.SetLongValue("esp", "box_type", esp->boxType);
	ini.SetBoolValue("esp", "player_weapons", esp->weaponIcons);
	ini.SetBoolValue("esp", "explosive_esp", esp->explosiveEsp);
	ini.SetBoolValue("esp", "dropped_item_esp", esp->droppedWeaponEsp);

	ini.SetDoubleValue("misc", "fov", vars->FOV);
	ini.SetDoubleValue("misc", "safe_x", vars->safeArea.x);
	ini.SetDoubleValue("misc", "safe_y", vars->safeArea.y);

	ini.SetBoolValue("visuals", "fps", vars->fps);
	ini.SetLongValue("visuals", "host_info", vars->hostName);
	ini.SetBoolValue("visuals", "host_geo_location", vars->hostGeoLocation);
	ini.SetBoolValue("visuals", "map_name", vars->mapName);
	ini.SetBoolValue("visuals", "health_bar", vars->healthBar);
	ini.SetBoolValue("visuals", "crosshair", vars->crosshair);
	ini.SetBoolValue("visuals", "controls", vars->controls);

	ini.SetDoubleValue("esp_color", "friendly_red", friendColor[0]);
	ini.SetDoubleValue("esp_color", "friendly_blue", friendColor[1]);
	ini.SetDoubleValue("esp_color", "friendly_green", friendColor[2]);
	ini.SetDoubleValue("esp_color", "friendly_alpha", friendColor[3]);

	ini.SetDoubleValue("esp_color", "enemy_red", enemColor[0]);
	ini.SetDoubleValue("esp_color", "enemy_blue", enemColor[1]);
	ini.SetDoubleValue("esp_color", "enemy_green", enemColor[2]);
	ini.SetDoubleValue("esp_color", "enemy_alpha", enemColor[3]);

	ini.SetDoubleValue("esp_color", "enemy_hittable_red", hittableColor[0]);
	ini.SetDoubleValue("esp_color", "enemy_hittable_blue", hittableColor[1]);
	ini.SetDoubleValue("esp_color", "enemy_hittable_green", hittableColor[2]);
	ini.SetDoubleValue("esp_color", "enemy_hittable_alpha", hittableColor[3]);

	ini.SetDoubleValue("esp_color", "enemy_visible_red", visibleColor[0]);
	ini.SetDoubleValue("esp_color", "enemy_visible_blue", visibleColor[1]);
	ini.SetDoubleValue("esp_color", "enemy_visible_green", visibleColor[2]);
	ini.SetDoubleValue("esp_color", "enemy_visible_alpha", visibleColor[3]);

	ini.SetDoubleValue("esp_color", "whitelisted_red", ignoredColor[0]);
	ini.SetDoubleValue("esp_color", "whitelisted_blue", ignoredColor[1]);
	ini.SetDoubleValue("esp_color", "whitelisted_green", ignoredColor[2]);
	ini.SetDoubleValue("esp_color", "whitelisted_alpha", ignoredColor[3]);

	ini.SetDoubleValue("esp_color", "prioritized_red", targetedClient[0]);
	ini.SetDoubleValue("esp_color", "prioritized_blue", targetedClient[1]);
	ini.SetDoubleValue("esp_color", "prioritized_green", targetedClient[2]);
	ini.SetDoubleValue("esp_color", "prioritized_alpha", targetedClient[3]);

	ini.SetDoubleValue("menu_color", "primary_red", darkMenuColor[0]);
	ini.SetDoubleValue("menu_color", "primary_blue", darkMenuColor[1]);
	ini.SetDoubleValue("menu_color", "primary_green", darkMenuColor[2]);
	ini.SetDoubleValue("menu_color", "primary_alpha", darkMenuColor[3]);
	ini.SetBoolValue("menu_color", "primary_rainbow", vars->primaryColorRainbow);
	ini.SetDoubleValue("menu_color", "primary_rainbow_speed", vars->pimaryRainbowSpeed);

	ini.SetDoubleValue("menu_color", "secondary_red", secondaryColor[0]);
	ini.SetDoubleValue("menu_color", "secondary_blue", secondaryColor[1]);
	ini.SetDoubleValue("menu_color", "secondary_green", secondaryColor[2]);
	ini.SetDoubleValue("menu_color", "secondary_alpha", secondaryColor[3]);
	ini.SetBoolValue("menu_color", "secondary_rainbow", vars->secondaryColorRainbow);
	ini.SetDoubleValue("menu_color", "secondary_rainbow_speed", vars->secondaryRainbowSpeed);

	ini.SetDoubleValue("menu_color", "background_red", lightMenuColor[0]);
	ini.SetDoubleValue("menu_color", "background_blue", lightMenuColor[1]);
	ini.SetDoubleValue("menu_color", "background_green", lightMenuColor[2]);
	ini.SetDoubleValue("menu_color", "background_alpha", lightMenuColor[3]);
	ini.SetBoolValue("menu_color", "background_rainbow", vars->backgroundColorRainbow);
	ini.SetDoubleValue("menu_color", "background_rainbow_speed", vars->backgroundRainbowSpeed);

	ini.SetBoolValue("menu_color", "font_shadow", vars->fontShadow);

	ini.SetLongValue("gamertag_settings", "rainbow_delay", vars->rainbowSpeed);
	ini.SetValue("gamertag_settings", "gamertag1", PresetGamertag[0]);
	ini.SetValue("gamertag_settings", "gamertag2", PresetGamertag[1]);
	ini.SetValue("gamertag_settings", "gamertag3", PresetGamertag[2]);
	ini.SetValue("gamertag_settings", "gamertag4", PresetGamertag[3]);
	ini.SetValue("gamertag_settings", "gamertag5", PresetGamertag[4]);
	ini.SetLongValue("gamertag_settings", "cycle_delay", vars->gamertagCycleDelay);

	ini.SaveFile("Aspire:\\AspireCE.ini");
}

void tools::loadIniFile()
{
	CSimpleIniA ini;
	ini.SetUnicode();

	if (ini.LoadFile("Aspire:\\AspireCE.ini") < 0)
		saveIniFile();

	aimbot->aimbotOn = ini.GetBoolValue("aimbot", "aimbot", 0);
	aimbot->aimbotType = ini.GetLongValue("aimbot", "aimbot_type", 0);
	aimbot->targetType = ini.GetLongValue("aimbot", "target_type", 0);
	aimbot->noSpread = ini.GetBoolValue("aimbot", "no_spread", 0);
	aimbot->autoShoot = ini.GetBoolValue("aimbot", "auto_shoot", 0);
	aimbot->noRecoil = ini.GetBoolValue("aimbot", "no_recoil", 0);
	aimbot->noSway = ini.GetBoolValue("aimbot", "no_sway", 0);
	aimbot->autoBone = ini.GetBoolValue("aimbot", "auto_bone", 0);

	aimbot->spinbotOn = ini.GetBoolValue("spinbot", "spinbot", 0);
	aimbot->spinbotX = ini.GetLongValue("spinbot", "spinbot_x", 0);
	aimbot->xSpeed = ini.GetLongValue("spinbot", "spinbot_x_speed", 1);
	aimbot->spinbotY = ini.GetLongValue("spinbot", "spinbot_y", 0);
	aimbot->ySpeed = ini.GetLongValue("spinbot", "spinbot_y_speed", 1);
	aimbot->spinbotZ = ini.GetLongValue("spinbot", "spinbot_z", 0);
	aimbot->zSpeed = ini.GetLongValue("spinbot", "spinbot_z_speed", 1);

	esp->enemy = ini.GetBoolValue("esp", "enemy", 0);
	esp->friendly = ini.GetBoolValue("esp", "friendly", 0);
	esp->snaplines = ini.GetLongValue("esp", "snapline_type", 0);
	esp->bones = ini.GetBoolValue("esp", "bones", 0);
	esp->names = ini.GetBoolValue("esp", "names", 0);
	esp->boxType = ini.GetLongValue("esp", "box_type", 0);
	esp->weaponIcons = ini.GetBoolValue("esp", "player_weapons", 0);
	esp->explosiveEsp = ini.GetBoolValue("esp", "explosive_esp", 0);
	esp->droppedWeaponEsp = ini.GetBoolValue("esp", "dropped_item_esp", 0);

	vars->FOV = ini.GetDoubleValue("misc", "fov", 65.0f);
	vars->safeArea.x = ini.GetDoubleValue("misc", "safe_x", 0.0f);
	vars->safeArea.y = ini.GetDoubleValue("misc", "safe_y", 0.0f);

	vars->fps = ini.GetBoolValue("visuals", "fps", 0);
	vars->hostName = ini.GetLongValue("visuals", "host_info", 0);
	vars->hostGeoLocation = ini.GetBoolValue("visuals", "host_geo_location", 0);
	vars->mapName = ini.GetBoolValue("visuals", "map_name", 0);
	vars->healthBar = ini.GetBoolValue("visuals", "health_bar", 0);
	vars->crosshair = ini.GetBoolValue("visuals", "crosshair", 0);
	vars->controls = ini.GetBoolValue("visuals", "controls", 0);

	friendColor[0] = ini.GetDoubleValue("esp_color", "friendly_red", 0.0f);
	friendColor[1] = ini.GetDoubleValue("esp_color", "friendly_blue", 1.0f);
	friendColor[2] = ini.GetDoubleValue("esp_color", "friendly_green", 0.0f);
	friendColor[3] = ini.GetDoubleValue("esp_color", "friendly_alpha", 1.0f);

	enemColor[0] = ini.GetDoubleValue("esp_color", "enemy_red", 1.0f);
	enemColor[1] = ini.GetDoubleValue("esp_color", "enemy_blue", 0.0f);
	enemColor[2] = ini.GetDoubleValue("esp_color", "enemy_green", 0.0f);
	enemColor[3] = ini.GetDoubleValue("esp_color", "enemy_alpha", 1.0f);

	hittableColor[0] = ini.GetDoubleValue("esp_color", "enemy_hittable_red", 1.0f);
	hittableColor[1] = ini.GetDoubleValue("esp_color", "enemy_hittable_blue", 0.64f);
	hittableColor[2] = ini.GetDoubleValue("esp_color", "enemy_hittable_green", 0.0f);
	hittableColor[3] = ini.GetDoubleValue("esp_color", "enemy_hittable_alpha", 1.0f);

	visibleColor[0] = ini.GetDoubleValue("esp_color", "enemy_visible_red", 1.0f);
	visibleColor[1] = ini.GetDoubleValue("esp_color", "enemy_visible_blue", 1.0f);
	visibleColor[2] = ini.GetDoubleValue("esp_color", "enemy_visible_green", 0.0f);
	visibleColor[3] = ini.GetDoubleValue("esp_color", "enemy_visible_alpha", 1.0f);

	ignoredColor[0] = ini.GetDoubleValue("esp_color", "whitelisted_red", 1.0f);
	ignoredColor[1] = ini.GetDoubleValue("esp_color", "whitelisted_blue", 1.0f);
	ignoredColor[2] = ini.GetDoubleValue("esp_color", "whitelisted_green", 1.0f);
	ignoredColor[3] = ini.GetDoubleValue("esp_color", "whitelisted_alpha", 1.0f);

	targetedClient[0] = ini.GetDoubleValue("esp_color", "prioritized_red", 1.0f);
	targetedClient[1] = ini.GetDoubleValue("esp_color", "prioritized_blue", 0.0f);
	targetedClient[2] = ini.GetDoubleValue("esp_color", "prioritized_green", 1.0f);
	targetedClient[3] = ini.GetDoubleValue("esp_color", "prioritized_alpha", 1.0f);

	darkMenuColor[0] = ini.GetDoubleValue("menu_color", "primary_red", 0.0f);
	darkMenuColor[1] = ini.GetDoubleValue("menu_color", "primary_blue", 0.47f);
	darkMenuColor[2] = ini.GetDoubleValue("menu_color", "primary_green", 0.84f);
	darkMenuColor[3] = ini.GetDoubleValue("menu_color", "primary_alpha", 1.0f);
	vars->primaryColorRainbow = ini.GetBoolValue("menu_color", "primary_rainbow", false);
	vars->pimaryRainbowSpeed = ini.GetDoubleValue("menu_color", "primary_rainbow_speed", 0.001f);

	secondaryColor[0] = ini.GetDoubleValue("menu_color", "secondary_red", 1.0f);
	secondaryColor[1] = ini.GetDoubleValue("menu_color", "secondary_blue", 1.0f);
	secondaryColor[2] = ini.GetDoubleValue("menu_color", "secondary_green", 1.0f);
	secondaryColor[3] = ini.GetDoubleValue("menu_color", "secondary_alpha", 1.0f);
	vars->secondaryColorRainbow = ini.GetBoolValue("menu_color", "secondary_rainbow", false);
	vars->secondaryRainbowSpeed = ini.GetDoubleValue("menu_color", "secondary_rainbow_speed", 0.001f);

	lightMenuColor[0] = ini.GetDoubleValue("menu_color", "background_red", 0.11f);
	lightMenuColor[1] = ini.GetDoubleValue("menu_color", "background_blue", 0.11f);
	lightMenuColor[2] = ini.GetDoubleValue("menu_color", "background_green", 0.11f);
	lightMenuColor[3] = ini.GetDoubleValue("menu_color", "background_alpha", 1.0f);
	vars->backgroundColorRainbow = ini.GetBoolValue("menu_color", "background_rainbow", false);
	vars->backgroundRainbowSpeed = ini.GetDoubleValue("menu_color", "background_rainbow_speed", 0.001f);

	vars->fontShadow = ini.GetBoolValue("menu_color", "font_shadow", 0);

	vars->rainbowSpeed = ini.GetLongValue("gamertag_settings", "rainbow_delay", 1);
	strncpy(PresetGamertag[0], (char*)ini.GetValue("gamertag_settings", "gamertag1", "Empty..."), 32);
	strncpy(PresetGamertag[1], (char*)ini.GetValue("gamertag_settings", "gamertag2", "Empty..."), 32);
	strncpy(PresetGamertag[2], (char*)ini.GetValue("gamertag_settings", "gamertag3", "Empty..."), 32);
	strncpy(PresetGamertag[3], (char*)ini.GetValue("gamertag_settings", "gamertag4", "Empty..."), 32);
	strncpy(PresetGamertag[4], (char*)ini.GetValue("gamertag_settings", "gamertag5", "Empty..."), 32);
	vars->gamertagCycleDelay = ini.GetLongValue("gamertag_settings", "cycle_delay", 1);


	vars->iniLoaded = true;
}

DWORD tools::_resolve(HMODULE Module, DWORD Export, DWORD off1, DWORD off2)
{
	SHORT t1, t2;
	DWORD *f;

	if (FAILED(XexGetProcedureAddress(Module, Export, &f)))
		return 0;

	t1 = f[off1] & 0xFFFF;
	t2 = f[off2] & 0xFFFF;

	return (t1 << 16) + t2;
}

#define InsertTailList(ListHead,Entry) {\
    PLIST_ENTRY _EX_Blink;\
    PLIST_ENTRY _EX_ListHead;\
    _EX_ListHead = (ListHead);\
    _EX_Blink = _EX_ListHead->Blink;\
    (Entry)->Flink = _EX_ListHead;\
    (Entry)->Blink = _EX_Blink;\
    _EX_Blink->Flink = (Entry);\
    _EX_ListHead->Blink = (Entry);\
}

void tools::addToList(HANDLE hModule)
{
	PLDR_DATA_TABLE_ENTRY Module;

	if (hModule == 0)
		return;

	Module = (PLDR_DATA_TABLE_ENTRY)hModule;

	// Add the module to the PsLoadedModuleList
	if (PsLoadedModuleList == 0)
		PsLoadedModuleList = (PLIST_ENTRY)_resolve(GetModuleHandle("xboxkrnl.exe"), 412, 9, 11);

	InsertTailList(PsLoadedModuleList, &Module->InLoadOrderLinks);
}

DWORD BytesRead = 0;
BOOL tools::WriteFileAlt(PCHAR Path, LPCVOID Buffer, DWORD dwBytesToWrite)
{
	HANDLE fileHandle = CreateFile(Path, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		CloseHandle(fileHandle);
		return FALSE;
	}
	WriteFile(fileHandle, Buffer, dwBytesToWrite, &BytesRead, NULL);
	CloseHandle(fileHandle);
	return TRUE;
}

HRESULT MountPath(const PCHAR szDrive, const PCHAR szDevice)
{
	PCHAR szSysStr = NULL;
	if (KeGetCurrentProcessType() == SYSTEM_PROC)
		szSysStr = "\\System??\\%s";
	else
		szSysStr = "\\??\\%s";

	CHAR szDestinationDrive[MAX_PATH];
	sprintf_s(szDestinationDrive, MAX_PATH, szSysStr, szDrive);

	STRING strDeviceName = MAKE_STRING(szDevice);
	STRING strLinkName = MAKE_STRING(szDestinationDrive);

	ObDeleteSymbolicLink(&strLinkName);

	return ObCreateSymbolicLink(&strLinkName, &strDeviceName);
}

void replaceAll(std::string& source, const std::string& from, const std::string& to)
{
	std::string newString;
	newString.reserve(source.length());  // avoids a few memory allocations

	std::string::size_type lastPos = 0;
	std::string::size_type findPos;

	while (std::string::npos != (findPos = source.find(from, lastPos)))
	{
		newString.append(source, lastPos, findPos - lastPos);
		newString += to;
		lastPos = findPos + from.length();
	}

	// Care for the rest after last occurrence
	newString += source.substr(lastPos);

	source.swap(newString);
}

void tools::RC4(void* Key, int ccKey, void* Data, int ccData)
{

	byte s[256];
	byte k[256];
	byte temp;
	byte* Key2 = (byte*)Key;
	byte* Data2 = (byte*)Data;
	int i, j;

	if (!Key || !Data || !ccData)
		return;

	for (i = 0; i < 256; i++)
	{
		s[i] = (byte)i;
		k[i] = Key2[i % ccKey];
	}

	j = 0;
	for (i = 0; i < 256; i++)
	{
		j = (j + s[i] + k[i]) % 256;
		temp = s[i];
		s[i] = s[j];
		s[j] = temp;
	}

	i = j = 0;
	for (int x = 0; x < ccData; x++)
	{
		i = (i + 1) % 256;
		j = (j + s[i]) % 256;
		temp = s[i];
		s[i] = s[j];
		s[j] = temp;
		int t = (s[i] + s[j]) % 256;
		Data2[x] ^= s[t];
	}
}

char *returnModuleName()
{
#if defined (BO1)
	char xexNameKey[0x8] = { 0xFC, 0xC5, 0xE1, 0x81, 0x13, 0xDB, 0xE5, 0xAF };
	char xexNameEnc[16] = {
		0x3C, 0xE5, 0xA9, 0xC3, 0xE8, 0xFD, 0xA1, 0x4C, 0x21, 0xA6, 0x85, 0xB5,
		0x4F, 0xBD, 0x0E, 0x94
	};
	RC4(xexNameKey, 0x8, xexNameEnc, 16);
	return xexNameEnc;
#elif defined (BO2)
	char xexNameKey[8] = { 0xD0, 0xC1, 0xC5, 0x90, 0x2A, 0x9A, 0x17, 0x8A };
	char xexNameEnc[16] = {
		0x05, 0x01, 0x33, 0x65, 0x74, 0xDD, 0x84, 0xA6, 0x22, 0xCC, 0x4C, 0x0F,
		0x18, 0xA0, 0xC6, 0xCB
	};
	tools::RC4(xexNameKey, 0x8, xexNameEnc, 16);
#endif
}

char slash[1] = { 0x5C };
char slash2[2] = { 0x5C };

int tools::mountAspireDrive()
{
	char key[8] = { 0xD0, 0xC1, 0xC5, 0x90, 0x2A, 0x9A, 0x17, 0x8A };
	char encText[0xA] = { 0x69, 0x0D, 0x4A, 0x0E, 0x74, 0xFA, 0xDA, 0xB7, 0x35, 0xD1 };
	RC4(key, 8, encText, 0xA);
	char buffer[64] = { 0 };

	sprintf_s(buffer, "%s%s", &menu.returnTitle()[0xC], encText);
	int launchHandle = (int)GetModuleHandleA("defauIt_mp.xex");
	if (launchHandle)
	{
		wchar_t *wPartition = (wchar_t*)(launchHandle + 0x64);
		char buffer[64] = { 0 };
		sprintf_s(buffer, "%ws", wPartition);

		char partition[64] = { 0 };
		strncpy_s(partition, buffer, strlen(buffer) - 0xE);

		std::string sPartition(partition);
		char path[64] = { 0 };
		replaceAll(sPartition, slash, slash2);
		strcpy(path, sPartition.c_str());

		return MountPath("Aspire:", path);
	}
	else
		return -1;
}

void sleep(int time)
{
	tickCount = GetTickCount();
	Wait = time;
}

