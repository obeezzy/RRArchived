#include "viewsalehome.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"
#include "user/userprofile.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

using namespace SaleQuery;

ViewSaleHome::ViewSaleHome(QObject *receiver) :
    SaleExecutor(COMMAND, {
                        { "from_date", QDate(QDate::currentDate().year(), 1, 1) },
                        { "to_date", QDate::currentDate() },
                        { "from_date_time", QDateTime(QDate(QDate::currentDate().year(), 1, 1), QTime(0, 0)) },
                        { "to_date_time", QDateTime::currentDateTime() },
                        { "limit", 5 }
                 }, receiver)
{

}

QueryResult ViewSaleHome::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QVariantMap params = request().params();
    QSqlQuery q(connection);
    QVariantList homeRecords;

    try {
        /* Total Revenue */ {
            const QList<QSqlRecord> records(callProcedure("GetTotalRevenue", {
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "from_date",
                                                                  params.value("from_date")
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "to_date",
                                                                  params.value("to_date")
                                                              }
                                                          }));

            QVariantList revenues;
            for (const QSqlRecord &record : records) {
                revenues.append(recordToMap(record));
            }

            QVariantMap totalRevenueInfo;
            totalRevenueInfo.insert("data_type", "total_revenue");
            totalRevenueInfo.insert("data_model", revenues);

            if (!revenues.isEmpty())
                homeRecords.append(totalRevenueInfo);
        }
        /* Most Sold Items */ {
            const QList<QSqlRecord> records(callProcedure("GetMostSoldItems", {
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

            QVariantList mostSoldItems;
            for (const QSqlRecord &record : records) {
                mostSoldItems.append(recordToMap(record));
            }

            QVariantMap mostSoldItemsInfo;
            mostSoldItemsInfo.insert("data_type", "most_sold_items");
            mostSoldItemsInfo.insert("data_model", mostSoldItems);

            if (!mostSoldItems.isEmpty())
                homeRecords.append(mostSoldItemsInfo);
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
