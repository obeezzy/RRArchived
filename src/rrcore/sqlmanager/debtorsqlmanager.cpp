#include "debtorsqlmanager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

#include "singletons/userprofile.h"

DebtorSqlManager::DebtorSqlManager(const QString &connectionName) :
    AbstractSqlManager(connectionName)
{

}

QueryResult DebtorSqlManager::execute(const QueryRequest &request)
{
    QueryResult result(request);

    try {
        if (request.command() == "add_new_debtor")
            addNewDebtor(request, result);
        else if (request.command() == "undo_add_new_debtor")
            undoAddNewDebtor(request, result);
        else if (request.command() == "update_debtor")
            updateDebtor(request, result);
        else if (request.command() == "view_debtors")
            viewDebtors(request, result);
        else if (request.command() == "remove_debtor")
            removeDebtor(request, result);
        else if (request.command() == "undo_remove_debtor")
            undoRemoveDebtor(request, result);
        else if (request.command() == "view_debt_transactions")
            viewDebtTransactions(request, result);
        else if (request.command() == "view_debtor_details")
            viewDebtorDetails(request, result);
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

void DebtorSqlManager::addNewDebtor(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();
    const QDateTime currentDateTime = QDateTime::currentDateTime();
    const QVariantList newDebtTransactions = params.value("new_debt_transactions").toList();
    int noteId = 0;
    int debtorId = 0;
    int clientId = 0;
    QVariantList debtTransactionIds;
    QVector<int> debtTransactionNoteIds;
    QSqlQuery q(connection);

    try {
        AbstractSqlManager::enforceArguments({ "preferred_name", "primary_phone_number" }, params);

        if (!DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::BeginTransactionFailed, q.lastError().text(), "Failed to start transation.");

        // STEP: Check if debt transactions exist
        if (newDebtTransactions.count() == 0)
            throw DatabaseException(DatabaseException::RRErrorCode::MissingArguments, q.lastError().text(),
                                    QString("No new debt transactions for debtor %1.").arg(params.value("preferred_name").toString()));

        // STEP: Ensure that debtor doesn't already exist
        if (!params.value("primary_phone_number").toString().isEmpty()) {
            q.prepare("SELECT phone_number FROM client "
                      "INNER JOIN debtor ON client.id = debtor.client_id "
                      "WHERE phone_number = :phone_number");
            q.bindValue(":phone_number", params.value("primary_phone_number"), QSql::Out);

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::AddDebtorFailure, q.lastError().text(), "Failed to check if client exists.");

            if (q.first())
                throw DatabaseException(DatabaseException::RRErrorCode::DuplicateEntryFailure, q.lastError().text(),
                                        "Failed to insert debtor because debtor already exists.");
        }

        auto addNote = [&q, &currentDateTime](const QString &note) {
            if (note.trimmed().isEmpty())
                return 0;

            // STEP: Add note
            q.prepare("INSERT INTO note (note, table_name, created, last_edited, user_id) VALUES ("
                      ":note, :table_name, :created, :last_edited, :user_id)");
            q.bindValue(":note", note.isNull() ? QVariant(QVariant::String) : note);
            q.bindValue(":table_name", "debtor");
            q.bindValue(":created", currentDateTime);
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::AddDebtorFailure, q.lastError().text(), "Failed to insert into note table.");

            return q.lastInsertId().toInt();
        };

        noteId = addNote(params.value("note").toString());

        // STEP: Add notes for each debt transaction
        for (int i = 0; i < newDebtTransactions.count(); ++i) {
            const QString note = newDebtTransactions.at(i).toMap().value("note").toString();
            debtTransactionNoteIds.append(addNote(note));
        }

        if (newDebtTransactions.count() != debtTransactionNoteIds.count())
            throw DatabaseException(DatabaseException::RRErrorCode::AddDebtorFailure, "",
                                    "Failed to match note ID count with transaction count.");

        // STEP: Add client
        q.prepare("INSERT INTO client (first_name, last_name, preferred_name, phone_number, address, note_id, archived, created, last_edited, "
                  "user_id) VALUES (:first_name, :last_name, :preferred_name, :phone_number, :address, :note_id, :archived, :created, "
                  ":last_edited, :user_id)");
        q.bindValue(":first_name", params.value("first_name"));
        q.bindValue(":last_name", params.value("last_name"));
        q.bindValue(":preferred_name", params.value("preferred_name"));
        q.bindValue(":phone_number", params.value("primary_phone_number"));
        q.bindValue(":address", params.value("address"));
        q.bindValue(":note_id", noteId == 0 ? QVariant(QVariant::Int) : noteId);
        q.bindValue(":archived", false);
        q.bindValue(":created", currentDateTime);
        q.bindValue(":last_edited", currentDateTime);
        q.bindValue(":user_id", UserProfile::instance().userId());

        if (!q.exec()) {
            if (q.lastError().number() == int(DatabaseException::MySqlErrorCode::DuplicateEntryError))
                throw DatabaseException(DatabaseException::RRErrorCode::DuplicateEntryFailure, q.lastError().text(),
                                        "Failed to insert client because client already exists.");
            else
                throw DatabaseException(DatabaseException::RRErrorCode::AddDebtorFailure, q.lastError().text(),
                                        "Failed to insert into client table.");
        }

        clientId = q.lastInsertId().toInt();

        // STEP: Add debtor
        q.prepare("INSERT INTO debtor (client_id, note_id, archived, created, last_edited, user_id) VALUES ("
                  ":client_id, :note_id, :archived, :created, :last_edited, :user_id)");
        q.bindValue(":client_id", clientId);
        q.bindValue(":note_id", noteId == 0 ? QVariant(QVariant::Int) : noteId);
        q.bindValue(":archived", false);
        q.bindValue(":created", currentDateTime);
        q.bindValue(":last_edited", currentDateTime);
        q.bindValue(":user_id", UserProfile::instance().userId());

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::AddDebtorFailure, q.lastError().text(), "Failed to insert into debtor table.");

        debtorId = q.lastInsertId().toInt();

        // STEP: Add debt transactions
        for (int i = 0; i < newDebtTransactions.count(); ++i) {
            q.prepare("INSERT INTO debt_transaction (debtor_id, transaction_table, transaction_id, note_id, archived, created, last_edited, "
                      "user_id) VALUES (:debtor_id, :transaction_table, :transaction_id, :note_id, :archived, "
                      ":created, :last_edited, :user_id)");
            q.bindValue(":debtor_id", debtorId);
            q.bindValue(":transaction_table", "debtor");
            q.bindValue(":transaction_id", 0);
            q.bindValue(":note_id", debtTransactionNoteIds.at(i) == 0 ? QVariant(QVariant::Int) : debtTransactionNoteIds.at(i));
            q.bindValue(":archived", false);
            q.bindValue(":created", currentDateTime);
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::AddDebtorFailure, q.lastError().text(),
                                        QStringLiteral("Failed to insert into debt transaction table."));

            const int debtTransactionId = q.lastInsertId().toInt();
            const double totalDebt = newDebtTransactions.at(i).toMap().value("total_debt").toDouble();
            const QVariantList &newDebtPayments = newDebtTransactions.at(i).toMap().value("new_debt_payments").toList();
            const QDateTime &dueDateTime = newDebtTransactions.at(i).toMap().value("due_date").toDateTime();
            double newDebt = totalDebt;

            if (dueDateTime <= QDateTime::currentDateTime())
                throw DatabaseException(DatabaseException::RRErrorCode::InvalidDueDate, q.lastError().text(),
                                        QStringLiteral("Due date is earlier than the current date."));

            debtTransactionIds.append(debtTransactionId);

            for (const QVariant &debtPayment : newDebtPayments) {
                const double amountPaid = debtPayment.toMap().value("amount").toDouble();
                const QString note = debtPayment.toMap().value("note").toString();

                const int noteId = !note.isEmpty() ? addNote(note) : 0;

                // Add debt payment
                q.prepare("INSERT INTO debt_payment (debt_transaction_id, total_amount, amount_paid, balance, currency, "
                          "due_date, note_id, archived, created, last_edited, user_id) VALUES (:debt_transaction_id, "
                          ":total_amount, :amount_paid, :balance, :currency, :due_date, :note_id, :archived, "
                          ":created, :last_edited, :user_id)");
                q.bindValue(":debt_transaction_id", debtTransactionId);
                q.bindValue(":total_amount", newDebt);
                q.bindValue(":amount_paid", amountPaid);
                q.bindValue(":balance", qAbs(newDebt - amountPaid));
                q.bindValue(":currency", "NGN");
                q.bindValue(":due_date", dueDateTime);
                q.bindValue(":note_id", noteId == 0 ? QVariant(QVariant::Int) : noteId);
                q.bindValue(":archived", false);
                q.bindValue(":created", currentDateTime);
                q.bindValue(":last_edited", currentDateTime);
                q.bindValue(":user_id", UserProfile::instance().userId());

                if (!q.exec())
                    throw DatabaseException(DatabaseException::RRErrorCode::AddDebtorFailure, q.lastError().text(),
                                            "Failed to insert into debt payment table.");

                newDebt -= amountPaid;

                if (newDebt < 0.0)
                    throw DatabaseException(DatabaseException::RRErrorCode::AmountOverpaid, q.lastError().text(),
                                            QStringLiteral("Total amount paid is greater than total debt."));
            }
        }

        QVariantMap outcome;
        outcome.insert("client_id", clientId);
        outcome.insert("debtor_id", debtorId);
        outcome.insert("debt_transaction_ids", debtTransactionIds);
        result.setOutcome(outcome);

        if (!DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::CommitTransationFailed, q.lastError().text(), "Failed to commit.");
    } catch (DatabaseException &) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        throw;
    }
}

