#-------------------------------------------------
#
# Project created by QtCreator 2018-07-24T08:24:33
#
#-------------------------------------------------

QT       += core qml quick quickcontrols2 widgets sql testlib

QT       -= gui

TARGET = tst_qmlincomepushertest
CONFIG   += console testcase
CONFIG   -= app_bundle

INCLUDEPATH += $$top_srcdir/src/rrcore \
    ../utils

LIBS += -L$$top_builddir/src/rrcore -lrrcore

TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += SRCDIR=\\\"$$PWD/\\\"

SOURCES += tst_qmlincomepushertest.cpp

include(../utils/utils.pri)
