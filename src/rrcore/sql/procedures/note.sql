USE ###DATABASENAME###;

DELIMITER //
CREATE PROCEDURE AddNote(
    IN iNote VARCHAR(200),
    IN iTableName VARCHAR(20),
    IN iUserId INTEGER
    )
BEGIN
    INSERT INTO note (note, table_name, created, last_edited, user_id)
        VALUES (iNote, iTableName, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);
	SELECT LAST_INSERT_ID();
END //
DELIMITER ;