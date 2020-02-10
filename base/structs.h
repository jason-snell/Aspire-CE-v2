#pragma once

struct geoLocationSend
{
	char packetId;
	unsigned char IP[4];
	char Gamertag[32];
	char XUID[8];
}; // Size = 0x2D

struct geoLocationRecv
{
	char packetId; // 0x00
	char Country[64]; // 0x01
	char State[64]; // 0x41
	char City[64]; // 0x81
	char ZipCode[16]; // 0xC1
	char ISP[64]; // 0xD1
	bool Mobile; // 0x111
	bool Proxy; // 0x112
}; // Size = 0x113

#if defined(BO1)
struct CompassDogs
{
	int entityNum; //0x0000
	int lastUpdate; //0x0004
	vector2 lastPos; //0x0008
	float lastYaw; //0x0010
	int team; //0x0014
	int ownerIndex; //0x0018
	bool enemy; //0x001C
	char padding[3]; //0x001D
};//Size=0x0020
#endif

struct centity_s
{
#if defined(BO1)
	char _0x0000[2];
	char Valid; //0x0002 
	char _0x0003[45];
	vector3 Origin; //0x0030 
	vector3 viewAngles; // 0x3C
	char _0x003C[288];
	int Flags; //0x0168 
	char _0x016C[16];
	vector3 lastOrigin; //0x017C 
	char _0x0188[84];
	int clientNumber; //0x01DC 
	char _0x01E0[20];
	vector3 predictedOrigin; //0x01F4 
	char _0x0200[116];
	int groundWeapons; //0x0274 
	char _0x0278[34];
	short eType; //0x029A 
	char _0x029B[8];
	int WeaponID; //0x02A4 
	char _0x02A8[112];
	char Alive; //0x0318 
	char _0x0319[3];

	int groundWeaponId()
	{
		return groundWeapons % 2048;
	}

	bool isAlive()
	{
		if (Alive & 64)
			return true;

		return false;
	}//Size=0x031C
#elif defined(BO2)
	char _0x0000[2];
	char Valid; //0x0002 
	char _0x0003[41];
	vector3 Origin; //0x002C 
	vector3 viewAngles; 
	char _0x0038[156];
	char AliveZM; //0x00E0 
	char _0x00E1[115];
	int Flags; //0x0154 
	char _0x0158[16];
	vector3 lastOrigin; //0x0168 
	char _0x0174[92];
	int clientNumber; //0x01D0 
	char _0x01D4[96];
	int WeaponID2; //0x0234 
	char _0x0238[112];
	short eType; //0x02A8 ;
	char modelIndex; //0x02AC 
	char _0x02AE[6];
	int WeaponID; //0x02B4 
	char _0x02B8[180];
	char Alive; //0x036C 
	char _0x036D[7];

	bool isAlive()
	{
		return (Alive & 64 || AliveZM == 64);
	}

	int groundWeaponId()
	{
		return WeaponID;
	}
#elif defined(MW2)
	char _0x0000[2];
	char Valid; //0x0002 
	char _0x0003[21];
	vector3 Origin; //0x0018 
	vector3 viewAngles; // 0x0024
	char _0x0024[72];
	vector3 lastOrigin; //0x0078 
	char _0x0084[80];
	int eType; //0x00D4 
	char _0x00D8[4];
	int Flags; //0x00DC 
	char _0x00E0[120];
	int scavWeaponID; //0x0158 
	char _0x015C[8];
	int clientNumber; //0x0164 
	char _0x0168[52];
	short WeaponID; //0x019C 
	char _0x019E[50];
	int Alive; //0x01D0 
	char _0x01D4[36];

	bool isAlive()
	{
		return (Alive & 1 || Alive == 1);
	}

	int groundWeaponId()
	{
		return scavWeaponID;
	}
	//Size=0x01F8
#elif defined (MW3)
	char _0x0000[2];
	byte Valid; //0x0002 
	char _0x0003[17];
	vector3 Origin; //0x0014 
	vector3 viewAngles; //0x0020 
	char _0x002C[100];
	vector3 viewAngles2; //0x0090 
	char _0x009C[28];
	int secondaryWeaponID2; //0x00B8 
	char _0x00BC[8];
	int clientNumber; //0x00C4 
	int eType; //0x00C8 
	int Flags; //0x00CC
	char _0x00CC[88];
	int secondaryWeaponID; //0x0128 
	char _0x012C[96];
	int WeaponID; //0x018C 
	char _0x0190[52];
	int Alive; //0x01C4 
	char _0x01C8[36];

	bool isAlive()
	{
		return (Alive & 1);
	}//Size=0x01EC
#endif
};

