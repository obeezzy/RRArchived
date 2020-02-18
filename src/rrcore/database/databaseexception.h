#ifndef DATABASEEXCEPTION_H
#define DATABASEEXCEPTION_H

#include "databaseerror.h"
#include <exception>
#include <QString>
#include <QDebug>

class DatabaseException : public std::exception
{
public:
    explicit DatabaseException(int errorCode);
    explicit DatabaseException(int errorCode,
                               const QString &message,
                               const QString &userMessage = "");

    explicit DatabaseException(DatabaseError::QueryErrorCode errorCode);
    explicit DatabaseException(DatabaseError::QueryErrorCode errorCode,
                               const QString &message,
                               const QString &userMessage = "");

    explicit DatabaseException(DatabaseError::MySqlErrorCode errorCode);
    explicit DatabaseException(DatabaseError::MySqlErrorCode errorCode,
                               const QString &message,
                               const QString &userMessage = "");

    int code() const;
    QString message() const;
    QString userMessage() const;
    const char *what() const noexcept override final;

    virtual QString toString() const;

    friend QDebug operator<<(QDebug debug,
                             const DatabaseException &databaseException)
    {
        QString debugMessage = databaseException.toString();

        // Surround message in quotes
        if (!databaseException.message().trimmed().isEmpty())
            debugMessage.replace(databaseException.message(),
                                 QStringLiteral("\"%1\"")
                                 .arg(databaseException.message()));
        // Surround user message in quotes
        if (!databaseException.userMessage().trimmed().isEmpty())
            debugMessage.replace(databaseException.userMessage(),
                     QStringLiteral("\"%1\"")
                     .arg(databaseException.userMessage()));

        debug << debugMessage.toStdString().c_str();
        return debug;
    }
private:
    int m_code;
    QString m_message;
    QString m_userMessage;
};

#endif // DATABASEEXCEPTION_H
