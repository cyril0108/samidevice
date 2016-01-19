#-------------------------------------------------
#
# Project created by QtCreator 2016-01-19T11:39:10
#
#-------------------------------------------------

QT       += widgets network

QT       -= gui

CONFIG += c++11

TARGET = ServerCore
TEMPLATE = lib

DEFINES += SERVERCORE_LIB

SOURCES += \
    ../../../ServerCore/DeviceCommand.cpp \
    ../../../ServerCore/DeviceInfo.cpp \
    ../../../ServerCore/DeviceManager.cpp \
    ../../../ServerCore/JsonGenerator.cpp \
    ../../../ServerCore/ServerBroadCast.cpp \
    ../../../ServerCore/servercore.cpp \
    ../../../ServerCore/ServerUtil.cpp

HEADERS += \
    ../../../ServerCore/common.h \
    ../../../ServerCore/Constant.h \
    ../../../ServerCore/DeviceCommand.h \
    ../../../ServerCore/DeviceInfo.h \
    ../../../ServerCore/DeviceManager.h \
    ../../../ServerCore/JsonGenerator.h \
    ../../../ServerCore/ServerBroadCast.h \
    ../../../ServerCore/servercore.h \
    ../../../ServerCore/servercore_global.h \
    ../../../ServerCore/ServerUtil.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
