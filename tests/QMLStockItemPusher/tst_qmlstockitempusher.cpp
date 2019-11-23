#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmlstockitempusher.h"
#include "database/databaseexception.h"
#include "mockdatabasethread.h"

class QMLStockItemPusherTest : public QObject
{
    Q_OBJECT

public:
    QMLStockItemPusherTest();

private Q_SLOTS:
    void init();
    void cleanup();

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
    void testPushSameItem();
    void testPushUpdatedItem();

    // void testQuantityCannotBeUpdated();
    // void testPushSameCategoryItems();
    // void testPushDifferentCategoryItems();
private:
    QMLStockItemPusher *m_stockItemPusher;
    MockDatabaseThread m_thread;
    QueryResult m_result;
};

QMLStockItemPusherTest::QMLStockItemPusherTest() :
    m_thread(&m_result)
{
    QLoggingCategory::setFilterRules(QStringLiteral("*.info=false"));
}

void QMLStockItemPusherTest::init()
{
    m_stockItemPusher = new QMLStockItemPusher(m_thread, this);
}

void QMLStockItemPusherTest::cleanup()
{
}

void QMLStockItemPusherTest::testSetCategory()
{
    QSignalSpy categoryChangedSpy(m_stockItemPusher, &QMLStockItemPusher::categoryChanged);

    QCOMPARE(m_stockItemPusher->category(), QString());

    m_stockItemPusher->setCategory("Category");

    QCOMPARE(m_stockItemPusher->category(), "Category");
    QCOMPARE(categoryChangedSpy.count(), 1);
    categoryChangedSpy.clear();

    m_stockItemPusher->setCategory("Category");
    QCOMPARE(categoryChangedSpy.count(), 0);
}

void QMLStockItemPusherTest::testSetItem()
{
    QSignalSpy itemChangedSpy(m_stockItemPusher, &QMLStockItemPusher::itemChanged);

    QCOMPARE(m_stockItemPusher->item(), QString());

    m_stockItemPusher->setItem("Item");

    QCOMPARE(m_stockItemPusher->item(), "Item");
    QCOMPARE(itemChangedSpy.count(), 1);
    itemChangedSpy.clear();

    m_stockItemPusher->setItem("Item");
    QCOMPARE(itemChangedSpy.count(), 0);
}

void QMLStockItemPusherTest::testSetDescription()
{
    QSignalSpy descriptionChangedSpy(m_stockItemPusher, &QMLStockItemPusher::descriptionChanged);

    QCOMPARE(m_stockItemPusher->description(), QString());

    m_stockItemPusher->setDescription("Description");

    QCOMPARE(m_stockItemPusher->description(), "Description");
    QCOMPARE(descriptionChangedSpy.count(), 1);
    descriptionChangedSpy.clear();

    m_stockItemPusher->setDescription("Description");
    QCOMPARE(descriptionChangedSpy.count(), 0);
}

void QMLStockItemPusherTest::testSetQuantity()
{
    QSignalSpy quantityChangedSpy(m_stockItemPusher, &QMLStockItemPusher::quantityChanged);

    QCOMPARE(m_stockItemPusher->quantity(), 0.0);

    m_stockItemPusher->setQuantity(1.5);

    QCOMPARE(m_stockItemPusher->quantity(), 1.5);
    QCOMPARE(quantityChangedSpy.count(), 1);
    quantityChangedSpy.clear();

    m_stockItemPusher->setQuantity(1.5);
    QCOMPARE(quantityChangedSpy.count(), 0);
}

void QMLStockItemPusherTest::testSetUnit()
{
    QSignalSpy unitChangedSpy(m_stockItemPusher, &QMLStockItemPusher::unitChanged);

    QCOMPARE(m_stockItemPusher->unit(), QString());

    m_stockItemPusher->setUnit("Unit");

    QCOMPARE(m_stockItemPusher->unit(), "Unit");
    QCOMPARE(unitChangedSpy.count(), 1);
    unitChangedSpy.clear();

    m_stockItemPusher->setUnit("Unit");
    QCOMPARE(unitChangedSpy.count(), 0);
}