void DebtorSqlManager::undoAddNewDebtor(const QueryRequest &request, QueryResult &result)
{
    Q_UNUSED(result)
    QSqlDatabase connection = QSqlDatabase::database(connectionName());

    const QVariantMap &params = request.params();
    const QVariantList &debtTransactionIds = params.value("outcome").toMap().value("debt_transaction_ids").toList();
    const QDateTime currentDateTime = QDateTime::currentDateTime();

    QSqlQuery q(connection);

    try {
        AbstractSqlManager::enforceArguments({ "client_id", "debtor_id", "debt_transaction_ids" }, params.value("outcome").toMap());

        if (!DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::BeginTransactionFailed, q.lastError().text(), "Failed to start transation.");

        // Remove debtor
        q.prepare("UPDATE debtor SET archived = 1, last_edited = :last_edited, user_id = :user_id WHERE id = :debtor_id");
        q.bindValue(":last_edited", currentDateTime);
        q.bindValue(":user_id", UserProfile::instance().userId());
        q.bindValue(":debtor_id", params.value("outcome").toMap().value("debtor_id"));

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::UndoAddDebtorFailure, q.lastError().text(),
                                    "Failed to archive debtor.");

        // Remove debt transaction
        for (const QVariant &debtTransactionId : debtTransactionIds) {
            q.prepare("UPDATE debt_transaction SET archived = 1, last_edited = :last_edited, user_id = :user_id "
                      "WHERE id = :debt_transaction_id");
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());
            q.bindValue(":debt_transaction_id", debtTransactionId);

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::UndoAddDebtorFailure, q.lastError().text(),
                                        "Failed to archive debt transaction.");
        }

        // Remove debt payment for debt transaction ID
        for (const QVariant &debtTransactionId : debtTransactionIds) {
            q.prepare("UPDATE debt_payment SET archived = 1, last_edited = :last_edited, user_id = :user_id "
                      "WHERE debt_transaction_id = :debt_transaction_id");
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());
            q.bindValue(":debt_transaction_id", debtTransactionId);

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::UndoAddDebtorFailure, q.lastError().text(),
                                        "Failed to archive debt payments.");
        }

        if (!DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::CommitTransationFailed, q.lastError().text(), "Failed to commit.");
    } catch (DatabaseException &) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        throw;
    }
}

