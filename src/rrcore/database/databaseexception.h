#ifndef DATABASEEXCEPTION_H
#define DATABASEEXCEPTION_H

#include <QDebug>
#include <QSqlError>
#include <QString>
#include <exception>
#include "databaseerror.h"

class DatabaseException : public std::exception
{
public:
    explicit DatabaseException(int errorCode, const QString& message,
                               const QSqlError& error = QSqlError());
    explicit DatabaseException(DatabaseError::QueryErrorCode errorCode,
                               const QString& message,
                               const QSqlError& error = QSqlError());
    explicit DatabaseException(DatabaseError::MySqlErrorCode errorCode,
                               const QString& message,
                               const QSqlError& error = QSqlError());

    int code() const;
    QString message() const;
    QSqlError sqlError() const;
    const char* what() const noexcept override final;

    virtual QString toString() const = 0;

    friend QDebug operator<<(QDebug debug,
                             const DatabaseException& databaseException)
    {
        QString debugMessage = databaseException.toString();

        // Surround message in quotes
        if (!databaseException.message().trimmed().isEmpty())
            debugMessage.replace(
                databaseException.message(),
                QStringLiteral("\"%1\"").arg(databaseException.message()));
        // Surround user message in quotes
        if (!databaseException.sqlError().databaseText().trimmed().isEmpty())
            debugMessage.replace(
                databaseException.sqlError().databaseText(),
                QStringLiteral("\"%1\"").arg(
                    databaseException.sqlError().databaseText()));

        debug << debugMessage.toStdString().c_str();
        return debug;
    }

private:
    int m_code;
    QString m_message;
    QSqlError m_sqlError;
};

#endif  // DATABASEEXCEPTION_H
