USE ###DATABASENAME###;

#START TRANSACTION;

#DELIMITER //

CREATE PROCEDURE ViewClients(
    IN iFilterColumn VARCHAR(100),
    IN iFilterText VARCHAR(100),
    IN iArchived TINYINT,
    OUT oClientId INTEGER,
    OUT oPreferredNmae VARCHAR(100),
    OUT oPhoneNumber VARCHAR(100)
    )
BEGIN
    IF LOWER(iFilterColumn) = "preferred_name" THEN
        SELECT id, preferred_name, phone_number
        INTO oClientId, oPreferredNmae, oPhoneNumber FROM client
        WHERE client.archived = iArchived AND client.preferred_name LIKE CONCAT('%', iFilterText, '%');
    ELSEIF LOWER(iFilterColumn) = "phone_number" THEN
        SELECT id, preferred_name, phone_number
        INTO oClientId, oPreferredNmae, oPhoneNumber FROM client
        WHERE client.archived = iArchived AND client.phone_number LIKE CONCAT('%', iFilterText, '%');
    ELSE
        SELECT id, preferred_name, phone_number
        INTO oClientId, oPreferredNmae, oPhoneNumber FROM client
        WHERE client.archived = iArchived;
   END IF;
END;#//

#DELIMITER ;

#COMMIT;
