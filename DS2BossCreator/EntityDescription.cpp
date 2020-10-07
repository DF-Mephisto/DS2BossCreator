#include "EntityDescription.h"

namespace entity_description
{
	QValidator* getValidator(EntryType type)
	{
		static const QRegExpValidator stringValidator(QRegExp(".{,40}"));
		static const QRegExpValidator dwordValidator(QRegExp("[0-9A-Fa-f]{,8}"));
		static const QRegExpValidator qwordValidator(QRegExp("[0-9A-Fa-f]{,16}"));
		static const QRegExpValidator byteValidator(QRegExp("[0-1][0-9]{,2}|25[0-5]|2[0-4][0-9]|[0-9]{,2}"));
		static const QDoubleValidator floatValidator;

		switch (type)
		{
		case StringType:
			return (QValidator*)&stringValidator;

		case ByteType:
			return (QValidator*)&byteValidator;

		case DWordType:
			return (QValidator*)&dwordValidator;

		case QWordType:
			return (QValidator*)&qwordValidator;

		case FloatType:
			return (QValidator*)&floatValidator;

		default:
			return nullptr;
		}
	}

	QMap<EntryType, QString> entryTypeNames = {
		{StringType, "String"},
		{ByteType, "Byte"},
		{DWordType, "4 Bytes"},
		{QWordType, "8 Bytes"},
		{FloatType, "Float"}
	};

	EntityDescription spellDescriptions[spellEntriesCount] = {
		-0x2F, false, DWordType, "Graphical effect on spawn", 
			"<span style=\"color: red\";>Not visible online<\\span>",
		-0x2B, false, ByteType, "Initial spawn position", 
			"0 - caster's position\n1 - parent's death position",
		-0x2A, false, ByteType, "Spawn direction", 
			"1 - caster's direction\n2 - at caster's head\n5 - parent's direction",
		-0x29, false, ByteType, "Online visibility", 
			"0 - not visible\n2 - visible",
		-0x28, false, ByteType, "Spawn flag", "",
		-0x27, false, FloatType, "Z coordinate", "",
		-0x23, false, FloatType, "Y coordinate", "",
		-0x1F, false, FloatType, "X coordinate", "",
		-0x1B, false, FloatType, "Vertical angle relative to caster", "",
		-0x17, false, FloatType, "Horizontal angle relative to caster", "",
		-0x13, false, FloatType, "Vertical angle (when locked on)", "",
		-0xF,  false, FloatType, "Horizontal angle (when locked on)", "",
		-0xB,  false, FloatType, "Vertical angle randomizer", 
			"larger = randomer",
		-0x7,  false, FloatType, "Horizontal angle randomizer", 
			"larger = randomer",
		-0x1,  false, ByteType, "Amount of bullets fired at once", "",
		0x0,   false, ByteType, "Amount of bullets fired sequentially", 
			">127 - inf",
		0x1,   false, FloatType, "Vertical spread when fired at once", "",
		0x5,   false, FloatType, "Horizontal spread when fired at once", "",
		0x9,   false, FloatType, "Firing delay", "",
		0xD,   false, FloatType, "Vertical spread when fired sequentially", "",
		0x11,  false, FloatType, "Horizontal spread when fired sequentially", "",
		0x15,  false, ByteType, "Spawn position when fired sequentially", 
			"0 - from hand\n1 - from hand then from body, alternately",
		0x18,  false, ByteType, "Graphical effect on cast when fired sequentially", 
			"0 - off\n1 - on\n",
		0x19,  false, FloatType, "Longevity", "",
		0x1D,  false, FloatType, "Initial speed, horizontal", "",
		0x21,  false, FloatType, "Acceleration start delay, horizontal", "",
		0x25,  false, FloatType, "Acceleration, horizontal", "",
		0x29,  false, FloatType, "Maximum speed, horizontal", "",
		0x2D,  false, FloatType, "Initial speed, vertical", "",
		0x31,  false, FloatType, "Acceleration start delay, vertical", "",
		0x35,  false, FloatType, "Acceleration, vertical", "",
		0x39,  false, FloatType, "Maximum speed, vertical", "",
		0x3D,  false, FloatType, "Initial tangential speed, vertical", "",
		0x41,  false, FloatType, "Tangential acceleration delay, vertical", "",
		0x45,  false, FloatType, "Maximum tangential speed, vertical", "",
		0x49,  false, FloatType, "Tangential acceleration, vertical", "",
		0x4D,  false, FloatType, "Initial tangential speed, horizontal", "",
		0x51,  false, FloatType, "Tangential acceleration delay, horizontal", "",
		0x55,  false, FloatType, "Tangential acceleration, horizontal", "",
		0x59,  false, FloatType, "Maximum tangential speed, horizontal", "",
		0x5E,  false, ByteType, "Tracking direction", "",
		0x61,  false, FloatType, "Maximum amount of times bullet changes angle", 
			"0 - inf",
		0x65,  false, FloatType, "Tracking maximum angle change", "",
		0x69,  false, FloatType, "Tracking time", "",
		0x6D,  false, FloatType, "Tracking-related float, affects bullet after collision", "",
		0x81,  false, ByteType, "Impact on npc and players", 
			"0 - has collision\n1 - passes through enemies but harms them\n02 - passes through enemies without visual effect or harming them",
		0x82,  false, ByteType, "Impact on walls", 
			"0 - has collision\n1 - passes through object\n2 - passes through object with no effect",
		0x83,  false, ByteType, "Impact on ceilings/floors", 
			"0 - has collision\n1 - passes through object\n2 - passes through object with no effect",
		0x84,  false, ByteType, "Impact on water", 
			"0 - has collision\n1 - passes thought water\n2- passes through water with no effect",
		0x85,  false, DWordType, "Sound/visual effect on bullet fired", "",
		0x8D,  false, DWordType, "Graphical effect on collision", "",
		0x91,  false, DWordType, "Graphical effect on death", "",
		0x95,  false, FloatType, "Longevity of graphical effect on death", "",
		0x9D,  false, DWordType, "Collision behavior", "",
		0xA3,  false, ByteType, "Removing old bullets if new one is created", 
			"0 - off\n1 - on",
		0xA5,  true,  DWordType, "Child projectile 1 (bullet id)", "",
		0xAD,  true,  DWordType, "Child projectile 2 (bullet id)", "",
		0xB5,  true,  DWordType, "Child projectile 3 (bullet id)", "",
		0xBD,  false, ByteType, "Spawn child bullets on collision", 
			"0 - no\n1 - yes",
		0xBE,  false, ByteType, "Spawn child bullets on timeout", 
			"0 - no\n1 - yes",
		0xC0,  false, ByteType, "Spawn child bullets while alive", 
			"0 - no\n1 - yes",
		0xD1,  false, DWordType, "Graphical effect on death", 
			"1 - burnt spot",
		0xE5,  false, DWordType, "Somehow changes vertical stats to horizontal stats", "",
		0xF9,  false, FloatType, "Time between spawning child bullets while alive", "",
	};

