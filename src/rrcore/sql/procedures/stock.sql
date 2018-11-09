USE ###DATABASENAME###;

#START TRANSACTION;
#DELIMITER //

CREATE PROCEDURE ViewStockCategories(
    IN iSortOrder VARCHAR(4)
    )
BEGIN
    IF LOWER(iSortOrder) = "descending" THEN
        SELECT id, category FROM category ORDER BY LOWER(category) DESC;
    ELSE
        SELECT id, category FROM category ORDER BY LOWER(category) ASC;
    END IF;
END //

#DELIMITER ;
#COMMIT;

--
#DROP PROCEDURE viewStockItems;
#START TRANSACTION;
#DELIMITER //

CREATE PROCEDURE ViewStockItems(
    IN iFilterColumn VARCHAR(20),
    IN iFilterText VARCHAR(100),
    IN iSortColumn VARCHAR(20),
    IN iSortOrder VARCHAR(15)
    )
BEGIN
    SELECT item.id AS item_id, category.id AS category_id, category.category, item.item, item.description,
        item.divisible, item.image, current_quantity.quantity,
        unit.id as unit_id, unit.unit, unit.cost_price,
        unit.retail_price, unit.currency, item.created, item.last_edited, item.user_id, item.user_id AS user
        FROM item
        INNER JOIN category ON item.category_id = category.id
        INNER JOIN unit ON item.id = unit.item_id
        INNER JOIN current_quantity ON item.id = current_quantity.item_id
        LEFT JOIN user ON item.user_id = user.id
        WHERE item.archived = 0 AND unit.base_unit_equivalent = 1
        AND category.category LIKE (CASE
                                    WHEN LOWER(iFilterColumn) = 'category'
                                    THEN CONCAT('%', iFilterText, '%')
                                    ELSE '%'
                                    END)
        AND item.item LIKE (CASE
                            WHEN LOWER(iFilterColumn) = 'item'
                            THEN CONCAT('%', iFilterText, '%')
                            ELSE '%'
                            END)
        ORDER BY (CASE
                    WHEN LOWER(iSortOrder) = 'descending' AND LOWER(iSortColumn) = 'category'
                    THEN LOWER(category.category) END) DESC,
                 (CASE
                    WHEN (iSortOrder IS NULL AND iSortColumn IS NULL) OR (LOWER(iSortOrder) <> 'descending' AND LOWER(iSortColumn) = 'category')
                    THEN LOWER(category.category) END) ASC,
        LOWER(item.item) ASC;
END #//

#DELIMITER ;
#COMMIT;