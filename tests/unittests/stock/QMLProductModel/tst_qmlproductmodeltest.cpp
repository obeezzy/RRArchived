#include "qmlapi/stock/qmlproductmodel.h"
#include "mockdatabasethread.h"
#include <QtTest>
#include <QCoreApplication>

class QMLProductModelTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();

    void testModel();
    void testViewProducts();
    void testRefresh();
    void testRemoveProduct();
    void testUndoRemoveProduct();
    void testFilterProduct();
private:
    QMLProductModel *m_productModel;
    MockDatabaseThread *m_thread;
};

void QMLProductModelTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_productModel = new QMLProductModel(*m_thread, this);
}

void QMLProductModelTest::cleanup()
{
    m_productModel->deleteLater();
    m_thread->deleteLater();
}

void QMLProductModelTest::testModel()
{
    QAbstractItemModelTester(m_productModel,
                             QAbstractItemModelTester::FailureReportingMode::Fatal,
                             this);
}

void QMLProductModelTest::testViewProducts()
{
    const QVariantMap product1 {
        { "category_id", 1 },
        { "category", "Category1" },
        { "product_id", 1 },
        { "product", "Product1" },
        { "description", "Description1" },
        { "quantity", 1.0 },
        { "unit_id", 1 },
        { "unit", "Unit1" },
        { "cost_price", 11.0 },
        { "retail_price", 10.0 },
        { "unit_price", 13.0 },
        { "available_quantity", 10.0 }
    };

    const QVariantMap product2 {
        { "category_id", 1 },
        { "category", "Category1" },
        { "product_id", 2 },
        { "product", "Product2" },
        { "description", "Description2" },
        { "quantity", 1.0 },
        { "unit_id", 2 },
        { "unit", "Unit2" },
        { "cost_price", 11.0 },
        { "retail_price", 10.0 },
        { "unit_price", 13.0 },
        { "available_quantity", 10.0 }
    };

    const QVariantList products {
        product1,
        product2
    };

    auto databaseWillReturn = [this](const QVariantList &products) {
        m_thread->result().setSuccessful(true);
        m_thread->result().setOutcome(QVariant());
        m_thread->result().setOutcome(QVariantMap {
                                { "products", products },
                                { "record_count", products.count() }
                            });
    };
    QSignalSpy successSpy(m_productModel, &QMLProductModel::success);
    QSignalSpy busyChangedSpy(m_productModel, &QMLProductModel::busyChanged);

    databaseWillReturn(products);

    QCOMPARE(m_productModel->rowCount(), 0);

    m_productModel->setCategoryId(1);
    QCOMPARE(busyChangedSpy.count(), 2);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<ModelResult>().code(), QMLProductModel::ViewProductsSuccess);
    successSpy.clear();

    QCOMPARE(m_productModel->rowCount(), 2);
    QCOMPARE(products.count(), 2);
    QCOMPARE(m_productModel->data(m_productModel->index(0, 0), QMLProductModel::CategoryRole).toString(),
             products.at(0).toMap()["product_category"].toString());
    QCOMPARE(m_productModel->data(m_productModel->index(0, 0), QMLProductModel::ProductRole).toString(),
             products.at(0).toMap()["product"].toString());
    QCOMPARE(m_productModel->data(m_productModel->index(0, 0), QMLProductModel::DescriptionRole).toString(),
             products.at(0).toMap()["description"].toString());
    QCOMPARE(m_productModel->data(m_productModel->index(0, 0), QMLProductModel::QuantityRole).toDouble(),
             products.at(0).toMap()["quantity"].toDouble());
    QCOMPARE(m_productModel->data(m_productModel->index(0, 0), QMLProductModel::UnitRole).toString(),
             products.at(0).toMap()["product_unit"].toString());
    QCOMPARE(m_productModel->data(m_productModel->index(0, 0), QMLProductModel::CostPriceRole).toDouble(),
             products.at(0).toMap()["cost_price"].toDouble());
    QCOMPARE(m_productModel->data(m_productModel->index(0, 0), QMLProductModel::RetailPriceRole).toDouble(),
             products.at(0).toMap()["retail_price"].toDouble());

    QCOMPARE(m_productModel->data(m_productModel->index(1, 0), QMLProductModel::CategoryRole).toString(),
             products.at(1).toMap()["product_category"].toString());
    QCOMPARE(m_productModel->data(m_productModel->index(1, 0), QMLProductModel::ProductRole).toString(),
             products.at(1).toMap()["product"].toString());
    QCOMPARE(m_productModel->data(m_productModel->index(1, 0), QMLProductModel::DescriptionRole).toString(),
             products.at(1).toMap()["description"].toString());
    QCOMPARE(m_productModel->data(m_productModel->index(1, 0), QMLProductModel::QuantityRole).toDouble(),
             products.at(1).toMap()["quantity"].toDouble());
    QCOMPARE(m_productModel->data(m_productModel->index(1, 0), QMLProductModel::UnitRole).toString(),
             products.at(1).toMap()["product_unit"].toString());
    QCOMPARE(m_productModel->data(m_productModel->index(1, 0), QMLProductModel::CostPriceRole).toDouble(),
             products.at(1).toMap()["cost_price"].toDouble());
    QCOMPARE(m_productModel->data(m_productModel->index(1, 0), QMLProductModel::RetailPriceRole).toDouble(),
             products.at(1).toMap()["retail_price"].toDouble());
}

