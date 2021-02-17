#pragma once
#include <QtWidgets>

class FocusControlDelegate : public QStyledItemDelegate {
public:
	FocusControlDelegate(QObject *parent = 0);
	virtual void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const override;
	void setFocusBorderEnabled(bool enabled);
	void setEditableEnabled(bool enabled);
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

protected:
	bool f_focus_border_enabled;
	bool editable;
};