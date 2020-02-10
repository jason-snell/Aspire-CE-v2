#pragma once

#define null -1337

extern void(*XNotifyQueueUI)(int dwType, int dwUserIndex, int dwPriority, wchar_t *pwszStringParam, unsigned long long qwParam);
extern int(*XamGetCurrentTitleID)();
extern int(*MmIsAddressValid)(int address);

extern XINPUT_STATE state;
extern unsigned int Wait, tickCount;

class tools
{
private:
	static DWORD PatchModuleImport(PLDR_DATA_TABLE_ENTRY Module, PCHAR ImportedModuleName, DWORD Ordinal, DWORD PatchAddress);
	static void xnotifyThread(wchar_t *text);
public:
	static void patchInJump(int *address, int destination, bool linked);
	static int relinkGPLR(int SFSOffset, int *saveStubAddress, int *originalAddress);
	static void hookFunctionStart(int *Address, int *saveStub, int Destination);
	static int resolveFunction(char *moduleName, int ordinal);
	static int patchModuleImport(char *module, char *importedModuleName, int ordinal, int patchAddress);
	static wchar_t *wc(char *c);
	static void XNotify(char *text);
	static bool keyIsDown(int key);
	static int gameTitle();
	static char *notifyText();
	static void saveIniFile();
	static void loadIniFile();
	static DWORD _resolve(HMODULE Module, DWORD Export, DWORD off1, DWORD off2);
	static void addToList(HANDLE hModule);
	static BOOL WriteFileAlt(PCHAR Path, LPCVOID Buffer, DWORD dwBytesToWrite);;
	static int mountAspireDrive();
	static void RC4(void* Key, int ccKey, void* Data, int ccData);
};

void sleep(int time);