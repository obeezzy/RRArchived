#ifndef DATABASEUTILS_H
#define DATABASEUTILS_H

#include <QObject>
#include <QVariant>

class QSqlQuery;
class QUrl;

class DatabaseUtils : public QObject
{
    Q_OBJECT
public:
    static void beginTransaction(QSqlQuery &q);
    static void commitTransaction(QSqlQuery &q);
    static void rollbackTransaction(QSqlQuery &q);

    static QByteArray imageUrlToByteArray(const QUrl &imageUrl, qint64 maxSize = 2 * 1024 * 1000 /* 2MB limit */); // Throws DatabaseException
    static QUrl byteArrayToImageUrl(const QByteArray &imageData);

    static bool connectToDatabase(const QString &userName, const QString &password, const QString &databaseName, const QString &connectionName);

    static QString createPasswordHash(const QString &password);
private:
    explicit DatabaseUtils(QObject *parent = nullptr);

    static QString generateFileName(const QByteArray &imageData);
};

#endif // DATABASEUTILS_H
