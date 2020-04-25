#include "mockdatabasethread.h"
#include "qmlapi/qmlsalecartmodel.h"
#include "utility/saleutils.h"
#include <QCoreApplication>
#include <QString>
#include <QtTest>

class QMLSaleCartModelTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void init();
    void cleanup();

    void testSetCustomerName();
    void testSetCustomerPhoneNumber();
    void testSetNote();
    void testClearAll();

    void testAddProduct();
    void testAddSameProduct();
    void testAddDifferentProducts();
    void testUpdateProduct();

    void testGetTotalCost();
    void testGetAmountPaid();
    void testGetCustomerId();

    void testNoDueDateSet();

    void testSubmitTransaction();
    void testSuspendTransaction();
    void testSetTransactionId();
    void testRetrieveSuspendedTransaction();
    void testSubmitEmptyTransaction();
    void testSuspendEmptyTransaction();
    void testRemoveProduct();
    void testSetProductQuantity();
private:
    QMLSaleCartModel *m_saleCartModel;
    MockDatabaseThread *m_thread;
};

void QMLSaleCartModelTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_saleCartModel = new QMLSaleCartModel(*m_thread, this);
}

void QMLSaleCartModelTest::cleanup()
{
    m_saleCartModel->deleteLater();
    m_thread->deleteLater();
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
    const QVariantMap customer {
        { "customer_name", "Customer" },
        { "customer_phone_number", "123456789"}
    };
    const QVariantMap product {
        { "category_id", 1 },
        { "category", "Category1" },
        { "product_id", 1 },
        { "product", "Product1" },
        { "quantity", 1.0 },
        { "unit_id", 1 },
        { "unit", "Unit" },
        { "cost_price", 11.0 },
        { "retail_price", 10.0 },
        { "unit_price", 13.0 },
        { "available_quantity", 10.0 }
    };
    QSignalSpy modelResetSpy(m_saleCartModel, &QMLSaleCartModel::modelReset);

    m_saleCartModel->setCustomerName(customer["customer_name"].toString());
    m_saleCartModel->setCustomerPhoneNumber(customer["customer_phone_number"].toString());
    m_saleCartModel->addProduct(product);
    QCOMPARE(m_saleCartModel->rowCount(), 1);

    m_saleCartModel->clearAll();

    QCOMPARE(modelResetSpy.count(), 1);
    QCOMPARE(m_saleCartModel->customerName(), QString());
    QCOMPARE(m_saleCartModel->customerPhoneNumber(), QString());
    QCOMPARE(m_saleCartModel->rowCount(), 0);
}

void QMLSaleCartModelTest::testAddProduct()
{
    QSignalSpy rowsInsertedSpy(m_saleCartModel, &QMLSaleCartModel::rowsInserted);

    const QVariantMap product {
        { "product_category_id", 1 },
        { "product_category", "Category1" },
        { "product_id", 1 },
        { "product", "Product1" },
        { "quantity", 1.0 },
        { "product_unit_id", 1 },
        { "product_unit", "Unit" },
        { "cost_price", 11.0 },
        { "retail_price", 10.0 },
        { "unit_price", 13.0 },
        { "available_quantity", 10.0 }
    };

    // STEP: Add product to model.
    m_saleCartModel->addProduct(product);

    // STEP: Ensure product was added and user was notified.
    QCOMPARE(rowsInsertedSpy.count(), 1);
    QCOMPARE(m_saleCartModel->rowCount(), 1);
}

