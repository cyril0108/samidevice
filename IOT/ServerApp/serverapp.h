#ifndef SERVERAPP_H
#define SERVERAPP_H

#include <QtWidgets/QMainWindow>
#include "ui_serverapp.h"

class ServerApp : public QMainWindow
{
	Q_OBJECT

public:
	ServerApp(QWidget *parent = 0);
	~ServerApp();

private:
	Ui::ServerAppClass ui;
};

#endif // SERVERAPP_H