void DebtorSqlManager::updateDebtor(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();
    const QVariantList &updatedDebtTransactions = params.value("updated_debt_transactions").toList();
    const QVariantList &newDebtTransactions = params.value("new_debt_transactions").toList();
    const QVariantList &removedDebtTransactionIds = params.value("removed_debt_transaction_ids").toList();
    const QVariantList &removedDebtPaymentIds = params.value("removed_debt_payment_ids").toList();
    int noteId = params.value("note_id").toInt();
    const QString &note = params.value("note").toString();
    int debtorId = params.value("debtor_id").toInt();
    int clientId = params.value("client_id").toInt();
    QVariantList updatedDebtTransactionIds;
    QVariantList newDebtTransactionIds;
    QVector<int> newDebtTransactionNoteIds;
    QSqlQuery q(connection);

    try {
        AbstractSqlManager::enforceArguments({ "client_id", "debtor_id", "preferred_name", "primary_phone_number" }, params);

        if (!DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::BeginTransactionFailed, q.lastError().text(),
                                    QStringLiteral("Failed to start transation."));

        // STEP: Check if debt transactions exist.
        if (updatedDebtTransactions.isEmpty() && newDebtTransactions.isEmpty())
            throw DatabaseException(DatabaseException::RRErrorCode::MissingArguments, q.lastError().text(),
                                    QString("No new/updated debt transactions for debtor %1.").arg(params.value("preferred_name").toString()));

        auto addNote = [&q](const QString &note) {
            if (note.trimmed().isEmpty())
                return 0;

            // STEP: Add note.
            q.prepare("INSERT INTO note (note, table_name, created, last_edited, user_id) VALUES ("
                      ":note, :table_name, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), :user_id)");
            q.bindValue(":note", note.isNull() ? QVariant(QVariant::String) : note);
            q.bindValue(":table_name", "debtor");
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::UpdateDebtorFailure, q.lastError().text(),
                                        QStringLiteral("Failed to insert into note table."));

            return q.lastInsertId().toInt();
        };

        auto updateNote = [&q](int noteId, const QString &note) {
            if (noteId <= 0 || note.trimmed().isEmpty())
                return;

            // STEP: Update note.
            q.prepare("UPDATE note SET note = :note, last_edited = CURRENT_TIMESTAMP(), "
                      "user_id = :user_id WHERE id = :note_id");
            q.bindValue(":note", note.isNull() ? QVariant(QVariant::String) : note);
            q.bindValue(":user_id", UserProfile::instance().userId());
            q.bindValue(":note_id", noteId);

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::UpdateDebtorFailure, q.lastError().text(),
                                        QStringLiteral("Failed to update note table."));
        };

        // STEP: Update notes for debtor.
        updateNote(noteId, note);

        // STEP: Add notes for updated debt transaction.
        for (int i = 0; i < updatedDebtTransactions.count(); ++i) {
            const int noteId = updatedDebtTransactions.at(i).toMap().value("note_id").toInt();
            const QString &note = updatedDebtTransactions.at(i).toMap().value("note").toString();
            updateNote(noteId, note);
        }

        // STEP: Add notes for new debt transaction.
        for (int i = 0; i < newDebtTransactions.count(); ++i) {
            const QString &note = newDebtTransactions.at(i).toMap().value("note").toString();
            newDebtTransactionNoteIds.append(addNote(note));
        }

        if (newDebtTransactions.count() != newDebtTransactionNoteIds.count())
            throw DatabaseException(DatabaseException::RRErrorCode::UpdateDebtorFailure, "",
                                    QStringLiteral("Failed to match note ID count with transaction count."));

        // STEP: Update client details.
        q.prepare("UPDATE client SET first_name = :first_name, last_name = :last_name, "
                  "preferred_name = :preferred_name, phone_number = :primary_phone_number, "
                  "last_edited = CURRENT_TIMESTAMP(), user_id = :user_id "
                  "WHERE id = :client_id");
        q.bindValue(":first_name", params.value("first_name"));
        q.bindValue(":last_name", params.value("last_name"));
        q.bindValue(":preferred_name", params.value("preferred_name"));
        q.bindValue(":primary_phone_number", params.value("primary_phone_number"));
        q.bindValue(":user_id", UserProfile::instance().userId());
        q.bindValue(":client_id", params.value("client_id"));

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::UpdateDebtorFailure, q.lastError().text(),
                                        QStringLiteral("Failed to update client table."));

        // STEP: Update debt transactions.
        for (int i = 0; i < updatedDebtTransactions.count(); ++i) {
            const int debtTransactionId = updatedDebtTransactions.at(i).toMap().value("debt_transaction_id").toInt();
            const double totalDebt = updatedDebtTransactions.at(i).toMap().value("total_debt").toDouble();
            const QDateTime &dueDateTime = updatedDebtTransactions.at(i).toMap().value("due_date").toDateTime();
            double newDebt = totalDebt;

            if (debtTransactionId <= 0)
                throw DatabaseException(DatabaseException::RRErrorCode::InvalidArguments, q.lastError().text(),
                                        QStringLiteral("Debt transaction ID used to update table is invalid."));

            if (dueDateTime.isNull() || !dueDateTime.isValid() || dueDateTime <= QDateTime::currentDateTime())
                throw DatabaseException(DatabaseException::RRErrorCode::InvalidDueDate, q.lastError().text(),
                                        QStringLiteral("Due date is earlier than the current date or invalid."));

            q.prepare("UPDATE debt_transaction SET last_edited = CURRENT_TIMESTAMP(), user_id = :user_id "
                      "WHERE id = :debt_transaction_id");
            q.bindValue(":user_id", UserProfile::instance().userId());
            q.bindValue(":debt_transaction_id", debtTransactionId);

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::UpdateDebtorFailure, q.lastError().text(),
                                        QStringLiteral("Failed to insert into debt transaction table."));

            // STEP: Update debt payments.
            const QVariantList &updatedDebtPayments(updatedDebtTransactions.at(i).toMap().value("updated_debt_payments").toList());
            for (int j = 0; j < updatedDebtPayments.count(); ++j) {
                const int debtPaymentId = updatedDebtPayments.at(j).toMap().value("debt_payment_id").toInt();
                const double amountPaid = updatedDebtPayments.at(j).toMap().value("amount_paid").toDouble();

                if (debtPaymentId <= 0)
                    throw DatabaseException(DatabaseException::RRErrorCode::InvalidArguments, q.lastError().text(),
                                            QStringLiteral("Debt payment ID used to update table is invalid."));

                q.prepare("UPDATE debt_payment SET total_amount = :total_amount, amount_paid = :amount_paid, "
                          "balance = :balance, due_date = :due_date, last_edited = CURRENT_TIMESTAMP(), user_id = :user_id "
                          "WHERE id = :debt_payment_id");
                q.bindValue(":debt_payment_id", debtPaymentId);
                q.bindValue(":total_amount", newDebt);
                q.bindValue(":amount_paid", amountPaid);
                q.bindValue(":balance", qAbs(newDebt - amountPaid));
                q.bindValue(":currency", "NGN");
                q.bindValue(":due_date", dueDateTime);
                q.bindValue(":user_id", UserProfile::instance().userId());

                if (!q.exec())
                    throw DatabaseException(DatabaseException::RRErrorCode::UpdateDebtorFailure, q.lastError().text(),
                                            QStringLiteral("Failed to update debt payment table."));

                newDebt -= amountPaid;

                if (newDebt < 0.0)
                    throw DatabaseException(DatabaseException::RRErrorCode::AmountOverpaid, q.lastError().text(),
                                            QStringLiteral("Total amount paid is greater than total debt."));
            }

            // STEP: Add new debt payments.
            const QVariantList &newDebtPayments(updatedDebtTransactions.at(i).toMap().value("new_debt_payments").toList());
            for (int j = 0; j < newDebtPayments.count(); ++j) {
                const double updatedDebt = newDebtPayments.at(j).toMap().value("total_amount").toDouble();
                const double amountPaid = newDebtPayments.at(j).toMap().value("amount_paid").toDouble();

                q.prepare("INSERT INTO debt_payment (debt_transaction_id, total_amount, amount_paid, balance, currency, "
                          "due_date, note_id, archived, created, last_edited, user_id) VALUES (:debt_transaction_id, "
                          ":total_amount, :amount_paid, :balance, :currency, :due_date, :note_id, :archived, "
                          "CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), :user_id)");
                q.bindValue(":debt_transaction_id", debtTransactionId);
                q.bindValue(":total_amount", updatedDebt);
                q.bindValue(":amount_paid", amountPaid);
                q.bindValue(":balance", qAbs(updatedDebt - amountPaid));
                q.bindValue(":currency", "NGN");
                q.bindValue(":due_date", dueDateTime);
                q.bindValue(":note_id", noteId == 0 ? QVariant(QVariant::Int) : noteId);
                q.bindValue(":archived", false);
                q.bindValue(":user_id", UserProfile::instance().userId());

                if (!q.exec())
                    throw DatabaseException(DatabaseException::RRErrorCode::AddDebtorFailure, q.lastError().text(),
                                            QStringLiteral("Failed to insert into debt payment table."));
            }
        }

        // STEP: Add debt transactions
        for (int i = 0; i < newDebtTransactions.count(); ++i) {
            q.prepare("INSERT INTO debt_transaction (debtor_id, transaction_table, transaction_id, note_id, "
                      "archived, created, last_edited, user_id) VALUES (:debtor_id, :transaction_table, "
                      ":transaction_id, :note_id, :archived, "
                      "CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), :user_id)");
            q.bindValue(":debtor_id", debtorId);
            q.bindValue(":transaction_table", "debtor");
            q.bindValue(":transaction_id", 0);
            q.bindValue(":note_id", newDebtTransactionNoteIds.at(i) == 0 ? QVariant(QVariant::Int) : newDebtTransactionNoteIds.at(i));
            q.bindValue(":archived", false);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::UpdateDebtorFailure, q.lastError().text(),
                                        QStringLiteral("Failed to insert into debt transaction table."));

            const int debtTransactionId = q.lastInsertId().toInt();
            const double totalDebt = newDebtTransactions.at(i).toMap().value("total_debt").toDouble();
            const QVariantList newDebtPayments = newDebtTransactions.at(i).toMap().value("new_debt_payments").toList();
            const QDateTime dueDateTime = newDebtTransactions.at(i).toMap().value("due_date").toDateTime();
            double newDebt = totalDebt;

            if (dueDateTime.isNull() || !dueDateTime.isValid() || dueDateTime <= QDateTime::currentDateTime())
                throw DatabaseException(DatabaseException::RRErrorCode::InvalidDueDate, q.lastError().text(),
                                        QStringLiteral("Due date is earlier than the current date or invalid."));

            newDebtTransactionIds.append(debtTransactionId);

            for (const QVariant &debtPayment : newDebtPayments) {
                const double amountPaid = debtPayment.toMap().value("amount").toDouble();
                const QString &note = debtPayment.toMap().value("note").toString();

                const int noteId = !note.isEmpty() ? addNote(note) : 0;

                // Add debt payment
                q.prepare("INSERT INTO debt_payment (debt_transaction_id, total_amount, amount_paid, balance, currency, "
                          "due_date, note_id, archived, created, last_edited, user_id) VALUES (:debt_transaction_id, "
                          ":total_amount, :amount_paid, :balance, :currency, :due_date, :note_id, :archived, "
                          "CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), :user_id)");
                q.bindValue(":debt_transaction_id", debtTransactionId);
                q.bindValue(":total_amount", newDebt);
                q.bindValue(":amount_paid", amountPaid);
                q.bindValue(":balance", qAbs(newDebt - amountPaid));
                q.bindValue(":currency", "NGN");
                q.bindValue(":due_date", dueDateTime);
                q.bindValue(":note_id", noteId == 0 ? QVariant(QVariant::Int) : noteId);
                q.bindValue(":archived", false);
                q.bindValue(":user_id", UserProfile::instance().userId());

                if (!q.exec())
                    throw DatabaseException(DatabaseException::RRErrorCode::UpdateDebtorFailure, q.lastError().text(),
                                            QStringLiteral("Failed to insert into debt payment table."));

                newDebt -= amountPaid;

                if (newDebt < 0.0)
                    throw DatabaseException(DatabaseException::RRErrorCode::AmountOverpaid, q.lastError().text(),
                                            QStringLiteral("Total amount paid is greater than total debt."));
            }
        }

        // STEP: Remove debt transactions.
        for (int i = 0; i < removedDebtTransactionIds.count(); ++i) {
            q.prepare("UPDATE debt_transaction SET archived = 1, CURRENT_TIMESTAMP(), user_id = :user_id "
                      "WHERE id = :debt_transaction_id");
            q.bindValue(":user_id", UserProfile::instance().userId());
            q.bindValue(":debt_transaction_id", removedDebtTransactionIds.at(i));

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::UpdateDebtorFailure, q.lastError().text(),
                                        QStringLiteral("Failed to remove debt transaction table."));
        }

        // STEP: Remove debt payments.
        for (int i = 0; i < removedDebtPaymentIds.count(); ++i) {
            q.prepare("UPDATE debt_payment SET archived = 1, CURRENT_TIMESTAMP(), user_id = :user_id "
                      "WHERE id = :debt_payment_id");
            q.bindValue(":user_id", UserProfile::instance().userId());
            q.bindValue(":debt_payment_id", removedDebtTransactionIds.at(i));

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::UpdateDebtorFailure, q.lastError().text(),
                                        QStringLiteral("Failed to remove debt payment table."));
        }

        QVariantMap outcome;
        outcome.insert("client_id", clientId);
        outcome.insert("debtor_id", debtorId);
        outcome.insert("updated_debt_transaction_ids", updatedDebtTransactionIds);
        outcome.insert("new_debt_transaction_ids", newDebtTransactionIds);
        result.setOutcome(outcome);

        if (!DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::CommitTransationFailed, q.lastError().text(),
                                    QStringLiteral("Failed to commit."));
    } catch (DatabaseException &) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        throw;
    }
}

