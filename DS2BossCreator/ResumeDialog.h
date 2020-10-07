#pragma once

#include <QtWidgets>
#include <vector>

#include "MemReader.h"
#include "ChangeResume.h"
#include "FocusControlDelegate.h"
#include "EntityDescription.h"

class ResumeDialog : public QDialog
{
	Q_OBJECT

public:
	ResumeDialog(MemReader& reader, QVector<ChangeResume>& changes, QWidget *parent = Q_NULLPTR);

private:
	QTableWidget* table;
	QPushButton* enableAll;
	QPushButton* disableAll;

	QMenu* menu;
	QAction* enableItemAction;
	QAction* deleteItemAction;
	int menuRow;
	int menuState;

	QVector<ChangeResume>& changes;
	MemReader& reader;

public slots:
	void enableAllItems(bool state);

private slots:
	void updateTable();
	void enableItem(int index, bool state);
	int getBaseOffset(ChangeType type);
	void handleContextMenu(const QPoint& pos);
	void deleteItem(int index);

	DWORD64 getAddress(ChangeType type, DWORD64 offset);

};