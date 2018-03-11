#ifndef DATABASEEXCEPTION_H
#define DATABASEEXCEPTION_H

#include <exception>
#include <QString>

class DatabaseException : public std::exception
{
public:
    enum ErrorCode {
        Unknown,
        SignInFailure,
        NoCommand,
        AddItemFailure,
        BeginTransactionFailed,
        CommitTransationFailed,
        UnsuccessfulQueryResult,
        ViewStockItemsFailed,
        ViewStockCategoriesFailed,
        AddTransactionFailure,
        UpdateTransactionFailure,
        UndoAddTransactionFailure,
        ViewSaleTransactionFailure
    };
    explicit DatabaseException(int errorCode);
    explicit DatabaseException(int errorCode, const QString &message, const QString &userMessage = "");
    ~DatabaseException() throw ();

    int code() const;
    QString message() const;
    QString userMessage() const;
    const char *what() const throw () override final;
private:
    int m_code;
    QString m_message;
    QString m_userMessage;
};

#endif // DATABASEEXCEPTION_H
