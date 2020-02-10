#include "main.h"

float *func::callRainbow(float speed, int index)
{
	static bool setupShit;
	if (!setupShit)
	{
		for (int i = 0; i < 49; i++)
		{
			rainbowcolor[i][0] = 1.0f;
			rainbowcolor[i][1] = 0.0f;
			rainbowcolor[i][2] = 0.0f;
			rainbowcolor[i][3] = 1.0f;
		}
		setupShit = true;
	}

	if (rainbowcolor[index][0] > 0.0f && rainbowcolor[index][2] <= 0.0f)
	{
		rainbowcolor[index][0] -= speed;
		rainbowcolor[index][1] += speed;
	}
	if (rainbowcolor[index][1] > 0.0f && rainbowcolor[index][0] <= 0.0f)
	{
		rainbowcolor[index][1] -= speed;
		rainbowcolor[index][2] += speed;
	}
	if (rainbowcolor[index][2] > 0.0f && rainbowcolor[index][1] <= 0.0f)
	{
		rainbowcolor[index][0] += speed;
		rainbowcolor[index][2] -= speed;
	}

	return rainbowcolor[index];
}

double func::radians(float Degrees)
{
	return 0.01745329238474369 * Degrees;
}

signed char func::clampChar(int i)
{
	if (i < -128) {
		return -128;
	}
	if (i > 127) {
		return 127;
	}
	return i;
}

bool func::worldToScreen(vector3 &worldLocation, vector2 &Screen)
{
	vector3 Local = vector3();
	vector3 Transform = vector3();

	Local = worldLocation - cg->RefDef.viewOrigin, Transform;
	Transform.x = Local.dotProduct(cg->RefDef.viewAxis[1]);
	Transform.y = Local.dotProduct(cg->RefDef.viewAxis[2]);
	Transform.z = Local.dotProduct(cg->RefDef.viewAxis[0]);

	if (Transform.z < 0.01f)
		return false;

	Screen.x = (vars->width / 2.0f) * (1.0f - (Transform.x / cg->RefDef.tanHalfFov.x / Transform.z));
	Screen.y = (vars->height / 2.0f) * (1.0f - (Transform.y / cg->RefDef.tanHalfFov.y / Transform.z));
	return (Transform.z > 0.0f);
}

bool func::clientWorldToScreen(vector3 clientheadPos, vector3 clientViewAngles, vector3 &out)
{
	vector3 viewAxis[3];
	float clientAngles[] = { 0, clientViewAngles.y, 0 };
	AnglesToAxis(clientViewAngles, viewAxis);
	vector3 shit = clientheadPos - cg->RefDef.viewOrigin;

	float Z = shit.dotProduct(viewAxis[0]);
	float X = shit.dotProduct(viewAxis[1]);
	float Y = shit.dotProduct(viewAxis[2]);

	if (Z >= 0.0f)
	{
		float flTemp = 1.0f / Z;

		out.x = (1.0f - (X / cg->RefDef.tanHalfFov.x * flTemp)) * ((float)cg->RefDef.Width / 2.0f);
		out.y = (1.0f - (Y / cg->RefDef.tanHalfFov.y * flTemp)) * ((float)cg->RefDef.Height / 2.0f);
		return false;
	}
	return true;
}

bool func::getTagPos(int i, short tagName, vector3 &pos)
{
	int DObj = Com_GetClientDObj(i, 0);

	if (!DObj || !tagName)
		return false;

	return CG_DObjGetWorldTagPos((int*)&Entities[i], DObj, tagName, pos);;
}

bool func::inGame()
{
	Entities = (centity_s*)*(int*)addr->centity;
	cg = (cgArray*)*(int*)addr->cg_t;
	ca = (clientActive*)*(int*)addr->clientActive_t;

	if (!cg || !Entities || !ca)
		return false;

#if defined(BO2)
	return (Dvar_GetBool("cl_ingame") && !CL_AllClientsDisconnected());
#elif defined (BO1)
	return (Dvar_GetBool("cl_ingame") && !CL_AllClientsDisconnected() && *(int*)addr->ingame == 10);
#else
	return (Dvar_GetBool("cl_ingame") && !CL_AllClientsDisconnected());
#endif
}

