#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmlsalecartmodel.h"
#include "mockdatabasethread.h"
#include "utility/saleutils.h"

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
    void testClearAll();

    void testAddItem();
    void testAddSameItem();
    void testAddDifferentItems();
    void testUpdateItem();

    void testGetTotalCost();
    void testGetAmountPaid();
    void testGetClientId();

    void testAddPayment();
    void testRemovePayment();
    void testClearPayments();
    void testNoDueDateSet();

    void testSubmitTransaction();
    void testSuspendTransaction();
    void testSetTransactionId();
    void testRetrieveSuspendedTransaction();
    void testSubmitEmptyTransaction();
    void testSuspendEmptyTransaction();
    void testRemoveItem();
    void testSetItemQuantity();
private:
    QMLSaleCartModel *m_saleCartModel;
    MockDatabaseThread m_thread;
    QueryResult m_result;
};

QMLSaleCartModelTest::QMLSaleCartModelTest() :
    m_thread(&m_result)
{
    QLoggingCategory::setFilterRules(QStringLiteral("*.info=false"));
}

void QMLSaleCartModelTest::init()
{
    m_saleCartModel = new QMLSaleCartModel(m_thread, this);
}

void QMLSaleCartModelTest::cleanup()
{
}

void QMLSaleCartModelTest::testSetCustomerName()
{
    QSignalSpy customerNameChangedSpy(m_saleCartModel, &QMLSaleCartModel::customerNameChanged);

    // STEP: Ensure customer name is not set.
    QCOMPARE(m_saleCartModel->customerName(), QString());

    // STEP: Set customer name.
    m_saleCartModel->setCustomerName(QStringLiteral("Customer"));

    // STEP: Ensure customer name is set and user is notified.
    QCOMPARE(m_saleCartModel->customerName(), QStringLiteral("Customer"));
    QCOMPARE(customerNameChangedSpy.count(), 1);
    customerNameChangedSpy.clear();

    // STEP: Ensure user is not notified if customer name is set to the same value.
    m_saleCartModel->setCustomerName(QStringLiteral("Customer"));
    QCOMPARE(customerNameChangedSpy.count(), 0);
}

void QMLSaleCartModelTest::testSetCustomerPhoneNumber()
{
    QSignalSpy customerPhoneNumberSpy(m_saleCartModel, &QMLSaleCartModel::customerPhoneNumberChanged);

    // STEP: Ensure customer phone number is not set.
    QCOMPARE(m_saleCartModel->customerPhoneNumber(), QString());

    // STEP: Set customer phone number.
    m_saleCartModel->setCustomerPhoneNumber("12345");

    // STEP: Ensure customer phone number is set and user is notified.
    QCOMPARE(m_saleCartModel->customerPhoneNumber(), "12345");
    QCOMPARE(customerPhoneNumberSpy.count(), 1);
    customerPhoneNumberSpy.clear();

    // STEP: Ensure user is not notified if customer phone number is set to the same value.
    m_saleCartModel->setCustomerPhoneNumber("12345");
    QCOMPARE(customerPhoneNumberSpy.count(), 0);
}

void QMLSaleCartModelTest::testSetNote()
{
    QSignalSpy noteChangedSpy(m_saleCartModel, &QMLSaleCartModel::noteChanged);

    // STEP: Ensure note is not set.
    QCOMPARE(m_saleCartModel->note(), QString());

    // STEP: Set note.
    m_saleCartModel->setNote("Note");

    // STEP: Ensure note is set and user is notified.
    QCOMPARE(m_saleCartModel->note(), "Note");
    QCOMPARE(noteChangedSpy.count(), 1);
    noteChangedSpy.clear();

    // STEP: Ensure user is not notified if note is set to the same value.
    m_saleCartModel->setNote("Note");
    QCOMPARE(noteChangedSpy.count(), 0);
}

