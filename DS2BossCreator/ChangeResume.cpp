#include "ChangeResume.h"

ChangeResume::ChangeResume()
{
	name = "";
	offset = 0;
	type = NoneChange;
	enabled = true;
}

ChangeResume::~ChangeResume()
{

}

QString ChangeResume::getName()  const
{
	return name;
}

void ChangeResume::setName(QString name)
{
	this->name = name;
}

DWORD64 ChangeResume::getOffset()  const
{
	return offset;
}

void ChangeResume::setOffset(DWORD64 offset)
{
	this->offset = offset;
}

ChangeType ChangeResume::getType()  const
{
	return type;
}

void ChangeResume::setType(ChangeType type)
{
	this->type = type;
}

bool ChangeResume::getEnabled()
{
	return enabled;
}

void ChangeResume::setEnabled(bool enabled)
{
	this->enabled = enabled;
}

const QByteArray& ChangeResume::getOldValue()  const
{
	return oldValue;
}

void ChangeResume::setOldValue(const QByteArray& value)
{
	oldValue = value;
}

const QByteArray& ChangeResume::getNewValue()  const
{
	return newValue;
}

void ChangeResume::setNewValue(const QByteArray& value)
{
	newValue = value;
}

int ChangeResume::getTypeSize()
{
	switch (type)
	{
	case CommonBulletChange:
	case EnemyBulletChange:
	case WorldBulletChange:
		return spellStructureSize;

	case MoveChange:
		return moveStructureSize;

	case MovesetChange:
		return movesetStructureSize;

	case WeaponsChange:
		return weaponsStructureSize;

	case NoneChange:
	default:
		return 0;
	}
}

bool ChangeResume::operator== (const ChangeResume& obj)
{
	return type == obj.type && offset == obj.offset;
}

QDataStream &operator<<(QDataStream &out, const ChangeResume &resume)
{
	out << resume.getName() << resume.getOffset() << (quint32)resume.getType()
		<< resume.getOldValue() << resume.getNewValue();
	return out;
}

QDataStream &operator>>(QDataStream &in, ChangeResume &resume)
{
	QString name;
	DWORD64 offset;
	quint32 type;
	QByteArray oldValue;
	QByteArray newValue;

	in >> name;
	in >> offset;
	in >> type;
	in >> oldValue;
	in >> newValue;
	
	resume.setName(name);
	resume.setOffset(offset);
	resume.setType((ChangeType)type);
	resume.setOldValue(oldValue);
	resume.setNewValue(newValue);
	resume.setEnabled(false);

	return in;
}