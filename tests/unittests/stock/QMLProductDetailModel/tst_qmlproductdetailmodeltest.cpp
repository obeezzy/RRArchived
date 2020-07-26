#include <QCoreApplication>
#include <QString>
#include <QtTest>
#include "mockdatabasethread.h"
#include "qmlapi/stock/qmlproductdetailmodel.h"

class QMLProductDetailModelTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();

    void testViewProductDetails();

private:
    QMLProductDetailModel* m_productDetailModel;
    MockDatabaseThread* m_thread;
};

void QMLProductDetailModelTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_productDetailModel = new QMLProductDetailModel(*m_thread, this);
}

void QMLProductDetailModelTest::cleanup()
{
    m_productDetailModel->deleteLater();
    m_thread->deleteLater();
}

void QMLProductDetailModelTest::testViewProductDetails()
{
    const QDateTime& currentDateTime = QDateTime::currentDateTime();
    const QVariantMap product{
        {"product_category_id", 1},
        {"product_category", "Category"},
        {"product_id", 1},
        {"product", "Product"},
        {"description", "Description"},
        {"quantity", 1.0},
        {"product_unit_id", 1},
        {"product_unit", "Unit"},
        {"cost_price", 2.0},
        {"retail_price", 3.0},
        {"unit_price", 13.0},
        {"available_quantity", 10.0},
        {"divisible", true},
        {"currency", Settings::instance().locale().currencySymbol(
                         QLocale::CurrencyIsoCode)},
        {"created", currentDateTime},
        {"last_edited", currentDateTime},
        {"user_id", 1},
        {"username", QStringLiteral("user")}};

    auto databaseWillReturn = [this](const QVariantMap& product) {
        m_thread->result().setSuccessful(true);
        m_thread->result().setOutcome(QVariant());
        m_thread->result().setOutcome(QVariantMap{{"product", product}});
    };
    QSignalSpy successSpy(m_productDetailModel,
                          &QMLProductDetailModel::success);
    QSignalSpy errorSpy(m_productDetailModel, &QMLProductDetailModel::error);
    QSignalSpy busyChangedSpy(m_productDetailModel,
                              &QMLProductDetailModel::busyChanged);
    QSignalSpy productIdChangedSpy(m_productDetailModel,
                                   &QMLProductDetailModel::productIdChanged);
    QSignalSpy productCategoryIdChangedSpy(
        m_productDetailModel, &QMLProductDetailModel::categoryIdChanged);
    QSignalSpy productCategoryChangedSpy(
        m_productDetailModel, &QMLProductDetailModel::categoryChanged);
    QSignalSpy productChangedSpy(m_productDetailModel,
                                 &QMLProductDetailModel::productChanged);
    QSignalSpy descriptionChangedSpy(
        m_productDetailModel, &QMLProductDetailModel::descriptionChanged);
    QSignalSpy divisibleChangedSpy(m_productDetailModel,
                                   &QMLProductDetailModel::divisibleChanged);
    QSignalSpy quantityChangedSpy(m_productDetailModel,
                                  &QMLProductDetailModel::quantityChanged);
    QSignalSpy unitIdChangedSpy(m_productDetailModel,
                                &QMLProductDetailModel::unitIdChanged);
    QSignalSpy unitChangedSpy(m_productDetailModel,
                              &QMLProductDetailModel::unitChanged);
    QSignalSpy costPriceChangedSpy(m_productDetailModel,
                                   &QMLProductDetailModel::costPriceChanged);
    QSignalSpy retailPriceChangedSpy(
        m_productDetailModel, &QMLProductDetailModel::retailPriceChanged);
    QSignalSpy currencyChangedSpy(m_productDetailModel,
                                  &QMLProductDetailModel::currencyChanged);
    QSignalSpy createdChangedSpy(m_productDetailModel,
                                 &QMLProductDetailModel::createdChanged);
    QSignalSpy lastEditedChangedSpy(m_productDetailModel,
                                    &QMLProductDetailModel::lastEditedChanged);
    QSignalSpy userIdChangedSpy(m_productDetailModel,
                                &QMLProductDetailModel::userIdChanged);
    QSignalSpy userChangedSpy(m_productDetailModel,
                              &QMLProductDetailModel::userChanged);

    // STEP: Instantiate model in QML.
    m_productDetailModel->componentComplete();

    // STEP: Ensure default values are set.
    QCOMPARE(m_productDetailModel->productId(), 0);
    QCOMPARE(productIdChangedSpy.count(), 0);
    QCOMPARE(m_productDetailModel->categoryId(), 0);
    QCOMPARE(productCategoryIdChangedSpy.count(), 0);
    QCOMPARE(m_productDetailModel->category(), QString());
    QCOMPARE(productCategoryChangedSpy.count(), 0);
    QCOMPARE(m_productDetailModel->product(), QString());
    QCOMPARE(productChangedSpy.count(), 0);
    QCOMPARE(m_productDetailModel->description(), QString());
    QCOMPARE(descriptionChangedSpy.count(), 0);
    QCOMPARE(m_productDetailModel->isDivisible(), true);
    QCOMPARE(divisibleChangedSpy.count(), 0);
    QCOMPARE(m_productDetailModel->quantity(), 0.0);
    QCOMPARE(quantityChangedSpy.count(), 0);
    QCOMPARE(m_productDetailModel->unitId(), 0);
    QCOMPARE(unitIdChangedSpy.count(), 0);
    QCOMPARE(m_productDetailModel->unit(), QString());
    QCOMPARE(unitChangedSpy.count(), 0);
    QCOMPARE(m_productDetailModel->costPrice(), 0.0);
    QCOMPARE(costPriceChangedSpy.count(), 0);
    QCOMPARE(m_productDetailModel->retailPrice(), 0.0);
    QCOMPARE(retailPriceChangedSpy.count(), 0);
    QCOMPARE(
        m_productDetailModel->currency(),
        Settings::instance().locale().currencySymbol(QLocale::CurrencyIsoCode));
    QCOMPARE(currencyChangedSpy.count(), 0);
    QCOMPARE(m_productDetailModel->created(), QDateTime());
    QCOMPARE(createdChangedSpy.count(), 0);
    QCOMPARE(m_productDetailModel->lastEdited(), QDateTime());
    QCOMPARE(lastEditedChangedSpy.count(), 0);
    QCOMPARE(m_productDetailModel->userId(), 0);
    QCOMPARE(userIdChangedSpy.count(), 0);
    QCOMPARE(m_productDetailModel->user(), QString());
    QCOMPARE(userChangedSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 0);

    databaseWillReturn(product);

    // STEP: Set item ID so that record can be fetched.
    m_productDetailModel->setProductId(1);
    QCOMPARE(m_productDetailModel->productId(), 1);
    QCOMPARE(productIdChangedSpy.count(), 1);
    productIdChangedSpy.clear();
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);

    // STEP: Ensure values are properly set.
    QCOMPARE(m_productDetailModel->productId(), product["product_id"].toInt());
    QCOMPARE(m_productDetailModel->categoryId(),
             product["product_category_id"].toInt());
    QCOMPARE(productCategoryIdChangedSpy.count(), 1);
    QCOMPARE(m_productDetailModel->category(),
             product["product_category"].toString());
    QCOMPARE(productCategoryChangedSpy.count(), 1);
    QCOMPARE(m_productDetailModel->product(), product["product"].toString());
    QCOMPARE(productChangedSpy.count(), 1);
    QCOMPARE(m_productDetailModel->description(),
             product["description"].toString());
    QCOMPARE(descriptionChangedSpy.count(), 1);
    QCOMPARE(m_productDetailModel->isDivisible(),
             product["divisible"].toBool());
    QCOMPARE(divisibleChangedSpy.count(), 0);
    QCOMPARE(m_productDetailModel->quantity(), product["quantity"].toDouble());
    QCOMPARE(quantityChangedSpy.count(), 1);
    QCOMPARE(m_productDetailModel->unitId(),
             product["product_unit_id"].toInt());
    QCOMPARE(unitIdChangedSpy.count(), 1);
    QCOMPARE(m_productDetailModel->unit(), product["product_unit"].toString());
    QCOMPARE(unitChangedSpy.count(), 1);
    QCOMPARE(m_productDetailModel->costPrice(),
             product["cost_price"].toDouble());
    QCOMPARE(costPriceChangedSpy.count(), 1);
    QCOMPARE(m_productDetailModel->retailPrice(),
             product["retail_price"].toDouble());
    QCOMPARE(retailPriceChangedSpy.count(), 1);
    QCOMPARE(m_productDetailModel->currency(), product["currency"].toString());
    QCOMPARE(currencyChangedSpy.count(), 0);
    QCOMPARE(m_productDetailModel->created(), product["created"].toDateTime());
    QCOMPARE(createdChangedSpy.count(), 1);
    QCOMPARE(m_productDetailModel->lastEdited(),
             product["last_edited"].toDateTime());
    QCOMPARE(lastEditedChangedSpy.count(), 1);
    QCOMPARE(m_productDetailModel->userId(), product["user_id"].toInt());
    QCOMPARE(userIdChangedSpy.count(), 1);
    QCOMPARE(m_productDetailModel->user(), QStringLiteral("user"));
    QCOMPARE(userChangedSpy.count(), 1);
    QCOMPARE(busyChangedSpy.count(), 0);
}

QTEST_GUILESS_MAIN(QMLProductDetailModelTest)

#include "tst_qmlproductdetailmodeltest.moc"
