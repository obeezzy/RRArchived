#include "dashboardsqlmanager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

#include "database/databaseexception.h"
#include "json/messagecenter.h"

DashboardSqlManager::DashboardSqlManager(const QString &connectionName) :
    AbstractSqlManager(connectionName)
{

}

QueryResult DashboardSqlManager::execute(const QueryRequest &request)
{
    QueryResult result(request);

    try {
        if (request.command() == "view_dashboard")
            viewDashboard(request, result);
        else
            throw DatabaseException(DatabaseError::QueryErrorCode::CommandNotFound, QString("Command not found: %1").arg(request.command()));

        result.setSuccessful(true);
    } catch (DatabaseException &e) {
        result.setSuccessful(false);
        result.setErrorCode(e.code());
        result.setErrorMessage(e.message());
        result.setErrorUserMessage(e.userMessage());
    }

    return result;
}

void DashboardSqlManager::viewDashboard(const QueryRequest &request, QueryResult &result)
{
    Q_UNUSED(request)

    QVariantMap outcome { { "record_count", 0 }, { "records", QVariantList {} } };
    result.setOutcome(outcome);

    fetchWelcomeMessage(result);
    fetchEmptyStockMessage(result);

    outcome.insert("record_count", result.outcome().toMap().value("records").toList().count());
    outcome.insert("records", result.outcome().toMap().value("records").toList());
    result.setOutcome(outcome);
}

void DashboardSqlManager::fetchWelcomeMessage(QueryResult &result)
{
    MessageCenter center;
    QVariantList records(result.outcome().toMap().value("records").toList());
    records.append(center.getMessage("welcome"));
    result.setOutcome(QVariantMap { { "record_count", 0 }, { "records", records } });
}

void DashboardSqlManager::fetchEmptyStockMessage(QueryResult &result)
{
    MessageCenter center;
    QVariantList records(result.outcome().toMap().value("records").toList());
    records.append(center.getMessage("empty_stock"));
    result.setOutcome(QVariantMap { { "record_count", 0 }, { "records", records } });
}