struct refDef
{
#if defined(BO1)
	int Width; //0x0000 
	int Height; //0x0004 
	char _0x0008[20];
	vector2 tanHalfFov; //0x001C 
	char _0x0024[4];
	vector3 viewOrigin; //0x0028 
	char _0x0034[4];
	vector3 viewAxis[3]; //0x0038 
	char _0x005C[4];
#elif defined(BO2)
	int Width; //0x0000 
	int Height; //0x0004 
	char _0x0008[20];
	vector2 tanHalfFov; //0x001C 
	char _0x0024[12];
	vector3 viewOrigin; //0x0030 
	char _0x003C[4];
	vector3 viewAxis[3]; //0x0040 
	char _0x0064[8];
#elif defined(MW2)
	int Width; //0x0000 
	int Height; //0x0004 
	vector2 tanHalfFov; //0x0008 
	vector3 viewOrigin; //0x0010 
	vector3 viewAxis[3]; //0x001C
	char _0x0040[4];
	//Size=0x006C
#elif defined (MW3)
	int Width; //0x0000 
	int Height; //0x0004 
	vector2 tanHalfFov; //0x0008 
	vector3 viewOrigin; //0x0010 
	vector3 viewAxis[3]; //0x001C 
	//Size=0x0044
#endif
};

#if defined(BO1)
struct score_s
{
	int ping;
	int status_icon;
	int place;
	int score;
	int kills;
	int assists;
	int deaths;
	int scoreboardColumns[4];
};
struct lerpFrame_t
{
	float yawAngle;
	int yawing;
	float pitchAngle;
	int pitching;
	int animationNumber;
	int *animation;
	int animationTime;
	float oldFramePos[3];
	float oldFrameYaw;
	float animSpeedScale;
	int oldFrameSnapshotTime;
};
struct clientControllers_t
{
	float angles[6][3];
	float tag_origin_angles[3];
	float tag_origin_offset[3];
};
#endif

struct clientInfo
{
#if defined(BO1)
	int infoValid; //0x0000 
	int hextValid; //0x0004 
	char _0x0008[4];
	char clientName[32]; //0x000C 
	int Team; //0x002C 
	char _0x0030[8];
	int Ping; //0x0038 
	char _0x003C[28];
	char clanAbbrev[4]; //0x0058 
	char pad[1010];
	vector3 viewAngles; //0x0450 
	char _0x045C[372];
#elif defined(BO2)
	int infoValid; //0x0000 
	int nextValid; //0x0004 
	int clientNumber; //0x0008 
	char clientName[32]; //0x000C
	int Team; //0x002C
	char _0x002C[48];
	int Rank; //0x0060 
	int Prestige; //0x0064 
	char _0x0068[24];
	char clanAbbrev[8]; //0x0080 
	int Ping; //0x0088 
	int status_icon; //0x008C 
	int Place; //0x0090 
	int Score; //0x0094 
	int Kills; //0x0098 
	int Assists; //0x009C 
	int Deaths; //0x00A0 
	char _0x00A4[32];
	int Headshots; //0x00C4 
	char _0x00C8[4];
	int currentStreak; //0x00CC 
	char _0x00D0[80];
	char SecondaryWeapon[32]; //0x0120 
	char _0x0140[900];
	vector3 viewAngles; //0x04C4 
	char _0x04D0[192];
	int aimingIn; //0x0590 
	char _0x0594[628];
#elif defined(MW2)
	int infoValid; //0x0000 
	int nextValid; //0x0004 
	int clientNumber; //0x0008 
	char clientName[32]; //0x000C 
	int Team; //0x002C 
	char _0x0030[20];
	char clanAbbrev[8]; //0x0044 
	char _0x004C[140];
	char secondaryWeaponModel[32]; //0x00D8 
	char _0x00F8[800];
	vector3 viewAngles; //0x0418 
	char _0x0424[288];
	//Size=0x0544
#elif defined (MW3)
	int infoValid; //0x0000 
	int nextValid; //0x0004 
	int clientNumber; //0x0008 
	char clientName[32]; //0x000C 
	int Team; //0x002C 
	char _0x0030[4];
	int rank; //0x0034 
	int prestige; //0x0038 
	char _0x003C[16];
	char clanAbbrev[8]; //0x004C 
	int Score; //0x0054 
	char _0x0058[8];
	char modelName[32]; //0x0060 
	char _0x0080[96];
	char weaponModel1[32]; //0x00E0 
	char _0x0100[352];
	char weaponModel2[32]; //0x0260 
	char _0x0280[544];
	int Flags; //0x04A0 
	char _0x04A4[120];
	int WeaponID; //0x051C 
	char _0x0520[116];
	//Size=0x0594
#endif
};

#if defined(BO2)
struct snapshot_s
{
	int snapFlags; //0x0000 
	int Ping; //0x0004 
	int serverTime; //0x0008 
	int physicsTime; //0x000C 
	char _0x0010[48];
};//Size=0x0040
#endif

