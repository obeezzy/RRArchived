TEMPLATE = subdirs

CONFIG += ordered   # This tells Qt to compile the following SUBDIRS in order

SUBDIRS += 3rdparty/fluid \
    src/rrcore \
    src \
    tests

DEFINES += QT_DEPRECATED_WARNINGS

PROJECT_NAME = RecordRackDesktop
PROJECT_VERSION = 0.1.0
DISTNAME = $${PROJECT_NAME}-$${PROJECT_VERSION}

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
