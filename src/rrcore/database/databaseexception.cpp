#include "databaseexception.h"
#include <QObject>
#include "databaseerror.h"

using namespace DatabaseError;

DatabaseException::DatabaseException(int errorCode) :
    m_code(errorCode)
{

}

DatabaseException::DatabaseException(int errorCode, const QString &message, const QString &userMessage) :
    m_code(errorCode), m_message(message), m_userMessage(userMessage)
{

}

DatabaseException::DatabaseException(QueryErrorCode errorCode) :
    m_code(static_cast<int>(errorCode))
{
    switch (errorCode) {
    case QueryErrorCode::NotYetImplementedError:
        m_message = QStringLiteral("Not yet implemented");
        m_userMessage = QStringLiteral("Not yet implemented");
        break;
    case QueryErrorCode::NoCommand:
        m_message = QStringLiteral("No command set.");
        m_userMessage = QStringLiteral("No command set.");
        break;
    default:
        break;
    }
}

DatabaseException::DatabaseException(QueryErrorCode errorCode, const QString &message, const QString &userMessage) :
    m_code(static_cast<int>(errorCode)), m_message(message), m_userMessage(userMessage)
{

}

int DatabaseException::code() const
{
    return m_code;
}

QString DatabaseException::message() const
{
    return m_message;
}

QString DatabaseException::userMessage() const
{
    return m_userMessage;
}

const char *DatabaseException::what() const noexcept
{
    return QObject::tr("Error %1: %2 [%3]")
            .arg(QString::number(static_cast<int>(m_code)),
                 m_message, m_userMessage).toStdString().c_str();
}
