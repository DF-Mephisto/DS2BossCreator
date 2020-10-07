#include "Helper.h"

Helper::Helper(MemReader& reader, HelperType helperType, int baseOffset, int entriesCount, int structureSize, int instructionSize, DWORD signatureSize,
	const byte* instruction, const EntityDescription* entityDescriptions, const char* mask, LPVOID hookScript, QString entityName, QWidget *parent)
	: reader(reader), baseOffset(baseOffset), entriesCount(entriesCount), 
	structureSize(structureSize), instructionSize(instructionSize), signatureSize(signatureSize), instruction(instruction),
	entityDescriptions(entityDescriptions), mask(mask), QWidget(parent)
{
	this->helperType = helperType;
	this->entityName = entityName;
	this->hookScript = hookScript;
	entityData = new byte[structureSize];

	mainAddress = new Entry(entityName + " address:", getValidator(QWordType));
	name = new Entry(entityName + " name:", getValidator(StringType));

	//MAKE TABLE
	table = new QTableWidget(entriesCount, 3);
	table->horizontalHeader()->setStretchLastSection(true);
	table->setColumnWidth(0, 300);
	table->setColumnWidth(1, 100);
	QStringList horHeaders;
	horHeaders << "Description" << "Type" << "Value";
	table->setHorizontalHeaderLabels(horHeaders);
	table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	table->verticalHeader()->hide();
	table->setFrameStyle(QFrame::NoFrame);
	table->setSelectionBehavior(QAbstractItemView::SelectRows);
	table->setItemDelegate(new FocusControlDelegate(table));
	fillTable();

	load = new QPushButton("Read");
	load->setMinimumHeight(40);
	save = new QPushButton("Write");
	save->setMinimumHeight(40);
	getEntities = new QPushButton("Get " + entityName);
	getEntities->setMinimumHeight(40);

	entryLayout = new QVBoxLayout;
	entryLayout->addWidget(mainAddress, 0.5);
	entryLayout->addWidget(name, 0.5);
	entryLayout->addWidget(table, 5);
	entryLayout->insertSpacing(1, -10);

	controlLayout = new QVBoxLayout;
	controlLayout->setAlignment(Qt::AlignTop);
	controlLayout->addWidget(load);
	controlLayout->addWidget(save);
	controlLayout->addWidget(getEntities);
	controlLayout->setContentsMargins(0, 5, 5, 0);

	globalLayout = new QHBoxLayout(this);
	QWidget* vBorder = new QWidget;
	vBorder->setFixedWidth(3);
	vBorder->setStyleSheet("background-color: #191940;");
	globalLayout->addLayout(entryLayout, 4);
	globalLayout->addWidget(vBorder);
	globalLayout->addLayout(controlLayout, 1);
	globalLayout->setMargin(0);
	setLayout(globalLayout);

	connect(load, SIGNAL(clicked()), this, SLOT(loadEntity()));
	connect(save, SIGNAL(clicked()), this, SLOT(saveEntity()));
	connect(getEntities, SIGNAL(clicked()), this, SLOT(openHookDialog()));
	
	for (int i = 0; i < structureSize; i++)
		entityData[i] = 0;

}

Helper::~Helper()
{
	delete[] entityData;
}

void Helper::fillTable()
{
	QTableWidgetItem* item = nullptr;

	for (int i = 0; i < entriesCount; i++)
	{
		EntryType type = entityDescriptions[i].type;
		QString name = entityDescriptions[i].name;
		QString tip = entityDescriptions[i].tip;

		item = new QTableWidgetItem;
		item->setText(name);
		item->setFlags(item->flags() ^ Qt::ItemIsEditable);
		item->setToolTip(tip);
		table->setItem(i, 0, item);

		item = new QTableWidgetItem;
		item->setText(entryTypeNames[type]);
		item->setFlags(item->flags() ^ Qt::ItemIsEditable);
		table->setItem(i, 1, item);

		QLineEdit *edit = new QLineEdit(table);
		edit->setValidator(getValidator(type));
		table->setCellWidget(i, 2, edit);
	}
}

void Helper::openHookDialog()
{
	if (!hookDialog)
	{
		if (!reader.getProcessStatus())
		{
			QMessageBox(QMessageBox::Critical, "Error", "DarkSoulsII.exe process not attached").exec();
			return;
		}

		hookDialog = new HookDialog(reader, instructionSize, signatureSize, hookScript, instruction, mask);
		hookDialog->setWindowTitle(entityName + " helper");
		hookDialog->show();
	}
	else
	{
		hookDialog->activateWindow();
	}
}

