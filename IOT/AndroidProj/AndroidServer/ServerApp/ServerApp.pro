TEMPLATE = app

QT += qml quick widgets

CONFIG += c++11

SOURCES += \
    ../../../ServerApp/AppInterface.cpp \
    ../../../ServerApp/DeviceHandle.cpp \
    ../../../ServerApp/DevicesInfoList.cpp \
    ../../../ServerApp/main.cpp \
    ../../../ServerApp/QmlServerApp.cpp \
    ../../../ServerApp/serverapp.cpp \
    ../../../ServerApp/EventSender.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    ../../../ServerApp/AppInterface.h \
    ../../../ServerApp/DeviceHandle.h \
    ../../../ServerApp/DevicesInfoList.h \
    ../../../ServerApp/QmlServerApp.h \
    ../../../ServerApp/serverapp.h \
    ../../../ServerApp/EventSender.h

DISTFILES += \
    ../../../ServerApp/main.qml \
    ../../../ServerApp/ServerAppForm.ui.qml


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ServerCore/release/ -lServerCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ServerCore/debug/ -lServerCore
else:unix:!macx: LIBS += -L$$OUT_PWD/../ServerCore/ -lServerCore

INCLUDEPATH += $$PWD/../ServerCore \
                $$PWD/../../..
DEPENDPATH += $$PWD/../ServerCore  \
                $$PWD/../../..

FORMS += \
    ../../../ServerApp/serverapp.ui

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS = \
        $$PWD/../../build-AndroidServer-Android_for_armeabi_v7a_GCC_4_9_Qt_5_5_1-Release/ServerCore/libServerCore.so
}
