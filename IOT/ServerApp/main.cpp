#include "serverapp.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ServerApp w;
	w.show();
	return a.exec();
}
