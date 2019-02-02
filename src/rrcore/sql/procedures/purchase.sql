USE ###DATABASENAME###;

---

CREATE PROCEDURE ViewPurchaseTransactions (
	IN iSuspended BOOLEAN,
    IN iArchived BOOLEAN,
    IN iFrom DATETIME,
    IN iTo DATETIME
)
BEGIN
	SELECT purchase_transaction.id as transaction_id, purchase_transaction.name as customer_name,
		purchase_transaction.client_id, balance, discount,
        suspended, note_id, note.note, purchase_transaction.archived, purchase_transaction.created,
        purchase_transaction.last_edited, purchase_transaction.user_id FROM purchase_transaction
        LEFT JOIN note ON purchase_transaction.note_id = note.id
        WHERE purchase_transaction.suspended = iSuspended AND purchase_transaction.archived = iArchived
        AND purchase_transaction.created BETWEEN IFNULL(iFrom, '1970-01-01 00:00:00')
									 AND IFNULL(iTo, CURRENT_TIMESTAMP()) ORDER BY created ASC;
END;

---

CREATE PROCEDURE AddPurchaseTransaction (
	IN iClientName VARCHAR(100),
    IN iClientId INTEGER,
    IN iBalance DECIMAL(19,2),
    IN iDiscount DECIMAL(19,2),
    IN iSuspended BOOLEAN,
    IN iNoteId INTEGER,
    IN iUserId INTEGER
)
BEGIN
    INSERT INTO purchase_transaction (name, client_id, balance, discount, suspended, note_id, archived,
		created, last_edited, user_id) VALUES (iClientName, iClientId, iBalance, iDiscount, iSuspended, iNoteId, FALSE,
        CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);

    SELECT LAST_INSERT_ID() AS purchase_transaction_id;
END;

---

CREATE PROCEDURE AddPurchasePayment (
	IN iPurchaseTransactionId INTEGER,
    IN iAmount DECIMAL(19,2),
    IN iMethod VARCHAR(15),
    IN iCurrency VARCHAR(4),
    IN iNoteId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	INSERT INTO purchase_payment (purchase_transaction_id, amount, method, currency, note_id,
		created, last_edited, user_id) VALUES (iPurchaseTransactionId, iAmount, iMethod, iCurrency,
        iNoteId, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);
END;

---

CREATE PROCEDURE AddPurchaseItem (
	IN iPurchaseTransactionId INTEGER,
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
	INSERT INTO purchase_item (purchase_transaction_id, item_id, unit_id, unit_price, quantity, cost, discount, currency, archived,
		created, last_edited, user_id) VALUES (iPurchaseTransactionId, iItemId, iUnitId, iUnitPrice, iQuantity, iCost, iDiscount,
        iCurrency, FALSE, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);
END;

---

CREATE PROCEDURE IsPurchaseTransactionSuspended (
	IN iTransactionId INTEGER
)
BEGIN
	SELECT suspended FROM purchase_transaction WHERE archived = 0 AND id = iTransactionId;
END;

---

CREATE PROCEDURE RevertPurchaseQuantityUpdate (
	IN iTransactionId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	UPDATE current_quantity
		INNER JOIN purchase_item ON current_quantity.item_id = purchase_item.item_id
		INNER JOIN purchase_transaction ON purchase_item.purchase_transaction_id = purchase_transaction.id
        SET current_quantity.quantity = current_quantity.quantity - purchase_item.quantity,
        current_quantity.last_edited = CURRENT_TIMESTAMP(), current_quantity.user_id = iUserId
		WHERE purchase_transaction.id = iTransactionId;
END;

---

CREATE PROCEDURE ViewPurchaseTransactionItems (
	IN iTransactionId INTEGER,
    IN iSuspended BOOLEAN,
    IN iArchived BOOLEAN
)
BEGIN
	SELECT category.id as category_id, category.category, purchase_item.item_id, item.item,
		purchase_item.unit_price, purchase_item.quantity, purchase_item.unit_id,
		unit.unit, purchase_item.cost, purchase_item.discount, purchase_item.currency, purchase_item.note_id, note.note,
        purchase_item.archived, purchase_item.created, purchase_item.last_edited, purchase_item.user_id, user_.user FROM purchase_item
        INNER JOIN item ON purchase_item.item_id = item.id
        INNER JOIN category ON category.id = item.category_id
        INNER JOIN unit ON purchase_item.unit_id = unit.id
        INNER JOIN purchase_transaction ON purchase_transaction.id = purchase_item.purchase_transaction_id
		LEFT JOIN user_ ON purchase_item.user_id = user_.id
        LEFT JOIN note ON purchase_transaction.note_id = note.id
        WHERE purchase_transaction_id = iTransactionId AND purchase_transaction.suspended = iSuspended
        AND purchase_transaction.archived = iArchived;
END;

---

CREATE PROCEDURE ViewPurchaseCart (
	IN iTransactionId INTEGER,
    IN iPurchaseTransactionArchived BOOLEAN,
    IN iPurchaseItemArchived BOOLEAN
)
BEGIN
	SELECT purchase_transaction.id as transaction_id, purchase_transaction.name as customer_name, purchase_transaction.client_id as client_id,
		client.phone_number as customer_phone_number,
		(SELECT SUM(cost) FROM purchase_item WHERE purchase_transaction_id = iTransactionId) AS total_cost, purchase_transaction.suspended,
        purchase_transaction.note_id, purchase_transaction.created,
        purchase_transaction.last_edited, purchase_transaction.user_id, category.id as category_id, category.category,
        purchase_item.item_id, item.item, purchase_item.unit_price as unit_price,
        purchase_item.quantity, current_quantity.quantity as available_quantity, unit.id as unit_id, unit.unit,
        unit.cost_price as cost_price, unit.retail_price as retail_price,
        purchase_item.cost, purchase_item.discount,
        purchase_item.currency, note.note FROM (purchase_item
        INNER JOIN purchase_transaction ON purchase_item.purchase_transaction_id = purchase_transaction.id
        INNER JOIN item ON purchase_item.item_id = item.id
        INNER JOIN unit ON purchase_item.item_id = unit.item_id
        INNER JOIN current_quantity ON purchase_item.item_id = current_quantity.item_id)
        INNER JOIN category ON item.category_id = category.id
        LEFT JOIN client ON purchase_transaction.client_id = client.id
        LEFT JOIN note ON purchase_item.note_id = note.id
        WHERE purchase_transaction.id = iTransactionId AND purchase_transaction.archived = iPurchaseTransactionArchived
        AND purchase_item.archived = iPurchaseItemArchived;
END;

---

CREATE PROCEDURE ArchivePurchaseTransaction (
    IN iArchived BOOLEAN,
	IN iTransactionId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	UPDATE purchase_transaction SET archived = iArchived, last_edited = CURRENT_TIMESTAMP(), user_id = iUserId WHERE id = iTransactionId;
END;

---

CREATE PROCEDURE UndoRevertPurchaseQuantityUpdate (
	IN iTransactionId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	UPDATE current_quantity
		INNER JOIN purchase_item ON current_quantity.item_id = purchase_item.item_id
		INNER JOIN purchase_transaction ON purchase_item.purchase_transaction_id = purchase_transaction.id
        SET current_quantity.quantity = current_quantity.quantity + purchase_item.quantity,
        current_quantity.last_edited = CURRENT_TIMESTAMP(), current_quantity.user_id = iUserId
		WHERE purchase_transaction.id = iTransactionId;
END;