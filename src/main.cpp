#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include "plugins.h"
#include "rrcore/database/databaseserver.h"
#include "rrcore/singletons/logger.h"

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN) || defined(Q_OS_ANDROID)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    //qputenv("QT_SCALE_FACTOR", "1.4");
    QApplication app(argc, argv);

    Logger::instance().start();

    QApplication::setApplicationName("Record Rack");
    QGuiApplication::setApplicationVersion("0.1.0");
    QGuiApplication::setOrganizationName("Gecko");
    QGuiApplication::setOrganizationDomain("recordrack.io");

    Plugins::registerFonts();
    Plugins::registerTypes();
    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