void QMLSaleCartModelTest::testAddSameProduct()
{
    QSignalSpy rowsInsertedSpy(m_saleCartModel, &QMLSaleCartModel::rowsInserted);
    QSignalSpy dataChangedSpy(m_saleCartModel, &QMLSaleCartModel::dataChanged);
    const QVariantMap productInfo {
        { "product_category_id", 1 },
        { "product_category", "Category1" },
        { "product_id", 1 },
        { "product", "Product1" },
        { "quantity", 2.0 },
        { "product_unit_id", 1 },
        { "product_unit", "Unit" },
        { "cost_price", 11.0 },
        { "retail_price", 10.0 },
        { "unit_price", 13.0 },
        { "available_quantity", 10.0 }
    };

    // STEP: Add an product to the model.
    m_saleCartModel->addProduct(productInfo);
    QCOMPARE(rowsInsertedSpy.count(), 1);
    QCOMPARE(m_saleCartModel->rowCount(), 1);
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::QuantityRole).toDouble(), 2.0);

    // STEP: Add the same product to the model.
    m_saleCartModel->addProduct(productInfo);
    QCOMPARE(dataChangedSpy.count(), 1);
    QCOMPARE(m_saleCartModel->rowCount(), 1);
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::QuantityRole).toDouble(), 4.0);
}

void QMLSaleCartModelTest::testAddDifferentProducts()
{
    QSignalSpy rowsInsertedSpy(m_saleCartModel, &QMLSaleCartModel::rowsInserted);
    const QVariantMap productInfo1 {
        { "category_id", 1 },
        { "category", "Category1" },
        { "product_id", 1 },
        { "product", "Product1" },
        { "quantity", 1.0 },
        { "unit_id", 1 },
        { "unit", "Unit" },
        { "cost_price", 11.0 },
        { "retail_price", 10.0 },
        { "unit_price", 13.0 },
        { "available_quantity", 10.0 }
    };
    const QVariantMap productInfo2 {
        { "category_id", 1 },
        { "category", "Category1" },
        { "product_id", 2 },
        { "product", "Item2" },
        { "quantity", 1.0 },
        { "unit_id", 1 },
        { "unit", "Unit" },
        { "cost_price", 11.0 },
        { "retail_price", 10.0 },
        { "unit_price", 13.0 },
        { "available_quantity", 10.0 }
    };

    // STEP: Add the same product twice to the model.
    m_saleCartModel->addProduct(productInfo1);
    m_saleCartModel->addProduct(productInfo1);

    // STEP: Add a different product to the model.
    m_saleCartModel->addProduct(productInfo2);

    // STEP: Ensure that there are two rows: one row with a quantity of 2, and the other with a quantity of 1.
    QCOMPARE(rowsInsertedSpy.count(), 2);
    QCOMPARE(m_saleCartModel->rowCount(), 2);
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::QuantityRole).toDouble(), 2.0);
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(1), QMLSaleCartModel::QuantityRole).toDouble(), 1.0);
}

void QMLSaleCartModelTest::testUpdateProduct()
{
    QSignalSpy dataChangedSpy(m_saleCartModel, &QMLSaleCartModel::dataChanged);
    const QVariantMap customer {
        { "customer_name", QStringLiteral("Customer") },
        { "customer_phone_number", QStringLiteral("1234567890") }
    };
    const QVariantMap product {
        { "product_category_id", 1 },
        { "product_category", "Category" },
        { "product_id", 1 },
        { "product", "Product" },
        { "quantity", 1.0 },
        { "product_unit_id", 1 },
        { "product_unit", "Unit" },
        { "unit_price", 13.0 },
        { "available_quantity", 10.0 }
    };

    // STEP: Add product to the model
    m_saleCartModel->setCustomerName(customer["customer_name"].toString());
    m_saleCartModel->setCustomerPhoneNumber(customer["customer_phone_number"].toString());
    m_saleCartModel->addProduct(product);
    QCOMPARE(m_saleCartModel->rowCount(), 1);

    const QVariantMap productUpdate {
        { "product_id", 1 },
        { "quantity", 4.0 },
        { "unit_price", 30.0 },
        { "cost", 120.0 }
    };

    // STEP: Update product details.
    m_saleCartModel->updateProduct(product["product_id"].toInt(), productUpdate);
    QCOMPARE(dataChangedSpy.count(), 1);

    // STEP: Ensure model is updated properly.
    QCOMPARE(m_saleCartModel->customerName(),
             customer["customer_name"].toString());
    QCOMPARE(m_saleCartModel->customerPhoneNumber(),
             customer["customer_phone_number"].toString());
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0),QMLSaleCartModel::ProductIdRole).toInt(),
             product["product_id"].toInt());
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::ProductRole).toString(),
             product["product"].toString());
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::QuantityRole).toDouble(),
             productUpdate["quantity"].toDouble());
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::UnitPriceRole).toDouble(),
             productUpdate["unit_price"].toDouble());
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::CostRole).toDouble(),
             productUpdate["cost"].toDouble());
}

