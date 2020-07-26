QT += core qml quick quickcontrols2 widgets sql testlib
QT -= gui

TARGET = tst_qmldebtpaymentmodeltest
CONFIG += cmdline warn_on testcase
CONFIG -= app_bundle

INCLUDEPATH += $$top_srcdir/src/rrcore \
    $$top_srcdir/tests/unittests/testutils

LIBS += -L$$top_builddir/src/rrcore -lrrcore

TEMPLATE = app

SOURCES +=  tst_qmldebtpaymentmodeltest.cpp

INCLUDEPATH += $$top_srcdir/src/rrcore \
    ../utils

LIBS += -L$$top_builddir/src/rrcore -lrrcore

include($$top_srcdir/tests/unittests/testutils/testutils.pri)
