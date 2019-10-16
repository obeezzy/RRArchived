TEMPLATE = subdirs # Empty project

win32:QTIFWDIR = C:\Qt\QtIFW2.0.1
unix:QTIFWDIR =

CONFIG(release, debug|release) {
    unix: INSTALLER_OFFLINE = $$OUT_PWD/rr-linux-x64.run
    win32 {
        INSTALLER_OFFLINE = $$OUT_PWD/rr-win32-x64.exe
        message(We go high)
        offlineInstaller.target = $$INSTALLER_OFFLINE
        offlineInstaller.commands += $$QTIFWDIR/bin/archivegen.exe rr.7z ../install-root $$escape_expand(\\n\\t)
        offlineInstaller.commands += $$QMAKE_MKDIR_CMD $${OUT_PWD}/packages/com.gecko.rr/data $$escape_expand(\\n\\t)
        offlineInstaller.commands += $$QMAKE_COPY rr.7z $$shell_path($$shell_quote($${OUT_PWD}/packages/com.gecko.rr/data)) $$escape_expand(\\n\\t)
        offlineInstaller.commands += $$QTIFWDIR/bin/binarycreator.exe --offline-only \
            -c $$PWD/config/config.xml -p $$PWD/packages $$offlineInstaller.target $$escape_expand(\\n\\t)
        offlineInstaller.depends = install

        offlineInstallerTarget.target = package # This is the real name of the target
        offlineInstallerTarget.depends = offlineInstaller
        QMAKE_EXTRA_TARGETS += offlineInstaller offlineInstallerTarget
        message("PACKAGE_COMMANDS" $$offlineInstaller.commands)
    }
}

OTHER_FILES += $$PWD/config/config.xml \
    $$PWD/packages/com.gecko.rr/meta/package.xml

#    # set variables that will contain the path name to the installers
#    INSTALLER_OFFLINE = $$OUT_PWD/../../InstallerRelease/Hello.offline
#    INSTALLER_ONLINE = $$OUT_PWD/../../InstallerRelease/Hello.online

#    # set a variable that should contain the path to the data folder
#    DESTDIR_WIN = $$PWD/packages/ru.evileg.helloinstaller/data
#    DESTDIR_WIN ~= s,/,\\,g
#    # set all the path from the application to the DLL need to copy
#    PWD_WIN = $$OUT_PWD/../../HelloRelease
#    PWD_WIN ~= s,/,\\,g

#    # First, you want to compile installers, you must copy the files from
#    # the output folder of the project together with all DLL
#    # in the data folder, which refers to build packages
#    copydata.commands = $(COPY_DIR) $$PWD_WIN $$DESTDIR_WIN
#    first.depends = $(first) copydata
#    export(first.depends)
#    export(copydata.commands)
#    # Asking a custom build target, at which the first fulfill kompirovanie files,
#    # and then, and the rest that follows the script QMake
#    QMAKE_EXTRA_TARGETS += first copydata

# Create the target assembly Offline Installer
