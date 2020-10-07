#include <QtWidgets>

#include "DS2BossCreator.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	

	app.setWindowIcon(QIcon(":/DS2BossCreator/eye"));
	DS2BossCreator w;
	w.show();
	return app.exec();
}
