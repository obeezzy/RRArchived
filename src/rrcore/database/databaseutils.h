#ifndef DATABASEUTILS_H
#define DATABASEUTILS_H

#include <QObject>
#include <QVariant>

class QSqlQuery;

class DatabaseUtils : public QObject
{
    Q_OBJECT
public:
    static bool beginTransaction(QSqlQuery &q);
    static bool commitTransaction(QSqlQuery &q);
    static bool rollbackTransaction(QSqlQuery &q);

    static QByteArray imageToByteArray(const QString &imageSource);
    static QString byteArrayToImage(const QByteArray &imageData);

    static bool connectToDatabase(const QString &userName, const QString &password, const QString &databaseName);
private:
    explicit DatabaseUtils(QObject *parent = nullptr);
};

#endif // DATABASEUTILS_H
