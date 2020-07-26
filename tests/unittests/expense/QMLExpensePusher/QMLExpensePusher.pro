#-------------------------------------------------
#
# Project created by QtCreator 2018-07-24T08:24:33
#
#-------------------------------------------------

QT       += core qml quick quickcontrols2 widgets sql testlib
QT       -= gui

TARGET = tst_qmlexpensepushertest
CONFIG   += console testcase
CONFIG   -= app_bundle

INCLUDEPATH += $$top_srcdir/src/rrcore \
    $$top_srcdir/tests/unittests/testutils

LIBS += -L$$top_builddir/src/rrcore -lrrcore

TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += SRCDIR=\\\"$$PWD/\\\"

SOURCES += tst_qmlexpensepushertest.cpp

include($$top_srcdir/tests/unittests/testutils/testutils.pri)
