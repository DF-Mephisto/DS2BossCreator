#pragma once

#include <QtWidgets>

#include "EntityDescription.h"
#include "MemReader.h"
#include "DrawingsNet.h"
#include "Entry.h"
#include "ResizableScrollArea.h"

using namespace std;
using namespace entity_description;

class DrawingsEditor : public QWidget
{
	Q_OBJECT

public:
	DrawingsEditor(MemReader& reader, QWidget *parent = Q_NULLPTR);
	~DrawingsEditor();

private:
	Entry* address;
	Entry* space;
	Entry* xCount;
	Entry* yCount;
	Entry* layersCount;
	QRegExpValidator* netSizeValidator;
	QPushButton* setBlocksBtn;

	QLabel* bulletsLeftLbl;
	QLabel* currentLayer;
	QComboBox* layerCb;

	QPushButton* enableBtn;
	QPushButton* refreshBtn;

	DrawingsNet* net;

	MemReader& reader;
	DWORD64 currentAddress;
	LPVOID dataAddress;
	byte originalBulletCount;
	byte* originalCoords;
	int hookKey;
	bool enabled;

	void fillLayersCb(int count);
	void layerChanged(int index);
	void setHook();
	void writeData(float spaceVal);

private slots:
	void setNetSize();
	void setBulletsLeft(int count);
	void enableScript();
	void refreshScript();
	void unhook();
};
