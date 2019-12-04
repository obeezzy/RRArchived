#include "viewuserprivileges.h"
#include "database/databaseexception.h"
#include "json/userprivilegecenter.h"

#include <QJsonDocument>
#include <QJsonObject>

using namespace UserQuery;

ViewUserPrivileges::ViewUserPrivileges(int userId,
                                       QObject *receiver) :
    UserExecutor(COMMAND, {
                    { "user_id", userId }
                 }, receiver)
{

}

QueryResult ViewUserPrivileges::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);
    const QVariantMap &params = request().params();

    try {
        const QList<QSqlRecord> &records(callProcedure("ViewUserPrivileges", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "user_id",
                                                               params.value("user_id")
                                                           }
                                                       }));

        QVariantMap userPrivileges;
        if (!records.isEmpty()) {
            userPrivileges = QJsonDocument::fromJson(recordToMap(records.first())
                                                     .value("user_privileges").toString().toUtf8()).object().toVariantMap();
        } else {
            UserPrivilegeCenter userPrivilegeCenter;
            userPrivileges = userPrivilegeCenter.getPrivileges().toMap();
        }

        result.setOutcome(QVariantMap {
                              { "user_privileges", userPrivileges },
                              { "record_count", userPrivileges.count() }
                          });

        return result;
    } catch (DatabaseException &) {
        throw;
    }
}
