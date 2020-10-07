#pragma once

#include <QtWidgets>

#include "Entry.h"
#include "EntityDescription.h"
#include "FocusControlDelegate.h"
#include "MemReader.h"
#include "ChangeResume.h"

using namespace std;
using namespace entity_description;

class MovesetEditor : public QWidget
{
	Q_OBJECT

public:
	MovesetEditor(MemReader& reader, QWidget *parent = Q_NULLPTR);
	~MovesetEditor();

private:
	QComboBox* hand;
	Entry* name;

	QTableWidget* table;
	QPushButton* load;
	QPushButton* save;

	QVBoxLayout* entryLayout;
	QVBoxLayout* controlLayout;
	QHBoxLayout* globalLayout;

	MemReader& reader;
	byte* entityData;

	void fillTable();

private slots:
	void loadEntity();
	void saveEntity();

signals:
	void saveResume(ChangeResume resume);

};