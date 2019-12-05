#include "adduser.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"
#include "user/userprofile.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace UserQuery;

AddUser::AddUser(const QString &firstName,
                 const QString &lastName,
                 const QString &userName,
                 const QString &password,
                 const QString &phoneNumber,
                 const QString &emailAddress,
                 const QUrl &imageUrl,
                 QObject *receiver) :
    UserExecutor(COMMAND, {
                        { "first_name", firstName },
                        { "last_name", lastName },
                        { "user_name", userName },
                        { "password", password },
                        { "phone_number", phoneNumber },
                        { "email_address", emailAddress },
                        { "image_url", imageUrl },
                        { "user_id", UserProfile::instance().userId() }
                 }, receiver)
{

}

QueryResult AddUser::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);
    const QVariantMap &params = request().params();
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        DatabaseUtils::beginTransaction(q);

        const QList<QSqlRecord> &records(callProcedure("AddRRUser", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "user_name",
                                                               params.value("user_name")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "password",
                                                               DatabaseUtils::createPasswordHash(params.value("password").toString())
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
                                                               params.value("note_id")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "user_id",
                                                               params.value("user_id")
                                                           }
                                                       }));

        if (!records.isEmpty()) {
            callProcedure("AddUserPrivileges", {
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "user_privileges",
                                  params.value("user_privileges")
                              },
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "user_id",
                                  records.first().value("user_id")
                              }
                          });

        } else {
            throw DatabaseException(DatabaseError::QueryErrorCode::AddUserFailed,
                                    "No user ID returned.");
        }


        callProcedure("AddSqlUser", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_name",
                              params.value("user_name")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "password",
                              params.value("password")
                          }
                      });

        DatabaseUtils::commitTransaction(q);
        return result;
    } catch (DatabaseException &e) {
        DatabaseUtils::rollbackTransaction(q);

        switch (e.code()) {
        case static_cast<int>(DatabaseError::MySqlErrorCode::DuplicateEntryError):
            throw DatabaseException(DatabaseError::QueryErrorCode::DuplicateEntryFailure, e.message(), e.userMessage());
        case static_cast<int>(DatabaseError::MySqlErrorCode::CreateUserError):
            throw DatabaseException(DatabaseError::QueryErrorCode::CreateUserFailed, e.message(), e.userMessage());
        case static_cast<int>(DatabaseError::MySqlErrorCode::UserDefinedException):
            throw DatabaseException(DatabaseError::QueryErrorCode::UserPreviouslyArchived, e.message(), e.userMessage());
        default:
            throw;
        }
    }
}