void QMLSaleCartModelTest::testClearAll()
{
    const QVariantMap itemInfo {
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

    // STEP: Add an item to the model.
    m_saleCartModel->setCustomerName("Customer");
    m_saleCartModel->setCustomerPhoneNumber("123456789");
    m_saleCartModel->addItem(itemInfo);

    // STEP: Clear all items in the model.
    m_saleCartModel->clearAll();

    // STEP: Ensure all items are cleared from the model.
    QCOMPARE(m_saleCartModel->customerName(), QString());
    QCOMPARE(m_saleCartModel->customerPhoneNumber(), QString());
    QCOMPARE(m_saleCartModel->rowCount(), 0);
}

void QMLSaleCartModelTest::testAddItem()
{
    QSignalSpy rowsInsertedSpy(m_saleCartModel, &QMLSaleCartModel::rowsInserted);

    const QVariantMap itemInfo {
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

    // STEP: Add item to model.
    m_saleCartModel->addItem(itemInfo);

    // STEP: Ensure item was added and user was notified.
    QCOMPARE(rowsInsertedSpy.count(), 1);
    QCOMPARE(m_saleCartModel->rowCount(), 1);
}

void QMLSaleCartModelTest::testAddSameItem()
{
    QSignalSpy rowsInsertedSpy(m_saleCartModel, &QMLSaleCartModel::rowsInserted);
    QSignalSpy dataChangedSpy(m_saleCartModel, &QMLSaleCartModel::dataChanged);
    const QVariantMap itemInfo {
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

    // STEP: Add an item to the model.
    m_saleCartModel->addItem(itemInfo);

    // STEP: Add the same item to the model.
    m_saleCartModel->addItem(itemInfo);

    // STEP: Ensure one row was added to the model, and the item has its quantity set to 2.
    QCOMPARE(rowsInsertedSpy.count(), 1);
    QCOMPARE(dataChangedSpy.count(), 1);
    QCOMPARE(m_saleCartModel->rowCount(), 1);
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::QuantityRole).toDouble(), 2.0);
}

void QMLSaleCartModelTest::testAddDifferentItems()
{
    QSignalSpy rowsInsertedSpy(m_saleCartModel, &QMLSaleCartModel::rowsInserted);
    const QVariantMap itemInfo1 {
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
    const QVariantMap itemInfo2 {
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

    // STEP: Add the same item twice to the model.
    m_saleCartModel->addItem(itemInfo1);
    m_saleCartModel->addItem(itemInfo1);

    // STEP: Add a different item to the model.
    m_saleCartModel->addItem(itemInfo2);

    // STEP: Ensure that there are two rows: one row with a quantity of 2, and the other with a quantity of 1.
    QCOMPARE(rowsInsertedSpy.count(), 2);
    QCOMPARE(m_saleCartModel->rowCount(), 2);
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::QuantityRole).toDouble(), 2.0);
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(1), QMLSaleCartModel::QuantityRole).toDouble(), 1.0);
}

void QMLSaleCartModelTest::testUpdateItem()
{
    QSignalSpy dataChangedSpy(m_saleCartModel, &QMLSaleCartModel::dataChanged);

    const QVariantMap itemInfo {
        { "category_id", 1 },
        { "category", "Category" },
        { "item_id", 1 },
        { "item", "Item" },
        { "quantity", 1.0 },
        { "unit_id", 1 },
        { "unit", "Unit" },
        { "unit_price", 13.0 },
        { "available_quantity", 10.0 }
    };

    // STEP: Add item to the model
    m_saleCartModel->setCustomerName("Customer");
    m_saleCartModel->setCustomerPhoneNumber("1234567890");
    m_saleCartModel->addItem(itemInfo);
    QCOMPARE(m_saleCartModel->rowCount(), 1);

    const QVariantMap itemUpdateInfo {
        { "quantity", 4.0 },
        { "unit_price", 30.0 },
        { "cost", 120.0 }
    };

    // STEP: Update item details.
    m_saleCartModel->updateItem(1, itemUpdateInfo);
    QCOMPARE(dataChangedSpy.count(), 1);

    // STEP: Ensure model is updated properly.
    QCOMPARE(m_saleCartModel->customerName(), "Customer");
    QCOMPARE(m_saleCartModel->customerPhoneNumber(), "1234567890");
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::ItemIdRole).toInt(), 1);
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::ItemRole).toString(), QStringLiteral("Item"));
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::QuantityRole).toDouble(), 4.0);
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::UnitPriceRole).toDouble(), 30.0);
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::CostRole).toDouble(), 120.0);
}

