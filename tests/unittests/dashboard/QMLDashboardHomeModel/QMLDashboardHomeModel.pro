#-------------------------------------------------
#
# Project created by QtCreator 2018-04-15T02:41:59
#
#-------------------------------------------------

QT       += core qml quick quickcontrols2 widgets sql testlib

QT       -= gui

TARGET = tst_qmldashboardhomemodeltest
CONFIG   += console testcase
CONFIG   -= app_bundle

INCLUDEPATH += $$top_srcdir/src/rrcore \
    $$top_srcdir/tests/unittests/testutils

LIBS += -L$$top_builddir/src/rrcore -lrrcore

TEMPLATE = app

SOURCES += \
        tst_qmldashboardhomemodeltest.cpp

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += SRCDIR=\\\"$$PWD/\\\"

include($$top_srcdir/tests/unittests/testutils/testutils.pri)
