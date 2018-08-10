#include "salesqlmanager.h"
#include <QSqlQuery>
#include <QDateTime>
#include <QSqlError>

SaleSqlManager::SaleSqlManager(QSqlDatabase connection) :
    AbstractSqlManager(connection)
{

}

QueryResult SaleSqlManager::execute(const QueryRequest &request)
{
    QueryResult result(request);

    try {
        if (request.command() == "add_sale_transaction")
            addSaleTransaction(request, result);
        else if (request.command() == "update_suspended_sale_transaction")
            updateSuspendedTransaction(request, result);
        else if (request.command() == "undo_add_sale_transaction")
            undoAddSaleTransaction(request, result);
        else if (request.command() == "view_sale_cart")
            viewSaleCart(request, result);
        else if (request.command() == "view_sale_transactions")
            viewSaleTransactions(request, result);
        else if (request.command() == "view_sale_transaction_items")
            viewSaleTransactionItems(request, result);
        else if (request.command() == "view_sale_home")
            viewSaleHome(request, result);
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

void SaleSqlManager::addSaleTransaction(const QueryRequest &request, QueryResult &result, bool skipSqlTransaction)
{
    const QVariantMap &params = request.params();
    const QVariantList &payments = params.value("payments").toList();
    const QVariantList &items = params.value("items").toList();
    const QDateTime &currentDateTime = QDateTime::currentDateTime();
    int noteId = 0;
    int salePaymentNoteId = 0;
    int clientId = 0;
    int newClientId = 0;
    int saleTransactionId = 0;
    int debtorId = 0;
    int debtTransactionId = 0;
    int creditorId = 0;
    int creditTransactionId = 0;

    QSqlQuery q(connection());

    try {
//        AbstractSqlManager::enforceArguments( { "action" }, params);

        if (!skipSqlTransaction && !DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::BeginTransactionFailed, q.lastError().text(),
                                    QStringLiteral("Failed to start transation."));

        // STEP: Get client ID
        if (!params.value("customer_phone_number").toString().trimmed().isEmpty() && !params.value("suspended").toBool()) {
            q.prepare("SELECT id FROM client WHERE phone_number = :customer_phone_number");
            q.bindValue(":customer_phone_number", params.value("customer_phone_number"), QSql::Out);

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::AddTransactionFailure, q.lastError().text(),
                                        QStringLiteral("Failed to get client ID."));

            if (q.first())
                clientId = q.value("id").toInt();
            else {
                q.prepare("INSERT INTO client (preferred_name, phone_number, archived, created, last_edited, user_id) VALUES "
                          "(:preferred_name, :phone_number, :archived, :created, :last_edited, :user_id)");
                q.bindValue(":preferred_name", params.value("customer_name"));
                q.bindValue(":phone_number", params.value("customer_phone_number"));
                q.bindValue(":archived", false);
                q.bindValue(":created", currentDateTime);
                q.bindValue(":last_edited", currentDateTime);
                q.bindValue(":user_id", UserProfile::instance().userId());

                if (!q.exec())
                    throw DatabaseException(DatabaseException::RRErrorCode::AddTransactionFailure, q.lastError().text(),
                                            QStringLiteral("Failed to insert client ID."));

                clientId = q.lastInsertId().toInt();
                newClientId = clientId;
            }

            if (!clientId)
                throw DatabaseException(DatabaseException::RRErrorCode::AddTransactionFailure, q.lastError().text(),
                                        QStringLiteral("Invalid sales transaction note ID returned."));
        }

        // STEP: Insert note
        if (!params.value("note").toString().trimmed().isEmpty()) {
            q.prepare("INSERT INTO note (note, table_name, created, last_edited, user_id) "
                      "VALUES (:note, :table_name, :created, :last_edited, :user_id)");
            q.bindValue(":note", params.value("note"));
            q.bindValue(":table_name", "sale_transaction");
            q.bindValue(":created", currentDateTime);
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::AddTransactionFailure, q.lastError().text(),
                                        QStringLiteral("Failed to insert sales transaction note."));

            noteId = q.lastInsertId().toInt();
            if (!noteId)
                throw DatabaseException(DatabaseException::RRErrorCode::AddTransactionFailure, q.lastError().text(),
                                        QStringLiteral("Invalid sales transaction note ID returned."));
        }

        // STEP: Insert sale transaction
        q.prepare("INSERT INTO sale_transaction (name, client_id, total_cost, amount_paid, balance, "
                  "discount, suspended, note_id, archived, created, "
                  "last_edited, user_id) VALUES (:customer_name, :client_id, :total_cost, "
                  ":amount_paid, :balance, :discount, :suspended, :note_id, "
                  ":archived, :created, :last_edited, :user_id)");
        q.bindValue(":customer_name", params.value("customer_name"));
        q.bindValue(":client_id", clientId > 0 ? clientId : QVariant(QVariant::Int));
        q.bindValue(":total_cost", params.value("total_cost"));
        q.bindValue(":amount_paid", params.value("amount_paid"));

        if (params.value("action").toString() == "give_change")
            q.bindValue(":balance", 0.0);
        else
            q.bindValue(":balance", qAbs(params.value("balance").toDouble()));
        //q.bindValue(":change_due", params.value("give_change").toBool() ? qAbs(params.value("balance").toDouble()) : 0.0);

        q.bindValue(":discount", params.value("discount", 0.0));
        q.bindValue(":suspended", params.value("suspended", false));
        q.bindValue(":archived", false);
        q.bindValue(":created", currentDateTime);
        q.bindValue(":last_edited", currentDateTime);
        q.bindValue(":user_id", UserProfile::instance().userId());

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::AddTransactionFailure, q.lastError().text(),
                                    QStringLiteral("Failed to insert sale transaction."));

        saleTransactionId = q.lastInsertId().toInt();
        if (!saleTransactionId)
            throw DatabaseException(DatabaseException::RRErrorCode::AddTransactionFailure, q.lastError().text(),
                                    QStringLiteral("Invalid sale transaction ID returned."));

        // STEP: Insert sale payments
        for (const QVariant &payment : payments) {
            QVariantMap paymentInfo = payment.toMap();
            q.prepare("INSERT INTO sale_payment (sale_transaction_id, amount, method, currency, note_id, "
                      "created, last_edited, user_id) VALUES (:sale_transaction_id, :amount, :method, "
                      ":currency, :note_id, :created, :last_edited, :user_id)");
            q.bindValue(":sale_transaction_id", saleTransactionId);
            q.bindValue(":amount", paymentInfo.value("amount"));
            q.bindValue(":method", paymentInfo.value("method"));
            q.bindValue(":note_id", salePaymentNoteId > 0 ? salePaymentNoteId : QVariant(QVariant::Int));
            q.bindValue(":currency", params.value("currency"));
            q.bindValue(":created", currentDateTime);
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::AddTransactionFailure, q.lastError().text(),
                                        QStringLiteral("Failed to insert sale payment."));

        }

        for (const QVariant &item : items) {
            QVariantMap itemInfo = item.toMap();
            // STEP: Deduct quantity if:
            // 1. This is a non-suspended transaction.
            // 2. This is a suspended transaction and you want to reserve the goods for this customer.
            if (!params.value("suspended", false).toBool()) {
                // Check available quantity
                double availableQuantity = 0.0;
                q.prepare("SELECT quantity FROM current_quantity WHERE item_id = :item_id");
                q.bindValue(":item_id", itemInfo.value("item_id"), QSql::Out);

                if (!q.exec())
                    throw DatabaseException(DatabaseException::RRErrorCode::AddTransactionFailure, q.lastError().text(),
                                            QStringLiteral("Failed to get available quantity."));

                if (q.first())
                    availableQuantity = q.value("quantity").toDouble();
                else
                    throw DatabaseException(DatabaseException::RRErrorCode::AddTransactionFailure, q.lastError().text(),
                                            QString("Item %1 does not exist in current quantity table.").arg(itemInfo.value("item_id").toInt()));

                if (availableQuantity < itemInfo.value("quantity").toDouble())
                    throw DatabaseException(DatabaseException::RRErrorCode::AddTransactionFailure, q.lastError().text(),
                                            QString("Insuffient quantity for item %1 -> available=%2, sold=%3")
                                            .arg(itemInfo.value("item_id").toString(),
                                                 QString::number(availableQuantity),
                                                 itemInfo.value("quantity").toString()));

                // Insert into initial_quantity
                q.prepare("INSERT INTO initial_quantity (item_id, quantity, unit_id, reason, archived, created, last_edited, user_id) "
                          "VALUES (:item_id, :quantity, :unit_id, :reason, :archived, :created, :last_edited, :user_id)");
                q.bindValue(":item_id", itemInfo.value("item_id"));
                q.bindValue(":quantity", availableQuantity);
                q.bindValue(":unit_id", itemInfo.value("unit_id"));
                q.bindValue(":reason", request.command());
                q.bindValue(":archived", false);
                q.bindValue(":created", currentDateTime);
                q.bindValue(":last_edited", currentDateTime);
                q.bindValue(":user_id", UserProfile::instance().userId());

                if (!q.exec())
                    throw DatabaseException(DatabaseException::RRErrorCode::AddTransactionFailure, q.lastError().text(),
                                            QStringLiteral("Failed to insert initial quantity."));

                itemInfo.insert("initial_quantity_id", q.lastInsertId());

                // STEP: Update current quantity
                q.prepare("UPDATE current_quantity SET quantity = :quantity, last_edited = :last_edited, user_id = :user_id "
                          "WHERE item_id = :item_id");
                q.bindValue(":item_id", itemInfo.value("item_id"));
                q.bindValue(":quantity", availableQuantity - itemInfo.value("quantity").toDouble());
                q.bindValue(":last_edited", currentDateTime);
                q.bindValue(":user_id", UserProfile::instance().userId());

                if (!q.exec())
                    throw DatabaseException(DatabaseException::RRErrorCode::AddTransactionFailure, q.lastError().text(),
                                            QStringLiteral("Failed to update current quantity."));
            }

            // STEP: Insert sale item
            q.prepare("INSERT INTO sale_item (sale_transaction_id, item_id, unit_id, unit_price, quantity, cost, discount, currency, "
                      "archived, created, last_edited, user_id) "
                      "VALUES (:sale_transaction_id, :item_id, :unit_id, :unit_price, :quantity, :cost, :discount, :currency, "
                      ":archived, :created, :last_edited, :user_id)");
            q.bindValue(":sale_transaction_id", saleTransactionId);
            q.bindValue(":item_id", itemInfo.value("item_id"));
            q.bindValue(":unit_id", itemInfo.value("unit_id"));
            q.bindValue(":unit_price", itemInfo.value("unit_price"));
            q.bindValue(":quantity", itemInfo.value("quantity"));
            q.bindValue(":cost", itemInfo.value("cost"));
            q.bindValue(":discount", itemInfo.value("discount", 0.0));
            q.bindValue(":currency", "NGN");
            q.bindValue(":archived", false);
            q.bindValue(":created", currentDateTime);
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::AddTransactionFailure, q.lastError().text(),
                                        QStringLiteral("Failed to insert sale item."));
        }

        // STEP: Insert debt or credit
        if (!params.value("overlook_balance").toBool() && !params.value("suspended").toBool() && params.value("balance").toDouble() > 0.0) {
            q.prepare("INSERT INTO debtor (client_id, note_id, archived, created, last_edited, user_id) "
                      "VALUES (:client_id, :note_id, :archived, :created, :last_edited, :user_id)");
            q.bindValue(":client_id", clientId);
            q.bindValue(":note_id", QVariant(QVariant::Int));
            q.bindValue(":archived", false);
            q.bindValue(":created", currentDateTime);
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::AddTransactionFailure, q.lastError().text(),
                                        QStringLiteral("Failed to insert debtor."));

            debtorId = q.lastInsertId().toInt();

            q.prepare("INSERT INTO debt_transaction (debtor_id, transaction_table, transaction_id, note_id, archived, "
                      "created, last_edited, user_id) "
                      "VALUES (:debtor_id, :transaction_table, :transaction_id, :note_id, :archived, :created, "
                      ":last_edited, :user_id)");
            q.bindValue(":debtor_id", debtorId);
            q.bindValue(":transaction_table", "sale_transaction");
            q.bindValue(":transaction_id", saleTransactionId);
            q.bindValue(":note_id", QVariant(QVariant::Int));
            q.bindValue(":archived", false);
            q.bindValue(":created", currentDateTime);
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::AddTransactionFailure,
                                        q.lastError().text(),
                                        QStringLiteral("Failed to insert debt transaction."));

            debtTransactionId = q.lastInsertId().toInt();
            if (!debtTransactionId)
                throw DatabaseException(DatabaseException::RRErrorCode::AddTransactionFailure,
                                        q.lastError().text(),
                                        QStringLiteral("Invalid debt transaction ID returned."));

            q.prepare("INSERT INTO debt_payment (debt_transaction_id, total_amount, amount_paid, balance, currency, due_date, note_id, "
                      "archived, created, last_edited, user_id) VALUES (:debt_transaction_id, :total_amount, :amount_paid, :balance, "
                      ":currency, :due_date, :note_id, :archived, :created, :last_edited, :user_id)");
            q.bindValue(":debt_transaction_id", debtTransactionId);
            q.bindValue(":total_amount", params.value("total_cost"));
            q.bindValue(":amount_paid", params.value("amount_paid"));
            q.bindValue(":balance", qAbs(params.value("balance").toDouble()));
            q.bindValue(":currency", "NGN");
            q.bindValue(":due_date", params.value("due_date"));
            q.bindValue(":note_id", QVariant(QVariant::Int));
            q.bindValue(":archived", false);
            q.bindValue(":created", currentDateTime);
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::AddTransactionFailure, q.lastError().text(),
                                        QStringLiteral("Failed to insert debt payment."));
        } else if (!params.value("overlook_balance").toBool() && !params.value("suspended").toBool() && params.value("balance").toDouble() < 0.0) {
            q.prepare("INSERT INTO creditor (client_id, note_id, archived, created, last_edited, user_id) "
                      "VALUES (:client_id, :note_id, :archived, :created, :last_edited, :user_id)");
            q.bindValue(":client_id", clientId);
            q.bindValue(":note_id", QVariant(QVariant::Int));
            q.bindValue(":archived", false);
            q.bindValue(":created", currentDateTime);
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::AddTransactionFailure, q.lastError().text(),
                                        QStringLiteral("Failed to insert creditor."));

            creditorId = q.lastInsertId().toInt();

            q.prepare("INSERT INTO credit_transaction (creditor_id, transaction_table, transaction_id, note_id, "
                      "archived, created, last_edited, user_id) "
                      "VALUES (:client_id, :transaction_table, :transaction_id, :note_id, :archived, :created, "
                      ":last_edited, :user_id)");
            q.bindValue(":creditor_id", creditorId);
            q.bindValue(":transaction_table", "sale_transaction");
            q.bindValue(":transaction_id", saleTransactionId);
            q.bindValue(":note_id", QVariant(QVariant::Int));
            q.bindValue(":archived", false);
            q.bindValue(":created", currentDateTime);
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::AddTransactionFailure, q.lastError().text(),
                                        QStringLiteral("Failed to insert credit transaction."));

            creditTransactionId = q.lastInsertId().toInt();
            if (!creditTransactionId)
                throw DatabaseException(DatabaseException::RRErrorCode::AddTransactionFailure, q.lastError().text(),
                                        QStringLiteral("Invalid credit transaction ID returned."));

            q.prepare("INSERT INTO credit_payment (credit_transaction_id, total_amount, amount_paid, balance, currency, due_date, note_id, "
                      "archived, created, last_edited, user_id) VALUES (:credit_transaction_id, :total_amount, :amount_paid, :balance, "
                      ":currency, :due_date, :note_id, :archived, :created, :last_edited, :user_id)");
            q.bindValue(":credit_transaction_id", creditTransactionId);
            q.bindValue(":total_amount", params.value("total_cost"));
            q.bindValue(":amount_paid", params.value("amount_paid"));
            q.bindValue(":balance", qAbs(params.value("balance").toDouble()));
            q.bindValue(":currency", "NGN");
            q.bindValue(":due_date", params.value("due_date"));
            q.bindValue(":note_id", QVariant(QVariant::Int));
            q.bindValue(":archived", false);
            q.bindValue(":created", currentDateTime);
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::AddTransactionFailure, q.lastError().text(),
                                        QStringLiteral("Failed to insert credit payment."));
        }

        if (!skipSqlTransaction && !DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::CommitTransationFailed, q.lastError().text(),
                                    QStringLiteral("Failed to commit."));

        QVariantMap outcome;
        outcome.insert("client_id", newClientId);
        outcome.insert("transaction_id", saleTransactionId);

        result.setOutcome(outcome);
    } catch (DatabaseException &) {
        if (!skipSqlTransaction && !DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        throw;
    }
}

