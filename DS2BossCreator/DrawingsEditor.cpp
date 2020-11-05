#include "DrawingsEditor.h"

DrawingsEditor::DrawingsEditor(MemReader& reader, QWidget *parent) 
	: reader(reader), QWidget(parent)
{
	enabled = false;
	originalCoords = new byte[12];

	address = new Entry("Spell address: ", getValidator(QWordType));
	space = new Entry("Space between bullets: ", getValidator(FloatType));
	space->setValue("1.0");

	netSizeValidator = new QRegExpValidator(QRegExp("[1-9][0-9]{,2}"), this);
	xCount = new Entry("Horizontal size: ", netSizeValidator);
	yCount = new Entry("Vertical size: ", netSizeValidator);
	layersCount = new Entry("Layers: ", netSizeValidator);
	xCount->setValue("15");
	yCount->setValue("9");
	layersCount->setValue("1");
	QPushButton* setBlocksBtn = new QPushButton("Set");

	QHBoxLayout* netSizeLayout = new QHBoxLayout;
	netSizeLayout->addWidget(xCount, 2);
	netSizeLayout->addWidget(yCount, 2);
	netSizeLayout->addWidget(layersCount, 2);
	netSizeLayout->addWidget(setBlocksBtn, 1);
	netSizeLayout->insertSpacing(3, 40);

	layerCb = new QComboBox;
	layerCb->setMinimumHeight(24);
	currentLayer = new QLabel("Current layer: ");
	bulletsLeftLbl = new QLabel;
	bulletsLeftLbl->setAlignment(Qt::AlignCenter);

	QHBoxLayout* layersLayout = new QHBoxLayout;
	layersLayout->addWidget(currentLayer, 1);
	layersLayout->addWidget(layerCb, 2);
	layersLayout->addWidget(bulletsLeftLbl, 2);
	layersLayout->setMargin(11);
	layersLayout->insertSpacing(2, 50);

	ResizableScrollArea* netSA = new ResizableScrollArea;
	net = new DrawingsNet();
	netSA->setWidget(net);
	connect(net, &DrawingsNet::changedBulletsLeft, this, &DrawingsEditor::setBulletsLeft);
	setNetSize();

	QVBoxLayout* settingLayout = new QVBoxLayout;
	settingLayout->addWidget(address, 0.5);
	settingLayout->addWidget(space, 0.5);
	settingLayout->addLayout(netSizeLayout, 0.5);
	settingLayout->addLayout(layersLayout, 0.5);
	settingLayout->addWidget(netSA, 5);
	settingLayout->insertSpacing(1, -20);
	settingLayout->insertSpacing(3, -10);
	settingLayout->insertSpacing(5, -10);

	enableBtn = new QPushButton("Enable");
	refreshBtn = new QPushButton("Refresh");
	refreshBtn->setEnabled(false);

	QVBoxLayout* controlLayout = new QVBoxLayout;
	controlLayout->setAlignment(Qt::AlignTop);
	controlLayout->addWidget(enableBtn);
	controlLayout->addWidget(refreshBtn);
	controlLayout->setContentsMargins(0, 5, 5, 0);

	QHBoxLayout* globalLayout = new QHBoxLayout(this);
	QWidget* vBorder = new QWidget;
	vBorder->setFixedWidth(3);
	vBorder->setStyleSheet("background-color: #191940;");
	globalLayout->addLayout(settingLayout, 4);
	globalLayout->addWidget(vBorder);
	globalLayout->addLayout(controlLayout, 1);
	globalLayout->setMargin(0);
	setLayout(globalLayout);

	connect(setBlocksBtn, SIGNAL(clicked()), this, SLOT(setNetSize()));
	connect(enableBtn, SIGNAL(clicked()), this, SLOT(enableScript()));
	connect(refreshBtn, SIGNAL(clicked()), this, SLOT(refreshScript()));
	connect(layerCb, qOverload<int>(&QComboBox::currentIndexChanged), this, &DrawingsEditor::layerChanged);
}

DrawingsEditor::~DrawingsEditor()
{
	delete[] originalCoords;
}

void DrawingsEditor::setNetSize()
{
	bool converted = false;
	int x, y, z;

	try
	{
		z = layersCount->getValue().toInt(&converted);
		if (!converted) throw converted;

		x = xCount->getValue().toInt(&converted);
		if (!converted) throw converted;

		y = yCount->getValue().toInt(&converted);
		if (!converted) throw converted;
	}
	catch (bool& e)
	{
		QMessageBox(QMessageBox::Critical, "Error", "Invalid data").exec();
		return;
	}

	fillLayersCb(z);
	layerCb->setCurrentIndex(0);
	net->setBlockCount(x, y, z);
}

