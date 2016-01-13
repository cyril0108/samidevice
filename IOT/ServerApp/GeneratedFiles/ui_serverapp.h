/********************************************************************************
** Form generated from reading UI file 'serverapp.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVERAPP_H
#define UI_SERVERAPP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ServerAppClass
{
public:
    QWidget *centralWidget;
    QPlainTextEdit *plainTextEdit;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *LoginButton;
    QPushButton *GetStatusButton;
    QPushButton *ResetDefaultButton;
    QPushButton *ResetWIFIButton;
    QPushButton *RebootButton;
    QLineEdit *DeviceNameLineEdit;
    QPushButton *ChangeDeviceNameButton;
    QPushButton *DeleteDeviceName;
    QLineEdit *PWLineEdit;
    QPushButton *ChangeAdminPWButton;
    QPushButton *ChangeUserPWButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ServerAppClass)
    {
        if (ServerAppClass->objectName().isEmpty())
            ServerAppClass->setObjectName(QStringLiteral("ServerAppClass"));
        ServerAppClass->resize(600, 400);
        centralWidget = new QWidget(ServerAppClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        plainTextEdit = new QPlainTextEdit(centralWidget);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(10, 20, 291, 321));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(350, 10, 160, 331));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        LoginButton = new QPushButton(verticalLayoutWidget);
        LoginButton->setObjectName(QStringLiteral("LoginButton"));

        verticalLayout->addWidget(LoginButton);

        GetStatusButton = new QPushButton(verticalLayoutWidget);
        GetStatusButton->setObjectName(QStringLiteral("GetStatusButton"));

        verticalLayout->addWidget(GetStatusButton);

        ResetDefaultButton = new QPushButton(verticalLayoutWidget);
        ResetDefaultButton->setObjectName(QStringLiteral("ResetDefaultButton"));

        verticalLayout->addWidget(ResetDefaultButton);

        ResetWIFIButton = new QPushButton(verticalLayoutWidget);
        ResetWIFIButton->setObjectName(QStringLiteral("ResetWIFIButton"));

        verticalLayout->addWidget(ResetWIFIButton);

        RebootButton = new QPushButton(verticalLayoutWidget);
        RebootButton->setObjectName(QStringLiteral("RebootButton"));

        verticalLayout->addWidget(RebootButton);

        DeviceNameLineEdit = new QLineEdit(verticalLayoutWidget);
        DeviceNameLineEdit->setObjectName(QStringLiteral("DeviceNameLineEdit"));

        verticalLayout->addWidget(DeviceNameLineEdit);

        ChangeDeviceNameButton = new QPushButton(verticalLayoutWidget);
        ChangeDeviceNameButton->setObjectName(QStringLiteral("ChangeDeviceNameButton"));

        verticalLayout->addWidget(ChangeDeviceNameButton);

        DeleteDeviceName = new QPushButton(verticalLayoutWidget);
        DeleteDeviceName->setObjectName(QStringLiteral("DeleteDeviceName"));

        verticalLayout->addWidget(DeleteDeviceName);

        PWLineEdit = new QLineEdit(verticalLayoutWidget);
        PWLineEdit->setObjectName(QStringLiteral("PWLineEdit"));

        verticalLayout->addWidget(PWLineEdit);

        ChangeAdminPWButton = new QPushButton(verticalLayoutWidget);
        ChangeAdminPWButton->setObjectName(QStringLiteral("ChangeAdminPWButton"));

        verticalLayout->addWidget(ChangeAdminPWButton);

        ChangeUserPWButton = new QPushButton(verticalLayoutWidget);
        ChangeUserPWButton->setObjectName(QStringLiteral("ChangeUserPWButton"));

        verticalLayout->addWidget(ChangeUserPWButton);

        ServerAppClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ServerAppClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        ServerAppClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ServerAppClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ServerAppClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ServerAppClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ServerAppClass->setStatusBar(statusBar);

        retranslateUi(ServerAppClass);

        QMetaObject::connectSlotsByName(ServerAppClass);
    } // setupUi

    void retranslateUi(QMainWindow *ServerAppClass)
    {
        ServerAppClass->setWindowTitle(QApplication::translate("ServerAppClass", "ServerApp", 0));
        LoginButton->setText(QApplication::translate("ServerAppClass", "Login", 0));
        GetStatusButton->setText(QApplication::translate("ServerAppClass", "Get Status", 0));
        ResetDefaultButton->setText(QApplication::translate("ServerAppClass", "Reset Default", 0));
        ResetWIFIButton->setText(QApplication::translate("ServerAppClass", "ResetWifi", 0));
        RebootButton->setText(QApplication::translate("ServerAppClass", "RebootButton", 0));
        ChangeDeviceNameButton->setText(QApplication::translate("ServerAppClass", "Change Device Name", 0));
        DeleteDeviceName->setText(QApplication::translate("ServerAppClass", "Delete Device Name", 0));
        ChangeAdminPWButton->setText(QApplication::translate("ServerAppClass", "Change Admin Password", 0));
        ChangeUserPWButton->setText(QApplication::translate("ServerAppClass", "Change User Password", 0));
    } // retranslateUi

};

namespace Ui {
    class ServerAppClass: public Ui_ServerAppClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVERAPP_H
