#include "Entry.h"

Entry::Entry(QString name, QValidator* validator, QWidget *parent) : QWidget(parent)
{
	this->name = new QLabel;
	this->name->setText(name);

	value = new QLineEdit;

	if (validator != Q_NULLPTR)
		value->setValidator(validator);

	layout = new QHBoxLayout(this);
	layout->addWidget(this->name, 1);
	layout->addWidget(value, 1);
	setLayout(layout);
}

QString Entry::getValue()
{
	return value->text();
}

void Entry::setValue(QString val)
{
	value->setText(val);
}

void Entry::setToolTip(QString tip)
{
	name->setToolTip(tip);
}