void QMLSaleCartModelTest::testGetTotalCost()
{
    const QVariantMap product1 {
        { "category_id", 1 },
        { "category", "Category1" },
        { "product_id", 1 },
        { "product", "Product1" },
        { "quantity", 1.0 },
        { "unit_id", 1 },
        { "unit", "Unit" },
        { "cost_price", 22.0 },
        { "retail_price", 10.0 },
        { "unit_price", 22.0 },
        { "available_quantity", 10.0 },
        { "cost", 22.0 }
    };
    const QVariantMap product2 {
        { "category_id", 1 },
        { "category", "Category1" },
        { "product_id", 2 },
        { "product", "Product2" },
        { "quantity", 4.0 },
        { "unit_id", 2 },
        { "unit", "Unit" },
        { "cost_price", 11.0 },
        { "retail_price", 10.0 },
        { "unit_price", 11.0 },
        { "available_quantity", 10.0 },
        { "cost", 44.0 }
    };
    QSignalSpy totalCostChangedSpy(m_saleCartModel, &QMLSaleCartModel::totalCostChanged);

    m_saleCartModel->addProduct(product1);
    m_saleCartModel->addProduct(product2);

    QCOMPARE(totalCostChangedSpy.count(), 2);
    QCOMPARE(m_saleCartModel->totalCost(), product1["cost"].toDouble() + product2["cost"].toDouble());
}

void QMLSaleCartModelTest::testGetAmountPaid()
{
    QSignalSpy amountPaidChangedSpy(m_saleCartModel, &QMLSaleCartModel::amountPaidChanged);
    const auto amountPaid = 12.35;

    QCOMPARE(m_saleCartModel->amountPaid(), 0.0);
    m_saleCartModel->setAmountPaid(0.0);
    QCOMPARE(amountPaidChangedSpy.count(), 0);

    m_saleCartModel->setAmountPaid(amountPaid);
    QCOMPARE(m_saleCartModel->amountPaid(), amountPaid);
    QCOMPARE(amountPaidChangedSpy.count(), 1);
    amountPaidChangedSpy.clear();

    m_saleCartModel->setAmountPaid(amountPaid);
    QCOMPARE(amountPaidChangedSpy.count(), 0);
}

