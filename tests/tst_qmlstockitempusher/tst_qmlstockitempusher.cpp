#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <QSqlQuery>
#include <QLoggingCategory>

#include "qmlapi/qmlstockitempusher.h"
#include "../utils/databaseclient.h"

const QString SELECT_INSERTED_ITEMS("SELECT ");

class QMLStockItemPusherTest : public QObject
{
    Q_OBJECT

public:
    QMLStockItemPusherTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void testSetCategory();
    void testSetItem();
    void testSetDescription();
    void testSetQuantity();
    void testSetUnit();
    void testSetCategoryNote();
    void testSetItemNote();
    void testSetDivisible();
    void testSetCostPrice();
    void testSetRetailPrice();
    void testSetTracked();
    void testSetImageSource();

    void testPushNewItem();

private:
    QMLStockItemPusher *m_stockItemPusher;
    DatabaseClient *m_client;
};

QMLStockItemPusherTest::QMLStockItemPusherTest()
{
    QLoggingCategory::setFilterRules(QStringLiteral("*.debug=false"));
}

void QMLStockItemPusherTest::initTestCase()
{
    m_stockItemPusher = new QMLStockItemPusher(this);
    m_client = new DatabaseClient;

    QVERIFY(m_client->initialize());
}

void QMLStockItemPusherTest::cleanupTestCase()
{
    m_stockItemPusher->deleteLater();
    delete m_client;
}

void QMLStockItemPusherTest::testSetCategory()
{
    QSignalSpy categoryChangedSpy(m_stockItemPusher, &QMLStockItemPusher::categoryChanged);

    QCOMPARE(m_stockItemPusher->category(), QString());

    m_stockItemPusher->setCategory("Category");

    QCOMPARE(m_stockItemPusher->category(), "Category");
    QCOMPARE(categoryChangedSpy.count(), 1);
}

void QMLStockItemPusherTest::testSetItem()
{
    QSignalSpy itemChangedSpy(m_stockItemPusher, &QMLStockItemPusher::itemChanged);

    QCOMPARE(m_stockItemPusher->item(), QString());

    m_stockItemPusher->setItem("Item");

    QCOMPARE(m_stockItemPusher->item(), "Item");
    QCOMPARE(itemChangedSpy.count(), 1);
}

void QMLStockItemPusherTest::testSetDescription()
{
    QSignalSpy descriptionChangedSpy(m_stockItemPusher, &QMLStockItemPusher::descriptionChanged);

    QCOMPARE(m_stockItemPusher->description(), QString());

    m_stockItemPusher->setDescription("Description");

    QCOMPARE(m_stockItemPusher->description(), "Description");
    QCOMPARE(descriptionChangedSpy.count(), 1);
}

void QMLStockItemPusherTest::testSetQuantity()
{
    QSignalSpy quantityChangedSpy(m_stockItemPusher, &QMLStockItemPusher::quantityChanged);

    QCOMPARE(m_stockItemPusher->quantity(), 0.0);

    m_stockItemPusher->setQuantity(1.5);

    QCOMPARE(m_stockItemPusher->quantity(), 1.5);
    QCOMPARE(quantityChangedSpy.count(), 1);
}

void QMLStockItemPusherTest::testSetUnit()
{
    QSignalSpy unitChangedSpy(m_stockItemPusher, &QMLStockItemPusher::unitChanged);

    QCOMPARE(m_stockItemPusher->unit(), QString());

    m_stockItemPusher->setUnit("Unit");

    QCOMPARE(m_stockItemPusher->unit(), "Unit");
    QCOMPARE(unitChangedSpy.count(), 1);
}

void QMLStockItemPusherTest::testSetCategoryNote()
{
    QSignalSpy categoryNoteChangedSpy(m_stockItemPusher, &QMLStockItemPusher::categoryNoteChanged);

    QCOMPARE(m_stockItemPusher->categoryNote(), QString());

    m_stockItemPusher->setCategoryNote("Category note");

    QCOMPARE(m_stockItemPusher->categoryNote(), "Category note");
    QCOMPARE(categoryNoteChangedSpy.count(), 1);
}

