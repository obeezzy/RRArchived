#include "databaseserver.h"
#include <QProcess>
#include "databasethread.h"
#include <QDebug>

DatabaseServer::DatabaseServer()
{

}

void DatabaseServer::start()
{
    QProcess process;
    //process.start("/etc/init.d/mysqld start");

    //process.waitForStarted();
}
