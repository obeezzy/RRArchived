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

--

DELIMITER //
CREATE PROCEDURE UpdateNote(
    IN iNote VARCHAR(200),
    IN iTableName VARCHAR(20),
    IN iUserId INTEGER
    )
BEGIN
    UPDATE note SET note = iNote, table_name = iTableName, last_edited = CURRENT_TIMESTAMP(), user_id = iUserId;
	SELECT LAST_INSERT_ID();
END //
DELIMITER ;