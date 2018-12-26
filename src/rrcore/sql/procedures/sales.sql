USE ###DATABASENAME###;

DELIMITER //
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
END //
DELIMITER ;

--
DELIMITER //
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
END //
DELIMITER ;

--

DELIMITER //
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
END //
DELIMITER ;

--

DELIMITER //
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
        AND sale_transaction.created BETWEEN iFrom AND iTo ORDER BY created ASC;
END //
DELIMITER ;