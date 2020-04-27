#include "qmlapi/qmlstockproductmodel.h"
#include "mockdatabasethread.h"
#include <QtTest>
#include <QCoreApplication>

class QMLStockProductModelTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();

    void testModel();
    void testViewStockProducts();
    void testRefresh();
    void testRemoveProduct();
    void testUndoRemoveProduct();
    void testFilterProduct();
private:
    QMLStockProductModel *m_stockProductModel;
    MockDatabaseThread *m_thread;
};

void QMLStockProductModelTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_stockProductModel = new QMLStockProductModel(*m_thread, this);
}

void QMLStockProductModelTest::cleanup()
{
    m_stockProductModel->deleteLater();
    m_thread->deleteLater();
}

void QMLStockProductModelTest::testModel()
{
    QAbstractItemModelTester(m_stockProductModel,
                             QAbstractItemModelTester::FailureReportingMode::Fatal,
                             this);
}

void QMLStockProductModelTest::testViewStockProducts()
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
    QSignalSpy successSpy(m_stockProductModel, &QMLStockProductModel::success);
    QSignalSpy busyChangedSpy(m_stockProductModel, &QMLStockProductModel::busyChanged);

    databaseWillReturn(products);

    QCOMPARE(m_stockProductModel->rowCount(), 0);

    m_stockProductModel->setCategoryId(1);
    QCOMPARE(busyChangedSpy.count(), 2);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<ModelResult>().code(), QMLStockProductModel::ViewProductsSuccess);
    successSpy.clear();

    QCOMPARE(m_stockProductModel->rowCount(), 2);
    QCOMPARE(products.count(), 2);
    QCOMPARE(m_stockProductModel->data(m_stockProductModel->index(0, 0), QMLStockProductModel::CategoryRole).toString(),
             products.at(0).toMap()["product_category"].toString());
    QCOMPARE(m_stockProductModel->data(m_stockProductModel->index(0, 0), QMLStockProductModel::ProductRole).toString(),
             products.at(0).toMap()["product"].toString());
    QCOMPARE(m_stockProductModel->data(m_stockProductModel->index(0, 0), QMLStockProductModel::DescriptionRole).toString(),
             products.at(0).toMap()["description"].toString());
    QCOMPARE(m_stockProductModel->data(m_stockProductModel->index(0, 0), QMLStockProductModel::QuantityRole).toDouble(),
             products.at(0).toMap()["quantity"].toDouble());
    QCOMPARE(m_stockProductModel->data(m_stockProductModel->index(0, 0), QMLStockProductModel::UnitRole).toString(),
             products.at(0).toMap()["product_unit"].toString());
    QCOMPARE(m_stockProductModel->data(m_stockProductModel->index(0, 0), QMLStockProductModel::CostPriceRole).toDouble(),
             products.at(0).toMap()["cost_price"].toDouble());
    QCOMPARE(m_stockProductModel->data(m_stockProductModel->index(0, 0), QMLStockProductModel::RetailPriceRole).toDouble(),
             products.at(0).toMap()["retail_price"].toDouble());

    QCOMPARE(m_stockProductModel->data(m_stockProductModel->index(1, 0), QMLStockProductModel::CategoryRole).toString(),
             products.at(1).toMap()["product_category"].toString());
    QCOMPARE(m_stockProductModel->data(m_stockProductModel->index(1, 0), QMLStockProductModel::ProductRole).toString(),
             products.at(1).toMap()["product"].toString());
    QCOMPARE(m_stockProductModel->data(m_stockProductModel->index(1, 0), QMLStockProductModel::DescriptionRole).toString(),
             products.at(1).toMap()["description"].toString());
    QCOMPARE(m_stockProductModel->data(m_stockProductModel->index(1, 0), QMLStockProductModel::QuantityRole).toDouble(),
             products.at(1).toMap()["quantity"].toDouble());
    QCOMPARE(m_stockProductModel->data(m_stockProductModel->index(1, 0), QMLStockProductModel::UnitRole).toString(),
             products.at(1).toMap()["product_unit"].toString());
    QCOMPARE(m_stockProductModel->data(m_stockProductModel->index(1, 0), QMLStockProductModel::CostPriceRole).toDouble(),
             products.at(1).toMap()["cost_price"].toDouble());
    QCOMPARE(m_stockProductModel->data(m_stockProductModel->index(1, 0), QMLStockProductModel::RetailPriceRole).toDouble(),
             products.at(1).toMap()["retail_price"].toDouble());
}

void QMLStockProductModelTest::testRefresh()
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

    m_stockProductModel->setCategoryId(1);
    QCOMPARE(m_stockProductModel->rowCount(), 0);

    databaseWillReturnSingleProduct();

    m_stockProductModel->refresh();
    QCOMPARE(m_stockProductModel->rowCount(), 1);
}

void QMLStockProductModelTest::testRemoveProduct()
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
    QSignalSpy successSpy(m_stockProductModel, &QMLStockProductModel::success);
    QSignalSpy errorSpy(m_stockProductModel, &QMLStockProductModel::error);

    databaseWillReturnSingleProduct();

    m_stockProductModel->setCategoryId(1);
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_stockProductModel->rowCount(), 1);

    databaseWillReturnRemovedProduct();

    m_stockProductModel->removeProduct(0);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(m_stockProductModel->rowCount(), 0);
    QCOMPARE(successSpy.takeFirst().first().value<ModelResult>().code(), QMLStockProductModel::RemoveProductSuccess);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);

    databaseWillReturnEmptyResult();

    m_stockProductModel->refresh();
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_stockProductModel->rowCount(), 0);
}

void QMLStockProductModelTest::testUndoRemoveProduct()
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
    QSignalSpy successSpy(m_stockProductModel, &QMLStockProductModel::success);

    databaseWillReturnSingleProduct();

    m_stockProductModel->setCategoryId(1);
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockProductModel->rowCount(), 1);

    databaseWillReturnRemovedProduct();

    m_stockProductModel->removeProduct(0);
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockProductModel->rowCount(), 0);

    databaseWillReturnSingleProduct();

    m_stockProductModel->undoLastCommit();
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockProductModel->rowCount(), 0);

    databaseWillReturnSingleProduct();

    m_stockProductModel->refresh();
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockProductModel->rowCount(), 1);
}

void QMLStockProductModelTest::testFilterProduct()
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
    QSignalSpy successSpy(m_stockProductModel, &QMLStockProductModel::success);

    databaseWillReturnSingleProduct();

    m_stockProductModel->setCategoryId(1);
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockProductModel->rowCount(), 1);

    databaseWillReturnSingleProduct();

    m_stockProductModel->setFilterColumn(QMLStockProductModel::ProductColumn);
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    m_stockProductModel->setFilterText("Product1");
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockProductModel->rowCount(), 1);

    databaseWillReturnSingleProduct();

    m_stockProductModel->setFilterText("Product");
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockProductModel->rowCount(), 1);

    databaseWillReturnEmptyResult();

    m_stockProductModel->setFilterText("Product2");
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockProductModel->rowCount(), 0);
}

QTEST_GUILESS_MAIN(QMLStockProductModelTest)

#include "tst_qmlstockproductmodeltest.moc"