void QMLProductModelTest::testRefresh()
{
    auto databaseWillReturnEmptyResult = [this]() {
        m_thread->result().setSuccessful(true);
        m_thread->result().setOutcome(QVariant());
    };
    auto databaseWillReturnSingleProduct = [this]() {
        m_thread->result().setSuccessful(true);
        m_thread->result().setOutcome(QVariant());

        const QVariantMap product {
            { "category_id", 1 },
            { "category", "Category1" },
            { "product_id", 1 },
            { "product", "Product1" },
            { "description", "Description1" },
            { "quantity", 1.0 },
            { "unit_id", 1 },
            { "unit", "Unit1" },
            { "cost_price", 11.0 },
            { "retail_price", 10.0 },
            { "unit_price", 13.0 },
            { "available_quantity", 10.0 }
        };

        const QVariantList products { product };

        m_thread->result().setOutcome(QVariantMap {
                                { "products", products },
                                { "record_count", products.count() }
                            });
    };

    databaseWillReturnEmptyResult();

    m_productModel->setCategoryId(1);
    QCOMPARE(m_productModel->rowCount(), 0);

    databaseWillReturnSingleProduct();

    m_productModel->refresh();
    QCOMPARE(m_productModel->rowCount(), 1);
}

void QMLProductModelTest::testRemoveProduct()
{
    auto databaseWillReturnEmptyResult = [this]() {
        m_thread->result().setSuccessful(true);
        m_thread->result().setOutcome(QVariant());
    };
    auto databaseWillReturnRemovedProduct = [this]() {
        m_thread->result().setSuccessful(true);
        m_thread->result().setOutcome(QVariant());

        m_thread->result().setOutcome(QVariantMap {
                                { "category_id", 1 },
                                { "product_id", 1 }
                            });
    };
    auto databaseWillReturnSingleProduct = [this]() {
        m_thread->result().setSuccessful(true);
        m_thread->result().setOutcome(QVariant());

        const QVariantMap product {
            { "category_id", 1 },
            { "category", "Category1" },
            { "product_id", 1 },
            { "product", "Product1" },
            { "description", "Description1" },
            { "quantity", 1.0 },
            { "unit_id", 1 },
            { "unit", "Unit1" },
            { "cost_price", 11.0 },
            { "retail_price", 10.0 },
            { "unit_price", 13.0 },
            { "available_quantity", 10.0 }
        };

        const QVariantList products { product };

        m_thread->result().setOutcome(QVariantMap {
                                { "products", products },
                                { "record_count", products.count() }
                            });
    };
    QSignalSpy successSpy(m_productModel, &QMLProductModel::success);
    QSignalSpy errorSpy(m_productModel, &QMLProductModel::error);

    databaseWillReturnSingleProduct();

    m_productModel->setCategoryId(1);
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_productModel->rowCount(), 1);

    databaseWillReturnRemovedProduct();

    m_productModel->removeProduct(0);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(m_productModel->rowCount(), 0);
    QCOMPARE(successSpy.takeFirst().first().value<ModelResult>().code(), QMLProductModel::RemoveProductSuccess);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);

    databaseWillReturnEmptyResult();

    m_productModel->refresh();
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_productModel->rowCount(), 0);
}

