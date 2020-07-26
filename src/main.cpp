#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include "plugins.h"
#include "singletons/logger.h"

int main(int argc, char* argv[])
{
#if defined(Q_OS_WIN) || defined(Q_OS_ANDROID)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);

    Logger::instance().start();

    QApplication::setApplicationName("Record Rack");
    QGuiApplication::setApplicationVersion("0.0.1");
    QGuiApplication::setOrganizationName("Gecko");
    QGuiApplication::setOrganizationDomain("recordrack.io");
    app.setWindowIcon(QIcon(":/images/rr_logo.png"));

    Plugins::registerFonts();
    Plugins::registerTypes();
    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;
    engine.addImportPath(
        QDir::fromNativeSeparators(QCoreApplication::applicationDirPath()) +
        "/../3rdparty/fluid/qml");
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    engine.rootContext()->setContextProperty("MainWindow",
                                             engine.rootObjects().last());

    return app.exec();
}
