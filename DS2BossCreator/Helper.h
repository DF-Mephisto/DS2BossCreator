#pragma once

#include <QtWidgets>

#include "Entry.h"
#include "EntityDescription.h"
#include "FocusControlDelegate.h"
#include "MemReader.h"
#include "HookDialog.h"
#include "ChangeResume.h"

using namespace std;
using namespace entity_description;

enum HelperType
{
	BulletHelperType,
	MoveHelperType
};

class Helper : public QWidget
{
	Q_OBJECT

public:
	Helper(MemReader& reader, HelperType helperType, int baseOffset, int entriesCount, int structureSize, int instructionSize, DWORD signatureSize,
		const byte* instruction, const EntityDescription* entityDescriptions, const char* mask, LPVOID hookScript, QString entityName, QWidget *parent = Q_NULLPTR);
	~Helper();

private:
	HelperType helperType;
	Entry* mainAddress;
	Entry* name;

	QTableWidget* table;
	QPushButton* load;
	QPushButton* save;
	QPushButton* getEntities;

	QVBoxLayout* entryLayout;
	QVBoxLayout* controlLayout;
	QHBoxLayout* globalLayout;

	QPointer<HookDialog> hookDialog;

	MemReader& reader;
	const int baseOffset;
	const int entriesCount;
	const int structureSize;
	const int instructionSize;
	const char* mask;
	const DWORD signatureSize;
	const byte* instruction;
	const EntityDescription* entityDescriptions;
	byte* entityData;
	LPVOID hookScript;
	QString entityName;

	void fillTable();
	ChangeType getChangeType(DWORD64 address);
	DWORD64 getOffset(DWORD64 address);

private slots:
	void loadEntity();
	void saveEntity();
	void openHookDialog();

signals:
	void saveResume(ChangeResume resume);

};