void QMLStockItemPusherTest::testSetItemNote()
{
    QSignalSpy itemNoteChangedSpy(m_stockItemPusher, &QMLStockItemPusher::itemNoteChanged);

    QCOMPARE(m_stockItemPusher->itemNote(), QString());

    m_stockItemPusher->setItemNote("Item note");

    QCOMPARE(m_stockItemPusher->itemNote(), "Item note");
    QCOMPARE(itemNoteChangedSpy.count(), 1);
}

void QMLStockItemPusherTest::testSetDivisible()
{
    QSignalSpy divisibleChangedSpy(m_stockItemPusher, &QMLStockItemPusher::divisibleChanged);

    QCOMPARE(m_stockItemPusher->isDivisible(), false);

    m_stockItemPusher->setDivisible(true);

    QCOMPARE(m_stockItemPusher->isDivisible(), true);
    QCOMPARE(divisibleChangedSpy.count(), 1);
}

void QMLStockItemPusherTest::testSetCostPrice()
{
    QSignalSpy costPriceChangedSpy(m_stockItemPusher, &QMLStockItemPusher::costPriceChanged);

    QCOMPARE(m_stockItemPusher->costPrice(), 0.0);

    m_stockItemPusher->setCostPrice(1234.56);

    QCOMPARE(m_stockItemPusher->costPrice(), 1234.56);
    QCOMPARE(costPriceChangedSpy.count(), 1);
}

void QMLStockItemPusherTest::testSetRetailPrice()
{
    QSignalSpy retailPriceChangedSpy(m_stockItemPusher, &QMLStockItemPusher::retailPriceChanged);

    QCOMPARE(m_stockItemPusher->retailPrice(), 0.0);

    m_stockItemPusher->setRetailPrice(1234.56);

    QCOMPARE(m_stockItemPusher->retailPrice(), 1234.56);
    QCOMPARE(retailPriceChangedSpy.count(), 1);
}

void QMLStockItemPusherTest::testSetTracked()
{
    QSignalSpy trackedChangedSpy(m_stockItemPusher, &QMLStockItemPusher::trackedChanged);

    QCOMPARE(m_stockItemPusher->isTracked(), false);

    m_stockItemPusher->setTracked(true);

    QCOMPARE(m_stockItemPusher->isTracked(), true);
    QCOMPARE(trackedChangedSpy.count(), 1);
}

void QMLStockItemPusherTest::testSetImageSource()
{
    QSignalSpy imageSourceChangedSpy(m_stockItemPusher, &QMLStockItemPusher::imageSourceChanged);

    QCOMPARE(m_stockItemPusher->imageSource(), QString());

    m_stockItemPusher->setImageSource("Image source");

    QCOMPARE(m_stockItemPusher->imageSource(), "Image source");
    QCOMPARE(imageSourceChangedSpy.count(), 1);
}

