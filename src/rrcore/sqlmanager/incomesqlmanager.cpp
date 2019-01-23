#include "incomesqlmanager.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

#include "database/databaseexception.h"
#include "database/databaseutils.h"
#include "singletons/userprofile.h"

IncomeSqlManager::IncomeSqlManager(const QString &connectionName) :
    AbstractSqlManager (connectionName)
{

}

QueryResult IncomeSqlManager::execute(const QueryRequest &request)
{
    QueryResult result(request);

    try {
        if (request.command() == "add_new_income_transaction")
            addNewIncomeTransaction(request);
        else if (request.command() == "view_income_transactions")
            viewIncomeTransactions(request, result);
        else
            throw DatabaseException(DatabaseException::RRErrorCode::CommandNotFound, QString("Command not found: %1").arg(request.command()));

        result.setSuccessful(true);
    } catch (DatabaseException &e) {
        result.setSuccessful(false);
        result.setErrorCode(e.code());
        result.setErrorMessage(e.message());
        result.setErrorUserMessage(e.userMessage());
    }

    return result;
}

void IncomeSqlManager::addNewIncomeTransaction(const QueryRequest &request)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();
    int noteId = 0;

    QSqlQuery q(connection);

    try {
        if (!DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::BeginTransactionFailed, q.lastError().text(), "Failed to start transation.");

        if (params.contains("note")) {
            // STEP: Insert note
            noteId = addNote(params.value("note").toString(), "income");
        }

        // STEP: Insert income transaction
        callProcedure("AddIncomeTransaction", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "client_id",
                              params.value("client_id", QVariant::Int)
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "name",
                              params.value("client_name")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "purpose",
                              params.value("purpose")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "amount",
                              params.value("amount")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "payment_method",
                              params.value("payment_method")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "currency",
                              params.value("currency", "NGN")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "note_id",
                              noteId
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              UserProfile::instance().userId()
                          }
                      });

        if (!DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::CommitTransationFailed, q.lastError().text(), "Failed to commit.");
    } catch (DatabaseException &) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        throw;
    }
}

void IncomeSqlManager::viewIncomeTransactions(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();

    QSqlQuery q(connection);

    try {
        if (!DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::BeginTransactionFailed, q.lastError().text(), "Failed to start transation.");

        // STEP: Insert income transaction
        const QList<QSqlRecord> &records (callProcedure("ViewIncomeTransactions", {
                                                            ProcedureArgument {
                                                                ProcedureArgument::Type::In,
                                                                "from",
                                                                params.value("from", QDateTime(QDate(QDate::currentDate().year(), 1, 1), QTime(12, 0)))
                                                            },
                                                            ProcedureArgument {
                                                                ProcedureArgument::Type::In,
                                                                "to",
                                                                params.value("to", QDateTime::currentDateTime())
                                                            },
                                                            ProcedureArgument {
                                                                ProcedureArgument::Type::In,
                                                                "archived",
                                                                params.value("archived", false)
                                                            }
                                                        }));

        QVariantList transactions;
        for (const QSqlRecord &record : records) {
            transactions.append(recordToMap(record));
        }

        result.setOutcome(QVariantMap {
                              { "transactions", transactions },
                              { "record_count", transactions.count() }
                          });

        if (!DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::CommitTransationFailed, q.lastError().text(), "Failed to commit.");
    } catch (DatabaseException &) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        throw;
    }
}
