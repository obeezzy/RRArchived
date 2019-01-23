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