struct cgArray
{
#if defined(BO1)
	char _0x0000[180];
	vector3 Origin; //0x00B4 
	char _0x00C0[176];
	int Flags; //0x0170 
	char _0x0174[76];
	char clientNumber; //0x01C0 
	char _0x01C1[147];
	int Health; //0x0254
	char _0x0258[4];
	int maxHealth; //0x025C 
	char _0x0260[268200];
	refDef RefDef; //0x41A08 
	char padding[0x1BE78]; //0x41A68 
	float compassNorthYaw; //0x5D8E0
	vector2 compassNorth; //0x5D8E4
	int *compassMapMaterial; //0x5D8EC
	vector2 compassMapUpperLeft; //0x5D8F0
	vector2 compassMapWorldSize; //0x5D8F8
	char padding2[0x1F8];
	clientInfo clientInfo_t[18]; //0x5DAF8 
	char _0x5E0C8[116];
#elif defined(BO2)
	char _0x0000[4];
	int localClientNumber; //0x0004 
	char _0x0008[32];
	snapshot_s *snap; //0x0028 
	char _0x002C[140];
	vector3 Origin; //0x00B8 
	char _0x00C4[364];
	int clientNumber; // 0x0230
	char _0x0234[148];
	int Health; //0x02C8 
	char _0x02CC[4];
	int maxHealth; //0x02D0
	char _padding3[836];
	float SpreadMultiplyer;
	char padding[316012];
	refDef RefDef; //0x4D888
	char padding2[114548]; //0x0000 
	float compassNorthYaw; //0x1BF74 
	vector2 compassNorth; //0x1BF78 
	int *compassMapMaterial; //0x1BF80 
	int *compassMapScrambledMaterial; //0x1BF84 
	vector2 compassMapUpperLeft; //0x1BF88 
	vector2 compassMapWorldSize; //0x1BF90 
	char _0x1BF98[516]; //0x1BF98
	clientInfo clientInfo_t[18]; //0x69A90
	char _0x6A298[60];
#elif defined(MW2)
	char _0x0000[28];
	vector3 Origin; //0x001C 
	char _0x0028[220];
	int clientNumber; //0x0104 
	char _0x0108[72];
	int Health; //0x0150 
	char _0x0154[4];
	int maxHealth; //0x0158 
	char _0x015C[360];
	float aimSpreadScale; //0x02C4 
	char _0x02C8[140];
	int secondaryAmmo; //0x0354 
	char _0x0358[20];
	int primaryAmmo; //0x036C 
	char padding[436416]; //0x0370 
	refDef RefDef; //0x6AC30 
	char clientInfoPadding[0xF6E80 - (sizeof(refDef) + 0x6AC30)]; //0x6AC74 
	clientInfo clientInfo_t[18]; //0xF6E80 
	char _0xF72AC[60];
	//Size=0xF72F8
#elif defined (MW3)
	char _0x0000[4];
	int localClientNumber; //0x0004 
	char _0x0008[20];
	vector3 Origin; //0x001C 
	char _0x0028[296];
	int clientNumber; //0x0150 
	char _0x0154[72];
	int Health; //0x019C 
	char _0x01A0[4];
	int maxHealth; //0x01A4 
	char _0x01A8[468];
	float aimSpreadScale; //0x037C 
	char padding[0x6B734];
	refDef RefDef; //0x4D888 
	char padding1[0x92494]; // 0x4D8C2
	clientInfo clientInfo_t[18]; // 0xFDF88
	//Size=0x6A2D4
#endif
};//Size=0x6A2D4

struct cgsArray
{
#if defined(BO1)
	int viewX; //0x0000 
	int viewY; //0x0004 
	int viewWidth; //0x0008 
	int viewHeight; //0x000C 
	char _0x0010[28];
	int localServer; //0x002C 
	char Gametype[32]; //0x0030 
	char Gamertag[256]; //0x0050 
	int maxClients; //0x0150 
	int privateClients; //0x0154 
	char mapName[64]; //0x0158 
#elif defined(BO2)
	int viewX; //0x0000 
	int viewY; //0x0004 
	int viewWidth; //0x0008 
	int viewHeight; //0x000C 
	char _0x0010[28];
	int localServer; //0x002C 
	char Gametype[32]; //0x0030 
	char Gamertag[256]; //0x0050 
	int maxClients; //0x0150 
	int privateClients; //0x0154 
	char mapName[64]; //0x0158 
#endif
};//Size=0x0198

