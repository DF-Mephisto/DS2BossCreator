#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <unordered_map>

using namespace std;

struct HookDescription
{
	DWORD64 allocatedMemory;
	DWORD64 instructionAddress;
	DWORD64 scriptSize;
	DWORD instructionSize;
	byte* originalInstruction;
};

class MemReader {
public:
	MemReader();
	~MemReader();

	bool initialize();
	bool getProcessStatus();
	void closeProc();

	bool dataCompare(const byte* data, const byte* sign, const char* mask, DWORD size);
	bool readMemory(LPVOID source, LPVOID destination, int size);
	bool writeMemory(LPVOID destination, LPVOID source, int size);
	int hookMemory(DWORD instructionSize, DWORD signatureSize, LPVOID script, const byte* instruction, const char* mask);
	void unhookMemory(int key);
	DWORD64 readPointer(const DWORD64* offsets, int length);
	DWORD64 getHookDataAddress(int key);
	DWORD64 findSignature(DWORD64 base, DWORD64 size, DWORD instructionSize, const byte* sign, const char* mask);
	DWORD64 getBaseAddress();
	DWORD64 getModuleSize();

private:
	wstring   processName;
	HANDLE    pHandle;
	DWORD	  pID;
	DWORD64   baseAddress;
	DWORD     moduleSize;
	int		  hooksCount;
	unordered_map<int, HookDescription> hooks;

	DWORD getProcess();
	DWORD64 getModule();
	HANDLE openProc();
	DWORD getScriptSize(LPVOID script);

	DWORD64	alloc(int size);
	void free(DWORD64 allocatedMemory);
};