void QMLSaleCartModelTest::testGetCustomerId()
{
    const QVariantMap customer {
        { "client_id", 1 },
        { "customer_name", QStringLiteral("Customer") },
        { "customer_phone_number", QStringLiteral("123456789") }
    };
    const QVariantMap product {
        { "sale_transaction_id", 1 },
        { "category_id", 1 },
        { "category", QStringLiteral("Category") },
        { "product_id", 1 },
        { "product", QStringLiteral("Product") },
        { "quantity", 1.0 },
        { "product_unit_id", 1 },
        { "product_unit", QStringLiteral("Unit") },
        { "cost_price", 11.0 },
        { "retail_price", 10.0 },
        { "unit_price", 13.0 },
        { "available_quantity", 10.0 }
    };

    auto databaseWillReturnEmptyResult = [this]() {
        m_thread->result().setSuccessful(true);
    };
    auto databaseWillReturn = [this](const QVariantMap &customer, const QVariantList &products) {
        m_thread->result().setSuccessful(true);
        m_thread->result().setOutcome(QVariantMap {
                                { "client_id", customer["client_id"] },
                                { "customer_name", customer["customer_name"] },
                                { "customer_phone_number", customer["customer_phone_number"] },
                                { "products", products }
                            });
    };
    QSignalSpy successSpy(m_saleCartModel, &QMLSaleCartModel::success);
    QSignalSpy errorSpy(m_saleCartModel, &QMLSaleCartModel::error);
    QSignalSpy busyChangedSpy(m_saleCartModel, &QMLSaleCartModel::busyChanged);

    // STEP: Ensure transaction ID is not set.
    QCOMPARE(m_saleCartModel->transactionId(), 0);

    // STEP: Set the mandatory fields (customer name and phone number).
    m_saleCartModel->setCustomerName(customer["customer_name"].toString());
    m_saleCartModel->setCustomerPhoneNumber(customer["customer_phone_number"].toString());

    // STEP: Sell one product to the client.
    m_saleCartModel->addProduct(product);

    databaseWillReturnEmptyResult();

    m_saleCartModel->submitTransaction({ { "due_date_time", QDateTime::currentDateTime().addDays(2) }});
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<ModelResult>().code(), QMLSaleCartModel::SubmitTransactionSuccess);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_saleCartModel->transactionId(), 0);

    databaseWillReturn(customer, QVariantList{ product });

    // STEP: Retrieve transaction.
    m_saleCartModel->setTransactionId(1);
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);

    // STEP: Ensure that the correct values are returned to the model.
    QCOMPARE(m_saleCartModel->customerId(),
             customer["customer_id"].toInt());
    QCOMPARE(m_saleCartModel->customerName(),
             customer["customer_name"].toString());
    QCOMPARE(m_saleCartModel->customerPhoneNumber(),
             customer["customer_phone_number"].toString());
    QCOMPARE(m_saleCartModel->rowCount(), 1);
    QCOMPARE(m_saleCartModel->index(0).data(QMLSaleCartModel::CategoryIdRole).toInt(),
             product["category_id"].toInt());
    QCOMPARE(m_saleCartModel->index(0).data(QMLSaleCartModel::CategoryRole).toString(),
             product["category"].toString());
    QCOMPARE(m_saleCartModel->index(0).data(QMLSaleCartModel::ProductIdRole).toInt(),
             product["product_id"].toInt());
    QCOMPARE(m_saleCartModel->index(0).data(QMLSaleCartModel::ProductRole).toString(),
             product["product"].toString());
    QCOMPARE(m_saleCartModel->index(0).data(QMLSaleCartModel::QuantityRole).toDouble(),
             product["quantity"].toDouble());
    QCOMPARE(m_saleCartModel->index(0).data(QMLSaleCartModel::AvailableQuantityRole).toDouble(),
             product["available_quantity"].toDouble());
    QCOMPARE(m_saleCartModel->index(0).data(QMLSaleCartModel::UnitIdRole).toInt(),
             product["product_unit_id"].toInt());
    QCOMPARE(m_saleCartModel->index(0).data(QMLSaleCartModel::UnitRole).toString(),
             product["product_unit"].toString());
    QCOMPARE(m_saleCartModel->index(0).data(QMLSaleCartModel::CostPriceRole).toDouble(),
             product["cost_price"].toDouble());
    QCOMPARE(m_saleCartModel->index(0).data(QMLSaleCartModel::RetailPriceRole).toDouble(),
             product["retail_price"].toDouble());
    QCOMPARE(m_saleCartModel->index(0).data(QMLSaleCartModel::UnitPriceRole).toDouble(),
             product["unit_price"].toDouble());
}

