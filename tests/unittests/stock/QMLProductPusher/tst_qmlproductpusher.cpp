#include <QCoreApplication>
#include <QString>
#include <QtTest>
#include "database/databaseexception.h"
#include "mockdatabasethread.h"
#include "qmlapi/stock/qmlproductpusher.h"

class QMLProductPusherTest : public QObject
{
    Q_OBJECT
private slots:
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
    QMLProductPusher* m_stockProductPusher;
    MockDatabaseThread* m_thread;
};

void QMLProductPusherTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_stockProductPusher = new QMLProductPusher(*m_thread, this);
}

void QMLProductPusherTest::cleanup()
{
    m_stockProductPusher->deleteLater();
    m_thread->deleteLater();
}

void QMLProductPusherTest::testSetCategory()
{
    QSignalSpy categoryChangedSpy(m_stockProductPusher,
                                  &QMLProductPusher::categoryChanged);

    QCOMPARE(m_stockProductPusher->category(), QString());

    m_stockProductPusher->setCategory("Category");

    QCOMPARE(m_stockProductPusher->category(), "Category");
    QCOMPARE(categoryChangedSpy.count(), 1);
    categoryChangedSpy.clear();

    m_stockProductPusher->setCategory("Category");
    QCOMPARE(categoryChangedSpy.count(), 0);
}

void QMLProductPusherTest::testSetProduct()
{
    QSignalSpy productChangedSpy(m_stockProductPusher,
                                 &QMLProductPusher::productChanged);

    QCOMPARE(m_stockProductPusher->product(), QString());

    m_stockProductPusher->setProduct("Product");

    QCOMPARE(m_stockProductPusher->product(), QStringLiteral("Product"));
    QCOMPARE(productChangedSpy.count(), 1);
    productChangedSpy.clear();

    m_stockProductPusher->setProduct("Product");
    QCOMPARE(productChangedSpy.count(), 0);
}

void QMLProductPusherTest::testSetDescription()
{
    QSignalSpy descriptionChangedSpy(m_stockProductPusher,
                                     &QMLProductPusher::descriptionChanged);

    QCOMPARE(m_stockProductPusher->description(), QString());

    m_stockProductPusher->setDescription("Description");

    QCOMPARE(m_stockProductPusher->description(), "Description");
    QCOMPARE(descriptionChangedSpy.count(), 1);
    descriptionChangedSpy.clear();

    m_stockProductPusher->setDescription("Description");
    QCOMPARE(descriptionChangedSpy.count(), 0);
}

void QMLProductPusherTest::testSetQuantity()
{
    QSignalSpy quantityChangedSpy(m_stockProductPusher,
                                  &QMLProductPusher::quantityChanged);

    QCOMPARE(m_stockProductPusher->quantity(), 0.0);

    m_stockProductPusher->setQuantity(1.5);

    QCOMPARE(m_stockProductPusher->quantity(), 1.5);
    QCOMPARE(quantityChangedSpy.count(), 1);
    quantityChangedSpy.clear();

    m_stockProductPusher->setQuantity(1.5);
    QCOMPARE(quantityChangedSpy.count(), 0);
}

void QMLProductPusherTest::testSetUnit()
{
    QSignalSpy unitChangedSpy(m_stockProductPusher,
                              &QMLProductPusher::unitChanged);

    QCOMPARE(m_stockProductPusher->unit(), QString());

    m_stockProductPusher->setUnit("Unit");

    QCOMPARE(m_stockProductPusher->unit(), "Unit");
    QCOMPARE(unitChangedSpy.count(), 1);
    unitChangedSpy.clear();

    m_stockProductPusher->setUnit("Unit");
    QCOMPARE(unitChangedSpy.count(), 0);
}

void QMLProductPusherTest::testSetCategoryNote()
{
    QSignalSpy categoryNoteChangedSpy(m_stockProductPusher,
                                      &QMLProductPusher::categoryNoteChanged);

    QCOMPARE(m_stockProductPusher->categoryNote(), QString());

    m_stockProductPusher->setCategoryNote("Category note");

    QCOMPARE(m_stockProductPusher->categoryNote(), "Category note");
    QCOMPARE(categoryNoteChangedSpy.count(), 1);
    categoryNoteChangedSpy.clear();

    m_stockProductPusher->setCategoryNote("Category note");
    QCOMPARE(categoryNoteChangedSpy.count(), 0);
}

void QMLProductPusherTest::testSetProductNote()
{
    QSignalSpy productNoteChangedSpy(m_stockProductPusher,
                                     &QMLProductPusher::productNoteChanged);

    QCOMPARE(m_stockProductPusher->productNote(), QString());

    m_stockProductPusher->setProductNote("Product note");

    QCOMPARE(m_stockProductPusher->productNote(), "Product note");
    QCOMPARE(productNoteChangedSpy.count(), 1);
    productNoteChangedSpy.clear();

    m_stockProductPusher->setProductNote("Product note");
    QCOMPARE(productNoteChangedSpy.count(), 0);
}

void QMLProductPusherTest::testSetDivisible()
{
    QSignalSpy divisibleChangedSpy(m_stockProductPusher,
                                   &QMLProductPusher::divisibleChanged);

    QCOMPARE(m_stockProductPusher->isDivisible(), true);

    m_stockProductPusher->setDivisible(false);

    QCOMPARE(m_stockProductPusher->isDivisible(), false);
    QCOMPARE(divisibleChangedSpy.count(), 1);
    divisibleChangedSpy.clear();

    m_stockProductPusher->setDivisible(false);
    QCOMPARE(divisibleChangedSpy.count(), 0);
}