void DebtorSqlManager::viewDebtors(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();
    QSqlQuery q(connection);

    try {
        // STEP: Get total balance for each debtor.
        if (params.value("filter_text").isNull() || params.value("filter_column").isNull()) {
            q.prepare("SELECT debtor.client_id, debtor.id AS debtor_id, client.preferred_name AS preferred_name, "
                      "debt_payment.balance AS total_debt, debtor.archived, "
                      "MAX(debt_payment.last_edited) FROM debt_payment "
                      "INNER JOIN debt_transaction ON debt_transaction.id = debt_payment.debt_transaction_id "
                      "INNER JOIN debtor ON debtor.id = debt_transaction.debtor_id "
                      "INNER JOIN client ON client.id = debtor.client_id "
                      "WHERE debt_transaction.archived = :archived "
                      "GROUP BY debtor.id, debt_payment.balance, "
                      "debt_payment.last_edited, debt_transaction.archived "
                      "HAVING MAX(debt_payment.last_edited) = debt_payment.last_edited "
                      "AND debt_transaction.archived = :archived");
            q.bindValue(":archived", params.value("archived", false), QSql::Out);

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::ViewDebtorsFailure,
                                        q.lastError().text(),
                                        QStringLiteral("Failed to fetch debtors."));
        } else {
            // STEP: Filter total balance for each debtor
            if (params.value("filter_column").toString() == "preferred_name") {
                q.prepare(QString("SELECT debtor.client_id, debtor.id AS debtor_id, client.preferred_name AS preferred_name, "
                                  "debt_payment.balance AS total_debt, debtor.archived FROM debt_payment "
                                  "INNER JOIN debt_transaction ON debt_transaction.id = debt_payment.debt_transaction_id "
                                  "INNER JOIN debtor ON debtor.id = debt_transaction.debtor_id "
                                  "INNER JOIN client ON client.id = debtor.client_id "
                                  "WHERE debt_transaction.archived = :archived AND client.preferred_name LIKE '%%1%'"
                                  "GROUP BY debtor.id, debt_payment.debt_transaction_id, debt_payment.balance, "
                                  "debt_payment.last_edited "
                                  "HAVING MAX(debt_payment.last_edited) = debt_payment.last_edited "
                                  "AND archived = :archived")
                          .arg(params.value("filter_text").toString()));
                q.bindValue(":archived", params.value("archived", false), QSql::Out);

                if (!q.exec())
                    throw DatabaseException(DatabaseException::RRErrorCode::ViewDebtorsFailure,
                                            q.lastError().text(),
                                            QStringLiteral("Failed to fetch debtors."));
            }
        }

        QVariantList debtors;
        while (q.next()) {
            qDebug() << "Debtor?" << q.value("debtor_id").toInt()
                     << q.value("preferred_name").toString()
                     << q.value("total_debt").toDouble()
                     << q.value("archived").toBool();
            debtors.append(recordToMap(q.record()));
        }

        result.setOutcome(QVariantMap { { "debtors", debtors }, { "record_count", debtors.count() } });
    } catch (DatabaseException &) {
        throw;
    }
}

