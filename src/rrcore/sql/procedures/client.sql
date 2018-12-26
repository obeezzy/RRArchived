USE ###DATABASENAME###;

DELIMITER //
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
END //
DELIMITER ;

--

DELIMITER //
CREATE PROCEDURE AddClient(
    IN iPreferredName VARCHAR(100),
    IN iPhoneNumber VARCHAR(20),
    IN iUserId INTEGER
    )
BEGIN
    INSERT INTO client (preferred_name, phone_number, archived, created, last_edited, user_id)
        SELECT * FROM (SELECT iPreferredName, iPhoneNumber, FALSE, CURRENT_TIMESTAMP() AS created, CURRENT_TIMESTAMP AS last_edited, iUserId) AS tmp
        WHERE NOT EXISTS (
            SELECT phone_number FROM client WHERE phone_number = iPhoneNumber
        ) LIMIT 1;
SELECT id FROM client WHERE phone_number = iPhoneNumber;
END //
DELIMITER ;