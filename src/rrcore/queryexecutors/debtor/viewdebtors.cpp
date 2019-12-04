#include "viewdebtors.h"
#include "database/databaseexception.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

using namespace DebtorQuery;

ViewDebtors::ViewDebtors(QObject *receiver) :
    DebtorExecutor(COMMAND, { }, receiver)
{

}

ViewDebtors::ViewDebtors(const QString &filterText,
                         const QString &filterColumn,
                         QObject *receiver) :
    DebtorExecutor(COMMAND, {
                        { "filter_text", filterText },
                        { "filter_column", filterColumn }
                   }, receiver)
{

}

QueryResult ViewDebtors::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();
    QList<QSqlRecord> records;
    QSqlQuery q(connection);

    try {
        // STEP: Get total balance for each debtor.
        if (params.value("filter_text").isNull() || params.value("filter_column").isNull()) {
            records = callProcedure("ViewDebtors", {
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "archived",
                                            params.value("archived", false)
                                        }
                                    });
        } else {
            // STEP: Filter total balance for each debtor
            if (params.value("filter_column").toString() == "preferred_name") {
                records = callProcedure("FilterDebtorsByName", {
                                            ProcedureArgument {
                                                ProcedureArgument::Type::In,
                                                "name",
                                                params.value("filter_text")
                                            },
                                            ProcedureArgument {
                                                ProcedureArgument::Type::In,
                                                "archived",
                                                params.value("archived")
                                            }
                                        });
            }
        }

        QVariantList debtors;
        for (const QSqlRecord &record : records)
            debtors.append(recordToMap(record));

        result.setOutcome(QVariantMap {
                              { "debtors", debtors },
                              { "record_count", debtors.count() }
                          });
        return result;
    } catch (DatabaseException &) {
        throw;
    }
}
