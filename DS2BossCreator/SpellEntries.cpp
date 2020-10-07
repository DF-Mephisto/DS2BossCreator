#include "SpellEntries.h"

SpellEntries::SpellEntries(QWidget *parent) : QWidget(parent)
{
	fillEntries();

	layout = new QVBoxLayout(this);
	for (int i = 0; i < spellEntriesCount; i++)
	{
		layout->addWidget(entries[i]);
	}

	layout->setAlignment(Qt::AlignTop);
	layout->setMargin(0);
	layout->setSpacing(0);
	setLayout(layout);
}

void SpellEntries::fillEntries()
{
	for (int i = 0; i < spellEntriesCount; i++)
	{
		EntryType type = spellDescriptions[i].type;
		QString name = spellDescriptions[i].name;
		QString tip = spellDescriptions[i].tip;

		Entry* entry = new Entry(name, getValidator(type));
		entry->setToolTip(tip);
		entries.push_back(entry);
	}
}