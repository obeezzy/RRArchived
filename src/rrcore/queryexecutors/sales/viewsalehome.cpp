#include "viewsalehome.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"
#include "user/userprofile.h"
#include "utility/saleutils.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

using namespace SaleQuery;

ViewSaleHome::ViewSaleHome(QObject *receiver) :
    SaleExecutor(COMMAND, {
                    { "limit", 5 }
                 }, receiver)
{

}

QueryResult ViewSaleHome::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    QVariantList homeRecords;

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        fetchTotalRevenue(homeRecords);
        fetchMostSoldProducts(homeRecords);

        result.setOutcome(QVariantMap {
                              { "records", homeRecords },
                              { "record_count", homeRecords.count() }
                          });
        return result;
    } catch (const DatabaseException &) {
        throw;
    }
}

void ViewSaleHome::fetchTotalRevenue(QVariantList &homeRecords)
{
    const QVariantMap &params{ request().params() };
    const auto &records(callProcedure("FetchTotalRevenue", {
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

    QVariantList revenues;
    for (const auto &record : records)
        revenues.append(recordToMap(record));

    QVariantMap totalRevenueInfo;
    totalRevenueInfo.insert("data_type", "total_revenue");
    totalRevenueInfo.insert("data_model", revenues);

    if (!revenues.isEmpty())
        homeRecords.append(totalRevenueInfo);
}

void ViewSaleHome::fetchMostSoldProducts(QVariantList &homeRecords)
{
    const QVariantMap &params{ request().params() };
    const auto &records(callProcedure("FetchMostSoldProducts", {
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
                                              "limit",
                                              params.value("limit")
                                          }
                                      }));

    QVariantList mostSoldProducts;
    for (const auto &record : records)
        mostSoldProducts.append(recordToMap(record));

    QVariantMap mostSoldProductsInfo;
    mostSoldProductsInfo.insert("data_type", "most_sold_products");
    mostSoldProductsInfo.insert("data_model", mostSoldProducts);

    if (!mostSoldProducts.isEmpty())
        homeRecords.append(mostSoldProductsInfo);
}
