USE ###DATABASENAME###;

#START TRANSACTION;

#DELIMITER //

CREATE PROCEDURE ViewStockItemDetails(
    INOUT ioItemId INTEGER,
    OUT oCategoryId INTEGER,
    OUT oCategory VARCHAR(100),
    OUT oItem VARCHAR(100),
    OUT oDescription VARCHAR(100),
    OUT oDivisible TINYINT,
    OUT oImage BLOB,
    OUT oQuantity DOUBLE,
    OUT oUnitId INTEGER,
    OUT oUnit VARCHAR(100),
    OUT oCostPrice DECIMAL(19,2),
    OUT oRetailPrice DECIMAL(19,2),
    OUT oCurrency VARCHAR(4),
    OUT oCreated DATETIME,
    OUT oLastEdited DATETIME,
    OUT oUserId INTEGER,
    OUT oUser VARCHAR(100)
    )
BEGIN
    SELECT item.id AS item_id, category.id AS category_id, category.category, item.item, item.description,
        item.divisible, item.image, current_quantity.quantity,
        unit.id as unit_id, unit.unit, unit.cost_price,
        unit.retail_price, unit.currency, item.created, item.last_edited, item.user_id, 'user' AS user
        INTO ioItemId, oCategoryId, oCategory, oItem, oDescription, oDivisible, oImage, oQuantity, oUnitId,
        oUnit, oCostPrice, oRetailPrice, oCurrency, oCreated, oLastEdited, oUserId, oUser
        FROM item
        INNER JOIN category ON item.category_id = category.id
        INNER JOIN unit ON item.id = unit.item_id
        INNER JOIN current_quantity ON item.id = current_quantity.item_id
        LEFT JOIN user ON item.user_id = user.id
        WHERE item.archived = 0 AND unit.base_unit_equivalent = 1
        AND item.id = ioItemId;
END# //

#DELIMITER ;

#COMMIT;