void QMLSaleCartModelTest::testGetTotalCost()
{
    QSignalSpy totalCostChangedSpy(m_saleCartModel, &QMLSaleCartModel::totalCostChanged);
    const QVariantMap itemInfo1 {
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
    const QVariantMap itemInfo2 {
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

    // STEP: Add 2 distinct items to the model.
    m_saleCartModel->addItem(itemInfo1);
    m_saleCartModel->addItem(itemInfo2);

    // STEP: Ensure that the total cost is updated properly.
    QCOMPARE(totalCostChangedSpy.count(), 2);
    QCOMPARE(m_saleCartModel->totalCost(), 33.0);
}

void QMLSaleCartModelTest::testGetAmountPaid()
{
    QSignalSpy amountPaidChangedSpy(m_saleCartModel, &QMLSaleCartModel::amountPaidChanged);

    // STEP: Add payments.
    m_saleCartModel->addPayment(3.0, QMLSaleCartModel::Cash);
    m_saleCartModel->addPayment(1.5, QMLSaleCartModel::Cash);

    // STEP: Ensure that the amount paid is updated properly.
    QCOMPARE(amountPaidChangedSpy.count(), 2);
    QCOMPARE(m_saleCartModel->amountPaid(), 4.5);
}

void QMLSaleCartModelTest::testGetClientId()
{
    const QVariantMap itemInfo {
        { "category_id", 1 },
        { "category", "Category" },
        { "item_id", 1 },
        { "item", "Item" },
        { "quantity", 1.0 },
        { "unit_id", 1 },
        { "unit", "Unit" },
        { "cost_price", 11.0 },
        { "retail_price", 10.0 },
        { "unit_price", 13.0 },
        { "available_quantity", 10.0 }
    };

    auto databaseWillReturnEmptyResult = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());
    };
    auto databaseWillReturnSubmittedTransaction = [this, &itemInfo]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());

        m_result.setOutcome(QVariantMap {
                                { "client_id", 1 },
                                { "customer_name", QStringLiteral("Customer") },
                                { "customer_phone_number", QStringLiteral("123456789") },
                                { "items", QVariantList { { itemInfo } } }
                            });
    };
    QSignalSpy successSpy(m_saleCartModel, &QMLSaleCartModel::success);
    QSignalSpy errorSpy(m_saleCartModel, &QMLSaleCartModel::error);
    QSignalSpy busyChangedSpy(m_saleCartModel, &QMLSaleCartModel::busyChanged);

    // STEP: Ensure transaction ID is not set.
    QCOMPARE(m_saleCartModel->transactionId(), -1);

    // STEP: Set the mandatory fields (customer name and phone number).
    m_saleCartModel->setCustomerName("Customer");
    m_saleCartModel->setCustomerPhoneNumber("123456789");

    // STEP: Sell one item to the client.
    m_saleCartModel->addItem(itemInfo);
    m_saleCartModel->addPayment(13.0, QMLSaleCartModel::Cash);

    databaseWillReturnEmptyResult();

    m_saleCartModel->submitTransaction();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLSaleCartModel::SuccessCode>(), QMLSaleCartModel::SubmitTransactionSuccess);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_saleCartModel->transactionId(), -1);

    databaseWillReturnSubmittedTransaction();

    // STEP: Retrieve transaction.
    m_saleCartModel->setTransactionId(1);
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);

    // STEP: Ensure that the correct values are returned to the model.
    QCOMPARE(m_saleCartModel->clientId(), 1);
    QCOMPARE(m_saleCartModel->customerName(), "Customer");
    QCOMPARE(m_saleCartModel->customerPhoneNumber(), "123456789");
    QCOMPARE(m_saleCartModel->rowCount(), 1);
    QCOMPARE(m_saleCartModel->index(0).data(QMLSaleCartModel::CategoryIdRole).toInt(), 1);
    QCOMPARE(m_saleCartModel->index(0).data(QMLSaleCartModel::CategoryRole).toString(), QStringLiteral("Category"));
    QCOMPARE(m_saleCartModel->index(0).data(QMLSaleCartModel::ItemIdRole).toInt(), 1);
    QCOMPARE(m_saleCartModel->index(0).data(QMLSaleCartModel::ItemRole).toString(), QStringLiteral("Item"));
    QCOMPARE(m_saleCartModel->index(0).data(QMLSaleCartModel::QuantityRole).toDouble(), 1.0);
    QCOMPARE(m_saleCartModel->index(0).data(QMLSaleCartModel::AvailableQuantityRole).toDouble(), 10.0);
    QCOMPARE(m_saleCartModel->index(0).data(QMLSaleCartModel::UnitIdRole).toInt(), 1);
    QCOMPARE(m_saleCartModel->index(0).data(QMLSaleCartModel::UnitRole).toString(), QStringLiteral("Unit"));
    QCOMPARE(m_saleCartModel->index(0).data(QMLSaleCartModel::CostPriceRole).toDouble(), 11.0);
    QCOMPARE(m_saleCartModel->index(0).data(QMLSaleCartModel::RetailPriceRole).toDouble(), 10.0);
    QCOMPARE(m_saleCartModel->index(0).data(QMLSaleCartModel::UnitPriceRole).toDouble(), 13.0);

}

