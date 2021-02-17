#include "DS2BossCreator.h"

DS2BossCreator::DS2BossCreator(QWidget *parent) : QWidget(parent)
{
	QLocale::setDefault(QLocale(QLocale::English));
	setWindowTitle("Dark Souls II Boss Maker @By Mephisto");

	setWindowFlags(Qt::Window);
	//setMinimumSize(800, 700);

	QFile styleFile(":/DS2BossCreator/dark");
	styleFile.open(QFile::ReadOnly);
	QString style(styleFile.readAll());
	setStyleSheet(style);

	setAutoFillBackground(true);
	QPalette pal;
	pal.setBrush(backgroundRole(), QBrush(QPixmap(":/DS2BossCreator/space")));
	setPalette(pal);

	tab = new QTabWidget;

	head = new Head(reader);

	//EDITORS
	spellEditor = new Helper(reader, BulletHelperType, spellBaseOffset, spellEntriesCount, spellStructureSize,
		spellOriginalInstructionSize, spellSignatureSize, spellInstruction, spellDescriptions, spellMask, BulletHelper, "Spell", tab);
	tab->addTab(spellEditor, "Spells");

	moveEditor = new Helper(reader, MoveHelperType, moveBaseOffset, moveEntriesCount, moveStructureSize,
		moveOriginalInstructionSize, moveSignatureSize, moveInstruction, moveDescriptions, moveMask, MoveHelper, "Move", tab);
	tab->addTab(moveEditor, "Moves");

	movesetEditor = new MovesetEditor(reader, tab);
	tab->addTab(movesetEditor, "Movesets");

	weaponEditor = new WeaponEditor(reader, tab);
	tab->addTab(weaponEditor, "Weapons");

	drawingsEditor = new DrawingsEditor(reader, tab);
	tab->addTab(drawingsEditor, "Drawings");

	//LAYOUT
	layout = new QVBoxLayout(this);
	layout->addWidget(head);
	layout->addWidget(tab);
	layout->setMargin(0);
	setLayout(layout);

	connect(spellEditor, SIGNAL(saveResume(ChangeResume)), head, SLOT(addResume(ChangeResume)));
	connect(moveEditor, SIGNAL(saveResume(ChangeResume)), head, SLOT(addResume(ChangeResume)));
	connect(movesetEditor, SIGNAL(saveResume(ChangeResume)), head, SLOT(addResume(ChangeResume)));
	connect(weaponEditor, SIGNAL(saveResume(ChangeResume)), head, SLOT(addResume(ChangeResume)));
	connect(head, SIGNAL(processDetached()), drawingsEditor, SLOT(unhook()));
}

DS2BossCreator::~DS2BossCreator()
{
	
}

void DS2BossCreator::closeEvent(QCloseEvent* e)
{
	qApp->closeAllWindows();
}

void DS2BossCreator::paintEvent(QPaintEvent* e)
{
	QWidget::paintEvent(e);

	/*QPainter painter;
	painter.begin(this);

	QPixmap pix = QPixmap(":/DS2BossCreator/pursuer");
	QSize size = pix.size();
	pix = pix.scaled(pix.width(), height(), Qt::KeepAspectRatio);
	painter.drawPixmap(width() - pix.width(), height() - pix.height(), pix.width(), pix.height(), pix);

	painter.end();*/
}