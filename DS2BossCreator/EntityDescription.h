#pragma once
#include <QtWidgets>
#include <Windows.h>

namespace entity_description
{
	extern "C"
	{
		void BulletHelper();
		void MoveHelper();
		void DrawingsScript();
	}

	enum EntryType
	{
		StringType,
		ByteType,
		WordType,
		DWordType,
		QWordType,
		FloatType
	};

	struct EntityDescription
	{
		int offset;
		bool doubleValue;
		EntryType type;
		QString name;
		QString tip;
	};

	const int spellBaseOffset = 0x2F;
	const int spellEntriesCount = 64;
	const int spellStructureSize = 320;
	const int spellOriginalInstructionSize = 10;
	const int spellSignatureSize = 10;
	const char spellMask[] = "xxxxxxxxxx";
	const byte spellInstruction[] = { 0x0F, 0xBE, 0x4F, 0x2F, 0x39, 0x8B, 0x20, 0x01, 0x00, 0x00 };
	extern EntityDescription spellDescriptions[spellEntriesCount];

	const int moveBaseOffset = 0x38;
	const int moveEntriesCount = 16;
	const int moveStructureSize = 80;
	const int moveOriginalInstructionSize = 8;
	const int moveSignatureSize = 10;
	const char moveMask[] = "xxxxxxxxxx";
	const byte moveInstruction[] = { 0x41, 0x8B, 0x0F, 0x48, 0x8B, 0x7C, 0x24, 0x78, 0x48, 0x8B };
	extern EntityDescription moveDescriptions[moveEntriesCount];

	const int movesetEntriesCount = 64;
	const int movesetStructureSize = 256;
	const int movesetPointerLength = 4;
	const DWORD64 movesetPointers[6][4] = {
		{ 0x160B8D0, 0xD0, 0x7F8, 0x138 },
		{ 0x160B8D0, 0xD0, 0x7F8, 0x180 },
		{ 0x160B8D0, 0xD0, 0x7F8, 0x1C8 },
		{ 0x160B8D0, 0xD0, 0x7F8, 0x60 },
		{ 0x160B8D0, 0xD0, 0x7F8, 0xA8 },
		{ 0x160B8D0, 0xD0, 0x7F8, 0xF0 }
	};
	extern EntityDescription movesetDescriptions[movesetEntriesCount];

	const int weaponsEntriesCount = 31;
	const int weaponsStructureSize = 188;
	const int weaponsPointerLength = 4;
	const DWORD64 weaponsPointers[6][4] = {
		{ 0x160B8D0, 0xD0, 0x7F8, 0x130 },
		{ 0x160B8D0, 0xD0, 0x7F8, 0x178 },
		{ 0x160B8D0, 0xD0, 0x7F8, 0x1C0 },
		{ 0x160B8D0, 0xD0, 0x7F0, 0x558 },
		{ 0x160B8D0, 0xD0, 0x7F8, 0xA0 },
		{ 0x160B8D0, 0xD0, 0x7F0, 0x5E8 }
	};
	extern EntityDescription weaponsDescriptions[weaponsEntriesCount];

	const int paramPointerLength = 4;
	const DWORD64 commonBulletParamPointer[] = { 0x160B8D0, 0x60, 0x20, 0xD8 };
	const DWORD64 enemyBulletParamPointer[] = { 0x160B8D0, 0x60, 0x30, 0xD8 };
	const DWORD64 worldBulletParamPointer[] = { 0x160B8D0, 0x60, 0x40, 0xD8 };
	const DWORD64 moveParamPointer[] = { 0x160B8D0, 0x18, 0x440, 0xD8 };
	const DWORD64 movesetParamPointer[] = { 0x160B8D0, 0x18, 0x430, 0xD8 };
	const DWORD64 weaponsParamPointer[] = { 0x160B8D0, 0x18, 0x420, 0xD8 };

	//DRAWINGS SCRIPT
	const int drawOriginalInstructionSize = 5;
	const int drawSignatureSize = 10;
	const char drawMask[] = "xxxxxxxxxx";
	const byte drawInstruction[] = { 0xF3, 0x0F, 0x10, 0x47, 0x10, 0xF3, 0x0F, 0x10, 0x57, 0x08 };

	//DISABLE DURABILITY
	const int wepDuraSignatureSize = 10;
	const char wepDuraMask[] = "x**xx****x";
	const byte wepDuraSignature[] = { 0xF3, 0x00, 0x00, 0xB4, 0xC3, 0x00, 0x00, 0x00, 0x00, 0x48 };
	const int wepDuraInstructionSize = 9;
	const byte wepDuraNewInstruction[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	const byte wepDuraOldInstruction[] = { 0xF3, 0x0F, 0x11, 0xB4, 0xC3, 0x94, 0x00, 0x00, 0x00 };

	const int gearDuraSignatureSize = 10;
	const char gearDuraMask[] = "x**xx****x";
	const byte gearDuraSignature[] = { 0xF3, 0x00, 0x00, 0x84, 0xD1, 0x00, 0x00, 0x00, 0x00, 0xC3 };
	const int gearDuraInstructionSize = 9;
	const byte gearDuraNewInstruction[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	const byte gearDuraOldInstruction[] = { 0xF3, 0x0F, 0x10, 0x84, 0xD1, 0xBC, 0x02, 0x00, 0x00 };

	const int ringDuraSignatureSize = 8;
	const char ringDuraMask[] = "x****xxx";
	const byte ringDuraSignature[] = { 0xF3, 0x00, 0x00, 0x00, 0x00, 0xC3, 0xD8, 0x03 };
	const int ringDuraInstructionSize = 10;
	const byte ringDuraNewInstruction[] = { 0xF3, 0x0F, 0x10, 0xD0, 0xF3, 0x0F, 0x10, 0xF0, 0x90, 0x90 };
	const byte ringDuraOldInstruction[] = { 0xF3, 0x42, 0x0F, 0x10, 0x94, 0xC3, 0xD8, 0x03, 0x00, 0x00 };

	const int infDuraSignatureSize = 11;
	const char infDuraMask[] = "xxxxxxxxxxx";
	const byte infDuraSignature[] = { 0xF3, 0x0F, 0x11, 0x52, 0x20, 0x48, 0x8B, 0xFA, 0x48, 0x8B, 0xD9 };
	const int infDuraInstructionSize = 5;
	const byte infDuraNewInstruction[] = { 0x90, 0x90, 0x90, 0x90, 0x90 };
	const byte infDuraOldInstruction[] = { 0xF3, 0x0F, 0x11, 0x52, 0x20 };

	extern QMap<EntryType, QString> entryTypeNames;
	QValidator* getValidator(EntryType type);
}