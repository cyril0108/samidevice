#include <QQuickView>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include "ClientApp.h"

#include "fakedevice.h"
#include <QtWidgets/QApplication>

#define QML_UI 

QQuickItem* m_pQmlManager;
CClientApp* m_pAppInterface;

int main(int argc, char *argv[])
{
#ifdef QML_UI 
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine* pQmlEngine = new QQmlApplicationEngine;
    m_pAppInterface = new CClientApp;
    m_pAppInterface->Initialize(pQmlEngine);
    pQmlEngine->load(QUrl(QStringLiteral("qrc:/FakeDevice/main.qml")));
#else
    QApplication app(argc, argv);
    FakeDevice w;
    w.show();
#endif
    return app.exec();
}