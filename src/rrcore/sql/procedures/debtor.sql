USE ###DATABASENAME###;

DELIMITER //
CREATE PROCEDURE AddDebtor (
	IN iClientId INTEGER,
    IN iNoteId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	INSERT INTO debtor (client_id, note_id, archived, created, last_edited, user_id)
		VALUES (iClientId, iNoteId, FALSE, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);
	SELECT LAST_INSERT_ID() AS id;
END //
DELIMITER ;

--

DELIMITER //
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
END //
DELIMITER ;

--

DELIMITER //
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
END //
DELIMITER ;

--

DELIMITER //
CREATE PROCEDURE ArchiveDebtTransaction (
	IN iTransactionTable VARCHAR(40),
    IN iTransactionId INTEGER,
	IN iUserId INTEGER
)
BEGIN
	UPDATE debt_transaction SET archived = 1, last_edited = CURRENT_TIMESTAMP(), user_id = iUserId
		WHERE transaction_table = iTransactionTable AND transaction_id = iTransactionId;
END //
DELIMITER ;

--

DELIMITER //
CREATE PROCEDURE TouchDebtTransaction (
	IN iDebtTransactionId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	UPDATE debt_transaction SET last_edited = CURRENT_TIMESTAMP(), user_id = iUserId
		WHERE id = iDebtTransactionId;
END //
DELIMITER ;

--

DELIMITER //
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
END //
DELIMITER ;

--

DELIMITER //
CREATE PROCEDURE ArchiveDebtTransaction (
	IN iDebtTransactionId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	UPDATE debt_transaction SET archived = 1, last_edited = CURRENT_TIMESTAMP(), user_id = iUserId
		WHERE id = iDebtTransactionId;
END //
DELIMITER ;

--

DELIMITER //
CREATE PROCEDURE ArchiveDebtPayment (
	IN iDebtPaymentId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	UPDATE debt_payment SET archived = 1, last_edited = CURRENT_TIMESTAMP(), user_id = iUserId
		WHERE id = iDebtPaymentId;
END //
DELIMITER ;

--

DELIMITER //
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
END //
DELIMITER ;

--

DELIMITER //
CREATE PROCEDURE ArchiveDebtor (
	IN iDebtorId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	UPDATE debtor SET archived = 1, last_edited = CURRENT_TIMESTAMP(),
		user_id = iUserId WHERE id = iDebtorId;
END //
DELIMITER ;

--

DELIMITER //
CREATE PROCEDURE ArchiveDebtTransaction (
	IN iDebtorId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	UPDATE debt_transaction SET archived = 1, last_edited = CURRENT_TIMESTAMP(),
		user_id = iUserId WHERE id = iDebtorId;
END //
DELIMITER ;

--

DELIMITER //
CREATE PROCEDURE GetDebtTransaction (
	IN iDebtorId INTEGER
)
BEGIN
	SELECT id FROM debt_transaction WHERE debtor_id = iDebtorId;
END //
DELIMITER ;

--

DELIMITER //
CREATE PROCEDURE UndoArchiveDebtor (
	IN iDebtorId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	UPDATE debtor SET archived = 0, last_edited = CURRENT_TIMESTAMP(),
		user_id = iUserId WHERE id = iDebtorId;
END //
DELIMITER ;

--

DELIMITER //
CREATE PROCEDURE UndoArchiveDebtTransaction (
	IN iDebtTransactionId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	UPDATE debt_transaction SET archived = 0, last_edited = CURRENT_TIMESTAMP(),
		user_id = iUserId WHERE id = iDebtTransactionId;
END //
DELIMITER ;

--

DELIMITER //
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
		note.note AS note, debtor.created, debtor.last_edited, debtor.user_id, user.user
		FROM debtor
		INNER JOIN client ON client.id = debtor.client_id
		LEFT JOIN user ON user.id = debtor.user_id
		LEFT JOIN note ON debtor.note_id = note.id
		WHERE debtor.archived = 0 AND debtor.id = iDebtorId;
END //
DELIMITER ;