void QMLSaleCartModelTest::testRetrieveSuspendedTransaction()
{
    const QVariantMap customer {
        { "client_id", 1 },
        { "customer_name", QStringLiteral("Customer") },
        { "customer_phone_number", QStringLiteral("123456789") }
    };
    const QVariantMap product {
        { "sale_transaction_id", 1 },
        { "category_id", 1 },
        { "category", "Category1" },
        { "product_id", 1 },
        { "product", "Product1" },
        { "quantity", 1.38 },
        { "product_unit_id", 1 },
        { "product_unit", "Unit1" },
        { "cost_price", 2.52 },
        { "retail_price", 3.95 },
        { "unit_price", 13.23 },
        { "available_quantity", 10.34 }
    };
    auto databaseWillReturnEmptyResult = [this]() {
        m_thread->result().setSuccessful(true);
    };
    auto databaseWillReturn = [this](const QVariantMap &customer, const QVariantList &products) {
        m_thread->result().setSuccessful(true);
        m_thread->result().setOutcome(QVariantMap {
                                { "client_id", customer["client_id"] },
                                { "customer_name", customer["customer_name"] },
                                { "customer_phone_number", customer["customer_phone_number"] },
                                { "products", products }
                            });
    };

    QSignalSpy errorSpy(m_saleCartModel, &QMLSaleCartModel::error);
    QSignalSpy successSpy(m_saleCartModel, &QMLSaleCartModel::success);
    QSignalSpy busyChangedSpy(m_saleCartModel, &QMLSaleCartModel::busyChanged);

    // STEP: Add product to the model.
    m_saleCartModel->setCustomerName(customer["customer_name"].toString());
    m_saleCartModel->addProduct(product);

    databaseWillReturnEmptyResult();

    // STEP: Suspend transaction.
    m_saleCartModel->suspendTransaction();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<ModelResult>().code(), QMLSaleCartModel::SuspendTransactionSuccess);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);

    databaseWillReturn(customer, QVariantList{ product });

    // STEP: Retrieve suspended transaction.
    m_saleCartModel->setTransactionId(product["sale_transaction_id"].toInt());
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<ModelResult>().code(), QMLSaleCartModel::RetrieveTransactionSuccess);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);

    // STEP: Ensure that model is re-populated correctly.
    QCOMPARE(m_saleCartModel->rowCount(), 1);
    QCOMPARE(m_saleCartModel->customerName(),
             customer["customer_name"].toString());
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::CategoryIdRole).toInt(),
             product["category_id"].toInt());
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::CategoryRole).toString(),
             product["category"].toString());
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::ProductIdRole).toInt(),
             product["product_id"].toInt());
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::ProductRole).toString(),
             product["product"].toString());
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::QuantityRole).toDouble(),
             product["quantity"].toDouble());
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::UnitIdRole).toInt(),
             product["product_unit_id"].toInt());
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::UnitRole).toString(),
             product["product_unit"].toString());
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::CostPriceRole).toDouble(),
             product["cost_price"].toDouble());
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::RetailPriceRole).toDouble(),
             product["retail_price"].toDouble());
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::UnitPriceRole).toDouble(),
             product["unit_price"].toDouble());
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::AvailableQuantityRole).toDouble(),
             product["available_quantity"].toDouble());
}

void QMLSaleCartModelTest::testRemoveProduct()
{
    const QVariantMap productInfo {
        { "category_id", 1 },
        { "category", "Category1" },
        { "product_id", 1 },
        { "product", "Product1" },
        { "quantity", 1.0 },
        { "unit_id", 1 },
        { "unit", "Unit1" },
        { "cost_price", 2.0 },
        { "retail_price", 3.0 },
        { "unit_price", 13.0 },
        { "available_quantity", 10.0 }
    };

    // STEP: Add product to the model.
    m_saleCartModel->addProduct(productInfo);
    QCOMPARE(m_saleCartModel->rowCount(), 1);

    // STEP: Remove product from the model.
    m_saleCartModel->removeProduct(1);
    QCOMPARE(m_saleCartModel->rowCount(), 0);
}

