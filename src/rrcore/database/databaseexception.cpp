#include "databaseexception.h"
#include "databaseerror.h"
#include <QObject>

using namespace DatabaseError;

DatabaseException::DatabaseException(int errorCode,
                                     const QString &message,
                                     const QSqlError &error) :
    m_code(errorCode),
    m_message(message),
    m_sqlError(error)
{}

DatabaseException::DatabaseException(QueryErrorCode errorCode,
                                     const QString &message,
                                     const QSqlError &error) :
    m_code(static_cast<int>(errorCode)),
    m_message(message),
    m_sqlError(error)
{}

DatabaseException::DatabaseException(MySqlErrorCode errorCode,
                                     const QString &message,
                                     const QSqlError &error) :
    m_code(static_cast<int>(errorCode)),
    m_message(message),
    m_sqlError(error)
{}

int DatabaseException::code() const
{
    return m_code;
}

QString DatabaseException::message() const
{
    return m_message;
}

QSqlError DatabaseException::sqlError() const
{
    return m_sqlError;
}

const char *DatabaseException::what() const noexcept
{
    return QObject::tr("Error %1: %2 [%3]")
            .arg(QString::number(static_cast<int>(m_code)),
                 m_message, m_sqlError.databaseText()).toStdString().c_str();
}