	EntityDescription moveDescriptions[moveEntriesCount] = {
		-0x38, false, DWordType, "Core move", "",
		-0x34, false, ByteType, "Attack type (tracking)", "",
		-0x30, false, FloatType, "Startup speed", "",
		-0x2C, false, FloatType, "Recovery speed", "",
		-0x28, false, FloatType, "Tracking start", "",
		-0x24, false, FloatType, "Tracking mid", "",
		-0x20, false, FloatType, "Tracking end", "",
		-0x1C, false, FloatType, "Weapon orientation Y axis", "",
		-0x10, false, FloatType, "Self pushback on hit", "",
		-0xC,  false, DWordType, "Hit data 1", "",
		-0x8,  false, DWordType, "Hit data 2", "",
		-0x4,  false, DWordType, "Hit data 3", "",
		0x0,   true,  DWordType, "Bullet end of swing (bullet id)", "",
		0x8,   true,  DWordType, "Bullet start of swing (bullet id)", 
			"not every weapon accepts this parameter",
		0x10,  false, DWordType, "Buff", "",
		0x14,  false, DWordType, "Bullet 3", "",
	};

	EntityDescription movesetDescriptions[100] = {
		0x0,  false, DWordType, "1H R1 #1", "",
		0x4,  false, DWordType, "1H R1 #2", "",
		0x8,  false, DWordType, "1H R2 #1", "",
		0xC,  false, DWordType, "1H R2 #2", "",
		0x10, false, DWordType, "1H R1 RUNNING", "",
		0x14, false, DWordType, "1H R1 ROLLING", "",
		0x18, false, DWordType, "1H R1 BACKSTEP", "",
		0x1C, false, DWordType, "1H FALLING", "",
		0x20, false, DWordType, "1H LANDING", "",
		0x24, false, DWordType, "1H JUMPING", "",
		0x28,  false, DWordType, "2H R1 #1", "",
		0x2C,  false, DWordType, "2H R1 #2", "",
		0x30,  false, DWordType, "2H R2 #1", "",
		0x34,  false, DWordType, "2H R2 #2", "",
		0x38, false, DWordType, "2H R1 RUNNING", "",
		0x3C, false, DWordType, "2H R1 ROLLING", "",
		0x40, false, DWordType, "2H R1 BACKSTEP", "",
		0x44, false, DWordType, "2H FALLING", "",
		0x48, false, DWordType, "2H LANDING", "",
		0x4C, false, DWordType, "2H JUMPING", "",
		0x50,  false, DWordType, "CHAIN 1 #1", "",
		0x54,  false, DWordType, "CHAIN 1 #2", "",
		0x58,  false, DWordType, "CHAIN 2 #1", "",
		0x5C,  false, DWordType, "CHAIN 2 #2", "",
		0x60, false, DWordType, "CHAIN RUNNING", "",
		0x64, false, DWordType, "CHAIN ROLLING", "",
		0x68, false, DWordType, "CHAIN BACKSTEP", "",
		0x6C, false, DWordType, "CHAIN FALLING", "",
		0x70, false, DWordType, "CHAIN LANDING", "",
		0x74, false, DWordType, "CHAIN JUMPING", "",
		0x78,  false, DWordType, "POWERSTANCE R1 #1", "",
		0x7C,  false, DWordType, "POWERSTANCE R1 #2", "",
		0x80,  false, DWordType, "POWERSTANCE R2 #1", "",
		0x84,  false, DWordType, "POWERSTANCE R2 #2", "",
		0x88, false, DWordType, "POWERSTANCE RUNNING", "",
		0x8C, false, DWordType, "POWERSTANCE ROLLING", "",
		0x90, false, DWordType, "POWERSTANCE BACKSTEP", "",
		0x94, false, DWordType, "POWERSTANCE FALLING", "",
		0x98, false, DWordType, "POWERSTANCE LANDING", "",
		0x9C, false, DWordType, "POWERSTANCE JUMPING", "",
		0xA0,  false, DWordType, "1H L1 #1", "",
		0xA4,  false, DWordType, "1H L1 #2", "",
		0xA8,  false, DWordType, "1H L2 #1", "",
		0xAC,  false, DWordType, "1H L2 #2", "",
		0xB0, false, DWordType, "1H L1 RUNNING", "",
		0xB4, false, DWordType, "1H L1 ROLLING", "",
		0xB8, false, DWordType, "1H L1 BACKSTEP", "",
		0xBC, false, DWordType, "1H L1 FALLING", "",
		0xC0, false, DWordType, "1H L1 LANDING", "",
		0xC4, false, DWordType, "1H L1 JUMPING", "",
		0xC8,  false, DWordType, "2H L1", "",
		0xCC,  false, DWordType, "2H L2", "",
		0xD0,  false, DWordType, "POWERSTANCE L1 #1", "",
		0xD4,  false, DWordType, "POWERSTANCE L1 #2", "",
		0xD8,  false, DWordType, "POWERSTANCE L2", "",
		0xDC,  false, DWordType, "GUARD BREAK 1", "",
		0xE0,  false, DWordType, "GUARD BREAK 2", "",
		0xE4,  false, DWordType, "SHIELD POKE 1 #1", "",
		0xE8,  false, DWordType, "SHIELD POKE 1 #2", "",
		0xEC,  false, DWordType, "SHIELD POKE 2 #1", "",
		0xF0,  false, DWordType, "SHIELD POKE 2 #2", "",
		0xF4,  false, DWordType, "RIPOSTE", "",
		0xF8,  false, DWordType, "GUARD BREAK STAB", "",
		0xFC,  false, DWordType, "BACKSTAB", ""
	};
}