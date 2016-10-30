#-------------------------------------------------
#
# Project created by QtCreator 2016-10-28T00:22:03
#
#-------------------------------------------------

QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = yeelink_demo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    yeelinkapi_v1.cpp

HEADERS  += mainwindow.h \
    yeelinkapi_v1.h

FORMS    += mainwindow.ui

RESOURCES += \
    asset.qrc