void QMLStockItemPusherTest::testPushNewItem()
{
    QSignalSpy busyChangedSpy(m_stockItemPusher, &QMLStockItemPusher::busyChanged);

    m_stockItemPusher->setCategory("Category");
    m_stockItemPusher->setItem("Item");
    m_stockItemPusher->setDescription("Description");
    m_stockItemPusher->setQuantity(10.5);
    m_stockItemPusher->setUnit("Unit");
    m_stockItemPusher->setCategoryNote("Category note");
    m_stockItemPusher->setItemNote("Item note");
    m_stockItemPusher->setDivisible(true);
    m_stockItemPusher->setCostPrice(10.85);
    m_stockItemPusher->setRetailPrice(20.12);
    m_stockItemPusher->setTracked(true);
    m_stockItemPusher->setImageSource("Image source");

    m_stockItemPusher->push();

    QTest::qWaitFor([&]() { return !m_stockItemPusher->isBusy(); }, 2000);
    QCOMPARE(busyChangedSpy.count(), 2); // busy must be true, then false
    
    /*****************************************************************************************************************************/
    /************************************ DATABASE OPERATIONS ********************************************************************/
    /*****************************************************************************************************************************/
    // Verify category data
    QSqlQuery q(m_client->connection());
    q.prepare("SELECT id, category, archived, created, last_edited, user_id FROM category");
    QVERIFY(q.exec());
    QCOMPARE(q.size(), 1);
    QVERIFY(q.first());
    QCOMPARE(q.value("id").toInt(), 1);
    QCOMPARE(q.value("category").toString(), "Category");
    QCOMPARE(q.value("archived").toBool(), false);
    QCOMPARE(q.value("created").toDateTime(), q.value("last_edited").toDateTime());
    QCOMPARE(q.value("user_id").toInt(), 0); // Temporary

    // Verify item data
    q.prepare("SELECT id, category_id, item, description, divisible, archived, created, last_edited, user_id FROM item");
    QVERIFY(q.exec());
    QCOMPARE(q.size(), 1);
    QVERIFY(q.first());
    QCOMPARE(q.value("id").toInt(), 1);
    QCOMPARE(q.value("category_id").toInt(), 1);
    QCOMPARE(q.value("item").toString(), "Item");
    QCOMPARE(q.value("description").toString(), "Description");
    QCOMPARE(q.value("divisible").toBool(), true);
    QCOMPARE(q.value("archived").toBool(), false);
    QCOMPARE(q.value("created").toDateTime(), q.value("last_edited").toDateTime());
    QCOMPARE(q.value("user_id").toInt(), 0); // Temporary

    // Verify current quantity data
    q.prepare("SELECT id, item_id, quantity, unit_id, created, last_edited, user_id FROM current_quantity");
    QVERIFY(q.exec());
    QCOMPARE(q.size(), 1);
    QVERIFY(q.first());
    QCOMPARE(q.value("id").toInt(), 1);
    QCOMPARE(q.value("item_id").toInt(), 1);
    QCOMPARE(q.value("quantity").toDouble(), 10.5);
    QCOMPARE(q.value("unit_id").toInt(), 1);
    QCOMPARE(q.value("created").toDateTime(), q.value("last_edited").toDateTime());
    QCOMPARE(q.value("user_id").toInt(), 0); // Temporary

    // Verify initial quantity data
    q.prepare("SELECT id, item_id, quantity, unit_id, reason, archived, created, last_edited, user_id FROM initial_quantity");
    QVERIFY(q.exec());
    QCOMPARE(q.size(), 1);
    QVERIFY(q.first());
    QCOMPARE(q.value("id").toInt(), 1);
    QCOMPARE(q.value("item_id").toInt(), 1);
    QCOMPARE(q.value("quantity").toDouble(), 10.5);
    QCOMPARE(q.value("unit_id").toInt(), 1);
    QCOMPARE(q.value("reason").toString(), "add_new_stock_item");
    QCOMPARE(q.value("archived").toBool(), false);
    QCOMPARE(q.value("created").toDateTime(), q.value("last_edited").toDateTime());
    QCOMPARE(q.value("user_id").toInt(), 0); // Temporary

    // Verify unit data
    q.prepare("SELECT id, item_id, unit, base_unit_equivalent, preferred, cost_price, retail_price, archived, created, last_edited, user_id FROM unit");
    QVERIFY(q.exec());
    QCOMPARE(q.size(), 1);
    QVERIFY(q.first());
    QCOMPARE(q.value("id").toInt(), 1);
    QCOMPARE(q.value("item_id").toInt(), 1);
    QCOMPARE(q.value("unit").toString(), "Unit");
    QCOMPARE(q.value("base_unit_equivalent").toInt(), 1);
    QCOMPARE(q.value("preferred").toBool(), true);
    QCOMPARE(q.value("cost_price").toDouble(), 10.85);
    QCOMPARE(q.value("retail_price").toDouble(), 20.12);
    QCOMPARE(q.value("archived").toBool(), false);
    QCOMPARE(q.value("created").toDateTime(), q.value("last_edited").toDateTime());
    QCOMPARE(q.value("user_id").toInt(), 0); // Temporary
    /*****************************************************************************************************************************/
    /******************************************** END DATABASE OPERATIONS ********************************************************/
    /*****************************************************************************************************************************/
}

QTEST_MAIN(QMLStockItemPusherTest)

#include "tst_qmlstockitempusher.moc"
