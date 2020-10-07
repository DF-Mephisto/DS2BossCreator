#include "FocusControlDelegate.h"

FocusControlDelegate::FocusControlDelegate(QObject *parent) : QStyledItemDelegate(parent) {
	f_focus_border_enabled = false;
	editable = true;
}

void FocusControlDelegate::setFocusBorderEnabled(bool enabled) {
	f_focus_border_enabled = enabled;
}

void FocusControlDelegate::initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const {
	QStyledItemDelegate::initStyleOption(option, index);
	if (!f_focus_border_enabled && option->state & QStyle::State_HasFocus)
		option->state = option->state & ~QStyle::State_HasFocus;
}

void FocusControlDelegate::setEditableEnabled(bool enabled)
{
	editable = enabled;
}

void FocusControlDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	if (editable)
		QStyledItemDelegate::setModelData(editor, model, index);
}