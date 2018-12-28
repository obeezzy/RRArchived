USE ###DATABASENAME###;

DELIMITER //
CREATE PROCEDURE CreateUser(
    IN iUserName VARCHAR(100),
    IN iPassword VARCHAR(100)
    )
BEGIN
    DECLARE `_HOST` CHAR(14) DEFAULT '@\'localhost\'';
    SET iUserName := CONCAT('\'', REPLACE(TRIM(iUserName), CHAR(39), CONCAT(CHAR(92), CHAR(39))), '\''),
    iPassword := CONCAT('\'', REPLACE(iPassword, CHAR(39), CONCAT(CHAR(92), CHAR(39))), '\'');
    SET @`sql` := CONCAT('CREATE USER ', iUserName, `_HOST`, ' IDENTIFIED BY ', iPassword);
    PREPARE `stmt` FROM @`sql`;
    EXECUTE `stmt`;
    SET @`sql` := CONCAT('GRANT ALL PRIVILEGES ON *.* TO ', iUserName, `_HOST`);
    PREPARE `stmt` FROM @`sql`;
    EXECUTE `stmt`;
    DEALLOCATE PREPARE `stmt`;
    FLUSH PRIVILEGES;
END //
DELIMITER ;