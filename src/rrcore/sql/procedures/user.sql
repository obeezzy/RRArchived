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
    IN iPasswordHash VARCHAR(256),
    IN iFirstName VARCHAR(100),
    IN iLastName VARCHAR(100),
    IN iPhoto BLOB,
    IN iPhoneNumber VARCHAR(100),
    IN iEmailAddress VARCHAR(100),
    IN iNoteId INTEGER,
    IN iUserId INTEGER
)
BEGIN
    SET @archived := NULL;
    SELECT archived INTO @archived FROM user_ WHERE user = iUser AND archived = TRUE;
    IF @archived IS NOT NULL THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'This user was previously archived.';
    END IF;

    INSERT INTO user_ (user, password, first_name, last_name, photo, phone_number, email_address, note_id, created, last_edited, user_id)
        VALUES (iUser, iPasswordHash, iFirstName, iLastName, iPhoto, iPhoneNumber, iEmailAddress, iNoteId, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);
    SELECT LAST_INSERT_ID() AS user_id;
END;

---

CREATE PROCEDURE ActivateUser (
    IN iUser VARCHAR(50),
    IN iActive BOOLEAN
)
BEGIN
    DECLARE _HOST CHAR(14) DEFAULT '@\'localhost\'';
    UPDATE user_ SET active = IFNULL(iActive, FALSE) WHERE user_.user = iUser;
    SET iUser := CONCAT('\'', REPLACE(TRIM(iUser), CHAR(39), CONCAT(CHAR(92), CHAR(39))), '\'');

    SET @sql := NULL;
    IF iActive = TRUE THEN
        SET @sql := CONCAT('ALTER USER ', iUser, _HOST, ' ACCOUNT UNLOCK');
    ELSE
        SET @sql := CONCAT('ALTER USER ', iUser, _HOST, ' ACCOUNT LOCK');
    END IF;

    PREPARE stmt FROM @sql;
    EXECUTE stmt;
    DEALLOCATE PREPARE stmt;
    FLUSH PRIVILEGES;
END

---

CREATE PROCEDURE GetUserDetails (
	iUserName VARCHAR(50)
)
BEGIN
	SELECT user_.id AS user_id, user_.user AS user_name, user_privilege.privileges AS user_privileges FROM user_
		LEFT JOIN user_privilege ON user_.id = user_privilege.user_id
        WHERE user = iUserName;
END

---

CREATE PROCEDURE ViewUsers (
    iArchived BOOLEAN
)
BEGIN
	SELECT id AS user_id, user, active FROM user_ WHERE archived = IFNULL(iArchived, FALSE) AND id > 1;
END;

---

CREATE PROCEDURE ViewUserPrivileges (
	IN iUserId INTEGER
)
BEGIN
	SELECT privileges AS user_privileges FROM user_privilege WHERE user_id = iUserId;
END;

---

CREATE PROCEDURE GrantSqlPrivilege (
    IN iUser VARCHAR(50),
    IN iPrivilege VARCHAR(30)
)
BEGIN
    DECLARE _HOST CHAR(14) DEFAULT '@\'localhost\'';
    SET iUser := CONCAT('\'', REPLACE(TRIM(iUser), CHAR(39), CONCAT(CHAR(92), CHAR(39))), '\'');
    SET @sql := CONCAT('GRANT ', iPrivilege, ' ON *.* TO ', iUser, _HOST);
    PREPARE stmt FROM @sql;
    EXECUTE stmt;
    DEALLOCATE PREPARE stmt;
    FLUSH PRIVILEGES;
END;

---

CREATE PROCEDURE RevokeSqlPrivilege (
    IN iUser VARCHAR(50),
    IN iPrivilege VARCHAR(30)
)
BEGIN
    DECLARE _HOST CHAR(14) DEFAULT '@\'localhost\'';
    SET iUser := CONCAT('\'', REPLACE(TRIM(iUser), CHAR(39), CONCAT(CHAR(92), CHAR(39))), '\'');
    SET @sql := CONCAT('REVOKE ', iPrivilege, ' ON *.* TO ', iUser, _HOST);
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
        FROM user_
        LEFT JOIN note ON user_.note_id = note.id
        WHERE user_.id = iUserId AND user_.archived = IFNULL(iArchived, FALSE);
END;

---

CREATE PROCEDURE ChangePassword (
    IN iUser VARCHAR(40),
    IN iOldPasswordHash VARCHAR(256),
    IN iNewPassword VARCHAR(256),
    IN iNewPasswordHash VARCHAR(256)
)
BEGIN
    DECLARE _HOST CHAR(14) DEFAULT '@\'localhost\'';

    SET @password := NULL;
    SELECT password INTO @password FROM user_ WHERE user = iUser AND password = iOldPasswordHash;
    IF @password IS NULL THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Old password does not match current password.';
    END IF;

    UPDATE user_ SET password = iNewPasswordHash WHERE user = iUser AND password = iOldPasswordHash;

	SET iUser := CONCAT('\'', REPLACE(TRIM(iUser), CHAR(39), CONCAT(CHAR(92), CHAR(39))), '\''),
        iNewPassword := CONCAT('\'', REPLACE(iNewPassword, CHAR(39), CONCAT(CHAR(92), CHAR(39))), '\'');
    SET @sql := CONCAT('ALTER USER ', iUser, _HOST, ' IDENTIFIED BY ', iNewPassword);
    PREPARE stmt FROM @sql;
    EXECUTE stmt;
    DEALLOCATE PREPARE stmt;
    FLUSH PRIVILEGES;
END;

---

CREATE PROCEDURE RemoveUser (
    iUser VARCHAR(40)
)
BEGIN
    DECLARE _HOST CHAR(14) DEFAULT '@\'localhost\'';

    UPDATE user_ SET archived = TRUE WHERE user_.user = iUser;

	SET iUser := CONCAT('\'', REPLACE(TRIM(iUser), CHAR(39), CONCAT(CHAR(92), CHAR(39))), '\'');
    SET @sql := CONCAT('DROP USER ', iUser, _HOST);
    PREPARE stmt FROM @sql;
    EXECUTE stmt;
    DEALLOCATE PREPARE stmt;
    FLUSH PRIVILEGES;
END;