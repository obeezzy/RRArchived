#ifndef DATABASEEXCEPTION_H
#define DATABASEEXCEPTION_H

#include <exception>
#include <QString>

class DatabaseException : public std::exception
{
public:
    enum class RRErrorCode {
        Unknown,
        SignInFailure,
        SignUpFailure,
        RemoveUserFailure,
        NoCommand,
        CommandNotFound,
        RequestTypeNotFound,
        NoValidConnection,
        AddItemFailure,
        UpdateItemFailure,
        BeginTransactionFailed,
        CommitTransationFailed,
        ViewStockItemsFailed,
        ViewStockCategoriesFailed,
        ViewStockItemDetailsFailed,
        AddTransactionFailure,
        UpdateTransactionFailure,
        UndoAddTransactionFailure,
        ViewSaleTransactionFailure,
        ConnectToTestDatabaseFailed,
        RunSqlOnTestDatabaseFailed,
        AddDebtorFailure,
        RemoveDebtorFailure,
        UndoRemoveDebtorFailure,
        ViewDebtorsFailure,
        ViewDebtTransactionsFailure,
        UndoAddDebtorFailure,
        ViewSaleHomeFailure,
        RemoveStockItemFailed,
        CreateTableFailed,
        CreateUserFailed,
        InvalidArguments,
        MissingArguments,
        UndoFailed,
        DuplicateEntryFailure,
        AmountOverpaid,
        InvalidDueDate,
        ResultMismatch
    };

    enum class MySqlErrorCode {
        DuplicateEntryError = 1062
    };
    explicit DatabaseException(int errorCode);
    explicit DatabaseException(RRErrorCode errorCode, const QString &message, const QString &userMessage = "");
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
