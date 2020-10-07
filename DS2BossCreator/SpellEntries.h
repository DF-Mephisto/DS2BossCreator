#include <QtWidgets>
#include <vector>

#include "Entry.h"
#include "EntityDescription.h"

using namespace std;
using namespace entity_description;

class SpellEntries : public QWidget
{
	Q_OBJECT

public:
	SpellEntries(QWidget *parent = Q_NULLPTR);

private:
	vector<Entry*> entries;
	QVBoxLayout* layout;

	void fillEntries();
};
