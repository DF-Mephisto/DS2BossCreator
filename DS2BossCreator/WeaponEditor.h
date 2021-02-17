#pragma once

#include <QtWidgets>

#include "Entry.h"
#include "EntityDescription.h"
#include "FocusControlDelegate.h"
#include "MemReader.h"
#include "ChangeResume.h"

using namespace std;
using namespace entity_description;

class WeaponEditor : public QWidget
{
	Q_OBJECT

public:
	WeaponEditor(MemReader& reader, QWidget *parent = Q_NULLPTR);
	~WeaponEditor();

private:
	MemReader& reader;
	byte* entityData;

	QComboBox* hand;
	Entry* name;

	QTableWidget* table;
	QPushButton* load;
	QPushButton* save;

	QVBoxLayout* entryLayout;
	QVBoxLayout* controlLayout;
	QHBoxLayout* globalLayout;

	void fillTable();

private slots:
	void loadEntity();
	void saveEntity();

signals:
	void saveResume(ChangeResume resume);

};