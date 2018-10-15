START TRANSACTION;

DROP PROCEDURE IF EXISTS ViewClients;
DELIMITER //
CREATE PROCEDURE ViewClients(
    IN archived TINYINT)
BEGIN

SELECT client.id AS client_id, client.preferred_name AS preferred_name,
client.phone_number AS phone_number FROM client
WHERE client.archived = archived;

END //
DELIMITER ;

COMMIT;