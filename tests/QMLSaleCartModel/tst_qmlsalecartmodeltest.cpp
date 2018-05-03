#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmlsalecartmodel.h"
#include "qmlapi/qmlstockitempusher.h"
#include "databaseclient.h"

#include <QSqlQuery>

class QMLSaleCartModelTest : public QObject
{
    Q_OBJECT

public:
    QMLSaleCartModelTest();

private Q_SLOTS:
    void init();
    void cleanup();

    void testSetCustomerName();
    void testSetCustomerPhoneNumber();
    void testSetNote();
    void testSetAmountPaid();
    void testClearAll();

    void testAddItem();
    void testAddSameItem();
    void testAddDifferentItems();

    void testGetTotalCost();
    void testGetClientId();

    // Long-running tests
    void testSubmitTransaction();
    void testSuspendTransaction();
    void testSetTransactionId();
    void testRetrieveSuspendedTransaction();
    void testSubmitEmptyTransaction();
    void testSuspendEmptyTransaction();

    void testUpdateSuspendedTransaction(); // TODO

private:
    QMLStockItemPusher *m_stockItemPusher;
    QMLSaleCartModel *m_saleCartModel;
    DatabaseClient *m_client;
};

QMLSaleCartModelTest::QMLSaleCartModelTest()
{
    QLoggingCategory::setFilterRules(QStringLiteral("*.debug=false"));
}

void QMLSaleCartModelTest::init()
{
    m_stockItemPusher = new QMLStockItemPusher(this);
    m_saleCartModel = new QMLSaleCartModel(this);
    m_client = new DatabaseClient;
}

void QMLSaleCartModelTest::cleanup()
{
    m_stockItemPusher->deleteLater();
    m_saleCartModel->deleteLater();
    delete m_client;
}

void QMLSaleCartModelTest::testSetTransactionId()
{
    QSignalSpy transactionIdChangedSpy(m_saleCartModel, &QMLSaleCartModel::transactionIdChanged);
    QVERIFY(m_client->initialize());

    QCOMPARE(m_saleCartModel->transactionId(), -1);

    // Push some items
    m_stockItemPusher->setCategory("Category1");
    m_stockItemPusher->setItem("Item1");
    m_stockItemPusher->setDescription("Description1");
    m_stockItemPusher->setDivisible(true);
    m_stockItemPusher->setQuantity(1.0);
    m_stockItemPusher->setUnit("Unit1");
    m_stockItemPusher->setCostPrice(2.0);
    m_stockItemPusher->setRetailPrice(3.0);
    m_stockItemPusher->push();

    QVERIFY(QTest::qWaitFor([&]() { return !m_stockItemPusher->isBusy(); }, 2000));

    QVariantMap itemInfo {
        { "category_id", 1 },
        { "category", "Category1" },
        { "item_id", 1 },
        { "item", "Item1" },
        { "quantity", 1.0 },
        { "unit_id", 1 },
        { "unit", "Unit" },
        { "cost_price", 11.0 },
        { "retail_price", 10.0 },
        { "unit_price", 13.0 },
        { "available_quantity", 10.0 }
    };

    m_saleCartModel->setCustomerName("Customer");
    m_saleCartModel->setCustomerPhoneNumber("123456789");
    m_saleCartModel->addItem(itemInfo);

    m_saleCartModel->submitTransaction();
    QVERIFY(QTest::qWaitFor([&]() { return !m_saleCartModel->isBusy(); }, 2000));
    QCOMPARE(m_saleCartModel->rowCount(), 0);

    m_saleCartModel->setTransactionId(1);

    QCOMPARE(m_saleCartModel->transactionId(), 1);
    QCOMPARE(transactionIdChangedSpy.count(), 1);
    transactionIdChangedSpy.clear();
    QCOMPARE(m_saleCartModel->rowCount(), 1);

    m_saleCartModel->setTransactionId(1);
    QCOMPARE(transactionIdChangedSpy.count(), 0);
}

void QMLSaleCartModelTest::testSetCustomerName()
{
    QSignalSpy customerNameChangedSpy(m_saleCartModel, &QMLSaleCartModel::customerNameChanged);

    QCOMPARE(m_saleCartModel->customerName(), QString());

    m_saleCartModel->setCustomerName("Customer");

    QCOMPARE(m_saleCartModel->customerName(), "Customer");
    QCOMPARE(customerNameChangedSpy.count(), 1);
    customerNameChangedSpy.clear();

    m_saleCartModel->setCustomerName("Customer");
    QCOMPARE(customerNameChangedSpy.count(), 0);
}