void DebtorSqlManager::removeDebtor(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();

    QSqlQuery q(connection);

    try {
        AbstractSqlManager::enforceArguments({ "debtor_id" }, params);

        if (params.value("debtor_id").toInt() <= 0)
            throw DatabaseException(DatabaseException::RRErrorCode::InvalidArguments, QString(),
                                    QStringLiteral("Debtor ID is invalid."));
        if (!DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::BeginTransactionFailed, q.lastError().text(),
                                    QStringLiteral("Failed to start transation."));

        q.prepare("UPDATE debtor SET archived = 1, last_edited = CURRENT_TIMESTAMP(), "
                  "user_id = :user_id WHERE id = :debtor_id");
        q.bindValue(":debtor_id", params.value("debtor_id"));
        q.bindValue(":user_id", UserProfile::instance().userId());

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::RemoveStockItemFailed, q.lastError().text(),
                                    QStringLiteral("Failed to remove debtor."));

        // Archive debt transactions.
        q.prepare("UPDATE debt_transaction SET archived = 1, last_edited = CURRENT_TIMESTAMP(), "
                  "user_id = :user_id WHERE id = :debtor_id");
        q.bindValue(":debtor_id", params.value("debtor_id"));
        q.bindValue(":user_id", UserProfile::instance().userId());

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::RemoveStockItemFailed, q.lastError().text(),
                                    QStringLiteral("Failed to remove debt transactions for debtor."));

        // Retrieve archived debt transaction IDs.
        q.prepare("SELECT id FROM debt_transaction WHERE debtor_id = :debtor_id");
        q.bindValue(":debtor_id", params.value("debtor_id"), QSql::Out);

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::RemoveStockItemFailed, q.lastError().text(),
                                    QStringLiteral("Failed to fetch debt transactions for debtor."));

        QVariantList debtTransactionIds;
        while (q.next()) {
            debtTransactionIds.append(q.value("id").toInt());
        }

        result.setOutcome(QVariantMap{ { "debtor_id", params.value("debtor_id") },
                                       { "debt_transaction_ids", debtTransactionIds },
                                       { "record_count", QVariant(1) },
                                       { "debtor_row", params.value("debtor_row") }
                          });

        if (!DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::CommitTransationFailed, q.lastError().text(), "Failed to commit.");
    } catch (DatabaseException &) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        throw;
    }
}

