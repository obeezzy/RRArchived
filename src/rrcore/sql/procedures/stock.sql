USE ###DATABASENAME###;

DELIMITER //
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
DELIMITER ;

--

DELIMITER //
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
END //
DELIMITER ;

--

DELIMITER //
CREATE PROCEDURE DeductStockQuantity(
	IN iItemId INTEGER,
	IN iQuantity DOUBLE,
    IN iUnitId INTEGER,
    IN iReason VARCHAR(200),
    IN iUserId INTEGER,
    OUT oInitialQuantityId INTEGER
)
BEGIN
	SET @availableQuantity := 0.0;
	SELECT quantity INTO @availableQuantity FROM current_quantity WHERE item_id = iItemId;

    IF @availableQuantity < iQuantity THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Available quantity is too low to make deduction.';
    END IF;

    INSERT INTO initial_quantity (item_id, quantity, unit_id, reason, archived, created, last_edited, user_id)
		VALUES (iItemId, @availableQuantity, iUnitId, iReason, FALSE, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);

    SELECT LAST_INSERT_ID() INTO oInitialQuantityId;

    UPDATE current_quantity SET quantity = @availableQuantity - iQuantity, last_edited = CURRENT_TIMESTAMP(), user_id = iUserId
		WHERE item_id = iItemId;
END //
DELIMITER ;

--

DELIMITER //
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
END //
DELIMITER ;