void QMLSaleCartModelTest::testSetCustomerPhoneNumber()
{
    QSignalSpy customerPhoneNumberSpy(m_saleCartModel, &QMLSaleCartModel::customerPhoneNumberChanged);

    QCOMPARE(m_saleCartModel->customerPhoneNumber(), QString());

    m_saleCartModel->setCustomerPhoneNumber("12345");

    QCOMPARE(m_saleCartModel->customerPhoneNumber(), "12345");
    QCOMPARE(customerPhoneNumberSpy.count(), 1);
    customerPhoneNumberSpy.clear();

    m_saleCartModel->setCustomerPhoneNumber("12345");
    QCOMPARE(customerPhoneNumberSpy.count(), 0);
}

void QMLSaleCartModelTest::testSetNote()
{
    QSignalSpy noteChangedSpy(m_saleCartModel, &QMLSaleCartModel::noteChanged);

    QCOMPARE(m_saleCartModel->note(), QString());

    m_saleCartModel->setNote("Note");

    QCOMPARE(m_saleCartModel->note(), "Note");
    QCOMPARE(noteChangedSpy.count(), 1);
    noteChangedSpy.clear();

    m_saleCartModel->setNote("Note");
    QCOMPARE(noteChangedSpy.count(), 0);
}

void QMLSaleCartModelTest::testSetAmountPaid()
{
    QSignalSpy amountPaidSpy(m_saleCartModel, &QMLSaleCartModel::amountPaidChanged);

    QCOMPARE(m_saleCartModel->amountPaid(), 0.0);

    m_saleCartModel->setAmountPaid(123.45);

    QCOMPARE(m_saleCartModel->amountPaid(), 123.45);
    QCOMPARE(amountPaidSpy.count(), 1);
    amountPaidSpy.clear();

    m_saleCartModel->setAmountPaid(123.45);
    QCOMPARE(amountPaidSpy.count(), 0);
}

void QMLSaleCartModelTest::testClearAll()
{
    QVariantMap itemInfo {
        { "category_id", 1 },
        { "category", "Category1" },
        { "item_id", 1 },
        { "item", "Item1" },
        { "quantity", 1.0 },
        { "unit_id", 1 },
        { "unit", "Unit" },
        { "cost_price", 11.0 },
        { "retail_price", 10.0 },
        { "unit_price", 13.0 },
        { "available_quantity", 10.0 }
    };

    m_saleCartModel->setCustomerName("Customer");
    m_saleCartModel->setCustomerPhoneNumber("123456789");
    m_saleCartModel->addItem(itemInfo);
    m_saleCartModel->clearAll();

    QCOMPARE(m_saleCartModel->customerName(), QString());
    QCOMPARE(m_saleCartModel->customerPhoneNumber(), QString());
    QCOMPARE(m_saleCartModel->rowCount(), 0);
}

void QMLSaleCartModelTest::testAddItem()
{
    QSignalSpy rowsInsertedSpy(m_saleCartModel, &QMLSaleCartModel::rowsInserted);

    QVariantMap itemInfo {
        { "category_id", 1 },
        { "category", "Category1" },
        { "item_id", 1 },
        { "item", "Item1" },
        { "quantity", 1.0 },
        { "unit_id", 1 },
        { "unit", "Unit" },
        { "cost_price", 11.0 },
        { "retail_price", 10.0 },
        { "unit_price", 13.0 },
        { "available_quantity", 10.0 }
    };

    m_saleCartModel->addItem(itemInfo);
    QCOMPARE(rowsInsertedSpy.count(), 1);
    QCOMPARE(m_saleCartModel->rowCount(), 1);
}

void QMLSaleCartModelTest::testAddSameItem()
{
    QSignalSpy rowsInsertedSpy(m_saleCartModel, &QMLSaleCartModel::rowsInserted);
    QSignalSpy dataChangedSpy(m_saleCartModel, &QMLSaleCartModel::dataChanged);

    QVariantMap itemInfo {
        { "category_id", 1 },
        { "category", "Category1" },
        { "item_id", 1 },
        { "item", "Item1" },
        { "quantity", 2.0 },
        { "unit_id", 1 },
        { "unit", "Unit" },
        { "cost_price", 11.0 },
        { "retail_price", 10.0 },
        { "unit_price", 13.0 },
        { "available_quantity", 10.0 }
    };

    m_saleCartModel->addItem(itemInfo);
    m_saleCartModel->addItem(itemInfo);
    QCOMPARE(rowsInsertedSpy.count(), 1);
    QCOMPARE(dataChangedSpy.count(), 1);
    QCOMPARE(m_saleCartModel->rowCount(), 1);
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::QuantityRole).toDouble(), 2.0);
}