void SaleSqlManager::updateSuspendedTransaction(const QueryRequest &request, QueryResult &result)
{
    const QVariantMap &params = request.params();
    const QDateTime &currentDateTime = QDateTime::currentDateTime();

    QSqlQuery q(connection());

    try {
        // STEP: Ensure suspended flag is set to true.
        if (!params.value("suspended").toBool())
            throw DatabaseException(DatabaseException::RRErrorCode::UpdateTransactionFailure, QString(), "Suspended flag must be set to true.");

        // STEP: Check if transaction is suspended. (VALIDATION)
        q.prepare("SELECT id FROM sale_transaction WHERE suspended = 1 AND archived = 0 AND id = :transaction_id");
        q.bindValue(":transaction_id", params.value("transaction_id"), QSql::Out);

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::UpdateTransactionFailure, q.lastError().text(), "Failed to fetch transaction ID.");

        if (!q.first())
            throw DatabaseException(DatabaseException::RRErrorCode::UpdateTransactionFailure, QString(), "Only suspended sale transactions can be updated.");

        if (!DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::BeginTransactionFailed, q.lastError().text(), "Failed to start transation.");

        addSaleTransaction(request, result, true);

        // STEP: Archive old sale transaction.
        q.prepare("UPDATE sale_transaction SET archived = 1, last_edited = :last_edited, user_id = :user_id WHERE id = :transaction_id");
        q.bindValue(":last_edited", currentDateTime);
        q.bindValue(":user_id", UserProfile::instance().userId());
        q.bindValue(":transaction_id", params.value("transaction_id"));

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::UpdateTransactionFailure, q.lastError().text(), "Failed to archive sale transaction.");

        // STEP: Archive old sale item.
        q.prepare("UPDATE sale_item SET archived = 1, last_edited = :last_edited, user_id = :user_id "
                  "WHERE sale_transaction_id = :transaction_id");
        q.bindValue(":last_edited", currentDateTime);
        q.bindValue(":user_id", UserProfile::instance().userId());
        q.bindValue(":transaction_id", params.value("transaction_id"));

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::UpdateTransactionFailure, q.lastError().text(), "Failed to archive sale item.");

        if (!DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::CommitTransationFailed, q.lastError().text(), "Failed to commit.");
    } catch (DatabaseException &) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        throw;
    }
}

