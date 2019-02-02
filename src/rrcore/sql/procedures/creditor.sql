
USE ###DATABASENAME###;

---

CREATE PROCEDURE AddCreditor (
	IN iClientId INTEGER,
    IN iNoteId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	INSERT INTO creditor (client_id, note_id, archived, created, last_edited, user_id)
		VALUES (iClientId, iNoteId, FALSE, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);
	SELECT LAST_INSERT_ID() AS id;
END;

---

CREATE PROCEDURE AddCreditPayment (
	IN iCreditTransactionId INTEGER,
    IN iTotalAmount DECIMAL(19,2),
    IN iAmountPaid DECIMAL(19,2),
    IN iBalance DECIMAL(19,2),
    IN iCurrency VARCHAR(4),
    IN iDueDate DATETIME,
    IN iNoteId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	INSERT INTO credit_payment (credit_transaction_id, total_amount, amount_paid, balance, currency, due_date, note_id, 
		archived, created, last_edited, user_id) VALUES (iCreditTransactionId, iTotalAmount, iAmountPaid, iBalance, 
		iCurrency, iDueDate, iNoteId, FALSE, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);
	SELECT LAST_INSERT_ID() AS id;
END;

---

CREATE PROCEDURE AddCreditTransaction (
	IN iCreditorId INTEGER,
    IN iTransactionTable VARCHAR(20),
    IN iTransactionId INTEGER,
    IN iNoteId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	INSERT INTO credit_transaction (creditor_id, transaction_table, transaction_id, note_id, archived, created, last_edited, user_id)
		VALUES (iCreditorId, iTransactionTable, iTransactionId, iNoteId, FALSE, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);

	SELECT LAST_INSERT_ID() AS id;
END;

---

CREATE PROCEDURE ArchiveCreditTransaction (
	IN iArchived BOOLEAN,
	IN iTransactionTable VARCHAR(40),
    IN iTransactionId INTEGER,
	IN iUserId INTEGER
)
BEGIN
	UPDATE credit_transaction SET archived = iArchived, last_edited = CURRENT_TIMESTAMP(), user_id = iUserId
		WHERE transaction_table = iTransactionTable AND transaction_id = iTransactionId;
END;