#include "expensesqlmanager.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

#include "database/databaseexception.h"
#include "database/databaseutils.h"
#include "user/userprofile.h"

ExpenseSqlManager::ExpenseSqlManager(const QString &connectionName) :
    AbstractSqlManager (connectionName)
{

}

QueryResult ExpenseSqlManager::execute(const QueryRequest &request)
{
    QueryResult result(request);

    try {
        if (request.command() == "add_new_expense_transaction")
            addNewExpenseTransaction(request);
        else if (request.command() == "view_expense_transactions")
            viewExpenseTransactions(request, result);
        else if (request.command() == "view_expense_report")
            viewExpenseReport(request, result);
        else
            throw DatabaseException(DatabaseError::QueryErrorCode::CommandNotFound,
                                    QString("Command not found: %1").arg(request.command()));

        result.setSuccessful(true);
    } catch (DatabaseException &e) {
        result.setSuccessful(false);
        result.setErrorCode(e.code());
        result.setErrorMessage(e.message());
        result.setErrorUserMessage(e.userMessage());
    }

    return result;
}

void ExpenseSqlManager::addNewExpenseTransaction(const QueryRequest &request)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();
    int noteId = 0;

    QSqlQuery q(connection);

    try {
        DatabaseUtils::beginTransaction(q);

        if (params.contains("note")) {
            // STEP: Insert note
            noteId = addNote(params.value("note").toString(), "expense");
        }

        // STEP: Insert expense transaction
        callProcedure("AddExpenseTransaction", {
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

        DatabaseUtils::commitTransaction(q);
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}

void ExpenseSqlManager::viewExpenseTransactions(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();

    QSqlQuery q(connection);

    try {
        const QList<QSqlRecord> &records (callProcedure("ViewExpenseTransactions", {
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

    } catch (DatabaseException &) {
        throw;
    }
}

void ExpenseSqlManager::viewExpenseReport(const QueryRequest &request, QueryResult &result)
{
    const QVariantMap &params = request.params();

    try {
        const QList<QSqlRecord> &records(callProcedure("ViewExpenseReport", {
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
                                                               "filter_column",
                                                               params.value("filter_column")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "filter_text",
                                                               params.value("filter_text")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "sort_column",
                                                               params.value("sort_column", QStringLiteral("purpose"))
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "sort_order",
                                                               params.value("sort_order").toInt() == Qt::DescendingOrder
                                                               ? "descending" : "ascending"
                                                           }
                                                       }));

        QVariantList transactions;
        for (const QSqlRecord &record : records) {
            transactions.append(recordToMap(record));
        }

        result.setOutcome(QVariantMap {
                              { "transactions", transactions },
                              { "record_count", transactions.count() },
                          });
    } catch (DatabaseException &) {
        throw;
    }
}
