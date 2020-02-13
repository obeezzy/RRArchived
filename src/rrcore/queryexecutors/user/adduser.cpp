#include "adduser.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"
#include "user/userprofile.h"
#include "utility/userutils.h"
#include "database/exceptions/exceptions.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace UserQuery;

AddUser::AddUser(const Utility::User &user,
                 QObject *receiver) :
    UserExecutor(COMMAND, {
                        { "first_name", user.firstName },
                        { "last_name", user.lastName },
                        { "user", user.user },
                        { "password", user.password },
                        { "phone_number", user.phoneNumber },
                        { "email_address", user.emailAddress },
                        { "image_url", user.imageUrl },
                        { "user_id", UserProfile::instance().userId() }
                 }, receiver)
{

}

QueryResult AddUser::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        DatabaseUtils::beginTransaction(q);

        const int userId = addRRUser();
        addUserPrivileges(userId);
        addSqlUser();

        DatabaseUtils::commitTransaction(q);
        return result;
    } catch (DatabaseException &e) {
        DatabaseUtils::rollbackTransaction(q);

        const QString &userName = request().params().value("user").toString();
        switch (e.code()) {
        case static_cast<int>(DatabaseError::MySqlErrorCode::DuplicateEntryError):
            throw DuplicateEntryException(QStringLiteral("User '%1' already exists.")
                                          .arg(userName));
        case static_cast<int>(DatabaseError::MySqlErrorCode::CreateUserError):
            throw FailedToCreateUserException(e.message());
        case static_cast<int>(DatabaseError::MySqlErrorCode::UserDefinedException):
            throw UserPreviouslyArchivedException(QStringLiteral("User '%1' has been archived. "
                                                                 "Unarchive the user or create a user with another name.")
                                                  .arg(userName));
        default:
            throw;
        }
    }
}

int AddUser::addRRUser()
{
    const QVariantMap &params = request().params();
    const QString &note = params.value("note").toString();
    const int noteId = QueryExecutor::addNote(note,
                                              QStringLiteral("user"),
                                              ExceptionPolicy::DisallowExceptions);

    const auto &records(callProcedure("AddRRUser", {
                                          ProcedureArgument {
                                              ProcedureArgument::Type::In,
                                              "user",
                                              params.value("user")
                                          },
                                          ProcedureArgument {
                                              ProcedureArgument::Type::In,
                                              "first_name",
                                              params.value("first_name")
                                          },
                                          ProcedureArgument {
                                              ProcedureArgument::Type::In,
                                              "last_name",
                                              params.value("last_name")
                                          },
                                          ProcedureArgument {
                                              ProcedureArgument::Type::In,
                                              "photo",
                                              DatabaseUtils::imageUrlToByteArray(params.value("image_url").toUrl())
                                          },
                                          ProcedureArgument {
                                              ProcedureArgument::Type::In,
                                              "phone_number",
                                              params.value("phone_number")
                                          },
                                          ProcedureArgument {
                                              ProcedureArgument::Type::In,
                                              "email_address",
                                              params.value("email_address")
                                          },
                                          ProcedureArgument {
                                              ProcedureArgument::Type::In,
                                              "note_id",
                                              noteId
                                          },
                                          ProcedureArgument {
                                              ProcedureArgument::Type::In,
                                              "user_id",
                                              UserProfile::instance().userId()
                                          }
                                      }));

    if (records.isEmpty())
        throw UnexpectedResultException(QStringLiteral("Expected user ID, received nothing."));

    return records.first().value("user_id").toInt();
}

void AddUser::addUserPrivileges(int userId)
{
    const QVariantMap &params = request().params();

    callProcedure("AddUserPrivileges", {
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "user_privileges",
                          params.value("user_privileges")
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "user_id",
                          userId
                      }
                  });
}

void AddUser::addSqlUser()
{
    const QVariantMap &params = request().params();

    callProcedure("AddSqlUser", {
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "user",
                          params.value("user")
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "password",
                          params.value("password")
                      }
                  });
}