void QMLSaleCartModelTest::testAddDifferentItems()
{
    QSignalSpy rowsInsertedSpy(m_saleCartModel, &QMLSaleCartModel::rowsInserted);

    QVariantMap itemInfo1 {
        { "category_id", 1 },
        { "category", "Category1" },
        { "item_id", 1 },
        { "item", "Item1" },
        { "quantity", 1.0 },
        { "unit_id", 1 },
        { "unit", "Unit" },
        { "cost_price", 11.0 },
        { "retail_price", 10.0 },
        { "unit_price", 13.0 },
        { "available_quantity", 10.0 }
    };

    QVariantMap itemInfo2 {
        { "category_id", 1 },
        { "category", "Category1" },
        { "item_id", 2 },
        { "item", "Item2" },
        { "quantity", 1.0 },
        { "unit_id", 1 },
        { "unit", "Unit" },
        { "cost_price", 11.0 },
        { "retail_price", 10.0 },
        { "unit_price", 13.0 },
        { "available_quantity", 10.0 }
    };

    m_saleCartModel->addItem(itemInfo1);
    m_saleCartModel->addItem(itemInfo1);
    m_saleCartModel->addItem(itemInfo2);
    QCOMPARE(rowsInsertedSpy.count(), 2);
    QCOMPARE(m_saleCartModel->rowCount(), 2);
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::QuantityRole).toDouble(), 2.0);
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(1), QMLSaleCartModel::QuantityRole).toDouble(), 1.0);
}

void QMLSaleCartModelTest::testGetTotalCost()
{
    QSignalSpy totalCostChangedSpy(m_saleCartModel, &QMLSaleCartModel::totalCostChanged);

    QVariantMap itemInfo1 {
        { "category_id", 1 },
        { "category", "Category1" },
        { "item_id", 1 },
        { "item", "Item1" },
        { "quantity", 1.0 },
        { "unit_id", 1 },
        { "unit", "Unit" },
        { "cost_price", 22.0 },
        { "retail_price", 10.0 },
        { "unit_price", 22.0 },
        { "available_quantity", 10.0 }
    };

    QVariantMap itemInfo2 {
        { "category_id", 1 },
        { "category", "Category1" },
        { "item_id", 2 },
        { "item", "Item2" },
        { "quantity", 1.0 },
        { "unit_id", 1 },
        { "unit", "Unit" },
        { "cost_price", 11.0 },
        { "retail_price", 10.0 },
        { "unit_price", 11.0 },
        { "available_quantity", 10.0 }
    };

    m_saleCartModel->addItem(itemInfo1);
    m_saleCartModel->addItem(itemInfo2);
    QCOMPARE(totalCostChangedSpy.count(), 2);
    QCOMPARE(m_saleCartModel->totalCost(), 33.0);
}

void QMLSaleCartModelTest::testGetClientId()
{
    QVERIFY(m_client->initialize());

    QCOMPARE(m_saleCartModel->transactionId(), -1);

    // Push some items
    m_stockItemPusher->setCategory("Category1");
    m_stockItemPusher->setItem("Item1");
    m_stockItemPusher->setDescription("Description1");
    m_stockItemPusher->setDivisible(true);
    m_stockItemPusher->setQuantity(1.0);
    m_stockItemPusher->setUnit("Unit1");
    m_stockItemPusher->setCostPrice(2.0);
    m_stockItemPusher->setRetailPrice(3.0);
    m_stockItemPusher->push();
    QVERIFY(QTest::qWaitFor([&]() { return !m_stockItemPusher->isBusy(); }, 2000));

    QVariantMap itemInfo {
        { "category_id", 1 },
        { "category", "Category1" },
        { "item_id", 1 },
        { "item", "Item1" },
        { "quantity", 1.0 },
        { "unit_id", 1 },
        { "unit", "Unit" },
        { "cost_price", 11.0 },
        { "retail_price", 10.0 },
        { "unit_price", 13.0 },
        { "available_quantity", 10.0 }
    };

    m_saleCartModel->setCustomerName("Customer");
    m_saleCartModel->setCustomerPhoneNumber("123456789");
    m_saleCartModel->addItem(itemInfo);

    m_saleCartModel->submitTransaction();
    QVERIFY(QTest::qWaitFor([&]() { return !m_saleCartModel->isBusy(); }, 2000));
    QCOMPARE(m_saleCartModel->transactionId(), -1);

    m_saleCartModel->setTransactionId(1);
    QVERIFY(QTest::qWaitFor([&]() { return !m_saleCartModel->isBusy(); }, 2000));
    QCOMPARE(m_saleCartModel->clientId(), 1);
    QCOMPARE(m_saleCartModel->customerName(), "Customer");
    QCOMPARE(m_saleCartModel->customerPhoneNumber(), "123456789");
}

