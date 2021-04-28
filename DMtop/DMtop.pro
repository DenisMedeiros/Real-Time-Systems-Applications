#-------------------------------------------------
#
# Project created by QtCreator 2017-08-30T19:09:30
#
#-------------------------------------------------

QT += core gui charts

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets core gui charts

TARGET = DMtop
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    util.cpp

HEADERS  += mainwindow.h \
    util.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