bool func::notTeam(int i)
{
	return (cg->clientInfo_t[cg->clientNumber].Team != cg->clientInfo_t[i].Team || cg->clientInfo_t[i].Team == 0);
}

int func::playerState()
{
#if defined (MW2)
	return (int)cg;
#else
	return CG_GetPredictedPlayerState(0);
#endif
}

vector3 func::getViewOrigin()
{
	vector3 ret = vector3();
	CG_GetPlayerViewOrigin(0, playerState(), ret);
	return ret;
}

int func::getWeaponShader(int weaponIndex)
{
	int weaponDef = 0, additive = 0;
#if defined(BO2)
	additive = 0x660;
	weaponDef = BG_GetWeaponDef(weaponIndex);
#elif defined(BO1)
	weaponDef = BG_GetWeaponDef(weaponIndex);
	additive = 0x578;
#else
	auto sub_820E22D8 = (int(*)(int))0x820E22D8;
	weaponDef = sub_820E22D8(weaponIndex);
	additive = 0x48;
#endif

	if (weaponDef == 0)
		return -1;
	int pointer = *(int*)(weaponDef + additive);
	if (pointer == 0)
		return -1;
	int pointer2 = *(int*)pointer;
	return (int)pointer2;
}

vector3 func::getClientViewAngles(int i)
{
#if defined(BO1)
	return Entities[i].viewAngles;
#elif defined(BO2)
	return Entities[i].viewAngles;
#elif defined(MW2)
	return Entities[i].viewAngles;
#elif defined(MW3)
	return Entities[i].viewAngles;
#endif
}

void func::endGame()
{
	if (inGame())
	{
		char endGameStr[64] = { 0 };
		sprintf_s(endGameStr, ";mr %i -1 endround;", *(int*)addr->serverId);
		Cbuf_AddText(0, endGameStr);
	}
}

int func::hostClientNum()
{
	int hostClientNumber = -1;
	static char host[64] = { 0 };
#if defined (MW2)
	strcpy(host, Dvar_GetString("sv_hostname"));
#elif defined (MW3)
	strcpy(host, (char*)(0x82720D90 + 0x15C));
#else
	strcpy(host, Dvar_GetString("party_hostname"));
#endif

	for (int i = 0; i < 18; i++)
	{
		char Gamertag[40] = { 0 };
		strncpy_s(Gamertag, (char*)(addr->netInfo + (i * addr->netInfoSize) + addr->netInfoGamertag), 32);
		if (!strcmp(host, Gamertag))
		{
			hostClientNumber = i;
			break;
		}
	}
	return hostClientNumber;
}

void func::setClientShield(int client)
{
	if (vars->clientToFace == client)
		vars->clientToFace = -1;
	else
		vars->clientToFace = client;
}

void func::setupEnterCustomTag()
{
	vars->enterCustomTag = true;
}

void func::setGamertag(char *gamertag)
{
#if defined (BO2)
	static bool once = false;
	if (!once)
	{
		once = true;
		memcpy((void*)0x82C55D00, bo2GamertagHook, 32);
		memcpy((void*)0x8293D724, bo2gamertagPatchInJump, 16);
		*(char*)0x8259B6A7 = 0x00;
		*(char*)0x822D1110 = 0x40;
	}
	func::inGame() ? strcpy((char*)0x841E1B30, gamertag) : strcpy((char*)0x82C55D60, gamertag);
#endif
}

#if defined (BO1)
void func::toggleDLC()
{
	static bool toggle = false;
	toggle = !toggle;

	toggle ? Cbuf_AddText(0, ";dlc0 0;dlc1 0;dlc2 0;dlc3 0;dlc4 0;") : Cbuf_AddText(0, ";dlc0 1;dlc1 1;dlc2 1;dlc3 1;dlc4 1;");
}
#endif