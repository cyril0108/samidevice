#include "fakedevice.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FakeDevice w;
	w.show();
	return a.exec();
}
