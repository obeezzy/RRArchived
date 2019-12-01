#ifndef NETWORKERROR_H
#define NETWORKERROR_H

#include <QString>
#include <QHash>
#include <type_traits>

namespace NetworkError
{
    enum class StatusCode {
        Unknown,
        ConnectionRefusedError,
        RemoteHostClosedError,
        HostNotFoundError,
        TimeoutError,
        OperationCanceledError,
        SslHandshakeFailedError,
        TemporaryNetworkFailureError,
        NetworkSessionFailedError,
        BackgroundRequestNotAllowedError,
        TooManyRedirectsError,
        InsecureRedirectError,
        ProxyConnectionRefusedError = 101,
        ProxyConnectionClosedError,
        ProxyNotFoundError,
        ProxyTimeoutError,
        ProxyAuthenticationRequiredError,
        RequestOk = 200,
        ContentAccessDenied = 201,
        ContentOperationNotPermittedError,
        ContentNotFoundError,
        AuthenticationRequiredError,
        ContentReSendError,
        ContentConflictError,
        ContentGoneError,
        BadRequestError = 400,
        InternalServerError = 401,
        OperationNotImplementedError = 402,
        ServiceUnavailaleError = 403,
        PageNotFoundError,
        ProtocolUnknownError = 301,
        ProtocolInvalidOpartionError = 302,
        UnknownNetworkError = 99,
        UnknownProxyError = 199,
        UnknownContentError = 299,
        ProtocolFailure = 399,
        UnknownServerError = 499
    };

    enum class MySqlErrorCode {
        UncommonError,
        DuplicateEntryError = 1062,
        CreateUserError = 1396,
        UserDefinedException = 1644,
        UserAccountIsLockedError = 3118
    };

    enum class ServerErrorCode {
        UnknownError = -1,
        RequestFailed,
        UnableToDetermineDestinationUrl,
        InvalidCredentialsError
    };

    static QHash<QString, ServerErrorCode> serverErrorHash = {
        { "invalid-credentials", ServerErrorCode::InvalidCredentialsError }
    };

    template <typename Enumeration>
    constexpr auto asInteger(Enumeration const value) -> typename std::underlying_type<Enumeration>::type
    {
        return static_cast<typename std::underlying_type<Enumeration>::type>(value);
    }
};

#endif // NETWORKERROR_H
