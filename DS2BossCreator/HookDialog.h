#pragma once

#include <QtWidgets>

#include "MemReader.h"
#include "FocusControlDelegate.h"

class HookDialog : public QDialog
{
	Q_OBJECT

public:
	HookDialog(MemReader& reader, DWORD instructionSize, DWORD signatureSize, LPVOID script, 
		const byte* instruction, const char* mask, QWidget *parent = Q_NULLPTR);
	~HookDialog();

private:
	QTableWidget* table;
	QPushButton* stop;

	QVBoxLayout* layout;

	MemReader& reader;
	byte* data;
	LPVOID dataAddress;
	int hookKey;
	int timerIndex;
	const int dataSize;

	void setHook(DWORD instructionSize, DWORD signatureSize, LPVOID script, const byte* instruction, const char* mask);

protected:
	void timerEvent(QTimerEvent* e) override;

private slots:
	void stopHookTimer();

signals:
	void stopHook();

};