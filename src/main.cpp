#include <QApplication>
#include <QQmlApplicationEngine>
#include <QDir>
#include <QQuickStyle>
#include <QDebug>
#include <QScreen>
#include <QLoggingCategory>
#include "plugins.h"
#include "rrcore/database/databaseserver.h"
#include "rrcore/database/databasethread.h"

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN) || defined(Q_OS_ANDROID)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    //qputenv("QT_SCALE_FACTOR", "1.4");
    QApplication app(argc, argv);

    //QLoggingCategory::setFilterRules(QStringLiteral("*.info=false"));

    QApplication::setApplicationName("Record Rack");
    QGuiApplication::setApplicationVersion("0.0.1");
    QGuiApplication::setOrganizationName("Gecko");
    QGuiApplication::setOrganizationDomain("www.gecko.rr.com");

    Plugins::registerTypes();
    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
