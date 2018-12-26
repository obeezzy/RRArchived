USE ###DATABASENAME###;

DELIMITER //
CREATE PROCEDURE AddNote(
    IN iNote VARCHAR(200),
    IN iTableName VARCHAR(20),
    IN iTransactionId INTEGER,
    IN iUserId INTEGER
    )
BEGIN
    INSERT INTO note (note, table_name, transaction_id, created, last_edited, user_id)
        VALUES (iNote, iTableName, iTransactionId, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);
	SELECT LAST_INSERT_ID();
END //
DELIMITER ;