void SaleSqlManager::viewSaleCart(const QueryRequest &request, QueryResult &result)
{
    QVariantMap params = request.params();
    QSqlQuery q(connection());
/*
SELECT sale_transaction.id as transaction_id,
sale_transaction.name as customer_name,
sale_transaction.client_id,
sale_transaction.total_cost,
sale_transaction.suspended,
sale_transaction.note_id,
sale_transaction.created,
sale_transaction.last_edited,
sale_transaction.user_id,
category.id as category_id,
category.category,
sale_item.item_id,
item.item,
sale_item.unit_price,
sale_item.quantity,
current_quantity.quantity as available_quantity,
unit.id as unit_id,
unit.unit,
sale_item.cost, sale_item.discount,
sale_item.currency, note.note FROM
(sale_item
INNER JOIN sale_transaction ON sale_item.sale_transaction_id = sale_transaction.id
INNER JOIN item ON sale_item.item_id = item.id
INNER JOIN unit ON sale_item.item_id = unit.item_id
INNER JOIN current_quantity ON sale_item.item_id = current_quantity.item_id)
INNER JOIN category ON item.category_id = category.id
LEFT JOIN note ON sale_item.note_id = note.id WHERE sale_transaction.archived = 0 AND sale_item.archived = 0 AND sale_transaction.id = 3;
*/
    try {
        q.prepare("SELECT sale_transaction.id as transaction_id, sale_transaction.name as customer_name, sale_transaction.client_id as client_id, "
                  "client.phone_number as customer_phone_number, "
                  "sale_transaction.total_cost, sale_transaction.suspended, sale_transaction.note_id, sale_transaction.created, "
                  "sale_transaction.last_edited, sale_transaction.user_id, category.id as category_id, category.category, "
                  "sale_item.item_id, item.item, sale_item.unit_price as unit_price, "
                  "sale_item.quantity, current_quantity.quantity as available_quantity, unit.id as unit_id, unit.unit, "
                  "unit.cost_price as cost_price, unit.retail_price as retail_price, "
                  "sale_item.cost, sale_item.discount, "
                  "sale_item.currency, note.note FROM (sale_item "
                  "INNER JOIN sale_transaction ON sale_item.sale_transaction_id = sale_transaction.id "
                  "INNER JOIN item ON sale_item.item_id = item.id "
                  "INNER JOIN unit ON sale_item.item_id = unit.item_id "
                  "INNER JOIN current_quantity ON sale_item.item_id = current_quantity.item_id) "
                  "INNER JOIN category ON item.category_id = category.id "
                  "LEFT JOIN client ON sale_transaction.client_id = client.id "
                  "LEFT JOIN note ON sale_item.note_id = note.id "
                  "WHERE sale_transaction.id = :transaction_id AND sale_transaction.archived = :sale_transaction_archived "
                  "AND sale_item.archived = :sale_item_archived");
        q.bindValue(":transaction_id", params.value("transaction_id"), QSql::Out);
        q.bindValue(":sale_transaction_archived", params.value("sale_transaction_archived", false), QSql::Out);
        q.bindValue(":sale_item_archived", params.value("sale_item_archived", false), QSql::Out);

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::ViewSaleTransactionFailure, q.lastError().text(), "Failed to fetch sale cart.");

        QVariantMap outcome;
        QVariantList items;
        while (q.next()) {
            items.append(recordToMap(q.record()));
        }

        if (!items.isEmpty()) {
            outcome.insert("transaction_id", params.value("transaction_id"));
            outcome.insert("client_id", items.first().toMap().value("client_id"));
            outcome.insert("customer_name", items.first().toMap().value("customer_name"));
            outcome.insert("customer_phone_number", items.first().toMap().value("customer_phone_number"));
            outcome.insert("total_cost", items.first().toMap().value("total_cost"));
            outcome.insert("items", items);
            outcome.insert("record_count", items.count());

            result.setOutcome(outcome);
        }
    } catch (DatabaseException &) {
        throw;
    }
}

