#include "networkexception.h"
#include <QObject>
#include "network/networkerror.h"

NetworkException::NetworkException(int errorCode, const QString &message,
                                   int statusCode, QString statusMessage,
                                   const QString &userMessage) :
    m_code(errorCode),
    m_message(message),
    m_statusCode(statusCode),
    m_statusMessage(statusMessage),
    m_userMessage(userMessage)
{

}

NetworkException::NetworkException(NetworkError::ServerErrorCode errorCode, const QString &message,
                                   int statusCode, QString statusMessage,
                                   const QString &userMessage) :
    m_code(NetworkError::asInteger(errorCode)),
    m_message(message),
    m_statusCode(statusCode),
    m_statusMessage(statusMessage),
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

int NetworkException::statusCode() const
{
    return m_statusCode;
}

QString NetworkException::statusMessage() const
{
    return m_statusMessage;
}

QString NetworkException::userMessage() const
{
    return m_userMessage;
}

const char *NetworkException::what() const noexcept
{
    return QObject::tr("Error %1: %2 [%3]").arg(QString::number(m_code),
                                                m_message,
                                                m_userMessage).toStdString().c_str();
}
