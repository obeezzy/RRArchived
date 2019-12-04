#ifndef DATABASEERROR_H
#define DATABASEERROR_H

#include <type_traits>
#include <QString>
#include <QHash>

namespace DatabaseError
{
    enum class QueryErrorCode {
        UnknownError,
        NotYetImplementedError,
        DatabaseInitializationFailed,
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
        UpdateDebtorFailure,
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
        ResultMismatch,
        ViewClientsFailure,
        ProcedureFailed,
        EmptyResultSet,
        ImageTooLarge,
        InsufficientUserPrivileges,
        AddUserFailed,
        OldPasswordWrong,
        UserAccountIsLocked,
        UserPreviouslyArchived
    };

    enum class MySqlErrorCode {
        UncommonError,
        DuplicateEntryError = 1062,
        CreateUserError = 1396,
        UserDefinedException = 1644,
        UserAccountIsLockedError = 3118
    };

    static QHash<QString, QueryErrorCode> queryErrorHash = {
        { "invalid-credentials", QueryErrorCode::SignInFailure }
    };

    template <typename Enumeration>
    constexpr auto asInteger(Enumeration const value) -> typename std::underlying_type<Enumeration>::type
    {
        return static_cast<typename std::underlying_type<Enumeration>::type>(value);
    }
};

#endif // DATABASEERROR_H
