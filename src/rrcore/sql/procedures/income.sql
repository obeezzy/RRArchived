USE ###DATABASENAME###;

---

CREATE PROCEDURE AddIncomeTransaction (
    IN iClientId INTEGER,
    IN iName VARCHAR(50),
    IN iPurpose VARCHAR(200),
    IN iAmountPaid DECIMAL(19,2),
    IN iPaymentMethod VARCHAR(20),
    IN iCurrency VARCHAR(4),
    IN iNoteId INTEGER,
    IN iUserId INTEGER
    )
BEGIN
    INSERT INTO income (name, client_id, purpose, amount_paid, payment_method,
		currency, note_id, archived, created, last_edited, user_id)
        VALUES (iName, iClientId, iPurpose, iAmountPaid, iPaymentMethod, iCurrency,
			iNoteId, FALSE, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);
	SELECT LAST_INSERT_ID();
END;