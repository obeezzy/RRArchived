TEMPLATE = app
TARGET = rr

QT += qml quick quickcontrols2 sql charts

CONFIG += c++14

INCLUDEPATH += rrcore
LIBS += -L$$OUT_PWD/rrcore -lrrcore

SOURCES += main.cpp \
    plugins.cpp

HEADERS += plugins.h

RESOURCES += qml/qml.qrc \
    icons/icons.qrc \
    images/images.qrc \
    fonts/fonts.qrc