void QMLStockItemPusherTest::testSetCategoryNote()
{
    QSignalSpy categoryNoteChangedSpy(m_stockItemPusher, &QMLStockItemPusher::categoryNoteChanged);

    QCOMPARE(m_stockItemPusher->categoryNote(), QString());

    m_stockItemPusher->setCategoryNote("Category note");

    QCOMPARE(m_stockItemPusher->categoryNote(), "Category note");
    QCOMPARE(categoryNoteChangedSpy.count(), 1);
    categoryNoteChangedSpy.clear();

    m_stockItemPusher->setCategoryNote("Category note");
    QCOMPARE(categoryNoteChangedSpy.count(), 0);
}

void QMLStockItemPusherTest::testSetItemNote()
{
    QSignalSpy itemNoteChangedSpy(m_stockItemPusher, &QMLStockItemPusher::itemNoteChanged);

    QCOMPARE(m_stockItemPusher->itemNote(), QString());

    m_stockItemPusher->setItemNote("Item note");

    QCOMPARE(m_stockItemPusher->itemNote(), "Item note");
    QCOMPARE(itemNoteChangedSpy.count(), 1);
    itemNoteChangedSpy.clear();

    m_stockItemPusher->setItemNote("Item note");
    QCOMPARE(itemNoteChangedSpy.count(), 0);
}

void QMLStockItemPusherTest::testSetDivisible()
{
    QSignalSpy divisibleChangedSpy(m_stockItemPusher, &QMLStockItemPusher::divisibleChanged);

    QCOMPARE(m_stockItemPusher->isDivisible(), false);

    m_stockItemPusher->setDivisible(true);

    QCOMPARE(m_stockItemPusher->isDivisible(), true);
    QCOMPARE(divisibleChangedSpy.count(), 1);
    divisibleChangedSpy.clear();

    m_stockItemPusher->setDivisible(true);
    QCOMPARE(divisibleChangedSpy.count(), 0);
}

void QMLStockItemPusherTest::testSetCostPrice()
{
    QSignalSpy costPriceChangedSpy(m_stockItemPusher, &QMLStockItemPusher::costPriceChanged);

    QCOMPARE(m_stockItemPusher->costPrice(), 0.0);

    m_stockItemPusher->setCostPrice(1234.56);

    QCOMPARE(m_stockItemPusher->costPrice(), 1234.56);
    QCOMPARE(costPriceChangedSpy.count(), 1);
    costPriceChangedSpy.clear();

    m_stockItemPusher->setCostPrice(1234.56);
    QCOMPARE(costPriceChangedSpy.count(), 0);
}

void QMLStockItemPusherTest::testSetRetailPrice()
{
    QSignalSpy retailPriceChangedSpy(m_stockItemPusher, &QMLStockItemPusher::retailPriceChanged);

    QCOMPARE(m_stockItemPusher->retailPrice(), 0.0);

    m_stockItemPusher->setRetailPrice(1234.56);

    QCOMPARE(m_stockItemPusher->retailPrice(), 1234.56);
    QCOMPARE(retailPriceChangedSpy.count(), 1);
    retailPriceChangedSpy.clear();

    m_stockItemPusher->setRetailPrice(1234.56);
    QCOMPARE(retailPriceChangedSpy.count(), 0);
}

void QMLStockItemPusherTest::testSetTracked()
{
    QSignalSpy trackedChangedSpy(m_stockItemPusher, &QMLStockItemPusher::trackedChanged);

    QCOMPARE(m_stockItemPusher->isTracked(), false);

    m_stockItemPusher->setTracked(true);

    QCOMPARE(m_stockItemPusher->isTracked(), true);
    QCOMPARE(trackedChangedSpy.count(), 1);
    trackedChangedSpy.clear();

    m_stockItemPusher->setTracked(true);
    QCOMPARE(trackedChangedSpy.count(), 0);
}

