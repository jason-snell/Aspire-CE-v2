#include "main.h"

typedef struct _XECRYPT_SHA_STATE {
	DWORD count; // 0x0 sz:0x4
	DWORD state[0x5]; // 0x4 sz:0x14
	BYTE buffer[0x40]; // 0x18 sz:0x40
} XECRYPT_SHA_STATE, *PXECRYPT_SHA_STATE; // size 88
C_ASSERT(sizeof(XECRYPT_SHA_STATE) == 0x58);

int(*funcByName)(int num);
void(*getCpuKey)(char *outBuffer);
BOOL(*GetSectionInfo)(int baseAddr, CONST PCHAR SectionName, PDWORD Address, PDWORD Length);
void(*sha1)(void* inData, unsigned int lenght, unsigned char* outBuffer);
void(*sha1_starts)(sha1_context *ctx);
HRESULT(*HvPeekBytes)(__int64 Address, PVOID Buffer, DWORD Size);
void(*sha1_update)(sha1_context *ctx, unsigned char *input, unsigned int length);
void(*sha1_finish)(sha1_context *ctx, unsigned char digest[20]);
void(*getCheatHash)(char *outBuffer);

#pragma optimize( "", off )
void auth::GenerateCPUKeyHash(DWORD AnnoyingIndex, BYTE* Result)
{
	char cpuKey[0x10] = { 0 };
	getCpuKey(cpuKey);

	DWORD CallingAddr;
	__asm mflr CallingAddr

	BYTE RealCPUKeySha[16];
	BYTE EncryptedConsoleType[16];
	BYTE HVUpdateSenquence[4];
	BYTE HVConsoleType[4];
	BYTE HiddenAddressA[4] = { 0x0E, 0x97, 0xAB, 0x92 };
	DWORD HiddenAddressB;
	BYTE DummyValue[4] = { 0x80, 0x94, 0x01, 0xA2 };
	BYTE HiddenAddressC[4] = { 0x0E, 0x9B, 0x01, 0x91 };
	DWORD HiddenAddressD;
	BYTE DummyValue1[4] = { 0x80, 0x98, 0xA4, 0xC1 };
	BYTE HiddenSizes[4] = { 0x0A, 0x87, 0xB7, 0x88 };
	BYTE IndexValues[4] = { 0x89, 0x9A, 0x03, 0x92 };
	Vector4 AnnoyingVector;
	vector3 AnnoyingVector2;
	XECRYPT_SHA_STATE shaState;
	sha1_context shaHash;

	PLDR_DATA_TABLE_ENTRY pldte = (PLDR_DATA_TABLE_ENTRY)aspireHandle;
	if (CallingAddr >= (DWORD)pldte->NtHeadersBase && CallingAddr <= ((DWORD)pldte->NtHeadersBase + pldte->SizeOfNtImage))
	{
		for (int i = AnnoyingIndex; i < 4; i++)
		{
			HiddenSizes[i] ^= HiddenAddressA[i];
			AnnoyingVector.a = ~HiddenAddressA[i];
			AnnoyingVector2.x = AnnoyingVector.a;
			IndexValues[i] ^= HiddenAddressC[i];
			AnnoyingVector.x = ~HiddenAddressC[i];
			AnnoyingVector2.y = AnnoyingVector.x;
			HiddenAddressA[i] ^= DummyValue[i];
			AnnoyingVector.y = ~HiddenSizes[i];
			AnnoyingVector2.z = AnnoyingVector.z;
			HiddenAddressC[i] ^= DummyValue1[i];
			AnnoyingVector.z = ~IndexValues[i];
		}
	}
	else
	{
		for (int i = 4; i < AnnoyingIndex; i--)
		{
			HiddenAddressA[i] ^= HiddenSizes[i];
			AnnoyingVector.a = ~HiddenAddressA[i];
			AnnoyingVector2.x = AnnoyingVector.a;
			HiddenAddressC[i] ^= IndexValues[i];
			AnnoyingVector.x = ~HiddenAddressC[i];
			AnnoyingVector2.y = AnnoyingVector.x;
			DummyValue[i] ^= HiddenAddressA[i];
			AnnoyingVector.y = ~HiddenSizes[i];
			AnnoyingVector2.z = AnnoyingVector.z;
			DummyValue1[i] ^= HiddenAddressC[i];
			AnnoyingVector.z = ~IndexValues[i];
		}
	}

	IndexValues[0] = 0;

	memcpy(&HiddenAddressB, HiddenAddressA, HiddenSizes[IndexValues[AnnoyingIndex]]);
	memcpy(&HiddenAddressD, HiddenAddressC, HiddenSizes[IndexValues[AnnoyingIndex]]);
	memset(Result, 0, HiddenSizes[IndexValues[1]]);
	sha1_starts(&shaHash);
	HvPeekBytes((__int64)HiddenSizes[IndexValues[3]], HVUpdateSenquence, HiddenSizes[IndexValues[AnnoyingIndex]]);
	sha1_update(&shaHash, (PBYTE)HVUpdateSenquence, HiddenSizes[IndexValues[AnnoyingIndex]]);
	memcpy(RealCPUKeySha, (LPVOID)HiddenAddressB, HiddenSizes[IndexValues[1]]);
	memcpy(EncryptedConsoleType, (LPVOID)HiddenAddressD, HiddenSizes[IndexValues[1]]);
	sha1_update(&shaHash, EncryptedConsoleType, HiddenSizes[IndexValues[1]]);
	sha1_update(&shaHash, RealCPUKeySha, HiddenSizes[IndexValues[1]]);
	HvPeekBytes((__int64)HiddenSizes[IndexValues[2]], HVConsoleType, HiddenSizes[IndexValues[AnnoyingIndex]]);
	sha1_update(&shaHash, (PBYTE)HVConsoleType, HiddenSizes[IndexValues[AnnoyingIndex]]);
	sha1_finish(&shaHash, Result);
	tools::RC4((BYTE*)cpuKey, HiddenSizes[IndexValues[1]], Result, HiddenSizes[IndexValues[1]]);

	// Done
	return;
}
#pragma optimize( "", on )

