USE ###DATABASENAME###;

---

CREATE PROCEDURE AddDebtor (
	IN iClientId INTEGER,
    IN iNoteId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	INSERT INTO debtor (client_id, note_id, archived, created, last_edited, user_id)
		VALUES (iClientId, iNoteId, FALSE, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);
	SELECT LAST_INSERT_ID() AS id;
END;

---

CREATE PROCEDURE AddDebtPayment (
	IN iDebtTransactionId INTEGER,
    IN iTotalAmount DECIMAL(19,2),
    IN iAmountPaid DECIMAL(19,2),
    IN iBalance DECIMAL(19,2),
    IN iCurrency VARCHAR(4),
    IN iDueDate DATETIME,
    IN iNoteId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	INSERT INTO debt_payment (debt_transaction_id, total_amount, amount_paid, balance, currency, due_date, note_id, 
		archived, created, last_edited, user_id) VALUES (iDebtTransactionId, iTotalAmount, iAmountPaid, iBalance, 
		iCurrency, iDueDate, iNoteId, FALSE, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);
	SELECT LAST_INSERT_ID() AS id;
END;

---

CREATE PROCEDURE AddDebtTransaction (
	IN iDebtorId INTEGER,
    IN iTransactionTable VARCHAR(20),
    IN iTransactionId INTEGER,
    IN iNoteId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	INSERT INTO debt_transaction (debtor_id, transaction_table, transaction_id, note_id, archived, created, last_edited, user_id)
		VALUES (iDebtorId, iTransactionTable, iTransactionId, iNoteId, FALSE, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);

	SELECT LAST_INSERT_ID() AS id;
END;

---

CREATE PROCEDURE ArchiveDebtTransaction1 (
	IN iTransactionTable VARCHAR(40),
    IN iTransactionId INTEGER,
	IN iUserId INTEGER
)
BEGIN
	UPDATE debt_transaction SET archived = 1, last_edited = CURRENT_TIMESTAMP(), user_id = iUserId
		WHERE transaction_table = iTransactionTable AND transaction_id = iTransactionId;
END;

---

CREATE PROCEDURE TouchDebtTransaction (
	IN iDebtTransactionId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	UPDATE debt_transaction SET last_edited = CURRENT_TIMESTAMP(), user_id = iUserId
		WHERE id = iDebtTransactionId;
END;

---

CREATE PROCEDURE UpdateDebtPayment (
	IN iDebtPaymentId INTEGER,
    IN iTotalAmount DECIMAL(19,2),
    IN iAmountPaid DECIMAL(19,2),
    IN iBalance DECIMAL(19,2),
    IN iCurrency VARCHAR(4),
    IN iDueDate DATETIME,
    IN iUserId INTEGER
)
BEGIN
	UPDATE debt_payment SET total_amount = iTotalAmount, amount_paid = iAmountPaid,
		balance = iBalance, due_date = iDueDate, last_edited = CURRENT_TIMESTAMP(), user_id = iUserId
        WHERE id = iDebtPaymentId;
END;

---

CREATE PROCEDURE ArchiveDebtTransaction2 (
	IN iDebtTransactionId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	UPDATE debt_transaction SET archived = 1, last_edited = CURRENT_TIMESTAMP(), user_id = iUserId
		WHERE id = iDebtTransactionId;
END;

---

CREATE PROCEDURE ArchiveDebtPayment (
	IN iDebtPaymentId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	UPDATE debt_payment SET archived = 1, last_edited = CURRENT_TIMESTAMP(), user_id = iUserId
		WHERE id = iDebtPaymentId;
END;

---

CREATE PROCEDURE FilterDebtorsByName (
	IN iFilterText VARCHAR(100),
    IN iArchived BOOLEAN
)
BEGIN
	SELECT debtor.client_id, debtor.id AS debtor_id, client.preferred_name AS preferred_name,
		debt_payment.balance AS total_debt, debtor.archived FROM debt_payment
        INNER JOIN debt_transaction ON debt_transaction.id = debt_payment.debt_transaction_id
        INNER JOIN debtor ON debtor.id = debt_transaction.debtor_id
        INNER JOIN client ON client.id = debtor.client_id
        WHERE debt_transaction.archived = IFNULL(iArchived, NULL) AND client.preferred_name LIKE iFilterText
        GROUP BY debtor.id, debt_payment.debt_transaction_id, debt_payment.balance,
        debt_payment.last_edited
        HAVING MAX(debt_payment.last_edited) = debt_payment.last_edited
        AND archived = IFNULL(iArchived, NULL);
END;

---

CREATE PROCEDURE ArchiveDebtor (
	IN iDebtorId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	UPDATE debtor SET archived = 1, last_edited = CURRENT_TIMESTAMP(),
		user_id = iUserId WHERE id = iDebtorId;
END;

---

CREATE PROCEDURE ArchiveDebtTransaction3 (
	IN iDebtorId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	UPDATE debt_transaction SET archived = 1, last_edited = CURRENT_TIMESTAMP(),
		user_id = iUserId WHERE id = iDebtorId;
END;

---

CREATE PROCEDURE GetDebtTransaction (
	IN iDebtorId INTEGER
)
BEGIN
	SELECT id FROM debt_transaction WHERE debtor_id = iDebtorId;
END;

---

CREATE PROCEDURE UndoArchiveDebtor (
	IN iDebtorId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	UPDATE debtor SET archived = 0, last_edited = CURRENT_TIMESTAMP(),
		user_id = iUserId WHERE id = iDebtorId;
END;

---

CREATE PROCEDURE UndoArchiveDebtTransaction (
	IN iDebtTransactionId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	UPDATE debt_transaction SET archived = 0, last_edited = CURRENT_TIMESTAMP(),
		user_id = iUserId WHERE id = iDebtTransactionId;
END;

---

CREATE PROCEDURE ViewTotalBalanceForDebtor (
	IN iDebtorId INTEGER
)
BEGIN
	SELECT debtor.client_id, debtor.id AS debtor_id, client.preferred_name AS preferred_name,
		(SELECT SUM(debt_payment.balance) FROM debt_payment
		INNER JOIN debt_transaction ON debt_transaction.id = debt_payment.debt_transaction_id
		INNER JOIN debtor ON debt_transaction.debtor_id = debtor.id
		WHERE debt_payment.debt_transaction_id IN
		(SELECT debt_transaction.id FROM debt_transaction
		WHERE debt_transaction.debtor_id = debtor.id AND debt_transaction.archived = 0)
		AND debt_payment.archived = 0 ORDER BY debt_payment.last_edited DESC LIMIT 1) AS total_debt,
		note.note AS note, debtor.created, debtor.last_edited, debtor.user_id, user_.user
		FROM debtor
		INNER JOIN client ON client.id = debtor.client_id
		LEFT JOIN user_ ON user_.id = debtor.user_id
		LEFT JOIN note ON debtor.note_id = note.id
		WHERE debtor.archived = 0 AND debtor.id = iDebtorId;
END;

---

CREATE PROCEDURE ViewDebtorDetails (
	IN iDebtorId INTEGER,
    IN iArchived BOOLEAN
)
BEGIN
	SELECT debtor.id AS debtor_id, client.preferred_name AS preferred_name,
		client.first_name, client.last_name, client.phone_number, debtor.archived,
        debtor.user_id, debtor.user_id AS user
        FROM debtor
        INNER JOIN client ON client.id = debtor.client_id
        WHERE debtor.id = iDebtorId AND debtor.archived = iArchived;
END;

---

CREATE PROCEDURE ViewFewDebtorDetails (
	IN iDebtorId INTEGER,
    IN iArchived BOOLEAN
)
BEGIN
	SELECT client.id AS client_id, client.preferred_name, client.phone_number AS primary_phone_number,
		note.note FROM client
		INNER JOIN debtor ON debtor.client_id = client.id
		LEFT JOIN note ON note.id = debtor.note_id
		WHERE debtor.id = iDebtorId AND debtor.archived = iArchived;
END;

---

CREATE PROCEDURE ViewDebtTransactions (
	IN iDebtorId INTEGER,
    IN iArchived BOOLEAN
)
BEGIN
	SELECT debt_transaction.id AS debt_transaction_id, debt_transaction.transaction_table AS related_transaction_table,
		debt_transaction.transaction_id AS related_transaction_id, debt_transaction.created AS debt_transaction_created,
		debt_payment.id AS debt_payment_id, debt_payment.total_amount, debt_payment.amount_paid,
		debt_payment.balance AS balance, debt_payment.currency,
		debt_payment.due_date, debt_transaction.note_id AS debt_transaction_note_id,
		debt_payment.note_id AS debt_payment_note_id, debt_transaction.archived,
		debt_payment.created AS debt_payment_created
		FROM debt_payment
		INNER JOIN debt_transaction ON debt_transaction.id = debt_payment.debt_transaction_id
		INNER JOIN debtor ON debtor.id = debt_transaction.debtor_id
		LEFT JOIN note ON note.id = debt_transaction.note_id
		WHERE debtor.id = iDebtorId AND debt_transaction.archived = iArchived
		ORDER BY debt_payment.last_edited ASC;
END;

---

CREATE PROCEDURE ViewDebtors (
	IN iArchived INTEGER
)
BEGIN
	SELECT debtor.client_id, debtor.id AS debtor_id, client.preferred_name AS preferred_name,
		debt_payment.balance AS total_debt, debtor.archived,
        MAX(debt_payment.last_edited) FROM debt_payment
        INNER JOIN debt_transaction ON debt_transaction.id = debt_payment.debt_transaction_id
		INNER JOIN debtor ON debtor.id = debt_transaction.debtor_id
        INNER JOIN client ON client.id = debtor.client_id
		WHERE debt_transaction.archived = IFNULL(iArchived, FALSE)
        GROUP BY debtor.id, debt_payment.balance,
        debt_payment.last_edited, debt_transaction.archived
        HAVING MAX(debt_payment.last_edited) = debt_payment.last_edited
        AND debt_transaction.archived = IFNULL(iArchived, FALSE);
END;