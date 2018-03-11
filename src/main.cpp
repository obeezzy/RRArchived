#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "plugins.h"
#include "rr-core/database/databaseserver.h"
#include "rr-core/database/databasethread.h"
#include <QDir>
#include <QQuickStyle>
#include <QDebug>
#include <QScreen>

int main(int argc, char *argv[])
{
    //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    setenv("QT_SCALE_FACTOR","1.4",1);
    QGuiApplication app(argc, argv);

    qDebug() << "pixelRatio=" << app.screens().first()->devicePixelRatio();
    DatabaseThread::instance().start();
    Plugins::registerTypes();
    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;
    engine.addImportPath(QCoreApplication::applicationDirPath() + QDir::separator() + QLatin1String("..") +
                             QDir::separator() + QLatin1String("fluid") + QDir::separator() + QLatin1String("qml"));
    engine.addImportPath(QCoreApplication::applicationDirPath() + QDir::separator() + QLatin1String("qml"));
    engine.load(QUrl(QLatin1String("../../RecordRack2/src/qml/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
