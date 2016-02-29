#-------------------------------------------------
#
# Project created by QtCreator 2016-02-22T14:46:03
#
#-------------------------------------------------
include(QtAwesome/QtAwesome/QtAwesome.pri)

QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CottBooth
TEMPLATE = app

CONFIG-=app_bundle
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    startscreen.cpp \
    settingswindow.cpp \
    settings.cpp \
    cameracontroller.cpp \
    session.cpp \
    sessionwindow.cpp

HEADERS  += mainwindow.h \
    startscreen.h \
    settingswindow.h \
    settings.h \
    cameracontroller.h \
    session.h \
    sessionwindow.h \
    globals.h

FORMS    += mainwindow.ui \
    startscreen.ui \
    settingswindow.ui \
    sessionwindow.ui

RESOURCES += \
    resources.qrc

copydata.commands = $(COPY_DIR) $$PWD/themes $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata




unix: LIBS += -L$$PWD/../../../../../../usr/local/lib/ -lgphoto2

INCLUDEPATH += $$PWD/../../../../../../usr/local/include/gphoto2
DEPENDPATH += $$PWD/../../../../../../usr/local/include/gphoto2
