#ifndef NETWORKEXCEPTION_H
#define NETWORKEXCEPTION_H

#include <exception>
#include <QString>

class NetworkException : public std::exception
{
public:
    NetworkException(int errorCode,
                     const QString &message,
                     const QString &userMessage);
    ~NetworkException() override;

    int code() const;
    QString message() const;
    QString userMessage() const;
    const char * what() const noexcept override final;
private:
    int m_code;
    QString m_message;
    QString m_userMessage;
};

#endif // NETWORKEXCEPTION_H
