TEMPLATE = subdirs

CONFIG += ordered   # This tells Qt to compile the following SUBDIRS in order

SUBDIRS += 3rdparty/fluid \
    src/rrcore \
    src \
    tests

PROJECT_NAME = RecordRackDesktop
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

unix {
    QMAKE_POST_LINK += $$quote(cp -rf $${PWD}/src/qml $${OUT_PWD}/src)

    QMAKE_CLEAN += -r $${OUT_PWD}/src

    # Build AppImage
    build_package.target = package
    build_package.commands = cd $${OUT_PWD} $$escape_expand(\\n\\t)
    build_package.commands += rm -rf $${OUT_PWD}/deployment $$escape_expand(\\n\\t)
    build_package.commands += cp -r $${PWD}/deployment $${OUT_PWD} $$escape_expand(\\n\\t)
    build_package.commands += cd $${OUT_PWD}/deployment $$escape_expand(\\n\\t)
    build_package.commands += wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage $$escape_expand(\\n\\t)
    build_package.commands += chmod +x linuxdeploy-x86_64.AppImage $$escape_expand(\\n\\t)
    build_package.commands += ./linuxdeploy-x86_64.AppImage --appdir $${OUT_PWD}/deployment/AppDir -e $${OUT_PWD}/src/rr --output appimage $$escape_expand(\\n\\t)
    build_package.commands += mv rr.AppImage rr-linux-x86_64.AppImage $$escape_expand(\\n\\t)
    build_package.commands += export DEPLOYFILE=rr-linux-x86_64.AppImage $$escape_expand(\\n\\t)

    QMAKE_EXTRA_TARGETS += build_package
}