void Helper::loadEntity()
{
	if (!reader.getProcessStatus())
	{
		QMessageBox(QMessageBox::Critical, "Error", "DarkSoulsII.exe process not attached").exec();
		return;
	}

	bool converted = false;
	DWORD64 entityAddr = mainAddress->getValue().toULongLong(&converted, 16);

	if (!converted || !reader.readMemory((LPVOID)(entityAddr - baseOffset), entityData, structureSize))
	{
		QMessageBox(QMessageBox::Critical, "Error", "Invalid address").exec();
		return;
	}

	for (int i = 0; i < entriesCount; i++)
	{
		EntryType type = entityDescriptions[i].type;

		switch (type)
		{
		case ByteType:
		{
			((QLineEdit*)table->cellWidget(i, 2))->
				setText(QString::number(*((byte*)(entityData + entityDescriptions[i].offset + baseOffset))));
			break;
		}

		case DWordType:
		{
			QString val = QString("%1").arg(*((DWORD*)(entityData + entityDescriptions[i].offset + baseOffset)), 0, 16).toUpper();
			((QLineEdit*)table->cellWidget(i, 2))->setText(val);
			break;
		}

		case QWordType:
		{
			QString val = QString("%1").arg(*((DWORD64*)(entityData + entityDescriptions[i].offset + baseOffset)), 0, 16).toUpper();
			((QLineEdit*)table->cellWidget(i, 2))->setText(val);
			break;
		}

		case FloatType:
		{
			((QLineEdit*)table->cellWidget(i, 2))->
				setText(QString::number(*((float*)(entityData + entityDescriptions[i].offset + baseOffset))));
			break;
		}
		}
	}
}

void Helper::saveEntity()
{
	if (!reader.getProcessStatus())
	{
		QMessageBox(QMessageBox::Critical, "Error", "DarkSoulsII.exe process not attached").exec();
		return;
	}

	bool converted = false;
	DWORD64 entityAddr = mainAddress->getValue().toULongLong(&converted, 16);

	if (!converted)
	{
		QMessageBox(QMessageBox::Critical, "Error", "Invalid address").exec();
		return;
	}

	ChangeResume resume;
	resume.setName(name->getValue());
	resume.setType(getChangeType(entityAddr));
	resume.setOffset(getOffset(entityAddr));
	resume.setEnabled(true);
	resume.setOldValue(QByteArray((char*)entityData, structureSize));

	try
	{
		int offset;
		EntryType type;
		bool doubleValue;

		for (int i = 0; i < entriesCount; i++)
		{
			offset = entityDescriptions[i].offset;
			type = entityDescriptions[i].type;
			doubleValue = entityDescriptions[i].doubleValue;

			switch (type)
			{
			case ByteType:
			{
				byte val = ((QLineEdit*)table->cellWidget(i, 2))->text().toInt(&converted, 10);

				if (!converted)
					throw converted;

				*((byte*)(entityData + offset + baseOffset)) = val;
				break;
			}

			case DWordType:
			{
				DWORD val = ((QLineEdit*)table->cellWidget(i, 2))->text().toULong(&converted, 16);

				if (!converted)
					throw converted;

				*((DWORD*)(entityData + offset + baseOffset)) = val;

				if (doubleValue)
					*((DWORD*)(entityData + offset + baseOffset + 4)) = val;

				break;
			}

			case QWordType:
			{
				DWORD64 val = ((QLineEdit*)table->cellWidget(i, 2))->text().toULongLong(&converted, 16);

				if (!converted)
					throw converted;

				*((DWORD64*)(entityData + offset + baseOffset)) = val;
				break;
			}

			case FloatType:
			{
				float val = ((QLineEdit*)table->cellWidget(i, 2))->text().toDouble(&converted);

				if (!converted)
					throw converted;

				*((float*)(entityData + offset + baseOffset)) = val;
				break;
			}
			}
		}
	}
	catch (bool& e)
	{
		QMessageBox(QMessageBox::Critical, "Error", "Invalid data").exec();
		return;
	}

	if (!reader.writeMemory((LPVOID)(entityAddr - baseOffset), entityData, structureSize))
	{
		QMessageBox(QMessageBox::Critical, "Error", "Couldn't save data").exec();
		return;
	}

	resume.setNewValue(QByteArray((char*)entityData, structureSize));
	emit saveResume(resume);
}

ChangeType Helper::getChangeType(DWORD64 address)
{
	switch (helperType)
	{
	case BulletHelperType:
	{
		if (address > reader.readPointer(worldBulletParamPointer, paramPointerLength))
			return WorldBulletChange;

		else if (address > reader.readPointer(enemyBulletParamPointer, paramPointerLength))
			return EnemyBulletChange;

		else if (address > reader.readPointer(commonBulletParamPointer, paramPointerLength))
			return CommonBulletChange;

		else
			return NoneChange;
	}
	case MoveHelperType:
		return MoveChange;
	}
}

DWORD64 Helper::getOffset(DWORD64 address)
{
	switch (getChangeType(address))
	{
	case WorldBulletChange:
		return address - reader.readPointer(worldBulletParamPointer, paramPointerLength);

	case EnemyBulletChange:
		return address - reader.readPointer(enemyBulletParamPointer, paramPointerLength);

	case CommonBulletChange:
		return address - reader.readPointer(commonBulletParamPointer, paramPointerLength);

	case MoveChange:
		return address - reader.readPointer(moveParamPointer, paramPointerLength);

	case NoneChange:
		return 0;
	}
}