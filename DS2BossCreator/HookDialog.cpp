#include "HookDialog.h"

HookDialog::HookDialog(MemReader& reader, DWORD instructionSize, DWORD signatureSize, LPVOID script, 
	const byte* instruction, const char* mask, QWidget *parent)
	: reader(reader), dataSize(255 * 12 + 1), QDialog(parent)
{
	setWindowFlags(Qt::Window);
	setAttribute(Qt::WA_DeleteOnClose, true);
	setMinimumSize(400, 500);

	QFile styleFile(":/DS2BossCreator/dark");
	styleFile.open(QFile::ReadOnly);
	QString style(styleFile.readAll());
	setStyleSheet(style);

	setAutoFillBackground(true);
	QPalette pal;
	pal.setBrush(backgroundRole(), QBrush(QPixmap(":/DS2BossCreator/space")));
	setPalette(pal);

	table = new QTableWidget(0, 2);
	table->horizontalHeader()->setStretchLastSection(true);
	table->setColumnWidth(0, 100);
	table->setColumnWidth(1, 200);
	QStringList horHeaders;
	horHeaders << "Count" << "Address";
	table->setHorizontalHeaderLabels(horHeaders);
	table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	table->verticalHeader()->hide();
	table->setFrameStyle(QFrame::NoFrame);
	table->setSelectionBehavior(QAbstractItemView::SelectRows);
	FocusControlDelegate* focusDelegate = new FocusControlDelegate(table);
	focusDelegate->setEditableEnabled(false);
	table->setItemDelegate(focusDelegate);

	stop = new QPushButton("Stop");
	QHBoxLayout* btnLayout = new QHBoxLayout;
	btnLayout->addWidget(stop, 0.5);
	btnLayout->setContentsMargins(20, 0, 20, 0);

	layout = new QVBoxLayout(this);
	layout->addWidget(table);
	layout->addLayout(btnLayout);
	layout->setContentsMargins(0, 0, 0, 20);
	layout->insertSpacing(1, 20);

	connect(stop, SIGNAL(clicked()), this, SLOT(stopHookTimer()));
	connect(this, SIGNAL(stopHook()), SLOT(stopHookTimer()));

	data = new byte[dataSize];
	setHook(instructionSize, signatureSize, script, instruction, mask);
}

HookDialog::~HookDialog()
{
	delete[] data;

	if (hookKey != -1)
	{
		reader.unhookMemory(hookKey);
	}
}

void HookDialog::setHook(DWORD instructionSize, DWORD signatureSize, LPVOID script, const byte* instruction, const char* mask)
{
	hookKey = reader.hookMemory(instructionSize, signatureSize, script, instruction, mask);

	if (hookKey == -1)
	{
		stop->setEnabled(false);
	}
	else
	{
		dataAddress = (LPVOID)reader.getHookDataAddress(hookKey);
		timerIndex = startTimer(200);
	}
}

void HookDialog::timerEvent(QTimerEvent* e)
{
	if (!reader.readMemory(dataAddress, data, dataSize))
	{
		emit stopHook();
		return;
	}
	
	byte bulletCount = data[0];
	
	QTableWidgetItem* item = nullptr;
	for (int i = table->rowCount(); i < bulletCount; i++)
	{
		table->insertRow(table->rowCount());
		item = new QTableWidgetItem;
		item->setFlags(item->flags() ^ Qt::ItemIsEditable);
		table->setItem(i, 0, item);

		item = new QTableWidgetItem;
		table->setItem(i, 1, item);
	}

	QString count;
	QString address;
	for (int i = 0; i < bulletCount; i++)
	{
		count = QString::number(*((DWORD*)(data + 1 + i * 12)));
		address = QString("%1").arg(*((DWORD64*)(data + 5 + i * 12)), 0, 16).toUpper();

		table->item(i, 0)->setText(count);
		table->item(i, 1)->setText(address);
	}
}

void HookDialog::stopHookTimer()
{
	killTimer(timerIndex);
	stop->setEnabled(false);
}