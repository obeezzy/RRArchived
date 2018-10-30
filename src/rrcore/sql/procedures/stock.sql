USE ###DATABASENAME###;

START TRANSACTION;

DELIMITER //

CREATE PROCEDURE ViewStockCategories(
    IN iSortOrder VARCHAR(4)
    )
BEGIN
    IF LOWER(iSortOrder) = "desc" THEN
        SELECT id, category FROM category ORDER BY LOWER(category) DESC;
    ELSE
        SELECT id, category FROM category ORDER BY LOWER(category) ASC;
    END IF;
END //

DELIMITER ;

COMMIT;