void SaleSqlManager::undoAddSaleTransaction(const QueryRequest &request, QueryResult &result)
{
    QVariantMap params = request.params();
    const QDateTime currentDateTime = QDateTime::currentDateTime();

    QSqlQuery q(connection());

    try {
        if (!DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::BeginTransactionFailed, q.lastError().text(), "Failed to start transation.");

        if (params.value("client_id").toInt() > 0) {
            q.prepare("UPDATE client SET archived = 1 - archived, last_edited = :last_edited, user_id = :user_id WHERE id = :client_id");
            q.bindValue(":client_id", params.value("client_id"));
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::UndoAddTransactionFailure, q.lastError().text(), "Failed to update client.");
        }

        if (params.value("transaction_id").toInt() > 0) {
            q.prepare("UPDATE sale_transaction SET archived = 1 - archived, last_edited = :last_edited, user_id = :user_id "
                      "WHERE id = :transaction_id");
            q.bindValue(":transaction_id", params.value("transaction_id"));
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::UndoAddTransactionFailure, q.lastError().text(), "Failed to update sale transaction.");

            q.prepare("UPDATE debt_transaction SET archived = 1 - archived, last_edited = :last_edited, user_id = :user_id "
                      "WHERE :transaction_table = :transaction_table AND transaction_id = :transaction_id");
            q.bindValue(":transaction_table", "sale_transaction");
            q.bindValue(":transaction_id", params.value("transaction_id"));
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::UndoAddTransactionFailure, q.lastError().text(), "Failed to update debt transaction.");

            q.prepare("UPDATE credit_transaction SET archived = 1 - archived, last_edited = :last_edited, user_id = :user_id "
                      "WHERE :transaction_table = :transaction_table AND transaction_id = :transaction_id");
            q.bindValue(":transaction_table", "sale_transaction");
            q.bindValue(":transaction_id", params.value("transaction_id"));
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::UndoAddTransactionFailure, q.lastError().text(), "Failed to update credit transaction.");
        }


