#pragma once

#define AngleToShort(X) ((DWORD)((X) * 65536 / 360) & 65535)

extern Detour<int> *XamInputGetStateDetour;
extern Detour<void> *R_EndFrameDetour;
extern Detour<void> *CL_WritePacketDetour;
extern Detour<void> *BG_AdvanceTraceDetour;
extern Detour<void> *R_AddCmdDrawStretchPicDetour;
#if defined (BO2)
extern Detour<int> *preGameAntiFreezeDetour;
extern Detour<void> *DynEntCl_DestoryEventDetour;
extern Detour<int> *xmemcpyDetour;
extern Detour<int> *NetDll_XNetGetTitleXnAddrDetour;
#endif

extern float rainbowcolor[50][4];
extern char Country[18][64];
extern char State[18][64];
extern char City[18][64];
extern char ZipCode[18][16];
extern char ISP[18][64];
extern bool PROXY[18];
extern bool MOBILE[18];
extern unsigned long long storedXUIDs[18];
extern char PresetGamertag[5][32];

//#define debug

enum BONE_NAMES
{
	j_head,
	j_helmet,
	j_neck,
	j_shoulder_le,
	j_shoulder_ri,
	j_elbow_le,
	j_elbow_ri,
	j_wrist_le,
	j_wrist_ri,
	j_hip_ri,
	j_hip_le,
	j_mainroot,
	j_knee_le,
	j_knee_ri,
	j_ankle_ri,
	j_ankle_le,
	j_ball_le,
};

enum SUB_MENUS
{
	ESPCOLOR_SUBMENU = 5,
	FRIENDLY_COLOR,
	ENEMY_COLOR,
	ENEMY_HITTABLE,
	ENEMY_VISIBLE,
	WHITELISTED_COLOR,
	PRIORITIZED_COLOR,
	GAMERTAG_SUBMENU,
	VISUALS_SUBMENU,
	COLOR_SUBMENU,
	MAIN_COLOR,
	FONT_COLOR,
	BACKGROUND_COLOR,
	PLAYER_SUBMENU,
};

enum BOUNDING_BOX_TYPE
{
	BOUNDING_PLAYER,
	BOUNDING_SCAVENGER,
	BOUNDING_C4,
	BOUNDING_CLAYMORE,
};

extern int CLIENT_LIST;

enum X_SPINBOT_TYPES
{
	SPINBOT_UP = 1,
	SPINBOT_DOWN,
	SPINBOT_UPDOWN
};

static char *X_SPINBOT_TYPE[] = {
	"OFF", "UP", "DOWN", "UP DOWN"
};

enum Y_SPINBOT_TYPES
{
	SPINBOT_Y_LEFT = 1,
	SPINBOT_Y_RIGHT,
	SPINBOT_Y_JITTER,
	SPINBOT_Y_ANTIAIM
};

static char *Y_SPINBOT_TYPE[] = {
	"OFF", "LEFT", "RIGHT", "JITTER", "ANTI AIM"
};

enum Z_SPINBOT_TYPES
{
	SPINBOT_Z_LEFT = 1,
	SPINBOT_Z_RIGHT,
	SPINBOT_Z_RANDOM
};

static char *Z_SPINBOT_TYPE[] = {
	"OFF", "LEFT", "RIGHT", "RANDOM"
};

enum EAIMBOT_TYPES
{
	AIMBOT_TYPE_NORMAL,
	AIMBOT_TYPE_SILENT,
};

static char *AIMBOT_TYPE[] = {
	"NORMAL", "SILENT"
};

enum ETARGET_TYPES
{
	TARGET_TYPE_HITTABLE,
	TARGET_TYPE_VISIBLE,
};

static char *TARGET_TYPE[] = {
	"HITTABLE", "VISIBLE"
};

enum EBOX_TYPE
{
	BOX_TYPE_OFF, BOX_TYPE_3D, BOX_TYPE_2D, BOX_TYPE_2DFILLED, BOX_TYPE_CORNER
};

static char *BOX_TYPE[] = {
	"OFF", "3D", "2D", "2D FILLED", "CORNER"
};

enum ESNAPLINE_TYPE
{
	SNAP_TYPE_OFF, SNAP_TYPE_TOP, SNAP_TYPE_CENTER, SNAP_TYPE_BOTTOM
};

enum EHOSTNAME_TYPE
{
	HOSTNAME_OFF, HOSTNAME_NAME, HOSTNAME_NAMEIP, HOSTNAME_IP
};

static char *HOSTNAME_TYPE[] = {
	"OFF", "NAME", "NAME:IP", "IP"
};

static char *SNAPLINE_TYPE[] = {
	"OFF", "TOP", "CENTER", "BOTTOM"
};

static char *boneNames[17] = {
	"j_head",
	"j_helmet",
	"j_neck",
	"j_shoulder_le",
	"j_shoulder_ri",
	"j_elbow_le",
	"j_elbow_ri",
	"j_wrist_le",
	"j_wrist_ri",
	"j_hip_ri",
	"j_hip_le",
	"j_mainroot",
	"j_knee_le",
	"j_knee_ri",
	"j_ankle_ri",
	"j_ankle_le",
	"j_ball_le",
};

static char *Bones[] = {
	"j_helmet", "j_head", "j_neck",
	"j_shoulder_le", "j_elbow_le", "j_wrist_le", "j_elbow_le", "j_shoulder_le", "j_neck",
	"j_shoulder_ri", "j_elbow_ri", "j_wrist_ri", "j_elbow_ri", "j_shoulder_ri", "j_neck",
	"j_spineupper", "j_spinelower", "j_hip_le", "j_knee_le", "j_ankle_le", "j_knee_le",
	"j_hip_le", "j_spinelower", "j_hip_ri", "j_knee_ri", "j_ankle_ri"
};

enum TITLE_ID_LIST
{
	COD_AW = 0x41560914,
	COD_GHOSTS = 0x415608FC,
	COD_BO1 = 0x41560855,
	COD_BO2 = 0x415608C3,
	COD_WAW = 0x4156081C,
	COD_MW2 = 0x41560817,
	COD_4 = 0x415607E6,
	COD_MW3 = 0x415608CB,
};

#if defined(BO2)
static char bo2GamertagHook[] = { 
	0x7C, 0x83, 0x23, 0x78, 0x3D, 0x60, 0x82,0xC5,
	0x38, 0x8B, 0x5D, 0x60, 0x3D, 0x60, 0x82, 0x4A,
	0x39, 0x6B, 0xDC, 0xA0, 0x38, 0xA0, 0x00, 0x20,
	0x7D, 0x69, 0x03, 0xA6, 0x4E, 0x80, 0x04, 0x20
};

static char bo2gamertagPatchInJump[] = {
	0x3D, 0x60, 0x82, 0xC5, 0x39, 0x6B, 0x5D, 0x00,
	0x7D, 0x69, 0x03, 0xA6, 0x4E, 0x80, 0x04, 0x20
};
#endif