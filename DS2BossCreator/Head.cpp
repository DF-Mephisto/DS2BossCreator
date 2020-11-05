#include "Head.h"

Head::Head(MemReader& reader, QWidget *parent)
	: reader(reader), QWidget(parent)
{
	openResumeDialogBtn = new QPushButton("Changes list", this);
	openSpellIdDialogBtn = new QPushButton("Spell IDs", this);
	saveBtn = new QPushButton("Save", this);
	loadBtn = new QPushButton("Load", this);
	attachProcessBtn = new QPushButton("Attach Process", this);

	equipmentDamageChb = new QCheckBox("Disable Equpment Damage", this);

	QVBoxLayout* saveLoadLayout = new QVBoxLayout;
	saveLoadLayout->addWidget(saveBtn);
	saveLoadLayout->addWidget(loadBtn);
	saveLoadLayout->setAlignment(Qt::AlignTop);

	QHBoxLayout* dialogLayout = new QHBoxLayout;
	dialogLayout->addWidget(attachProcessBtn);
	dialogLayout->addWidget(openResumeDialogBtn);
	dialogLayout->addWidget(openSpellIdDialogBtn);

	QVBoxLayout* utilityLayout = new QVBoxLayout;
	utilityLayout->addLayout(dialogLayout);
	utilityLayout->addWidget(equipmentDamageChb);
	utilityLayout->setAlignment(Qt::AlignTop);

	QHBoxLayout* globalLayout = new QHBoxLayout(this);
	globalLayout->addLayout(utilityLayout, 2);
	globalLayout->addLayout(saveLoadLayout, 1);
	globalLayout->insertSpacing(1, 200);

	setLayout(globalLayout);

	connect(openResumeDialogBtn, SIGNAL(clicked()), this, SLOT(openResumeDialog()));
	connect(openSpellIdDialogBtn, SIGNAL(clicked()), this, SLOT(openSpellIdDialog()));
	connect(saveBtn, SIGNAL(clicked()), this, SLOT(serializeData()));
	connect(loadBtn, SIGNAL(clicked()), this, SLOT(deserializeData()));
	connect(equipmentDamageChb, SIGNAL(toggled(bool)), this, SLOT(enableEquipmentDamage(bool)));
	connect(attachProcessBtn, &QPushButton::clicked, [&] {
		attachProcess(!reader.getProcessStatus());
	});
}

void Head::addResume(ChangeResume resume)
{
	if (resume.getType() == NoneChange)
		return;

	for (int i = 0; i < changes.size(); i++)
	{
		if (resume == changes[i])
		{
			changes[i].setName(resume.getName());
			changes[i].setNewValue(resume.getNewValue());
			changes[i].setEnabled(resume.getEnabled());
			emit updateResumeDialog();
			return;
		}
	}

	changes.push_back(resume);
	emit updateResumeDialog();
}

