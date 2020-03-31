QT += core qml quick quickcontrols2 widgets sql testlib
QT -= gui

CONFIG += cmdline warn_on testcase
CONFIG -= app_bundle

INCLUDEPATH += $$top_srcdir/src/rrcore \
    ../utils

LIBS += -L$$top_builddir/src/rrcore -lrrcore

TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += SRCDIR=\\\"$$PWD/\\\"

SOURCES +=  tst_qmlexpensetransactionmodeltest.cpp

include(../utils/utils.pri)
