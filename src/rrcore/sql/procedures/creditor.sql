
USE ###DATABASENAME###;

DELIMITER //
CREATE PROCEDURE AddCreditor(
	IN iClientId INTEGER,
    IN iNote VARCHAR(200),
    IN iUserId INTEGER
)
BEGIN
	SET @noteId := 0;
	IF iNote IS NOT NULL THEN
		INSERT INTO note (note, table_name, created, last_edited, user_id)
			VALUES (iNote, 'creditor', CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);
		SELECT LAST_INSERT_ID() INTO @noteId;
    END IF;
    
	INSERT INTO creditor (client_id, note_id, archived, created, last_edited, user_id)
		VALUES (iClientId, iNote, FALSE, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);
	SELECT LAST_INSERT_ID() AS id;
END //
DELIMITER ;

--

DELIMITER //
CREATE PROCEDURE AddCreditPayment(
	IN iCreditTransactionId INTEGER,
    IN iTotalAmount DECIMAL(19,2),
    IN iAmountPaid DECIMAL(19,2),
    IN iBalance DECIMAL(19,2),
    IN iCurrency VARCHAR(4),
    IN iDueDate DATETIME,
    IN iNote VARCHAR(200),
    IN iUserId INTEGER
)
BEGIN
	SET @noteId := 0;
	IF iNote IS NOT NULL THEN
		INSERT INTO note (note, table_name, created, last_edited, user_id)
			VALUES (iNote, 'credit_payment', CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);
		SELECT LAST_INSERT_ID() INTO @noteId;
    END IF;
    
	INSERT INTO credit_payment (credit_transaction_id, total_amount, amount_paid, balance, currency, due_date, note_id, 
		archived, created, last_edited, user_id) VALUES (iCreditTransactionId, iTotalAmount, iAmountPaid, iBalance, 
		iCurrency, iDueDate, @noteId, FALSE, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);
	SELECT LAST_INSERT_ID() AS id;
END //
DELIMITER ;

--

DELIMITER //
CREATE PROCEDURE AddCreditTransaction(
	IN iCreditorId INTEGER,
    IN iTransactionTable VARCHAR(20),
    IN iTransactionId INTEGER,
    IN iNote VARCHAR(200),
    IN iUserId INTEGER
)
BEGIN
	SET @noteId := 0;
    IF iNote IS NOT NULL THEN
		INSERT INTO note (note, table_name, transaction_id, created, last_edited, user_id)
			VALUES (iNote, 'credit_transaction', CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);
		SELECT LAST_INSERT_ID() INTO @noteId;
    END IF;

	INSERT INTO credit_transaction (creditor_id, transaction_table, transaction_id, note_id, archived, created, last_edited, user_id)
		VALUES (iCreditorId, iTransactionTable, iTransactionId, @noteId, FALSE, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);

	SELECT LAST_INSERT_ID() AS id;
END //
DELIMITER ;

--

DELIMITER //
CREATE PROCEDURE ArchiveCreditTransaction(
	IN iTransactionTable VARCHAR(40),
    IN iTransactionId INTEGER,
	IN iUserId INTEGER
)
BEGIN
	UPDATE credit_transaction SET archived = 1, last_edited = CURRENT_TIMESTAMP(), user_id = iUserId
		WHERE transaction_table = iTransactionTable AND transaction_id = iTransactionId;
END //
DELIMITER ;