void DrawingsEditor::fillLayersCb(int count)
{
	QStringList cbItems;

	for (int i = 0; i < count; i++)
	{
		cbItems << QString::number(i + 1);
	}

	layerCb->clear();
	layerCb->addItems(cbItems);
	layerCb->setCurrentIndex(0);
}

void DrawingsEditor::setBulletsLeft(int count)
{
	bulletsLeftLbl->setText("Bullets left: " + QString::number(count));
}

void DrawingsEditor::layerChanged(int index)
{
	if (index >= 0)
		net->setCurrentLayer(index);
}

void DrawingsEditor::enableScript()
{
	if (!reader.getProcessStatus())
	{
		QMessageBox(QMessageBox::Critical, "Error", "DarkSoulsII.exe process not attached").exec();
		return;
	}

	if (!enabled)
		setHook();
	else
		unhook();

}

void DrawingsEditor::refreshScript()
{
	float spaceVal;
	DWORD64 newAddress;
	bool converted = false;

	try
	{
		newAddress = address->getValue().toULongLong(&converted, 16);
		if (!converted) throw converted;

		spaceVal = space->getValue().toFloat(&converted);
		if (!converted) throw converted;
	}
	catch (bool& e)
	{
		QMessageBox(QMessageBox::Critical, "Error", "Invalid data").exec();
		return;
	}

	reader.writeMemory((LPVOID)(currentAddress - 1), (LPVOID)&originalBulletCount, 1);
	reader.writeMemory((LPVOID)(currentAddress - 0x27), (LPVOID)originalCoords, 12);
	currentAddress = newAddress;

	writeData(spaceVal);
}

void DrawingsEditor::setHook()
{
	float spaceVal;
	bool converted = false;

	try
	{
		currentAddress = address->getValue().toULongLong(&converted, 16);
		if (!converted) throw converted;

		spaceVal = space->getValue().toFloat(&converted);
		if (!converted) throw converted;
	}
	catch (bool& e)
	{
		QMessageBox(QMessageBox::Critical, "Error", "Invalid data").exec();
		return;
	}

	hookKey = reader.hookMemory(drawOriginalInstructionSize, drawSignatureSize, DrawingsScript, drawInstruction, drawMask);

	if (hookKey == -1)
	{
		QMessageBox(QMessageBox::Critical, "Error", QString("Couldn't make a hook.\nClose spell helper and try again")).exec();
		return;
	}
	else
	{
		enabled = true;
		refreshBtn->setEnabled(true);
		enableBtn->setText("Disable");

		dataAddress = (LPVOID)reader.getHookDataAddress(hookKey);
		writeData(spaceVal);
	}
}

void DrawingsEditor::unhook()
{
	if (!enabled)
		return;

	reader.writeMemory((LPVOID)(currentAddress - 1), (LPVOID)&originalBulletCount, 1);
	reader.writeMemory((LPVOID)(currentAddress - 0x27), (LPVOID)originalCoords, 12);

	reader.unhookMemory(hookKey);

	enabled = false;
	refreshBtn->setEnabled(false);
	enableBtn->setText("Enable");
}

void  DrawingsEditor::writeData(float spaceVal)
{
	int totalBullets = net->getTotalBullets();
	int dataSize = 8 + 1 + 1 + totalBullets * 12;
	byte* data = new byte[dataSize];

	*(DWORD64*)data = currentAddress;
	*(data + 8) = (byte)totalBullets;
	*(data + 9) = (byte)0;

	bool*** bullets = net->getBullets();

	int dataOffset = 8 + 1 + 1;
	float posOffset = net->getXCount() * spaceVal / 2;
	for (int i = 0; i < net->getLayers(); i++)
		for (int j = 0; j < net->getYCount(); j++)
			for (int k = 0; k < net->getXCount(); k++)
				if (bullets[i][j][k])
				{
					*((float*)(data + dataOffset + 0)) = k * spaceVal - posOffset;
					*((float*)(data + dataOffset + 4)) = (net->getYCount() - 1 - j) * spaceVal;
					*((float*)(data + dataOffset + 8)) = i * spaceVal;

					dataOffset += 12;
				}

	reader.writeMemory(dataAddress, (LPVOID)data, dataSize);

	reader.readMemory((LPVOID)(currentAddress - 1), (LPVOID)&originalBulletCount, 1);
	reader.readMemory((LPVOID)(currentAddress - 0x27), (LPVOID)originalCoords, 12);
	reader.writeMemory((LPVOID)(currentAddress - 1), (LPVOID)&totalBullets, 1);

	delete[] data;
}