void QMLSaleCartModelTest::testAddPayment()
{
    QSignalSpy amountPaidChangedSpy(m_saleCartModel, &QMLSaleCartModel::amountPaidChanged);

    // STEP: Add payments.
    m_saleCartModel->addPayment(9.0, QMLSaleCartModel::Cash);
    m_saleCartModel->addPayment(1.0, QMLSaleCartModel::DebitCard);
    m_saleCartModel->addPayment(3.0, QMLSaleCartModel::CreditCard);

    QCOMPARE(amountPaidChangedSpy.count(), 3);

    // STEP: Ensure payments were added properly.
    QCOMPARE(m_saleCartModel->payments().count(), 3);
    QCOMPARE(m_saleCartModel->payments().at(0).amount, 9.0);
    QCOMPARE(m_saleCartModel->payments().at(0).method, static_cast<SalePayment::PaymentMethod>(QMLSaleCartModel::Cash));
    QCOMPARE(m_saleCartModel->payments().at(1).amount, 1.0);
    QCOMPARE(m_saleCartModel->payments().at(1).method, static_cast<SalePayment::PaymentMethod>(QMLSaleCartModel::DebitCard));
    QCOMPARE(m_saleCartModel->payments().at(2).amount, 3.0);
    QCOMPARE(m_saleCartModel->payments().at(2).method, static_cast<SalePayment::PaymentMethod>(QMLSaleCartModel::CreditCard));

    // STEP: Ensure amount paid was updated properly.
    QCOMPARE(m_saleCartModel->amountPaid(), 13.0);
}

void QMLSaleCartModelTest::testRemovePayment()
{
    QSignalSpy amountPaidChangedSpy(m_saleCartModel, &QMLSaleCartModel::amountPaidChanged);

    // STEP: Add payments.
    m_saleCartModel->addPayment(9.0, QMLSaleCartModel::Cash);
    m_saleCartModel->addPayment(1.0, QMLSaleCartModel::DebitCard);
    m_saleCartModel->addPayment(3.0, QMLSaleCartModel::CreditCard);
    amountPaidChangedSpy.clear();

    // STEP: Ensure amount paid was updated properly.
    QCOMPARE(m_saleCartModel->amountPaid(), 13.0);

    // STEP: Remove first payment.
    m_saleCartModel->removePayment(0);

    // STEP: Ensure payment was removed properly.
    QCOMPARE(m_saleCartModel->payments().count(), 2);
    QCOMPARE(m_saleCartModel->payments().at(0).amount, 1.0);
    QCOMPARE(m_saleCartModel->payments().at(0).method, static_cast<SalePayment::PaymentMethod>(QMLSaleCartModel::DebitCard));
    QCOMPARE(m_saleCartModel->payments().at(1).amount, 3.0);
    QCOMPARE(m_saleCartModel->payments().at(1).method, static_cast<SalePayment::PaymentMethod>(QMLSaleCartModel::CreditCard));

    // STEP: Ensure amount paid was updated properly.
    QCOMPARE(m_saleCartModel->amountPaid(), 4.0);
    QCOMPARE(amountPaidChangedSpy.count(), 1);
}

