USE ###DATABASENAME###;

---

CREATE PROCEDURE AddExpenseTransaction (
    IN iClientId INTEGER,
    IN iName VARCHAR(50),
    IN iPurpose VARCHAR(200),
    IN iAmount DECIMAL(19,2),
    IN iPaymentMethod VARCHAR(20),
    IN iCurrency VARCHAR(4),
    IN iNoteId INTEGER,
    IN iUserId INTEGER
    )
BEGIN
    INSERT INTO expense (name, client_id, purpose, amount, payment_method,
		currency, note_id, archived, created, last_edited, user_id)
        VALUES (iName, iClientId, iPurpose, iAmount, iPaymentMethod, iCurrency,
			iNoteId, FALSE, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);
	SELECT LAST_INSERT_ID();
END;

---

CREATE PROCEDURE ViewExpenseTransactions (
	IN iFrom DATETIME,
    IN iTo DATETIME,
    IN iArchived BOOLEAN
)
BEGIN
	SELECT id AS transaction_id, client_id, name AS client_name, amount FROM expense
		WHERE created BETWEEN iFrom AND iTo AND archived = iArchived;
END

---

CREATE PROCEDURE ViewExpenseReport (
    IN iFrom DATETIME,
    IN iTo DATETIME,
    IN iFilterColumn VARCHAR(20),
    IN iFilterText VARCHAR(100),
    IN iSortColumn VARCHAR(20),
    IN iSortOrder VARCHAR(15)
)
BEGIN
    SELECT id AS expense_id, purpose, amount FROM expense
        WHERE created BETWEEN IFNULL(iFrom, '1970-01-01 00:00:00')
                                    AND IFNULL(iTo, CURRENT_TIMESTAMP())
        AND expense.archived = FALSE
        AND purpose LIKE (CASE
                            WHEN LOWER(iFilterColumn) = 'purpose'
                            THEN CONCAT('%', iFilterText, '%')
                            ELSE '%'
                            END)
        ORDER BY (CASE
                    WHEN LOWER(iSortOrder) = 'descending' AND LOWER(iSortColumn) = 'purpose'
                    THEN LOWER(expense.purpose) END) DESC,
                 (CASE
                    WHEN (iSortOrder IS NULL AND iSortColumn IS NULL) OR (LOWER(iSortOrder) <> 'descending' AND LOWER(iSortColumn) = 'purpose')
                    THEN LOWER(expense.purpose) END) ASC,
        LOWER(expense.purpose) ASC;
END;