//        UPDATE current_quantity  INNER JOIN sale_item on current_quantity.item_id = sale_item.item_id
//                INNER JOIN sale_transaction ON sale_item.sale_transaction_id = sale_transaction.id SET
//                current_quantity.quantity = current_quantity.quantity + sale_item.quantity WHERE sale_transaction.id = 21;

        q.prepare("UPDATE current_quantity "
                  "INNER JOIN sale_item ON current_quantity.item_id = sale_item.item_id "
                  "INNER JOIN sale_transaction ON sale_item.sale_transaction_id = sale_transaction.id "
                  "SET current_quantity.quantity = current_quantity.quantity + sale_item.quantity, "
                  "current_quantity.last_edited = :last_edited, current_quantity.user_id = :user_id "
                  "WHERE sale_transaction.id = :transaction_id");
        q.bindValue(":transaction_id", params.value("transaction_id"));
        q.bindValue(":last_edited", currentDateTime);
        q.bindValue(":user_id", UserProfile::instance().userId());

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::UndoAddTransactionFailure, q.lastError().text(), "Failed to update current quantity.");

        if (!DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::CommitTransationFailed, q.lastError().text(), "Failed to commit.");

        result.setOutcome(request.params());
    } catch (DatabaseException &) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        throw;
    }
}

