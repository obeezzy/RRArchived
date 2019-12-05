#ifndef DATABASEEXCEPTION_H
#define DATABASEEXCEPTION_H

#include <exception>
#include <QString>
#include <QDebug>
#include "databaseerror.h"

class DatabaseException : public std::exception
{
public:
    explicit DatabaseException(int errorCode);
    explicit DatabaseException(int errorCode, const QString &message, const QString &userMessage = "");

    explicit DatabaseException(DatabaseError::QueryErrorCode errorCode);
    explicit DatabaseException(DatabaseError::QueryErrorCode errorCode, const QString &message, const QString &userMessage = "");

    explicit DatabaseException(DatabaseError::MySqlErrorCode errorCode);
    explicit DatabaseException(DatabaseError::MySqlErrorCode errorCode, const QString &message, const QString &userMessage = "");

    int code() const;
    QString message() const;
    QString userMessage() const;
    const char *what() const noexcept override final;

    friend QDebug operator<<(QDebug debug, const DatabaseException &databaseException)
    {
        debug.nospace() << "DatabaseException("
                        << "code=" << databaseException.code()
                        << ", message=" << databaseException.message()
                        << ", details=" << databaseException.userMessage()
                        << ")";

        return debug;
    }
private:
    int m_code;
    QString m_message;
    QString m_userMessage;
};

#endif // DATABASEEXCEPTION_H
