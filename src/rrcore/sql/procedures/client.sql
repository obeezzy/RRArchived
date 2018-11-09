USE ###DATABASENAME###;

#START TRANSACTION;
#DELIMITER //

CREATE PROCEDURE ViewClients(
    IN iFilterColumn VARCHAR(100),
    IN iFilterText VARCHAR(100),
    IN iArchived TINYINT,
    OUT oClientId INTEGER,
    OUT oPreferredName VARCHAR(100),
    OUT oPhoneNumber VARCHAR(100)
    )
BEGIN
    IF LOWER(iFilterColumn) = 'preferred_name' THEN
        SELECT id, preferred_name, phone_number
        INTO oClientId, oPreferredName, oPhoneNumber FROM client
        WHERE client.archived = iArchived AND client.preferred_name LIKE CONCAT('%', iFilterText, '%');
    ELSEIF LOWER(iFilterColumn) = 'phone_number' THEN
        SELECT id, preferred_name, phone_number
        INTO oClientId, oPreferredName, oPhoneNumber FROM client
        WHERE client.archived = iArchived AND client.phone_number LIKE CONCAT('%', iFilterText, '%');
    ELSE
        SELECT id, preferred_name, phone_number
        INTO oClientId, oPreferredName, oPhoneNumber FROM client
        WHERE client.archived = iArchived;
   END IF;
END;#//

#DELIMITER ;
#COMMIT;

--

START TRANSACTION;
DELIMITER //

DROP PROCEDURE IF EXISTS AddClient;
CREATE PROCEDURE AddClient(
    IN iCustomerName VARCHAR(100),
    IN iCustomerPhoneNumber VARCHAR(20),
    IN iAlternatePhoneNumber VARCHAR(20),
    IN iAddress VARCHAR(100),
    IN iNote VARCHAR(100),
    IN iUserId INTEGER
    )
this_proc:BEGIN
    IF iCustomerName IS NULL OR iCustomerPhoneNumber IS NULL THEN
        LEAVE this_proc;
    END IF;

    SET @clientId = 0;

    SAVEPOINT beginning;

    INSERT INTO note (note, table_name, created, last_edited, user_id) VALUES (iNote, 'client', CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);
    
    INSERT IGNORE INTO client (preferred_name, phone_number, archived, created, last_edited, user_id)
        VALUES (iCustomerName, iCustomerPhoneNumber, FALSE, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);

    SELECT LAST_INSERT_ID() INTO @clientId;

    IF @clientId = 0 THEN
        SELECT MAX(id) INTO @clientId FROM client LIMIT 1;
    END IF;

    IF @clientId = 0 THEN
        ROLLBACK TO beginning;
    ELSE IF;

    SELECT @clientId AS client_id;
END //

DELIMITER ;
COMMIT;