void QMLSaleCartModelTest::testSetProductQuantity()
{
    const QVariantMap product {
        { "category_id", 1 },
        { "category", "Category1" },
        { "product_id", 1 },
        { "product", "Product1" },
        { "quantity", 1.56 },
        { "unit_id", 1 },
        { "unit", "Unit1" },
        { "cost_price", 2.0 },
        { "retail_price", 3.0 },
        { "unit_price", 13.0 },
        { "available_quantity", 10.0 }
    };

    // STEP: Add product to the model.
    m_saleCartModel->addProduct(product);
    QCOMPARE(m_saleCartModel->rowCount(), 1);

    // STEP: Ensure a quantity of 1 is set.
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::QuantityRole).toDouble(),
             product["quantity"].toDouble());

    // STEP: Set the quantity to an invalid value.
    m_saleCartModel->setProductQuantity(product["product_id"].toInt(), 0.0);
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::QuantityRole).toDouble(),
             product["quantity"].toDouble());
    m_saleCartModel->setProductQuantity(product["product_id"].toInt(), -1.0);
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::QuantityRole).toDouble(),
             product["quantity"].toDouble());

    // STEP: Set the quantity to a value greater than the available quantity. Ensure the value caps at
    // the available quantity.
    m_saleCartModel->setProductQuantity(product["product_id"].toInt(), 11.0);
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::QuantityRole).toDouble(), 10.0);

    // STEP: Set the quantity to an valid value.
    m_saleCartModel->setProductQuantity(product["product_id"].toInt(), 5.5);
    QCOMPARE(m_saleCartModel->data(m_saleCartModel->index(0), QMLSaleCartModel::QuantityRole).toDouble(), 5.5);
}

void QMLSaleCartModelTest::testNoDueDateSet()
{
    const QString customerName = QStringLiteral("Customer");
    const QVariantMap product {
        { "category_id", 1 },
        { "category", "Category1" },
        { "product_id", 1 },
        { "product", "Product1" },
        { "quantity", 1.0 },
        { "unit_id", 1 },
        { "unit", "Unit" },
        { "cost_price", 11.0 },
        { "retail_price", 10.0 },
        { "unit_price", 13.0 },
        { "available_quantity", 10.0 },
        { "cost", 11.0 }
    };
    auto databaseWillReturnEmptyResult = [this]() {
        m_thread->result().setSuccessful(true);
    };
    QSignalSpy successSpy(m_saleCartModel, &QMLSaleCartModel::success);
    QSignalSpy errorSpy(m_saleCartModel, &QMLSaleCartModel::error);
    QSignalSpy busyChangedSpy(m_saleCartModel, &QMLSaleCartModel::busyChanged);

    m_saleCartModel->setCustomerName(customerName);
    m_saleCartModel->addProduct(product);
    m_saleCartModel->setAmountPaid(5.0);

    databaseWillReturnEmptyResult();

    m_saleCartModel->submitTransaction();
    QCOMPARE(busyChangedSpy.count(), 0); // NOTE: Caught in model, without crossing to database thread
    QCOMPARE(errorSpy.count(), 1);
    QCOMPARE(errorSpy.takeFirst().first().value<ModelResult>().code(), QMLSaleCartModel::NoDueDateSetError);
    QCOMPARE(successSpy.count(), 0);
}

void QMLSaleCartModelTest::testSubmitTransaction()
{
    auto databaseWillReturnEmptyResult = [this]() {
        m_thread->result().setSuccessful(true);
    };

    QSignalSpy rowsInsertedSpy(m_saleCartModel, &QMLSaleCartModel::rowsInserted);
    QSignalSpy successSpy(m_saleCartModel, &QMLSaleCartModel::success);
    QSignalSpy errorSpy(m_saleCartModel, &QMLSaleCartModel::error);
    QSignalSpy busyChangedSpy(m_saleCartModel, &QMLSaleCartModel::busyChanged);

    const QVariantMap product {
        { "category_id", 1 },
        { "category", "Category1" },
        { "product_id", 1 },
        { "product", "Product1" },
        { "quantity", 1.0 },
        { "unit_id", 1 },
        { "unit", "Unit" },
        { "cost_price", 11.35 },
        { "retail_price", 10.56 },
        { "unit_price", 13.45 },
        { "available_quantity", 10.0 }
    };

    // STEP: Add product to the model.
    m_saleCartModel->setCustomerName("Customer");
    m_saleCartModel->addProduct(product);

    databaseWillReturnEmptyResult();

    // STEP: Submit transaction.
    m_saleCartModel->submitTransaction();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<ModelResult>().code(), QMLSaleCartModel::SubmitTransactionSuccess);
    QCOMPARE(errorSpy.count(), 0);
}