void QMLProductPusherTest::testSetCostPrice()
{
    QSignalSpy costPriceChangedSpy(m_stockProductPusher,
                                   &QMLProductPusher::costPriceChanged);

    QCOMPARE(m_stockProductPusher->costPrice(), 0.0);

    m_stockProductPusher->setCostPrice(1234.56);

    QCOMPARE(m_stockProductPusher->costPrice(), 1234.56);
    QCOMPARE(costPriceChangedSpy.count(), 1);
    costPriceChangedSpy.clear();

    m_stockProductPusher->setCostPrice(1234.56);
    QCOMPARE(costPriceChangedSpy.count(), 0);
}

void QMLProductPusherTest::testSetRetailPrice()
{
    QSignalSpy retailPriceChangedSpy(m_stockProductPusher,
                                     &QMLProductPusher::retailPriceChanged);

    QCOMPARE(m_stockProductPusher->retailPrice(), 0.0);

    m_stockProductPusher->setRetailPrice(1234.56);

    QCOMPARE(m_stockProductPusher->retailPrice(), 1234.56);
    QCOMPARE(retailPriceChangedSpy.count(), 1);
    retailPriceChangedSpy.clear();

    m_stockProductPusher->setRetailPrice(1234.56);
    QCOMPARE(retailPriceChangedSpy.count(), 0);
}

void QMLProductPusherTest::testSetTracked()
{
    QSignalSpy trackedChangedSpy(m_stockProductPusher,
                                 &QMLProductPusher::trackedChanged);

    QCOMPARE(m_stockProductPusher->isTracked(), true);

    m_stockProductPusher->setTracked(false);

    QCOMPARE(m_stockProductPusher->isTracked(), false);
    QCOMPARE(trackedChangedSpy.count(), 1);
    trackedChangedSpy.clear();

    m_stockProductPusher->setTracked(false);
    QCOMPARE(trackedChangedSpy.count(), 0);
}

void QMLProductPusherTest::testSetImageSource()
{
    QSignalSpy imageUrlChangedSpy(m_stockProductPusher,
                                  &QMLProductPusher::imageUrlChanged);

    QCOMPARE(m_stockProductPusher->imageUrl(), QUrl());

    m_stockProductPusher->setImageUrl(QUrl("Image source"));

    QCOMPARE(m_stockProductPusher->imageUrl(), QUrl("Image source"));
    QCOMPARE(imageUrlChangedSpy.count(), 1);
    imageUrlChangedSpy.clear();

    m_stockProductPusher->setImageUrl(QUrl("Image source"));
    QCOMPARE(imageUrlChangedSpy.count(), 0);
}

void QMLProductPusherTest::testPushNewProduct()
{
    auto databaseWillReturnEmptyResult = [this]() {
        m_thread->result().setSuccessful(true);
    };
    QSignalSpy busyChangedSpy(m_stockProductPusher,
                              &QMLProductPusher::busyChanged);
    QSignalSpy successSpy(m_stockProductPusher, &QMLProductPusher::success);

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

    QCOMPARE(busyChangedSpy.count(), 2);  // busy must be true, then false
    QCOMPARE(successSpy.count(), 1);
}

void QMLProductPusherTest::testPushSameProduct()
{
    auto databaseWillReturnEmptyResult = [this]() {
        m_thread->result().setSuccessful(true);
    };
    auto databaseWillReturnDuplicateEntryError = [this]() {
        m_thread->result().setSuccessful(false);
        m_thread->result().setErrorCode(static_cast<int>(
            DatabaseError::MySqlErrorCode::UserDefinedException));
    };
    QSignalSpy busyChangedSpy(m_stockProductPusher,
                              &QMLProductPusher::busyChanged);
    QSignalSpy errorSpy(m_stockProductPusher, &QMLProductPusher::error);
    QSignalSpy successSpy(m_stockProductPusher, &QMLProductPusher::success);

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
    QCOMPARE(errorSpy.takeFirst().first().value<ModelResult>().code(),
             QMLProductPusher::DuplicateEntryError);
    errorSpy.clear();
}

void QMLProductPusherTest::testPushUpdatedProduct()
{
    auto databaseWillReturnEmptyResult = [this]() {
        m_thread->result().setSuccessful(true);
    };
    QSignalSpy busyChangedSpy(m_stockProductPusher,
                              &QMLProductPusher::busyChanged);
    QSignalSpy errorSpy(m_stockProductPusher, &QMLProductPusher::error);
    QSignalSpy successSpy(m_stockProductPusher, &QMLProductPusher::success);

    // STEP: Ensure product ID is not set.
    QCOMPARE(m_stockProductPusher->productId(), 0);

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
    QCOMPARE(successSpy.takeFirst().first().value<ModelResult>().code(),
             QMLProductPusher::AddProductSuccess);
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
    QCOMPARE(busyChangedSpy.count(), 2);  // "busy" must be true, then false
    busyChangedSpy.clear();
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<ModelResult>().code(),
             QMLProductPusher::UpdateProductSuccess);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    errorSpy.clear();
}

QTEST_GUILESS_MAIN(QMLProductPusherTest)

#include "tst_qmlproductpusher.moc"
