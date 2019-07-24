QT       += core qml quick quickcontrols2 widgets sql testlib

QT       -= gui

TARGET = tst_qmlusermodeltest
CONFIG   += console testcase
CONFIG   -= app_bundle

INCLUDEPATH += ../../src/rrcore \
    ../utils

LIBS += -L$$OUT_PWD/../../src/rrcore -lrrcore

TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += tst_qmlusermodeltest.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

include(../utils/utils.pri)