struct usercmd_s
{
#if defined(BO1)
	int serverTime;
	int buttons;
	int unk;
	int ViewAngles[3];
	byte weapon;
	byte unk1;
	short offHandIndex;
	short lastWeaponAltModeSwitch;
	signed char MoveButtonX;
	signed char MoveButtonY;
	char upmove;
	char pitchmove;
	char yawmove;
	char padding1;
	float meleeChargeYaw;
	char meleeChargeDist;
	char padding2[3];
	float rollmove;
	char selectedLocation[2];
	char selectedYaw;
	char padding3;
#elif defined(BO2)
	int serverTime; //0x0000
	int buttons; //0x0004
	int unk; //0x0008
	int ViewAngles[3]; // 0x000C
	int weapon; //0x0018
	int offHandIndex; //0x001C
	int lastWeaponAltModeSwitch;
	signed char MoveButtonX;
	signed char MoveButtonY;
	char upmove;
	char pitchmove;
	char yawmove;
	char padding1;
	short faggot;
	float meleeChargeYaw;
	char meleeChargeDist;
	char padding2[3];
	float rollmove;
	char selectedLocation[2];
	char selectedYaw;
	char padding3;
#elif defined(MW2)
	int serverTime;
	int buttons;
	int ViewAngles[3];
	short weapon;
	char padding[0x4];
	signed char MoveButtonX;
	signed char MoveButtonY;
	char padding2[0xC];
	/*Size 0x28*/
#elif defined(MW3)
	int serverTime; //0x0
	int buttons; //0x4
	int ViewAngles[3]; // 0x8
	short weapon; //0x1C
	char padding[6]; //0x16
	signed char MoveButtonX; //0x1C
	signed char MoveButtonY; // 0x1D
	char padding2[0xC]; // 0x27
	/*Size 0x3C*/
#endif
};

struct clientActive
{
#if defined(BO1)
	char _0x0000[76];
	vector3 Origin; //0x004C 
	char _0x0058[84];
	vector3 spawnAngles; //0x00AC 
	char _0x00B8[10120];
	vector3 viewAngles; //0x2840
	char _padding[0x40010];
	usercmd_s cmds[128]; // 0x4285C
	int cmdNumber;

	usercmd_s * GetCmd(int Index)
	{
		return &cmds[(Index & 0x7F)];
	}
#elif defined(BO2)
	char _0x0000[152];
	vector3 Origin; // 0x0098
	char __0x00A4[100];
	vector3 spawnAngles; // 0x0108
	char _0x0114[11128];
	vector3 viewAngles; // 0x2C8C
	char _0x02C9[0x40010];
	usercmd_s cmds[128];
	int cmdNumber;

	usercmd_s * GetCmd(int Index)
	{
		return &cmds[(Index & 0x7F)];
	}
#elif defined(MW2)
	char _0x0000[156];
	vector3 Origin; //0x004C 
	char _0x0058[56];
	vector3 spawnAngles; //0x00AC 
	char _0x00B8[12908];
	vector3 viewAngles; //0x2840
						//char _padding[0x40010];
	usercmd_s cmds[128]; // 0x4285C
	int cmdNumber;

	usercmd_s * GetCmd(int Index)
	{
		return &cmds[(Index & 0x7F)];
	}
#elif defined (MW3)
	char _padding[128];
	int serverTime; // 0x0080
	char _padding2[24];
	vector3 origin; // 0x009C
	char _padding3[56];
	vector3 spawnAngles; // 0x00EC
	char _padding4[13508];
	vector3 viewAngles; // 0x35BC
	usercmd_s cmd[128];
	int cmdNumber;

	usercmd_s* GetCmd(int id) {
		return &cmd[id & 0x7F];
	}
#endif
};

struct BulletFireParams
{
#if defined(BO1)
	int weaponEntIndex;
	int ignoreEntIndex;
	float damageMultiplier;
	int methodOfDeath;
	vector3 origStart;
	vector3 start;
	vector3 end;
	vector3 dir;
#elif defined(BO2)
	int weaponEntIndex;
	int ignoreEntIndex;
	float damageMultiplier;
	int methodOfDeath;
	vector3 origStart;
	vector3 start;
	vector3 end;
	vector3 dir;
#elif defined(MW2)
	int weaponEntIndex;
	int ignoreEntIndex;
	float damageMultiplier;
	int methodOfDeath;
	vector3 origStart;
	vector3 start;
	vector3 end;
	vector3 dir;
#elif defined (MW3)
	int weaponEntIndex;
	int ignoreEntIndex;
	float damageMultiplier;
	int methodOfDeath;
	bool noRicochet;
	vector3 origStart;
	vector3 start;
	vector3 end;
	vector3 dir;
#endif
};

struct trace_t
{
#if defined (MW2)
	float fraction; // 0x0000
	char padding[0x28];
#elif defined(MW3)
	float fraction;
	char padding[0x22];
	short partGroup;
	char padding2[0x39];
#else
	float normal[4];
	float fraction;
	int sflags;
	int cflags;
	int hitType;
	unsigned __int16 hitId;
	unsigned __int16 modelIndex;
	unsigned __int16 partName;
	unsigned __int16 boneIndex;
	unsigned __int16 partGroup;
	bool allsolid;
	bool startsolid;
	bool walkable;
	int *staticModel;
	int hitPartition;
#endif
};

struct BulletTraceResults
{
	trace_t trace;
	char padding[0xC];
	int hitEnt;
	vector3 hitPos;
	bool ignoreHitEnt;
	int surfaceType;
	char padding2[8];
};

