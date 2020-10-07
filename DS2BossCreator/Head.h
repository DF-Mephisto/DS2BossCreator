#pragma once

#include <QtWidgets>
#include <vector>

#include "ChangeResume.h"
#include "ResumeDialog.h"
#include "MemReader.h"
#include "SpellIdDialog.h"

using namespace std;

class Head : public QWidget
{
	Q_OBJECT

public:
	Head(MemReader& reader, QWidget *parent = Q_NULLPTR);

private:
	QPushButton* openResumeDialogBtn;
	QPushButton* openSpellIdDialogBtn;
	QPushButton* saveBtn;
	QPushButton* loadBtn;
	QPushButton* attachProcessBtn;
	QCheckBox* equipmentDamageChb;
	QPointer<ResumeDialog> resumeDialog;
	QPointer<SpellIdDialog> spellIdDialog;

	QVector<ChangeResume> changes;
	MemReader& reader;

private slots:
	void addResume(ChangeResume resume);
	void openResumeDialog();
	void openSpellIdDialog();
	void serializeData();
	void deserializeData();
	void attachProcess(bool state);
	void enableEquipmentDamage(bool state);

signals:
	void updateResumeDialog();

};