void QMLSaleCartModelTest::testSuspendTransaction()
{
    auto databaseWillReturnEmptyResult = [this]() {
        m_thread->result().setSuccessful(true);
    };
    QSignalSpy successSpy(m_saleCartModel, &QMLSaleCartModel::success);
    QSignalSpy errorSpy(m_saleCartModel, &QMLSaleCartModel::error);

    const QVariantMap productInfo {
        { "category_id", 1 },
        { "category", "Category1" },
        { "product_id", 1 },
        { "product", "Product1" },
        { "quantity", 1.0 },
        { "unit_id", 1 },
        { "unit", "Unit" },
        { "cost_price", 11.0 },
        { "retail_price", 10.0 },
        { "unit_price", 13.0 },
        { "available_quantity", 10.0 }
    };

    // STEP: Add an product to the model.
    m_saleCartModel->setCustomerName(QStringLiteral("Customer"));
    m_saleCartModel->addProduct(productInfo);

    databaseWillReturnEmptyResult();

    // STEP: Suspend transaction.
    m_saleCartModel->suspendTransaction();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<ModelResult>().code(), QMLSaleCartModel::SuspendTransactionSuccess);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_saleCartModel->customerName(), QString());
    QCOMPARE(m_saleCartModel->rowCount(), 0);
}

void QMLSaleCartModelTest::testSetTransactionId()
{
    const QVariantMap product {
        { "category_id", 1 },
        { "category", "Category1" },
        { "product_id", 1 },
        { "product", "Product1" },
        { "quantity", 1.0 },
        { "unit_id", 1 },
        { "unit", "Unit" },
        { "unit_price", 13.0 },
        { "available_quantity", 10.0 }
    };
    auto databaseWillReturnEmptyResult = [this]() {
        m_thread->result().setSuccessful(true);
    };
    auto databaseWillReturnSubmittedTransaction = [this, &product]() {
        m_thread->result().setSuccessful(true);

        m_thread->result().setOutcome(QVariantMap {
                                { "client_id", 1 },
                                { "customer_name", QStringLiteral("Customer") },
                                { "customer_phone_number", QStringLiteral("123456789") },
                                { "products", QVariantList { { product } } }
                            });
    };
    QSignalSpy successSpy(m_saleCartModel, &QMLSaleCartModel::success);
    QSignalSpy errorSpy(m_saleCartModel, &QMLSaleCartModel::error);
    QSignalSpy busyChangedSpy(m_saleCartModel, &QMLSaleCartModel::busyChanged);
    QSignalSpy transactionIdChangedSpy(m_saleCartModel, &QMLSaleCartModel::transactionIdChanged);

    // STEP: Ensure transaction ID is not set.
    QCOMPARE(m_saleCartModel->transactionId(), 0);

    // STEP: Add product to the model.
    m_saleCartModel->setCustomerName(QStringLiteral("Customer"));
    m_saleCartModel->setCustomerPhoneNumber(QStringLiteral("123456789"));
    m_saleCartModel->addProduct(product);

    databaseWillReturnEmptyResult();

    // STEP: Submit transaction.
    m_saleCartModel->submitTransaction();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<ModelResult>().code(), QMLSaleCartModel::SubmitTransactionSuccess);
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
    QCOMPARE(successSpy.takeFirst().first().value<ModelResult>().code(), QMLSaleCartModel::RetrieveTransactionSuccess);
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
        m_thread->result().setSuccessful(true);
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
    QCOMPARE(errorSpy.takeFirst().first().value<ModelResult>().code(), QMLSaleCartModel::EmptyCartError);
}

void QMLSaleCartModelTest::testSuspendEmptyTransaction()
{
    auto databaseWillReturnEmptyResult = [this]() {
        m_thread->result().setSuccessful(true);
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
    QCOMPARE(errorSpy.takeFirst().first().value<ModelResult>().code(), QMLSaleCartModel::EmptyCartError);
    QCOMPARE(successSpy.count(), 0);
}

QTEST_MAIN(QMLSaleCartModelTest)

#include "tst_qmlsalecartmodeltest.moc"
