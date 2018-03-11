#include "stocksqlmanager.h"
#include <QSqlQuery>
#include <QDateTime>
#include <QSqlDriver>
#include <QImage>
#include <QByteArray>
#include <QBuffer>
#include <QSqlError>
#include <QVariant>
#include <QVariantList>
#include <QVariantMap>

#include "rr-core/singletons/userprofile.h"
#include "rr-core/database/databaseexception.h"
#include "rr-core/database/databaseutils.h"

StockSqlManager::StockSqlManager(QSqlDatabase connection)
    : AbstractSqlManager(connection)
{

}

QueryResult StockSqlManager::execute(const QueryRequest &request)
{
    QueryResult result(request);

    try {
        if (request.command() == "add_new_item")
            addNewItem(request);
        else if (request.command() == "view_stock_items")
            viewStockItems(request, result);
        else if (request.command() == "view_stock_categories")
            viewStockCategories(request, result);

        result.setSuccessful(true);
    } catch (DatabaseException &e) {
        result.setSuccessful(false);
        result.setErrorCode(e.code());
        result.setErrorMessage(e.message());
        result.setErrorUserMessage(e.userMessage());
    }

    return result;
}

void StockSqlManager::addNewItem(const QueryRequest &request)
{
    const QVariantMap params = request.params();
    const QDateTime currentDateTime = QDateTime::currentDateTime();
    int categoryNoteId = 0;
    int itemNoteId = 0;
    int categoryId = 0;
    int itemId = 0;
    int unitId = 0;

    QSqlQuery q(connection());

    try {
        if (!DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::BeginTransactionFailed, q.lastError().text(), "Failed to start transation.");

        // Insert category note
        if (!params.value("category_note").toString().trimmed().isEmpty()) {
            q.prepare("INSERT INTO note (note, created, last_edited, user_id) VALUES (:note, :created, :last_edited, :user_id)");
            q.bindValue(":note", params.value("category_note").toString());
            q.bindValue(":created", currentDateTime);
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::AddItemFailure, q.lastError().text(), "Failed to insert category note.");

            categoryNoteId = q.lastInsertId().toInt();

            if (!categoryNoteId)
                throw DatabaseException(DatabaseException::AddItemFailure, q.lastError().text(), "Invalid category note ID returned.");
        }

        // Insert item note
        if (!params.value("item_note").toString().trimmed().isEmpty()) {
            q.prepare("INSERT INTO note (note, created, last_edited, user_id) VALUES (:note, :archived, :created, :last_edited, :user_id)");
            q.bindValue(":note", params.value("item_note").toString());
            q.bindValue(":created", currentDateTime);
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::AddItemFailure, q.lastError().text(), "Failed to insert item note.");

            itemNoteId = q.lastInsertId().toInt();
            if (!itemNoteId)
                throw DatabaseException(DatabaseException::AddItemFailure, q.lastError().text(), "Invalid item note ID returned.");
        }


        // Insert category
        q.prepare("INSERT INTO category (category, short_form, note_id, archived, created, last_edited, user_id) "
                  "VALUES (:category, :short_form, :note_id, :archived, :created, :last_edited, :user_id)");
        q.bindValue(":category", params.value("category").toString());
        q.bindValue(":short_form", QVariant(QVariant::String));
        q.bindValue(":note_id", categoryNoteId > 0 ? categoryNoteId : QVariant(QVariant::Int));
        q.bindValue(":archived", false);
        q.bindValue(":created", currentDateTime);
        q.bindValue(":last_edited", currentDateTime);
        q.bindValue(":user_id", UserProfile::instance().userId());

        if (!q.exec())
            throw DatabaseException(DatabaseException::AddItemFailure, q.lastError().text(), "Failed to insert category.");

        categoryId = q.lastInsertId().toInt();
        if (!categoryId)
            throw DatabaseException(DatabaseException::AddItemFailure, q.lastError().text(), "Invalid category ID returned.");

        // Insert item
        q.prepare("INSERT INTO item (category_id, item, short_form, description, barcode, divisible, image, is_tracked, is_untracked, "
                  "is_service, note_id, archived, created, last_edited, user_id) "
                  "VALUES (:category_id, :item, :short_form, :description, :barcode, :divisible, :image, :is_tracked, :is_untracked, "
                  ":is_service, :note_id, :archived, :created, :last_edited, :user_id)");
        q.bindValue(":category_id", categoryId);
        q.bindValue(":item", params.value("item").toString());
        q.bindValue(":short_form", QVariant(QVariant::String));
        q.bindValue(":description", params.value("description").toString());
        q.bindValue(":barcode", QVariant(QVariant::String));
        q.bindValue(":divisible", params.value("divisible").toBool());

        // Store image as BLOB
        q.bindValue(":image", imageToByteArray(params.value("image_source").toString()));

        q.bindValue(":is_tracked", params.value("tracked").toBool());
        q.bindValue(":is_untracked", !params.value("tracked").toBool());
        q.bindValue(":is_service", false);
        q.bindValue(":note_id", itemNoteId > 0 ? itemNoteId : QVariant(QVariant::Int));
        q.bindValue(":archived", false);
        q.bindValue(":created", currentDateTime);
        q.bindValue(":last_edited", currentDateTime);
        q.bindValue(":user_id", UserProfile::instance().userId());

        if (!q.exec())
            throw DatabaseException(DatabaseException::AddItemFailure, q.lastError().text(), "Failed to insert item.");

        itemId = q.lastInsertId().toInt();
        if (!itemId)
            throw DatabaseException(DatabaseException::AddItemFailure, q.lastError().text(), "Invalid item ID returned.");


        // Insert unit
        q.prepare("INSERT INTO unit (item_id, unit, short_form, base_unit_equivalent, cost_price, retail_price, currency, note_id, "
                  "archived, created, last_edited, user_id) "
                  "VALUES (:item_id, :unit, :short_form, :base_unit_equivalent, :cost_price, :retail_price, :currency, :note_id, "
                  ":archived, :created, :last_edited, :user_id)");
        q.bindValue(":item_id", itemId);
        q.bindValue(":unit", params.value("unit").toString());
        q.bindValue(":short_form", QVariant(QVariant::String));
        q.bindValue(":base_unit_equivalent", 1);
        q.bindValue(":cost_price", params.value("cost_price").toString());
        q.bindValue(":retail_price", params.value("retail_price").toString());
        q.bindValue(":currency", "NGN");
        q.bindValue(":note_id", QVariant(QVariant::Int));
        q.bindValue(":archived", false);
        q.bindValue(":created", currentDateTime);
        q.bindValue(":last_edited", currentDateTime);
        q.bindValue(":user_id", UserProfile::instance().userId());

        // Store image as BLOB
        q.bindValue(":image", imageToByteArray(params.value("image_source").toString()));

        q.bindValue(":is_tracked", params.value("tracked").toBool());
        q.bindValue(":is_untracked", !params.value("tracked").toBool());
        q.bindValue(":is_service", false);
        q.bindValue(":note_id", itemNoteId > 0 ? itemNoteId : QVariant(QVariant::Int));
        q.bindValue(":archived", false);
        q.bindValue(":created", currentDateTime);
        q.bindValue(":last_edited", currentDateTime);
        q.bindValue(":user_id", UserProfile::instance().userId());

        if (!q.exec())
            throw DatabaseException(DatabaseException::AddItemFailure, q.lastError().text(), "Failed to insert unit.");

        unitId = q.lastInsertId().toInt();
        if (!unitId)
            throw DatabaseException(DatabaseException::AddItemFailure, q.lastError().text(), "Invalid unit ID returned.");


        // Insert quantity into initial_quantity table
        q.prepare("INSERT INTO initial_quantity (item_id, quantity, unit_id, created, last_edited, user_id) "
                  "VALUES (:item_id, :quantity, :unit_id, :created, :last_edited, :user_id)");
        q.bindValue(":item_id", itemId);
        q.bindValue(":quantity", params.value("quantity").toDouble());
        q.bindValue(":unit_id", unitId);
        q.bindValue(":created", currentDateTime);
        q.bindValue(":last_edited", currentDateTime);
        q.bindValue(":user_id", UserProfile::instance().userId());

        if (!q.exec())
            throw DatabaseException(DatabaseException::AddItemFailure, q.lastError().text(), "Failed to insert quantity into initial_quantity.");


        // Insert unit into current_quantity table
        q.prepare("INSERT INTO current_quantity (item_id, quantity, unit_id, created, last_edited, user_id) "
                  "VALUES (:item_id, :quantity, :unit_id, :created, :last_edited, :user_id)");
        q.bindValue(":item_id", itemId);
        q.bindValue(":quantity", params.value("quantity").toDouble());
        q.bindValue(":unit_id", unitId);
        q.bindValue(":created", currentDateTime);
        q.bindValue(":last_edited", currentDateTime);
        q.bindValue(":user_id", UserProfile::instance().userId());

        if (!q.exec())
            throw DatabaseException(DatabaseException::AddItemFailure, q.lastError().text(), "Failed to insert quantity into current_quantity.");

        if (!DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::CommitTransationFailed, q.lastError().text(), "Failed to commit.");
    } catch (DatabaseException &) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        throw;
    }
}