void SaleSqlManager::viewSaleTransactions(const QueryRequest &request, QueryResult &result)
{
    const QVariantMap &params = request.params();
    QSqlQuery q(connection());

    try {
        q.prepare("SELECT sale_transaction.id as transaction_id, sale_transaction.name as customer_name, "
                  "sale_transaction.client_id,  total_cost, amount_paid, balance, discount, "
                  "suspended, note_id, note.note, sale_transaction.archived, sale_transaction.created, "
                  "sale_transaction.last_edited, sale_transaction.user_id FROM sale_transaction "
                  "LEFT JOIN note ON sale_transaction.note_id = note.id "
                  "WHERE sale_transaction.suspended = :suspended AND sale_transaction.archived = :archived "
                  "AND sale_transaction.created BETWEEN :from AND :to ORDER BY created ASC");
        q.bindValue(":suspended", params.value("suspended", false), QSql::Out);
        q.bindValue(":archived", params.value("archived", false), QSql::Out);
        q.bindValue(":from", params.value("from", QDateTime()), QSql::Out);
        q.bindValue(":to", params.value("to", QDateTime::currentDateTime()), QSql::Out);

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::ViewSaleTransactionFailure,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to fetch sale transactions."));

        QVariantList transactions;
        while (q.next()) {
            transactions.append(recordToMap(q.record()));
        }

        result.setOutcome(QVariantMap { { "transactions", transactions }, { "record_count", transactions.count() } });
    } catch (DatabaseException &) {
        throw;
    }
}

