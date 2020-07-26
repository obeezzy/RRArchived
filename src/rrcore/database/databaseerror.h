#ifndef DATABASEERROR_H
#define DATABASEERROR_H

#include <QHash>
#include <QString>
#include <type_traits>

namespace DatabaseError {
enum class QueryErrorCode
{
    UnknownError,
    NotYetImplementedError,
    DatabaseInitializationFailed,
    SignUpFailure,
    MissingCommandError,
    BeginTransactionFailed,
    CommitTransationFailed,
    SqlStatementFailedError,
    CreateUserFailed,
    InvalidArguments,
    MissingArguments,
    DuplicateEntryFailure,
    AmountOverpaid,
    InvalidDueDate,
    ResultMismatch,
    ProcedureFailed,
    ImageTooLarge,
    IncorrectPasswordError,
    UserAccountIsLocked,
    UserPreviouslyArchived,
    ArgumentMismatch,
    UnexpectedResultError,
    NoExistingRecordError,
    InvalidCredentialsError,
    ConnectionFailedError
};

enum class MySqlErrorCode
{
    UncommonError,
    AccessDeniedError = 1045,
    DuplicateEntryError = 1062,
    CreateUserError = 1396,
    UserDefinedException = 1644,
    UserAccountIsLockedError = 3118
};

static QHash<QString, QueryErrorCode> queryErrorHash = {
    {"invalid-credentials", QueryErrorCode::InvalidCredentialsError}};

template <typename Enumeration>
constexpr auto asInteger(Enumeration const value) ->
    typename std::underlying_type<Enumeration>::type
{
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}
};  // namespace DatabaseError

#endif  // DATABASEERROR_H
