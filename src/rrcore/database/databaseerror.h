#ifndef DATABASEERROR_H
#define DATABASEERROR_H

namespace DatabaseError
{
    enum class RRErrorCode {
        Unknown,
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
};

#endif // DATABASEERROR_H
