#pragma once

#include <QtWidgets>

#include "MemReader.h"
#include "Head.h"
#include "Helper.h"
#include "MovesetEditor.h"
#include "DrawingsEditor.h"
#include "WeaponEditor.h"

class DS2BossCreator : public QWidget
{
	Q_OBJECT

public:
	DS2BossCreator(QWidget *parent = Q_NULLPTR);
	~DS2BossCreator();

private:
	MemReader reader;

	Head* head;
	QTabWidget* tab;
	QVBoxLayout* layout;

	Helper* spellEditor;
	Helper* moveEditor;
	MovesetEditor* movesetEditor;
	DrawingsEditor* drawingsEditor;
	WeaponEditor* weaponEditor;

protected:
	void closeEvent(QCloseEvent* e) override;
	void paintEvent(QPaintEvent* e) override;

};
