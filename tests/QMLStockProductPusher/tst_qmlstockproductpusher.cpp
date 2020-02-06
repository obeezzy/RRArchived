#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmlstockproductpusher.h"
#include "database/databaseexception.h"
#include "mockdatabasethread.h"

class QMLStockProductPusherTest : public QObject
{
    Q_OBJECT

public:
    QMLStockProductPusherTest();

private Q_SLOTS:
    void init();
    void cleanup();

    void testSetCategory();
    void testSetProduct();
    void testSetDescription();
    void testSetQuantity();
    void testSetUnit();
    void testSetCategoryNote();
    void testSetProductNote();
    void testSetDivisible();
    void testSetCostPrice();
    void testSetRetailPrice();
    void testSetTracked();
    void testSetImageSource();

    void testPushNewProduct();
    void testPushSameProduct();
    void testPushUpdatedProduct();

    // void testQuantityCannotBeUpdated();
    // void testPushSameCategoryProducts();
    // void testPushDifferentCategoryProducts();
private:
    QMLStockProductPusher *m_stockProductPusher;
    MockDatabaseThread m_thread;
    QueryResult m_result;
};

QMLStockProductPusherTest::QMLStockProductPusherTest() :
    m_thread(&m_result)
{
    QLoggingCategory::setFilterRules(QStringLiteral("*.info=false"));
}

void QMLStockProductPusherTest::init()
{
    m_stockProductPusher = new QMLStockProductPusher(m_thread, this);
}

void QMLStockProductPusherTest::cleanup()
{
}

void QMLStockProductPusherTest::testSetCategory()
{
    QSignalSpy categoryChangedSpy(m_stockProductPusher, &QMLStockProductPusher::categoryChanged);

    QCOMPARE(m_stockProductPusher->category(), QString());

    m_stockProductPusher->setCategory("Category");

    QCOMPARE(m_stockProductPusher->category(), "Category");
    QCOMPARE(categoryChangedSpy.count(), 1);
    categoryChangedSpy.clear();

    m_stockProductPusher->setCategory("Category");
    QCOMPARE(categoryChangedSpy.count(), 0);
}

void QMLStockProductPusherTest::testSetProduct()
{
    QSignalSpy productChangedSpy(m_stockProductPusher, &QMLStockProductPusher::productChanged);

    QCOMPARE(m_stockProductPusher->product(), QString());

    m_stockProductPusher->setProduct("Product");

    QCOMPARE(m_stockProductPusher->product(), QStringLiteral("Product"));
    QCOMPARE(productChangedSpy.count(), 1);
    productChangedSpy.clear();

    m_stockProductPusher->setProduct("Product");
    QCOMPARE(productChangedSpy.count(), 0);
}

void QMLStockProductPusherTest::testSetDescription()
{
    QSignalSpy descriptionChangedSpy(m_stockProductPusher, &QMLStockProductPusher::descriptionChanged);

    QCOMPARE(m_stockProductPusher->description(), QString());

    m_stockProductPusher->setDescription("Description");

    QCOMPARE(m_stockProductPusher->description(), "Description");
    QCOMPARE(descriptionChangedSpy.count(), 1);
    descriptionChangedSpy.clear();

    m_stockProductPusher->setDescription("Description");
    QCOMPARE(descriptionChangedSpy.count(), 0);
}

void QMLStockProductPusherTest::testSetQuantity()
{
    QSignalSpy quantityChangedSpy(m_stockProductPusher, &QMLStockProductPusher::quantityChanged);

    QCOMPARE(m_stockProductPusher->quantity(), 0.0);

    m_stockProductPusher->setQuantity(1.5);

    QCOMPARE(m_stockProductPusher->quantity(), 1.5);
    QCOMPARE(quantityChangedSpy.count(), 1);
    quantityChangedSpy.clear();

    m_stockProductPusher->setQuantity(1.5);
    QCOMPARE(quantityChangedSpy.count(), 0);
}

void QMLStockProductPusherTest::testSetUnit()
{
    QSignalSpy unitChangedSpy(m_stockProductPusher, &QMLStockProductPusher::unitChanged);

    QCOMPARE(m_stockProductPusher->unit(), QString());

    m_stockProductPusher->setUnit("Unit");

    QCOMPARE(m_stockProductPusher->unit(), "Unit");
    QCOMPARE(unitChangedSpy.count(), 1);
    unitChangedSpy.clear();

    m_stockProductPusher->setUnit("Unit");
    QCOMPARE(unitChangedSpy.count(), 0);
}

