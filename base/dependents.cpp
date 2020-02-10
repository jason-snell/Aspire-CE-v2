#include "main.h"

Detour<int> *XamInputGetStateDetour;
Detour<void> *R_EndFrameDetour;
Detour<void> *CL_WritePacketDetour;
Detour<void> *BG_AdvanceTraceDetour;
Detour<void> *R_AddCmdDrawStretchPicDetour;
#if defined (BO2)
Detour<int> *preGameAntiFreezeDetour;
Detour<void> *DynEntCl_DestoryEventDetour;
Detour<int> *xmemcpyDetour;
Detour<int> *NetDll_XNetGetTitleXnAddrDetour;
#endif

int CLIENT_LIST = 5;
float rainbowcolor[50][4] = { 0 };
char Country[18][64] = { 0 };
char State[18][64] = { 0 };
char City[18][64] = { 0 };
char ZipCode[18][16] = { 0 };
char ISP[18][64];
bool PROXY[18] = { false };
bool MOBILE[18] = { false };
unsigned long long storedXUIDs[18] = { 0 };
char PresetGamertag[5][32] = { 0 };