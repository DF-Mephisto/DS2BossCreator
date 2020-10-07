#pragma once

#include <QtWidgets>

class Entry : public QWidget
{
	Q_OBJECT

public:
	Entry(QString name, QValidator* validator = Q_NULLPTR, QWidget *parent = Q_NULLPTR);

	QString getValue();
	void setValue(QString val);
	void setToolTip(QString tip);

private:
	QLabel* name;
	QLineEdit* value;

	QHBoxLayout* layout;
};