void QMLSaleCartModelTest::testClearPayments()
{
    QSignalSpy amountPaidChangedSpy(m_saleCartModel, &QMLSaleCartModel::amountPaidChanged);

    // STEP: Add payments.
    m_saleCartModel->addPayment(9.0, QMLSaleCartModel::Cash);
    m_saleCartModel->addPayment(1.0, QMLSaleCartModel::DebitCard);
    m_saleCartModel->addPayment(3.0, QMLSaleCartModel::CreditCard);
    amountPaidChangedSpy.clear();

    // STEP: Clear payments.
    m_saleCartModel->clearPayments();

    // STEP: Ensure payments were cleared.
    QCOMPARE(m_saleCartModel->payments().count(), 0);

    // STEP: Ensure amount paid was updated properly.
    QCOMPARE(m_saleCartModel->amountPaid(), 0);
    QCOMPARE(amountPaidChangedSpy.count(), 1);
}

void QMLSaleCartModelTest::testRetrieveSuspendedTransaction()
{
    const QVariantMap itemInfo {
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
    auto databaseWillReturnEmptyResult = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());
    };
    auto databaseWillReturnSubmittedTransaction = [this, &itemInfo]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());

        m_result.setOutcome(QVariantMap {
                                { "client_id", 1 },
                                { "customer_name", QStringLiteral("Customer") },
                                { "customer_phone_number", QStringLiteral("123456789") },
                                { "items", QVariantList { { itemInfo } } }
                            });
    };

    QSignalSpy errorSpy(m_saleCartModel, &QMLSaleCartModel::error);
    QSignalSpy successSpy(m_saleCartModel, &QMLSaleCartModel::success);
    QSignalSpy busyChangedSpy(m_saleCartModel, &QMLSaleCartModel::busyChanged);

    // STEP: Add item to the model.
    m_saleCartModel->setCustomerName("Customer");
    m_saleCartModel->addItem(itemInfo);

    databaseWillReturnEmptyResult();

    // STEP: Suspend transaction.
    m_saleCartModel->suspendTransaction();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLSaleCartModel::SuccessCode>(), QMLSaleCartModel::SuspendTransactionSuccess);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);

    databaseWillReturnSubmittedTransaction();

    // STEP: Retrieve suspended transaction.
    m_saleCartModel->setTransactionId(1);
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLSaleCartModel::SuccessCode>(), QMLSaleCartModel::RetrieveTransactionSuccess);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);

    // STEP: Ensure that model is re-populated correctly.
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

void QMLSaleCartModelTest::testRemoveItem()
{
    const QVariantMap itemInfo {
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

    // STEP: Add item to the model.
    m_saleCartModel->addItem(itemInfo);
    QCOMPARE(m_saleCartModel->rowCount(), 1);

    // STEP: Remove item from the model.
    m_saleCartModel->removeItem(1);
    QCOMPARE(m_saleCartModel->rowCount(), 0);
}

void QMLSaleCartModelTest::testSetItemQuantity()
{
    const QVariantMap itemInfo {
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

    // STEP: Add item to the model.
    m_saleCartModel->addItem(itemInfo);
    QCOMPARE(m_saleCartModel->rowCount(), 1);

    // STEP: Ensure a quantity of 1 is set.
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::QuantityRole).toDouble(), 1.0);

    // STEP: Set the quantity to an invalid value.
    m_saleCartModel->setItemQuantity(1, 0.0);
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::QuantityRole).toDouble(), 1.0);
    m_saleCartModel->setItemQuantity(1, -1.0);
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::QuantityRole).toDouble(), 1.0);

    // STEP: Set the quantity to a value greater than the available quantity. Ensure the value caps at
    // the available quantity.
    m_saleCartModel->setItemQuantity(1, 11.0);
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::QuantityRole).toDouble(), 10.0);

    // STEP: Set the quantity to an valid value.
    m_saleCartModel->setItemQuantity(1, 5.5);
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::QuantityRole).toDouble(), 5.5);
}

