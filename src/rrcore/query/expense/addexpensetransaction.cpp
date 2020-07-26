#include "addexpensetransaction.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "database/databaseexception.h"
#include "user/userprofile.h"
#include "utility/common/note.h"
#include "utility/expense/expensetransaction.h"

using namespace Query::Expense;

AddExpenseTransaction::AddExpenseTransaction(
    const Utility::Expense::ExpenseTransaction& transaction, QObject* receiver)
    : ExpenseExecutor(
          COMMAND,
          {
              {"name", transaction.client.preferredName},
              {"purpose", transaction.purpose},
              {"amount", transaction.amount.toDouble()},
              {"payment_method", transaction.paymentMethod.toString()},
              {"currency", transaction.amount.currency().isoCode()},
          },
          receiver)
{}

QueryResult AddExpenseTransaction::execute()
{
    QueryResult result{request()};
    result.setSuccessful(true);

    const QVariantMap& params = request().params();
    int noteId = 0;
    const QString& note = params.value("note").toString();

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        QueryExecutor::beginTransaction(q);

        noteId = QueryExecutor::addNote(note, QStringLiteral("expense"),
                                        ExceptionPolicy::DisallowExceptions);

        callProcedure(
            "AddExpenseTransaction",
            {ProcedureArgument{ProcedureArgument::Type::In, "client_id",
                               params.value("client_id")},
             ProcedureArgument{ProcedureArgument::Type::In, "name",
                               params.value("name")},
             ProcedureArgument{ProcedureArgument::Type::In, "purpose",
                               params.value("purpose")},
             ProcedureArgument{ProcedureArgument::Type::In, "amount",
                               params.value("amount")},
             ProcedureArgument{ProcedureArgument::Type::In, "payment_method",
                               params.value("payment_method")},
             ProcedureArgument{ProcedureArgument::Type::In, "currency",
                               params.value("currency")},
             ProcedureArgument{ProcedureArgument::Type::In, "note_id", noteId},
             ProcedureArgument{ProcedureArgument::Type::In, "user_id",
                               UserProfile::instance().userId()}});

        QueryExecutor::commitTransaction(q);
        return result;
    } catch (const DatabaseException&) {
        QueryExecutor::rollbackTransaction(q);
        throw;
    }
}
