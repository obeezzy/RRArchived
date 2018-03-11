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
            addTransaction(request, result);
        else if (request.command() == "update_suspended_sale_transaction")
            updateSuspendedTransaction(request, result);
        else if (request.command() == "undo_add_sale_transaction")
            undoAddTransaction(request, result);
        else if (request.command() == "view_sale_cart")
            viewSaleCart(request, result);
        else if (request.command() == "view_sale_transactions")
            viewSaleTransactions(request, result);
        else if (request.command() == "view_sale_items_for_transaction")
            viewSaleItemsForTransaction(request, result);

        result.setSuccessful(true);
    } catch (DatabaseException &e) {
        result.setSuccessful(false);
        result.setErrorCode(e.code());
        result.setErrorMessage(e.message());
        result.setErrorUserMessage(e.userMessage());
    }

    return result;
}

void SaleSqlManager::addTransaction(const QueryRequest &request, QueryResult &result, bool skipSqlTransaction)
{
    const QVariantMap params = request.params();
    QVariantList items = params.value("items").toList();
    const QDateTime currentDateTime = QDateTime::currentDateTime();
    int noteId = 0;
    int clientId = 0;
    int newClientId = 0;
    int saleTransactionId = 0;
    int debtTransactionId = 0;
    int creditTransactionId = 0;

    QSqlQuery q(connection());

    try {
        if (!skipSqlTransaction && !DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::BeginTransactionFailed, q.lastError().text(), "Failed to start transation.");

        // STEP: Get client ID
        if (!params.value("customer_phone_number").toString().trimmed().isEmpty() && !params.value("suspended").toBool()) {
            q.prepare("SELECT id FROM client WHERE phone_number = :phone_number");
            q.bindValue(":phone_number", params.value("customer_phone_number"), QSql::Out);

            if (!q.exec())
                throw DatabaseException(DatabaseException::AddTransactionFailure, q.lastError().text(), "Failed to get client ID.");

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
                    throw DatabaseException(DatabaseException::AddTransactionFailure, q.lastError().text(), "Failed to insert client ID.");

                clientId = q.lastInsertId().toInt();
                newClientId = clientId;
            }

            if (!clientId)
                throw DatabaseException(DatabaseException::AddTransactionFailure, q.lastError().text(), "Invalid sales transaction note ID returned.");
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
                throw DatabaseException(DatabaseException::AddTransactionFailure, q.lastError().text(), "Failed to insert sales transaction note.");

            noteId = q.lastInsertId().toInt();
            if (!noteId)
                throw DatabaseException(DatabaseException::AddTransactionFailure, q.lastError().text(), "Invalid sales transaction note ID returned.");
        }

        // STEP: Insert sale transaction
        q.prepare("INSERT INTO sale_transaction (name, client_id, total_cost, amount_paid, balance, discount, suspended, note_id, archived, created, "
                  "last_edited, user_id) VALUES (:name, :client_id, :total_cost, :amount_paid, :balance, :discount, :suspended, :note_id, :archived, "
                  ":created, :last_edited, :user_id)");
        q.bindValue(":name", params.value("customer_name"));
        q.bindValue(":client_id", clientId > 0 ? clientId : QVariant(QVariant::Int));
        q.bindValue(":total_cost", params.value("total_cost"));
        q.bindValue(":amount_paid", params.value("amount_paid"));
        q.bindValue(":balance", params.value("balance"));
        q.bindValue(":discount", params.value("discount", 0.0));
        q.bindValue(":suspended", params.value("suspended", false));
        q.bindValue(":archived", false);
        q.bindValue(":created", currentDateTime);
        q.bindValue(":last_edited", currentDateTime);
        q.bindValue(":user_id", UserProfile::instance().userId());

        if (!q.exec())
            throw DatabaseException(DatabaseException::AddTransactionFailure, q.lastError().text(), "Failed to insert sale transaction.");

        saleTransactionId = q.lastInsertId().toInt();
        if (!saleTransactionId)
            throw DatabaseException(DatabaseException::AddTransactionFailure, q.lastError().text(), "Invalid sale transaction ID returned.");

        for (QVariant &item : items) {
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
                    throw DatabaseException(DatabaseException::AddTransactionFailure, q.lastError().text(), "Failed to get available quantity.");

                if (q.first())
                    availableQuantity = q.value("quantity").toDouble();
                else
                    throw DatabaseException(DatabaseException::AddTransactionFailure, q.lastError().text(), QString("Item %1 does not exist in current quantity table.").arg(itemInfo.value("item_id").toInt()));

                if (availableQuantity < itemInfo.value("quantity").toDouble())
                    throw DatabaseException(DatabaseException::AddTransactionFailure, q.lastError().text(),
                                            QString("Insuffient quantity for item %1 -> available=%2, sold=%3").arg(itemInfo.value("item_id").toDouble(), availableQuantity, itemInfo.value("quantity").toDouble()));

                // Insert into initial_quantity
                q.prepare("INSERT INTO initial_quantity (item_id, quantity, archived, created, last_edited, user_id) "
                          "VALUES (:item_id, :quantity, :archived, :created, :last_edited, :user_id)");
                q.bindValue(":item_id", itemInfo.value("item_id"));
                q.bindValue(":quantity", availableQuantity);
                q.bindValue(":archived", false);
                q.bindValue(":created", currentDateTime);
                q.bindValue(":last_edited", currentDateTime);
                q.bindValue(":user_id", UserProfile::instance().userId());

                if (!q.exec())
                    throw DatabaseException(DatabaseException::AddTransactionFailure, q.lastError().text(), "Failed to insert initial quantity.");

                itemInfo.insert("initial_quantity_id", q.lastInsertId());

                // STEP: Update current quantity
                q.prepare("UPDATE current_quantity SET quantity = :quantity, last_edited = :last_edited, user_id = :user_id "
                          "WHERE item_id = :item_id");
                q.bindValue(":item_id", itemInfo.value("item_id"));
                q.bindValue(":quantity", availableQuantity - itemInfo.value("quantity").toDouble());
                q.bindValue(":last_edited", currentDateTime);
                q.bindValue(":user_id", UserProfile::instance().userId());

                if (!q.exec())
                    throw DatabaseException(DatabaseException::AddTransactionFailure, q.lastError().text(), "Failed to update current quantity.");
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
                throw DatabaseException(DatabaseException::AddTransactionFailure, q.lastError().text(), "Failed to insert sale item.");
        }

        // STEP: Insert debt or credit
        if (!params.value("overlook_balance").toBool() && !params.value("suspended").toBool() && params.value("balance").toDouble() > 0.0) {
            q.prepare("INSERT INTO debt_transaction (client_id, transaction_table, transaction_id, note_id, archived, created, last_edited, user_id) "
                      "VALUES (:client_id, :transaction_table, :transaction_id, :note_id, :created, :last_edited, :user_id)");
            q.bindValue(":client_id", clientId);
            q.bindValue(":transaction_table", "sale_transaction");
            q.bindValue("transaction_id", saleTransactionId);
            q.bindValue(":note_id", QVariant(QVariant::Int));
            q.bindValue(":created", currentDateTime);
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::AddTransactionFailure, q.lastError().text(), "Failed to insert debt transaction.");

            debtTransactionId = q.lastInsertId().toInt();
            if (!debtTransactionId)
                throw DatabaseException(DatabaseException::AddTransactionFailure, q.lastError().text(), "Invalid debt transaction ID returned.");

            q.prepare("INSERT INTO debt_payment (debt_transaction_id, total_debt, amount_paid, balance, currency, due_date, note_id, "
                      "archived, created, last_edited, user_id) VALUES (:debt_transaction_id, :total_amount, :amount_paid, :balance, "
                      ":currency, :due_date, :note_id, :created, :last_edited, :user_id)");
            q.bindValue(":debt_transaction_id", debtTransactionId);
            q.bindValue(":total_amount", params.value("total_cost"));
            q.bindValue("amount_paid", params.value("amount_paid"));
            q.bindValue("balance", params.value("balance"));
            q.bindValue("currency", "NGN");
            q.bindValue("due_date", params.value("due_date"));
            q.bindValue(":note_id", QVariant(QVariant::Int));
            q.bindValue(":created", currentDateTime);
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::AddTransactionFailure, q.lastError().text(), "Failed to insert debt payment.");
        } else if (!params.value("overlook_balance").toBool() && !params.value("suspended").toBool() && params.value("balance").toDouble() < 0.0) {
            q.prepare("INSERT INTO credit_transaction (client_id, transaction_table, transaction_id, note_id, archived, created, last_edited, user_id) "
                      "VALUES (:client_id, :transaction_table, :transaction_id, :note_id, :created, :last_edited, :user_id)");
            q.bindValue(":client_id", clientId);
            q.bindValue(":transaction_table", "sale_transaction");
            q.bindValue("transaction_id", saleTransactionId);
            q.bindValue(":note_id", QVariant(QVariant::Int));
            q.bindValue(":created", currentDateTime);
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::AddTransactionFailure, q.lastError().text(), "Failed to insert credit transaction.");

            creditTransactionId = q.lastInsertId().toInt();
            if (!creditTransactionId)
                throw DatabaseException(DatabaseException::AddTransactionFailure, q.lastError().text(), "Invalid credit transaction ID returned.");

            q.prepare("INSERT INTO credit_payment (credit_transaction_id, total_amount, amount_paid, balance, currency, due_date, note_id, "
                      "archived, created, last_edited, user_id) VALUES (:credit_transaction_id, :total_amount, :amount_paid, :balance, "
                      ":currency, :due_date, :note_id, :created, :last_edited, :user_id)");
            q.bindValue(":credit_transaction_id", creditTransactionId);
            q.bindValue(":total_amount", params.value("total_cost"));
            q.bindValue("amount_paid", params.value("amount_paid"));
            q.bindValue("balance", qAbs(params.value("balance").toDouble()));
            q.bindValue("currency", "NGN");
            q.bindValue("due_date", params.value("due_date"));
            q.bindValue(":note_id", QVariant(QVariant::Int));
            q.bindValue(":created", currentDateTime);
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::AddTransactionFailure, q.lastError().text(), "Failed to insert credit payment.");
        }

        if (!skipSqlTransaction && !DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::CommitTransationFailed, q.lastError().text(), "Failed to commit.");

        QVariantMap outcome;
        outcome.insert("client_id", newClientId);
        outcome.insert("transaction_id", saleTransactionId);

        result.setOutcome(outcome);
    } catch (DatabaseException &) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        throw;
    }
}