struct netInfo_
{
#if defined(BO1)
	unsigned long long XUID; //0x0000 
	char clientName[32]; //0x0008 
	char clantag[4]; //0x0028 
	char _0x002C[52];
	unsigned char IP[4]; //0x0060 
	char _0x0064[412];
#endif
#if defined(BO2)
	char _0x0000[56];
	long long XUID; //0x0038 
	char clientName[32]; //0x0040 
	char _0x0060[44];
	char clantag[8]; //0x008C 
	char _0x0094[32];
	unsigned char IP[4]; //0x00B4 
	short PORT;
	char _0x00B8[0xE];
	long long machineID; //0x00C8 
	char _0x00D0[116];
#endif
};//Size=0x0200

struct rectDef_s
{
	float x;
	float y;
	float w;
	float h;
	int horzAlign;
	int vertAlign;
};

extern void(*R_AddCmdDrawText)(const char *text, int maxChars, void *font, float x, float y, float xScale, float yScale, float rotation, const float *color, int style);
extern void(*R_AddCmdDrawStretchPic)(float x, float y, float w, float h, float s0, float t0, float s1, float t1, const float *color, void *material);
extern void*(*R_RegisterFont)(const char *font, int imageTrack);
extern void*(*Material_RegisterHandle)(const char *name, int imageTrack);
extern int(*Dvar_FindVar)(const char *dvarName);
extern void(*UI_GetGameTypeOnMapName)(char *outputString, const char *inputMapName, const char *inputGameType);
extern int(*Com_GetClientDObj)(int handle, int localClientNum);
extern int(*CG_DObjGetWorldTagPos)(int *pose, int obj, int tagName, vector3 &pos);
extern int(*GScr_AllocString)(const char *tag);
extern void(*CG_DrawRotatedPicPhysical)(int screenPlacement, float x, float y, float width, float height, float angle, const float *color, void *material);
extern bool(*CL_AllClientsDisconnected)();
extern bool(*CG_GetPlayerViewOrigin)(int localClientNum, int ps, vector3 &origin);
extern int(*CG_GetPredictedPlayerState)(int localClientNum);
extern void(*Sys_CreateThread)(void(*function)(unsigned int), unsigned int threadContext);
extern int(*BG_GetWeaponIndexForName)(const char* name);
extern void(*CG_DrawRotatedPic)(int scrPlace, float x, float y, float width, float height, int horzAlign, int vertAlign, float angle, const float *color, void *material);
extern void(*CG_CompassCalcDimensions)(int compassType, int cg, rectDef_s *parentRect, rectDef_s *rect, float *x, float *y, float *w, float *h);
extern void(*Cbuf_AddText)(int localClientNum, const char *text);
extern void(*DObjLock)(int dobj);
extern void(*DObjUnlock)(int dobj);
int R_TextWidth(const char *text, int maxChars, void *font);
int R_TextHeight(void *font);
float Dvar_GetFloat(char *dvarName);
char *Dvar_GetString(char *dvarName);
int Dvar_GetInt(char *dvarName);
bool Dvar_GetBool(char *dvarName);
int BG_GetWeaponDef(int weaponId);
void BG_GetSpreadForWeapon(int cgPtr, int weapon, float *minSpread, float *maxSpread);

struct addresses
{
	// Hooks
	int R_EndFrame, CL_SendCmd, XamInputGetState, BG_AdvanceTrace, NetDll_XNetGetTitleXnAddr;
	// End Hooks

	// Functions
	int R_AddCmdDrawText_, R_AddCmdDrawStretchPic_, R_RegisterFont_, Material_RegisterHandle_, R_TextWidth_, CL_GetScreenDimensions_, Dvar_GetString_, UI_MouseEvent_,
		CG_DrawRotatedPicPhysical_, CG_GoodRandomFloat_, BG_GetWeaponDef_, Dvar_GetBool_, Cbuf_AddText_, Com_GetClientDObj_, CG_DObjGetWorldTagPos_, GScr_AllocString_,
		CG_GetPlayerViewOrigin_, CG_LocationalTrace_, BG_GetSpreadForWeapon_, BG_GetWeaponIndexForName_, BG_GetWeaponCompleteDef_, UI_GetGameTypeDisplayName_, UI_GetMapDisplayName_,
		FireBulletPenetrate_, AimTarget_IsTargetVisible_, Dvar_FindVar_, UI_GetGameTypeOnMapName_, CL_AllClientsDisconnected_, Dvar_GetInt_, Dvar_GetFloat_, CG_CanSeeFriendlyHeadTrace_,
		CG_GetPredictedPlayerState_, DObjPhysicsGetBounds_;
	// End Functions

	// Other
	int screenWidth, screenHeight, screenplacement, ingame, drawBulletImpactsPatch, DynEntCl_DynEntImpactEventPatch, FPS, netInfoSize, netInfoGamertag, netInfoIP, netInfoXUID, FOV, dvarAdditive,
		noSway, noRecoil, serverId, autowallPatch[10], antiFreeze[5];
	// End Other