void Head::enableEquipmentDamage(bool state)
{
	if (!reader.getProcessStatus())
	{
		equipmentDamageChb->setCheckState(Qt::Unchecked);
		return;
	}

	DWORD64 baseAddress = reader.getBaseAddress();
	DWORD64 moduleSize = reader.getModuleSize();
	static DWORD64 weaponDurabilityAddress = 0;
	static DWORD64 gearDurabilityAddress = 0;
	static DWORD64 ringDurabilityAddress = 0;
	static DWORD64 infDurabilityAddress = 0;

	if (state)
	{
		if (!(weaponDurabilityAddress = reader.findSignature(baseAddress, moduleSize, wepDuraSignatureSize, wepDuraSignature, wepDuraMask)) ||
			!(gearDurabilityAddress = reader.findSignature(baseAddress, moduleSize, gearDuraSignatureSize, gearDuraSignature, gearDuraMask)) ||
			!(ringDurabilityAddress = reader.findSignature(baseAddress, moduleSize, ringDuraSignatureSize, ringDuraSignature, ringDuraMask)) ||
			!(infDurabilityAddress = reader.findSignature(baseAddress, moduleSize, infDuraSignatureSize, infDuraSignature, infDuraMask)))
		{
			QMessageBox(QMessageBox::Critical, "Error", "Couldn't find durability signature").exec();
			equipmentDamageChb->setCheckState(Qt::Unchecked);
			return;
		}

		reader.writeMemory((LPVOID)weaponDurabilityAddress, (LPVOID)wepDuraNewInstruction, wepDuraInstructionSize);
		reader.writeMemory((LPVOID)gearDurabilityAddress, (LPVOID)gearDuraNewInstruction, gearDuraInstructionSize);
		reader.writeMemory((LPVOID)ringDurabilityAddress, (LPVOID)ringDuraNewInstruction, ringDuraInstructionSize);
		reader.writeMemory((LPVOID)infDurabilityAddress, (LPVOID)infDuraNewInstruction, infDuraInstructionSize);
	}
	else
	{
		reader.writeMemory((LPVOID)weaponDurabilityAddress, (LPVOID)wepDuraOldInstruction, wepDuraInstructionSize);
		reader.writeMemory((LPVOID)gearDurabilityAddress, (LPVOID)gearDuraOldInstruction, gearDuraInstructionSize);
		reader.writeMemory((LPVOID)ringDurabilityAddress, (LPVOID)ringDuraOldInstruction, ringDuraInstructionSize);
		reader.writeMemory((LPVOID)infDurabilityAddress, (LPVOID)infDuraOldInstruction, infDuraInstructionSize);
	}
}

void Head::attachProcess(bool state)
{
	if (state)
	{
		if (!reader.initialize())
		{
			QMessageBox(QMessageBox::Critical, "Error", "Couldn't find DarkSouls2.exe process").exec();
			return;
		}

		attachProcessBtn->setText("Detach process");
	}
	else
	{
		emit processDetached();
		equipmentDamageChb->setCheckState(Qt::Unchecked);
		reader.closeProc();
		attachProcessBtn->setText("Attach process");
	}
}

void Head::openResumeDialog()
{
	if (!resumeDialog)
	{
		if (!reader.getProcessStatus())
		{
			QMessageBox(QMessageBox::Critical, "Error", "DarkSoulsII.exe process not attached").exec();
			return;
		}

		resumeDialog = new ResumeDialog(reader, changes);
		resumeDialog->show();

		connect(this, SIGNAL(updateResumeDialog()), resumeDialog, SLOT(updateTable()));
	}
	else
	{
		resumeDialog->activateWindow();
	}
}

void Head::openSpellIdDialog()
{
	if (!spellIdDialog)
	{
		spellIdDialog = new SpellIdDialog;
		spellIdDialog->show();
	}
	else
	{
		spellIdDialog->activateWindow();
	}
}

void Head::serializeData()
{
	if (!reader.getProcessStatus())
	{
		QMessageBox(QMessageBox::Critical, "Error", "DarkSoulsII.exe process not attached").exec();
		return;
	}

	QString path = QFileDialog::getSaveFileName(nullptr, "Save file", "", "*.ds2bm");
	if (path.isEmpty())
		return;

	QFile file(path);
	if (!file.open(QIODevice::WriteOnly))
	{
		QMessageBox(QMessageBox::Critical, "Error", "Couldn't save file").exec();
		return;
	}

	QDataStream out(&file);
	out << 0xFFAAFF32 << changes;
	file.close();
}

void Head::deserializeData()
{
	QString path = QFileDialog::getOpenFileName(nullptr, "Open file", "", "*.ds2bm");
	if (path.isEmpty())
		return;

	QFile file(path);
	if (!file.open(QIODevice::ReadOnly))
	{
		QMessageBox(QMessageBox::Critical, "Error", "Couldn't load file").exec();
		return;
	}

	QDataStream in(&file);

	DWORD32 formatID;
	in >> formatID;
	if (formatID != 0xFFAAFF32)
	{
		QMessageBox(QMessageBox::Critical, "Error", "File is corrupted or has wrong format").exec();
		return;
	}

	in >> changes;
	file.close();
}