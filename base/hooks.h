#pragma once

class hooks
{
public:
	static int XamInputGetStateHook(int dwUserIndex, int r4, PXINPUT_STATE pState);
	static void R_EndFrameHook();
	static void CL_WritePacketHook(int localClientNum);
#if defined (BO2)
	static void BG_AdvanceTraceHook(BulletFireParams *bp, BulletTraceResults *br, float dist);
	static int preGameAntiFreezeHook(int r3, int r4, int r5, int r6, int r7, int r8);
	static void DynEntCl_DestroyEventHook(int r3, short r4, int r5, vector3 &hitPos, vector3 &hitDir);
	static int xmemcpyHook(void *Dst, const void *Source, int Size);
	static int NetDll_XNetGetTitleXnAddrHook(int xnc, XNADDR *xna);
#endif
	static void R_AddCmdDrawStretchPicHook(float x, float y, float w, float h, float s0, float t0, float s1, float t1, const float *color, void *material);
};