void SaleSqlManager::viewSaleTransactionItems(const QueryRequest &request, QueryResult &result)
{
    const QVariantMap &params = request.params();
    QSqlQuery q(connection());

    try {
        AbstractSqlManager::enforceArguments({ "transaction_id" }, params);

        q.prepare("SELECT category.id as category_id, category.category, sale_item.item_id, item.item, "
                  "sale_item.unit_price, sale_item.quantity, sale_item.unit_id, "
                  "unit.unit, sale_item.cost, sale_item.discount, sale_item.currency, sale_item.note_id, note.note, "
                  "sale_item.archived, sale_item.created, sale_item.last_edited, sale_item.user_id, user.user FROM sale_item "
                  "INNER JOIN item ON sale_item.item_id = item.id "
                  "INNER JOIN category ON category.id = item.category_id "
                  "INNER JOIN unit ON sale_item.unit_id = unit.id "
                  "INNER JOIN sale_transaction ON sale_transaction.id = sale_item.sale_transaction_id "
                  "LEFT JOIN user ON sale_item.user_id = user.id "
                  "LEFT JOIN note ON sale_transaction.note_id = note.id "
                  "WHERE sale_transaction_id = :transaction_id AND sale_transaction.suspended = :suspended "
                  "AND sale_transaction.archived = :archived");
        q.bindValue(":transaction_id", params.value("transaction_id"));
        q.bindValue(":suspended", params.value("suspended", false));
        q.bindValue(":archived", params.value("archived", false));

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::ViewSaleTransactionFailure,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to fetch sale items for transaction."));

        QVariantList items;
        while (q.next()) {
            items.append(recordToMap(q.record()));
        }

        result.setOutcome(QVariantMap { { "items", items }, { "record_count", items.count() } });
    } catch (DatabaseException &) {
        throw;
    }
}

