#include "qmlapi/qmlstockproductdetailrecord.h"
#include "mockdatabasethread.h"
#include <QString>
#include <QtTest>
#include <QCoreApplication>

class QMLStockProductDetailRecordTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();

    void testViewStockProductDetails();
private:
    QMLStockProductDetailRecord *m_stockProductDetailRecord;
    MockDatabaseThread *m_thread;
};

void QMLStockProductDetailRecordTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_stockProductDetailRecord = new QMLStockProductDetailRecord(*m_thread, this);
}

void QMLStockProductDetailRecordTest::cleanup()
{
    m_stockProductDetailRecord->deleteLater();
    m_thread->deleteLater();
}

void QMLStockProductDetailRecordTest::testViewStockProductDetails()
{
    const QDateTime &currentDateTime = QDateTime::currentDateTime();
    const QVariantMap product {
        { "product_category_id", 1 },
        { "product_category", "Category" },
        { "product_id", 1 },
        { "product", "Product" },
        { "description", "Description" },
        { "quantity", 1.0 },
        { "product_unit_id", 1 },
        { "product_unit", "Unit" },
        { "cost_price", 2.0 },
        { "retail_price", 3.0 },
        { "unit_price", 13.0 },
        { "available_quantity", 10.0 },
        { "divisible", true },
        { "currency", Settings::instance().locale().currencySymbol(QLocale::CurrencyIsoCode) },
        { "created", currentDateTime },
        { "last_edited", currentDateTime },
        { "user_id", 1 },
        { "user", QStringLiteral("user") }
    };

    auto databaseWillReturn = [this](const QVariantMap &product) {
        m_thread->result().setSuccessful(true);
        m_thread->result().setOutcome(QVariant());
        m_thread->result().setOutcome(QVariantMap { { "product", product } });
    };
    QSignalSpy successSpy(m_stockProductDetailRecord, &QMLStockProductDetailRecord::success);
    QSignalSpy errorSpy(m_stockProductDetailRecord, &QMLStockProductDetailRecord::error);
    QSignalSpy busyChangedSpy(m_stockProductDetailRecord, &QMLStockProductDetailRecord::busyChanged);
    QSignalSpy productIdChangedSpy(m_stockProductDetailRecord, &QMLStockProductDetailRecord::productIdChanged);
    QSignalSpy productCategoryIdChangedSpy(m_stockProductDetailRecord, &QMLStockProductDetailRecord::categoryIdChanged);
    QSignalSpy productCategoryChangedSpy(m_stockProductDetailRecord, &QMLStockProductDetailRecord::categoryChanged);
    QSignalSpy productChangedSpy(m_stockProductDetailRecord, &QMLStockProductDetailRecord::productChanged);
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
    QCOMPARE(m_stockProductDetailRecord->productId(), 0);
    QCOMPARE(productIdChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->categoryId(), 0);
    QCOMPARE(productCategoryIdChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->category(), QString());
    QCOMPARE(productCategoryChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->product(), QString());
    QCOMPARE(productChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->description(), QString());
    QCOMPARE(descriptionChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->isDivisible(), true);
    QCOMPARE(divisibleChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->quantity(), 0.0);
    QCOMPARE(quantityChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->unitId(), 0);
    QCOMPARE(unitIdChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->unit(), QString());
    QCOMPARE(unitChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->costPrice(), 0.0);
    QCOMPARE(costPriceChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->retailPrice(), 0.0);
    QCOMPARE(retailPriceChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->currency(),
             Settings::instance().locale().currencySymbol(QLocale::CurrencyIsoCode));
    QCOMPARE(currencyChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->created(), QDateTime());
    QCOMPARE(createdChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->lastEdited(), QDateTime());
    QCOMPARE(lastEditedChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->userId(), 0);
    QCOMPARE(userIdChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->user(), QString());
    QCOMPARE(userChangedSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 0);

    databaseWillReturn(product);

    // STEP: Set item ID so that record can be fetched.
    m_stockProductDetailRecord->setProductId(1);
    QCOMPARE(m_stockProductDetailRecord->productId(), 1);
    QCOMPARE(productIdChangedSpy.count(), 1);
    productIdChangedSpy.clear();
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);

    // STEP: Ensure values are properly set.
    QCOMPARE(m_stockProductDetailRecord->productId(),
             product["product_id"].toInt());
    QCOMPARE(m_stockProductDetailRecord->categoryId(),
             product["product_category_id"].toInt());
    QCOMPARE(productCategoryIdChangedSpy.count(), 1);
    QCOMPARE(m_stockProductDetailRecord->category(),
             product["product_category"].toString());
    QCOMPARE(productCategoryChangedSpy.count(), 1);
    QCOMPARE(m_stockProductDetailRecord->product(),
             product["product"].toString());
    QCOMPARE(productChangedSpy.count(), 1);
    QCOMPARE(m_stockProductDetailRecord->description(),
             product["description"].toString());
    QCOMPARE(descriptionChangedSpy.count(), 1);
    QCOMPARE(m_stockProductDetailRecord->isDivisible(),
             product["divisible"].toBool());
    QCOMPARE(divisibleChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->quantity(),
             product["quantity"].toDouble());
    QCOMPARE(quantityChangedSpy.count(), 1);
    QCOMPARE(m_stockProductDetailRecord->unitId(),
             product["product_unit_id"].toInt());
    QCOMPARE(unitIdChangedSpy.count(), 1);
    QCOMPARE(m_stockProductDetailRecord->unit(),
             product["product_unit"].toString());
    QCOMPARE(unitChangedSpy.count(), 1);
    QCOMPARE(m_stockProductDetailRecord->costPrice(),
             product["cost_price"].toDouble());
    QCOMPARE(costPriceChangedSpy.count(), 1);
    QCOMPARE(m_stockProductDetailRecord->retailPrice(),
             product["retail_price"].toDouble());
    QCOMPARE(retailPriceChangedSpy.count(), 1);
    QCOMPARE(m_stockProductDetailRecord->currency(),
             product["currency"].toString());
    QCOMPARE(currencyChangedSpy.count(), 0);
    QCOMPARE(m_stockProductDetailRecord->created(),
             product["created"].toDateTime());
    QCOMPARE(createdChangedSpy.count(), 1);
    QCOMPARE(m_stockProductDetailRecord->lastEdited(),
             product["last_edited"].toDateTime());
    QCOMPARE(lastEditedChangedSpy.count(), 1);
    QCOMPARE(m_stockProductDetailRecord->userId(),
             product["user_id"].toInt());
    QCOMPARE(userIdChangedSpy.count(), 1);
    QCOMPARE(m_stockProductDetailRecord->user(), QStringLiteral("user"));
    QCOMPARE(userChangedSpy.count(), 1);
    QCOMPARE(busyChangedSpy.count(), 0);
}

QTEST_MAIN(QMLStockProductDetailRecordTest)

#include "tst_qmlstockproductdetailrecordtest.moc"