void SaleSqlManager::updateSuspendedTransaction(QueryRequest request, QueryResult &result)
{
    QVariantMap params = request.params();
    // Make sure transaction is always suspended.
    params.insert("suspended", true);
    request.setCommand(request.command(), params, request.type());

    const QDateTime currentDateTime = QDateTime::currentDateTime();

    QSqlQuery q(connection());

    try {
        // STEP: Check if transaction is suspended. (VALIDATION)
        q.prepare("SELECT id FROM sale_transaction WHERE id = :transaction_id AND suspended = :suspended AND archived = :archived");
        q.bindValue(":transaction_id", params.value("transaction_id"), QSql::Out);
        q.bindValue(":suspended", true);
        q.bindValue(":archived", false);
        if (!q.exec())
            throw DatabaseException(DatabaseException::UpdateTransactionFailure, q.lastError().text(), "Failed to fetch transaction ID.");

        if (!q.first())
            throw DatabaseException(DatabaseException::UpdateTransactionFailure, QString(), "Only suspended sale transactions can be updated.");

        if (!DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::BeginTransactionFailed, q.lastError().text(), "Failed to start transation.");

        addTransaction(request, result, true);

        // STEP: Archive old sale transaction.
        q.prepare("UPDATE sale_transaction SET archived = :archived, last_edited = :last_edited, user_id = :user_id WHERE id = :transaction_id");
        q.bindValue(":archived", true);
        q.bindValue(":last_edited", currentDateTime);
        q.bindValue(":user_id", UserProfile::instance().userId());
        q.bindValue(":transaction_id", params.value("transaction_id"));

        if (!q.exec())
            throw DatabaseException(DatabaseException::UpdateTransactionFailure, q.lastError().text(), "Failed to archive sale transaction.");

        // STEP: Archive old sale item.
        q.prepare("UPDATE sale_item SET archived = :archived, last_edited = :last_edited, user_id = :user_id "
                  "WHERE sale_transaction_id = :transaction_id");
        q.bindValue(":archived", true);
        q.bindValue(":last_edited", currentDateTime);
        q.bindValue(":user_id", UserProfile::instance().userId());
        q.bindValue(":transaction_id", params.value("transaction_id"));

        if (!q.exec())
            throw DatabaseException(DatabaseException::UpdateTransactionFailure, q.lastError().text(), "Failed to archive sale item.");

        if (!DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::CommitTransationFailed, q.lastError().text(), "Failed to commit.");
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
        q.prepare("SELECT sale_transaction.id as transaction_id, sale_transaction.name as customer_name, sale_transaction.client_id, "
                  "sale_transaction.total_cost, sale_transaction.suspended, sale_transaction.note_id, sale_transaction.created, "
                  "sale_transaction.last_edited, sale_transaction.user_id, category.id as category_id, category.category, "
                  "sale_item.item_id, item.item, sale_item.unit_price as unit_price, "
                  "sale_item.quantity, current_quantity.quantity as available_quantity, unit.id as unit_id, unit.unit, "
                  "sale_item.cost, sale_item.discount, "
                  "sale_item.currency, note.note FROM (sale_item "
                  "INNER JOIN sale_transaction ON sale_item.sale_transaction_id = sale_transaction.id "
                  "INNER JOIN item ON sale_item.item_id = item.id "
                  "INNER JOIN unit ON sale_item.item_id = unit.item_id "
                  "INNER JOIN current_quantity ON sale_item.item_id = current_quantity.item_id) "
                  "INNER JOIN category ON item.category_id = category.id "
                  "LEFT JOIN note ON sale_item.note_id = note.id "
                  "WHERE sale_transaction.id = :transaction_id AND sale_transaction.archived = :sale_transaction_archived "
                  "AND sale_item.archived = :sale_item_archived");
        q.bindValue(":transaction_id", params.value("transaction_id"), QSql::Out);
        q.bindValue(":sale_transaction_archived", params.value("sale_transaction_archived", false), QSql::Out);
        q.bindValue(":sale_item_archived", params.value("sale_item_archived", false), QSql::Out);

        if (!q.exec())
            throw DatabaseException(DatabaseException::ViewSaleTransactionFailure, q.lastError().text(), "Failed to fetch sale cart.");

        QVariantMap outcome;
        QVariantList records;
        while (q.next()) {
            records.append(recordToMap(q.record()));
        }

        if (!records.isEmpty()) {
            outcome.insert("transaction_id", params.value("transaction_id"));
            outcome.insert("customer_name", records.first().toMap().value("customer_name"));
            outcome.insert("customer_phone_number", records.first().toMap().value("customer_phone_number"));
            outcome.insert("total_cost", records.first().toMap().value("total_cost"));
            outcome.insert("items", records);

            result.setOutcome(outcome);
        }
    } catch (DatabaseException &) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        throw;
    }
}

