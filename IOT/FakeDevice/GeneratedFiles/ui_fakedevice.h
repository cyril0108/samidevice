/********************************************************************************
** Form generated from reading UI file 'fakedevice.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FAKEDEVICE_H
#define UI_FAKEDEVICE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FakeDeviceClass
{
public:
    QWidget *centralWidget;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPlainTextEdit *plainTextEdit;
    QVBoxLayout *verticalLayout;
    QPushButton *listDeviceBtn;
    QPushButton *sendCmdBtn;
    QSpacerItem *verticalSpacer;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FakeDeviceClass)
    {
        if (FakeDeviceClass->objectName().isEmpty())
            FakeDeviceClass->setObjectName(QStringLiteral("FakeDeviceClass"));
        FakeDeviceClass->resize(654, 580);
        centralWidget = new QWidget(FakeDeviceClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayoutWidget = new QWidget(centralWidget);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 10, 621, 501));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        plainTextEdit = new QPlainTextEdit(horizontalLayoutWidget);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));

        horizontalLayout->addWidget(plainTextEdit);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        listDeviceBtn = new QPushButton(horizontalLayoutWidget);
        listDeviceBtn->setObjectName(QStringLiteral("listDeviceBtn"));

        verticalLayout->addWidget(listDeviceBtn);

        sendCmdBtn = new QPushButton(horizontalLayoutWidget);
        sendCmdBtn->setObjectName(QStringLiteral("sendCmdBtn"));

        verticalLayout->addWidget(sendCmdBtn);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout);

        FakeDeviceClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(FakeDeviceClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 654, 21));
        FakeDeviceClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(FakeDeviceClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        FakeDeviceClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(FakeDeviceClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        FakeDeviceClass->setStatusBar(statusBar);

        retranslateUi(FakeDeviceClass);

        QMetaObject::connectSlotsByName(FakeDeviceClass);
    } // setupUi

    void retranslateUi(QMainWindow *FakeDeviceClass)
    {
        FakeDeviceClass->setWindowTitle(QApplication::translate("FakeDeviceClass", "FakeDevice", 0));
        listDeviceBtn->setText(QApplication::translate("FakeDeviceClass", "List Devices", 0));
        sendCmdBtn->setText(QApplication::translate("FakeDeviceClass", "Send Command", 0));
    } // retranslateUi

};

namespace Ui {
    class FakeDeviceClass: public Ui_FakeDeviceClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FAKEDEVICE_H
