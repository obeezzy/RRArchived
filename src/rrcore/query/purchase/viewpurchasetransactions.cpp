#include "viewpurchasetransactions.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "database/databaseexception.h"
#include "utility/common/datetimespan.h"

using namespace Query::Purchase;

ViewPurchaseTransactions::ViewPurchaseTransactions(
    const Utility::DateTimeSpan& dateTimeSpan,
    const Utility::RecordGroup::Flags& flags, QObject* receiver)
    : PurchaseExecutor(
          COMMAND,
          {{"from", dateTimeSpan.from},
           {"to", dateTimeSpan.to},
           {"suspended", flags.testFlag(Utility::RecordGroup::Suspended)},
           {"archived", flags.testFlag(Utility::RecordGroup::Archived)}},
          receiver)
{}

QueryResult ViewPurchaseTransactions::execute()
{
    QueryResult result{request()};
    result.setSuccessful(true);

    const QVariantMap& params = request().params();

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        const auto& records(callProcedure(
            "ViewPurchaseTransactions",
            {ProcedureArgument{ProcedureArgument::Type::In, "suspended",
                               params.value("suspended")},
             ProcedureArgument{ProcedureArgument::Type::In, "archived",
                               params.value("archived")},
             ProcedureArgument{ProcedureArgument::Type::In, "from",
                               params.value("from")},
             ProcedureArgument{ProcedureArgument::Type::In, "to",
                               params.value("to")}}));

        QVariantList transactions;
        for (const auto& record : records)
            transactions.append(recordToMap(record));

        result.setOutcome(QVariantMap{{"transactions", transactions},
                                      {"record_count", transactions.count()}});
        return result;
    } catch (const DatabaseException&) {
        throw;
    }
}
