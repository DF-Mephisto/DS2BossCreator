#include "ResumeDialog.h"

ResumeDialog::ResumeDialog(MemReader& reader, QVector<ChangeResume>& changes, QWidget *parent)
	: reader(reader), changes(changes), QDialog(parent)
{
	setWindowFlags(Qt::Window);
	setAttribute(Qt::WA_DeleteOnClose, true);
	setMinimumSize(600, 500);
	resize(600, 500);
	setWindowTitle("Changes list");

	QFile styleFile(":/DS2BossCreator/dark");
	styleFile.open(QFile::ReadOnly);
	QString style(styleFile.readAll());
	setStyleSheet(style);

	setAutoFillBackground(true);
	QPalette pal;
	pal.setBrush(backgroundRole(), QBrush(QPixmap(":/DS2BossCreator/space")));
	setPalette(pal);

	menu = new QMenu(this);
	enableItemAction = menu->addAction("Enable");
	deleteItemAction = menu->addAction("Remove");
	connect(enableItemAction, &QAction::triggered, [&] {
		enableItem(menuRow, menuState);
	});
	connect(deleteItemAction, &QAction::triggered, [&] {
		deleteItem(menuRow);
	});

	table = new QTableWidget(0, 2);
	table->horizontalHeader()->setStretchLastSection(true);
	table->setColumnWidth(0, 300);
	QStringList horHeaders;
	horHeaders << "Name" << "Address";
	table->setHorizontalHeaderLabels(horHeaders);
	table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	table->verticalHeader()->hide();
	table->setFrameStyle(QFrame::NoFrame);
	table->setSelectionBehavior(QAbstractItemView::SelectRows);
	FocusControlDelegate* focusDelegate = new FocusControlDelegate(table);
	focusDelegate->setEditableEnabled(false);
	table->setItemDelegate(focusDelegate);
	table->setContextMenuPolicy(Qt::CustomContextMenu);

	enableAll = new QPushButton("Enable All");
	disableAll = new QPushButton("Disable All");

	QHBoxLayout* btnLayout = new QHBoxLayout;
	btnLayout->addWidget(enableAll, 1);
	btnLayout->addWidget(disableAll, 1);
	btnLayout->setSpacing(40);
	btnLayout->setContentsMargins(20, 0, 20, 0);

	QVBoxLayout* globalLayout = new QVBoxLayout(this);
	globalLayout->addWidget(table);
	globalLayout->addLayout(btnLayout);
	globalLayout->setContentsMargins(0, 0, 0, 20);
	globalLayout->insertSpacing(1, 20);

	connect(enableAll, &QPushButton::clicked, [&] {
		enableAllItems(true);
	});
	connect(disableAll, &QPushButton::clicked, [&] {
		enableAllItems(false);
	});
	connect(table, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(handleContextMenu(const QPoint&)));

	updateTable();
}

void ResumeDialog::updateTable()
{
	table->setRowCount(changes.size());

	QTableWidgetItem* item = nullptr;
	for (int i = 0; i < changes.size(); i++)
	{
		item = new QTableWidgetItem;
		item->setText(changes[i].getName());
		if (changes[i].getEnabled())
			item->setBackground(QBrush(qRgb(46, 139, 87)));
		table->setItem(i, 0, item);

		item = new QTableWidgetItem;
		item->setText(QString("%1").arg(getAddress(changes[i].getType(), changes[i].getOffset()), 0, 16).toUpper());
		if (changes[i].getEnabled())
			item->setBackground(QBrush(qRgb(46, 139, 87)));
		table->setItem(i, 1, item);
	}
}

void ResumeDialog::enableItem(int index, bool state)
{
	ChangeResume& resume = changes[index];

	byte* data;
	if (state)
		data = (byte*)resume.getNewValue().data();
	else
		data = (byte*)resume.getOldValue().data();

	if (!reader.writeMemory((LPVOID)(getAddress(resume.getType(), resume.getOffset()) - getBaseOffset(resume.getType())),
		(LPVOID)data, resume.getTypeSize()))
	{
		QMessageBox(QMessageBox::Critical, "Error", QString("Couldn't process data at %1 index").arg(index)).exec();
		return;
	}

	resume.setEnabled(state);
	updateTable();
}

void ResumeDialog::deleteItem(int index)
{
	ChangeResume& resume = changes[index];
	reader.writeMemory((LPVOID)(getAddress(resume.getType(), resume.getOffset()) - getBaseOffset(resume.getType())),
		(LPVOID)resume.getOldValue().data(), resume.getTypeSize());

	changes.erase(changes.begin() + index);
	updateTable();
}

void ResumeDialog::enableAllItems(bool state)
{
	if (!reader.getProcessStatus())
	{
		QMessageBox(QMessageBox::Critical, "Error", "DarkSoulsII.exe process not attached").exec();
		return;
	}

	for (int i = 0; i < changes.size(); i++)
		enableItem(i, state);
}

void ResumeDialog::handleContextMenu(const QPoint& pos)
{
	QTableWidgetItem *item = table->itemAt(pos);
	if (item) {
		menuRow = item->row();
		menuState = !changes[menuRow].getEnabled();
		enableItemAction->setText(menuState ? "Enable" : "Disable");
		
		menu->exec(QCursor::pos());
	}
}

DWORD64 ResumeDialog::getAddress(ChangeType type, DWORD64 offset)
{
	switch (type)
	{
	case CommonBulletChange:
		return reader.readPointer(commonBulletParamPointer, paramPointerLength) + offset;

	case EnemyBulletChange:
		return reader.readPointer(enemyBulletParamPointer, paramPointerLength) + offset;

	case WorldBulletChange:
		return reader.readPointer(worldBulletParamPointer, paramPointerLength) + offset;

	case MoveChange:
		return reader.readPointer(moveParamPointer, paramPointerLength) + offset;

	case MovesetChange:
		return reader.readPointer(movesetParamPointer, paramPointerLength) + offset;

	default:
		return 0;
	}
}

int ResumeDialog::getBaseOffset(ChangeType type)
{
	switch (type)
	{
	case CommonBulletChange:
	case EnemyBulletChange:
	case WorldBulletChange:
		return spellBaseOffset;

	case MoveChange:
		return moveBaseOffset;

	case MovesetChange:
		return 0;

	default:
		return 0;
	}
}