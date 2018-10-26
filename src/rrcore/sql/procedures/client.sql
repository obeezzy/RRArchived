USE ###DATABASENAME###;

START TRANSACTION;

DELIMITER //
CREATE PROCEDURE ViewClients(
    IN archived TINYINT,
    OUT rClientId INTEGER,
    OUT rPreferredNmae VARCHAR(100),
    OUT rPhoneNumber VARCHAR(100)
    )
BEGIN

SELECT id, preferred_name, phone_number
INTO rClientId, rPreferredNmae, rPhoneNumber FROM client
WHERE client.archived = archived;

END //
DELIMITER ;

COMMIT;