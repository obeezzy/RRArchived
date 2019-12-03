#include "viewdashboard.h"
#include "json/messagecenter.h"

using namespace DashboardQuery;

ViewDashboard::ViewDashboard(QObject *receiver) :
    DashboardExecutor(COMMAND, { }, receiver)
{

}

QueryResult ViewDashboard::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    QVariantMap outcome { { "record_count", 0 }, { "records", QVariantList {} } };
    result.setOutcome(outcome);

    fetchWelcomeMessage(result);
    fetchEmptyStockMessage(result);

    outcome.insert("record_count", result.outcome().toMap().value("records").toList().count());
    outcome.insert("records", result.outcome().toMap().value("records").toList());
    result.setOutcome(outcome);

    return result;
}

void ViewDashboard::fetchWelcomeMessage(QueryResult &result)
{
    MessageCenter center;
    QVariantList records(result.outcome().toMap().value("records").toList());
    records.append(center.getMessage("welcome"));
    result.setOutcome(QVariantMap { { "record_count", 0 }, { "records", records } });
}

void ViewDashboard::fetchEmptyStockMessage(QueryResult &result)
{
    MessageCenter center;
    QVariantList records(result.outcome().toMap().value("records").toList());
    records.append(center.getMessage("empty_stock"));
    result.setOutcome(QVariantMap { { "record_count", 0 }, { "records", records } });
}
