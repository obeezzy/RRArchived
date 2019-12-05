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
#include <QSqlError>

#include "database/databaseexception.h"

#include <QDebug>

const int HASH_INPUT_LENGTH = 64;

DatabaseUtils::DatabaseUtils(QObject *parent)
    : QObject(parent)
{

}

void DatabaseUtils::beginTransaction(QSqlQuery &q)
{
    if (!q.exec("SET AUTOCOMMIT = 0") || !q.exec("START TRANSACTION"))
        throw DatabaseException(DatabaseError::QueryErrorCode::BeginTransactionFailed, q.lastError().text(),
                                "Failed to start transation.");
}

void DatabaseUtils::commitTransaction(QSqlQuery &q)
{
    if (!q.exec("COMMIT"))
        throw DatabaseException(DatabaseError::QueryErrorCode::CommitTransationFailed, q.lastError().text(),
                                "Failed to commit.");
}

void DatabaseUtils::rollbackTransaction(QSqlQuery &q)
{
    if (!q.exec("ROLLBACK"))
        qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());
}

bool DatabaseUtils::connectToDatabase(const QString &userName, const QString &password,
                                      const QString &databaseName, const QString &connectionName)
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

QByteArray DatabaseUtils::imageUrlToByteArray(const QUrl &imageUrl, qint64 maxSize)
{
    if (imageUrl.isEmpty())
        return QByteArray();

    QImage image(imageUrl.toLocalFile());
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");

    if (ba.size() > maxSize)
        throw DatabaseException(DatabaseError::QueryErrorCode::ImageTooLarge,
                                QStringLiteral("Image too large (%1 bytes). Expected size should be less than %2").arg(ba.size()).arg(maxSize),
                                QStringLiteral("Image too large (%1 bytes). Expected size should be less than %2").arg(ba.size()).arg(maxSize));
    return ba;
}

QUrl DatabaseUtils::byteArrayToImageUrl(const QByteArray &imageData)
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
