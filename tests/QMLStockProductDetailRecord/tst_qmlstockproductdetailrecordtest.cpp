#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmlstockproductdetailrecord.h"
#include "mockdatabasethread.h"

class QMLStockProductDetailRecordTest : public QObject
{
    Q_OBJECT

public:
    QMLStockProductDetailRecordTest();

private Q_SLOTS:
    void init();
    void cleanup();

    void testViewStockProductDetails();

private:
    QMLStockProductDetailRecord *m_stockProductDetailRecord;
    MockDatabaseThread m_thread;
    QueryResult m_result;
};

QMLStockProductDetailRecordTest::QMLStockProductDetailRecordTest() :
    m_thread(&m_result)
{
    QLoggingCategory::setFilterRules(QStringLiteral("*.info=false"));
}

void QMLStockProductDetailRecordTest::init()
{
    m_stockProductDetailRecord = new QMLStockProductDetailRecord(m_thread, this);
}

void QMLStockProductDetailRecordTest::cleanup()
{
}

void QMLStockProductDetailRecordTest::testViewStockProductDetails()
{
    auto databaseWillReturnSingleItem = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());
        const QVariantMap itemInfo {
            { "category_id", 1 },
            { "category", "Category" },
            { "item_id", 1 },
            { "item", "Item" },
            { "description", "Description" },
            { "quantity", 1.0 },
            { "unit_id", 1 },
            { "unit", "Unit" },
            { "cost_price", 2.0 },
            { "retail_price", 3.0 },
            { "unit_price", 13.0 },
            { "available_quantity", 10.0 },
            { "divisible", true },
            { "currency", QStringLiteral("NGN") },
            { "created", QDateTime::currentDateTime() },
            { "last_edited", QDateTime::currentDateTime() },
            { "user", QStringLiteral("user") }
        };
        m_result.setOutcome(QVariantMap { { "item", itemInfo } });
    };
    QSignalSpy successSpy(m_stockProductDetailRecord, &QMLStockProductDetailRecord::success);
    QSignalSpy errorSpy(m_stockProductDetailRecord, &QMLStockProductDetailRecord::error);
    QSignalSpy busyChangedSpy(m_stockProductDetailRecord, &QMLStockProductDetailRecord::busyChanged);
    QSignalSpy itemIdChangedSpy(m_stockProductDetailRecord, &QMLStockProductDetailRecord::productIdChanged);
    QSignalSpy categoryIdChangedSpy(m_stockProductDetailRecord, &QMLStockProductDetailRecord::categoryIdChanged);
    QSignalSpy categoryChangedSpy(m_stockProductDetailRecord, &QMLStockProductDetailRecord::categoryChanged);
    QSignalSpy itemChangedSpy(m_stockProductDetailRecord, &QMLStockProductDetailRecord::productChanged);
    QSignalSpy descriptionChangedSpy(m_stockProductDetailRecord, &QMLStockProductDetailRecord::descriptionChanged);
    QSignalSpy divisibleChangedSpy(m_stockProductDetailRecord, &QMLStockProductDetailRecord::divisibleChanged);
    QSignalSpy quantityChangedSpy(m_stockProductDetailRecord, &QMLStockProductDetailRecord::quantityChanged);
    QSignalSpy unitIdChangedSpy(m_stockProductDetailRecord, &QMLStockProductDetailRecord::unitIdChanged);
    QSignalSpy unitChangedSpy(m_stockProductDetailRecord, &QMLStockProductDetailRecord::unitChanged);
    QSignalSpy costPriceChangedSpy(m_stockProductDetailRecord, &QMLStockProductDetailRecord::costPriceChanged);
    QSignalSpy retailPriceChangedSpy(m_stockProductDetailRecord, &QMLStockProductDetailRecord::retailPriceChanged);
    QSignalSpy currencyChangedSpy(m_stockProductDetailRecord, &QMLStockProductDetailRecord::currencyChanged);
    QSignalSpy createdChangedSpy(m_stockProductDetailRecord, &QMLStockProductDetailRecord::createdChanged);
    QSignalSpy lastEditedChangedSpy(m_stockProductDetailRecord, &QMLStockProductDetailRecord::lastEditedChanged);
    QSignalSpy userIdChangedSpy(m_stockProductDetailRecord, &QMLStockProductDetailRecord::userIdChanged);
    QSignalSpy userChangedSpy(m_stockProductDetailRecord, &QMLStockProductDetailRecord::userChanged);

    // STEP: Instantiate model in QML.
    m_stockProductDetailRecord->componentComplete();

    // STEP: Ensure default values are set.
    QCOMPARE(m_stockProductDetailRecord->productId(), -1);
    QCOMPARE(itemIdChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->categoryId(), -1);
    QCOMPARE(categoryIdChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->category(), QString());
    QCOMPARE(categoryChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->product(), QString());
    QCOMPARE(itemChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->description(), QString());
    QCOMPARE(descriptionChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->isDivisible(), false);
    QCOMPARE(divisibleChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->quantity(), 0.0);
    QCOMPARE(quantityChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->unitId(), -1);
    QCOMPARE(unitIdChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->unit(), QString());
    QCOMPARE(unitChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->costPrice(), 0.0);
    QCOMPARE(costPriceChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->retailPrice(), 0.0);
    QCOMPARE(retailPriceChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->currency(), QString());
    QCOMPARE(currencyChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->created(), QDateTime());
    QCOMPARE(createdChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->lastEdited(), QDateTime());
    QCOMPARE(lastEditedChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->userId(), -1);
    QCOMPARE(userIdChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->user(), QString());
    QCOMPARE(userChangedSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 0);

    databaseWillReturnSingleItem();

    // STEP: Set item ID so that record can be fetched.
    m_stockProductDetailRecord->setProductId(1);
    QCOMPARE(m_stockProductDetailRecord->productId(), 1);
    QCOMPARE(itemIdChangedSpy.count(), 1);
    itemIdChangedSpy.clear();
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);

    // STEP: Ensure values are properly set.
    QCOMPARE(m_stockProductDetailRecord->productId(), 1);
    QCOMPARE(m_stockProductDetailRecord->categoryId(), 1);
    QCOMPARE(categoryIdChangedSpy.count(), 1);
    QCOMPARE(m_stockProductDetailRecord->category(), QStringLiteral("Category"));
    QCOMPARE(categoryChangedSpy.count(), 1);
    QCOMPARE(m_stockProductDetailRecord->product(), QStringLiteral("Product"));
    QCOMPARE(itemChangedSpy.count(), 1);
    QCOMPARE(m_stockProductDetailRecord->description(), QStringLiteral("Description"));
    QCOMPARE(descriptionChangedSpy.count(), 1);
    QCOMPARE(m_stockProductDetailRecord->isDivisible(), true);
    QCOMPARE(divisibleChangedSpy.count(), 1);
    QCOMPARE(m_stockProductDetailRecord->quantity(), 1.0);
    QCOMPARE(quantityChangedSpy.count(), 1);
    QCOMPARE(m_stockProductDetailRecord->unitId(), 1);
    QCOMPARE(unitIdChangedSpy.count(), 1);
    QCOMPARE(m_stockProductDetailRecord->unit(), QStringLiteral("Unit"));
    QCOMPARE(unitChangedSpy.count(), 1);
    QCOMPARE(m_stockProductDetailRecord->costPrice(), 2.0);
    QCOMPARE(costPriceChangedSpy.count(), 1);
    QCOMPARE(m_stockProductDetailRecord->retailPrice(), 3.0);
    QCOMPARE(retailPriceChangedSpy.count(), 1);
    QCOMPARE(m_stockProductDetailRecord->currency(), QStringLiteral("NGN"));
    QCOMPARE(currencyChangedSpy.count(), 1);
    QCOMPARE(m_stockProductDetailRecord->created(), m_stockProductDetailRecord->lastEdited());
    QCOMPARE(createdChangedSpy.count(), 1);
    QCOMPARE(lastEditedChangedSpy.count(), 1);
    QCOMPARE(m_stockProductDetailRecord->userId(), 0);
    QCOMPARE(userIdChangedSpy.count(), 1);
    QCOMPARE(m_stockProductDetailRecord->user(), QStringLiteral("user"));
    QCOMPARE(userChangedSpy.count(), 1);
    QCOMPARE(busyChangedSpy.count(), 0);
}

QTEST_MAIN(QMLStockProductDetailRecordTest)

#include "tst_qmlstockproductdetailrecordtest.moc"
