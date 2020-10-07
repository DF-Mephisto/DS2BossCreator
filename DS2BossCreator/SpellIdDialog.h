#pragma once

#include <QtWidgets>

#include "SpellID.h"
#include "FocusControlDelegate.h"

using namespace std;

class SpellIdDialog : public QDialog
{
public:
	SpellIdDialog(QWidget *parent = Q_NULLPTR);

private:
	QTabWidget* mainTab;
	QTabWidget* commonSpellsTab;

	QTableWidget* makeTable(const QMap<QString, QString>& values);
};