void DebtorSqlManager::undoRemoveDebtor(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();
    const QVariantList &debtTransactionIds = params.value("outcome").toMap().value("debt_transaction_ids").toList();

    QSqlQuery q(connection);

    try {
        AbstractSqlManager::enforceArguments({ "debtor_id" }, params);

        if (params.value("debtor_id").toInt() <= 0)
            throw DatabaseException(DatabaseException::RRErrorCode::InvalidArguments, QString(), "Debtor ID is invalid.");

        if (!DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::BeginTransactionFailed, q.lastError().text(), "Failed to start transation.");

        q.prepare("UPDATE debtor SET archived = 0, last_edited = CURRENT_TIMESTAMP(), "
                  "user_id = :user_id WHERE id = :debtor_id");
        q.bindValue(":debtor_id", params.value("debtor_id"));
        q.bindValue(":user_id", UserProfile::instance().userId());

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::RemoveStockItemFailed, q.lastError().text(),
                                    QStringLiteral("Failed to undo 'remove debtor'."));

        // STEP: Restore archived transactions.
        for (int i = 0; i < debtTransactionIds.count(); ++i) {
            q.prepare("UPDATE debt_transaction SET archived = 0, last_edited = CURRENT_TIMESTAMP(), "
                      "user_id = :user_id WHERE id = :debt_transaction_id");
            q.bindValue(":debt_transaction_id", debtTransactionIds.at(i).toInt());
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::RemoveStockItemFailed, q.lastError().text(),
                                        QStringLiteral("Failed to update debt transactions during undo."));
        }

        // STEP: Get total balance for each debtor.
        q.prepare("SELECT debtor.client_id, debtor.id AS debtor_id, client.preferred_name AS preferred_name, "
                  "(SELECT SUM(debt_payment.balance) FROM debt_payment "
                  "INNER JOIN debt_transaction ON debt_transaction.id = debt_payment.debt_transaction_id "
                  "INNER JOIN debtor ON debt_transaction.debtor_id = debtor.id "
                  "WHERE debt_payment.debt_transaction_id IN "
                  "(SELECT debt_transaction.id FROM debt_transaction "
                  "WHERE debt_transaction.debtor_id = debtor.id AND debt_transaction.archived = 0) "
                  "AND debt_payment.archived = 0 ORDER BY debt_payment.last_edited DESC LIMIT 1) AS total_debt, "
                  "note.note AS note, debtor.created, debtor.last_edited, debtor.user_id, user.user "
                  "FROM debtor "
                  "INNER JOIN client ON client.id = debtor.client_id "
                  "LEFT JOIN user ON user.id = debtor.user_id "
                  "LEFT JOIN note ON debtor.note_id = note.id "
                  "WHERE debtor.archived = 0 AND debtor.id = :debtor_id");
        q.bindValue(":debtor_id", params.value("debtor_id"), QSql::Out);

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::ViewDebtorsFailure, q.lastError().text(),
                                    QStringLiteral("Failed to fetch removed debtor."));

        QVariantMap debtor;
        if (q.first()) {
            debtor = recordToMap(q.record());
            result.setOutcome(QVariantMap { { "debtor", QVariant(debtor) },
                                            { "record_count", 1 },
                                            { "debtor_id", params.value("debtor_id") },
                                            { "debtor_row", params.value("debtor_row") }
                              });
        } else {
            throw DatabaseException(DatabaseException::RRErrorCode::UndoRemoveDebtorFailure,
                                    q.lastError().text(),
                                    QStringLiteral("Unable to fetch remove debtor."));
        }

        if (!DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::CommitTransationFailed, q.lastError().text(),
                                    QStringLiteral("Failed to commit."));
    } catch (DatabaseException &) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        throw;
    }
}

