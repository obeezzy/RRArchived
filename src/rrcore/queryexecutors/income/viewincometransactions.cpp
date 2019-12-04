#include "viewincometransactions.h"
#include "database/databaseexception.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

using namespace IncomeQuery;

ViewIncomeTransactions::ViewIncomeTransactions(const QDateTime &from,
                                               const QDateTime &to,
                                               bool archived,
                                               QObject *receiver) :
    IncomeExecutor(COMMAND, {
                        { "from", from },
                        { "to", to },
                        { "archived", archived }
                   }, receiver)
{

}

QueryResult ViewIncomeTransactions::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();

    QSqlQuery q(connection);

    try {
        // STEP: Insert income transaction
        const QList<QSqlRecord> &records (callProcedure("ViewIncomeTransactions", {
                                                            ProcedureArgument {
                                                                ProcedureArgument::Type::In,
                                                                "from",
                                                                params.value("from")
                                                            },
                                                            ProcedureArgument {
                                                                ProcedureArgument::Type::In,
                                                                "to",
                                                                params.value("to")
                                                            },
                                                            ProcedureArgument {
                                                                ProcedureArgument::Type::In,
                                                                "archived",
                                                                params.value("archived", false)
                                                            }
                                                        }));

        QVariantList transactions;
        for (const QSqlRecord &record : records) {
            transactions.append(recordToMap(record));
        }

        result.setOutcome(QVariantMap {
                              { "transactions", transactions },
                              { "record_count", transactions.count() }
                          });

        return result;
    } catch (DatabaseException &) {
        throw;
    }
}
