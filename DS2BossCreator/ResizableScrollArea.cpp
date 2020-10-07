#include "ResizableScrollArea.h"

ResizableScrollArea::ResizableScrollArea(QWidget *parent) : QScrollArea(parent)
{
	
}

void ResizableScrollArea::resizeEvent(QResizeEvent* e)
{
	widget()->setFixedWidth(e->size().width());
}