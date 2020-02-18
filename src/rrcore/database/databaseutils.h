#ifndef DATABASEUTILS_H
#define DATABASEUTILS_H

#include <QObject>
#include <QVariant>

class QSqlQuery;
class QUrl;

class DatabaseUtils
{
public:
    static void beginTransaction(QSqlQuery &q);
    static void commitTransaction(QSqlQuery &q);
    static void rollbackTransaction(QSqlQuery &q);

    static QByteArray imageUrlToByteArray(const QUrl &imageUrl,
                                          qint64 maxSize = 2 * 1024 * 1000 /* 2MB limit */);
    static QUrl byteArrayToImageUrl(const QByteArray &imageData) noexcept;
private:
    DatabaseUtils() = delete;
    DatabaseUtils(const DatabaseUtils &) = delete;
    DatabaseUtils &operator=(const DatabaseUtils &) = delete;

    static QString generateFileName(const QByteArray &imageData) noexcept;
};

#endif // DATABASEUTILS_H
