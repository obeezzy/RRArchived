#include "viewdebtors.h"
#include "database/databaseexception.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

using namespace Query::Debtor;

ViewDebtors::ViewDebtors(QObject *receiver) :
    DebtorExecutor(COMMAND, {
                    { "archived", false }
                   }, receiver)
{

}

ViewDebtors::ViewDebtors(const Utility::RecordGroup::Flags &flags,
                         QObject *receiver) :
    DebtorExecutor(COMMAND, {
                    { "archived", flags.testFlag(Utility::RecordGroup::Archived) }
                   }, receiver)
{

}

QueryResult ViewDebtors::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    const QVariantMap &params = request().params();

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        const auto &records = callProcedure("ViewDebtors", {
                                                ProcedureArgument {
                                                    ProcedureArgument::Type::In,
                                                    "archived",
                                                    params.value("archived")
                                                }
                                            });

        QVariantList debtors;
        for (const auto &record : records)
            debtors.append(recordToMap(record));

        result.setOutcome(QVariantMap {
                              { "debtors", debtors },
                              { "record_count", debtors.count() }
                          });
        return result;
    } catch (const DatabaseException &) {
        throw;
    }
}
