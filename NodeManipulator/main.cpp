#include "nodemanipulator.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	NodeManipulator w;
	w.show();
	return a.exec();
}