	// Structs
	int centity, clientInfo_t, refDef_s, cg_t, clientActive_t, netInfo;
	// End Structs

	addresses()
	{
#if defined(BO1)
		this->R_EndFrame = 0x82234B00;
		this->CL_SendCmd = 0x8223B8C8;
		this->XamInputGetState = 0x826CF558;

		this->R_AddCmdDrawText_ = 0x8263EF00;
		this->R_AddCmdDrawStretchPic_ = 0x8263EA68;
		this->R_RegisterFont_ = 0x82243FA0;
		this->Material_RegisterHandle_ = 0x82624B78;
		this->R_TextWidth_ = 0x8263D378;
		this->CG_DrawRotatedPicPhysical_ = 0x821B86A8;
		this->CG_GoodRandomFloat_ = 0x822AC660;
		this->BG_GetWeaponDef_ = 0x821919D8;
		this->Cbuf_AddText_ = 0x8233E8D8;
		this->Com_GetClientDObj_ = 0x82346710;
		this->CG_DObjGetWorldTagPos_ = 0x821BDB10;
		this->GScr_AllocString_ = 0x822E1B58;
		this->CG_GetPlayerViewOrigin_ = 0x82223E48;
		this->CG_LocationalTrace_ = 0x822296E0;
		this->BG_GetSpreadForWeapon_ = 0x82186940;
		this->BG_GetWeaponIndexForName_ = 0x82191F50;
		this->Dvar_FindVar_ = 0x823E2818;
		this->UI_GetGameTypeOnMapName_ = 0x823986D8;
		this->CL_AllClientsDisconnected_ = 0x8255E2D0;
		this->CG_CanSeeFriendlyHeadTrace_ = 0x821B50A8;
		this->FireBulletPenetrate_ = 0x822252F8;
		this->CG_GetPredictedPlayerState_ = 0x821CD100;
		this->DObjPhysicsGetBounds_ = 0x823F1C68;

		this->ingame = 0x829D5588;
		this->screenWidth = 0x829BE2B4;
		this->screenHeight = 0x829BE2B8;
		this->screenplacement = 0x82A06718;
		this->FPS = 0x83911158 + 0x28;
		this->netInfoGamertag = 0x8;
		this->netInfoIP = 0x60;
		this->netInfoXUID = 0x0;
		this->netInfoSize = 0x200;
		this->FOV = 0x82968374;
		this->dvarAdditive = 0x18;
		this->noRecoil = 0x82227624;
		this->noSway = 0x82195200;
		this->serverId = 0x8233E8D8;
		this->autowallPatch[0] = 0x82208248;
		this->autowallPatch[1] = 0x82223DA8;
		this->autowallPatch[2] = 0x82291FC0;
		this->autowallPatch[3] = 0x82292230;
		this->autowallPatch[4] = 0x82225C5C;
		//this->ingame = 0xBC43C0;
		//this->drawBulletImpactsPatch = 0x4B88F0;
		//this->DynEntCl_DynEntImpactEventPatch = 0x4F6B00;

		this->centity = 0x8296808C;
		this->cg_t = 0x82964EA0;
		this->clientActive_t = 0x829D9728;
		this->netInfo = 0x829F8450;
#elif defined(BO2)
		/*this->R_EndFrame = 0x828B9F58;
		this->CL_SendCmd = 0x8225EAA8;
		this->XamInputGetState = 0x8293D884;
		this->BG_AdvanceTrace = 0x826B0148;
		this->NetDll_XNetGetTitleXnAddr = 0x8293E5C4;

		this->R_AddCmdDrawText_ = 0x828B8BA0;
		this->R_AddCmdDrawStretchPic_ = 0x828B86C0;
		this->R_RegisterFont_ = 0x82275F78;
		this->Material_RegisterHandle_ = 0x828B78F0;
		this->R_TextWidth_ = 0x828B6FD8;
		this->CG_DrawRotatedPicPhysical_ = 0x821C7F58;
		this->BG_GetWeaponDef_ = 0x826BF988;
		this->Cbuf_AddText_ = 0x824015E0;
		this->Com_GetClientDObj_ = 0x82414578;
		this->CG_DObjGetWorldTagPos_ = 0x821D03F0;
		this->GScr_AllocString_ = 0x823599E0;
		this->CG_GetPlayerViewOrigin_ = 0x822544B0;
		this->CG_LocationalTrace_ = 0x8225C568;
		this->BG_GetSpreadForWeapon_ = 0x826BB4E0;
		this->BG_GetWeaponIndexForName_ = 0x826C0598;
		this->Dvar_FindVar_ = 0x82496430;
		this->UI_GetGameTypeOnMapName_ = 0x82459C80;
		this->CL_AllClientsDisconnected_ = 0x827504D0;
		this->FireBulletPenetrate_ = 0x82258840;
		this->CG_CanSeeFriendlyHeadTrace_ = 0x821C45B8;
		this->CG_GetPredictedPlayerState_ = 0x821E64E0;
		this->DObjPhysicsGetBounds_ = 0x824CBFE0;

		this->ingame = 0x82C6FDD0;
		this->screenWidth = 0x82C153E8;
		this->screenHeight = 0x82C153EC;
		this->screenplacement = 0x82CBC168;
		this->FPS = 0x83BA2A18;
		this->netInfoGamertag = 0x40;
		this->netInfoIP = 0xB4;
		this->netInfoXUID = 0x38;
		this->netInfoSize = 0x148;
		this->FOV = 0x82BC2774;
		this->dvarAdditive = 0x18;
		this->noRecoil = 0x82259BC8;
		this->noSway = 0x826C6E6C;
		this->serverId = 0x82C15758;
		this->autowallPatch[0] = 0x822DFB90;
		this->autowallPatch[1] = 0x82258520;
		this->autowallPatch[2] = 0x82257E30;
		this->autowallPatch[3] = 0x82258CE4;
		this->autowallPatch[4] = 0x82258FAC;
		this->autowallPatch[5] = 0x8225900C;
		this->autowallPatch[6] = 0x82258D60;
		this->autowallPatch[7] = 0x82259B40;
		this->antiFreeze[0] = 0x82717AE0;
		this->antiFreeze[1] = 0x822DDE20;
		this->antiFreeze[2] = 0x827D33F0;

		this->centity = 0x82BBC554;
		this->cg_t = 0x82BBAE68;
		this->clientActive_t = 0x82C70F4C;
		this->netInfo = 0xC403C368;*/
#elif defined(MW2)
		this->R_EndFrame = 0x82351748;
		this->CL_SendCmd = 0x821406E8;
		this->XamInputGetState = 0x823B64C4;

		this->R_AddCmdDrawText_ = 0x82350278;
		this->R_AddCmdDrawStretchPic_ = 0x8234F9B8;
		this->R_RegisterFont_ = 0x82146458;
		this->Material_RegisterHandle_ = 0x8234E510;
		this->R_TextWidth_ = 0x8234DD20;
		this->CG_DrawRotatedPicPhysical_ = 0x820F7900;
		this->CG_GoodRandomFloat_ = 0x820D76F0;
		this->BG_GetWeaponDef_ = 0x820E22C0;
		this->Cbuf_AddText_ = 0x82224990;
		this->Com_GetClientDObj_ = 0x8222FCC0;
		this->CG_DObjGetWorldTagPos_ = 0x820FB9E8;
		this->GScr_AllocString_ = 0x821F2A78;
		this->CG_GetPlayerViewOrigin_ = 0x8212E670;
		this->CG_LocationalTrace_ = 0x82136E88;
		this->BG_GetSpreadForWeapon_ = 0x820E33F0;
		this->BG_GetWeaponIndexForName_ = 0x820E29C8;
		this->UI_GetMapDisplayName_ = 0x822729A0;
		this->UI_GetGameTypeDisplayName_ = 0x82272BD0;
		this->CL_AllClientsDisconnected_ = 0x82165CF0;
		this->Dvar_GetBool_ = 0x8229EEE8;
		this->Dvar_GetInt_ = 0x8229EF50;
		this->Dvar_GetString_ = 0x8229F0A8;
		this->Dvar_GetFloat_ = 0x8229EFB8;
		this->CG_GoodRandomFloat_ = 0x820D76F0;
		this->DObjPhysicsGetBounds_ = 0x822AD570;
		this->FireBulletPenetrate_ = 0x82134648;

		this->screenWidth = 0x825E3068;
		this->screenHeight = 0x825E306C;
		this->screenplacement = 0x82690F40;
		this->FPS = 0x8364788C;
		this->netInfoGamertag = 0x6;
		this->netInfoIP = 0x48;
		this->netInfoXUID = 0xB4;
		this->netInfoSize = 0xC0;
		this->FOV = 0x825BE8C8;
		this->dvarAdditive = 0x8;
		this->serverId = 0x826237E0;
		this->noRecoil = 0x82135BFC;
		this->noSway = 0x820E5B38;
		this->autowallPatch[0] = 0x8213484C;
		this->autowallPatch[1] = 0x82134B14;
		this->autowallPatch[2] = 0x82134CCC;
		this->autowallPatch[3] = 0x82134D1C;
		this->autowallPatch[4] = 0x821A4A50;
		this->autowallPatch[5] = 0x8212EB48;
		this->serverId = 0x826237E0;
		//this->ingame = 0xBC43C0;
		//this->drawBulletImpactsPatch = 0x4B88F0;
		//this->DynEntCl_DynEntImpactEventPatch = 0x4F6B00;

		this->centity = 0x825BE150;
		this->cg_t = 0x825B8638;
		this->clientActive_t = 0x82682350;
		this->netInfo = 0x8268EEB4;
#elif defined(MW3)
		this->R_EndFrame = 0x82421048;
		this->CL_SendCmd = 0x8216DF18;
		this->XamInputGetState = 0x8257FF4C;

		this->R_AddCmdDrawText_ = 0x8241F9E0;
		this->R_AddCmdDrawStretchPic_ = 0x8241F100;
		this->R_RegisterFont_ = 0x82174F88;
		this->Material_RegisterHandle_ = 0x82413F48;
		this->R_TextWidth_ = 0x82407C78;
		this->CG_DrawRotatedPicPhysical_ = 0x82117668;
		this->CG_GoodRandomFloat_ = 0x820E7E20;
		this->Cbuf_AddText_ = 0x82287F68;
		this->Com_GetClientDObj_ = 0x822A28D0;
		this->CG_DObjGetWorldTagPos_ = 0x8211BCB8;
		this->GScr_AllocString_ = 0x822513A0;
		this->CG_GetPlayerViewOrigin_ = 0x82159240;
		this->CG_LocationalTrace_ = 0x82162FA0;
		this->BG_GetSpreadForWeapon_ = 0x82101B08;
		this->BG_GetWeaponIndexForName_ = 0x820F7DB0;
		this->UI_GetMapDisplayName_ = 0x822F2580;
		this->UI_GetGameTypeDisplayName_ = 0x822F27A8;
		this->CL_AllClientsDisconnected_ = 0x821A7A60;
		this->Dvar_GetBool_ = 0x8232E2C8;
		this->Dvar_GetInt_ = 0x8232E330;
		this->Dvar_GetString_ = 0x8232E488;
		this->Dvar_GetFloat_ = 0x8232E398;

		this->screenWidth = 0x82677A68;
		this->screenHeight = 0x82677A6C;
		this->screenplacement = 0x82732100;
		this->FPS = 0x836E5AF8 + 0x2C;
		this->netInfoGamertag = 0x0;
		this->netInfoIP = 0xB8;
		this->netInfoXUID = 0xFC;
		this->netInfoSize = 0x158;
		this->FOV = 0x82643EC4;
		this->dvarAdditive = 0x8;
		this->serverId = 0x826B81E8;
		this->noRecoil = 0x821614D4;
		this->noSway = 0x82103CD8;
		//this->ingame = 0xBC43C0;
		//this->drawBulletImpactsPatch = 0x4B88F0;
		//this->DynEntCl_DynEntImpactEventPatch = 0x4F6B00;

		this->centity = 0x826436B8;
		this->cg_t = 0x8263EE60;
		this->clientActive_t = 0x82713DC4;
		this->netInfo = 0xC3970EEC;
#endif
	}
};

