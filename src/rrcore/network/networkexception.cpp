#include "networkexception.h"
#include <QObject>

NetworkException::NetworkException(int errorCode, const QString &message, const QString &userMessage) :
    m_code(errorCode),
    m_message(message),
    m_userMessage(userMessage)
{

}

NetworkException::~NetworkException()
{

}

int NetworkException::code() const
{
    return m_code;
}

QString NetworkException::message() const
{
    return m_message;
}

QString NetworkException::userMessage() const
{
    return m_userMessage;
}

const char *NetworkException::what() const noexcept
{
    return QObject::tr("Error %1: %2 [%3]").arg(QString::number(static_cast<int>(m_code)),
                                                m_message,
                                                m_userMessage).toStdString().c_str();
}