void DebtorSqlManager::viewDebtTransactions(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();
    int clientId = 0;
    QString preferredName;
    QString primaryPhoneNumber;
    QString debtorNote;
    QSqlQuery q(connection);

    try {
        AbstractSqlManager::enforceArguments({ "debtor_id" }, params);

        q.prepare("SELECT client.id AS client_id, client.preferred_name, client.phone_number AS primary_phone_number, "
                  "note.note FROM client "
                  "INNER JOIN debtor ON debtor.client_id = client.id "
                  "LEFT JOIN note ON note.id = debtor.note_id "
                  "WHERE debtor.id = :debtor_id AND debtor.archived = :archived");
        q.bindValue(":debtor_id", params.value("debtor_id"), QSql::Out);
        q.bindValue(":archived", params.value("archived", false), QSql::Out);

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::ViewDebtTransactionsFailure,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to fetch client details for debt transactions."));

        if (q.first()) {
            clientId = q.value("client_id").toInt();
            preferredName = q.value("preferred_name").toString();
            primaryPhoneNumber = q.value("primary_phone_number").toString();
            debtorNote = q.value("note").toString();
        }

        q.prepare("SELECT debt_transaction.id AS debt_transaction_id, debt_transaction.transaction_table AS related_transaction_table, "
                  "debt_transaction.transaction_id AS related_transaction_id, debt_transaction.created AS debt_transaction_created, "
                  "debt_payment.id AS debt_payment_id, debt_payment.total_amount, debt_payment.amount_paid, "
                  "debt_payment.balance AS balance, debt_payment.currency, "
                  "debt_payment.due_date, debt_transaction.note_id AS debt_transaction_note_id, "
                  "debt_payment.note_id AS debt_payment_note_id, debt_transaction.archived, "
                  "debt_payment.created AS debt_payment_created "
                  "FROM debt_payment "
                  "INNER JOIN debt_transaction ON debt_transaction.id = debt_payment.debt_transaction_id "
                  "INNER JOIN debtor ON debtor.id = debt_transaction.debtor_id "
                  "LEFT JOIN note ON note.id = debt_transaction.note_id "
                  "WHERE debtor.id = :debtor_id AND debt_transaction.archived = :archived "
                  "ORDER BY debt_payment.last_edited ASC");
        q.bindValue(":debtor_id", params.value("debtor_id"), QSql::Out);
        q.bindValue(":archived", params.value("archived", false), QSql::Out);

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::ViewDebtTransactionsFailure,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to fetch debt transactions."));

        QVariantList transactions;
        QVariantList paymentGroups;

        while (q.next()) {
            QVariantMap transactionRecord;
            const int debtTransactionId = q.value("debt_transaction_id").toInt();
            const QString &relatedTransactionTable = q.value("related_transaction_table").toString();
            const int relatedTransactionId = q.value("related_transaction_id").toInt();
            const double totalDebt = q.value("balance").toDouble();
            const QDateTime &dueDate = q.value("due_date").toDateTime();
            const QDateTime created = q.value("debt_transaction_created").toDateTime();

            transactionRecord.insert("debt_transaction_id", debtTransactionId);
            transactionRecord.insert("related_transaction_table", relatedTransactionTable);
            transactionRecord.insert("related_transaction_id", relatedTransactionId);
            transactionRecord.insert("total_debt", totalDebt);
            transactionRecord.insert("note_id", q.value("debt_transaction_note_id"));
            transactionRecord.insert("due_date", dueDate);
            transactionRecord.insert("created", created);

            QVariantList payments;

            do {
                QVariantMap paymentRecord;
                paymentRecord.insert("debt_transaction_id", debtTransactionId);
                paymentRecord.insert("debt_payment_id", q.value("debt_payment_id"));
                paymentRecord.insert("total_amount", q.value("total_amount"));
                paymentRecord.insert("amount_paid", q.value("amount_paid"));
                paymentRecord.insert("balance", q.value("balance"));
                paymentRecord.insert("currency", q.value("currency"));
                paymentRecord.insert("due_date", q.value("due_date"));
                paymentRecord.insert("note_id", q.value("debt_payment_note_id"));
                paymentRecord.insert("archived", q.value("archived"));
                paymentRecord.insert("created", q.value("debt_payment_created"));

                payments.append(paymentRecord);
            } while (q.next() && debtTransactionId == q.value("debt_transaction_id").toInt());

            transactions.append(transactionRecord);
            paymentGroups.append(QVariant(payments));
            q.previous();
        }

        if (transactions.count() != paymentGroups.count())
            throw DatabaseException(DatabaseException::RRErrorCode::ResultMismatch,
                                    QString("Transaction count (%1) and payment group count (%2) are unequal.")
                                    .arg(transactions.count()).arg(paymentGroups.count()));

        result.setOutcome(QVariantMap { { "client_id", clientId },
                                        { "debtor_id", params.value("debtor_id") },
                                        { "preferred_name", preferredName },
                                        { "primary_phone_number", primaryPhoneNumber },
                                        { "note", debtorNote },
                                        { "transactions", transactions },
                                        { "payment_groups", paymentGroups },
                                        { "record_count", transactions.count() } });
    } catch (DatabaseException &) {
        throw;
    }
}

void DebtorSqlManager::viewDebtorDetails(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();
    QSqlQuery q(connection);

    try {
        AbstractSqlManager::enforceArguments({ "debtor_id" }, params);

        // STEP: Get total balance for each debtor
        q.prepare("SELECT debtor.id AS debtor_id, client.preferred_name AS preferred_name, "
                  "client.first_name, client.last_name, client.phone_number, debtor.archived, "
                  "debtor.user_id, debtor.user_id AS user "
                  "FROM debtor "
                  "INNER JOIN client ON client.id = debtor.client_id "
                  "WHERE debtor.id = :debtor_id AND debtor.archived = :archived");
        q.bindValue(":debtor_id", params.value("debtor_id", false), QSql::Out);
        q.bindValue(":archived", params.value("archived", false), QSql::Out);

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::ViewDebtorsFailure,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to fetch debtor details."));

        QVariantMap debtorInfo;
        if (q.next()) {
            debtorInfo = recordToMap(q.record());
            result.setOutcome(QVariantMap { { "debtor", debtorInfo }, { "record_count", 1 } });
        }
    } catch (DatabaseException &) {
        throw;
    }
}