void QMLStockProductPusherTest::testSetCategoryNote()
{
    QSignalSpy categoryNoteChangedSpy(m_stockProductPusher, &QMLStockProductPusher::categoryNoteChanged);

    QCOMPARE(m_stockProductPusher->categoryNote(), QString());

    m_stockProductPusher->setCategoryNote("Category note");

    QCOMPARE(m_stockProductPusher->categoryNote(), "Category note");
    QCOMPARE(categoryNoteChangedSpy.count(), 1);
    categoryNoteChangedSpy.clear();

    m_stockProductPusher->setCategoryNote("Category note");
    QCOMPARE(categoryNoteChangedSpy.count(), 0);
}

void QMLStockProductPusherTest::testSetProductNote()
{
    QSignalSpy productNoteChangedSpy(m_stockProductPusher, &QMLStockProductPusher::productNoteChanged);

    QCOMPARE(m_stockProductPusher->productNote(), QString());

    m_stockProductPusher->setProductNote("Product note");

    QCOMPARE(m_stockProductPusher->productNote(), "Product note");
    QCOMPARE(productNoteChangedSpy.count(), 1);
    productNoteChangedSpy.clear();

    m_stockProductPusher->setProductNote("Product note");
    QCOMPARE(productNoteChangedSpy.count(), 0);
}

void QMLStockProductPusherTest::testSetDivisible()
{
    QSignalSpy divisibleChangedSpy(m_stockProductPusher, &QMLStockProductPusher::divisibleChanged);

    QCOMPARE(m_stockProductPusher->isDivisible(), false);

    m_stockProductPusher->setDivisible(true);

    QCOMPARE(m_stockProductPusher->isDivisible(), true);
    QCOMPARE(divisibleChangedSpy.count(), 1);
    divisibleChangedSpy.clear();

    m_stockProductPusher->setDivisible(true);
    QCOMPARE(divisibleChangedSpy.count(), 0);
}

void QMLStockProductPusherTest::testSetCostPrice()
{
    QSignalSpy costPriceChangedSpy(m_stockProductPusher, &QMLStockProductPusher::costPriceChanged);

    QCOMPARE(m_stockProductPusher->costPrice(), 0.0);

    m_stockProductPusher->setCostPrice(1234.56);

    QCOMPARE(m_stockProductPusher->costPrice(), 1234.56);
    QCOMPARE(costPriceChangedSpy.count(), 1);
    costPriceChangedSpy.clear();

    m_stockProductPusher->setCostPrice(1234.56);
    QCOMPARE(costPriceChangedSpy.count(), 0);
}

void QMLStockProductPusherTest::testSetRetailPrice()
{
    QSignalSpy retailPriceChangedSpy(m_stockProductPusher, &QMLStockProductPusher::retailPriceChanged);

    QCOMPARE(m_stockProductPusher->retailPrice(), 0.0);

    m_stockProductPusher->setRetailPrice(1234.56);

    QCOMPARE(m_stockProductPusher->retailPrice(), 1234.56);
    QCOMPARE(retailPriceChangedSpy.count(), 1);
    retailPriceChangedSpy.clear();

    m_stockProductPusher->setRetailPrice(1234.56);
    QCOMPARE(retailPriceChangedSpy.count(), 0);
}

void QMLStockProductPusherTest::testSetTracked()
{
    QSignalSpy trackedChangedSpy(m_stockProductPusher, &QMLStockProductPusher::trackedChanged);

    QCOMPARE(m_stockProductPusher->isTracked(), false);

    m_stockProductPusher->setTracked(true);

    QCOMPARE(m_stockProductPusher->isTracked(), true);
    QCOMPARE(trackedChangedSpy.count(), 1);
    trackedChangedSpy.clear();

    m_stockProductPusher->setTracked(true);
    QCOMPARE(trackedChangedSpy.count(), 0);
}

void QMLStockProductPusherTest::testSetImageSource()
{
    QSignalSpy imageUrlChangedSpy(m_stockProductPusher, &QMLStockProductPusher::imageUrlChanged);

    QCOMPARE(m_stockProductPusher->imageUrl(), QUrl());

    m_stockProductPusher->setImageUrl(QUrl("Image source"));

    QCOMPARE(m_stockProductPusher->imageUrl(), QUrl("Image source"));
    QCOMPARE(imageUrlChangedSpy.count(), 1);
    imageUrlChangedSpy.clear();

    m_stockProductPusher->setImageUrl(QUrl("Image source"));
    QCOMPARE(imageUrlChangedSpy.count(), 0);
}

