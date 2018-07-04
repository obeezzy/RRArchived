#include "databaseutils.h"
#include <QSqlQuery>
#include <QBuffer>
#include <QImage>
#include <QByteArray>

DatabaseUtils::DatabaseUtils(QObject *parent)
    : QObject(parent)
{

}

bool DatabaseUtils::beginTransaction(QSqlQuery &q)
{
    if (!q.exec("SET AUTOCOMMIT = 0"))
        return false;

    return q.exec("START TRANSACTION");
}

bool DatabaseUtils::commitTransaction(QSqlQuery &q)
{
    return q.exec("COMMIT");
}

bool DatabaseUtils::rollbackTransaction(QSqlQuery &q)
{
    return q.exec("ROLLBACK");
}

bool DatabaseUtils::connectToDatabase(const QString &userName, const QString &password, const QString &databaseName, const QString &connectionName)
{
    if (connectionName.isEmpty())
        return false;

    QSqlDatabase connection;

    if (!QSqlDatabase::contains(connectionName))
        connection = QSqlDatabase::addDatabase("QMYSQL", connectionName);
    else
        connection = QSqlDatabase::database(connectionName);

    connection.setDatabaseName(databaseName);
    connection.setHostName("localhost");
    connection.setPort(3306);
    connection.setUserName(userName);
    connection.setPassword(password);
    connection.setConnectOptions("MYSQL_OPT_RECONNECT = 1");

    if (!connection.open())
        return false;

    return true;
}

QByteArray DatabaseUtils::imageToByteArray(const QString &imageSource)
{
    if (imageSource.trimmed().isEmpty())
        return QByteArray();

    QImage image(imageSource);
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer);

    return ba;
}

QString DatabaseUtils::byteArrayToImage(const QByteArray &imageData)
{
    if (imageData.isNull())
        return QString();

    QString imageSource;

    return imageSource;
}
