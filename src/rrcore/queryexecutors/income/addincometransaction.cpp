#include "addincometransaction.h"
#include "database/databaseutils.h"
#include "database/databaseexception.h"
#include "user/userprofile.h"
#include "singletons/settings.h"
#include "utility/commonutils.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace IncomeQuery;

AddIncomeTransaction::AddIncomeTransaction(const Client &client,
                                           const QString &purpose,
                                           qreal amount,
                                           const Utility::PaymentMethod &paymentMethod,
                                           QObject *receiver) :
    IncomeExecutor(COMMAND, {
                        { "client_name", client.preferredName },
                        { "purpose", purpose },
                        { "amount", amount },
                        { "payment_method", Utility::asString(paymentMethod) },
                        { "currency", Settings::DEFAULT_CURRENCY }
                   }, receiver)
{

}

QueryResult IncomeQuery::AddIncomeTransaction::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    const QVariantMap &params = request().params();
    const QString &note = params.value("note").toString();
    int noteId = 0;

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        DatabaseUtils::beginTransaction(q);

        noteId = QueryExecutor::addNote(note,
                                        QStringLiteral("income"),
                                        ExceptionPolicy::DisallowExceptions);

        callProcedure("AddIncomeTransaction", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "client_id",
                              params.value("client_id")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "name",
                              params.value("client_name")
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
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}
