#-------------------------------------------------
#
# Project created by QtCreator 2019-05-14T14:12:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = snake
TEMPLATE = app


SOURCES += \
        gamewidget.cpp \
        main.cpp \
        widget.cpp

HEADERS += \
        gamewidget.h \
        widget.h


CONFIG += c++11

RESOURCES += \
    image.qrc
