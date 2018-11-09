USE ###DATABASENAME###;

#START TRANSACTION;
#DELIMITER //

CREATE PROCEDURE ViewStockItemDetails(
    INOUT ioItemId INTEGER,
    OUT oCategoryId INTEGER,
    OUT oCategory VARCHAR(100),
    OUT oItem VARCHAR(100),
    OUT oDescription VARCHAR(100),
    OUT oDivisible TINYINT,
    OUT oImage BLOB,
    OUT oQuantity DOUBLE,
    OUT oUnitId INTEGER,
    OUT oUnit VARCHAR(100),
    OUT oCostPrice DECIMAL(19,2),
    OUT oRetailPrice DECIMAL(19,2),
    OUT oCurrency VARCHAR(4),
    OUT oCreated DATETIME,
    OUT oLastEdited DATETIME,
    OUT oUserId INTEGER,
    OUT oUser VARCHAR(100)
    )
BEGIN
    SELECT item.id AS item_id, category.id AS category_id, category.category, item.item, item.description,
        item.divisible, item.image, current_quantity.quantity,
        unit.id AS unit_id, unit.unit, unit.cost_price,
        unit.retail_price, unit.currency, item.created, item.last_edited, item.user_id, 'user' AS user
        INTO ioItemId, oCategoryId, oCategory, oItem, oDescription, oDivisible, oImage, oQuantity, oUnitId,
        oUnit, oCostPrice, oRetailPrice, oCurrency, oCreated, oLastEdited, oUserId, oUser
        FROM item
        INNER JOIN category ON item.category_id = category.id
        INNER JOIN unit ON item.id = unit.item_id
        INNER JOIN current_quantity ON item.id = current_quantity.item_id
        LEFT JOIN user ON item.user_id = user.id
        WHERE item.archived = 0 AND unit.base_unit_equivalent = 1
        AND item.id = ioItemId;
END# //

#DELIMITER ;
#COMMIT;

--

START TRANSACTION;
DELIMITER //

DROP PROCEDURE IF EXISTS AddSaleTransaction;
CREATE PROCEDURE AddSaleTransaction(
    IN iCustomerName VARCHAR(100),
    IN iClientId VARCHAR(100),
    IN iTotalCost DECIMAL(19,2),
    IN iBalance DECIMAL(19,2),
    IN iDiscount DECIMAL(19,2),
    IN iSuspended BOOLEAN,
    IN iPayments JSON,
    IN iItems JSON,
    IN iNote VARCHAR(200),
    IN iUserId INTEGER
    )
BEGIN
    SET @noteId = 0;
    SET @saleTransactionId = 0;

    IF iNote IS NOT NULL THEN
        INSERT INTO note (note, table_name, created, last_edited, user_id) VALUES (iNote, 'sale_transaction', CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);
        SELECT LAST_INSERT_ID() INTO @noteId;
    END IF;

    INSERT INTO sale_transaction (name, client_id, total_cost, balance, discount, suspended, note_id, archived, created, last_edited, user_id)
        VALUES (iCustomerName, iClientId, iTotalCost, iBalance, iDiscount, iSuspended, @noteId, FALSE, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);
    SELECT LAST_INSERT_ID() INTO @saleTransactionId;

    INSERT INTO sale_payment (sale_transaction_id, amount, method, currency, note_id,
        created, last_edited, user_id) VALUES (@saleTransactionId, :amount, :method,
        :currency, :note_id, :created, :last_edited, :user_id));
END //

DELIMITER ;
COMMIT;
/*
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
    } */