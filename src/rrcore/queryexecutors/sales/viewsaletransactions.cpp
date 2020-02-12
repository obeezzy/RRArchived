#include "viewsaletransactions.h"
#include "database/databaseexception.h"
#include "utility/commonutils.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

using namespace SaleQuery;

ViewSaleTransactions::ViewSaleTransactions(const DateTimeSpan &dateTimeSpan,
                                           const RecordGroup::Flags &RecordGroups,
                                           QObject *receiver) :
    SaleExecutor(COMMAND, {
                    { "from", dateTimeSpan.from },
                    { "to", dateTimeSpan.to },
                    { "suspended", RecordGroups.testFlag(RecordGroup::Suspended) },
                    { "archived", RecordGroups.testFlag(RecordGroup::Archived) }
                 }, receiver)
{

}

QueryResult ViewSaleTransactions::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    const QVariantMap &params = request().params();

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        const auto &records(callProcedure("ViewSaleTransactions", {
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
                                                  "suspended",
                                                  params.value("suspended")
                                              },
                                              ProcedureArgument {
                                                  ProcedureArgument::Type::In,
                                                  "archived",
                                                  params.value("archived")
                                              }
                                          }));

        QVariantList transactions;
        for (const auto &record : records)
            transactions.append(recordToMap(record));

        result.setOutcome(QVariantMap {
                              { "transactions", transactions },
                              { "record_count", transactions.count() }
                          });
        return result;
    } catch (DatabaseException &) {
        throw;
    }
}
