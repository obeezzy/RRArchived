#include <QApplication>
#include <QQmlApplicationEngine>
#include <QDir>
#include <QQuickStyle>
#include <QDebug>
#include <QScreen>
#include "plugins.h"
#include "rrcore/database/databaseserver.h"
#include "rrcore/database/databasethread.h"

int main(int argc, char *argv[])
{
    //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    qputenv("QT_SCALE_FACTOR", "1.4");
    QApplication app(argc, argv);

    QApplication::setApplicationName("Record Rack");
    QGuiApplication::setApplicationVersion("0.0.1");
    QGuiApplication::setOrganizationName("Gecko");
    QGuiApplication::setOrganizationDomain("www.gecko.rr.com");

    //qDebug() << "pixelRatio=" << app.screens().first()->devicePixelRatio();
    DatabaseThread::instance().start();
    Plugins::registerTypes();
    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("../../RecordRackDesktop/src/qml/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