void QMLSaleCartModelTest::testRetrieveSuspendedTransaction()
{
    QVERIFY(m_client->initialize());

    // Push some items
    m_stockItemPusher->setCategory("Category1");
    m_stockItemPusher->setItem("Item1");
    m_stockItemPusher->setDescription("Description1");
    m_stockItemPusher->setDivisible(true);
    m_stockItemPusher->setQuantity(10.0);
    m_stockItemPusher->setUnit("Unit1");
    m_stockItemPusher->setCostPrice(2.0);
    m_stockItemPusher->setRetailPrice(3.0);
    m_stockItemPusher->push();
    QVERIFY(QTest::qWaitFor([&]() { return !m_stockItemPusher->isBusy(); }, 2000));

    QVariantMap itemInfo {
        { "category_id", 1 },
        { "category", "Category1" },
        { "item_id", 1 },
        { "item", "Item1" },
        { "quantity", 1.0 },
        { "unit_id", 1 },
        { "unit", "Unit1" },
        { "cost_price", 2.0 },
        { "retail_price", 3.0 },
        { "unit_price", 13.0 },
        { "available_quantity", 10.0 }
    };

    m_saleCartModel->setCustomerName("Customer");
    m_saleCartModel->addItem(itemInfo);

    m_saleCartModel->suspendTransaction();
    QVERIFY(QTest::qWaitFor([&]() { return !m_saleCartModel->isBusy(); }, 2000));

    m_saleCartModel->setTransactionId(1);
    QVERIFY(QTest::qWaitFor([&]() { return !m_saleCartModel->isBusy(); }, 2000));
    QCOMPARE(m_saleCartModel->rowCount(), 1);
    QCOMPARE(m_saleCartModel->customerName(), "Customer");
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::CategoryIdRole).toInt(), 1);
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::CategoryRole).toString(), "Category1");
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::ItemIdRole).toInt(), 1);
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::ItemRole).toString(), "Item1");
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::QuantityRole).toDouble(), 1.0);
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::UnitIdRole).toInt(), 1);
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::UnitRole).toString(), "Unit1");
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::CostPriceRole).toDouble(), 2.0);
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::RetailPriceRole).toDouble(), 3.0);
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::UnitPriceRole).toDouble(), 13.0);
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::AvailableQuantityRole).toDouble(), 10.0);
}

void QMLSaleCartModelTest::testUpdateSuspendedTransaction()
{

}

void QMLSaleCartModelTest::testSubmitTransaction()
{
    QSignalSpy rowsInsertedSpy(m_saleCartModel, &QMLSaleCartModel::rowsInserted);
    QSignalSpy successSpy(m_saleCartModel, &QMLSaleCartModel::success);
    QVERIFY(m_client->initialize());

    // Push some items
    m_stockItemPusher->setCategory("Category1");
    m_stockItemPusher->setItem("Item1");
    m_stockItemPusher->setDescription("Description1");
    m_stockItemPusher->setDivisible(true);
    m_stockItemPusher->setQuantity(1.0);
    m_stockItemPusher->setUnit("Unit1");
    m_stockItemPusher->setCostPrice(2.0);
    m_stockItemPusher->setRetailPrice(3.0);
    m_stockItemPusher->push();

    QVERIFY(QTest::qWaitFor([&]() { return !m_stockItemPusher->isBusy(); }, 2000));

    QVariantMap itemInfo {
        { "category_id", 1 },
        { "category", "Category1" },
        { "item_id", 1 },
        { "item", "Item1" },
        { "quantity", 1.0 },
        { "unit_id", 1 },
        { "unit", "Unit" },
        { "cost_price", 11.0 },
        { "retail_price", 10.0 },
        { "unit_price", 13.0 },
        { "available_quantity", 10.0 }
    };

    m_saleCartModel->setCustomerName("Customer");
    m_saleCartModel->addItem(itemInfo);
    QCOMPARE(rowsInsertedSpy.count(), 1);
    rowsInsertedSpy.clear();

    m_saleCartModel->submitTransaction();
    QVERIFY(QTest::qWaitFor([&]() { return !m_saleCartModel->isBusy(); }, 2000));
    QCOMPARE(successSpy.count(), 1);
}

