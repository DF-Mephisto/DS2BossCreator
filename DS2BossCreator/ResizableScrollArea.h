#pragma once

#include <QtWidgets>

class ResizableScrollArea : public QScrollArea
{
	Q_OBJECT

public:
	ResizableScrollArea(QWidget *parent = Q_NULLPTR);

protected:
	void resizeEvent(QResizeEvent* e) override;
};