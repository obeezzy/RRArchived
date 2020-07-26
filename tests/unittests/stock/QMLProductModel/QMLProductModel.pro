#-------------------------------------------------
#
# Project created by QtCreator 2018-07-06T04:46:31
#
#-------------------------------------------------

QT       += core qml quick quickcontrols2 widgets sql testlib
QT       -= gui

TARGET = tst_qmlproductmodeltest
CONFIG   += console testcase
CONFIG   -= app_bundle

INCLUDEPATH += $$top_srcdir/src/rrcore \
    $$top_srcdir/tests/unittests/testutils

LIBS += -L$$top_builddir/src/rrcore -lrrcore

TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += SRCDIR=\\\"$$PWD/\\\"

SOURCES += \
        tst_qmlproductmodeltest.cpp

include($$top_srcdir/tests/unittests/testutils/testutils.pri)
