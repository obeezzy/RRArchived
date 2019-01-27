USE ###DATABASENAME###;

---

CREATE PROCEDURE AddSqlUser (
    IN iUser VARCHAR(100),
    IN iPassword VARCHAR(100)
    )
BEGIN
    DECLARE _HOST CHAR(14) DEFAULT '@\'localhost\'';
    SET iUser := CONCAT('\'', REPLACE(TRIM(iUser), CHAR(39), CONCAT(CHAR(92), CHAR(39))), '\''),
        iPassword := CONCAT('\'', REPLACE(iPassword, CHAR(39), CONCAT(CHAR(92), CHAR(39))), '\'');
    SET @sql := CONCAT('CREATE USER ', iUser, _HOST, ' IDENTIFIED BY ', iPassword);
    PREPARE stmt FROM @sql;
    EXECUTE stmt;
    SET @sql := CONCAT('GRANT ALL PRIVILEGES ON *.* TO ', iUser, _HOST);
    PREPARE stmt FROM @sql;
    EXECUTE stmt;
    DEALLOCATE PREPARE stmt;
    FLUSH PRIVILEGES;
END;

---

CREATE PROCEDURE AddRRUser (
    IN iUser VARCHAR(100),
    IN iFirstName VARCHAR(100),
    IN iLastName VARCHAR(100),
    IN iPhoto BLOB,
    IN iPhoneNumber VARCHAR(100),
    IN iEmailAddress VARCHAR(100),
    IN iNoteId INTEGER,
    IN iUserId INTEGER
)
BEGIN
    INSERT INTO user (user, first_name, last_name, photo, phone_number, email_address, note_id, created, last_edited, user_id)
        VALUES (iUser, iFirstName, iLastName, iPhoto, iPhoneNumber, iEmailAddress, iNoteId, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);
    SELECT LAST_INSERT_ID() AS user_id;
END;

---

CREATE PROCEDURE ActivateUser (
    IN iUserId INTEGER,
    IN iActive BOOLEAN
)
BEGIN
    UPDATE user SET active = IFNULL(iActive, FALSE) WHERE id = iUserId;
END;

---

CREATE PROCEDURE GetUserDetails (
	iUserName VARCHAR(50)
)
BEGIN
	SELECT user.id AS user_id, user.user AS user_name, user_privilege.privileges AS user_privileges FROM user
		LEFT JOIN user_privilege ON user.id = user_privilege.user_id
        WHERE user = iUserName;
END

---

CREATE PROCEDURE ViewUsers (
    iArchived BOOLEAN
)
BEGIN
	SELECT id AS user_id, user, active FROM user WHERE archived = IFNULL(iArchived, FALSE) AND id > 1;
END;

---

CREATE PROCEDURE ViewUserPrivileges (
	IN iUserId INTEGER
)
BEGIN
	SELECT privileges AS user_privileges FROM user_privilege WHERE user_id = iUserId;
END;

---

CREATE PROCEDURE GrantPrivilege (
    IN iPrivilege VARCHAR(30)
)
BEGIN
    DECLARE _HOST CHAR(14) DEFAULT '@\'localhost\'';
    SET @user := CONCAT('\'', REPLACE(TRIM(iUser), CHAR(39), CONCAT(CHAR(92), CHAR(39))), '\'');
    SET @sql := CONCAT('GRANT ', iPrivilege, ' ON *.* TO ', @user, _HOST);
    PREPARE stmt FROM @sql;
    EXECUTE stmt;
    DEALLOCATE PREPARE stmt;
    FLUSH PRIVILEGES;
END;

---

CREATE PROCEDURE AddUserPrivileges (
    IN iPrivileges JSON,
    IN iUserId INTEGER
)
BEGIN
    INSERT INTO user_privilege (user_id, privileges, created, last_edited)
        VALUES (iUserId, iPrivileges, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP());
END;

---

CREATE PROCEDURE UpdateUserPrivileges (
    IN iPrivileges JSON,
    IN iUserId INTEGER
)
BEGIN
    UPDATE user_privilege SET privileges = iPrivileges WHERE user_id = iUserId;
END;

---

CREATE PROCEDURE ViewUserDetails (
    IN iUserId INTEGER,
    IN iArchived BOOLEAN
)
BEGIN
    SELECT first_name, last_name, user AS user_name, photo, phone_number, email_address, active, note
        FROM user
        LEFT JOIN note ON user.note_id = note.id
        WHERE user.id = iUserId AND user.archived = IFNULL(iArchived, FALSE);
END;