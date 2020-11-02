#pragma once

#include "MemReader.h"

MemReader::MemReader() {
	processName = L"DarkSoulsII.exe";

	pID = 0;
	pHandle = 0;
	baseAddress = 0;
	hooksCount = 0;
}

MemReader::~MemReader() {

}

bool MemReader::initialize()
{
	getProcess();
	getModule();
	openProc();
	return getProcessStatus();
}

DWORD MemReader::getProcess() {
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pInfo;
	pInfo.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(snapshot, &pInfo)) {
		do {
			if (processName == pInfo.szExeFile) {
				pID = pInfo.th32ProcessID;
				CloseHandle(snapshot);
				return pInfo.th32ProcessID;
			}
		} while (Process32Next(snapshot, &pInfo));
	}
}

DWORD64 MemReader::getModule() {
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);
	MODULEENTRY32 mInfo;
	mInfo.dwSize = sizeof(MODULEENTRY32);
	Module32First(snapshot, &mInfo);
	baseAddress = (DWORD64)mInfo.modBaseAddr;
	moduleSize = mInfo.modBaseSize;
	CloseHandle(snapshot);
	return baseAddress;
}

HANDLE MemReader::openProc() {
	return pHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, pID);
}

void MemReader::closeProc() {
	CloseHandle(pHandle);
}

bool MemReader::getProcessStatus()
{
	DWORD exitCode = 0;
	GetExitCodeProcess(pHandle, &exitCode);

	return exitCode == STILL_ACTIVE ? true : false;
}

DWORD64 MemReader::getBaseAddress()
{
	return baseAddress;
}

DWORD64 MemReader::getModuleSize()
{
	return moduleSize;
}

bool MemReader::readMemory(LPVOID source, LPVOID destination, int size)
{
	return ReadProcessMemory(pHandle, source, destination, size, NULL);
}

bool MemReader::writeMemory(LPVOID destination, LPVOID source, int size)
{
	return WriteProcessMemory(pHandle, destination, source, size, NULL);
}

DWORD64 MemReader::readPointer(const DWORD64* offsets, int length)
{
	DWORD64 res = baseAddress;

	for (int i = 0; i < length; i++)
	{
		if (!readMemory((LPVOID)(res + offsets[i]), &res, 8))
			return 0;
	}

	return res;
}

DWORD64	MemReader::alloc(int size)
{
	MEMORY_BASIC_INFORMATION mbi;
	DWORD64 freeMemory = 0;
	DWORD64 offset = 0;
	DWORD64 allocatedMemory = 0;
	while ((freeMemory = baseAddress - offset) <= baseAddress)
	{
		VirtualQueryEx(pHandle, (LPCVOID)freeMemory, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
		if (mbi.State == MEM_FREE)
		{
			if (allocatedMemory = (DWORD64)VirtualAllocEx(pHandle, (LPVOID)freeMemory, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE))
				break;
		}
		offset += mbi.RegionSize;
	}

	return allocatedMemory;
}

void MemReader::free(DWORD64 allocatedMemory)
{
	VirtualFreeEx(pHandle, (LPVOID)allocatedMemory, 0, MEM_RELEASE);
}

int MemReader::hookMemory(DWORD instructionSize, DWORD signatureSize, LPVOID script, const byte* instruction, const char* mask)
{
	if (!getProcessStatus())
		return -1;

	HookDescription hook;

	//Get instruction address to be changed
	hook.instructionSize = instructionSize;
	hook.instructionAddress = findSignature(baseAddress, moduleSize, signatureSize, instruction, mask);
	if (!hook.instructionAddress)
		return -1;

	//Allocate memory in game process for our code and get its size
	hook.allocatedMemory = alloc(4096);
	hook.scriptSize = getScriptSize(script);
	if (!hook.allocatedMemory)
		return -1;

	//Get offsets for jmp and ret
	DWORD jmp_address = 0 - (hook.instructionAddress - hook.allocatedMemory) - 5;
	DWORD ret_address = instructionSize - 5 - (hook.allocatedMemory - hook.instructionAddress) - hook.scriptSize;

	//jmp &ret_address
	byte endBuf[5] = { 0xE9 };
	memcpy(endBuf + 1, &ret_address, 4);

	//Write script code to the allocated memory
	writeMemory((LPVOID)hook.allocatedMemory, (LPVOID)script, hook.scriptSize);
	writeMemory((LPVOID)(hook.allocatedMemory + hook.scriptSize), endBuf, 5);

	//Save original game code
	hook.originalInstruction = new byte[instructionSize];
	readMemory((LPVOID)hook.instructionAddress, hook.originalInstruction, instructionSize);

	//Change original game instruction to our jmp
	memcpy(endBuf + 1, &jmp_address, 4);
	writeMemory((LPVOID)hook.instructionAddress, endBuf, 5);

	byte nop = 0x90;
	for (int i = 0; i < instructionSize - 5; i++)
		writeMemory((LPVOID)(hook.instructionAddress + 5 + i), &nop, 1);

	hooks.insert(pair<DWORD, HookDescription>(hooksCount++, hook));
	return hooksCount - 1;
}

void MemReader::unhookMemory(int key)
{
	auto hook = hooks.find(key);
	if (hook == hooks.end())
		return;

	writeMemory((LPVOID)hook->second.instructionAddress, hook->second.originalInstruction, hook->second.instructionSize);
	free(hook->second.allocatedMemory);
	delete[] hook->second.originalInstruction;
	hooks.erase(key);
}

DWORD64 MemReader::getHookDataAddress(int key)
{
	auto hook = hooks.find(key);
	if (hook == hooks.end())
		return 0;

	return hook->second.allocatedMemory + hook->second.scriptSize + 5;
}

DWORD64 MemReader::findSignature(DWORD64 base, DWORD64 size, DWORD instructionSize, const byte* sign, const char* mask) {
	MEMORY_BASIC_INFORMATION mbi = { 0 };
	DWORD64 offset = 0;
	while (offset < size) {
		VirtualQueryEx(pHandle, (LPCVOID)(base + offset), &mbi, sizeof(MEMORY_BASIC_INFORMATION));
		if (mbi.State != MEM_FREE) {
			byte* buffer = new byte[mbi.RegionSize];
			ReadProcessMemory(pHandle, mbi.BaseAddress, buffer, mbi.RegionSize, NULL);
			for (int i = 0; i < mbi.RegionSize; i++) {
				if (dataCompare(buffer + i, sign, mask, instructionSize)) {
					delete[] buffer;
					return (DWORD64)mbi.BaseAddress + i;
				}
			}
			delete[] buffer;
		}
		offset += mbi.RegionSize;
	}
	return 0;
}

bool MemReader::dataCompare(const byte* data, const byte* sign, const char* mask, DWORD size) {
	for (int i = 0; i < size; i++) {
		if (mask[i] == 'x' && data[i] != sign[i]) return false;
	}
	return true;
}

DWORD MemReader::getScriptSize(LPVOID script)
{
	int size = 0;
	byte* ptr = (byte*)script;

	while (*((DWORD*)ptr) != 0x90909090)
	{
		size++;
		ptr++;
	}

	return size;
}