#ifndef DATABASEEXCEPTION_H
#define DATABASEEXCEPTION_H

#include <exception>
#include <QString>
#include "databaseerror.h"

class DatabaseException : public std::exception
{
public:
    explicit DatabaseException(int errorCode);
    explicit DatabaseException(int errorCode, const QString &message, const QString &userMessage = "");

    explicit DatabaseException(DatabaseError::RRErrorCode errorCode);
    explicit DatabaseException(DatabaseError::RRErrorCode errorCode, const QString &message, const QString &userMessage = "");

    explicit DatabaseException(DatabaseError::MySqlErrorCode errorCode);
    explicit DatabaseException(DatabaseError::MySqlErrorCode errorCode, const QString &message, const QString &userMessage = "");

    int code() const;
    QString message() const;
    QString userMessage() const;
    const char *what() const noexcept override final;
private:
    int m_code;
    QString m_message;
    QString m_userMessage;
};

#endif // DATABASEEXCEPTION_H
