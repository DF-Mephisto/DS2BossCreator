#pragma once

#include <QtWidgets>
#include <Windows.h>

#include "EntityDescription.h"

using namespace entity_description;

enum ChangeType
{
	NoneChange,
	CommonBulletChange,
	EnemyBulletChange,
	WorldBulletChange,
	MoveChange,
	MovesetChange,
	WeaponsChange
};

class ChangeResume
{
public:
	ChangeResume();
	~ChangeResume();

	int getTypeSize();

	QString getName() const;
	void setName(QString name);
	DWORD64 getOffset()  const;
	void setOffset(DWORD64 offset);
	ChangeType getType()  const;
	void setType(ChangeType type);
	const QByteArray& getOldValue()  const;
	void setOldValue(const QByteArray& value);
	const QByteArray& getNewValue()  const;
	void setNewValue(const QByteArray& value);
	bool getEnabled();
	void setEnabled(bool enabled);

	bool operator== (const ChangeResume& obj);

	friend QDataStream &operator<<(QDataStream &out, const ChangeResume &resume);
	friend QDataStream &operator>>(QDataStream &in, ChangeResume &resume);

private:
	QString name;
	DWORD64 offset;
	ChangeType type;
	QByteArray oldValue;
	QByteArray newValue;
	bool enabled;
	
};
Q_DECLARE_METATYPE(ChangeResume)