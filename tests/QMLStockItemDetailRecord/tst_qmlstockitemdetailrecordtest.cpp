#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <QSqlQuery>
#include <QLoggingCategory>

#include "qmlapi/qmlstockitemdetailrecord.h"
#include "qmlapi/qmlstockitempusher.h"
#include "../utils/databaseclient.h"

class QMLStockItemDetailRecordTest : public QObject
{
    Q_OBJECT
public:
    QMLStockItemDetailRecordTest();
private Q_SLOTS:
    void init();
    void cleanup();

    // Long-running tests
    void testViewStockItemDetails();
private:
    QMLStockItemDetailRecord *m_stockItemDetailRecord;
    QMLStockItemPusher *m_stockItemPusher;
    DatabaseClient *m_client;
};

QMLStockItemDetailRecordTest::QMLStockItemDetailRecordTest()
{
    QLoggingCategory::setFilterRules(QStringLiteral("*.info=false"));
}

void QMLStockItemDetailRecordTest::init()
{
    m_stockItemDetailRecord = new QMLStockItemDetailRecord(this);
    m_stockItemPusher = new QMLStockItemPusher(this);
    m_client = new DatabaseClient;
}

void QMLStockItemDetailRecordTest::cleanup()
{
    m_stockItemDetailRecord->deleteLater();
    m_stockItemPusher->deleteLater();
    delete m_client;
}

