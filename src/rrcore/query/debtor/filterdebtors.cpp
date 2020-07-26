#include "filterdebtors.h"
#include "database/databaseexception.h"
#include "utility/common/filtercriteria.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

using namespace Query::Debtor;

FilterDebtors::FilterDebtors(const Utility::FilterCriteria &filterCriteria,
                             QObject *receiver) :
    DebtorExecutor(COMMAND, {
                    { "filter_column", filterCriteria.column },
                    { "filter_text", filterCriteria.text }
                   }, receiver)
{

}

QueryResult FilterDebtors::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    const QVariantMap &params = request().params();
    QVariantList debtors;

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        QueryExecutor::enforceArguments({ "filter_text",
                                          "filter_column"
                                        }, params);

        filterDebtors(debtors);

        result.setOutcome(QVariantMap {
                              { "debtors", debtors },
                              { "record_count", debtors.count() }
                          });
        return result;
    } catch (const DatabaseException &) {
        throw;
    }
}

void FilterDebtors::filterDebtors(QVariantList &debtors)
{
    const QVariantMap &params = request().params();
    const QString &filterColumn = params.value("filter_column").toString();

    if (filterColumn == QStringLiteral("preferred_name")) {
        filterDebtorsByName(debtors);
    } else {
        const auto &records = callProcedure("FilterDebtors", {
                                                ProcedureArgument {
                                                    ProcedureArgument::Type::In,
                                                    "archived",
                                                    params.value("archived")
                                                }
                                            });
        for (const auto &record : records)
            debtors.append(recordToMap(record));
    }
}

void FilterDebtors::filterDebtorsByName(QVariantList &debtors)
{
    const QVariantMap &params = request().params();
    const auto &records =  callProcedure("FilterDebtorsByName", {
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

    for (const auto &record : records)
        debtors.append(recordToMap(record));
}
