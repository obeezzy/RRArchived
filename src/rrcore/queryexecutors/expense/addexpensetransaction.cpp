#include "addexpensetransaction.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"
#include "user/userprofile.h"
#include "singletons/settings.h"
#include "utility/commonutils.h"
#include "utility/expenseutils.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace ExpenseQuery;

AddExpenseTransaction::AddExpenseTransaction(const Utility::ExpenseTransaction &transaction,
                                             QObject *receiver) :
    ExpenseExecutor(COMMAND, {
                    { "name", transaction.client.preferredName },
                    { "purpose", transaction.purpose },
                    { "amount", transaction.amount },
                    { "payment_method", transaction.paymentMethod.toString() },
                    { "currency", Settings::DEFAULT_CURRENCY },
                    }, receiver)
{

}

QueryResult AddExpenseTransaction::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    const QVariantMap &params = request().params();
    int noteId = 0;
    const QString &note = params.value("note").toString();

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        DatabaseUtils::beginTransaction(q);

        noteId = QueryExecutor::addNote(note,
                                        QStringLiteral("expense"),
                                        ExceptionPolicy::DisallowExceptions);

        callProcedure("AddExpenseTransaction", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "client_id",
                              params.value("client_id")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "name",
                              params.value("name")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "purpose",
                              params.value("purpose")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "amount",
                              params.value("amount")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "payment_method",
                              params.value("payment_method")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "currency",
                              params.value("currency")
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
                      });

        DatabaseUtils::commitTransaction(q);
        return result;
    } catch (const DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}
