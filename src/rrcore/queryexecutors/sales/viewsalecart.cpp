#include "viewsalecart.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"
#include "user/userprofile.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace SaleQuery;

ViewSaleCart::ViewSaleCart(qint64 transactionId, QObject *receiver) :
    SaleExecutor(COMMAND, {
                    { "transaction_id", transactionId },
                    { "sale_transaction_archived", false },
                    { "sale_item_archived", false }
                 }, receiver)
{

}

QueryResult ViewSaleCart::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();
    QSqlQuery q(connection);

    try {
        const QList<QSqlRecord> records(callProcedure("ViewSaleCart", {
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::In,
                                                              "transaction_id",
                                                              params.value("transaction_id")
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::In,
                                                              "sale_transaction_archived",
                                                              params.value("sale_transaction_archived")
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::In,
                                                              "sale_item_archived",
                                                              params.value("sale_item_archived")
                                                          }
                                                      }));

        QVariantList items;
        for (const QSqlRecord &record : records)
            items.append(recordToMap(record));

        if (!items.isEmpty()) {
            QVariantMap outcome { items.first().toMap() };
            outcome.insert("items", items);
            outcome.insert("record_count", items.count());
            result.setOutcome(outcome);
        }
        return result;
    } catch (DatabaseException &) {
        throw;
    }
}
