#-------------------------------------------------
#
# Project created by QtCreator 2018-03-11T03:45:40
#
#-------------------------------------------------

QT       += core qml quick quickcontrols2 widgets sql testlib
QT       -= gui

TARGET = tst_qmlstockproductpusher
CONFIG   += console testcase
CONFIG   -= app_bundle

INCLUDEPATH += $$top_srcdir/src/rrcore \
    ../utils

LIBS += -L$$top_builddir/src/rrcore -lrrcore

TEMPLATE = app

SOURCES += \
    tst_qmlstockproductpusher.cpp

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += SRCDIR=\\\"$$PWD/\\\"

include(../utils/utils.pri)
