#include "main.h"

addresses *addr = { 0 };
variables *vars = { 0 };
AIMBOT *aimbot = { 0 };
Esp *esp = { 0 };

cgsArray *cgs;
centity_s *Entities;
cgArray *cg;
clientActive *ca;
netInfo_ *netInfo;
trace_t trace;
rectDef_s rect;
rectDef_s parentRect;

void(*R_AddCmdDrawText)(const char *text, int maxChars, void *font, float x, float y, float xScale, float yScale, float rotation, const float *color, int style);
void(*R_AddCmdDrawStretchPic)(float x, float y, float w, float h, float s0, float t0, float s1, float t1, const float *color, void *material);
void*(*R_RegisterFont)(const char *font, int imageTrack);
void*(*Material_RegisterHandle)(const char *name, int imageTrack);
int(*Dvar_FindVar)(const char *dvarName);
void(*UI_GetGameTypeOnMapName)(char *outputString, const char *inputMapName, const char *inputGameType);
int(*Com_GetClientDObj)(int handle, int localClientNum);
int(*CG_DObjGetWorldTagPos)(int *pose, int obj, int tagName, vector3 &pos);
int(*GScr_AllocString)(const char *tag);
void(*CG_DrawRotatedPicPhysical)(int screenPlacement, float x, float y, float width, float height, float angle, const float *color, void *material);
bool(*CL_AllClientsDisconnected)();
bool(*CG_GetPlayerViewOrigin)(int localClientNum, int ps, vector3 &origin);
int(*CG_GetPredictedPlayerState)(int localClientNum);
void(*Sys_CreateThread)(void(*function)(unsigned int), unsigned int threadContext);
int(*BG_GetWeaponIndexForName)(const char* name);
void(*CG_DrawRotatedPic)(int scrPlace, float x, float y, float width, float height, int horzAlign, int vertAlign, float angle, const float *color, void *material);
void(*CG_CompassCalcDimensions)(int compassType, int cg, rectDef_s *parentRect, rectDef_s *rect, float *x, float *y, float *w, float *h);
void(*Cbuf_AddText)(int localClientNum, const char *text);
void(*DObjLock)(int dobj);
void(*DObjUnlock)(int dobj);

int R_TextWidth(const char *text, int maxChars, void *font)
{
#if defined(BO2)
	int(*R_TextWidth_BO2)(int localClientNum, const char *text, int maxChars, void *font) = (int(*)(int, const char *, int, void *))addr->R_TextWidth_;
	return R_TextWidth_BO2(0, text, maxChars, font);
#else
	int(*R_TextWidth_)(const char *text, int maxChars, void *font) = (int(*)(const char *, int, void *))addr->R_TextWidth_;
	return R_TextWidth_(text, maxChars, font);
#endif
	return 0;
}

int R_TextHeight(void *font)
{
	return *(int*)((int)font + 4);
}

float Dvar_GetFloat(char *dvarName)
{
	int addAmount = 0;

#if defined(BO2)
	addAmount = 0x18;
	int dvar_t = Dvar_FindVar(dvarName);
	if (dvar_t == 0)
		return 0.0f;
	return *(float*)(dvar_t + addAmount);
#elif defined(BO1)
	addAmount = 0x18;
	int dvar_t = Dvar_FindVar(dvarName);
	if (dvar_t == 0)
		return 0.0f;
	return *(float*)(dvar_t + addAmount);
#else
	auto Dvar_GetFloat_ = (float(*)(const char *dvarName))addr->Dvar_GetFloat_;
	return Dvar_GetFloat_(dvarName);
#endif
}

char *Dvar_GetString(char *dvarName)
{
	int addAmount = 0;

#if defined(BO2)
	addAmount = 0x18;
	int dvar_t = Dvar_FindVar(dvarName);
	if (dvar_t == 0)
		return "";
	return (char*)(*(int*)(dvar_t + addAmount));
#elif defined(BO1)
	addAmount = 0x18;
	int dvar_t = Dvar_FindVar(dvarName);
	if (dvar_t == 0)
		return "";
	return (char*)(*(int*)(dvar_t + addAmount));
#else
	auto Dvar_GetString_ = (char*(*)(const char *dvarName))addr->Dvar_GetString_;
	return Dvar_GetString_(dvarName);
#endif
}

int Dvar_GetInt(char *dvarName)
{
	int addAmount = 0;

#if defined(BO2)
	addAmount = 0x18;
	int dvar_t = Dvar_FindVar(dvarName);
	if (dvar_t == 0)
		return 0;
	return *(int*)(dvar_t + addAmount);
#elif defined(BO1)
	addAmount = 0x18;
	int dvar_t = Dvar_FindVar(dvarName);
	if (dvar_t == 0)
		return 0;
	return *(int*)(dvar_t + addAmount);
#else
	auto Dvar_GetInt_ = (int(*)(const char *dvarName))addr->Dvar_GetInt_;
	return Dvar_GetInt_(dvarName);
#endif
}

bool Dvar_GetBool(char *dvarName)
{
	int addAmount = 0;

#if defined(BO2)
	addAmount = 0x18;
	int dvar_t = Dvar_FindVar(dvarName);
	if (dvar_t == 0)
		return false;
	return *(bool*)(dvar_t + addAmount);
#elif defined(BO1)
	addAmount = 0x18;
	int dvar_t = Dvar_FindVar(dvarName);
	if (dvar_t == 0)
		return false;
	return *(bool*)(dvar_t + addAmount);
#else
	auto Dvar_GetBool_ = (int(*)(const char *dvarName))addr->Dvar_GetBool_;
	return Dvar_GetBool_(dvarName);
#endif
}

int BG_GetWeaponDef(int weaponId)
{
	int(*BG_GetWeaponDef_)(int weaponId) = (int(*)(int))addr->BG_GetWeaponDef_;
	return BG_GetWeaponDef_(weaponId);
}

void BG_GetSpreadForWeapon(int cgPtr, int weapon, float *minSpread, float *maxSpread)
{
#if defined(BO1)
	auto BG_GetSpreadForWeapon = (void(*)(int, int, float *, float *))addr->BG_GetSpreadForWeapon_;
	BG_GetSpreadForWeapon(cgPtr, BG_GetWeaponDef(weapon), minSpread, maxSpread);
#elif defined (MW2)
	auto BG_GetSpreadForWeapon = (void(*)(int, int, float *, float *))addr->BG_GetSpreadForWeapon_;
	BG_GetSpreadForWeapon(cgPtr, BG_GetWeaponDef(weapon), minSpread, maxSpread);
#else
	auto BG_GetSpreadForWeapon = (void(*)(int, int, float *, float *))addr->BG_GetSpreadForWeapon_;
	BG_GetSpreadForWeapon(cgPtr, weapon, minSpread, maxSpread);
#endif
}