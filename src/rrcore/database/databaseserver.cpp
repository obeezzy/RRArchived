#include "databaseserver.h"
#include <QDebug>
#include <QProcess>
#include "databasethread.h"

DatabaseServer::DatabaseServer() {}

void DatabaseServer::start()
{
    QProcess process;
    // process.start("/etc/init.d/mysqld start");

    // process.waitForStarted();
}