bool auth::connect(int *outAddrs)
{
#if defined (debug)
	return true;
#endif
	for (int i = 0; i < 5; i++)
	{
#if defined (debug)
		unsigned char IP[4] = { 192, 168, 1, 12 };
#else
		unsigned char IP[4] = { 158, 69, 0, 19 };
#endif
		if (sockets::Connect(IP, 6498))
		{
			cheatData sData = { 0 };

			char cpuKey[16] = { 0 };
			getCpuKey(cpuKey);
			sData.packetId = 2;
			GenerateCPUKeyHash(0, (BYTE*)sData.securityHash);
			memcpy(sData.CPUKey, cpuKey, 16);
			DWORD address, length;
			GetSectionInfo(0x91A40000, ".text", &address, &length);
			sha1((void*)address, length - 0x800, (unsigned char*)sData.textHash);
			getCheatHash(sData.fileHash);

			if (sockets::Send((char*)&sData, sizeof(sData)))
			{
				if (sockets::Receive((char*)&sData, sizeof(sData)))
				{
					switch (sData.packetId)
					{
					case BANNED_PACKET:
						tools::XNotify(sData.message);
						vars->userBanned = true;
						break;
					case TIME_EXPIRED:
						takeDownDetours();
						break;
					case OUTDATED_PACKET:
						tools::XNotify(sData.message);
						break;
					case AUTHED_PACKET:
						for (int i = 0; i < 0x40; i++)
							sData.addresses[i] = _byteswap_ulong(sData.addresses[i]);
						memcpy(outAddrs, sData.addresses, sizeof(sData.addresses));
						break;
					}
				}
			}

			sockets::Close();
			return true;
		}
		else
		{
			char messageBuf[0x100] = { 0 };
			sprintf_s(messageBuf, "Aspire CE - Connection Failed (%i/5)", i + 1);
			tools::XNotify(messageBuf);
			Sleep(3000);
			vars->isUnloading = true;
			if (i == 4)
				return false;
		}
	}
	return false;
}