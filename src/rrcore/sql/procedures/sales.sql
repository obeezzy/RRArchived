USE ###DATABASENAME###;

---

CREATE PROCEDURE AddSaleItem(
	IN iSaleTransactionId INTEGER,
    IN iItemId INTEGER,
    IN iUnitId INTEGER,
    IN iUnitPrice DECIMAL(19,2),
    IN iQuantity DOUBLE,
    IN iCost DECIMAL(19,2),
    IN iDiscount DECIMAL(19,2),
    IN iCurrency VARCHAR(4),
    IN iUserId INTEGER
)
BEGIN
	INSERT INTO sale_item (sale_transaction_id, item_id, unit_id, unit_price, quantity, cost, discount, currency, archived,
		created, last_edited, user_id) VALUES (iSaleTransactionId, iItemId, iUnitId, iUnitPrice, iQuantity, iCost, iDiscount,
        iCurrency, FALSE, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);
END;

---

CREATE PROCEDURE AddSalePayment(
	IN iSaleTransactionId INTEGER,
    IN iAmount DECIMAL(19,2),
    IN iMethod VARCHAR(15),
    IN iCurrency VARCHAR(4),
    IN iNote VARCHAR(200),
    IN iUserId INTEGER
)
BEGIN
    INSERT INTO note (note, table_name, created, last_edited, user_id)
		VALUES (iNote, 'sale_payment', CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);

	SELECT LAST_INSERT_ID() INTO @noteId;

	INSERT INTO sale_payment (sale_transaction_id, amount, method, currency, note_id,
		created, last_edited, user_id) VALUES (iSaleTransactionId, iAmount, iMethod, iCurrency,
        @noteId, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);
END;

---

CREATE PROCEDURE AddSaleTransaction(
	IN iClientName VARCHAR(100),
    IN iClientId INTEGER,
    IN iBalance DECIMAL(19,2),
    IN iDiscount DECIMAL(19,2),
    IN iSuspended BOOLEAN,
    IN iNote VARCHAR(200),
    IN iUserId INTEGER
    )
BEGIN
    IF iNote IS NOT NULL THEN
		INSERT INTO note (note, table_name, created, last_edited, user_id)
			VALUES (iNote, 'sale_transaction', CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);
        
		SELECT LAST_INSERT_ID() INTO @noteId;
	END IF;

    INSERT INTO sale_transaction (name, client_id, balance, discount, suspended, note_id, archived,
		created, last_edited, user_id) VALUES (iClientName, iClientId, iBalance, iDiscount, iSuspended, @noteId, FALSE,
        CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);

    SELECT LAST_INSERT_ID() AS id;
END;

---

CREATE PROCEDURE ViewSaleTransactions(
	IN iSuspended BOOLEAN,
    IN iArchived BOOLEAN,
    IN iFrom DATETIME,
    IN iTo DATETIME
)
BEGIN
	SELECT sale_transaction.id as transaction_id, sale_transaction.name as customer_name,
		sale_transaction.client_id, balance, discount,
        suspended, note_id, note.note, sale_transaction.archived, sale_transaction.created,
        sale_transaction.last_edited, sale_transaction.user_id FROM sale_transaction
        LEFT JOIN note ON sale_transaction.note_id = note.id
        WHERE sale_transaction.suspended = iSuspended AND sale_transaction.archived = iArchived
        AND sale_transaction.created BETWEEN IFNULL(iFrom, '1970-01-01 00:00:00')
									 AND IFNULL(iTo, CURRENT_TIMESTAMP()) ORDER BY created ASC;
END;

---

CREATE PROCEDURE IsSaleTransactionSuspended(
	IN iTransactionId INTEGER
)
BEGIN
	SELECT suspended FROM sale_transaction WHERE archived = 0 AND id = iTransactionId;
END;

---