void QMLStockProductPusherTest::testPushNewProduct()
{
    auto databaseWillReturnEmptyResult = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());
    };
    QSignalSpy busyChangedSpy(m_stockProductPusher, &QMLStockProductPusher::busyChanged);
    QSignalSpy successSpy(m_stockProductPusher, &QMLStockProductPusher::success);

    m_stockProductPusher->setCategory(QStringLiteral("Category"));
    m_stockProductPusher->setProduct(QStringLiteral("Product"));
    m_stockProductPusher->setDescription(QStringLiteral("Description"));
    m_stockProductPusher->setQuantity(10.5);
    m_stockProductPusher->setUnit(QStringLiteral("Unit"));
    m_stockProductPusher->setCategoryNote(QStringLiteral("Category note"));
    m_stockProductPusher->setProductNote(QStringLiteral("Product note"));
    m_stockProductPusher->setDivisible(true);
    m_stockProductPusher->setCostPrice(10.85);
    m_stockProductPusher->setRetailPrice(20.12);
    m_stockProductPusher->setTracked(true);
    m_stockProductPusher->setImageUrl(QUrl("Image source"));

    databaseWillReturnEmptyResult();

    m_stockProductPusher->push();

    QCOMPARE(busyChangedSpy.count(), 2); // busy must be true, then false
    QCOMPARE(successSpy.count(), 1);
}

void QMLStockProductPusherTest::testPushSameProduct()
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
    QSignalSpy busyChangedSpy(m_stockProductPusher, &QMLStockProductPusher::busyChanged);
    QSignalSpy errorSpy(m_stockProductPusher, &QMLStockProductPusher::error);
    QSignalSpy successSpy(m_stockProductPusher, &QMLStockProductPusher::success);

    databaseWillReturnEmptyResult();

    m_stockProductPusher->push();

    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();

    databaseWillReturnDuplicateEntryError();

    m_stockProductPusher->push();
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(successSpy.count(), 0);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 1);
    QCOMPARE(errorSpy.takeFirst().first().value<QMLStockProductPusher::ErrorCode>(), QMLStockProductPusher::DuplicateEntryError);
    errorSpy.clear();
}

void QMLStockProductPusherTest::testPushUpdatedProduct()
{
    auto databaseWillReturnEmptyResult = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());
    };
    QSignalSpy busyChangedSpy(m_stockProductPusher, &QMLStockProductPusher::busyChanged);
    QSignalSpy errorSpy(m_stockProductPusher, &QMLStockProductPusher::error);
    QSignalSpy successSpy(m_stockProductPusher, &QMLStockProductPusher::success);

    // STEP: Ensure product ID is not set.
    QCOMPARE(m_stockProductPusher->productId(), -1);

    // STEP: Add a single product to the database.
    m_stockProductPusher->setCategory("Category");
    m_stockProductPusher->setProduct(QStringLiteral("Product"));
    m_stockProductPusher->setDescription("Description");
    m_stockProductPusher->setQuantity(10.5);
    m_stockProductPusher->setUnit("Unit");
    m_stockProductPusher->setCategoryNote("Category note");
    m_stockProductPusher->setProductNote("Product note");
    m_stockProductPusher->setDivisible(true);
    m_stockProductPusher->setCostPrice(10.85);
    m_stockProductPusher->setRetailPrice(20.12);
    m_stockProductPusher->setTracked(true);
    m_stockProductPusher->setImageUrl(QUrl("Image source"));

    databaseWillReturnEmptyResult();

    m_stockProductPusher->push();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLStockProductPusher::SuccessCode>(), QMLStockProductPusher::AddProductSuccess);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    errorSpy.clear();

    databaseWillReturnEmptyResult();

    // STEP: Update the previously added product's properties.
    m_stockProductPusher->setProductId(1);
    m_stockProductPusher->setCategory("Category2");
    m_stockProductPusher->setProduct("Product2");
    m_stockProductPusher->setDescription("Description2");
    m_stockProductPusher->setUnit("Unit2");
    m_stockProductPusher->setCategoryNote("Category note2");
    m_stockProductPusher->setProductNote("Product note2");
    m_stockProductPusher->setDivisible(false);
    m_stockProductPusher->setCostPrice(12.34);
    m_stockProductPusher->setRetailPrice(56.78);
    m_stockProductPusher->setTracked(false);
    m_stockProductPusher->setImageUrl(QUrl("Image source2"));
    m_stockProductPusher->push();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2); // "busy" must be true, then false
    busyChangedSpy.clear();
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLStockProductPusher::SuccessCode>(), QMLStockProductPusher::UpdateProductSuccess);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    errorSpy.clear();
}

QTEST_MAIN(QMLStockProductPusherTest)

#include "tst_qmlstockproductpusher.moc"