void QMLStockItemDetailRecordTest::testViewStockItemDetails()
{
    QSignalSpy successSpy(m_stockItemDetailRecord, &QMLStockItemDetailRecord::success);
    QSignalSpy errorSpy(m_stockItemDetailRecord, &QMLStockItemDetailRecord::error);
    QSignalSpy busyChangedSpy(m_stockItemDetailRecord, &QMLStockItemDetailRecord::busyChanged);
    QSignalSpy itemIdChangedSpy(m_stockItemDetailRecord, &QMLStockItemDetailRecord::itemIdChanged);
    QSignalSpy categoryIdChangedSpy(m_stockItemDetailRecord, &QMLStockItemDetailRecord::categoryIdChanged);
    QSignalSpy categoryChangedSpy(m_stockItemDetailRecord, &QMLStockItemDetailRecord::categoryChanged);
    QSignalSpy itemChangedSpy(m_stockItemDetailRecord, &QMLStockItemDetailRecord::itemChanged);
    QSignalSpy descriptionChangedSpy(m_stockItemDetailRecord, &QMLStockItemDetailRecord::descriptionChanged);
    QSignalSpy divisibleChangedSpy(m_stockItemDetailRecord, &QMLStockItemDetailRecord::divisibleChanged);
    QSignalSpy quantityChangedSpy(m_stockItemDetailRecord, &QMLStockItemDetailRecord::quantityChanged);
    QSignalSpy unitIdChangedSpy(m_stockItemDetailRecord, &QMLStockItemDetailRecord::unitIdChanged);
    QSignalSpy unitChangedSpy(m_stockItemDetailRecord, &QMLStockItemDetailRecord::unitChanged);
    QSignalSpy costPriceChangedSpy(m_stockItemDetailRecord, &QMLStockItemDetailRecord::costPriceChanged);
    QSignalSpy retailPriceChangedSpy(m_stockItemDetailRecord, &QMLStockItemDetailRecord::retailPriceChanged);
    QSignalSpy currencyChangedSpy(m_stockItemDetailRecord, &QMLStockItemDetailRecord::currencyChanged);
    QSignalSpy createdChangedSpy(m_stockItemDetailRecord, &QMLStockItemDetailRecord::createdChanged);
    QSignalSpy lastEditedChangedSpy(m_stockItemDetailRecord, &QMLStockItemDetailRecord::lastEditedChanged);
    QSignalSpy userIdChangedSpy(m_stockItemDetailRecord, &QMLStockItemDetailRecord::userIdChanged);
    QSignalSpy userChangedSpy(m_stockItemDetailRecord, &QMLStockItemDetailRecord::userChanged);

    QVERIFY(m_client->initialize());

    // STEP: Add an item to the database.
    m_stockItemPusher->setCategory("Category");
    m_stockItemPusher->setItem("Item");
    m_stockItemPusher->setDescription("Description");
    m_stockItemPusher->setDivisible(true);
    m_stockItemPusher->setQuantity(1.0);
    m_stockItemPusher->setUnit("Unit");
    m_stockItemPusher->setCostPrice(2.0);
    m_stockItemPusher->setRetailPrice(3.0);
    m_stockItemPusher->push();
    QVERIFY(QTest::qWaitFor([&]() { return !m_stockItemPusher->isBusy(); }, 2000));

    // STEP: Instantiate model in QML.
    m_stockItemDetailRecord->componentComplete();

    // STEP: Ensure default values are set.
    QCOMPARE(m_stockItemDetailRecord->itemId(), -1);
    QCOMPARE(itemIdChangedSpy.count(), 0);
    QCOMPARE(m_stockItemDetailRecord->categoryId(), -1);
    QCOMPARE(categoryIdChangedSpy.count(), 0);
    QCOMPARE(m_stockItemDetailRecord->category(), QString());
    QCOMPARE(categoryChangedSpy.count(), 0);
    QCOMPARE(m_stockItemDetailRecord->item(), QString());
    QCOMPARE(itemChangedSpy.count(), 0);
    QCOMPARE(m_stockItemDetailRecord->description(), QString());
    QCOMPARE(descriptionChangedSpy.count(), 0);
    QCOMPARE(m_stockItemDetailRecord->isDivisible(), false);
    QCOMPARE(divisibleChangedSpy.count(), 0);
    QCOMPARE(m_stockItemDetailRecord->quantity(), 0.0);
    QCOMPARE(quantityChangedSpy.count(), 0);
    QCOMPARE(m_stockItemDetailRecord->unitId(), -1);
    QCOMPARE(unitIdChangedSpy.count(), 0);
    QCOMPARE(m_stockItemDetailRecord->unit(), QString());
    QCOMPARE(unitChangedSpy.count(), 0);
    QCOMPARE(m_stockItemDetailRecord->costPrice(), 0.0);
    QCOMPARE(costPriceChangedSpy.count(), 0);
    QCOMPARE(m_stockItemDetailRecord->retailPrice(), 0.0);
    QCOMPARE(retailPriceChangedSpy.count(), 0);
    QCOMPARE(m_stockItemDetailRecord->currency(), QString());
    QCOMPARE(currencyChangedSpy.count(), 0);
    QCOMPARE(m_stockItemDetailRecord->created(), QDateTime());
    QCOMPARE(createdChangedSpy.count(), 0);
    QCOMPARE(m_stockItemDetailRecord->lastEdited(), QDateTime());
    QCOMPARE(lastEditedChangedSpy.count(), 0);
    QCOMPARE(m_stockItemDetailRecord->userId(), -1);
    QCOMPARE(userIdChangedSpy.count(), 0);
    QCOMPARE(m_stockItemDetailRecord->user(), QString());
    QCOMPARE(userChangedSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 0);


    // STEP: Set item ID so that record can be fetched.
    m_stockItemDetailRecord->setItemId(1);
    QCOMPARE(m_stockItemDetailRecord->itemId(), 1);
    QCOMPARE(itemIdChangedSpy.count(), 1);
    itemIdChangedSpy.clear();
    QVERIFY(QTest::qWaitFor([&]() { return !m_stockItemDetailRecord->isBusy(); }, 2000));
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);

    // STEP: Ensure values are properly set.
    QCOMPARE(m_stockItemDetailRecord->itemId(), 1);
    QCOMPARE(m_stockItemDetailRecord->categoryId(), 1);
    QCOMPARE(categoryIdChangedSpy.count(), 1);
    QCOMPARE(m_stockItemDetailRecord->category(), QStringLiteral("Category"));
    QCOMPARE(categoryChangedSpy.count(), 1);
    QCOMPARE(m_stockItemDetailRecord->item(), QStringLiteral("Item"));
    QCOMPARE(itemChangedSpy.count(), 1);
    QCOMPARE(m_stockItemDetailRecord->description(), QStringLiteral("Description"));
    QCOMPARE(descriptionChangedSpy.count(), 1);
    QCOMPARE(m_stockItemDetailRecord->isDivisible(), true);
    QCOMPARE(divisibleChangedSpy.count(), 1);
    QCOMPARE(m_stockItemDetailRecord->quantity(), 1.0);
    QCOMPARE(quantityChangedSpy.count(), 1);
    QCOMPARE(m_stockItemDetailRecord->unitId(), 1);
    QCOMPARE(unitIdChangedSpy.count(), 1);
    QCOMPARE(m_stockItemDetailRecord->unit(), QStringLiteral("Unit"));
    QCOMPARE(unitChangedSpy.count(), 1);
    QCOMPARE(m_stockItemDetailRecord->costPrice(), 2.0);
    QCOMPARE(costPriceChangedSpy.count(), 1);
    QCOMPARE(m_stockItemDetailRecord->retailPrice(), 3.0);
    QCOMPARE(retailPriceChangedSpy.count(), 1);
    QCOMPARE(m_stockItemDetailRecord->currency(), QStringLiteral("NGN"));
    QCOMPARE(currencyChangedSpy.count(), 1);
    QCOMPARE(m_stockItemDetailRecord->created(), m_stockItemDetailRecord->lastEdited());
    QCOMPARE(createdChangedSpy.count(), 1);
    QCOMPARE(lastEditedChangedSpy.count(), 1);
    QCOMPARE(m_stockItemDetailRecord->userId(), 0);
    QCOMPARE(userIdChangedSpy.count(), 1);
    QCOMPARE(m_stockItemDetailRecord->user(), QStringLiteral("0"));
    QCOMPARE(userChangedSpy.count(), 1);
    QCOMPARE(busyChangedSpy.count(), 0);
}

QTEST_MAIN(QMLStockItemDetailRecordTest)

#include "tst_qmlstockitemdetailrecordtest.moc"