void SaleSqlManager::viewSaleHome(const QueryRequest &request, QueryResult &result)
{
    QVariantMap params = request.params();
    QSqlQuery q(connection());
    QVariantList records;
    const int currentYear = QDate::currentDate().year();

    try {
        {
            /*
            SELECT DATE(created), SUM(amount_paid) FROM sale_transaction WHERE archived = 0 AND suspended = 0
            AND DATE(created) BETWEEN DATE('2018-03-05') AND CURDATE() GROUP BY DATE(created);
            */
            q.prepare("SELECT DATE(sale_transaction.created) AS created, SUM(sale_transaction.amount_paid) AS amount_paid FROM sale_transaction "
                      "WHERE sale_transaction.suspended = 0 AND sale_transaction.archived = 0 "
                      "AND DATE(sale_transaction.created) BETWEEN DATE(:from_date) AND DATE(:to_date) "
                      "GROUP BY DATE(sale_transaction.created)");
            q.bindValue(":from_date", params.value("from_date", QDate(QDate::currentDate().year(), 1, 1)));
            q.bindValue(":to_date", params.value("to_date", QDate::currentDate()));

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::ViewSaleHomeFailure, q.lastError().text(), "Failed to fetch total revenue.");

            QVariantList revenues;
            while (q.next()) {
                revenues.append(recordToMap(q.record()));
            }

            QVariantMap totalRevenueInfo;
            totalRevenueInfo.insert("data_type", "total_revenue");
            totalRevenueInfo.insert("data_model", revenues);

            if (!revenues.isEmpty())
                records.append(totalRevenueInfo);
        }
        {
            /*
SELECT category.id AS category_id,
category.category, sale_item.item_id,
item.item, SUM(sale_item.cost - sale_item.discount) AS total_cost,
SUM(sale_item.quantity) AS quantity
FROM sale_item
INNER JOIN item ON item.id = sale_item.item_id
INNER JOIN category ON category.id = item.category_id
WHERE sale_item.created BETWEEN '2018-01-01 00:00:00' AND '2019-01-01 00:00:00')
GROUP BY sale_item.item_id
ORDER BY SUM(sale_item.quantity) DESC
LIMIT 5;
            */
            q.prepare("SELECT category.id AS category_id, category.category, sale_item.item_id, item.item, "
                      "SUM(sale_item.cost - sale_item.discount) AS total_revenue, SUM(sale_item.quantity) AS total_quantity "
                      "FROM sale_item "
                      "INNER JOIN item ON item.id = sale_item.item_id "
                      "INNER JOIN category ON category.id = item.category_id "
                      "WHERE sale_item.created BETWEEN :from_date_time AND :to_date_time "
                      "GROUP BY sale_item.item_id "
                      "ORDER BY SUM(sale_item.quantity) DESC "
                      "LIMIT :limit");
            q.bindValue(":from_date_time", params.value("from_date_time", QDateTime(QDate(currentYear, 0, 0), QTime(0, 0))));
            q.bindValue(":to_date_time", params.value("to_date_time", QDateTime::currentDateTime()));
            q.bindValue(":limit", params.value("limit", 5));

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::ViewSaleHomeFailure, q.lastError().text(), "Failed to fetch most sold items.");

            QVariantList mostSoldItems;
            while (q.next()) {
                mostSoldItems.append(recordToMap(q.record()));
            }

            QVariantMap mostSoldItemsInfo;
            mostSoldItemsInfo.insert("data_type", "most_sold_items");
            mostSoldItemsInfo.insert("data_model", mostSoldItems);

            if (!mostSoldItems.isEmpty())
                records.append(mostSoldItemsInfo);
        }

        QVariantMap outcome;
        outcome.insert("record_count", records.count());
        outcome.insert("records", records);
        result.setOutcome(outcome);
    } catch (DatabaseException &) {
        throw;
    }
}