void StockSqlManager::viewStockItems(const QueryRequest &request, QueryResult &result)
{
    const QVariantMap params = request.params();

    try {
        QSqlQuery q(connection());
        QString itemInfoQuery = "SELECT item.id AS item_id, category.id AS category_id, category.category, item.item, item.description, "
                                "item.divisible, item.image, item.is_tracked, current_quantity.quantity, "
                                "unit.id as unit_id, unit.unit, unit.cost_price, "
                                "unit.retail_price, unit.currency, item.created, item.last_edited, item.user_id, item.user_id AS user "
                                "FROM item "
                                "INNER JOIN category ON item.category_id = category.id "
                                "INNER JOIN unit ON item.id = unit.item_id "
                                "INNER JOIN current_quantity ON item.id = current_quantity.item_id "
                                "LEFT JOIN user ON item.user_id = user.id "
                                "WHERE item.archived = 0 AND unit.base_unit_equivalent = 1 "
                                "%1 ORDER BY LOWER(item.item) ASC, LOWER(category.category) %2";
        QString sortOrder;
        QString filterColumn;

        if (params.value("sort_column").toString() == "item" && params.value("sort_order").toString() == "descending") {
            sortOrder = "DESC";
        } else {
            sortOrder = "ASC";
        }

        if (!params.value("filter_text").isNull() && !params.value("filter_column").isNull()) {
            if (params.value("filter_column") == "item") {
                filterColumn = QString(" AND item.item LIKE '%%1%'").arg(params.value("filter_text").toString());
            } else if (params.value("filter_column") == "category") {
                filterColumn = QString(" AND category.category LIKE '%%1%'").arg(params.value("filter_text").toString());
            }
        }

        q.prepare(itemInfoQuery.arg(filterColumn, sortOrder));

        if (!q.exec())
            throw DatabaseException(DatabaseException::ViewStockItemsFailed, q.lastError().text(), "Failed to fetch tracked stock items.");

        QVariantMap outcome;

        while (q.next()) {
            int categoryId = q.value("category_id").toInt();
            QString category = q.value("category").toString();

            QVariantList items;

           do {
                QVariantMap itemRecord;
                itemRecord.insert("item_id", q.value("item_id"));
                itemRecord.insert("category_id", categoryId);
                itemRecord.insert("item", q.value("item"));
                itemRecord.insert("description", q.value("description"));
                itemRecord.insert("divisible", q.value("divisible"));
                itemRecord.insert("image", byteArrayToImage(q.value("image").toByteArray()));
                itemRecord.insert("quantity", q.value("quantity"));
                itemRecord.insert("unit", q.value("unit"));
                itemRecord.insert("unit_id", q.value("unit_id"));
                itemRecord.insert("cost_price", q.value("cost_price"));
                itemRecord.insert("retail_price", q.value("retail_price"));
                itemRecord.insert("currency", q.value("currency"));
                itemRecord.insert("created", q.value("created"));
                itemRecord.insert("last_edited", q.value("last_edited"));
                itemRecord.insert("user", q.value("user")); // TODO: Inner join on user table

                items.append(itemRecord);
            } while (q.next() && categoryId == q.value("category_id").toInt());

            outcome.insert(category, items);
            q.previous();
        }

        /*
        qDebug() << "--------------------Outcome-----------------------";
        QMapIterator<QString, QVariant> categoryIter(categoryRecords);
        while (categoryIter.hasNext()) {
            categoryIter.next();
            qDebug() << categoryIter.key();
            qDebug() << "-------------------------------------------------";
            const QVariantList &items = categoryIter.value().toList();

            for (const QVariant &itemRecord : items) {
                qDebug() << "Item name:" << itemRecord.toMap().value("item").toString();
            }

            qDebug() << "-------------------------------------------------";
        }
        */
        result.setOutcome(outcome);
    } catch (DatabaseException &) {
        throw;
    }
}

void StockSqlManager::viewStockCategories(const QueryRequest &request, QueryResult &result)
{
    Q_UNUSED(request)

    try {
        QSqlQuery q(connection());
        q.prepare("SELECT id as category_id, category FROM category WHERE archived = 0 ORDER BY LOWER(category) ASC");

        if (!q.exec())
            throw DatabaseException(DatabaseException::ViewStockCategoriesFailed, q.lastError().text(), "Failed to fetch categories.");

        QVariantMap outcome;
        QVariantList categories;

        while (q.next()) {
            categories.append(recordToMap(q.record()));
        }

        outcome.insert("categories", categories);
        result.setOutcome(outcome);
    } catch (DatabaseException &) {
        throw;
    }
}

QByteArray StockSqlManager::imageToByteArray(const QString &imageSource)
{
    if (imageSource.trimmed().isEmpty())
        return QByteArray();

    QImage image(imageSource);
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer);

    return ba;
}

QString StockSqlManager::byteArrayToImage(const QByteArray &imageData)
{
    if (imageData.isNull())
        return QString();

    QString imageSource;

    return imageSource;
}
