#include "addincometransaction.h"
#include "database/databaseutils.h"
#include "database/databaseexception.h"
#include "user/userprofile.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace IncomeQuery;

AddIncomeTransaction::AddIncomeTransaction(const QString &clientName,
                                           const QString &purpose,
                                           qreal amount,
                                           const QString &paymentMethod,
                                           QObject *receiver) :
    IncomeExecutor(COMMAND, {
                        { "client_name", clientName },
                        { "purpose", purpose },
                        { "amount", amount },
                        { "payment_method", paymentMethod }
                   }, receiver)
{

}

QueryResult IncomeQuery::AddIncomeTransaction::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();
    int noteId = 0;

    QSqlQuery q(connection);

    try {
        DatabaseUtils::beginTransaction(q);

        if (params.contains("note")) {
            // STEP: Insert note
            noteId = addNote(params.value("note").toString(), "income");
        }

        // STEP: Insert income transaction
        callProcedure("AddIncomeTransaction", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "client_id",
                              params.value("client_id", QVariant::Int)
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
                              params.value("currency", "NGN")
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
