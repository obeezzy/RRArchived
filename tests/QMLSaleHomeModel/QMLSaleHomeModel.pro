#-------------------------------------------------
#
# Project created by QtCreator 2018-04-15T02:58:05
#
#-------------------------------------------------

QT       += core qml quick quickcontrols2 widgets sql testlib

QT       -= gui

TARGET = tst_qmlsalehomemodeltest
CONFIG   += console testcase
CONFIG   -= app_bundle

INCLUDEPATH += ../../src/rrcore \
    ../utils

LIBS += -L$$OUT_PWD/../../src/rrcore -lrrcore

TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        tst_qmlsalehomemodeltest.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

include(../utils/utils.pri)
