#include "databaseutils.h"
#include <QSqlQuery>
#include <QBuffer>
#include <QImage>
#include <QByteArray>
#include <QUrl>
#include <QFile>
#include <QUuid>
#include <QCryptographicHash>
#include <QStandardPaths>

#include "database/databaseexception.h"

#include <QDebug>

const int HASH_INPUT_LENGTH = 64;

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

QString DatabaseUtils::createPasswordHash(const QString &password)
{
    if (password.trimmed().isEmpty())
        return QString();

    return QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
}

QByteArray DatabaseUtils::imageToByteArray(const QString &imageSource, qint64 maxSize)
{
    if (imageSource.trimmed().isEmpty())
        return QByteArray();

    QImage image(QUrl{imageSource}.toLocalFile());
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");

    if (ba.size() > maxSize)
        throw DatabaseException(DatabaseException::RRErrorCode::ImageTooLarge,
                                QStringLiteral("Image too large (%1 bytes). Expected size should be less than %2").arg(ba.size()).arg(maxSize),
                                QStringLiteral("Image too large (%1 bytes). Expected size should be less than %2").arg(ba.size()).arg(maxSize));
    return ba;
}

QString DatabaseUtils::byteArrayToImage(const QByteArray &imageData)
{
    if (imageData.isNull())
        return QString();

    const QString &imageSource = generateFileName(imageData);
    QFile file(imageSource);
    file.open(QIODevice::WriteOnly);
    file.write(QByteArray::fromHex(imageData));

    return QUrl::fromLocalFile(imageSource).toString();
}

QString DatabaseUtils::generateFileName(const QByteArray &imageData)
{
    QByteArray ba;
    if (imageData.size() < HASH_INPUT_LENGTH) {
        for (int i = 0; i < imageData.size(); ++i)
            ba.append(imageData.at(i));
    } else {
        for (int i = 0; i < HASH_INPUT_LENGTH; ++i)
            ba.append(imageData.at(i));
    }

    return QString("%1/%2.png").arg(QStandardPaths::writableLocation(QStandardPaths::TempLocation),
                                    QString(QCryptographicHash::hash(ba, QCryptographicHash::Md5).toHex()));
}
