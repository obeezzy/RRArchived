USE ###DATABASENAME###;

---

CREATE PROCEDURE AddNote(
    IN iNote VARCHAR(200),
    IN iTableName VARCHAR(20),
    IN iUserId INTEGER
    )
BEGIN
    INSERT INTO note (note, table_name, created, last_edited, user_id)
        VALUES (iNote, iTableName, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);
	SELECT LAST_INSERT_ID();
END;

---

CREATE PROCEDURE UpdateNote (
	IN iNoteId INTEGER,
    IN iNote VARCHAR(200),
    IN iTableName VARCHAR(20),
    IN iUserId INTEGER
    )
BEGIN
    UPDATE note SET note = iNote, table_name = iTableName, last_edited = CURRENT_TIMESTAMP(), user_id = iUserId
		WHERE id = iNoteId;
END;