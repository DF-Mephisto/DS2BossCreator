#include "SpellIdDialog.h"

SpellIdDialog::SpellIdDialog(QWidget *parent) 
	: QDialog(parent)
{
	setWindowFlags(Qt::Window);
	setAttribute(Qt::WA_DeleteOnClose, true);
	setMinimumSize(500, 500);
	resize(500, 600);
	setWindowTitle("Bullet list");

	QFile styleFile(":/DS2BossCreator/dark");
	styleFile.open(QFile::ReadOnly);
	QString style(styleFile.readAll());
	setStyleSheet(style);

	setAutoFillBackground(true);
	QPalette pal;
	pal.setBrush(backgroundRole(), QBrush(QPixmap(":/DS2BossCreator/space")));
	setPalette(pal);

	mainTab = new QTabWidget;
	commonSpellsTab = new QTabWidget(mainTab);

	QTableWidget* idList = makeTable(sorceries);
	idList->setParent(commonSpellsTab);
	commonSpellsTab->addTab(idList, "Sorceries");

	idList = makeTable(pyromancies);
	idList->setParent(commonSpellsTab);
	commonSpellsTab->addTab(idList, "Pyromancies");

	idList = makeTable(miracles);
	idList->setParent(commonSpellsTab);
	commonSpellsTab->addTab(idList, "Miracles");

	idList = makeTable(hexes);
	idList->setParent(commonSpellsTab);
	commonSpellsTab->addTab(idList, "Hexes");

	idList = makeTable(special);
	idList->setParent(commonSpellsTab);
	commonSpellsTab->addTab(idList, "Special");

	mainTab->addTab(commonSpellsTab, "Common bullets");

	idList = makeTable(enemyBullets);
	idList->setParent(mainTab);
	mainTab->addTab(idList, "Enemy bullets");

	idList = makeTable(worldBullets);
	idList->setParent(mainTab);
	mainTab->addTab(idList, "World bullets");

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(mainTab);
	layout->setMargin(0);

	setLayout(layout);
}

QTableWidget* SpellIdDialog::makeTable(const QMap<QString, QString>& values)
{
	QTableWidget* table = new QTableWidget(values.size(), 2);
	table->horizontalHeader()->setStretchLastSection(true);
	table->setColumnWidth(0, 120);
	QStringList horHeaders;
	horHeaders << "ID" << "Name";
	table->setHorizontalHeaderLabels(horHeaders);
	table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	table->setFrameStyle(QFrame::NoFrame);
	table->setShowGrid(false);
	table->setSelectionBehavior(QAbstractItemView::SelectRows);
	FocusControlDelegate* focusDelegate = new FocusControlDelegate(table);
	focusDelegate->setEditableEnabled(false);
	table->setItemDelegate(focusDelegate);

	QTableWidgetItem* item = nullptr;
	int index = 0;

	for (auto row : values.keys())
	{
		item = new QTableWidgetItem;
		item->setText(row);
		table->setItem(index, 0, item);

		item = new QTableWidgetItem;
		item->setText(values[row]);
		table->setItem(index, 1, item);

		index++;
	}

	table->setSortingEnabled(true);
	table->sortByColumn(0, Qt::AscendingOrder);

	return table;
}