#include "debtorsqlmanager.h"

#include <QSqlQuery>
#include <QSqlError>

DebtorSqlManager::DebtorSqlManager(QSqlDatabase connection) :
    AbstractSqlManager(connection)
{

}

QueryResult DebtorSqlManager::execute(const QueryRequest &request)
{
    QueryResult result(request);

    try {
        if (request.command() == "view_debtors")
            viewDebtors(request, result);
        else
            throw DatabaseException(DatabaseException::CommandNotFound, QString("Command not found: %1").arg(request.command()));

        result.setSuccessful(true);
    } catch (DatabaseException &e) {
        result.setSuccessful(false);
        result.setErrorCode(e.code());
        result.setErrorMessage(e.message());
        result.setErrorUserMessage(e.userMessage());
    }

    return result;
}

void DebtorSqlManager::viewDebtors(const QueryRequest &request, QueryResult &result)
{
    const QVariantMap &params = request.params();
    QSqlQuery q(connection());

    /*
      SELECT debtor.client_id, client.preferred_name AS name,
      (SELECT SUM(debt_payment.balance) FROM debt_payment
      INNER JOIN debt_transaction ON debt_transaction.id = debt_payment.debt_transaction_id
      INNER JOIN debtor ON debt_transaction.client_id = debtor.client_id
      WHERE debt_payment.debt_transaction_id IN
     (SELECT debt_transaction.id FROM debt_transaction WHERE debt_transaction.client_id = debtor.client_id AND debt_transaction.archived = 0)
     AND debt_payment.archived = 0 ORDER BY debt_payment.last_edited DESC LIMIT 1) AS balance,
      note.note AS note, debtor.created, debtor.last_edited, debtor.user_id, user.user
      FROM debtor
      INNER JOIN client ON client.id = debtor.client_id
      LEFT JOIN user on user.id = debtor.user_id
      LEFT JOIN note ON debtor.note_id = note.id
      WHERE debtor.archived = 0;
     */
    try {
        // Get total balance for each debtor
        q.prepare("SELECT debtor.client_id, client.preferred_name AS name, "
                  "(SELECT SUM(debt_payment.balance) FROM debt_payment "
                  "INNER JOIN debt_transaction ON debt_transaction.id = debt_payment.debt_transaction_id "
                  "INNER JOIN debtor ON debt_transaction.client_id = debtor.client_id "
                  "WHERE debt_payment.debt_transaction_id IN "
                  "(SELECT debt_transaction.id FROM debt_transaction WHERE debt_transaction.client_id = debtor.client_id AND debt_transaction.archived = 0) "
                  "AND debt_payment.archived = 0 ORDER BY debt_payment.last_edited DESC LIMIT 1) AS total_balance, "
                  "note.note AS note, debtor.created, debtor.last_edited, debtor.user_id, user.user "
                  "FROM debtor "
                  "INNER JOIN client ON client.id = debtor.client_id "
                  "LEFT JOIN user on user.id = debtor.user_id "
                  "LEFT JOIN note ON debtor.note_id = note.id "
                  "WHERE debtor.archived = :archived");
        q.bindValue(":archived", params.value("archived", false), QSql::Out);

        if (!q.exec())
            throw DatabaseException(DatabaseException::ViewDebtorsFailure, q.lastError().text(), "Failed to fetch debtors.");

        QVariantList debtors;
        while (q.next()) {
            debtors.append(recordToMap(q.record()));
        }

        result.setOutcome(QVariantMap { { "debtors", debtors }, { "recordCount", debtors.count() } });
    } catch (DatabaseException &) {
        throw;
    }
}