void QMLSaleCartModelTest::testSuspendTransaction()
{
    QSignalSpy successSpy(m_saleCartModel, &QMLSaleCartModel::success);
    QVERIFY(m_client->initialize());

    // Push some items
    m_stockItemPusher->setCategory("Category1");
    m_stockItemPusher->setItem("Item1");
    m_stockItemPusher->setDescription("Description1");
    m_stockItemPusher->setDivisible(true);
    m_stockItemPusher->setQuantity(1.0);
    m_stockItemPusher->setUnit("Unit1");
    m_stockItemPusher->setCostPrice(2.0);
    m_stockItemPusher->setRetailPrice(3.0);
    m_stockItemPusher->push();

    QVERIFY(QTest::qWaitFor([&]() { return !m_stockItemPusher->isBusy(); }, 2000));

    QVariantMap itemInfo {
        { "category_id", 1 },
        { "category", "Category1" },
        { "item_id", 1 },
        { "item", "Item1" },
        { "quantity", 1.0 },
        { "unit_id", 1 },
        { "unit", "Unit" },
        { "cost_price", 11.0 },
        { "retail_price", 10.0 },
        { "unit_price", 13.0 },
        { "available_quantity", 10.0 }
    };

    m_saleCartModel->setCustomerName("Customer");
    m_saleCartModel->addItem(itemInfo);

    m_saleCartModel->suspendTransaction();
    QVERIFY(QTest::qWaitFor([&]() { return !m_saleCartModel->isBusy(); }, 2000));
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(m_saleCartModel->customerName(), QString());
    QCOMPARE(m_saleCartModel->rowCount(), 0);

    QSqlQuery q(m_client->connection());
    q.prepare("SELECT suspended FROM sale_transaction WHERE id = 1");
    QVERIFY(q.exec());
    QVERIFY(q.size());
    QVERIFY(q.first());
    QCOMPARE(q.value(0).toBool(), true);
}

void QMLSaleCartModelTest::testSubmitEmptyTransaction()
{
    QSignalSpy emptyCartErrorSpy(m_saleCartModel, &QMLSaleCartModel::error);

    QVERIFY(m_client->initialize());

    // Push some items
    m_stockItemPusher->setCategory("Category1");
    m_stockItemPusher->setItem("Item1");
    m_stockItemPusher->setDescription("Description1");
    m_stockItemPusher->setDivisible(true);
    m_stockItemPusher->setQuantity(10.0);
    m_stockItemPusher->setUnit("Unit1");
    m_stockItemPusher->setCostPrice(2.0);
    m_stockItemPusher->setRetailPrice(3.0);
    m_stockItemPusher->push();
    QVERIFY(QTest::qWaitFor([&]() { return !m_stockItemPusher->isBusy(); }, 2000));

    m_saleCartModel->setCustomerName("Customer");
    m_saleCartModel->setCustomerPhoneNumber("1234567890");

    m_saleCartModel->submitTransaction();
    QVERIFY(QTest::qWaitFor([&]() { return !m_saleCartModel->isBusy(); }, 2000));
    QCOMPARE(emptyCartErrorSpy.count(), 1);
    const QList<QVariant> arguments = emptyCartErrorSpy.takeFirst();
    QVERIFY(arguments.at(0).toInt() == QMLSaleCartModel::EmptyCartError);
}

void QMLSaleCartModelTest::testSuspendEmptyTransaction()
{
    QSignalSpy emptyCartErrorSpy(m_saleCartModel, &QMLSaleCartModel::error);

    QVERIFY(m_client->initialize());

    // Push some items
    m_stockItemPusher->setCategory("Category1");
    m_stockItemPusher->setItem("Item1");
    m_stockItemPusher->setDescription("Description1");
    m_stockItemPusher->setDivisible(true);
    m_stockItemPusher->setQuantity(10.0);
    m_stockItemPusher->setUnit("Unit1");
    m_stockItemPusher->setCostPrice(2.0);
    m_stockItemPusher->setRetailPrice(3.0);
    m_stockItemPusher->push();
    QVERIFY(QTest::qWaitFor([&]() { return !m_stockItemPusher->isBusy(); }, 2000));

    m_saleCartModel->setCustomerName("Customer");
    m_saleCartModel->setCustomerPhoneNumber("1234567890");

    m_saleCartModel->suspendTransaction();
    QVERIFY(QTest::qWaitFor([&]() { return !m_saleCartModel->isBusy(); }, 2000));
    QCOMPARE(emptyCartErrorSpy.count(), 1);
    const QList<QVariant> arguments = emptyCartErrorSpy.takeFirst();
    QVERIFY(arguments.at(0).toInt() == QMLSaleCartModel::EmptyCartError);
}

QTEST_MAIN(QMLSaleCartModelTest)

#include "tst_qmlsalecartmodeltest.moc"