void QMLStockItemPusherTest::testSetImageSource()
{
    QSignalSpy imageSourceChangedSpy(m_stockItemPusher, &QMLStockItemPusher::imageSourceChanged);

    QCOMPARE(m_stockItemPusher->imageSource(), QString());

    m_stockItemPusher->setImageSource("Image source");

    QCOMPARE(m_stockItemPusher->imageSource(), "Image source");
    QCOMPARE(imageSourceChangedSpy.count(), 1);
    imageSourceChangedSpy.clear();

    m_stockItemPusher->setImageSource("Image source");
    QCOMPARE(imageSourceChangedSpy.count(), 0);
}

void QMLStockItemPusherTest::testPushNewItem()
{
    auto databaseWillReturnEmptyResult = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());
    };
    QSignalSpy busyChangedSpy(m_stockItemPusher, &QMLStockItemPusher::busyChanged);
    QSignalSpy successSpy(m_stockItemPusher, &QMLStockItemPusher::success);

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

    databaseWillReturnEmptyResult();

    m_stockItemPusher->push();

    QCOMPARE(busyChangedSpy.count(), 2); // busy must be true, then false
    QCOMPARE(successSpy.count(), 1);
}

void QMLStockItemPusherTest::testPushSameItem()
{
    auto databaseWillReturnEmptyResult = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());
    };
    auto databaseWillReturnDuplicateEntryError = [this]() {
        m_result.setSuccessful(false);
        m_result.setOutcome(QVariant());
        m_result.setErrorCode(static_cast<int>(DatabaseError::MySqlErrorCode::UserDefinedException));
    };
    QSignalSpy busyChangedSpy(m_stockItemPusher, &QMLStockItemPusher::busyChanged);
    QSignalSpy errorSpy(m_stockItemPusher, &QMLStockItemPusher::error);
    QSignalSpy successSpy(m_stockItemPusher, &QMLStockItemPusher::success);

    databaseWillReturnEmptyResult();

    m_stockItemPusher->push();

    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();

    databaseWillReturnDuplicateEntryError();

    m_stockItemPusher->push();
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(successSpy.count(), 0);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 1);
    QCOMPARE(errorSpy.takeFirst().first().value<QMLStockItemPusher::ErrorCode>(), QMLStockItemPusher::DuplicateEntryError);
    errorSpy.clear();
}

void QMLStockItemPusherTest::testPushUpdatedItem()
{
    auto databaseWillReturnEmptyResult = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());
    };
    QSignalSpy busyChangedSpy(m_stockItemPusher, &QMLStockItemPusher::busyChanged);
    QSignalSpy errorSpy(m_stockItemPusher, &QMLStockItemPusher::error);
    QSignalSpy successSpy(m_stockItemPusher, &QMLStockItemPusher::success);

    // STEP: Ensure item ID is not set.
    QCOMPARE(m_stockItemPusher->itemId(), -1);

    // STEP: Add a single item to the database.
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

    databaseWillReturnEmptyResult();

    m_stockItemPusher->push();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLStockItemPusher::SuccessCode>(), QMLStockItemPusher::AddItemSuccess);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    errorSpy.clear();

    databaseWillReturnEmptyResult();

    // STEP: Update the previously added item's properties.
    m_stockItemPusher->setItemId(1);
    m_stockItemPusher->setCategory("Category2");
    m_stockItemPusher->setItem("Item2");
    m_stockItemPusher->setDescription("Description2");
    m_stockItemPusher->setUnit("Unit2");
    m_stockItemPusher->setCategoryNote("Category note2");
    m_stockItemPusher->setItemNote("Item note2");
    m_stockItemPusher->setDivisible(false);
    m_stockItemPusher->setCostPrice(12.34);
    m_stockItemPusher->setRetailPrice(56.78);
    m_stockItemPusher->setTracked(false);
    m_stockItemPusher->setImageSource("Image source2");
    m_stockItemPusher->push();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2); // "busy" must be true, then false
    busyChangedSpy.clear();
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLStockItemPusher::SuccessCode>(), QMLStockItemPusher::UpdateItemSuccess);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    errorSpy.clear();
}

QTEST_MAIN(QMLStockItemPusherTest)

#include "tst_qmlstockitempusher.moc"