void SaleSqlManager::undoAddTransaction(const QueryRequest &request, QueryResult &result)
{
    QVariantMap params = request.params();
    const QDateTime currentDateTime = QDateTime::currentDateTime();

    QSqlQuery q(connection());

    try {
        if (!DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::BeginTransactionFailed, q.lastError().text(), "Failed to start transation.");

        if (params.value("client_id").toInt() > 0) {
            q.prepare("UPDATE client SET archived = 1 - archived, last_edited = :last_edited, user_id = :user_id WHERE id = :client_id");
            q.bindValue(":client_id", params.value("client_id"));
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::UndoAddTransactionFailure, q.lastError().text(), "Failed to update client.");
        }

        if (params.value("transaction_id").toInt() > 0) {
            q.prepare("UPDATE sale_transaction SET archived = 1 - archived, last_edited = :last_edited, user_id = :user_id "
                      "WHERE id = :transaction_id");
            q.bindValue(":transaction_id", params.value("transaction_id"));
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::UndoAddTransactionFailure, q.lastError().text(), "Failed to update sale transaction.");

            q.prepare("UPDATE debt_transaction SET archived = 1 - archived, last_edited = :last_edited, user_id = :user_id "
                      "WHERE :transaction_table = :transaction_table AND transaction_id = :transaction_id");
            q.bindValue(":transaction_table", "sale_transaction");
            q.bindValue(":transaction_id", params.value("transaction_id"));
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::UndoAddTransactionFailure, q.lastError().text(), "Failed to update debt transaction.");

            q.prepare("UPDATE credit_transaction SET archived = 1 - archived, last_edited = :last_edited, user_id = :user_id "
                      "WHERE :transaction_table = :transaction_table AND transaction_id = :transaction_id");
            q.bindValue(":transaction_table", "sale_transaction");
            q.bindValue(":transaction_id", params.value("transaction_id"));
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::UndoAddTransactionFailure, q.lastError().text(), "Failed to update credit transaction.");
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
            throw DatabaseException(DatabaseException::UndoAddTransactionFailure, q.lastError().text(), "Failed to update current quantity.");

        if (!DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::CommitTransationFailed, q.lastError().text(), "Failed to commit.");

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
        q.prepare("SELECT sale_transaction.id as transaction_id, sale_transaction.name as customer_name, sale_transaction.client_id, "
                  "total_cost, amount_paid, balance, discount, "
                  "suspended, note_id, note.note, sale_transaction.archived, sale_transaction.created, sale_transaction.last_edited, "
                  "sale_transaction.user_id FROM sale_transaction "
                  "LEFT JOIN note ON sale_transaction.note_id = note.id "
                  "WHERE suspended = :suspended AND archived = :archived");
        q.bindValue(":suspended", params.value("suspended", false), QSql::Out);
        q.bindValue(":archived", params.value("archived", false), QSql::Out);

        if (!q.exec())
            throw DatabaseException(DatabaseException::ViewSaleTransactionFailure, q.lastError().text(), "Failed to fetch sale transactions.");

        QVariantList transactions;
        while (q.next()) {
            transactions.append(recordToMap(q.record()));
        }

        result.setOutcome(QVariantMap { { "transactions", transactions } });
    } catch (DatabaseException &) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        throw;
    }
}

void SaleSqlManager::viewSaleItemsForTransaction(const QueryRequest &request, QueryResult &result)
{
    QVariantMap params = request.params();
    QSqlQuery q(connection());

    try {
        q.prepare("SELECT sale_transaction.*, note.note FROM sale_transaction LEFT JOIN note ON sale_transaction.note_id = note.id "
                  "WHERE suspended = :suspended AND archived = :archived");
        q.bindValue(":suspended", params.value("suspended", false));
        q.bindValue(":archived", params.value("archived", false));

        if (!q.exec())
            throw DatabaseException(DatabaseException::ViewSaleTransactionFailure, q.lastError().text(), "Failed to fetch sale items for transaction.");

        QVariantList records;
        while (q.next()) {
            records.append(recordToMap(q.record()));
        }

        result.setOutcome(QVariantMap { { "items", records } });
    } catch (DatabaseException &) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        throw;
    }
}
