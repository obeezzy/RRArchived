USE ###DATABASENAME###;

---

CREATE PROCEDURE ViewStockCategories(
    IN iSortOrder VARCHAR(4)
    )
BEGIN
    IF LOWER(iSortOrder) = "descending" THEN
        SELECT id, category FROM category ORDER BY LOWER(category) DESC;
    ELSE
        SELECT id, category FROM category ORDER BY LOWER(category) ASC;
    END IF;
END;

---

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
END;

---

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
END;

---

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
END;

---

CREATE PROCEDURE AddStockCategory (
	IN iCategory VARCHAR(100),
    IN iShortForm VARCHAR(10),
    IN iNoteId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	INSERT IGNORE INTO category (category, short_form, note_id, archived, created, last_edited, user_id)
		VALUES (iCategory, iShortForm, iNoteId, FALSE, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);

	IF LAST_INSERT_ID() > 0 THEN
		SELECT LAST_INSERT_ID() AS id;
	ELSE
		SELECT id FROM category WHERE category = iCategory;
    END IF;
END;

---

CREATE PROCEDURE AddStockItem (
	IN iCategoryId INTEGER,
    IN iItem VARCHAR(200),
    IN iShortForm VARCHAR(10),
    IN iDescription VARCHAR(200),
    IN iBarcode VARCHAR(50),
    IN iDivisible BOOLEAN,
    IN iImage BLOB,
    IN iNoteId INTEGER,
    IN iUserId INTEGER
)
BEGIN
    SET @itemAlreadyExists := NULL;
    SELECT id INTO @itemAlreadyExists FROM item WHERE LOWER(item) = LOWER(iItem) AND archived = FALSE;
    IF @itemAlreadyExists IS NOT NULL THEN
        SIGNAL SQLSTATE '80001'
            SET MESSAGE_TEXT = 'Product already exists.';
    END IF;

	INSERT INTO item (category_id, item, short_form, description, barcode, divisible, image,
		note_id, archived, created, last_edited, user_id)
		VALUES (iCategoryId, iItem, iShortForm, iDescription, iBarcode, iDivisible, iImage,
		iNoteId, FALSE, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);

	    IF LAST_INSERT_ID() > 0 THEN
		    SELECT LAST_INSERT_ID() AS id;
	    ELSE
		    SELECT id FROM item WHERE item = iItem;
        END IF;
END;

---

CREATE PROCEDURE AddStockUnit (
	IN iItemId INTEGER,
    IN iUnit VARCHAR(100),
    IN iShortForm VARCHAR(10),
    IN iBaseUnitEquivalent INTEGER,
    IN iCostPrice DECIMAL(19,2),
    IN iRetailPrice DECIMAL(19,2),
    IN iPreferred BOOLEAN,
    IN iCurrency VARCHAR(4),
    IN iNoteId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	INSERT INTO unit (item_id, unit, short_form, base_unit_equivalent, cost_price, retail_price, preferred, currency, note_id,
		archived, created, last_edited, user_id)
		VALUES (iItemId, iUnit, iShortForm, iBaseUnitEquivalent, iCostPrice, iRetailPrice, iPreferred, iCurrency, iNoteId,
		FALSE, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);

	IF LAST_INSERT_ID() > 0 THEN
		SELECT LAST_INSERT_ID() AS id;
	ELSE
		SELECT id FROM unit WHERE unit = iUnit;
    END IF;
END;

---

CREATE PROCEDURE AddInitialQuantity (
	IN iItemId INTEGER,
    IN iQuantity DOUBLE,
    IN iUnitId INTEGER,
    IN iReason VARCHAR(20),
    IN iUserId INTEGER
)
BEGIN
	INSERT INTO initial_quantity (item_id, quantity, unit_id, reason, archived, created, last_edited, user_id)
		VALUES (iItemId, iQuantity, iUnitId, iReason, FALSE, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);
END;

---

CREATE PROCEDURE AddCurrentQuantity (
	IN iItemId INTEGER,
    IN iQuantity DOUBLE,
    IN iUnitId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	INSERT INTO current_quantity (item_id, quantity, unit_id, created, last_edited, user_id)
		VALUES (iItemId, iQuantity, iUnitId, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), iUserId);
END;

---

CREATE PROCEDURE GetStockCategoryId (
	IN iItemId INTEGER
)
BEGIN
	SELECT (SELECT category_id FROM item WHERE item.id = iItemId) AS category_id;
END;

---

CREATE PROCEDURE UpdateStockItem (
	IN iCategoryId INTEGER,
    IN iItemId INTEGER,
    IN iItem VARCHAR(100),
    IN iShortForm VARCHAR(10),
    IN iDescription VARCHAR(200),
    IN iBarcode VARCHAR(50),
    IN iDivisible BOOLEAN,
    IN iImage BLOB,
    IN iNoteId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	UPDATE item SET category_id = iCategoryId, item = iItem, short_form = iShortForm, description = iDescription,
		barcode = iBarcode, divisible = iDivisible, image = iImage,
        note_id = (CASE WHEN iNoteId IS NOT NULL THEN iNoteId ELSE note_id END), archived = FALSE,
		last_edited = CURRENT_TIMESTAMP(), user_id = iUserId
		WHERE item.id = iItemId;
END;

---

CREATE PROCEDURE UpdateStockUnit (
	IN iItemId INTEGER,
	IN iUnit VARCHAR(100),
    IN iShortForm VARCHAR(10),
    IN iBaseUnitEquivalent INTEGER,
    IN iCostPrice DECIMAL(19,2),
    IN iRetailPrice DECIMAL(19,2),
    IN iPreferred BOOLEAN,
    IN iCurrency VARCHAR(4),
    IN iNoteId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	UPDATE unit SET unit = iUnit, short_form = iShortForm,
		base_unit_equivalent = iBaseUnitEquivalent, cost_price = iCostPrice, retail_price = iRetailPrice,
        preferred = iPreferred, currency = iCurrency, note_id = iNoteId, archived = FALSE,
        last_edited = CURRENT_TIMESTAMP(), user_id = iUserId WHERE item_id = iItemId;
END;

---

CREATE PROCEDURE ArchiveStockItem(
	IN iItemId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	UPDATE item SET archived = 1, last_edited = CURRENT_TIMESTAMP(), user_id = iUserId WHERE id = iItemId;
	SELECT category.id as category_id, item.id as item_id FROM item 
		INNER JOIN category ON item.category_id = category.id
		WHERE item.id = iItemId;
END;

---

CREATE PROCEDURE UndoArchiveStockItem(
	IN iItemId INTEGER,
    IN iUserId INTEGER
)
BEGIN
	UPDATE item SET archived = 0, last_edited = CURRENT_TIMESTAMP(), user_id = iUserId WHERE id = iItemId;
	SELECT item.id AS item_id, category.id AS category_id, category.category, item.item, item.description,
		item.divisible, item.image, current_quantity.quantity,
		unit.id as unit_id, unit.unit, unit.cost_price,
		unit.retail_price, unit.currency, item.created, item.last_edited, item.user_id, item.user_id AS user
		FROM item
		INNER JOIN category ON item.category_id = category.id
		INNER JOIN unit ON item.id = unit.item_id
		INNER JOIN current_quantity ON item.id = current_quantity.item_id
		LEFT JOIN user ON item.user_id = iUserId
		WHERE item.archived = 0 AND unit.base_unit_equivalent = 1
		AND item.id = iItemId;
END;