#ifndef NETWORKEXCEPTION_H
#define NETWORKEXCEPTION_H

#include <exception>
#include <QString>
#include <QDebug>

namespace NetworkError {
    enum class ServerErrorCode;
}

class NetworkException : public std::exception
{
public:
    NetworkException(int errorCode,
                     const QString &message,
                     int statusCode = 400,
                     QString statusMessage = QString(),
                     const QString &userMessage = QString());
    NetworkException(NetworkError::ServerErrorCode errorCode,
                     const QString &message,
                     int statusCode = 400,
                     QString statusMessage = QString(),
                     const QString &userMessage = QString());
    ~NetworkException() override;

    int code() const;
    QString message() const;
    int statusCode() const;
    QString statusMessage() const;
    QString userMessage() const;
    const char * what() const noexcept override final;

    friend QDebug operator<<(QDebug debug, const NetworkException &networkException)
    {
        debug.nospace() << "NetworkException("
                        << "code=" << networkException.code()
                        << ", message=" << networkException.message()
                        << ", statusCode=" << networkException.statusCode()
                        << ", statusMessage=" << networkException.statusMessage()
                        << ")";

        return debug;
    }
private:
    int m_code;
    QString m_message;
    int m_statusCode;
    QString m_statusMessage;
    QString m_userMessage;
};

#endif // NETWORKEXCEPTION_H
