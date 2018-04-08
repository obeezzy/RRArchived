#include "databaseexception.h"
#include <QObject>

DatabaseException::DatabaseException(int errorCode) :
    m_code(errorCode)
{

}

DatabaseException::DatabaseException(int errorCode, const QString &message, const QString &userMessage) :
    m_code(errorCode), m_message(message), m_userMessage(userMessage)
{

}

DatabaseException::~DatabaseException() throw ()
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

const char *DatabaseException::what() const throw ()
{
    return QObject::tr("Error %1: %2 [%3]").arg(QString::number((int) m_code), m_message, m_userMessage).toStdString().c_str();
}
