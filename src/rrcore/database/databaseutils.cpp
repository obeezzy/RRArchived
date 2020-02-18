#include "databaseutils.h"
#include "database/databaseexception.h"
#include "database/exceptions/exceptions.h"
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
#include <QDebug>

const int HASH_INPUT_LENGTH = 64;

void DatabaseUtils::beginTransaction(QSqlQuery &q)
{
    if (!q.exec("SET AUTOCOMMIT = 0") || !q.exec("START TRANSACTION"))
        throw DatabaseException(DatabaseError::QueryErrorCode::BeginTransactionFailed,
                                q.lastError().text(),
                                "Failed to start transation.");
}

void DatabaseUtils::commitTransaction(QSqlQuery &q)
{
    if (!q.exec("COMMIT"))
        throw DatabaseException(DatabaseError::QueryErrorCode::CommitTransationFailed,
                                q.lastError().text(),
                                "Failed to commit.");
}

void DatabaseUtils::rollbackTransaction(QSqlQuery &q)
{
    if (!q.exec("ROLLBACK"))
        qCritical("Failed to rollback failed transaction! %s",
                  q.lastError().text().toStdString().c_str());
}

QByteArray DatabaseUtils::imageUrlToByteArray(const QUrl &imageUrl,
                                              qint64 maxSize)
{
    if (imageUrl.isEmpty())
        return QByteArray();

    QImage image(imageUrl.toLocalFile());
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");

    if (ba.size() > maxSize)
        throw ImageTooLargeException(ba.size(), maxSize);
    return ba;
}

QUrl DatabaseUtils::byteArrayToImageUrl(const QByteArray &imageData) noexcept
{
    if (imageData.isNull())
        return QString();

    const QString &imageSource = generateFileName(imageData);
    QFile file(imageSource);
    file.open(QIODevice::WriteOnly);
    file.write(QByteArray::fromHex(imageData));

    return QUrl::fromLocalFile(imageSource).toString();
}

QString DatabaseUtils::generateFileName(const QByteArray &imageData) noexcept
{
    QByteArray ba;
    if (imageData.size() < HASH_INPUT_LENGTH) {
        for (int i = 0; i < imageData.size(); ++i)
            ba.append(imageData.at(i));
    } else {
        for (int i = 0; i < HASH_INPUT_LENGTH; ++i)
            ba.append(imageData.at(i));
    }

    return QStringLiteral("%1/%2.png").arg(QStandardPaths::writableLocation(QStandardPaths::TempLocation),
                                           QString(QCryptographicHash::hash(ba, QCryptographicHash::Md5).toHex()));
}
