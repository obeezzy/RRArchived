#include "viewpurchasehome.h"
#include "database/databaseexception.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

using namespace PurchaseQuery;

ViewPurchaseHome::ViewPurchaseHome(QObject *receiver) :
    PurchaseExecutor(COMMAND, {
                        { "from_date_time", QDateTime(QDate(QDate::currentDate().year(), 1, 1), QTime(0, 0)) },
                        { "to_date_time", QDateTime::currentDateTime() },
                        { "limit", 5 }
                     }, receiver)
{

}

QueryResult ViewPurchaseHome::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();
    QSqlQuery q(connection);
    QVariantList homeRecords;

    try {
        /* Last Purchased Products */ {
            const QList<QSqlRecord> &records(callProcedure("GetLastPurchasedProducts", {
                                                               ProcedureArgument {
                                                                   ProcedureArgument::Type::In,
                                                                   "from_date_time",
                                                                   params.value("from_date_time")
                                                               },
                                                               ProcedureArgument {
                                                                   ProcedureArgument::Type::In,
                                                                   "to_date_time",
                                                                   params.value("to_date_time")
                                                               },
                                                               ProcedureArgument {
                                                                   ProcedureArgument::Type::In,
                                                                   "limit",
                                                                   params.value("limit")
                                                               }
                                                           }));

            QVariantList lastPurchasedProducts;
            for (const auto &record : records)
                lastPurchasedProducts.append(recordToMap(record));

            QVariantMap lastPurchasedProductsInfo;
            lastPurchasedProductsInfo.insert("chart_type", "last_purchased_products");
            lastPurchasedProductsInfo.insert("chart_model", lastPurchasedProductsInfo);

            if (!lastPurchasedProducts.isEmpty())
                homeRecords.append(lastPurchasedProducts);
        }

        QVariantMap outcome;
        outcome.insert("record_count", homeRecords.count());
        outcome.insert("records", homeRecords);
        result.setOutcome(outcome);
        return result;
    } catch (DatabaseException &) {
        throw;
    }
}
