#include "viewexpensetransactions.h"
#include "database/databaseexception.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>

using namespace ExpenseQuery;

ViewExpenseTransactions::ViewExpenseTransactions(const QDateTime &from,
                                                 const QDateTime &to,
                                                 bool archived,
                                                 QObject *receiver) :
    ExpenseExecutor(COMMAND, {
                        { "from", from },
                        { "to", to },
                        { "archived", archived }
                    }, receiver)
{

}

QueryResult ViewExpenseTransactions::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();

    QSqlQuery q(connection);

    try {
        QueryExecutor::enforceArguments({ "from", "to" }, params);
        const QList<QSqlRecord> &records (callProcedure("ViewExpenseTransactions", {
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
                                                                params.value("archived")
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
