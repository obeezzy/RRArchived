#include "viewpurchasetransactions.h"
#include "database/databaseexception.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace PurchaseQuery;

ViewPurchaseTransactions::ViewPurchaseTransactions(const QDateTime &from,
                                                   const QDateTime &to,
                                                   bool suspended,
                                                   bool archived,
                                                   QObject *receiver) :
    PurchaseExecutor(COMMAND, {
                        { "from", from },
                        { "to", to },
                        { "suspended", suspended },
                        { "archived", archived }
                     }, receiver)
{

}

QueryResult PurchaseQuery::ViewPurchaseTransactions::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();
    QSqlQuery q(connection);

    try {
        const QList<QSqlRecord> &records(callProcedure("ViewPurchaseTransactions", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "suspended",
                                                               params.value("suspended")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "archived",
                                                               params.value("archived")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "from",
                                                               params.value("from")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "to",
                                                               params.value("to")
                                                           }
                                                       }));

        QVariantList transactions;
        for (const QSqlRecord &record : records) {
            transactions.append(recordToMap(record));
        }

        result.setOutcome(QVariantMap { { "transactions", transactions }, { "record_count", transactions.count() } });
        return result;
    } catch (DatabaseException &) {
        throw;
    }
}