void QMLSaleCartModelTest::testNoDueDateSet()
{
    auto databaseWillReturnEmptyResult = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());
    };
    QSignalSpy successSpy(m_saleCartModel, &QMLSaleCartModel::success);
    QSignalSpy errorSpy(m_saleCartModel, &QMLSaleCartModel::error);
    QSignalSpy busyChangedSpy(m_saleCartModel, &QMLSaleCartModel::busyChanged);

    const QVariantMap itemInfo {
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

    // STEP: Add item to the model.
    m_saleCartModel->setCustomerName("Customer");
    m_saleCartModel->addItem(itemInfo);

    databaseWillReturnEmptyResult();

    // STEP: Submit transaction.
    m_saleCartModel->submitTransaction();
    QCOMPARE(errorSpy.count(), 1);
    QCOMPARE(errorSpy.takeFirst().first().value<QMLSaleCartModel::ErrorCode>(), QMLSaleCartModel::NoDueDateSetError);
    QCOMPARE(successSpy.count(), 0);
}

void QMLSaleCartModelTest::testSubmitTransaction()
{
    auto databaseWillReturnEmptyResult = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());
    };

    QSignalSpy rowsInsertedSpy(m_saleCartModel, &QMLSaleCartModel::rowsInserted);
    QSignalSpy successSpy(m_saleCartModel, &QMLSaleCartModel::success);
    QSignalSpy errorSpy(m_saleCartModel, &QMLSaleCartModel::error);
    QSignalSpy busyChangedSpy(m_saleCartModel, &QMLSaleCartModel::busyChanged);

    const QVariantMap itemInfo {
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

    // STEP: Add item to the model.
    m_saleCartModel->setCustomerName("Customer");
    m_saleCartModel->addItem(itemInfo);
    m_saleCartModel->addPayment(13.0, QMLSaleCartModel::Cash);
    QCOMPARE(rowsInsertedSpy.count(), 1);
    rowsInsertedSpy.clear();

    databaseWillReturnEmptyResult();

    // STEP: Submit transaction.
    m_saleCartModel->submitTransaction();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLSaleCartModel::SuccessCode>(), QMLSaleCartModel::SubmitTransactionSuccess);
    QCOMPARE(errorSpy.count(), 0);
}

void QMLSaleCartModelTest::testSuspendTransaction()
{
    auto databaseWillReturnEmptyResult = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());
    };
    QSignalSpy successSpy(m_saleCartModel, &QMLSaleCartModel::success);
    QSignalSpy errorSpy(m_saleCartModel, &QMLSaleCartModel::error);

    const QVariantMap itemInfo {
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

    // STEP: Add an item to the model.
    m_saleCartModel->setCustomerName(QStringLiteral("Customer"));
    m_saleCartModel->addItem(itemInfo);

    databaseWillReturnEmptyResult();

    // STEP: Suspend transaction.
    m_saleCartModel->suspendTransaction();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLSaleCartModel::SuccessCode>(), QMLSaleCartModel::SuspendTransactionSuccess);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_saleCartModel->customerName(), QString());
    QCOMPARE(m_saleCartModel->rowCount(), 0);
}

