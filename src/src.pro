TEMPLATE = app
TARGET = rr

QT += qml quick quickcontrols2 sql charts
QT += core svg printsupport

CONFIG += c++14

INCLUDEPATH += rrcore
LIBS += -L$$OUT_PWD/rrcore -lrrcore

SOURCES += main.cpp \
    plugins.cpp

HEADERS += plugins.h

win32:DESTDIR = $$OUT_PWD

RESOURCES += qml/qml.qrc \
    icons/icons.qrc \
    images.qrc \
    fonts/fonts.qrc \
    lottie/lottie.qrc

win32:Release {
    RR_INSTALL_ROOT = $$OUT_PWD/../install-root

    target.path = $$RR_INSTALL_ROOT
    target.files = $$OUT_PWD/rr.exe
    target.commands += $$QMAKE_COPY $$shell_path($$shell_quote($$OUT_PWD/rr.exe)) $$shell_path($$shell_quote($${RR_INSTALL_ROOT})) $$escape_expand(\\n\\t)
    target.commands += $$QMAKE_COPY $$shell_path($$shell_quote($$OUT_PWD/rrcore/rrcore.dll)) $$shell_path($$shell_quote($${RR_INSTALL_ROOT})) $$escape_expand(\\n\\t)
    target.commands += setx PATH "C:\\Qt\\Tools\\mingw730_64\\bin;%PATH%" $$escape_expand(\\n\\t)
    target.commands += $$[QT_INSTALL_BINS]/windeployqt --qmldir $$PWD/qml $${RR_INSTALL_ROOT}/rr.exe $$escape_expand(\\n\\t)
    target.commands += $$[QT_INSTALL_BINS]/windeployqt --qmldir $$PWD/qml $${RR_INSTALL_ROOT}/rrcore.dll $$escape_expand(\\n\\t)
    #documentation.files += readme.txt
    #documentation.path += $$PWD/docs
    #INSTALLS += documentation
    INSTALLS += target
}
message("COMMANDS" $$target.commands)
