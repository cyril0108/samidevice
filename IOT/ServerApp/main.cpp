#include <QQuickView>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include "AppInterface.h"
#include <QtWidgets/QApplication>
#include "serverapp.h"
#define QML_UI 

QQuickItem* m_pQmlManager;
CAppInterface* m_pAppInterface;

int main(int argc, char *argv[])
{
#ifdef QML_UI 
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine* pQmlEngine = new QQmlApplicationEngine;
    m_pAppInterface = new CAppInterface;
    m_pAppInterface->Initialize(pQmlEngine);
    pQmlEngine->load(QUrl(QStringLiteral("main.qml")));
#else
    QApplication app(argc, argv);
    ServerApp w;
    w.show();
#endif
    return app.exec();
}