void QMLProductModelTest::testUndoRemoveProduct()
{
    auto databaseWillReturnRemovedProduct = [this]() {
        m_thread->result().setSuccessful(true);

        m_thread->result().setOutcome(QVariantMap {
                                { "category_id", 1 },
                                { "product_id", 1 }
                            });
    };
    auto databaseWillReturnSingleProduct = [this]() {
        m_thread->result().setSuccessful(true);

        const QVariantMap product {
            { "category_id", 1 },
            { "category", "Category1" },
            { "product_id", 1 },
            { "product", "Product1" },
            { "description", "Description1" },
            { "quantity", 1.0 },
            { "unit_id", 1 },
            { "unit", "Unit1" },
            { "cost_price", 11.0 },
            { "retail_price", 10.0 },
            { "unit_price", 13.0 },
            { "available_quantity", 10.0 }
        };

        const QVariantList products { product };

        m_thread->result().setOutcome(QVariantMap {
                                { "products", products },
                                { "record_count", products.count() }
                            });
    };
    QSignalSpy successSpy(m_productModel, &QMLProductModel::success);

    databaseWillReturnSingleProduct();

    m_productModel->setCategoryId(1);
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_productModel->rowCount(), 1);

    databaseWillReturnRemovedProduct();

    m_productModel->removeProduct(0);
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_productModel->rowCount(), 0);

    databaseWillReturnSingleProduct();

    m_productModel->undoLastCommit();
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_productModel->rowCount(), 0);

    databaseWillReturnSingleProduct();

    m_productModel->refresh();
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_productModel->rowCount(), 1);
}

void QMLProductModelTest::testFilterProduct()
{
    auto databaseWillReturnEmptyResult = [this]() {
        m_thread->result().setSuccessful(true);
        m_thread->result().setOutcome(QVariant());
    };
    auto databaseWillReturnSingleProduct = [this]() {
        m_thread->result().setSuccessful(true);
        m_thread->result().setOutcome(QVariant());

        const QVariantMap product {
            { "category_id", 1 },
            { "category", "Category1" },
            { "product_id", 1 },
            { "product", "Product1" },
            { "description", "Description1" },
            { "quantity", 1.0 },
            { "unit_id", 1 },
            { "unit", "Unit1" },
            { "cost_price", 11.0 },
            { "retail_price", 10.0 },
            { "unit_price", 13.0 },
            { "available_quantity", 10.0 }
        };

        const QVariantList products { product };

        m_thread->result().setOutcome(QVariantMap {
                                { "products", products },
                                { "record_count", products.count() }
                            });
    };
    QSignalSpy successSpy(m_productModel, &QMLProductModel::success);

    databaseWillReturnSingleProduct();

    m_productModel->setCategoryId(1);
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_productModel->rowCount(), 1);

    databaseWillReturnSingleProduct();

    m_productModel->setFilterColumn(QMLProductModel::ProductColumn);
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    m_productModel->setFilterText("Product1");
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_productModel->rowCount(), 1);

    databaseWillReturnSingleProduct();

    m_productModel->setFilterText("Product");
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_productModel->rowCount(), 1);

    databaseWillReturnEmptyResult();

    m_productModel->setFilterText("Product2");
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_productModel->rowCount(), 0);
}

QTEST_GUILESS_MAIN(QMLProductModelTest)

#include "tst_qmlproductmodeltest.moc"
