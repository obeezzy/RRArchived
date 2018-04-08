TEMPLATE = subdirs

CONFIG += ordered   # This tells Qt to compile the following SUBDIRS in order

SUBDIRS += src/rrcore \
    src \
    3rdparty/fluid \
    tests

PROJECT_NAME = RecordRack2
PROJECT_VERSION = 0.1.0

DISTNAME = $${PROJECT_NAME}-$${PROJECT_VERSION}

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#copyQml.commands = $(COPY_DIR) $$PWD/src/qml $$OUT_PWD/src
#QMAKE_EXTRA_TARGETS += copyQml

#win32 {
#    PWD_WIN = $${PWD}
#    PWD_WIN ~= s,/,\\,g

#    QMAKE_POST_LINK += $$quote(mkdir DestFolder)
#    QMAKE_POST_LINK += $$quote(xcopy $${PWD_WIN}\\TestData $${OUT_PWD_WIN}\\TestData /E)

#    QMAKE_CLEAN += /s /f /q TestData && rd /s /q TestData
#}

unix {
    QMAKE_POST_LINK += $$quote(cp -rf $${PWD}/src/qml $${OUT_PWD}/src)

    QMAKE_CLEAN += -r $${OUT_PWD}/src
}
