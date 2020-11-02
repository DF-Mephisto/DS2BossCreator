#include "ResizableScrollArea.h"

ResizableScrollArea::ResizableScrollArea(QWidget *parent) : QScrollArea(parent)
{
	
}

void ResizableScrollArea::resizeEvent(QResizeEvent* e)
{
	QScrollArea::resizeEvent(e);
	/*if (widget()->height() < e->size().height())
		widget()->setFixedHeight(e->size().height());

	if (widget()->width() < e->size().width())
		widget()->setFixedWidth(e->size().width());*/
}