CREATE PROCEDURE ArchiveSaleTransaction(
	IN iSaleTransactionId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	UPDATE sale_transaction SET archived = 1, last_edited = CURRENT_TIMESTAMP(), user_id = iUserId WHERE id = iSaleTransactionId;
    UPDATE sale_item SET archived = 1, last_edited = CURRENT_TIMESTAMP(), user_id = iUserId WHERE sale_transaction_id = iSaleTransactionId;
END;

---

CREATE PROCEDURE ViewSaleCart(
	IN iTransactionId INTEGER,
    IN iSaleTransactionArchived BOOLEAN,
    IN iSaleItemArchived BOOLEAN
)
BEGIN
	SELECT sale_transaction.id as transaction_id, sale_transaction.name as customer_name, sale_transaction.client_id as client_id,
		client.phone_number as customer_phone_number,
		(SELECT SUM(cost) FROM sale_item WHERE sale_transaction_id = iTransactionId) AS total_cost, sale_transaction.suspended,
        sale_transaction.note_id, sale_transaction.created,
        sale_transaction.last_edited, sale_transaction.user_id, category.id as category_id, category.category,
        sale_item.item_id, item.item, sale_item.unit_price as unit_price,
        sale_item.quantity, current_quantity.quantity as available_quantity, unit.id as unit_id, unit.unit,
        unit.cost_price as cost_price, unit.retail_price as retail_price,
        sale_item.cost, sale_item.discount,
        sale_item.currency, note.note FROM (sale_item
        INNER JOIN sale_transaction ON sale_item.sale_transaction_id = sale_transaction.id
        INNER JOIN item ON sale_item.item_id = item.id
        INNER JOIN unit ON sale_item.item_id = unit.item_id
        INNER JOIN current_quantity ON sale_item.item_id = current_quantity.item_id)
        INNER JOIN category ON item.category_id = category.id
        LEFT JOIN client ON sale_transaction.client_id = client.id
        LEFT JOIN note ON sale_item.note_id = note.id
        WHERE sale_transaction.id = iTransactionId AND sale_transaction.archived = iSaleTransactionArchived
        AND sale_item.archived = iSaleItemArchived;
END;

---

CREATE PROCEDURE ViewSaleTransactionItems(
	IN iTransactionId INTEGER,
    IN iSuspended BOOLEAN,
    IN iArchived BOOLEAN
)
BEGIN
	SELECT category.id as category_id, category.category, sale_item.item_id, item.item,
		sale_item.unit_price, sale_item.quantity, sale_item.unit_id,
		unit.unit, sale_item.cost, sale_item.discount, sale_item.currency, sale_item.note_id, note.note,
        sale_item.archived, sale_item.created, sale_item.last_edited, sale_item.user_id, user_.user FROM sale_item
        INNER JOIN item ON sale_item.item_id = item.id
        INNER JOIN category ON category.id = item.category_id
        INNER JOIN unit ON sale_item.unit_id = unit.id
        INNER JOIN sale_transaction ON sale_transaction.id = sale_item.sale_transaction_id
		LEFT JOIN user_ ON sale_item.user_id = user_.id
        LEFT JOIN note ON sale_transaction.note_id = note.id
        WHERE sale_transaction_id = iTransactionId AND sale_transaction.suspended = iSuspended
        AND sale_transaction.archived = iArchived;
END;

---

CREATE PROCEDURE `RevertSaleQuantityUpdate` (
	IN iTransactionId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	UPDATE current_quantity
		INNER JOIN sale_item ON current_quantity.item_id = sale_item.item_id
		INNER JOIN sale_transaction ON sale_item.sale_transaction_id = sale_transaction.id
        SET current_quantity.quantity = current_quantity.quantity + sale_item.quantity,
        current_quantity.last_edited = CURRENT_TIMESTAMP(), current_quantity.user_id = iUserId
		WHERE sale_transaction.id = iTransactionId;
END;

---

CREATE PROCEDURE GetTotalRevenue (
	IN iFrom DATE,
    IN iTo DATE
)
BEGIN
	SELECT DATE(sale_transaction.created) AS created, SUM(sale_payment.amount) AS amount_paid FROM sale_transaction
		INNER JOIN sale_payment ON sale_payment.sale_transaction_id = sale_transaction.id
		WHERE sale_transaction.suspended = 0 AND sale_transaction.archived = 0
		AND DATE(sale_transaction.created) BETWEEN DATE(iFrom) AND DATE(iTo)
		GROUP BY DATE(sale_transaction.created);
END;

---

CREATE PROCEDURE GetMostSoldItems (
	IN iFrom DATETIME,
    IN iTo DATETIME,
    IN iLimit INTEGER
)
BEGIN
	SELECT category.id AS category_id, category.category, sale_item.item_id, item.item,
		SUM(sale_item.cost - sale_item.discount) AS total_revenue, SUM(sale_item.quantity) AS total_quantity
		FROM sale_item
		INNER JOIN item ON item.id = sale_item.item_id
		INNER JOIN category ON category.id = item.category_id
		WHERE sale_item.created BETWEEN iFrom AND iTo
		GROUP BY sale_item.item_id
		ORDER BY SUM(sale_item.quantity) DESC
		LIMIT iLimit;
END;