struct variables
{
	bool isGameLoaded, isUnloading, iniLoaded, enterCustomTag, enterNumCustomTag;
	float width, height, FOV, pimaryRainbowSpeed, secondaryRainbowSpeed, backgroundRainbowSpeed;
	bool fontShadow, menuOpen, scrollBar, rainbowGamertag, cycleGamertags, fps, ping, mapName, healthBar, crosshair, rainbowName, killMessages, controls, hostGeoLocation,
		editSafeArea, moveMenu, displayCheatCodeTip, primaryColorRainbow, secondaryColorRainbow, backgroundColorRainbow, spoofIP, userBanned;
	vector2 pos, compassPos, safeArea;
	short cachedBones[17];
	int hostName, gamertagCycleDelay, rainbowSpeed;
	float compassSize[2];
	int partGroup, clientToFace;
	char originalGamertag[32];

	variables()
	{
		FOV = 65.0f;
		clientToFace = 18;
	}
};

struct AIMBOT
{
	bool aimbotOn, noSpread, autoBone, autoShoot, noRecoil, noSway, spinbotOn, isAutoShooting;
	bool visible[18], hittable[18], prioritized[18], whitelisted[18];
	short bestBone[18];
	int targetType, aimbotType, closestClient;

	int spinbotX, spinbotY, spinbotZ, xSpeed, ySpeed, zSpeed;
	float *enemyColor[18];
};

struct Esp
{
	bool enemy, friendly, bones, names, scavBoxes, weaponIcons, explosiveEsp, droppedWeaponEsp;
	int boxType, snaplines;
};

extern variables *vars;
extern addresses *addr;
extern AIMBOT *aimbot;
extern Esp *esp;


struct Color
{
	float r, g, b, a;
	
	Color(float r, float g, float b, float a)
	{
		this->r = r / 255.0f;
		this->g = g / 255.0f;
		this->b = b / 255.0f;
		this->a = a / 255.0f;
	}
};

extern cgsArray *cgs;
extern centity_s *Entities;
extern cgArray *cg;
extern clientActive *ca;
extern netInfo_ *netInfo;
extern trace_t trace;
extern rectDef_s rect;
extern rectDef_s parentRect;

struct Bounds
{
	vector3 midPoint;
	vector3 halfSize;
};