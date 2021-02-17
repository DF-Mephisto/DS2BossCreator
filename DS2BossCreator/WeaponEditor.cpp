#include "WeaponEditor.h"

WeaponEditor::WeaponEditor(MemReader& reader, QWidget *parent) : 
	reader(reader), QWidget(parent)
{
	entityData = new byte[weaponsStructureSize];
	name = new Entry("Weapon name:", getValidator(StringType));

	//MAKE COMBOBOX
	hand = new QComboBox;
	hand->setMinimumHeight(24);
	QStringList cbItems;
	cbItems << "R1" << "R2" << "R3" << "L1" << "L2" << "L3";
	hand->addItems(cbItems);
	hand->setCurrentIndex(0);

	QLabel* currentHand = new QLabel("Hand: ");

	QHBoxLayout* cbLayout = new QHBoxLayout;
	cbLayout->addWidget(currentHand, 1);
	cbLayout->addWidget(hand, 1);
	cbLayout->setMargin(11);

	//MAKE TABLE
	table = new QTableWidget(weaponsEntriesCount, 3);
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
	save = new QPushButton("Write");

	entryLayout = new QVBoxLayout;
	entryLayout->addLayout(cbLayout, 0.5);
	entryLayout->addWidget(name, 0.5);
	entryLayout->addWidget(table, 5);
	entryLayout->insertSpacing(1, -10);

	controlLayout = new QVBoxLayout;
	controlLayout->setAlignment(Qt::AlignTop);
	controlLayout->addWidget(load);
	controlLayout->addWidget(save);
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

	for (int i = 0; i < weaponsStructureSize; i++)
		entityData[i] = 0;

}

WeaponEditor::~WeaponEditor()
{
	delete[] entityData;
}

void WeaponEditor::fillTable()
{
	QTableWidgetItem* item = nullptr;

	for (int i = 0; i < weaponsEntriesCount; i++)
	{
		EntryType type = weaponsDescriptions[i].type;
		QString name = weaponsDescriptions[i].name;
		QString tip = weaponsDescriptions[i].tip;

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

void WeaponEditor::loadEntity()
{
	if (!reader.getProcessStatus())
	{
		QMessageBox(QMessageBox::Critical, "Error", "DarkSoulsII.exe process not attached").exec();
		return;
	}

	int handID = hand->currentIndex();

	if (!reader.readMemory((LPVOID)reader.readPointer(weaponsPointers[handID], weaponsPointerLength), entityData, weaponsStructureSize))
	{
		QMessageBox(QMessageBox::Critical, "Error", "Couldn't load data").exec();
		return;
	}

	for (int i = 0; i < weaponsEntriesCount; i++)
	{
		EntryType type = weaponsDescriptions[i].type;

		switch (type)
		{
		case ByteType:
		{
			((QLineEdit*)table->cellWidget(i, 2))->
				setText(QString::number(*((byte*)(entityData + weaponsDescriptions[i].offset))));
			break;
		}

		case WordType:
		{
			((QLineEdit*)table->cellWidget(i, 2))->
				setText(QString::number(*((WORD*)(entityData + weaponsDescriptions[i].offset))));
			break;
		}

		case DWordType:
		{
			QString val = QString("%1").arg(*((DWORD*)(entityData + weaponsDescriptions[i].offset)), 0, 16).toUpper();
			((QLineEdit*)table->cellWidget(i, 2))->setText(val);
			break;
		}

		case QWordType:
		{
			QString val = QString("%1").arg(*((DWORD64*)(entityData + weaponsDescriptions[i].offset)), 0, 16).toUpper();
			((QLineEdit*)table->cellWidget(i, 2))->setText(val);
			break;
		}

		case FloatType:
		{
			((QLineEdit*)table->cellWidget(i, 2))->
				setText(QString::number(*((float*)(entityData + weaponsDescriptions[i].offset))));
			break;
		}
		}
	}
}

void WeaponEditor::saveEntity()
{
	if (!reader.getProcessStatus())
	{
		QMessageBox(QMessageBox::Critical, "Error", "DarkSoulsII.exe process not attached").exec();
		return;
	}

	bool converted = false;
	int handID = hand->currentIndex();

	ChangeResume resume;
	resume.setName(name->getValue());
	resume.setType(WeaponsChange);
	resume.setOffset(reader.readPointer(weaponsPointers[handID], weaponsPointerLength) - reader.readPointer(weaponsParamPointer, paramPointerLength));
	resume.setEnabled(true);
	resume.setOldValue(QByteArray((char*)entityData, movesetStructureSize));

	try
	{
		int offset;
		EntryType type;

		for (int i = 0; i < weaponsEntriesCount; i++)
		{
			offset = weaponsDescriptions[i].offset;
			type = weaponsDescriptions[i].type;

			switch (type)
			{
			case ByteType:
			{
				byte val = ((QLineEdit*)table->cellWidget(i, 2))->text().toInt(&converted, 10);

				if (!converted)
					throw converted;

				*((byte*)(entityData + offset)) = val;
				break;
			}

			case WordType:
			{
				WORD val = ((QLineEdit*)table->cellWidget(i, 2))->text().toUShort(&converted, 10);

				if (!converted)
					throw converted;

				*((WORD*)(entityData + offset)) = val;
				break;
			}

			case DWordType:
			{
				DWORD val = ((QLineEdit*)table->cellWidget(i, 2))->text().toULong(&converted, 16);

				if (!converted)
					throw converted;

				*((DWORD*)(entityData + offset)) = val;
				break;
			}

			case QWordType:
			{
				DWORD64 val = ((QLineEdit*)table->cellWidget(i, 2))->text().toULongLong(&converted, 16);

				if (!converted)
					throw converted;

				*((DWORD64*)(entityData + offset)) = val;
				break;
			}

			case FloatType:
			{
				float val = ((QLineEdit*)table->cellWidget(i, 2))->text().toDouble(&converted);

				if (!converted)
					throw converted;

				*((float*)(entityData + offset)) = val;
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

	if (!reader.writeMemory((LPVOID)reader.readPointer(weaponsPointers[handID], weaponsPointerLength), entityData, weaponsStructureSize))
	{
		QMessageBox(QMessageBox::Critical, "Error", "Couldn't save data").exec();
		return;
	}

	resume.setNewValue(QByteArray((char*)entityData, weaponsStructureSize));
	emit saveResume(resume);
}