void QMLSaleCartModelTest::testSetTransactionId()
{
    const QVariantMap itemInfo {
        { "category_id", 1 },
        { "category", "Category1" },
        { "item_id", 1 },
        { "item", "Item1" },
        { "quantity", 1.0 },
        { "unit_id", 1 },
        { "unit", "Unit" },
        { "unit_price", 13.0 },
        { "available_quantity", 10.0 }
    };
    auto databaseWillReturnEmptyResult = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());
    };
    auto databaseWillReturnSubmittedTransaction = [this, &itemInfo]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());

        m_result.setOutcome(QVariantMap {
                                { "client_id", 1 },
                                { "customer_name", QStringLiteral("Customer") },
                                { "customer_phone_number", QStringLiteral("123456789") },
                                { "items", QVariantList { { itemInfo } } }
                            });
    };
    QSignalSpy successSpy(m_saleCartModel, &QMLSaleCartModel::success);
    QSignalSpy errorSpy(m_saleCartModel, &QMLSaleCartModel::error);
    QSignalSpy busyChangedSpy(m_saleCartModel, &QMLSaleCartModel::busyChanged);
    QSignalSpy transactionIdChangedSpy(m_saleCartModel, &QMLSaleCartModel::transactionIdChanged);

    // STEP: Ensure transaction ID is not set.
    QCOMPARE(m_saleCartModel->transactionId(), -1);

    // STEP: Add item to the model.
    m_saleCartModel->setCustomerName(QStringLiteral("Customer"));
    m_saleCartModel->setCustomerPhoneNumber(QStringLiteral("123456789"));
    m_saleCartModel->addItem(itemInfo);
    m_saleCartModel->addPayment(13.0, QMLSaleCartModel::Cash);

    databaseWillReturnEmptyResult();

    // STEP: Submit transaction.
    m_saleCartModel->submitTransaction();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLSaleCartModel::SuccessCode>(), QMLSaleCartModel::SubmitTransactionSuccess);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_saleCartModel->rowCount(), 0);

    databaseWillReturnSubmittedTransaction();

    // STEP: Retrieve transaction
    m_saleCartModel->setTransactionId(1);
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLSaleCartModel::SuccessCode>(), QMLSaleCartModel::RetrieveTransactionSuccess);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_saleCartModel->transactionId(), 1);
    QCOMPARE(transactionIdChangedSpy.count(), 1);
    transactionIdChangedSpy.clear();
    QCOMPARE(m_saleCartModel->rowCount(), 1);

    // STEP: Ensure user is not notified when transaction ID was set again.
    m_saleCartModel->setTransactionId(1);
    QCOMPARE(transactionIdChangedSpy.count(), 0);
}


void QMLSaleCartModelTest::testSubmitEmptyTransaction()
{
    auto databaseWillReturnEmptyResult = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());
    };
    QSignalSpy successSpy(m_saleCartModel, &QMLSaleCartModel::success);
    QSignalSpy errorSpy(m_saleCartModel, &QMLSaleCartModel::error);

    // STEP: Set mandatory fields.
    m_saleCartModel->setCustomerName(QStringLiteral("Customer"));
    m_saleCartModel->setCustomerPhoneNumber(QStringLiteral("1234567890"));

    databaseWillReturnEmptyResult();

    // STEP: Submit empty transaction and expect an error.
    m_saleCartModel->submitTransaction();
    QCOMPARE(successSpy.count(), 0);
    QCOMPARE(errorSpy.count(), 1);
    QCOMPARE(errorSpy.takeFirst().first().value<QMLSaleCartModel::ErrorCode>(), QMLSaleCartModel::EmptyCartError);
}

void QMLSaleCartModelTest::testSuspendEmptyTransaction()
{
    auto databaseWillReturnEmptyResult = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());
    };
    QSignalSpy successSpy(m_saleCartModel, &QMLSaleCartModel::success);
    QSignalSpy errorSpy(m_saleCartModel, &QMLSaleCartModel::error);
    QSignalSpy busyChangedSpy(m_saleCartModel, &QMLSaleCartModel::busyChanged);

    // STEP: Set mandatory fields.
    m_saleCartModel->setCustomerName(QStringLiteral("Customer"));
    m_saleCartModel->setCustomerPhoneNumber(QStringLiteral("1234567890"));

    databaseWillReturnEmptyResult();

    // STEP: Suspend transaction.
    m_saleCartModel->suspendTransaction();
    QCOMPARE(errorSpy.count(), 1);
    QCOMPARE(errorSpy.takeFirst().first().value<QMLSaleCartModel::ErrorCode>(), QMLSaleCartModel::EmptyCartError);
    QCOMPARE(successSpy.count(), 0);
}

QTEST_MAIN(QMLSaleCartModelTest)

#include "tst_qmlsalecartmodeltest.moc"
